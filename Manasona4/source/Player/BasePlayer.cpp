#include "TDNLIB.h"
#include "BasePlayer.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "AI\AI.h"
#include "../Effect/Particle.h"
#include "../system/System.h"
#include "../PointLight/PointLight.h"

//_/_/_/_/_/_/__/_/__/_/__/_/__
// 定数
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// 移動が終わって待機に戻る移動値ライン
const float BasePlayer::c_FRONT_BRAKE_LINE = .55f;	// 走り中に操作を離してブレーキに移行するまでの移動値ライン
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 2.2f;

const int BasePlayer::c_RECOVERY_FLAME = 8;			// リカバリーステートにいる時間

const int BasePlayer::c_OVERDRIVE_MAX_GAGE = 100;	// 覚醒ゲージの最大値
const int BasePlayer::c_OVERDRIVE_MAX_TIME = 360;	// 覚醒が切れるまでの時間

const int BasePlayer::c_THROW_ESCAPE_FRAME = 8;	// 投げぬけの猶予フレーム
const int BasePlayer::c_THROW_MISS_FRAME = 30;	// 投げ外しロスのフレーム(全キャラ共通だろうという考え)
const int BasePlayer::c_THROW_RELEASE_FRAME = 15;	// 投げ抜けで、パシンてなってる間のフレーム(これも全キャラ共通だろう)
// アクションフレーム情報読み込み
void BasePlayer::LoadAttackFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "アタックフレームのテキスト入ってない");

	char skip[64];	// 読み飛ばし用変数

	FOR((int)BASE_ACTION_STATE::END)
	{
		ifs >> skip;

		// 始動、持続、硬直フレームを読み込み
		int count(0);

		for (int j = 0; j < (int)FRAME_STATE::END; j++)
		{
			int frame;
			ifs >> frame;
			for (int k = 0; k < frame; k++)
			{
				m_ActionFrameList[i][count++] = (FRAME_STATE)j;
				MyAssert(count < 256, "アクションフレームの合計は255以下にしてください");
			}
		}
		// 終端
		m_ActionFrameList[i][count] = FRAME_STATE::END;
	}
}

BasePlayer::BasePlayer(int deviceID, TEAM team, bool bAI) :m_bAI(bAI), m_team(team), BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO), m_bLand(false), m_bAerialJump(true), m_ActionState(BASE_ACTION_STATE::NO_ACTION),
m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentActionFrame(0), m_RecoveryFlame(0), m_bEscape(false), m_score(0), m_CollectScore(0), m_pAI(nullptr),
m_recoveryCount(0), m_HitStopFrame(0),// ★★★バグの原因　ひっとすトップ初期化のし忘れ。
m_InvincibleColRate(0), m_InvincibleColRateFlame(0), m_bInvincibleColRateUpFlag(false),
m_OverDriveGage(0), m_bOverDrive(false), m_OverDriveFrame(0), m_OverDriveType(OVERDRIVE_TYPE::BURST),
m_bMoveUpdate(true), m_bThrowSuccess(false),
m_bGuard(false), m_GuardFollowFrame(0),
m_ThrowPlayerID(ENTITY_ID::ID_ERROR),
m_MaxHP(0), m_HP(0),
m_bGameTimerStopFlag(false), m_HeavehoStopTimer(0)
{
	// とりあえず、モコイさん
	m_pStand = new Stand::Mokoi(this);


	// デフォルト設定
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// エフェクトマネージャー
	m_PanelEffectMGR = new PanelEffectManager();
	m_UVEffectMGR	 = new UVEffectManager();

	// 初期化
	memset(m_InputList, 0, sizeof(m_InputList));
	ZeroMemory(&m_jump, sizeof(Jump));
	ZeroMemory(&m_RushAttack, sizeof(RushAttack));
	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<BasePlayer>(this);
	m_pStateMachine->SetGlobalState(BasePlayerState::Global::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance());
	m_pStateMachine->SetPreviousState(BasePlayerState::Wait::GetInstance());

	// AIかどうかで　分岐
	//if(bAI) m_pAI = new AI(m_deviceID, this);
	m_pAI = nullptr;

	// IDで座標の分岐
	switch (deviceID)
	{
	case 0:
		m_pos.Set(-50, 20, 0);
		break;

	case 1:
		m_pos.Set(-25, 30, 0);
		break;

	case 2:
		m_pos.Set(25, 40, 0);
		break;

	case 3:
		m_pos.Set(50, 50, 0);
		break;
	}

	m_angleY = DIR_ANGLE[(int)m_dir];

	// 喰らってる中に喰らってるかうんと初期化
	m_RecoveryDamageCount.clear();


	m_pComboUI = new ComboUI(&m_RecoveryFlame);

}

void BasePlayer::InitAI()
{
	// AIフラグかつ、AIがnewされてなかったら
	//if (m_pAI) return;
	if (m_bAI) m_pAI = new AI(m_deviceID, this);
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_pStand);
	SAFE_DELETE(m_PanelEffectMGR);
	SAFE_DELETE(m_UVEffectMGR);
	SAFE_DELETE(m_pAI);
	SAFE_DELETE(m_pComboUI);

	delete m_pHitSquare;
}

void BasePlayer::Update(bool bControl)
{
	// 覚醒の更新
	OverDriveUpdate();

	// ガードエフェクト更新
	GuardEffectUpdate();

	// 1more覚醒していたらスタンドの動きを止める
	if (GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == false)
	{
		// スタンド更新
		m_pStand->Update();
	}

	// 無敵時間の更新
	if (KeyBoardTRG(KB_H))
	{
		SetInvincible(240, 1);
	}
	InvincibleUpdate();


	// ★硬直時間のデクリメント
	if (m_RecoveryFlame > 0)
		m_RecoveryFlame--;

	// ★ヒットストップ中
	if (m_HitStopFrame > 0)
	{
		m_HitStopFrame--;
	}
	// ヒットストップが言わるまで動かない
	else
	{
		// アクションフレームの更新
		if (isFrameAction())
		{
			// 攻撃フレームなら
			if (m_ActionDatas[(int)m_ActionState].isAttackData())
			{
				// 今の経過時間とディレイフレームになったら
				if (m_ActionDatas[(int)m_ActionState].pAttackData->WhiffDelayFrame == m_CurrentActionFrame)
				{
					// (A列車)ここで攻撃判定が発動した瞬間を取ってきてる
					
					// 振りエフェクト発動（仮）！
					AddEffectAction(m_pos + Vector3(0, 5, -3) , m_ActionDatas[(int)m_ActionState].pAttackData->WhiffEffectType);

					LPCSTR SE_ID = m_ActionDatas[(int)m_ActionState].pAttackData->WhiffSE;
					// 空振りSE入ってたら
					if (SE_ID)
					{
						// ディレイフレーム経過したら再生
						se->Play((LPSTR)SE_ID);
					}
				}


				//// もし始動・持続の間にその攻撃が「対空」効果を持っていた場合
				//// 対空フラグをONにするそれ以外はOFFに
				//if ((m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE ||
				//	m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::START)
				//	&& m_ActionDatas[(int)m_ActionState].pAttackData->bAntiAir == true	)
				//{
				//	m_move.y += 0.35f;
				//}
				//else
				//{

				//}

			}
			//else
			//{
			//	// ようわからんのでこっちでも一応対空OFFにしとこ

			//}

			
			// ActionFlameがフレームや
			//if (m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE || 
			//	m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::START)
			//{
			//	// 攻撃フレームなら(これがなかったら攻撃のデータがないものも見てしまう)
			//	if (m_ActionDatas[(int)m_ActionState].isAttackData())
			//	{
			//		// 今どの業のステートかわかる
			//		if (m_ActionDatas[(int)m_ActionState].pAttackData->bAntiAir == true)
			//		{
			//			m_move.y += 0.35f;
			//		}
			//	}
			//	
			//}

			// モーションのフレームを更新！
			m_CurrentActionFrame++;

			// フレーム最後まで再生したら
			if (m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame] == FRAME_STATE::END)
			{
				// ★アクションステート解除
				m_ActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}

		// 入力受付
		if (bControl)
		{
			if (m_bAI)	AIControl();
			else		Control();
		}

		// 入力受付後にステートマシン更新
		m_pStateMachine->Update();

		//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
		//else m_move.x = 0;

		// ★★★　なぞのバグは　ここをとおってないからおこる

		// 動きの制御
		Move();

		// メッシュの更新
		m_pObj->Animation();
		m_pObj->SetAngle(m_angleY);	// 左右のアングルのセット
		m_pObj->SetPos(m_pos);
		m_pObj->Update();
	}

	// アングル補間処理
	const float AnglePercentage = (m_pStateMachine->isInState(*BasePlayerState::Escape::GetInstance())) ? .9f : .65f;
	m_angleY = m_angleY * AnglePercentage + DIR_ANGLE[(int)m_dir] * (1 - AnglePercentage);

	// エフェクトマネージャー更新 (ヒットストップ無視)
	m_PanelEffectMGR->Update();
	m_UVEffectMGR->Update();
	
	// コンボUI
	m_pComboUI->Update();

}

void BasePlayer::Move() 
{
	// 移動量更新
	if (
		//m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false
		m_bMoveUpdate
		)
	{	// ペルソナ発動中じゃなかったら移動
		m_move.y -= c_GRAVITY;
		//if (m_move.y <= -3.0f) { m_move.y = -3.0f; } // 落ちる速度を抑制
		m_move.y = max(-2.75f, m_move.y);// 落ちる速度を抑制

		// 空気抵抗(xの値を徐々に減らしていく)
		if (m_bLand)
		{
			//if (m_move.x > 0)		m_move.x = max(m_move.x - .055f, 0);
			//else if (m_move.x < 0)	m_move.x = min(m_move.x + .055f, 0);
			m_move.x *= 0.92f;	// 減速(A列車:この値はキャラ固有の値)
		}
		else
		{
			//if (m_move.x > 0)		m_move.x = max(m_move.x - .035f, 0);
			//else if (m_move.x < 0)	m_move.x = min(m_move.x + .035f, 0);
			m_move.x *= 0.98f;	// 減速(A列車:この値はキャラ固有の値)
		}

		// 左右のMove値
		//m_move.x = Math::Clamp(m_move.x, -m_maxSpeed, m_maxSpeed);
	}

	// ペルソナ発動中移動
	else
	{
		m_move *= 0.8f;	// 減速(A列車:この値はキャラ固有の値)
	}

}

// 無敵の制御
void BasePlayer::InvincibleUpdate()
{
	// ここだけのお名前
	enum 
	{
		FLASH_SPEED = 15
	};

	// 無敵時間の更新
	if (m_InvincibleTime > 0)
	{
		//m_InvincibleLV = (--m_InvincibleTime <= 0) ? 0 : m_InvincibleLV;
		m_InvincibleTime--;

		
		m_InvincibleColRateFlame++;
		m_InvincibleColRateFlame = m_InvincibleColRateFlame % FLASH_SPEED;
		
		// 上げ下げのフラグ切り替え
		if (m_InvincibleColRateFlame == 0)
		{
			m_bInvincibleColRateUpFlag = (m_bInvincibleColRateUpFlag == false) ? true : false;
		}

		// 上げフラグがONならだんだん白く
		if (m_bInvincibleColRateUpFlag == false)
		{
			m_InvincibleColRate = ((float)(m_InvincibleColRateFlame) / FLASH_SPEED);
		}
		else
		{
			m_InvincibleColRate = 1.0f - ((float)(m_InvincibleColRateFlame) / FLASH_SPEED);
		}
	

		// ★　色の調整 (ここはセンスの問題)
		m_InvincibleColRate *= 0.25f;	// ちょっと薄く
		m_InvincibleColRate += 0.1f;    // けど点滅がない状態でも白っぽい

	}
	else
	{
		// タイムが0以下に達したらおしまい　初期化
		m_InvincibleLV = 0;
		m_InvincibleTime = 0;

		m_InvincibleColRate = 0.0f;
		m_InvincibleColRateFlame = 0;
		m_bInvincibleColRateUpFlag = false;
		
	}

}

// 覚醒の制御
void BasePlayer::OverDriveUpdate()
{
	// 覚醒しているか
	if (m_bOverDrive == false)
	{
		m_OverDriveGage++;
		// ゲージが最大まで溜まったら
		if (m_OverDriveGage >= c_OVERDRIVE_MAX_GAGE)
		{
			m_OverDriveGage = c_OVERDRIVE_MAX_GAGE;
		}

	}
	else // 覚醒中
	{
		// 覚醒の種類に応じて
		switch (m_OverDriveType)
		{
		case OVERDRIVE_TYPE::ONEMORE:

			// オーラのパーティクル
			ParticleManager::EffectOverDrive(Vector3(GetPos().x, GetPos().y, -0.5f));	// 若干手前

			break;
		case OVERDRIVE_TYPE::BURST:

			break;
		default:
			break;
		}

		m_OverDriveFrame--;
		if (m_OverDriveFrame <= 0) // 覚醒時間が0になったら覚醒終わり
		{
			m_OverDriveFrame = 0;
			m_bOverDrive = false;
		}
	}

}

void BasePlayer::ActionOverDrive(OVERDRIVE_TYPE type)
{
	// 覚醒の種類
	m_OverDriveType = type;

	// アクション
	m_bOverDrive = true;
	
	m_OverDriveGage = 0; //空に
	m_OverDriveFrame = c_OVERDRIVE_MAX_TIME;

	// ジャンプの権利も復活
	m_bAerialJump = true;
}

void BasePlayer::Control()
{
	// 押したキー判定
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_InputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_deviceID);
	}

}

void BasePlayer::AIControl()
{
	//static int frame(0);
	//if (++frame > 90)
	//{
	//	frame = 0;
	//	m_InputList[(int)PLAYER_INPUT::C] = 3;
	//	//m_InputList[(int)PLAYER_INPUT::LEFT] = 3;
	//}

	////static int frame2(0);
	////if (++frame2 > 65)
	////{
	////	frame2 = 0;
	////	m_InputList[(int)PLAYER_INPUT::B] = 3;
	////	m_InputList[(int)PLAYER_INPUT::DOWN] = 3;
	////}
	//m_InputList[(int)PLAYER_INPUT::LEFT] = 1;	// Aおしっぱ


	// AIのステートマシンを更新
	m_pAI->Update();

}

void BasePlayer::UpdatePos()
{
	// (TODO)ヒットストップしてなかったら
	if (m_HitStopFrame <= 0)
	{
		//if (m_pStateMachine->isInState(*BasePlayerState::StandAction::GetInstance()) == false)// ペルソナ発動中じゃなかったら移動
		{
			// 座標更新
			m_pos += m_move;
		}
	}
}

void BasePlayer::Render()
{
	// スタンド描画
	m_pStand->Render(shaderM, "Persona");

	// 無敵の白色チカチカレートを送る
	shaderM->SetValue("g_InvincibleColRate", m_InvincibleColRate);
	m_pObj->Render(shaderM, "PlayerToon");

	// 無敵なら加算で重ねて描画
	if (m_InvincibleTime > 0) m_pObj->Render(RS::ADD);

	// ここで現在のステートマシンの状態を確認
	if (m_deviceID == 0)
	{
		m_pStateMachine->Render();// ステートマシンでの描画
		
		if (m_pAI != nullptr)
		{
			 m_pAI->GetFSM()->Render();// AIステートマシンでの描画
		}
	}


	// エフェクトマネージャー描画
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();

#ifdef _DEBUG
	// 判定の描画
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), m_pHitSquare, sizeof(CollisionShape::Square));
	square.pos += m_pos;

	Vector3 wv[3];	// ワールドバーテックス
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// スクリーンバーテックス
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* 攻撃判定の描画 */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			square.pos += m_pos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}

#endif

	// デバッグ
	tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "スタンドゲージ : %d", m_pStand->GetStandGage());
	tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "スタンドストック: %d", m_pStand->GetStandStock());
	
	tdnText::Draw(32 + m_deviceID * 250, 430, 0xff00ff80, "OD ゲージ: %d", m_OverDriveGage);
	tdnText::Draw(32 + m_deviceID * 250, 460, 0xffff8000, "OD残り時間: %d", m_OverDriveFrame);


	Vector2 pos2d = Math::WorldToScreen(m_pos);
	
	//if (m_deviceID == 1 || m_deviceID == 2)
	{
		tdnText::Draw((int)pos2d.x, (int)pos2d.y - 150, 0xff00ff80, "C_Score : %d", m_CollectScore);
		tdnText::Draw((int)pos2d.x, (int)pos2d.y-100, 0xffff8000, "%dP!!", m_deviceID);

	}


}

void BasePlayer::Render(tdnShader* shader, char* name)
{
	// スタンド描画
	m_pStand->Render(shader, name);

	// 無敵の白色を送る
	shader->SetValue("g_InvincibleColRate", m_InvincibleColRate);
	// デバッグ
	tdnText::Draw(332 + m_deviceID * 350, 260, 0xff00ff80, "m_InvColRate : %.2f", m_InvincibleColRate);

	m_pObj->Render(shader, name);
	// 無敵なら加算で重ねて描画
	if (m_InvincibleTime > 0) m_pObj->Render(RS::ADD);

	if (m_deviceID == 3)
	{
		m_pStateMachine->Render();// ステートマシンでの描画
		m_pAI->GetFSM()->Render();// AIステートマシンでの描画
	}
	

	// エフェクトマネージャー描画
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();

#ifdef _DEBUG
	// 判定の描画
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), m_pHitSquare, sizeof(CollisionShape::Square));
	square.pos += m_pos;

	Vector3 wv[3];	// ワールドバーテックス
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// スクリーンバーテックス
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* 攻撃判定の描画 */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			square.pos += m_pos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}

#endif

	// デバッグ
	tdnText::Draw(32 + m_deviceID * 250, 560, 0xff00ff80, "CollectScore : %d", m_CollectScore);
	tdnText::Draw(32 + m_deviceID * 250, 600, 0xffff8000, "Score : %d", m_score);

	tdnText::Draw(32 + m_deviceID * 250, 630, 0xff00ff80, "スタンドゲージ : %d", m_pStand->GetStandGage());
	tdnText::Draw(32 + m_deviceID * 250, 660, 0xffff8000, "スタンドストック: %d", m_pStand->GetStandStock());

}

void BasePlayer::RenderDeferred()
{
	// スタンド描画
	m_pStand->Render(shaderM, "G_Buffer");

	m_pObj->Render(shaderM,"G_Buffer");
}

void BasePlayer::RenderUI()
{
	// コンボUI
	m_pComboUI->Render(100 + (m_deviceID * 950), 200);

}

// ステートマシンへの他から来るメッセージ
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

// ★エフェクト発動★　（全てのエフェクトの発動場所）
void BasePlayer::AddEffectAction(Vector3 pos, EFFECT_TYPE effectType)
{
	// 何のエフェクトを重ねるかはここで俺が決める。

    // キャラクター向きによる向き
    float diaAngle = (m_dir == DIR::RIGHT) ? 0.0f : 3.14f;

	// 集まるエフェクトの場所補正
	Vector3 convAddPos = Vector3(0, -10, 0);

	switch (effectType)
	{
	case EFFECT_TYPE::DAMAGE:
	{
		//m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURN);
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE);
		PointLightMgr->AddPointLight(pos + Vector3(0, 5, 0), Vector3(1.0f, 0.4f, 0.0f), 20, 4, 20, 4, 15);// ポイントライトエフェクト！
		Vector3 FlyVector(m_move);
		FlyVector.Normalize();
		ParticleManager::EffectHit(pos, FlyVector);
	}
		break;
	case EFFECT_TYPE::WHIFF:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::INEFFECT_MINI);
		
        m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::HIT_IMPACT,
            1, 2, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));

	
		break;
	case EFFECT_TYPE::RECOVERY:
		m_PanelEffectMGR->AddEffect(pos + Vector3(0, 5, 0), PANEL_EFFECT_TYPE::ClEAR);


		break;
	case EFFECT_TYPE::PERSONA:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::PERSONA,
			1, 2.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectPersonaTrigger(pos);
		PointLightMgr->AddPointLight(pos + Vector3(0, 3, 0) , Vector3(0, .4f, 2.0f), 50, 4, 40, 10, 30);// ポイントライトエフェクト！

		break;
	case EFFECT_TYPE::DROP_IMPACT:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::IMPACT,
			1.0f, 1.35f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::SHOCK_WAVE,
			1.0f, 1.55f, Vector3(0, 0, 0), Vector3(0, 0, 0));

		break;
	case EFFECT_TYPE::UPPER:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::UPPER,
			1.0f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));

		break;

	case EFFECT_TYPE::FINISH_HIT:
		ParticleManager::EffectFinish(pos);

		break;
	case EFFECT_TYPE::BURST:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::ONEMORE_BURST);

		break;
	case EFFECT_TYPE::ONEMORE_BURST_START:
	
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURST_PREV);

	//m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::BURST_BALL, 0.9, 0.75
	//		, Vector3(0, 0, 0), Vector3(0, 0, 0));

	 m_UVEffectMGR->AddEffect(pos+convAddPos, UV_EFFECT_TYPE::CONV, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0));
	 m_UVEffectMGR->AddEffect(pos + convAddPos+Vector3(0,-5,0), UV_EFFECT_TYPE::CONV2, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0),4);
	 m_UVEffectMGR->AddEffect(pos + convAddPos + Vector3(-0.5, -7, 0), UV_EFFECT_TYPE::CONV3, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0),8);
	 m_UVEffectMGR->AddEffect(pos + convAddPos, UV_EFFECT_TYPE::CONV4, 1.5f, 1.5f
		 , Vector3(0, 0, 0), Vector3(0, 0, 0),12);
		break;
	case EFFECT_TYPE::RUN:
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::RUN,
			0.8f, 1.0f, Vector3(0, diaAngle, 0), Vector3(0, diaAngle, 0));
		ParticleManager::EffectRunSmoke(m_pos, (m_dir != DIR::LEFT));

		break;
	case EFFECT_TYPE::GUARD_BREAK:
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::GLASS);

		break;
	default:
		assert(0);	// そんなエフェクトは存在しない AddEffectAction()
		break;
	}

}

// ガードEffect
void BasePlayer::GuardEffectAction()
{
	// ガードエフェクト発動
	m_UVEffectMGR->AddEffectRoop(GetCenterPos(), UV_EFFECT_TYPE::GUARD);

	m_PanelEffectMGR->AddEffect(GetCenterPos()+Vector3(0,0,-5), PANEL_EFFECT_TYPE::GUARD);

}

void BasePlayer::GuardEffectStop()
{
	// ガードエフェクト終了
	m_UVEffectMGR->StopEffectRoop(UV_EFFECT_TYPE::GUARD);
}

void BasePlayer::GuardEffectUpdate()
{
	// キャラクターに追従
	// ガードエフェクト更新
	m_UVEffectMGR->GetBaseUVEffect(UV_EFFECT_TYPE::GUARD)->SetPos(GetCenterPos());
}