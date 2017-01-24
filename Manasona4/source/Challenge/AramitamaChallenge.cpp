#include "AramitamaChallenge.h"

//+-------------------------------
// �e�L�`�������W
//+-------------------------------

AramitamaChallenge1::AramitamaChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "���t�@�~���[";

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
	m_pIntroTips = new TipsCard("�A���~�^�}�̓X�L���𓖂Ă邱�Ƃō��Q�[�W���������܂��B\n���Q�[�W�����܂�ƃX�L���̏I��ɒ��Ԃ�������U�����܂��B\n���ԂƋ��͂���y�R���{�z���o���܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�A���~�^�}�͍��Q�[�W�����܂�܂ł͎ア�ł����B\n���Q�[�W�����܂�����͍ŋ��ł��B");

	// �^�X�N�Z�b�g
	AddTaskData("�X�L���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�󒆉�]�U���|�󒆂Ł�");

	Init();
}

void AramitamaChallenge1::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	// �Q�[�WMAX�Œ�
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::MAX;
}

// ���ꂼ��̃N���A����
void AramitamaChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		_SKILL,
		AERIAL_ATTACK,
		AERIAL_SKILL,
	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{

		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_ATTACK);
			}

		}

		// ���󒆃X�L���𓖂Ă���
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIAL)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_SKILL);
			}

		}


	}
}


//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
AramitamaChallenge2::AramitamaChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�⏕�Z���E�E";

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
	m_pIntroTips = new TipsCard("�⏕�Z�������Z�͍��Q�[�W�𗭂܂�Β��Ԃ��g��\n�U���Z�ɂȂ�܂��B���킵�Ă݂܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n��肭�g�������|�M���܂��傤�B");

	// �^�X�N�Z�b�g
	AddTaskData("�X�L���|��");
	AddTaskData("�󒆉���|�󒆂Ł�+��");
	AddTaskData("�`���[�W�|��+��");

	Init();
}

void AramitamaChallenge2::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	// �Q�[�WMAX�Œ�
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::MAX;
}

// ���ꂼ��̃N���A����
void AramitamaChallenge2::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		SKILL,
		AERIAL_SKILL,
		CHARGE
	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(SKILL);
			}

		}

		// ���󒆃X�L��
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIALDROP)
		{
		
			TaskSuccess(AERIAL_SKILL);		

		}

		// �`���[�W
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{
		
			TaskSuccess(CHARGE);

		}
	}
}

//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
AramitamaChallenge3::AramitamaChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�~�^�}�t�@�C�i��";

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
	m_pIntroTips = new TipsCard("���ԒB�Ƌ��͂��镡�G�ȁy�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n���Q�[�W�𗭂ߋ��ق̔j��͂ő�������|���܂��傤�B");

	// �^�X�N�Z�b�g
	AddTaskData("�X�L���|��");

	AddTaskData("�΋�U���|��+��");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�󒆉�]�U���|�󒆂Ł�");

	AddTaskData("�`���[�W�|��+��");

	AddTaskData("�~�^�}�L���m���|��");

	Init();
}

void AramitamaChallenge3::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	// �Q�[�WMAX�Œ�
	SelectDataMgr->Get()->tagTrainingDatas.eWassyoiType = WASSYOI_TYPE::MAX;
	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// ���ꂼ��̃N���A����
void AramitamaChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;




	//�����ŐF�X�N���A�����撣��I
	enum
	{
		_SKILL,
		_ANTIAIR,
		JUMP,
		AERIAL_ATTACK,
		AERIAL_JUMP,
		AERIAL_ATTACK2,
		AERIAL_SKILL,
		CHARGE,
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
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
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

		// ���󒆃X�L���𓖂Ă���
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::AERIAL)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(AERIAL_SKILL);
			}

		}

		// �`���[�W
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::SQUAT)
		{

			TaskSuccess(CHARGE);

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

