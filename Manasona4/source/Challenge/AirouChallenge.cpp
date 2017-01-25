#include "AirouChallenge.h"

//+-------------------------------
// �A�C���\�`�������W
//+-------------------------------

AirouChallenge1::AirouChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�����U��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 120;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("�ʏ�U������U��ňЗ͂��ƂĂ������L�����N�^�[�ł��B\n�܂��͍U���𓖂Ă邽�߂ɒ��i�Z�≺�i�Z��g�ݍ���\n�K�[�h������y�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n����̃K�[�h������U���͑ΐl��ł͂ƂĂ���؂ł��B \n�ϋɓI�Ɂy�R���{�z�ɑg�ݍ���ōs���܂��傤�B");

	// �^�X�N�Z�b�g
	AddTaskData("�W�����v�U���|�󒆂Ł�");
	AddTaskData("���Ⴊ�ݍU���|��+��");
	AddTaskData("�U���|��");
	AddTaskData("�ǌ��|��");
	AddTaskData("�X�L���|��");
	AddTaskData("�K�E�Z�|��");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Airou/1.bin";

	Init();
}

void AirouChallenge1::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// ���ꂼ��̃N���A����
void AirouChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		AERIAL_ATTACK = 0,
		DOWN_ATTACK = 1,
		ATTACK = 2,
		ATTACK2 = 3,
		SKIL = 4,
		OVER_DRIVE = 5,

	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{
		// �󒆍U���𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// ���i�U���𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DOWN_ATTACK);
			}

		}

		// ��i�ڂ𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 0)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ATTACK);
			}

		}

		// ��i�ڂ𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 1)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ATTACK2);
			}

		}

		// �O�i�ڂ𓖂Ă���N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 2)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(SKIL);
			}

		}

		// �K�E�Z�X�e�[�g�ɂ�����N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(OVER_DRIVE);
			}

		}

	}
}

//+-------------------------------
//	�Q�Ԗ�
//+-------------------------------
AirouChallenge2::AirouChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�΋�U��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 120;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("�󒆂̑���ɗL����\n�΋�U������́y�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�������葊��̓��������čU�������񐧂���̂��厖�ł��B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Airou/2.bin";

	// �^�X�N�Z�b�g
	AddTaskData("�΋�U���|��+��");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�Y�̕K�E�Z�|�󒆂Ł�+��");

	Init();
}

void AirouChallenge2::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	// SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::JUMP;


}

// ���ꂼ��̃N���A����
void AirouChallenge2::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		ANTI_AIR,
		JUMP,
		AERIAL_ATTACK,
		AERIAL_JUMP,
		AERIAL_ATTACK2,
		DROP_SKILL,
	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{

		if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ANTI_AIR);
			}

		}

		// �W�����v������N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()))
		{
			TaskSuccess(JUMP);
		}



		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// �󒆃W�����v������N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()))
		{
			TaskSuccess(AERIAL_JUMP);
		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK2);
			}

		}

		// ���󒆁��X�L���𓖂Ă���
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) && 
			pPerson->GetSkillActionType()== SKILL_ACTION_TYPE::AERIALDROP)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}


//+-------------------------------
//	3�Ԗ�
//+-------------------------------
AirouChallenge3::AirouChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�ǂœ������";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 120;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pString[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pString[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("��������́y�R���{�z���o���܂��傤�B\n�ǁ���ʒ[�̎�");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�Ǎۂɂ�蕁�i�ł͌q����Ȃ����͂ȃR���{���ł��܂����B\n������ȊO�ɂ��Ǎۂłł���R���{�͑�R����܂��B\n���񎩕������̃R���{�������܂��傤�B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Airou/3.bin";

	// �^�X�N�Z�b�g
	AddTaskData("�����|�~");
	AddTaskData("�΋�U���|��+��");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�Y�̕K�E�Z�|�󒆂Ł�+��");

	Init();
}

void AirouChallenge3::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
	

}

// ���ꂼ��̃N���A����
void AirouChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		THROW,
		ANTI_AIR,
		JUMP,
		AERIAL_ATTACK,
		AERIAL_JUMP,
		AERIAL_ATTACK2,
		DROP_SKILL,
	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{
		// ����
		if (pPerson->GetFSM()->isInState(*BasePlayerState::ThrowSuccess::GetInstance()) == true)
		{	
			TaskSuccess(THROW);
		}

		if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(ANTI_AIR);
			}

		}

		// �W�����v������N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()))
		{
			TaskSuccess(JUMP);
		}



		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// �󒆃W�����v������N���A
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()))
		{
			TaskSuccess(AERIAL_JUMP);
		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK2);
			}

		}

		// ���󒆁��X�L���𓖂Ă���
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIALDROP)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}
