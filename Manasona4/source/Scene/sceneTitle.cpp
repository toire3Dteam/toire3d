#include "TDNLIB.h"
#include "system/FrameworkEx.h"
#include "sceneTitle.h"
#include "SceneMenu.h"

//=============================================================================================
//		��	��	��	��	�J	��
bool sceneTitle::Initialize()
{
	// ����֘A������
	m_pMovie = new tdnMovie("DATA/Title/op.wmv", true);
	m_pMovie->Play();

	// �摜������
	m_pImages[IMAGE::BACK] = new tdn2DObj(m_pMovie->GetTexture());
	m_fLoadPercentage = .5f;	// ���[�h����

	m_fLoadPercentage = 1.0f;	// ���[�h����

	return true;
}

sceneTitle::~sceneTitle()
{
	FOR(IMAGE::MAX) delete m_pImages[i];
	delete m_pMovie;
}
//=============================================================================================


//=============================================================================================
//		�X			�V
void sceneTitle::Update()
{
	// �G���^�[�L�[�ŃX�^�[�g
	FOR(4)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, 1) == 3 || KEY(KEY_C, 1) == 3 || KEY(KEY_D, 1) == 3)
		{
			MainFrameEx->ChangeScene(new sceneMenu);
			break;
		}
	}
}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneTitle::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// ����摜
	m_pImages[IMAGE::BACK]->Render(0,0);
}
//
//=============================================================================================