#include "AI.h"
#include "AIState.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"

/**********************************/
//		AIクラス
/**********************************/

AI::AI(SIDE side,BasePlayer* myBasePlayer , AI_TYPE AIType)
:BaseGameEntity((ENTITY_ID(ENTITY_ID::ID_AI_FIRST + (int)side)))
{
	// AIに必要なパラメーター
	m_iChaseFrame = 0;		// 一途に追いかけてる時間
	m_iWaitFrame = 0;		// もじもじしてる時間
	m_iGuardFrame = 0;		// ガードしてる時間

	// 練習用
	m_iPracticeGuardFrame = 0;
	m_iPracticeAttackIntervalFrame = 0;
	m_bPracticeGuardFlag = false;

	// 二回押し関連
	m_bDoublePush = false;
	m_iDoublePushFrame = 0;
	m_eDoublePushInput = PLAYER_INPUT::UP;

	// どっこいダッシュフラグ
	m_bDokkoiDush = false;

	// [11/25] 親のターゲットと指定してるプレイヤーを指定
	m_pTargetPlayer = myBasePlayer->GetTargetPlayer();

	// このAIを所有しているプレイヤーのポインター
	m_pMyBasePlayer = myBasePlayer;

	// AIのタイプ設定
	m_eAIType = AIType;

	// AIの種類によってステートマシンを切り替え
	switch (m_eAIType)
	{
	case AI_TYPE::CPU_EASY:
	case AI_TYPE::CPU_NORMAL:
	case AI_TYPE::CPU_HARD:
	case AI_TYPE::CPU_YOKOE:

		// 戦闘用
		// ステートマシン　引数は自分自身のポインタ
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::Global::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::Wait::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::Wait::GetInstance());

		break;
	case AI_TYPE::PRACTICE_LAND:
		// 練習用ステートマシン
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeLand::GetInstance());

		break;
	case AI_TYPE::PRACTICE_SQUAT:
		// 練習用ステートマシン
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeSquat::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeSquat::GetInstance());

		break;
	case AI_TYPE::PRACTICE_JUMP:
		// 空中に飛び続ける練習用ステートマシン
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeJump::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeJump::GetInstance());

		break;
	case AI_TYPE::PRACTICE_ATTACK:
		// 攻撃を振り続ける練習用ステートマシン
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeAttack::GetInstance());

		break;

	case AI_TYPE::INTERRUPT_INVINCIBLE:
		// ガード中に無敵割り込みを行う確認用ステートマシン
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::InterruptInvincible::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::InterruptInvincible::GetInstance());

		break;
	default:
		break;
	}


	// インプットフラグ初期化
	memset(m_bPushFlag, false, sizeof(bool) * (int)PLAYER_INPUT::MAX);
}

AI::~AI()
{
	SAFE_DELETE(m_pStateMachine);
}

void AI::Control()
{
	//for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	//{
	//	// 押されてるフラグがたってたら、とりあえず0にするのは置いとく
	//	if (m_bPushFlag[i])
	//	{
	//		m_bPushFlag[i] = false;
	//		continue;
	//	}

	//	// 離す処理
	//	int flag = m_pMyBasePlayer->GetInputList((PLAYER_INPUT)i);	// 現在のインプットの状態を取得
	//	// 入力リセット
	//	if (flag == 2)
	//		flag = 0;									// 離した後
	//	else if (flag == 1 || flag == 3) flag = 2;	// 離した瞬間
	//	m_pMyBasePlayer->SetInputList((PLAYER_INPUT)i, flag);		// 設定したインプットをプレイヤーに設定

	//}
}

void AI::Update()
{

	// AIのステートマシンになって更新している
	m_pStateMachine->Update();
	
	//Control();
	// 依頼を受けた後すぐ反応したいのでステートマシンの下で
	DoublePushUpdate();
}

// 二回押し更新
void AI::DoublePushUpdate()
{
	// 二回押しの依頼受けたら発動する
	if (m_bDoublePush == false) return;

	// 最初の一回は始めに押す
	if (m_iDoublePushFrame == 0)
	{
		// ボタン押す
		PushInputList(m_eDoublePushInput);
	}

	// 二回目を押す
	if (m_iDoublePushFrame == 2)
	{
		// ボタン押す
		PushInputList(m_eDoublePushInput);

		// 終り
		m_bDoublePush = false;
	}

	// フレーム更新
	m_iDoublePushFrame++;

}

// トレーニング・練習用
void AI::UpdateTraining()
{
	// 結局ステートの変更前に設定し直しているので必要ないけど一応
	// 設定が変更されていたら切り替える
	//if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyState 
	//	== (ENEMY_STATE_TYPE)PlayerDataMgr->m_TrainingData.iEnemyState)
	//{
	//	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState
	//		= (ENEMY_STATE_TYPE)PlayerDataMgr->m_TrainingData.iEnemyState;
	//}

	//if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard
	//	== (ENEMY_GUARD_TYPE)PlayerDataMgr->m_TrainingData.iEnemyGuard)
	//{
	//	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard
	//		= (ENEMY_GUARD_TYPE)PlayerDataMgr->m_TrainingData.iEnemyGuard;
	//}

	//if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch
	//	== (ENEMY_GUARD_SWITCH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch)
	//{
	//	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch
	//		= (ENEMY_GUARD_SWITCH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch;
	//}

	//if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech
	//	== (ENEMY_TECH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyTech)
	//{
	//	SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech
	//		= (ENEMY_TECH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyTech;
	//}

	//if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech
	//	== (ENEMY_THROW_TECH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyThrowTech)
	//{
	//	SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech
	//		= (ENEMY_THROW_TECH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyThrowTech;
	//}

	switch (SelectDataMgr->Get()->tagTrainingDatas.eEnemyState)
	{
	case ENEMY_STATE_TYPE::STAND:
		// 設定したタイプと違っていたら
		if (m_eAIType != AI_TYPE::PRACTICE_LAND)
		{
			ChangeAIType(AI_TYPE::PRACTICE_LAND);
		}

		break;
	case ENEMY_STATE_TYPE::SQUAT:
		// 設定したタイプと違っていたら
		if (m_eAIType != AI_TYPE::PRACTICE_SQUAT)
		{
			ChangeAIType(AI_TYPE::PRACTICE_SQUAT);
		}

		break;
	case ENEMY_STATE_TYPE::JUMP:
		// 設定したタイプと違っていたら
		if (m_eAIType != AI_TYPE::PRACTICE_JUMP)
		{
			ChangeAIType(AI_TYPE::PRACTICE_JUMP);
		}

		break;
	case ENEMY_STATE_TYPE::ATTACK:
		// 設定したタイプと違っていたら
		if (m_eAIType != AI_TYPE::PRACTICE_ATTACK)
		{
			ChangeAIType(AI_TYPE::PRACTICE_ATTACK);
		}

		break;
	case ENEMY_STATE_TYPE::COM:

		// COMレベルによって強さを変える
		if (SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv <= 30)
		{
			// 設定したタイプと違っていたら
			if (m_eAIType != AI_TYPE::CPU_EASY)
			{
				ChangeAIType(AI_TYPE::CPU_EASY);
			}
		}
		else if (SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv <= 60)
		{
			// 設定したタイプと違っていたら
			if (m_eAIType != AI_TYPE::CPU_NORMAL)
			{
				ChangeAIType(AI_TYPE::CPU_NORMAL);
			}
		}
		else if (SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv <= 85)
		{
			// 設定したタイプと違っていたら
			if (m_eAIType != AI_TYPE::CPU_HARD)
			{
				ChangeAIType(AI_TYPE::CPU_HARD);
			}
		}
		else 
		{
			// 設定したタイプと違っていたら
			if (m_eAIType != AI_TYPE::CPU_YOKOE)
			{
				ChangeAIType(AI_TYPE::CPU_YOKOE);
			}
		}

	
		break;
	case ENEMY_STATE_TYPE::T_DOKKOI_ATTACK:
		// 設定したタイプと違っていたら
		if (m_eAIType != AI_TYPE::T_DOKKOI_ATTACK)
		{
			ChangeAIType(AI_TYPE::T_DOKKOI_ATTACK);
		}
		

		break;
	case ENEMY_STATE_TYPE::T_DOWN_ATTACK:
		// 設定したタイプと違っていたら
		if (m_eAIType != AI_TYPE::T_DOWN_ATTACK)
		{
			ChangeAIType(AI_TYPE::T_DOWN_ATTACK);
		}

		break;
	default:
		MyAssert(0, "そのタイプはない");
		break;
	}

}

// AIの種類を変える		※ ステート切り替えはSetで変えてます
void AI::ChangeAIType(AI_TYPE type)
{
	// AIのタイプ設定
	m_eAIType = type;

	// AIの種類によってステートマシンを切り替え
	switch (m_eAIType)
	{
	case AI_TYPE::CPU_EASY:
	case AI_TYPE::CPU_NORMAL:
	case AI_TYPE::CPU_HARD:
	case AI_TYPE::CPU_YOKOE:

		// 戦闘用
		// ステートマシン　引数は自分自身のポインタ
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::Global::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::Wait::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::Wait::GetInstance());

		break;
	case AI_TYPE::PRACTICE_LAND:
		// 練習用ステートマシン
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeLand::GetInstance());

		break;
	case AI_TYPE::PRACTICE_SQUAT:
		// 練習用ステートマシン
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeSquat::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeSquat::GetInstance());

		break;
	case AI_TYPE::PRACTICE_JUMP:
		// 空中に飛び続ける練習用ステートマシン
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeJump::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeJump::GetInstance());

		break;
	case AI_TYPE::PRACTICE_ATTACK:
		// 攻撃を振り続ける練習用ステートマシン
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeAttack::GetInstance());

		break;
	case AI_TYPE::T_DOKKOI_ATTACK:
		// 中段攻撃を振り続ける練習用ステートマシン
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeDokkoiAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeDokkoiAttack::GetInstance());

		break;
	case AI_TYPE::T_DOWN_ATTACK:
		// 下段攻撃を振り続ける練習用ステートマシン
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(AIState::PracticeDownAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeDownAttack::GetInstance());

		break;
	default:
		break;
	}



}

// (TODO)二回押しというより　タイミングや何回押すかを決めれるやつにするとのちのち便利かも
// 二回押し
void AI::ActionDoublePush(PLAYER_INPUT inputNo)
{
	// 他の二回押し処理が終わるまで受け付けない
	if (m_bDoublePush == true) return;
	m_iDoublePushFrame = 0;

	m_eDoublePushInput = inputNo;
	
	m_bDoublePush = true; // 二回押し実行

}

// ステートマシンへの他から来るメッセージ
bool AI::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

