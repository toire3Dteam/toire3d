#include "Tutorial.h"
#include "Sound\SoundManager.h"

//+-----------------------------
//	�x�[�X
//+-----------------------------

BaseTutorial::BaseTutorial(int iTitleNo)
{
	m_iTitleNo = iTitleNo;
	// �^�C�g����
	m_pTaskTitle.pString = "No.";
	
	// 10���ȉ��Ȃ�
	if (m_iTitleNo < 10)
	{
		m_pTaskTitle.pString += "0";// 0�ǉ�
	}
	m_pTaskTitle.pString += std::to_string(m_iTitleNo) + " ";

	
	m_pTaskTitle.bClear = false;

	m_iWaitFrame = 0;
	m_iWaitFrameMAX = 0;
	m_bClearFlag = false;

	m_pTaskIcon = new tdn2DObj("Data/UI/Tutorial/TaskIcon.png"); 
	m_bAllTaskClear = false;

	m_pIntroTips = nullptr;
	m_pClearTips = nullptr;;

	// �^�X�N�ꗗ
	m_aTask.clear();
	m_vTaskPos.x = 50;
	m_vTaskPos.y = 250;



	// �Z���N�g�i���o�[
	m_iSelectTask = 0;

	m_pClearPic = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPic->OrderAlphaMove(55, 6, 45);
	
	m_pClearPicRip = new tdn2DAnim("Data/UI/Tutorial/Clear.png");
	m_pClearPicRip->OrderRipple(14, 1, 0.025f);

	m_iDeviceID = 0;
}

BaseTutorial::~BaseTutorial()
{
	SAFE_DELETE(m_pTaskIcon);
	SAFE_DELETE(m_pIntroTips);
	SAFE_DELETE(m_pClearTips);
	SAFE_DELETE(m_pClearPic);
	SAFE_DELETE(m_pClearPicRip);

	m_aTask.clear();	
}

void BaseTutorial::Init(int iDeviceID)
{
	m_iWaitFrame = m_iWaitFrameMAX;
	m_bClearFlag = false;


	// �N���A�t���O����������
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		m_aTask[i].bClear = false;
	}

	m_iSelectTask = 0;

	m_iDeviceID = iDeviceID;

	// �O��̉��o������
	m_pClearPic->Stop();
	m_pClearPicRip->Stop();

	// �����̏󋵐ݒ�
	// �K�v�ȏ󋵂�����Ȃ�e������������
	SelectDataMgr->Get()->tagTrainingDatas.eHpRecovery = HP_RECOVERY_TYPE::AUTO_RECOVERY;
	SelectDataMgr->Get()->tagTrainingDatas.iHp1P=100;
	SelectDataMgr->Get()->tagTrainingDatas.iHp2P=100;
	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 0;
	SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery = PARTNER_RECOVERY_TYPE::DEFAULT;
	SelectDataMgr->Get()->tagTrainingDatas.eTrainingInfo = TRAINING_INFO_TYPE::NONE;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState=ENEMY_STATE_TYPE::STAND;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard=ENEMY_GUARD_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch = ENEMY_GUARD_SWITCH_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech = ENEMY_TECH_TYPE::ALL;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech = ENEMY_THROW_TECH_TYPE::NO;
	SelectDataMgr->Get()->tagTrainingDatas.iEnemyLv = 20;

}

void BaseTutorial::Update()
{	
	// �N���A���
	if (m_bClearFlag == true)
	{
		// �J�E���g����
		m_iWaitFrame--;
		if (m_iWaitFrame <= 0)
		{
			// ���̎��_�� EndTipsStart()�֐��ŏ󋵂�؂�ւ���
			m_iWaitFrame = 0;
		}
	}
	else
	{
		// �^�X�N���S���N���A����Ă邩�`�F�b�N�I
		int iClearNum = 0;
		for (int i = 0; i < (int)m_aTask.size(); i++)
		{
			if (m_aTask[i].bClear == true)
			{
				iClearNum++;
			}

			// �����N���A�����^�X�N���Ɠ����ɂȂ�����
			if ((int)m_aTask.size() == iClearNum)
			{
				// ���b�X���N���A�I
				ActionClear();
			}
		}
	}


	
	// Tips
	m_pIntroTips->Update(m_iDeviceID);
	m_pClearTips->Update(m_iDeviceID);

	// �N���A���o�p
	m_pClearPic->Update();
	m_pClearPicRip->Update();

}

void BaseTutorial::Render()
{

	// �^�X�N�^�C�g��
	for (int j = 0; j < m_pTaskTitle.iStingLength + 3/*����蒷��*/; j++)
		m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8 - 16 , (int)m_vTaskPos.y - 40 - 4, 32, 32, 0, 0, 32, 32);
	tdnFont::RenderString(m_pTaskTitle.pString.c_str(), "HG�޼��E", 22, (int)m_vTaskPos.x+16, (int)m_vTaskPos.y - 40 , 0xffffffff, RS::COPY);


	// �^�X�N��
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		for (int j = 0; j < m_aTask[i].iStingLength; j++)
		{
			m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4, 32, 32, 32 * m_aTask[i].bClear , 0, 32, 32);
		
			// �I�����Ă���^�X�N�Ȃ�
			if (m_iSelectTask == i)
			{
				m_pTaskIcon->Render((int)m_vTaskPos.x + (j * 11) - 8, (int)m_vTaskPos.y + (i * 40) - 4,
					32, 32, 0, 32, 32, 32);
			}
		
		}

		DWORD fontCol = 0xffffffff;
		if (m_iSelectTask == i)fontCol = 0xff222222;
		tdnFont::RenderString(m_aTask[i].pString.c_str(), "HG�޼��E", 22, (int)m_vTaskPos.x, (int)m_vTaskPos.y + (i * 40), fontCol, RS::COPY);
	}

	// TIPS
	m_pIntroTips->Render();
	m_pClearTips->Render();

	// �N���A���o�p
	m_pClearPic->Render(0, 0);
	m_pClearPicRip->Render(0, 0, RS::ADD);
}

void BaseTutorial::ActionIntroTips()
{
	//�A�N�V�����I���͉摜�o������
	m_pIntroTips->Action();

}
void BaseTutorial::StopIntroTips()
{
	m_pIntroTips->End();
}

void BaseTutorial::ActionClearTips()
{
	//�A�N�V�����I���͉摜�o������
	m_pClearTips->Action();

}
void BaseTutorial::StopClearTips()
{
	m_pClearTips->End();
}

void BaseTutorial::ActionClear()
{
	m_bClearFlag = true;

	// ���ŃN���A���o
	m_pClearPic->Action();
	m_pClearPicRip->Action();

	// SE
	se->Play("�I�[���N���A");
}


//void BaseTutorial::TaskReset()
//{
//	// �N���A�t���O����������
//	for (int i = 0; i < (int)m_aTask.size(); i++)
//	{
//		m_aTask[i].bClear = false;
//	}
//
//	m_iSelectTask = 0;
//
//}


void BaseTutorial::AddTaskData(LPSTR string)
{
	Task data;
	data.pString = string;
	data.bClear = false;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0;
	UINT	addByte = 0;
	//	�I�[�����܂Ń��[�v
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&string[i]);
		addByte += myByte;
	}
	data.iStingLength = addByte;

	m_aTask.push_back(data);

}

void BaseTutorial::TaskSuccess(int select)
{
	// ��������v���Ă�����̂̂ݔ��� ����ɂ�菇�Ԓʂ�^�X�N�����Ȃ��K�v������
	if (select == m_iSelectTask)
	{

		if ((int)m_aTask.size() == m_iSelectTask)
		{
			assert(0);// �T�C�Y�����߂�
		}

		m_aTask[m_iSelectTask].bClear = true;
		m_iSelectTask++;

		// SE
		se->Play("�X�e�b�v�N���A");
	}
}

//+------------------------------------------------------
//	��������
//+------------------------------------------------------

WalkTutorial::WalkTutorial(int iTitleNo):BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "����";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�܂��͈ړ��̊�{�A�u�����v�ł��B\n�����L�[�ŃL�����N�^�[�𓮂����Ă݂܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�@����̕����֕����L�[�������Ɓu�O�����v�A\n����̋t�����։����Ɓu�������v�ɂȂ�܂��B");


	// �^�X�N�Z�b�g
	AddTaskData("�O�����|���������ςȂ�");
	AddTaskData("�������|���������ςȂ�");

	Init();
}

void WalkTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	m_iWalkFrame = 0;
	m_iBackFrame = 0;
}


//void WalkTutorial::Update()
//{
//	// ������
//	BaseTutorial::Update(); 
//}
//
//void WalkTutorial::Render()
//{
//	// ������
//	BaseTutorial::Render();
//
//
//	//tdnFont::RenderFont2D(str, 32, 200, 600, 0xffffffff);
//
//}

// ���ꂼ��̃N���A����
void WalkTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum 
	{
		WALK = 0, BACK = 1
	};

	// �`�t���[�������X�e�[�g�ɂ�����ŏ��N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()))
	{
		m_iWalkFrame++;

		if (m_iWalkFrame >= 20)
		{
			TaskSuccess(WALK);
		}
	}
	else
	{
		m_iWalkFrame = 0;
	}

	// �`�t���[���������X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::BackWalk::GetInstance()))
	{
		m_iBackFrame++;

		if (m_iBackFrame >= 20)
		{
			TaskSuccess(BACK);
		}
	}
	else
	{
		m_iBackFrame = 0;
	}

}


//+------------------------------------------------------
//	���Ⴊ�ݐ���
//+------------------------------------------------------

SquatTutorial::SquatTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "���Ⴊ��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;
	
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
	m_pIntroTips = new TipsCard("���̓L�����N�^�[�����Ⴊ�܂��Ă݂܂��傤�B\n���ł��̏�ł��Ⴊ�݂܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�^��_�Ɠ��͂��Ă��A���Ⴊ�ނ��Ƃ��o���܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("���Ⴊ�݁|���������ςȂ�");

	Init();
}

void SquatTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	m_iSquatFrame = 0;
}

// ���ꂼ��̃N���A����
void SquatTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum
	{
		SQUAT = 0
	};

	// �`�t���[�����Ⴊ�݃X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()))
	{
		m_iSquatFrame++;

		if (m_iSquatFrame >= 20)
		{
			TaskSuccess(SQUAT);
		}
	
	}else
	{
		m_iSquatFrame = 0;
	}


}


//+------------------------------------------------------
//	�W�����v�̐���
//+------------------------------------------------------

JumpTutorial::JumpTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�W�����v";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�u�W�����v�v�Ɓu�󒆃W�����v�v�ł��B\n�n��Ł����������ƂŁu�W�����v�v�����܂��B\n�󒆂ł�����x���������Ɓu�󒆃W�����v�v�ɂȂ�܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�W�����v�v�́��⁨�Ɠ��͂��邱�ƂŁA\n�u�O�W�����v�v��u���W�����v�v�ɂȂ�܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("�W�����v�|��");
	AddTaskData("�󒆃W�����v�|�󒆂Ł�");

	Init();
}

void JumpTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);
}

// ���ꂼ��̃N���A����
void JumpTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum
	{
		JUMP = 0, AERIAL_JUMP = 1
	};

	// �W�����v�X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()))
	{
		TaskSuccess(JUMP);
	}

	// �󒆃W�����v�X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()))
	{
		TaskSuccess(AERIAL_JUMP);
	}

}


//+------------------------------------------------------
//	�U���̐���
//+------------------------------------------------------

AttackTutorial::AttackTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�U��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	//m_pIntroTips = new TipsCard("�����A���x�͊�{�̍U�����@���w�т܂��傤�B\n���̏�Ł��A���Ⴊ��Ł��A�W�����v���Ɂ��A\n����Ƌt�����Ł��������Ă݂܂��傤�B");
	m_pIntroTips = new TipsCard("�����A���x�͊�{�̍U�����@���w�т܂��傤�B\n���̏�Ł��������Ă݂܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�����L�[�Ƃ̑g�ݍ��킹�ŗl�X�ȍU�����J��o���܂��B");// \n�u�΋�U���v�͋󒆂���̍U���ɑ΂��āy���G�z�ł��B

	// �^�X�N�Z�b�g
	AddTaskData("�U���|��");
	//AddTaskData("���Ⴊ�ݍU���|���{��");
	//AddTaskData("�W�����v�U���|�󒆂Ł�");
	//AddTaskData("�΋�U���|���{��");


	Init();
}

void AttackTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

}

// ���ꂼ��̃N���A����
void AttackTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum
	{
		ATTACK = 0, SQUAT_ATTACK = 1, JUMP_ATTACK = 2, ANTIAIR_ATTACK = 3
	};

	// �U���X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack()==true)
		{
			TaskSuccess(ATTACK);
		}	
	}

	// ���Ⴊ�ݍU���X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			//TaskSuccess(SQUAT_ATTACK);
			TaskSuccess(ATTACK);
		}
	}

	// �󒆍U���X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			//TaskSuccess(JUMP_ATTACK);
			TaskSuccess(ATTACK);
		}
	}

	// �΋�U���X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			//TaskSuccess(ANTIAIR_ATTACK);
			TaskSuccess(ATTACK);
		}
	}
}

//+------------------------------------------------------
//	�X�L���U���̐���
//+------------------------------------------------------

SkillTutorial::SkillTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�X�L��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�u�X�L���v�̓L�����N�^�[���ɂ��鋭�͂ȋZ�ł��B\n���{�^���ŃX�L���𔭓��ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�X�L���v�̓L�����N�^�[���ɗl�X�ȋZ������܂��B\n�܂��u�U���v�����{�I�Ɍq���Ĕ����ł��܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("�X�L���|��");

	Init();
}

void SkillTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

}

// ���ꂼ��̃N���A����
void SkillTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	enum
	{
		SKILL = 0
	};

	// �X�L���X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(SKILL);
		}

	}

}


//+------------------------------------------------------
//	�K�E�Z�̐���
//+------------------------------------------------------

OverDriveTutorial::OverDriveTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�K�E�Z";

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
	m_pIntroTips = new TipsCard("�Q�[�W��50�ȏ゠�鎞�ɇ����������ނƁB\n�u�K�E�Z�v�𔭓��ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�K�E�Z�v�͂ƂĂ����͂ȋZ�ł��B\n�܂��u�U���v��u�X�L���v����q���Ĕ����ł��܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("�K�E�Z�|�Q�[�W50%�ȏ�Ň�");

	Init();
}

void OverDriveTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// ���ꂼ��̃N���A����
void OverDriveTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	enum
	{
		OVER_DRIVE = 0
	};

	// �K�E�Z�X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()))
	{
		//if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(OVER_DRIVE);
		}

	}

}

//+------------------------------------------------------
//	���b�V���U���̐���
//+------------------------------------------------------

RushTutorial::RushTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�A�ŃR���{";

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
	m_pIntroTips = new TipsCard("�u�A�ŃR���{�v�́u�U���v�𑱂��邱�ƂŁA\n�y�R���{�z���q�����Ă����V�X�e���ł��B\n����ɋ߂Â��A���{�^����A�ł��Ă݂܂��傤�I");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n���{�^����A�ł��邾���Łu�X�L���v�Ɍq����A\nSP�Q�[�W������΁A�����I�Ɂu�K�E�Z�v�܂Ōq���Ă���܂�\n���߂Ă̕��́A�܂��u�A�ŃR���{�v�Ő���Ă݂܂��傤�B");

	// �^�X�N�Z�b�g
	AddTaskData("�U���|��");
	AddTaskData("�ǌ��|��");
	AddTaskData("�X�L���|��");
	AddTaskData("�K�E�Z�|��");

	Init();
}

void RushTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;

}

// ���ꂼ��̃N���A����
void RushTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;	


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		ATTACK = 0,
		ATTACK2 = 1,
		SKIL = 2,
		OVER_DRIVE = 3,

	};

	// �ҋ@�ɖ߂�ƍŏ������蒼��
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) )
		//pPerson->GetTargetPlayer()->GetRecoveryFrame() <= 0)
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
	}
	else
	{


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


//+------------------------------------------------------
//	�����K�[�h�̐���
//+------------------------------------------------------

StandGuardTutorial::StandGuardTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�����K�[�h";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�U���̎��͎��ł��B�u�����K�[�h�v���s���܂��傤\n����Ƌt�����ɕ����L�[���������ςȂ��ɂ��āA\n3��A�U����h���܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�����K�[�h�v�̓W�����v�U����u���i�U���v�Ȃǂ�h���܂��B\n�������u���Ⴊ�ݍU���v��u�������v�Ȃǂ�\n�y���i�Z�z�͖h���Ȃ��̂Œ��ӂ��K�v�ł��B");

	// �^�X�N�Z�b�g
	AddTaskData("�����K�[�h�|���������ςȂ�");
	AddTaskData("�����K�[�h�|���������ςȂ�");
	AddTaskData("�����K�[�h�|���������ςȂ�");

	Init();
}

void StandGuardTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::T_DOKKOI_ATTACK;

	m_bStop = false;
	m_iCount = 0;
}

// ���ꂼ��̃N���A����
void StandGuardTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		SUCCESS1 = 0,
		SUCCESS2 = 1,
		SUCCESS3 = 2,
	};

	// ���������ꍇ�K�[�h��Ԃ������Ă��玟�̃^�X�N��
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == false)
	{
		m_bStop = false;
	}

	// �K�[�h������N���A
	if (m_bStop == false && 
		pPerson->isGuardSuccess() == true)
	{
		//for (int i = 0; i < m_aTask.size(); i++)
		//{

		//}
		if (m_iCount == 0)		TaskSuccess(SUCCESS1);
		else if (m_iCount == 1)	TaskSuccess(SUCCESS2);
		else if (m_iCount == 2)	TaskSuccess(SUCCESS3);

		m_bStop = true;
		m_iCount++;
	}


}



//+------------------------------------------------------
//	���Ⴊ�݃K�[�h�̐���
//+------------------------------------------------------

SquatGuardTutorial::SquatGuardTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "���Ⴊ�݃K�[�h";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�u���Ⴊ�݃K�[�h�v�͒n���̊�{�ł�!\n����Ƌt�����Ɏ΂߉��ɕ����L�[���������ςȂ��ɂ��āA\n3��A�U����h���܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u���Ⴊ�݃K�[�h�v�̓W�����v�U���Ȃǂ̋󒆂��痈��U���� \n�u���i�U���v�Ȃǂ͖h���Ȃ��ł����A\n�n��̑���ɑ΂��ẮA�w�ǂ̍U����h���܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("���Ⴊ�݃K�[�h�|�^�������ςȂ�");
	AddTaskData("���Ⴊ�݃K�[�h�|�^�������ςȂ�");
	AddTaskData("���Ⴊ�݃K�[�h�|�^�������ςȂ�");

	Init();
}

void SquatGuardTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 50;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::T_DOWN_ATTACK;

	m_bStop = false;
	m_iCount = 0;
}

// ���ꂼ��̃N���A����
void SquatGuardTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		SUCCESS1 = 0,
		SUCCESS2 = 1,
		SUCCESS3 = 2,
	};

	// ���������ꍇ�K�[�h��Ԃ������Ă��玟�̃^�X�N��
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == false)
	{
		m_bStop = false;
	}

	// �K�[�h������N���A
	if (m_bStop == false &&
		pPerson->isGuardSuccess() == true)
	{
		//for (int i = 0; i < m_aTask.size(); i++)
		//{

		//}
		if (m_iCount == 0)		TaskSuccess(SUCCESS1);
		else if (m_iCount == 1)	TaskSuccess(SUCCESS2);
		else if (m_iCount == 2)	TaskSuccess(SUCCESS3);

		m_bStop = true;
		m_iCount++;
	}

}


//+------------------------------------------------------
//	����̐���
//+------------------------------------------------------

EscapeTutorial::EscapeTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "���";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�u����v�͒n��ōU����O���ֈړ����������s���ł��B\n���{�^���������Ă݂܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u����v�ňړ����Ă���Ԃ́A\n�y�����Z�z�ȊO�̍U���ɑ΂��āy���G��ԁz�ł��B");

	// �^�X�N�Z�b�g
	AddTaskData("����|��");

	Init();
}

void EscapeTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);


}

// ���ꂼ��̃N���A����
void EscapeTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		ESCAPE = 0,
	};

	// ��������琬��
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Escape::GetInstance()) == true)
	{
		TaskSuccess(ESCAPE);
	}

}



//+------------------------------------------------------
//	�����̐���
//+------------------------------------------------------

ThrowTutorial::ThrowTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "����";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("����ɋ߂Â��āB�~��������\n�n��ɂ��鑊����u�����v�邱�Ƃ��o���܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�����v�͒͂܂����������Ɂ~�{�^���������y���������z\n�ł����K�[�h���鎖���o���܂���B\n�K�[�h����肢����ɂ́A�u�����v�����ʓI�ł��B");

	// �^�X�N�Z�b�g
	AddTaskData("�����|�~");

	Init();
}

void ThrowTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);


}

// ���ꂼ��̃N���A����
void ThrowTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		THROW = 0,
	};

	// ���������琬��
	if (pPerson->GetFSM()->isInState(*BasePlayerState::ThrowHold::GetInstance()) == true)
	{
		TaskSuccess(THROW);
	}

}


//+------------------------------------------------------
//	���G�Z�̐���
//+------------------------------------------------------

InvincibleTutorial::InvincibleTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "���G�Z";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("���G�Z�͑S�L�����N�^�[�Ŏg����\n����̍U�����󂯂Ȃ��y���G��ԁz�̂���U���ł��B\n���������Ĕ����ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u���G�Z�v�͍U����h�����͂ȋZ�ł��B�������A�͂�����\n�v���I�Ȍ������܂��̂Ŏg�����ɂ͒��ӂ��K�v�ł��B");

	// �^�X�N�Z�b�g
	AddTaskData("���G�Z�|��");

	Init();
}

void InvincibleTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);


}

// ���ꂼ��̃N���A����
void InvincibleTutorial::TaskUpdate(BasePlayer * pPerson)
{
	// �N���A���Ă�����n�W�N
	if (m_bClearFlag == true)return;


	//�����ŐF�X�N���A�����撣��I
	enum
	{
		INVINCIBLE = 0,
	};

	// ���G�Z�q�b�g
	if (pPerson->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) == true)
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(INVINCIBLE);
		}

	}

}



//+------------------------------------------------------
//	�������
//+------------------------------------------------------

DushTutorial::DushTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�_�b�V��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("���H�I�Ȉړ����@�u�_�b�V���v�ł��B\n��������Ɍ������ă^�E�^���Ɠ�񁨉��������܂��傤�B\n�������ςȂ��Ń_�b�V���𑱂��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�@�u�_�b�V���v�͑���ɋ߂Â����̊�{�ƂȂ�܂��B");


	// �^�X�N�Z�b�g
	AddTaskData("�_�b�V���|�f���������������ςȂ�");

	Init();
}

void DushTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	m_iDushFrame = 0;
}

// ���ꂼ��̃N���A����
void DushTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	
	enum
	{
		DUSH = 0,
	};

	// �`�t���[������X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Run::GetInstance()))
	{
		m_iDushFrame++;

		if (m_iDushFrame >= 20)
		{
			TaskSuccess(DUSH);
		}
	}
	else
	{
		m_iDushFrame = 0;
	}

}


//+------------------------------------------------------
//	�o�b�N�X�e�b�v����
//+------------------------------------------------------

BackStepTutorial::BackStepTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�o�b�N�X�e�b�v";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�u�o�b�N�X�e�b�v�v�ł��B\n����Ƃ͋t�����Ɍ������ă^�E�^���Ɠ�񁩉����܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n����̍U�����������A��������肽�����Ɏg�p���܂��傤�B");


	// �^�X�N�Z�b�g
	AddTaskData("�o�b�N�X�e�b�v�|�f��������");

	Init();
}

void BackStepTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

}

// ���ꂼ��̃N���A����
void BackStepTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum
	{
		BACK_STEP = 0,
	};

	// �o�b�N�X�e�b�v������N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::BackStep::GetInstance()))
	{
		TaskSuccess(BACK_STEP);
	}

}


//+------------------------------------------------------
//	�p�[�g�i�[����
//+------------------------------------------------------

PartnerTutorial::PartnerTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�p�[�g�i�[�X�L��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�����̑I�񂾃p�[�g�i�[���g�p����u�p�[�g�i�[�X�L���v�ł�\n�u�p�[�g�i�[�Q�[�W�v���ő�܂ŗ��܂��Ă����Ԃ�\n���{�^�����������ƂŔ����ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�@�y�p�[�g�i�[�z�͎g�p�ł���܂Ŏ��Ԃ͂�����܂����A\n�ƂĂ����͂ȋZ���s���Ă���܂��B\n�����ɂ������y�p�[�g�i�[�z�������܂��傤�B");

	// �^�X�N�Z�b�g
	AddTaskData("�p�[�g�i�[�X�L���|P�Q�[�W�ő�Ł�");

	Init();
}

void PartnerTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.ePartnerRecovery = PARTNER_RECOVERY_TYPE::MAX;

}

// ���ꂼ��̃N���A����
void PartnerTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum
	{
		PARTNER_ATTACK = 0,
	};

	// �p�[�g�i�[�����ŃN���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::StandAction::GetInstance()))
	{
		if (pPerson->GetStand()->isHit() == true)
		{
			TaskSuccess(PARTNER_ATTACK);
		}
	
	}

}


//+------------------------------------------------------
//	���i�U������
//+------------------------------------------------------

DokkoiTutorial::DokkoiTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "���i�U��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("�u���i�U���v�́u���Ⴊ�݃K�[�h�v�ł͖h���Ȃ����͂Ȉꌂ�ł�\n���{�^���ōU�����q�b�g��������A\n�������{�^���������Ɓy�_�b�V���L�����Z���z���ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n��������֐i�ށy�_�b�V���L�����Z���z����\n�u�U���v��u�X�L���v�ȂǂŒǌ����ł��܂��B");


	// �^�X�N�Z�b�g
	AddTaskData("���i�U���|��");
	AddTaskData("�_�b�V���L�����Z���|���i�U��HIT�つor��");

	Init();
}

void DokkoiTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

}

// ���ꂼ��̃N���A����
void DokkoiTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	enum
	{
		DOKKOI = 0, DUSH_CHANCEL = 1
	};

	// ���i�Z
	if (pPerson->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()))
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(DOKKOI);
		}
	}

	// �_�b�V���L�����Z��
	if (pPerson->GetActionState() == BASE_ACTION_STATE::FRAMECOUNT)
	{
		TaskSuccess(DUSH_CHANCEL);
	}
}


//+------------------------------------------------------
//	����������
//+------------------------------------------------------

DownAttackTutorial::DownAttackTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "������";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

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
	m_pIntroTips = new TipsCard("����������Ⴊ�݂Ł��{�^���������Ɓu�������v�ɂȂ�܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�������v�͑����K���_�E����������y���i�Z�z�ł��B\n��肭�_�E�������邱�ƂŃo�g���̗�����Ăэ��݂܂��傤�B");


	// �^�X�N�Z�b�g
	AddTaskData("�������|�_+��");

	Init();
}

void DownAttackTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

}

// ���ꂼ��̃N���A����
void DownAttackTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	enum
	{
		DOWN_ATTACK = 0,
	};

	// �p�[�g�i�[�����ŃN���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()))
	{
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(DOWN_ATTACK);
		}
	}

}


//+------------------------------------------------------
//	�o������
//+------------------------------------------------------

OverDriveOneMoreTutorial::OverDriveOneMoreTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�o��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 90;

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
	m_pIntroTips = new TipsCard("SP�Q�[�W��100�����Ԃ�\n���{�^���������Ɓu�o���v���������܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�o���v���͎��ԂƂƂ���SP�Q�[�W���������Ă����܂����A\n�u�K�E�Z�v�����ł������ł��܂��B����ɂ͔������ɋZ�̌���\n���������ł��邽�߁y�R���{�z�ő�_���[�W��_�����Ƃ��ł��܂�");

	// �^�X�N�Z�b�g
	AddTaskData("�o���|��");

	Init();
}

void OverDriveOneMoreTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 100;
}

// ���ꂼ��̃N���A����
void OverDriveOneMoreTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	enum
	{
		OD = 0,
	};

	// �����ŃN���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()))
	{
	
		TaskSuccess(OD);
		
	}

}


//+------------------------------------------------------
//	�o�[�X�g�o������
//+------------------------------------------------------

OverDriveBurstTutorial::OverDriveBurstTutorial(int iTitleNo) : BaseTutorial(iTitleNo)
{
	// �^�C�g����
	m_pTaskTitle.pString += "�o�[�X�g�o��";

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
	m_pIntroTips = new TipsCard("SP�Q�[�W��100�����Ԃő���̍U�����󂯂Ă��鎞��\n���{�^���������Ƒ���𐁂���΂��u�o���v����\n�u�o�[�X�g�o���v���������܂��B");
	m_pClearTips = new TipsCard("�����l�ł����I\n�ȏ�Ń`���[�g���A���͏I���ł��B");
	//m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�o�[�X�g�o���v�͑���𐁂���΂��u�o���v���܂�\n���ɋ��͂ł����u�o���v����SP�Q�[�W�������������Ă����܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("�o�[�X�g�o���|�_���[�W���󂯂Ă���Œ��Ɂ�");

	Init();
}

void OverDriveBurstTutorial::Init(int iDeviceID)
{
	// ������
	BaseTutorial::Init(iDeviceID);

	SelectDataMgr->Get()->tagTrainingDatas.iSpGage = 100;
	SelectDataMgr->Get()->tagTrainingDatas.eEnemyState = ENEMY_STATE_TYPE::T_DOKKOI_ATTACK;

}

// ���ꂼ��̃N���A����
void OverDriveBurstTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I
	enum
	{
		OD = 0,
	};

	// �����ŃN���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_Burst::GetInstance()))
	{
		TaskSuccess(OD);

	}

}
