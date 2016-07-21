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
	/* プレイヤーたち更新 */
	FOR(m_NumPlayer)
	{
		m_pPlayers[i]->Update();			// モーションとか移動値の作成とか、基本的な更新。
	}

	/* プレイヤーVSプレイヤーの攻撃判定 */
	bool *bHit = new bool[m_NumPlayer];					// 複数ヒット用
	memset(bHit, false, sizeof(bool) * m_NumPlayer);	// falseで初期化
	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			bool receive;	// 判定結果受取り用変数

			receive = CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you]);
			bHit[my] = (bHit[my]) ? true : receive;		// 当たってたら、その後の判定結果とか気にしなくていい

			receive = CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my]);
			bHit[you] = (bHit[you]) ? true : receive;	// 当たってたら、その後の判定結果とか気にしなくていい
		}
	}
	// 攻撃結果確定
	FOR(m_NumPlayer) if(bHit[i]) m_pPlayers[i]->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする
	delete[] bHit;	// ポインタ配列の解放


	/* 位置を確定 */
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

bool PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you)
{
	if (my->isAttackFrame()) // 攻撃フレーム中なら
	{
		if (you->GetInvincibleLV() <= my->GetPierceLV() &&	// 相手が無敵でない
			!my->GetAttackData()->bHit)									// まだ攻撃を当ててない
		{
			if (Math::Length(my->GetPos(), you->GetPos()) < 20)
			{
				// ヒット時の処理
				//my->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする

				/* メッセージ送信 */

				// まず、攻撃をヒットさせた人に送信
				HIT_ATTACK_INFO hai;
				hai.HitPlayerDeviceID = you->GetDeviceID();	// ダメージを与えた相手の番号
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + my->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);

				// そして、ダメージを受けた人に送信
				HIT_DAMAGE_INFO hdi;
				hdi.BeInvincible = my->GetAttackData()->bBeInvincible;	// 無敵になるかどうか
				hdi.damage = my->GetAttackData()->damage;				// ダメージ(スコア)
				hdi.FlyVector = my->GetAttackData()->FlyVector;			// 吹っ飛びベクトル
				if (my->GetPos().x > you->GetPos().x) hdi.FlyVector.x *= -1;
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + you->GetDeviceID()), MESSAGE_TYPE::HIT_DAMAGE, &hdi);

				// 音出す
				LPCSTR seID = my->GetAttackSE();
				if (strcmp(seID, "") != 0) se->Play((LPSTR)seID);

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