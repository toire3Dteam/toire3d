#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../particle_2d/particle_2d.h"

#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"

//BaseEffect* g_eff;
//EffectManager;
PanelEffectManager* m_panel;
UVEffectManager* g_uvEffect;

//******************************************************************
//		�������E���
//******************************************************************

bool sceneMain::Initialize()
{

	//g_eff = new HitEffect();
	m_panel = new PanelEffectManager();
	g_uvEffect = new UVEffectManager();


	// �J����������
	m_pCamera = new Camera();

	m_fLoadPercentage = .25f;	// ���[�h����

	// �X�e�[�W������
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SYUTEN, m_pCamera);	// �֐��̒��ō���Ă��炤

	m_fLoadPercentage = .5f;	// ���[�h����

	// �v���C���[������
	//m_pPlayerMgr = new PlayerManager(4, m_pStage);
	PlayerMgr->Initialize(4, m_pStage);

	// �v���C���[�̍��W�̃A�h���X���J�����ɓn���Ă�����(�����邱�Ƃ͐�΂ɖ����A�����Q�Ƃ��邾��)
	m_pCamera->SetPlayersPos();

	m_fLoadPercentage = 1.0f;	// ���[�h����

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
	PlayerMgr->Release();
	//EffectMgr.Release();
	SAFE_DELETE(m_panel);
	SAFE_DELETE(g_uvEffect);

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
	PlayerMgr->Update();

	// �G���^�[�ŃG�t�F�N�g�J�����������Ă݂�
	if (KeyBoardTRG(KB_ENTER))
	{
		EFFECT_CAMERA_INFO eci;
		eci.scriptID = 3;
		MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
	}

	if (KeyBoardTRG(KB_L))
	{
		//g_eff->Action(0,0);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::BURN);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::NOTICE);

		m_panel->AddEffect(Vector3(0, 0, -5), PANEL_EFFECT_TYPE::DAMAGE);
		g_uvEffect->AddEffect(Vector3(0, 0, -5), UV_EFFECT_TYPE::WAVE);
	}

	//g_eff->Update();
	//EffectMgr.Update();
	m_panel->Update();
	g_uvEffect->Update();
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
	PlayerMgr->Render();

	//g_eff->Render3D();
	//EffectMgr.Render();
	m_panel->Render();

	//EffectMgr.Render3D();
	m_panel->Render3D();
	g_uvEffect->Render();


	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_pCamera->m_pos.x, m_pCamera->m_pos.y, m_pCamera->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_pCamera->m_target.x, m_pCamera->m_target.y, m_pCamera->m_target.z);
}