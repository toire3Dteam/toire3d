#include "TDNLIB.h"
#include "Player.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"

Fighter::Base::Base(int deviceID) :m_Gravity(.14f), m_dir(DIR::LEFT),m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square), m_pObj(nullptr), m_move(VECTOR_ZERO)
{
	// デフォルト設定
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

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
}

Fighter::Base::~Base()
{
	SAFE_DELETE(m_pObj);
	delete m_pHitSquare;
}

void Fighter::Base::Update()
{
	// 入力受付
	Control();

	// メッシュの更新
	m_pObj->Animation();
	m_pObj->SetAngle((m_dir == DIR::LEFT) ? PI * .5f : PI * 1.5f);	// 左右のアングルのセット
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// 移動量更新
	m_move.y -= m_Gravity;
}

void Fighter::Base::Control()
{
	const float speed = 2.0f;
	if (KeyBoard(KB_LEFT)) m_move.x = -speed;
	else if (KeyBoard(KB_RIGHT)) m_move.x = speed;
	else m_move.x = 0;
}

void Fighter::Base::UpdatePos()
{
	// 座標更新
	m_pos += m_move;
}

void Fighter::Base::Render()
{
	m_pObj->Render();
}

Fighter::Airou::Airou(int deviceID) :Base(deviceID)
{
	// アイルーメッシュ
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou.IEM");
}

PlayerManager::PlayerManager(int NumPlayer, Stage::Base *pStage) :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// エンティティID登録
m_pStage(pStage), m_NumPlayer(NumPlayer)
{
	// プレイヤ確保
	m_pPlayers = new Fighter::Base*[NumPlayer];

	FOR(NumPlayer)
	{
		m_pPlayers[i] = new Fighter::Airou(i);
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
		m_pStage->Collision(m_pPlayers[i]);	// ステージとの判定で、move値をどうこういじった後に
		m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新
	}
}

void PlayerManager::Render()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	// 今のところ送られることはないのかな
	return false;
}