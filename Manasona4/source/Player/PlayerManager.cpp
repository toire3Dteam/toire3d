#include "PlayerManager.h"
#include "Airou/Airou.h"
#include "Nazenara/Nazenara.h"
#include "Teki\Teki.h"
#include "Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"
#include "../Collision/Collision.h"

// 実体の宣言
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// エンティティID登録
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr),
m_bHeaveHoDriveOverFlow(false)
{
	m_PointAteam = 0;
	m_PointBteam = 0;

	m_OverDriveDim = 255;
}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage, SideData SideDatas[(int)SIDE::ARRAY_MAX])
{
	m_NumPlayer = NumPlayer;
	m_pStage = pStage;

	// プレイヤ確保
	m_pPlayers = new BasePlayer*[NumPlayer];

	FOR(NumPlayer)
	{
		// (TODO)チームを今は仮で振り分け!! 2016/10.04日　左か右をTeamで変えてる
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// キャラクターで分岐する
		switch (SideDatas[(int)side].character)
		{
		case CHARACTER::AIROU:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			break;
		case CHARACTER::TEKI:
			m_pPlayers[i] = new Teki(side, SideDatas[(int)side]);
			break;
		case CHARACTER::NAZENARA:
			m_pPlayers[i] = new Nazenara(side, SideDatas[(int)side]);
			break;
		default:
			m_pPlayers[i] = new Airou(side, SideDatas[(int)side]);
			//MyAssert(false,"そんなキャラクターはいない")
			break;
		}
		m_pPlayers[i]->InitActionDatas();		// ★攻撃情報を各キャラに初期化させる
		m_pPlayers[i]->InitMotionDatas();		// 各キャラごとのモーション番号
	}

	// プレイヤーが2人という前提のコード
	assert(NumPlayer == 2);
	m_pPlayers[0]->SetTargetPlayer(m_pPlayers[1]);
	m_pPlayers[1]->SetTargetPlayer(m_pPlayers[0]);

	FOR(NumPlayer)
	{
		// (TODO)チームを今は仮で振り分け!! 2016/10.04日　左か右をTeamで変えてる
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// AI初期化 なぜここで初期化するというと、初期化の中でプレイヤーを参照することがあり、全員の初期化が完了した後に呼び出したいから
		m_pPlayers[i]->InitAI(SideDatas[(int)side].eAIType);
	}
}

PlayerManager::~PlayerManager()
{
	// ダブルポインターの開放
	FOR(m_NumPlayer)delete m_pPlayers[i];
	delete[] m_pPlayers;
}

void PlayerManager::Update(PLAYER_UPDATE flag)
{	
	// 誰かが1More覚醒していたら全員の動きを止める
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isGameTimerStopFlag() == true)
		{
			m_pPlayers[i]->Update(flag);			// モーションとか移動値の作成とか、基本的な更新。
			
			//m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
			//m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新

			// ●暗転処理
			m_OverDriveDim = max(m_OverDriveDim - 0.1f , 0.25f);

			return; // ほかのキャラクターの所へは断じて通さない
		}
		
	}
	// 誰も覚醒していなかったので通常通り更新
	
	// ●暗転解放処理
	m_OverDriveDim = min(m_OverDriveDim + 0.1f, 1.0f);
	
	

	/* プレイヤーたち更新 */
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)m_pPlayers[i]->UpdateDrive();			// モーションとか移動値の作成とか、基本的な更新。
		else m_pPlayers[i]->Update(flag);
	}

	// チームポイント計算
	//CalcTeamPoint();
}

void PlayerManager::UpdateHit()
{

}

void PlayerManager::UpdatePos()
{
	/* 位置を確定 */
	FOR(m_NumPlayer)
	{
		// 相手が時を止めてたらスルーする
		if (m_pPlayers[i]->GetTargetPlayer()->isGameTimerStopFlag()) continue;

		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
		m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新
	}
}

void PlayerManager::Render()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)
			m_pPlayers[i]->RenderDrive();
		else 
			m_pPlayers[i]->Render();

	}
}

void PlayerManager::Render(tdnShader* shader, char* name)
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render(shader, name);
}

void PlayerManager::RenderShadow()
{
	// プレイヤーの影描画用
	FOR(m_NumPlayer) m_pPlayers[i]->RenderShadow();

}

void PlayerManager::RenderDeferred()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderDeferred();

}

void PlayerManager::RenderUI()
{
	// プレイヤーのUI描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderUI();
}

//void PlayerManager::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
//{
//	/* メッセージ送信 */
//
//	// ダメージ受けたよメッセージ
//	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);
//
//	// ダメージを与えるやつに対してヒットストップをかける
//	pAttackPlayer->SetHitStopFrame(pHitDamageInfo->iHitStopFrame);
//	//pDamagePlayer->SetHitStopFrame(pHitDamageInfo->iHitStopFrame);
//}

bool PlayerManager::HandleMessage(const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		m_bHeaveHoDriveOverFlow = true;
		break;
	}
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
		if (m_pPlayers[i]->GetSide()==SIDE::LEFT)
		{
			m_PointAteam += m_pPlayers[i]->GetScore();
		}
		else
		{
			m_PointBteam += m_pPlayers[i]->GetScore();
		}	
	}


}

//void PlayerManager::UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you)
//{
//	const Vector3 v(you->GetPos() - my->GetPos());
//	if (v.x > 0)
//	{
//		my->SetTargetDir(DIR::RIGHT);
//		you->SetTargetDir(DIR::LEFT);
//	}
//	else if (v.x < 0)
//	{
//		my->SetTargetDir(DIR::LEFT);
//		you->SetTargetDir(DIR::RIGHT);
//	}
//}