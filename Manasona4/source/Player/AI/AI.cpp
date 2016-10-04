#include "AI.h"
#include "AIState.h"

/**********************************/
//		AIクラス
/**********************************/

AI::AI(int DeviceID,BasePlayer* myBasePlayer) 
:BaseGameEntity((ENTITY_ID(ENTITY_ID::ID_AI_FIRST + DeviceID)))
{
	// AIに必要なパラメーター
	m_iChaseFrame = 0;		// 一途に追いかけてる時間
	m_iWaitFrame = 0;		// もじもじしてる時間
	m_iGuardFrame = 0;		// ガードしてる時間

	m_iPracticeGuardFrame = 120;
	m_bPracticeGuardFlag = true;

	m_pTargetPlayer = nullptr;

	// このAIを所有しているプレイヤーのポインター
	m_pMyBasePlayer = myBasePlayer;


	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<AI>(this);
	m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
	m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());

	// インプットフラグ初期化
	memset(m_bPushFlag, false, sizeof(bool) * (int)PLAYER_INPUT::MAX);
}

AI::~AI()
{
	SAFE_DELETE(m_pStateMachine);
}

void AI::Control()
{
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		// 押されてるフラグがたってたら、とりあえず0にするのは置いとく
		if (m_bPushFlag[i])
		{
			m_bPushFlag[i] = false;
			continue;
		}

		int flag = m_pMyBasePlayer->GetInputList((PLAYER_INPUT)i);	// 現在のインプットの状態を取得
		// 入力リセット
		if (flag == 2)
			flag = 0;									// 離した後
		else if (flag == 1 || flag == 3) flag = 2;	// 離した瞬間
		m_pMyBasePlayer->SetInputList((PLAYER_INPUT)i, flag);		// 設定したインプットをプレイヤーに設定

	}
}

void AI::Update()
{
	Control();


	// AIのステートマシンになって更新している
	m_pStateMachine->Update();
}

// ステートマシンへの他から来るメッセージ
bool AI::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

