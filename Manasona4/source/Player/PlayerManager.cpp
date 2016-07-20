#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

// 実体の宣言
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// エンティティID登録
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr)
{

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
		m_pPlayers[i] = new Airou(i);
		m_pPlayers[i]->InitAttackDatas();		// ★攻撃情報を各キャラに初期化させる
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
	// プレイヤーたち更新
	FOR(m_NumPlayer)
	{
		m_pPlayers[i]->Update();			// モーションとか移動値の作成とか、基本的な更新。
	}

	// プレイヤーVSプレイヤーの攻撃判定
	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			CollisionPlayerAttack(my, you);
			CollisionPlayerAttack(you, my);
		}
	}

	// 位置を確定
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i]);	// ステージとの判定で、move値をどうこういじった後に
		m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新
	}
}

void PlayerManager::Render()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

void PlayerManager::CollisionPlayerAttack(int my, int you)
{
	if (m_pPlayers[my]->isAttackFrame()) // 攻撃フレーム中なら
	{
		if (m_pPlayers[you]->GetInvincibleLV() <= m_pPlayers[my]->GetPierceLV()) // 相手が無敵でない
		{
			if (Math::Length(m_pPlayers[my]->GetPos(), m_pPlayers[you]->GetPos()) < 10)
			{
				// ヒット時の処理

				/* メッセージ送信 */

				// まず、攻撃をヒットさせた人に送信
				HIT_ATTACK_INFO hai;
				hai.HitPlayerNo = you;	// ダメージを与えた相手の番号
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + my), MESSAGE_TYPE::HIT_ATTACK, &hai);

				// そして、ダメージを受けた人に送信
				HIT_DAMAGE_INFO hdi;
				hdi.BeInvincible = m_pPlayers[my]->GetAttackData()->bBeInvincible;
				hdi.damage = m_pPlayers[my]->GetAttackData()->damage;
				hdi.FlyVector = Vector2(0, 4);
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + you), MESSAGE_TYPE::HIT_DAMAGE, &hdi);

				// 音出す
				LPCSTR seID = m_pPlayers[my]->GetAttackSE();
				if (strcmp(seID, "") != 0) se->Play((LPSTR)seID);
			}
		}
	}
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	// 今のところ送られることはないのかな
	return false;
}