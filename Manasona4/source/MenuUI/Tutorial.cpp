#include "Tutorial.h"

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

void BaseTutorial::Init()
{
	m_iWaitFrame = m_iWaitFrameMAX;
	m_bClearFlag = false;


	// �N���A�t���O����������
	for (int i = 0; i < (int)m_aTask.size(); i++)
	{
		m_aTask[i].bClear = false;
	}

	m_iSelectTask = 0;
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
	m_pIntroTips->Update();
	m_pClearTips->Update();

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
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�����v�͈ړ��̊�{�ł��B");


	// �^�X�N�Z�b�g
	AddTaskData("�O�����|���������ςȂ�");
	AddTaskData("�������|���������ςȂ�");

	Init();
}

void WalkTutorial::Init()
{
	// ������
	BaseTutorial::Init();

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
	m_pIntroTips = new TipsCard("���͂��Ⴊ�݂ł��B\n���ł��Ⴊ�݂܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u���Ⴊ�݁v�͊y�����ˁB");

	// �^�X�N�Z�b�g
	AddTaskData("���Ⴊ�݁|���������ςȂ�");

	Init();
}

void SquatTutorial::Init()
{
	// ������
	BaseTutorial::Init();

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
	m_pIntroTips = new TipsCard("���̓W�����v�ł��B\n���ŃW�����v���܂��B\n������x���ŋ󒆂Ŕ�ׂ܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�W�����v�v�͊y�����ˁB");

	// �^�X�N�Z�b�g
	AddTaskData("�W�����v�|��");
	AddTaskData("�󒆃W�����v�|�󒆂Ł�");

	Init();
}

void JumpTutorial::Init()
{
	// ������
	BaseTutorial::Init();

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
	m_pIntroTips = new TipsCard("���͍U���ł��B\n���ōU�����܂��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�U���v�͊y�����ˁB�A�[��");

	// �^�X�N�Z�b�g
	AddTaskData("�U���|��");

	Init();
}

void AttackTutorial::Init()
{
	// ������
	BaseTutorial::Init();

}

// ���ꂼ��̃N���A����
void AttackTutorial::TaskUpdate(BasePlayer * pPerson)
{
	//�����ŐF�X�N���A�����撣��I

	enum
	{
		ATTACK = 0
	};

	// �U���X�e�[�g�ɂ�����N���A
	if (pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()))
	{
		if (pPerson->isHitAttack()==true)
		{
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
	m_pTaskTitle.pSting = "No.05 �h���C�u�U��";

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
	m_pIntroTips = new TipsCard("���̓h���C�u�U���ł��B\n�L�����N�^�|�ɂ��F�X�ȃh���C�u�U��������܂��B \n���Ńh���C�u�U���ł��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�h���C�u�v�͊y�����ˁB��Ԃ������A�[��");

	// �^�X�N�Z�b�g
	AddTaskData("�h���C�u�U���|��");

	Init();
}

void SkillTutorial::Init()
{
	// ������
	BaseTutorial::Init();

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
		if (pPerson->isHitAttack() == true)
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
	m_pIntroTips = new TipsCard("���͕K�E�Z�ł��B\n�Q�[�W��50%�ȏ�Ł��ŕK�E�Z�ł��B");
	m_pClearTips = new TipsCard("�悭�ł��܂����I\n�u�K�E�Z�v�̓}�W�L�`���ˁB��Ԃ������A�[��");

	// �^�X�N�Z�b�g
	AddTaskData("�K�E�Z�|�Q�[�W50%�ȏ�Ł�");

	Init();
}

void OverDriveTutorial::Init()
{
	// ������
	BaseTutorial::Init();

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
		if (pPerson->isHitAttack() == true)
		{
			TaskSuccess(OVER_DRIVE);
		}

	}

}