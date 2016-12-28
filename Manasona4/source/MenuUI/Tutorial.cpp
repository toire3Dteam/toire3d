#include "Tutorial.h"
#include "Sound\SoundManager.h"

//+-----------------------------
//	�x�[�X
//+-----------------------------

BaseTutorial::BaseTutorial()
{
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

	// �^�C�g����
	m_pTaskTitle.pSting = "None";
	m_pTaskTitle.bClear = false;
	m_pTaskTitle.pSting = 0;

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
	tdnFont::RenderString(m_pTaskTitle.pSting, "HG�޼��E", 22, (int)m_vTaskPos.x+16, (int)m_vTaskPos.y - 40 , 0xffffffff, RS::COPY);


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
		tdnFont::RenderString(m_aTask[i].pSting, "HG�޼��E", 22, (int)m_vTaskPos.x, (int)m_vTaskPos.y + (i * 40), fontCol, RS::COPY);
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
	data.pSting = string;
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

WalkTutorial::WalkTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.01 ����";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;
	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
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

	// 30�t���[�������X�e�[�g�ɂ�����ŏ��N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()))
	{
		m_iWalkFrame++;

		if (m_iWalkFrame >= 30)
		{
			TaskSuccess(WALK);
		}
	}
	else
	{
		m_iWalkFrame = 0;
	}

	// 30�t���[���������X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::BackWalk::GetInstance()))
	{
		m_iBackFrame++;

		if (m_iBackFrame >= 30)
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

SquatTutorial::SquatTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.02 ���Ⴊ��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;
	
	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;
	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
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

	// 30�t���[�����Ⴊ�݃X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()))
	{
		m_iSquatFrame++;

		if (m_iSquatFrame >= 30)
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

JumpTutorial::JumpTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.03 �W�����v";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;
	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
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

AttackTutorial::AttackTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.04 �U��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	//m_pIntroTips = new TipsCard("�����A���x�͊�{�̍U�����@���w�т܂��傤�B\n���̏�Ł��A���Ⴊ��Ł��A�W�����v���Ɂ��A\n����Ƌt�����Ł��������Ă݂܂��傤�B");
	m_pIntroTips = new TipsCard("�����A���x�͊�{�̍U�����@���w�т܂��傤�B\n���̏�Ł��������Ă݂܂��傤�B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�����L�[�Ƃ̑g�ݍ��킹�ŗl�X�ȍU�����J��o���܂��B\n�u�΋�U���v�͋󒆂���̍U���ɑ΂��āy���G�z�ł��B");

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

SkillTutorial::SkillTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.05 �X�L��";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
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

OverDriveTutorial::OverDriveTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.06 �K�E�Z";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 120;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
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

RushTutorial::RushTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.07 ���b�V���R���{";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 120;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("�����Ɓ��B\n�u�K�E�Z�v�𔭓��ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�K�E�Z�v�͂ƂĂ����͂ȋZ�ł��B\n�܂��u�U���v��u�X�L���v����q���Ĕ����ł��܂��B");

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
	if (pPerson->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()))
	{
		Init(m_iDeviceID);// �f�o�C�X�͎������g�Ɠ���
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


//+------------------------------------------------------
//	�����K�[�h�̐���
//+------------------------------------------------------

StandGuardTutorial::StandGuardTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.08 �����K�[�h";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("�K�[�h�O��B\n�u�K�E�Z�v�𔭓��ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�K�E�Z�v�͂ƂĂ����͂ȋZ�ł��B\n�܂��u�U���v��u�X�L���v����q���Ĕ����ł��܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("�K�[�h�|��");
	AddTaskData("�K�[�h�|��");
	AddTaskData("�K�[�h�|��");

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

SquatGuardTutorial::SquatGuardTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.09 ���Ⴊ�݃K�[�h";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("���Ⴊ�݃K�[�h�O��B\n�u�K�E�Z�v�𔭓��ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�K�E�Z�v�͂ƂĂ����͂ȋZ�ł��B\n�܂��u�U���v��u�X�L���v����q���Ĕ����ł��܂��B");

	// �^�X�N�Z�b�g
	AddTaskData("���Ⴊ�݃K�[�h�|�^");
	AddTaskData("���Ⴊ�݃K�[�h�|�^");
	AddTaskData("���Ⴊ�݃K�[�h�|�^");

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
//	���Ⴊ�݃K�[�h�̐���
//+------------------------------------------------------

EscapeTutorial::EscapeTutorial()
{
	// �^�C�g����
	m_pTaskTitle.pSting = "No.10 ���";

	// �N���A����I��܂�
	m_iWaitFrameMAX = 60;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0; UINT	addByte = 0;

	// �I�[�����܂Ń��[�v
	for (UINT i = 0; m_pTaskTitle.pSting[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&m_pTaskTitle.pSting[i]);
		addByte += myByte;
	}
	m_pTaskTitle.iStingLength = addByte;

	// Tips
	m_pIntroTips = new TipsCard("����B\n�u�K�E�Z�v�𔭓��ł��܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�K�E�Z�v�͂ƂĂ����͂ȋZ�ł��B\n�܂��u�U���v��u�X�L���v����q���Ĕ����ł��܂��B");

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
