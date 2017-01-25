#include "TekiChallenge.h"

//+-------------------------------
// �e�L�`�������W
//+-------------------------------

TekiChallenge1::TekiChallenge1(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�ǂɒǂ�����";

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
	m_pIntroTips = new TipsCard("�e�L�͋ߋ������ʒ[�ł̍U�������ɋ����L�����N�^�[�ł��B\n�܂��͑������ʒ[�ɒǂ����ށy�R���{�z���o���܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�U����ړ����x���D�G�ȕ��AHP�������Ⴍ�ł���ア�ł��B\n����Ɏ哱�������点�Ȃ���������S�����܂��傤�B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Teki/1.bin";

	// �^�X�N�Z�b�g
	AddTaskData("�������|�_+��");
	AddTaskData("�L�����b�V���|��");
	AddTaskData("�����Ԋu���J���Ēǌ��|��");

	Init();
}

void TekiChallenge1::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	
}

// ���ꂼ��̃N���A����
void TekiChallenge1::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		_DOWN,
		_SKILL,
		_SKILL2,

	};

	// ����̍d�������������蒼��
	if (pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{

		if (pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_DOWN);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
			}

		}


		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND2)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL2);
			}

		}


	}
}


//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
TekiChallenge2::TekiChallenge2(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�󒆃R���{";

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
	m_pIntroTips = new TipsCard("�󒆂ɂ��鑊���n�ʂɗ��Ƃ��y�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�󒆃R���{�͑傫���_���[�W��^�����̂�\n�ϋɓI�ɑ_���Ă����܂��傤�B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Teki/2.bin";

	// �^�X�N�Z�b�g
	AddTaskData("�΋�U���|��+��");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�Y�̕K�E�Z�|�󒆂Ł�");

	Init();
}

void TekiChallenge2::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::JUMP;


}

// ���ꂼ��̃N���A����
void TekiChallenge2::TaskUpdate(BasePlayer * pPerson)
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

		// ���󒆃X�L���𓖂Ă���
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}




//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
TekiChallenge3::TekiChallenge3(int iTitleNo) : BaseChallenge(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "��ʒ[�R���{";

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
	m_pIntroTips = new TipsCard("����𕂂����󒆃R���{�֌q����y�R���{�z���o���܂��傤�B\n");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n����ł������i�Z�̑���������\n����ɑ傫���_���[�W��^�����̂Ŕ��ɋ��͂ł��B");

	// ���v���C�f�[�^�̖��O
	m_strReplayDataName = "Data/ChallengeData/Teki/3.bin";

	// �^�X�N�Z�b�g
	AddTaskData("�������|�_+��");
	AddTaskData("�L�����b�V���|��");
	AddTaskData("�U���|��");
	AddTaskData("�ǉ��U���|��");
	AddTaskData("�΋�U���|��+��");

	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�W�����v�L�����Z���|��");
	AddTaskData("�󒆍U���|�󒆂Ł�");
	AddTaskData("�Y�̕K�E�Z�|�󒆂Ł�");

	Init();
}

void TekiChallenge3::Init(int iDeviceID)
{
	// ������
	BaseChallenge::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.eStartPosition = START_POSITION::RIGHT;

}

// ���ꂼ��̃N���A����
void TekiChallenge3::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;




	//�����ŐF�X�N���A�����撣��I
	enum
	{
		_DOWN,
		_SKILL,
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
		if (pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_DOWN);
			}

		}

		// 
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) &&
			pPerson->GetSkillActionType() == SKILL_ACTION_TYPE::LAND)
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(_SKILL);
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
		if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()))
		{
			if (pPerson->isHitAttack() == true)
			{
				TaskSuccess(DROP_SKILL);
			}

		}

	}
}

