#include "MenuIcon.h"
#include "system\System.h"

//+-------------------------
//	�x�[�X���j���[�A�C�R��
//+-------------------------

// �R���X�g���N�^�E�f�X�g���N�^
BaseMenuIcon::BaseMenuIcon()
{
	//m_pFont = nullptr;			// �����摜
	m_pIcon = nullptr;
	//m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/Icon.png");// �A�C�R���摜
	//m_pIcon->OrderNone();
	m_fScale = 1.0f;
	m_fNextScale = 1.0f;

	m_vPos = VECTOR2_ZERO;		// ���݂̍��W
	m_vNextPos = VECTOR2_ZERO;	// �����̍��W

	//m_vPrevPos = VECTOR2_ZERO;	// �ߋ��̍��W
	//m_vCurrPos = VECTOR2_ZERO;	// ���݂̍��W
	//m_fMoveRate = 0.0f;			// �ߋ��E���݂̈ړ��̕�Ԃ̒l
	//m_fMovePower = 0.1f;

	m_bSelectFlag = false;

	m_pInfo = nullptr;
	m_vInfoPos.x = 0.0f;
	m_vInfoPos.y = 592.0f;
}

BaseMenuIcon::~BaseMenuIcon()
{
	//SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pIcon);
	SAFE_DELETE(m_pInfo);
}

// �X�V�E�`��
void BaseMenuIcon::Update()
{
	// �ꏊ��Ԃ̍X�V
	//m_fMoveRate += m_fMovePower;
	//m_fMoveRate = Math::Clamp(m_fMoveRate, 0.0f, 1.0f);
	m_vPos = m_vNextPos * 0.3f + m_vPos * 0.7f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_vPos.x - m_vNextPos.x) &&
		0.9f >= abs(m_vPos.y - m_vNextPos.y))
	{
		m_vPos = m_vNextPos;
	}

	// �X�P�[���̍X�V
	m_fScale = m_fNextScale* 0.3f + m_fScale* 0.7f;
	// �덷�ɂȂ�΍��킹�鏈��
	if (0.00000001f >= abs(m_fScale - m_fNextScale))m_fScale = m_fNextScale;

	//m_pFont->SetScale(m_fScale);
	m_pIcon->SetScale(m_fScale);
	//m_pFont->Update();
	m_pIcon->Update();

	m_pInfo->Update();

}

void BaseMenuIcon::Render(int addY)
{
	// ��Ԃ����ꏊ�ɕ`��
	m_pIcon->Render((int)m_vPos.x, (int)m_vPos.y+ addY, 512, 512, m_bSelectFlag * 512, 0, 512, 512);
	//m_pFont->Render((int)m_vPos.x, (int)m_vPos.y);

	m_pInfo->Render((int)m_vInfoPos.x, (int)m_vInfoPos.y);

}

//void BaseMenuIcon::RenderGray()
//{
//	// ��Ԃ����ꏊ�ɕ`��
//	m_pIcon->Render((int)m_vPos.x, (int)m_vPos.y, 512, 512,
//		m_bSelectFlag * 512, 0, 512, 512,shader2D,"Gray");
//	//m_pFont->Render((int)m_vPos.x, (int)m_vPos.y);
//
//	m_pInfo->Render((int)m_vInfoPos.x, (int)m_vInfoPos.y);
//}

void BaseMenuIcon::Action(int delayFrame)
{
	//m_pFont->Action(delayFrame);
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 512);
	m_pIcon->Action(delayFrame);

}


//+-------------------------
//	�`���[�g���A���A�C�R��
//+-------------------------

TutorialIcon::TutorialIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/TutorialIcon.png");	// �A�C�R���摜

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/TutorialInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128 , (int)m_vInfoPos.y);


}

TutorialIcon::~TutorialIcon()
{
}

void TutorialIcon::Update()
{
	BaseMenuIcon::Update();

}

void TutorialIcon::Render()
{
	BaseMenuIcon::Render();
	
}

//+-------------------------
//	�o�g���A�C�R��
//+-------------------------

BattleIcon::BattleIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/battleIcon.png");	// �A�C�R���摜
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/battleInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);

}

BattleIcon::~BattleIcon()
{
}

void BattleIcon::Update()
{
	BaseMenuIcon::Update();
}

void BattleIcon::Render()
{
	BaseMenuIcon::Render();
}


//+-------------------------
//	�g���[�j���O�A�C�R��
//+-------------------------

TrainingIcon::TrainingIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/TrainingIcon.png");	// �A�C�R���摜
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/TrainingInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);


}

TrainingIcon::~TrainingIcon()
{
}

void TrainingIcon::Update()
{
	BaseMenuIcon::Update();
}

void TrainingIcon::Render()
{
	BaseMenuIcon::Render();
}


//+-------------------------
//	�R���N�V�����A�C�R��
//+-------------------------

CollectIcon::CollectIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/CollectIcon.png");	// �A�C�R���摜
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/CollectInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);


}

CollectIcon::~CollectIcon()
{
}

void CollectIcon::Update()
{
	BaseMenuIcon::Update();
}

void CollectIcon::Render()
{
	BaseMenuIcon::Render();
}


//+-------------------------
//	�I�v�V�����A�C�R��
//+-------------------------

OptionIcon::OptionIcon()
{
	m_pIcon = new tdn2DAnim("Data/UI/Menu/Icon/OptionIcon.png");	// �A�C�R���摜
	m_pIcon->OrderMoveAppeared(12, (int)m_vPos.x, (int)m_vPos.y + 128);

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/OptionInfo.png");
	m_pInfo->OrderMoveAppeared(12, (int)m_vInfoPos.x + 128, (int)m_vInfoPos.y);


}

OptionIcon::~OptionIcon()
{
}

void OptionIcon::Update()
{
	BaseMenuIcon::Update();
}

void OptionIcon::Render()
{
	BaseMenuIcon::Render();
}
