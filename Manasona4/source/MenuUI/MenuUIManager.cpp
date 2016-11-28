#include "MenuUIManager.h"

//+--------------------------
//	���j���[UI�}�l�[�W���[
//+--------------------------

MenuUIManager::MenuUIManager(int iSelectNo)
{
	// �Z���N�g�i���o�[
	m_iSelectNo = iSelectNo;

	//�@�A�C�R���̕�
	m_fIconWidth = 256;
	m_fIconHeight = -48;


	int xSize = 0;

	// UI�A�C�R��
	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		switch (MENU_ICON_TYPE(i))
		{
		case MENU_ICON_TYPE::TUTORIAL:
			m_pIcon[i] = new TutorialIcon();
			break;
		case MENU_ICON_TYPE::BATTLE:
			m_pIcon[i] = new BattleIcon();
			break;
		case MENU_ICON_TYPE::TRAINING:
			m_pIcon[i] = new TrainingIcon();
			break;
		case MENU_ICON_TYPE::COLLECT:
			m_pIcon[i] = new CollectIcon();
			break;
		case MENU_ICON_TYPE::OPTION:
			m_pIcon[i] = new OptionIcon();
			break;
		case MENU_ICON_TYPE::ARRAY_END:
			
			break;
		default:
			break;
		}

		int rate = 0;
		int heght = 0;
		if (i == (m_iSelectNo))// �I�����Ă�G
		{
			rate = 64;
			heght = 32;
		}
		if (i == (m_iSelectNo + 1))rate = 64;// �I�����Ă�G�̂��̂��Ƃ̃A�C�R��			
		xSize += rate;// �{��


		// �|�W�V�����̏����ʒu�ݒ�
		float ScroolX = (m_iSelectNo*-m_fIconWidth);
		m_pIcon[i]->SetPos(Vector2(48 + (i * m_fIconWidth) + ScroolX + xSize,
			m_fIconHeight + heght));

		// �|�W�V����
		//m_pIcon[i]->SetPos(Vector2((m_fIconWidth*i)+128, m_fIconHeight));
		
		// �X�P�[��
		m_pIcon[i]->SetScale(0.5f);
	}


	
	// ����
	m_pblueLine = new tdn2DObj("Data/UI/Menu/BlueLine.png");
	m_iBlueLineY = 0;
	m_iNextBlueLineY = 0;

	m_pBlackLine = new tdn2DAnim("Data/UI/Menu/BlackLine.png");
	m_pBlackLine->OrderMoveAppeared(32, -1280, 0);

	//
	m_pInfo = new tdn2DAnim("Data/UI/Menu/Information.png");
	m_iInfoY = 592;
	m_pInfo->OrderMoveAppeared(16, 0, m_iInfoY + 128);
	
	// ����
	m_pCoinUI = new CoinUI(Vector2(15.0f, 540.0f));

	// ��
	pExp = new tdn2DObj("Data/UI/Menu/exp.png");

	// ����
	m_pMoveMgr = new tdnMovie("DATA/UI/Menu/Menu2.wmv");
	m_pMove = new tdn2DObj(m_pMoveMgr->GetTexture());
	m_pMoveMgr->Play();

}

MenuUIManager::~MenuUIManager()
{
	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pIcon[i]);
	}

	SAFE_DELETE(m_pCoinUI);

	SAFE_DELETE(m_pblueLine);
	SAFE_DELETE(m_pBlackLine);
	SAFE_DELETE(m_pInfo);

	SAFE_DELETE(pExp);

	SAFE_DELETE(m_pMoveMgr);
	SAFE_DELETE(m_pMove);
}

// �X�V
void MenuUIManager::Update()
{
	// ����̃��[�v�X�V
	m_pMoveMgr->LoopUpdate();

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		m_pIcon[i]->Update();
	}

	m_pInfo->Update();

	m_pBlackLine->Update();

	m_pCoinUI->Update();

	// ��������
	Move();
}

// ��������
void MenuUIManager::Move()
{
	// ���C���̓���
	m_iBlueLineY = (int)(m_iBlueLineY*0.7f + m_iNextBlueLineY*0.3f);


}

// �`��
void MenuUIManager::Render()
{
	// ��
	pExp->Render(0, 0);
	m_pMove->Render(0, 0);

	m_pBlackLine->Render(0, 0);

	// ����
	m_pblueLine->Render(0, m_iBlueLineY);

	// �����̕�
	m_pInfo->Render(0, m_iInfoY);

	// ����
	m_pCoinUI->Render();

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		//if (m_iSelectNo == i)
		//{
			m_pIcon[i]->Render(m_iBlueLineY);
		//}
		//else
		//{
		//	m_pIcon[i]->RenderGray();
		//}
	
	}

	tdnText::Draw(100, 100, 0xffffffff, "%d�Z���N�g�ԍ�", m_iSelectNo);
}

// [����]�A�N�V����
void MenuUIManager::Action()
{
	int iWaitFrame = 0;

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		if ( i >= m_iSelectNo)
		{
			iWaitFrame += 6;
		}		

		m_pIcon[i]->Action(iWaitFrame);


		// �I�����Ă�A�C�R���̂�
		if (i == m_iSelectNo)
		{
			m_pIcon[i]->GetInfo()->Action(14);
		}
	}

	m_pInfo->Action();
	m_pBlackLine->Action();
	m_pCoinUI->Action();
}

// ����A�N�V����[�I����2��]
void MenuUIManager::ActionFall()
{
	// ���C�������܂�
	m_iNextBlueLineY = -500;


}

void MenuUIManager::ActionUp()
{
	// ���C���グ�܂�
	m_iNextBlueLineY = 0;

}

// ��������S�ď���
void MenuUIManager::InfoStop()
{
	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		m_pIcon[i]->GetInfo()->Stop();
	}
}

void MenuUIManager::InfoAction()
{
	m_pIcon[m_iSelectNo]->GetInfo()->Action();
}

void MenuUIManager::ChangeSelectNo(int setSelectNo)
{
	m_iSelectNo = setSelectNo;

	int xSize = 0;

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		int rate = 0;
		int heght = 0;
		if (i == (m_iSelectNo))// �I�����Ă�G
		{
			rate = 64;
			heght = 32;
		}
		if (i == (m_iSelectNo+1))rate = 64;// �I�����Ă�G�̂��̂��Ƃ̃A�C�R��			
		xSize += rate;// �{��
		
		// ���̃|�W�V������ݒ�
		float ScroolX = (setSelectNo*-m_fIconWidth);
		m_pIcon[i]->SetNextPos(Vector2(48 + (i * m_fIconWidth)+ ScroolX + xSize,
			m_fIconHeight+ heght));

		// �I�����Ă�A�C�R���̏���
		if (setSelectNo == i)
		{
			m_pIcon[i]->SetSelectFlag(true);
			m_pIcon[i]->SetNextScale(1.0f);
			m_pIcon[i]->GetInfo()->Action();
		}
		else
		{
			m_pIcon[i]->SetSelectFlag(false);
			m_pIcon[i]->SetNextScale(0.5f);
			m_pIcon[i]->GetInfo()->Stop();
		}

	}	

}
