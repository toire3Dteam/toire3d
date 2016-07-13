#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../particle_2d/particle_2d.h"
#include "../Effect/EffectManager.h"
#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/Player.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

//******************************************************************
//		�������E���
//******************************************************************

bool sceneMain::Initialize()
{
	// �X�e�[�W������
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SENJO);	// �֐��̒��ō���Ă��炤

	// �v���C���[������
	m_pPlayerMgr = new PlayerManager(4);

	// �J����������
	m_pCamera = new Camera(m_pStage, m_pPlayerMgr);

	// �I���ȏ�����
	m_pMyMusicMgr = new MyMusicManager(MY_MUSIC_ID::TOILE);
	m_pMyMusicMgr->Play();

	return true;
}

sceneMain::~sceneMain()
{
	delete m_pMyMusicMgr;
	delete m_pStage;
	delete m_pCamera;
	delete m_pPlayerMgr;
}

//******************************************************************
//		����
//******************************************************************

bool sceneMain::Update()
{
	// �J�����X�V
	m_pCamera->Update();

	// �X�e�[�W�X�V
	m_pStage->Update();

	// �v���C���[�X�V
	m_pPlayerMgr->Update(m_pStage);

	// �G���^�[�ŃG�t�F�N�g�J�����������Ă݂�
	if (KeyBoardTRG(KB_ENTER))
	{
		EFFECT_CAMERA_INFO eci;
		eci.scriptID = 3;
		MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
	}

	return true;
}

//******************************************************************
//		�`��
//******************************************************************

void sceneMain::Render()
{
	// �J����
	m_pCamera->Activate();

	// �X�e�[�W�`��
	m_pStage->Render();

	// �v���C���[
	m_pPlayerMgr->Render();

	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_pCamera->m_pos.x, m_pCamera->m_pos.y, m_pCamera->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_pCamera->m_target.x, m_pCamera->m_target.y, m_pCamera->m_target.z);
}