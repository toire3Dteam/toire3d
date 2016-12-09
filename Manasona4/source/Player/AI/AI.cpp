#include "AI.h"
#include "AIState.h"

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

	m_iPracticeGuardFrame = 0;
	m_iPracticeAttackIntervalFrame = 0;

	m_bPracticeGuardFlag = false;

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

		break;
	case AI_TYPE::PRACTICE_LAND:
		// ���K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());

		break;
	case AI_TYPE::PRACTICE_JUMP:
		// �󒆂ɔ�ё�������K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeJump::GetInstance());

		break;
	case AI_TYPE::PRACTICE_ATTACK:
		// �U����U�葱������K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeAttack::GetInstance());

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
	Control();

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
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::Global::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::Wait::GetInstance());

		break;
	case AI_TYPE::PRACTICE_LAND:
		// ���K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeLand::GetInstance());

		break;
	case AI_TYPE::PRACTICE_JUMP:
		// �󒆂ɔ�ё�������K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeJump::GetInstance());

		break;
	case AI_TYPE::PRACTICE_ATTACK:
		// �U����U�葱������K�p�X�e�[�g�}�V��
		m_pStateMachine = new StateMachine<AI>(this);
		m_pStateMachine->SetGlobalState(AIState::PracticeGlobal::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(AIState::PracticeAttack::GetInstance());

		break;
	default:
		break;
	}



}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool AI::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

