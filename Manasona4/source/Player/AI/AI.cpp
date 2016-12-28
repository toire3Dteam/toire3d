#include "AI.h"
#include "AIState.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"

/**********************************/
//		AI�N���X
/**********************************/

AI::AI(SIDE side,BasePlayer* myBasePlayer , AI_TYPE AIType)
:BaseGameEntity((ENTITY_ID(ENTITY_ID::ID_AI_FIRST + (int)side)))
{
	// AI�ɕK�v�ȃp�����[�^�[
	m_iChaseFrame = 0;		// ��r�ɒǂ������Ă鎞��
	m_iWaitFrame = 0;		// �����������Ă鎞��
	m_iGuardFrame = 0;		// �K�[�h���Ă鎞��

	// ���K�p
	m_iPracticeGuardFrame = 0;
	m_iPracticeAttackIntervalFrame = 0;
	m_bPracticeGuardFlag = false;

	// ��񉟂��֘A
	m_bDoublePush = false;
	m_iDoublePushFrame = 0;
	m_eDoublePushInput = PLAYER_INPUT::UP;

	// [11/25] �e�̃^�[�Q�b�g�Ǝw�肵�Ă�v���C���[���w��
	m_pTargetPlayer = myBasePlayer->GetTargetPlayer();

	// ����AI�����L���Ă���v���C���[�̃|�C���^�[
	m_pMyBasePlayer = myBasePlayer;

	// AI�̃^�C�v�ݒ�
	m_eAIType = AIType;

	// AI�̎�ނɂ���ăX�e�[�g�}�V����؂�ւ�
	switch (m_eAIType)
	{
	case AI_TYPE::CPU_EASY:
	case AI_TYPE::CPU_NORMAL:
	case AI_TYPE::CPU_HARD:
	case AI_TYPE::CPU_YOKOE:

		// �퓬�p
		// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::Global::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::Wait::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::Wait::GetInstance());

		break;
	case AI_TYPE::PRACTICE_LAND:
		// ���K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeLand::GetInstance());

		break;
	case AI_TYPE::PRACTICE_SQUAT:
		// ���K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeSquat::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeSquat::GetInstance());

		break;
	case AI_TYPE::PRACTICE_JUMP:
		// �󒆂ɔ�ё�������K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeJump::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeJump::GetInstance());

		break;
	case AI_TYPE::PRACTICE_ATTACK:
		// �U����U�葱������K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeAttack::GetInstance());

		break;

	case AI_TYPE::INTERRUPT_INVINCIBLE:
		// �K�[�h���ɖ��G���荞�݂��s���m�F�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::InterruptInvincible::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::InterruptInvincible::GetInstance());

		break;
	default:
		break;
	}


	// �C���v�b�g�t���O������
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
	//	// ������Ă�t���O�������Ă���A�Ƃ肠����0�ɂ���̂͒u���Ƃ�
	//	if (m_bPushFlag[i])
	//	{
	//		m_bPushFlag[i] = false;
	//		continue;
	//	}

	//	// ��������
	//	int flag = m_pMyBasePlayer->GetInputList((PLAYER_INPUT)i);	// ���݂̃C���v�b�g�̏�Ԃ��擾
	//	// ���̓��Z�b�g
	//	if (flag == 2)
	//		flag = 0;									// ��������
	//	else if (flag == 1 || flag == 3) flag = 2;	// �������u��
	//	m_pMyBasePlayer->SetInputList((PLAYER_INPUT)i, flag);		// �ݒ肵���C���v�b�g���v���C���[�ɐݒ�

	//}
}

void AI::Update()
{

	// AI�̃X�e�[�g�}�V���ɂȂ��čX�V���Ă���
	m_pStateMachine->Update();
	
	//Control();
	// �˗����󂯂��シ�������������̂ŃX�e�[�g�}�V���̉���
	DoublePushUpdate();
}

// ��񉟂��X�V
void AI::DoublePushUpdate()
{
	// ��񉟂��̈˗��󂯂��甭������
	if (m_bDoublePush == false) return;

	// �ŏ��̈��͎n�߂ɉ���
	if (m_iDoublePushFrame == 0)
	{
		// �{�^������
		PushInputList(m_eDoublePushInput);
	}

	// ���ڂ�����
	if (m_iDoublePushFrame == 2)
	{
		// �{�^������
		PushInputList(m_eDoublePushInput);

		// �I��
		m_bDoublePush = false;
	}

	// �t���[���X�V
	m_iDoublePushFrame++;

}

// �g���[�j���O�E���K�p
void AI::UpdateTraining()
{
	// ���ǃX�e�[�g�̕ύX�O�ɐݒ肵�����Ă���̂ŕK�v�Ȃ����ǈꉞ
	// �ݒ肪�ύX����Ă�����؂�ւ���
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
		// �ݒ肵���^�C�v�ƈ���Ă�����
		if (m_eAIType != AI_TYPE::PRACTICE_LAND)
		{
			ChangeAIType(AI_TYPE::PRACTICE_LAND);
		}

		break;
	case ENEMY_STATE_TYPE::SQUAT:
		// �ݒ肵���^�C�v�ƈ���Ă�����
		if (m_eAIType != AI_TYPE::PRACTICE_SQUAT)
		{
			ChangeAIType(AI_TYPE::PRACTICE_SQUAT);
		}

		break;
	case ENEMY_STATE_TYPE::JUMP:
		// �ݒ肵���^�C�v�ƈ���Ă�����
		if (m_eAIType != AI_TYPE::PRACTICE_JUMP)
		{
			ChangeAIType(AI_TYPE::PRACTICE_JUMP);
		}

		break;
	case ENEMY_STATE_TYPE::ATTACK:
		// �ݒ肵���^�C�v�ƈ���Ă�����
		if (m_eAIType != AI_TYPE::PRACTICE_ATTACK)
		{
			ChangeAIType(AI_TYPE::PRACTICE_ATTACK);
		}

		break;
	case ENEMY_STATE_TYPE::COM:

		// COM���x���ɂ���ċ�����ς���
		if (SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv <= 30)
		{
			// �ݒ肵���^�C�v�ƈ���Ă�����
			if (m_eAIType != AI_TYPE::CPU_EASY)
			{
				ChangeAIType(AI_TYPE::CPU_EASY);
			}
		}
		else if (SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv <= 60)
		{
			// �ݒ肵���^�C�v�ƈ���Ă�����
			if (m_eAIType != AI_TYPE::CPU_NORMAL)
			{
				ChangeAIType(AI_TYPE::CPU_NORMAL);
			}
		}
		else if (SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv <= 85)
		{
			// �ݒ肵���^�C�v�ƈ���Ă�����
			if (m_eAIType != AI_TYPE::CPU_HARD)
			{
				ChangeAIType(AI_TYPE::CPU_HARD);
			}
		}
		else 
		{
			// �ݒ肵���^�C�v�ƈ���Ă�����
			if (m_eAIType != AI_TYPE::CPU_YOKOE)
			{
				ChangeAIType(AI_TYPE::CPU_YOKOE);
			}
		}

	
		break;
	case ENEMY_STATE_TYPE::T_DOKKOI_ATTACK:
		// �ݒ肵���^�C�v�ƈ���Ă�����
		if (m_eAIType != AI_TYPE::T_DOKKOI_ATTACK)
		{
			ChangeAIType(AI_TYPE::T_DOKKOI_ATTACK);
		}
		

		break;
	case ENEMY_STATE_TYPE::T_DOWN_ATTACK:
		// �ݒ肵���^�C�v�ƈ���Ă�����
		if (m_eAIType != AI_TYPE::T_DOWN_ATTACK)
		{
			ChangeAIType(AI_TYPE::T_DOWN_ATTACK);
		}

		break;
	default:
		MyAssert(0, "���̃^�C�v�͂Ȃ�");
		break;
	}

}

// AI�̎�ނ�ς���		�� �X�e�[�g�؂�ւ���Set�ŕς��Ă܂�
void AI::ChangeAIType(AI_TYPE type)
{
	// AI�̃^�C�v�ݒ�
	m_eAIType = type;

	// AI�̎�ނɂ���ăX�e�[�g�}�V����؂�ւ�
	switch (m_eAIType)
	{
	case AI_TYPE::CPU_EASY:
	case AI_TYPE::CPU_NORMAL:
	case AI_TYPE::CPU_HARD:
	case AI_TYPE::CPU_YOKOE:

		// �퓬�p
		// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::Global::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::Wait::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::Wait::GetInstance());

		break;
	case AI_TYPE::PRACTICE_LAND:
		// ���K�p�X�e�[�g�}�V��
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeLand::GetInstance());

		break;
	case AI_TYPE::PRACTICE_SQUAT:
		// ���K�p�X�e�[�g�}�V��
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeSquat::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeSquat::GetInstance());

		break;
	case AI_TYPE::PRACTICE_JUMP:
		// �󒆂ɔ�ё�������K�p�X�e�[�g�}�V��
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeJump::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeJump::GetInstance());

		break;
	case AI_TYPE::PRACTICE_ATTACK:
		// �U����U�葱������K�p�X�e�[�g�}�V��
		//m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeAttack::GetInstance());

		break;
	case AI_TYPE::T_DOKKOI_ATTACK:
		// ���i�U����U�葱������K�p�X�e�[�g�}�V��
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeDokkoiAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeDokkoiAttack::GetInstance());

		break;
	case AI_TYPE::T_DOWN_ATTACK:
		// ���i�U����U�葱������K�p�X�e�[�g�}�V��
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeDownAttack::GetInstance());
		m_pStateMachine->SetPreviousState(AIState::PracticeDownAttack::GetInstance());

		break;
	default:
		break;
	}



}

// (TODO)��񉟂��Ƃ������@�^�C�~���O�≽�񉟂��������߂���ɂ���Ƃ̂��̂��֗�����
// ��񉟂�
void AI::ActionDoublePush(PLAYER_INPUT inputNo)
{
	// ���̓�񉟂��������I���܂Ŏ󂯕t���Ȃ�
	if (m_bDoublePush == true) return;
	m_iDoublePushFrame = 0;

	m_eDoublePushInput = inputNo;
	
	m_bDoublePush = true; // ��񉟂����s

}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool AI::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

