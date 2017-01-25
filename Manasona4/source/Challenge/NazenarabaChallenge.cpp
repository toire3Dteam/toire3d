#include "NazenarabaChallenge.h"

//+-------------------------------
// (��)�`�������W
//+-------------------------------

NazenarabaChallenge1::NazenarabaChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "������������";

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
	m_pIntroTips = new TipsCard("(��)�͒������̗�����肪���ɋ����L�����N�^�[�ł��B\n�܂��͒���������q����y�R���{�z���o���܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n(��)�͋ߋ����ł̐킢�͋��ł��B\n������񂹕t���Ȃ���������S�����܂��傤�B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Nazenaraba/1.bin";

	// �^�X�N�Z�b�g
	AddTaskData("����𕂂����|��+��");
	AddTaskData("�����Ă鑊�����������|��");

	AddTaskData("�߂Â���������U���|��");
	AddTaskData("�ǌ��|��");
	AddTaskData("�X�L���|��");
	AddTaskData("�K�E�Z�|��");

	Init();
}

void NazenarabaChallenge1::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);


	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
}

// ���ꂼ��̃N���A����
void NazenarabaChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		_SKILL,
		_SKILL2,
		ATTACK ,
		ATTACK2 ,
		SKIL ,
		OVER_DRIVE,
	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{
		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
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


//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
NazenarabaChallenge2::NazenarabaChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�����ĉ�������";

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
	m_pIntroTips = new TipsCard("�����y�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�߂Â��Ă�������͓�����\n�����̓��ӂȋ����Ɏ����Ă����܂��傤�B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Nazenaraba/2.bin";

	// �^�X�N�Z�b�g
	AddTaskData("�����|�~");

	AddTaskData("�����Ă鑊�����������|��");

	AddTaskData("�߂Â���������U���|��");
	AddTaskData("�ǌ��|��");
	AddTaskData("�X�L���|��");
	AddTaskData("�K�E�Z�|��");

	Init();
}

void NazenarabaChallenge2::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);


	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
}

// ���ꂼ��̃N���A����
void NazenarabaChallenge2::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		THROW,
		_SKILL2,
		ATTACK,
		ATTACK2,
		SKIL,
		OVER_DRIVE,
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



		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
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




//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
NazenarabaChallenge3::NazenarabaChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "(��)�R���{";

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
	m_pIntroTips = new TipsCard("�X�L������󒆃R���{�֌q����y�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n����ɑ傫���_���[�W��^�����\n�y�R���{�z�Ȃ̂Ŕ��ɋ��͂ł��B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Nazenaraba/3.bin";

	// �^�X�N�Z�b�g
	AddTaskData("����𕂂����|��+��");
	AddTaskData("�����Ă鑊�����������|��");

	AddTaskData("�߂Â���������U���|��");
	AddTaskData("�ǌ��|��");

	AddTaskData("�΋�U���|��+��");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�Y�̕K�E�Z�|�󒆂Ł�+��");

	Init();
}

void NazenarabaChallenge3::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);


}

// ���ꂼ��̃N���A����
void NazenarabaChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;




	//�����ŐF�X�N���A�����撣��I
	enum
	{
		_SKILL,
		_SKILL2,
		_ATTACK1,
		_ATTACK2,
		_ANTIAIR,
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
		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
			}

		}


		// �ʏ�U��
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 0)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_ATTACK1);
			}

		}

		//
		if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) &&
			pPerson->GetRushStep() == 1)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_ATTACK2);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_ANTIAIR);
			}

		}


		//+----------------------------------------------------------------
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

