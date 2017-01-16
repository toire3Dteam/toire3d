#include "TDNLIB.h"
#include "system/FrameworkEx.h"
#include "sceneTitle.h"
#include "sceneTitleState.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"

//=============================================================================================
//		��	��	��	��	�J	��
//=============================================================================================
sceneTitle::sceneTitle() :BaseGameEntity(ENTITY_ID::SCENE_TITLE) {}
bool sceneTitle::Initialize()
{
	// ����֘A������
	m_pMovie = new tdnMovie("DATA/Title/op.wmv", true);
	m_pMovie->Play();

	// �摜������
	m_pImages[IMAGE::BACK] = new tdn2DObj(m_pMovie->GetTexture());
	
	m_pLogo = new tdn2DAnim("Data/Title/logo.png");
	m_pLogo->OrderMoveAppeared(90, 0, 720 / 4);

	m_fLoadPercentage = .5f;	// ���[�h����
	


	m_fLoadPercentage = 1.0f;	// ���[�h����

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneTitle>(this);
	m_pStateMachine->SetCurrentState(SceneTitleState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�


	return true;
}

sceneTitle::~sceneTitle()
{
	FOR(IMAGE::MAX) delete m_pImages[i];
	SAFE_DELETE(m_pMovie);
	SAFE_DELETE(m_pLogo);

	SAFE_DELETE(m_pStateMachine);

}
//=============================================================================================


//=============================================================================================
//		�X			�V
void sceneTitle::Update()
{
	// �t�F�[�h�X�V
	Fade::Update();

	{
		// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
		m_pStateMachine->Update();
		return;
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

	// ���X�e�[�g�}�V���`��
	m_pStateMachine->Render();

	// �t�F�[�h
	Fade::Render();

	//int work;
	//m_pMovie->GetTextureRenderer()->get_AvgFrameRate(&work);
	//tdnText::Draw(0, 30, 0xffffffff, "���σt���[�����[�g          :%d", work);
	//m_pMovie->GetTextureRenderer()->get_AvgSyncOffset(&work);
	//tdnText::Draw(0, 60, 0xffffffff, "���ϗ\�莞�ԍ�(�~���b)�@    :%d", work);
	//m_pMovie->GetTextureRenderer()->get_DevSyncOffset(&work);
	//tdnText::Draw(0, 90, 0xffffffff, "���Ԃ̊Ԃ̕W���΍�(�~���b)  :%d", work);
	//m_pMovie->GetTextureRenderer()->get_FramesDrawn(&work);
	//tdnText::Draw(0, 120, 0xffffffff, "�`��t���[����             :%d", work);
	//m_pMovie->GetTextureRenderer()->get_FramesDroppedInRenderer(&work);
	//tdnText::Draw(0, 150, 0xffffffff, "�h���b�v�t���[����         :%d", work);
	//m_pMovie->GetTextureRenderer()->get_Jitter(&work);
	//tdnText::Draw(0, 180, 0xffffffff, "���̃t���[���W���΍�       :%d", work);
}
//
//=============================================================================================