#include "AI.h"
#include "AIState.h"

/**********************************/
//		AI�N���X
/**********************************/

AI::AI(SIDE side,BasePlayer* myBasePlayer)
:BaseGameEntity((ENTITY_ID(ENTITY_ID::ID_AI_FIRST + (int)side)))
{
	// AI�ɕK�v�ȃp�����[�^�[
	m_iChaseFrame = 0;		// ��r�ɒǂ������Ă鎞��
	m_iWaitFrame = 0;		// �����������Ă鎞��
	m_iGuardFrame = 0;		// �K�[�h���Ă鎞��

	m_iPracticeGuardFrame = 120;
	m_bPracticeGuardFlag = true;

	// [11/25] �e�̃^�[�Q�b�g�Ǝw�肵�Ă�v���C���[���w��
	m_pTargetPlayer = myBasePlayer->GetTargetPlayer();

	// ����AI�����L���Ă���v���C���[�̃|�C���^�[
	m_pMyBasePlayer = myBasePlayer;


	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<AI>(this);
	m_pStateMachine->SetGlobalState(AIState::Global::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(AIState::Wait::GetInstance());

	// �C���v�b�g�t���O������
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
		// ������Ă�t���O�������Ă���A�Ƃ肠����0�ɂ���̂͒u���Ƃ�
		if (m_bPushFlag[i])
		{
			m_bPushFlag[i] = false;
			continue;
		}

		int flag = m_pMyBasePlayer->GetInputList((PLAYER_INPUT)i);	// ���݂̃C���v�b�g�̏�Ԃ��擾
		// ���̓��Z�b�g
		if (flag == 2)
			flag = 0;									// ��������
		else if (flag == 1 || flag == 3) flag = 2;	// �������u��
		m_pMyBasePlayer->SetInputList((PLAYER_INPUT)i, flag);		// �ݒ肵���C���v�b�g���v���C���[�ɐݒ�

	}
}

void AI::Update()
{
	Control();


	// AI�̃X�e�[�g�}�V���ɂȂ��čX�V���Ă���
	m_pStateMachine->Update();
}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool AI::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

