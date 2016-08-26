#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"

// 実体の宣言
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// エンティティID登録
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr)
{
	m_PointAteam = 0;
	m_PointBteam = 0;

	m_OverDriveDim = 255;
}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage)
{
	m_NumPlayer = NumPlayer;
	m_pStage = pStage;

	// プレイヤ確保
	m_pPlayers = new BasePlayer*[NumPlayer];

	FOR(NumPlayer)
	{
		// チームを今は仮で振り分け!!
		TEAM team;
		if (i % 2 == 0)
		{
			team = TEAM::A;
		}
		else
		{
			team = TEAM::B;
		}

		m_pPlayers[i] = new Airou(i, team, (/*i == 3|| i == 2 || i == 0 ||*/ i == 1));
		m_pPlayers[i]->InitActionDatas();		// ★攻撃情報を各キャラに初期化させる
	}


	FOR(NumPlayer)
	{
		// AI初期化 なぜここで初期化するというと、初期化の中でプレイヤーを参照することがあり、全員の初期化が完了した後に呼び出したいから
		m_pPlayers[i]->InitAI();
	}
}

PlayerManager::~PlayerManager()
{
	// ダブルポインターの開放
	FOR(m_NumPlayer)delete m_pPlayers[i];
	delete[] m_pPlayers;
}

void PlayerManager::Update()
{	
	// 誰かが1More覚醒していたら全員の動きを止める
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == true)
		{
			m_pPlayers[i]->Update();			// モーションとか移動値の作成とか、基本的な更新。
			
			m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
			m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新

			// ●暗転処理
			 m_OverDriveDim = max(m_OverDriveDim-40, 64);	

			return; // ほかのキャラクターの所へは断じて通さない
		}
		
	}
	// 誰も覚醒していなかったので通常通り更新
	
	// ●暗転処理
	m_OverDriveDim=min(m_OverDriveDim+40, 255);
	
	

	/* プレイヤーたち更新 */
	FOR(m_NumPlayer)
	{
		m_pPlayers[i]->Update();			// モーションとか移動値の作成とか、基本的な更新。
	}

	/* プレイヤーVSプレイヤーの攻撃判定 */
	//bool bHit[m_NumPlayer];
	bool *bHit = new bool[m_NumPlayer];					// 複数ヒット用
	bool *bHitStand = new bool[m_NumPlayer];
	memset(bHit, false, sizeof(bool) * m_NumPlayer);	// falseで初期化
	memset(bHitStand, false, sizeof(bool)* m_NumPlayer);	// falseで初期化
	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			m_pStage->Sinking(m_pPlayers[my], m_pPlayers[you]);	// めり込み判定

			bool receive;	// 判定結果受取り用変数

			// ( my->you you->my )つまり交互に当たっていないかチェックする

			/* プレイヤーVSプレイヤーの攻撃判定 */
			receive = CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you]);
			bHit[my] = (bHit[my]) ? true : receive;		// 当たってたら、その後の判定結果とか気にしなくていい
			receive = CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my]);
			bHit[you] = (bHit[you]) ? true : receive;	// 当たってたら、その後の判定結果とか気にしなくていい

			/* スタンドVSプレイヤーの攻撃判定 */
			receive = CollisionStandAttack(m_pPlayers[my]->GetStand(), m_pPlayers[you]);
			bHitStand[my] = (bHitStand[my]) ? true : receive;		// 当たってたら、その後の判定結果とか気にしなくていい
			receive = CollisionStandAttack(m_pPlayers[you]->GetStand(), m_pPlayers[my]);
			bHitStand[you] = (bHitStand[you]) ? true : receive;		// 当たってたら、その後の判定結果とか気にしなくていい
		}
	}
	// プレイヤーVSプレイヤー攻撃結果確定
	FOR(m_NumPlayer)
	{
		if (bHit[i]) if(m_pPlayers[i]->isAttackState()) m_pPlayers[i]->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする
		if (bHitStand[i]) m_pPlayers[i]->GetStand()->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする
	}


	/* 位置を確定 */
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
		m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新
	}


	delete[] bHit;	// ポインタ配列の解放
	delete[] bHitStand;


	// チームポイント計算
	CalcTeamPoint();

}

void PlayerManager::Render()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

void PlayerManager::Render(tdnShader* shader, char* name)
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render(shader, name);
}

void PlayerManager::RenderDeferred()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderDeferred();

}

bool PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you)
{
	// チーム同じだよ！
	if (my->GetTeam() == you->GetTeam()) return false;

	// 攻撃系のステートじゃないよ！
	if (!my->isAttackState()) return false;

	// 相手がエスケープ中だよ！
	if (you->isEscape()) return false;

	if (my->isActiveFrame()) // 攻撃フレーム中なら
	{
		// 渾身の対空の処理
		/***************************************/
		// 相手も攻撃中なら
		if (you->isAttackState() == true)
		{
			// その攻撃が　もし始動・持続の間に
			// 「対空」効果を持っていた場合
			if ((you->GetActionFrame() == FRAME_STATE::ACTIVE ||
				you->GetActionFrame() == FRAME_STATE::START)
				&& you->GetAttackData()->bAntiAir== true)
			{
				// 自分が空中で攻撃していた場合
				if (my->isLand() == false)return false;
			}

		}
		/******************************************/

		if (you->GetInvincibleLV() <= my->GetAttackData()->pierceLV &&	// 相手が無敵でない
			!my->GetAttackData()->bHit)									// まだ攻撃を当ててない
		{
			// 攻撃判定形状と相手の四角で判定をとる
			CollisionShape::Square AttackShape, YouShape;
			memcpy_s(&AttackShape, sizeof(CollisionShape::Square), my->GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
			if (my->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			AttackShape.pos += my->GetPos();
			YouShape.pos += you->GetPos();
			if (Collision::HitCheck(&AttackShape, &YouShape))
			{
				// ヒット時の処理
				//my->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする

				/* メッセージ送信 */

				// 相手がヒットしたときの地上にいたか空中にいたか
				int iHitPlace = (you->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

				// フィニッシュアーツかどうか
				bool bFinish = (my->GetActionState() == BASE_ACTION_STATE::FINISH);

				// まず、攻撃をヒットさせた人に送信
				HIT_ATTACK_INFO HitAttackInfo;
				HitAttackInfo.HitPlayerDeviceID = you->GetDeviceID();									// ダメージを与えた相手の番号
				HitAttackInfo.hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// 自分自身にもの自分のヒットストップ
				HitAttackInfo.HitScore = my->GetAttackData()->HitScore;									// ダメージ(スコア)
				HitAttackInfo.bFinishAttack = bFinish;													// ふぃにしゅアーツかどうか
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + my->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &HitAttackInfo);
				// 数字エフェクト追加
				Vector2 screenPos= Math::WorldToScreen(my->GetPos());
				NumberEffect.AddNumber(screenPos.x, screenPos.y - 200, my->GetAttackData()->HitScore, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);


				// そして、ダメージを受けた人に送信
				HIT_DAMAGE_INFO HitDamageInfo;
				HitDamageInfo.BeInvincible = my->GetAttackData()->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
				HitDamageInfo.damage = my->GetAttackData()->damage;				// ダメージ(スコア)
				HitDamageInfo.FlyVector = my->GetAttackData()->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
				HitDamageInfo.hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// ヒットストップ
				HitDamageInfo.recoveryFlame = my->GetAttackData()->places[iHitPlace].recoveryFlame;		// 硬直時間
				HitDamageInfo.HitEffectType = (int)my->GetAttackData()->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
				HitDamageInfo.iAttackType = (int)my->GetActionState();						// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
				HitDamageInfo.bFinishAttack = bFinish;										// ふぃにしゅアーツかどうか
				if (
					//my->GetPos().x > you->GetPos().x// 位置関係によるベクトル
					my->GetDir() == DIR::LEFT			// 当てた人の向きによるベクトル
					) HitDamageInfo.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + you->GetDeviceID()), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// カメラに振動メッセージを送る
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &my->GetAttackData()->ShakeCameraInfo);

				// 音出す
				LPCSTR seID = my->GetAttackData()->HitSE;
				if (seID) se->Play((LPSTR)seID);

				return true;	// 当たった
			}
		}
	}
	return false;	// 当たらなかった
}

bool PlayerManager::CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou)
{
	// チーム同じだよ！
	if (pStand->GetPlayer()->GetTeam() == pYou->GetTeam()) return false;

	// スタンドがアクティブじゃないよ！
	if (!pStand->isActive()) return false;
	
	// スタンドがアタックタイプじゃないよ！
	if (pStand->GetAttackData() == nullptr)return false;

	// 相手がエスケープ中だよ！
	if (pYou->isEscape()) return false;
	
	if (pStand->isAttackFrame()) // 攻撃フレーム中なら
	{
		if (pYou->GetInvincibleLV() == 0 &&								// 相手が無敵でない
			!pStand->GetAttackData()->bHit)									// まだ攻撃を当ててない
		{
			// 攻撃判定形状と相手の四角で判定をとる
			CollisionShape::Square AttackShape, YouShape;
			memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pStand->GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			memcpy_s(&YouShape, sizeof(CollisionShape::Square), pYou->GetHitSquare(), sizeof(CollisionShape::Square));
			if (pStand->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			AttackShape.pos += pStand->GetPos();
			YouShape.pos += pYou->GetPos();
			if (Collision::HitCheck(&AttackShape, &YouShape))
			{
				// ヒット時の処理
				//my->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする
	
				/* メッセージ送信 */
	
				// 相手がヒットしたときの地上にいたか空中にいたか
				int iHitPlace = (pYou->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

				// まず、攻撃をヒットさせた人に送信
				HIT_ATTACK_INFO hai;
				hai.HitPlayerDeviceID = pYou->GetDeviceID();					// ダメージを与えた相手の番号
				hai.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// 自分自身にもの自分のヒットストップ
				hai.HitScore = pStand->GetAttackData()->HitScore;				// ダメージ(スコア)
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
				// 数字エフェクト追加
				Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
				NumberEffect.AddNumber(screenPos.x, screenPos.y - 200, pStand->GetAttackData()->HitScore, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

				// そして、ダメージを受けた人に送信
				HIT_DAMAGE_INFO hdi;
				hdi.BeInvincible = pStand->GetAttackData()->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
				hdi.damage = pStand->GetAttackData()->damage;				// ダメージ(スコア)
				hdi.FlyVector = pStand->GetAttackData()->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
				hdi.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// ヒットストップ
				hdi.recoveryFlame = pStand->GetAttackData()->places[iHitPlace].recoveryFlame;		// 硬直時間
				hdi.HitEffectType = (int)pStand->GetAttackData()->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
				hdi.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTIONのときは、コンボ継続関連のを無視する
				if (
					//my->GetPos().x > you->GetPos().x// 位置関係によるベクトル
					pStand->GetDir() == DIR::LEFT			// 当てた人の向きによるベクトル
					) hdi.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pYou->GetDeviceID()), MESSAGE_TYPE::HIT_DAMAGE, &hdi);
	

				// カメラに振動メッセージを送る
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pStand->GetAttackData()->ShakeCameraInfo);

				// 音出す
				LPCSTR seID = pStand->GetAttackData()->HitSE;
				if (seID) se->Play((LPSTR)seID);
	
				return true;	// 当たった
			}
		}
	}
	return false;	// 当たらなかった
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	// 今のところ送られることはないのかな
	return false;
}

// チームポイント計算
void PlayerManager::CalcTeamPoint()
{
	m_PointAteam = 0;
	m_PointBteam = 0;
	
	// プレイヤーたち描画
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->GetTeam()==TEAM::A)
		{
			m_PointAteam += m_pPlayers[i]->GetScore();
		}
		else
		{
			m_PointBteam += m_pPlayers[i]->GetScore();
		}	
	}


}
