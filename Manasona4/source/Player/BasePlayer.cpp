#include "TDNLIB.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"

//_/_/_/_/_/_/__/_/__/_/__/_/__
// 定数
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// 移動が終わって待機に戻る移動値ライン
const float BasePlayer::c_FRONT_BRAKE_LINE = .55f;	// 走り中に操作を離してブレーキに移行するまでの移動値ライン
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 2.2f;

static const float DIR_ANGLE[(int)DIR::MAX] =
{
	PI * 1.5f,	// 左向き
	PI * .5,	// 右向き
};

// 攻撃フレーム情報読み込み
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
			}
		}
		// 終端
		m_ActionFrameList[i][count] = FRAME_STATE::END;
	}
}

BasePlayer::BasePlayer(int deviceID) :BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO), m_bLand(false), m_bAerialJump(true), m_ActionState(BASE_ACTION_STATE::NO_ACTION),
m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentActionFrame(0), m_RecoveryFlame(0), m_bEscape(false)
{
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

	// IDで座標の分岐
	switch (deviceID)
	{
	case 0:
		m_pos.Set(-50, 20, 0);
		break;

	case 1:
		m_pos.Set(-25, 20, 0);
		break;

	case 2:
		m_pos.Set(25, 20, 0);
		break;

	case 3:
		m_pos.Set(50, 20, 0);
		break;
	}

	m_angleY = DIR_ANGLE[(int)m_dir];
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_PanelEffectMGR);
	SAFE_DELETE(m_UVEffectMGR);
	
	delete m_pHitSquare;
}

void BasePlayer::Update()
{
	// エフェクトマネージャー更新 (ヒットストップ無視)
	m_PanelEffectMGR->Update();
	m_UVEffectMGR->Update();

	// ★硬直時間のデクリメント
	if (m_RecoveryFlame > 0)m_RecoveryFlame--;

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


					LPCSTR SE_ID = m_ActionDatas[(int)m_ActionState].pAttackData->WhiffSE;
					// 空振りSE入ってたら
					if (SE_ID)
					{
						// ディレイフレーム経過したら再生
						se->Play((LPSTR)SE_ID);
					}
				}
			}

			// フレーム最後まで再生したら
			if (m_ActionFrameList[m_ActionState][m_CurrentActionFrame++] == FRAME_STATE::END)
			{
				// ★アクションステート解除
				m_ActionState = BASE_ACTION_STATE::NO_ACTION;
			}
		}

		// 入力受付
		Control();

		// 入力受付後にステートマシン更新
		m_pStateMachine->Update();

		//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
		//else m_move.x = 0;

		// 動きの制御
		Move();

		// メッシュの更新
		m_pObj->Animation();
		m_pObj->SetAngle(m_angleY);	// 左右のアングルのセット
		m_pObj->SetPos(m_pos);
		m_pObj->Update();
	}
	// アングル補間処理
	m_angleY = m_angleY*.65f + DIR_ANGLE[(int)m_dir] * .35f;
}

void BasePlayer::Move() 
{
	// 移動量更新
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

void BasePlayer::Control()
{
	// 押したキー判定
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_InputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_deviceID);
	}

}

void BasePlayer::UpdatePos()
{
	// (TODO)ヒットストップしてなかったら
	if (m_HitStopFrame <= 0)
	{
		// 座標更新
		m_pos += m_move;
	}
}

void BasePlayer::Render()
{
	m_pObj->Render();
	
	m_pStateMachine->Render();// ステートマシンでの描画

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

	switch (effectType)
	{
	case EFFECT_TYPE::DAMAGE:
		//m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURN);
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE);
		break;
	default:
		assert(0);	// そんなエフェクトは存在しない AddEffectAction()
		break;
	}

}
