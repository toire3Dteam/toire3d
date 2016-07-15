#include "TDNLIB.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"

BasePlayer::BasePlayer(int deviceID) :BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)), m_Gravity(.14f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square), m_pObj(nullptr), m_move(VECTOR_ZERO)
{
	// デフォルト設定
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// 初期化
	memset(m_InputList, 0, sizeof(m_InputList));

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
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pStateMachine);
	delete m_pHitSquare;
}

void BasePlayer::Update()
{
	// 入力受付
	Control();

	// 入力受付後にステートマシン更新
	m_pStateMachine->Update();

	//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
	//else m_move.x = 0;

	// メッシュの更新
	m_pObj->Animation();
	m_pObj->SetAngle((m_dir == DIR::LEFT) ? PI * .5f : PI * 1.5f);	// 左右のアングルのセット
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// 移動量更新
	m_move.y -= m_Gravity;
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
	// 座標更新
	m_pos += m_move;
}

void BasePlayer::Render()
{
	m_pObj->Render();
	
	m_pStateMachine->Render();// ステートマシンでの描画
}

// ステートマシンへの他から来るメッセージ
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}