#include "TutorialManager.h"

//+----------------------------
//	�`���[�g���A���}�l�[�W���[
//+----------------------------

// ���̂̐錾
TutorialManager* TutorialManager::m_pInstance = nullptr;

TutorialManager::TutorialManager()
{
	// �S�Ẵ`���[�g���A����������
	for (int i = 0; i < TUTORIAL_TYPE::ARRAY_END; i++)
	{
		switch ((TUTORIAL_TYPE)i)
		{
		case WALK:
			m_pTutorial[i] = new WalkTutorial();
			break;
		case ARRAY_END:
			assert(0);
			break;
		default:			
			assert(0);
			break;
		}

	}

	// �����ݒ�
	m_eType = TUTORIAL_TYPE::WALK;
}

TutorialManager::~TutorialManager()
{
	// �S�Ẵ`���[�g���A�������
	for (int i = 0; i < TUTORIAL_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTutorial[i]);
	}

}

// ������
void TutorialManager::Init(TUTORIAL_TYPE type)
{
	m_eType = type;// �`���[�g���A���̃^�C�v�ύX

	m_pTutorial[m_eType]->Init();

}

// �X�V
void TutorialManager::Update()
{
	// �`���[�g���A���X�V
	m_pTutorial[m_eType]->Update();

}

// �^�X�N�p�X�V
void TutorialManager::TaskUpdate(BasePlayer * pPerson)
{
	// �^�X�N�p�X�V
	m_pTutorial[m_eType]->TaskUpdate(pPerson);

}

// �`��
void TutorialManager::Render()
{
	// �`���[�g���A���`��
	m_pTutorial[m_eType]->Render();
}
