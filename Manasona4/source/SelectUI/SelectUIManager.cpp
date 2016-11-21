#include "SelectUIManager.h"
#include "Data\SelectData.h"

//+-----------------------------------
//	�L�����N�^�[�Z���N�g�}�l�[�W���[
//+-----------------------------------

SelectUIManager::SelectUIManager()
{
	// ��������s���q�@�f�o�C�X�ԍ��͍��͉�
	m_pLeftSide = new SelectUI(SIDE::LEFT, ENTITY_ID::SELECT_UI_LEFT, 0);
	m_pRightSide = new SelectUI(SIDE::RIGHT, ENTITY_ID::SELECT_UI_RIGHT, 1);

	//for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	//{
	//	m_pPic[i] = nullptr;
	//}

	//m_pPic[PIC_TYPE::BG] = new tdn2DAnim("DATA/UI/CharacterSelect/BG.png");
	//m_pPic[PIC_TYPE::BG]->OrderNone();
	//m_pPic[PIC_TYPE::BG]->Action();

	//m_pPic[PIC_TYPE::TITLE] = new tdn2DAnim("DATA/UI/CharacterSelect/title.png");
	//m_pPic[PIC_TYPE::TITLE]->OrderMoveAppeared(16, 0, -200);

	//m_pPic[PIC_TYPE::BLACK_LINE] = new tdn2DAnim("DATA/UI/CharacterSelect/BlackLine.png");
	//m_pPic[PIC_TYPE::BLACK_LINE]->OrderMoveAppeared(24, 1280, 0);

	//m_pPic[PIC_TYPE::INFO_PLATE] = new tdn2DAnim("DATA/UI/CharacterSelect/Information.png");
	//m_pPic[PIC_TYPE::INFO_PLATE]->OrderMoveAppeared(24, 0, 592 + 200);

	//// ������BOX
	//m_pPic[PIC_TYPE::INFO_BOX_LEFT] = new tdn2DAnim("DATA/UI/CharacterSelect/Info/InfoBox.png");
	//m_pPic[PIC_TYPE::INFO_BOX_LEFT]->OrderMoveAppeared(12, - 384, 354);
	//
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT] = new tdn2DAnim("DATA/UI/CharacterSelect/Info/InfoBox.png");
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT]->OrderMoveAppeared(12, 899 + 384 , 354);

	// �L�����A�C�R��
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{

		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/airou.png");
			break;
		case CHARACTER::TEKI:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/teki.png");
			break;
		default:
			MyAssert(0,"����ȃA�C�R���͂Ȃ�");
			break;
		}	

		// ���ʐݒ�
		m_pCharacterIcon[i]->OrderMoveAppeared(12, 384 + (i * 88), 482 + 384);
	}


}

SelectUIManager::~SelectUIManager()
{
	SAFE_DELETE(m_pLeftSide);
	SAFE_DELETE(m_pRightSide);

	//for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	//{
	//	SAFE_DELETE(m_pPic[i]);
	//}

	// �L�����A�C�R��
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		SAFE_DELETE(m_pCharacterIcon[i])
	}

}

//	�X�V���Ƀf�o�C�X�擾
void SelectUIManager::Update(bool bControl)
{

	
	// �X�V
	//for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	//{
	//	m_pPic[i]->Update();
	//}

	// �L�����A�C�R���X�V
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		m_pCharacterIcon[i]->Update();
	}

	// ���ŃX�e�[�g�}�V���ɂ�镡�G�ȍX�V������
	m_pLeftSide->Update(bControl);
	m_pRightSide->Update(bControl);
	
}

void SelectUIManager::Render()
{
	//m_pPic[PIC_TYPE::BG]->Render(0, 0);

	//m_pPic[PIC_TYPE::BLACK_LINE]->Render(0, 0);

	// �L�����A�C�R��
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		if (m_pLeftSide->GetSelectCharacter() == i|| m_pRightSide->GetSelectCharacter() == i)
		{
			m_pCharacterIcon[i]->SetARGB(0xffffffff);
		}else
		{
			m_pCharacterIcon[i]->SetARGB(0xffaaaaaa);
		}

		m_pCharacterIcon[i]->Render(384 + (i * 88), 482);
	}

	// ���ŃX�e�[�g�}�V���ɂ�镡�G�ȕ`�悪����
	m_pLeftSide->Render();
	m_pRightSide->Render();

	//m_pPic[PIC_TYPE::INFO_BOX_LEFT]->Render(0, 354);
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT]->Render(899, 354);

	//m_pPic[PIC_TYPE::TITLE]->Render(0, 0);
	//m_pPic[PIC_TYPE::INFO_PLATE]->Render(0, 592 + 5);

}

// �����G�`��
void SelectUIManager::RenderCharacter()
{
	// �����̗����G
	m_pLeftSide->RenderCharacter();
	m_pRightSide->RenderCharacter();

}

void SelectUIManager::FirstAction()
{
	//m_pPic[PIC_TYPE::TITLE]->Action();
	//m_pPic[PIC_TYPE::BLACK_LINE]->Action(6);
	//m_pPic[PIC_TYPE::INFO_PLATE]->Action(6);

	// �L�����A�C�R���X�V
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		m_pCharacterIcon[i]->Action(i * 8);
	}

	//m_pPic[PIC_TYPE::INFO_BOX_LEFT]->Action();
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT]->Action();

}
