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
	m_pMovie = new tdnMovie("DATA/Title/op.wmv", true, false);	

	m_pMovieBGLine = new tdnMovie("DATA/Title/titleBG2.wmv");
	m_pBGLine = new tdn2DObj(m_pMovieBGLine->GetTexture());

	// �摜������
	m_pImages[IMAGE::MOVIE] = new tdn2DObj(m_pMovie->GetTexture());
	m_pImages[IMAGE::BG] = new tdn2DObj("Data/Title/BG.png");
	m_pImages[IMAGE::TITLE] = new tdn2DObj("Data/Title/Title.png");
	m_pImages[IMAGE::CLOUD1] = new tdn2DObj("Data/Title/cloud1.png");
	m_pImages[IMAGE::CLOUD2] = new tdn2DObj("Data/Title/cloud2.png");

	m_pLogo = new tdn2DAnim("Data/Title/logo.png");
	m_pLogo->OrderMoveAppeared(90, 0, 720 / 4);
	
	m_pPreaseAnyButton = new tdn2DAnim("Data/Title/PreaseAnyButton.png");
	m_pPreaseAnyButton->OrderAlphaMove(150, 70, 80);

	m_fCloudU = 0.0f;

	m_fLoadPercentage = .5f;	// ���[�h����
	
	m_fLoadPercentage = 1.0f;	// ���[�h����

	m_iGameTimer = 0;

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
	SAFE_DELETE(m_pPreaseAnyButton);

	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_pMovieBGLine);
	SAFE_DELETE(m_pBGLine);

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