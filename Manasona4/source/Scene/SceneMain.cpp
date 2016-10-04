#include "TDNLIB.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../Effect/Particle.h"
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"
#include "../Timer/Timer.h"
#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "SceneMainState.h"
#include "../Fade/Fade.h"
#include "../DeferredEx/DeferredEx.h"

#include "../PointLight/PointLight.h"

#include "../Number/Number.h"
#include "../UI/GameUI.h"
//#include "../UI/Combo.h"

///Combo* com;

//BaseEffect* g_eff;
//EffectManager;
PanelEffectManager* m_panel;
UVEffectManager* g_uvEffect;

int stopTimer = 0;


//******************************************************************
//		�������E���
//******************************************************************
sceneMain::sceneMain() :BaseGameEntity(ENTITY_ID::SCENE_MAIN)
{

}
bool sceneMain::Initialize()
{
	//com = new Combo();

	m_dirLight = Vector3(1, -1, 1);

	//g_eff = new HitEffect();
	m_panel	   = new PanelEffectManager();
	m_fLoadPercentage = .05f;	// ���[�h����
	g_uvEffect = new UVEffectManager();

	m_fLoadPercentage = .1f;	// ���[�h����
	// �J����������
	//CameraMgr = new Camera();

	m_fLoadPercentage = .25f;	// ���[�h����

	// �X�e�[�W������
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SENJO, CameraMgr);	// �֐��̒��ō���Ă��炤

	m_fLoadPercentage = .5f;	// ���[�h����

	// �v���C���[������
	//m_pPlayerMgr = new PlayerManager(4, m_pStage);
	PlayerMgr->Initialize(2, m_pStage);

	m_fLoadPercentage = .75f;	// ���[�h����

	// �v���C���[�̍��W�̃A�h���X���J�����ɓn���Ă�����(�����邱�Ƃ͐�΂ɖ����A�����Q�Ƃ��邾��)
	CameraMgr->SetPlayersPos();

	m_fLoadPercentage = 1.0f;	// ���[�h����

	// �p�[�e�B�N��������
	ParticleManager::Initialize("DATA/Effect/particle.png", 2048);

	// �I���ȏ�����
	m_pMyMusicMgr = new MyMusicManager(MY_MUSIC_ID::SENJO);
	m_pMyMusicMgr->Play();

	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	m_bShaderFlag = true;

	PointLightMgr;

	NumberEffect;
	
	GameUIMgr;

	// �^�C��������
	TimeMgr->Reset(60);

	m_stageScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneMain>(this);
	m_pStateMachine->SetCurrentState(SceneMainState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�

	return true;
}

sceneMain::~sceneMain()
{
	delete m_pMyMusicMgr;
	delete m_pStage;
	//delete CameraMgr;
	PlayerMgr->Release();
	//EffectMgr.Release();
	SAFE_DELETE(m_panel);
	SAFE_DELETE(g_uvEffect);
	ParticleManager::Release();
	DeferredManagerEx.Release();
	SAFE_DELETE(m_stageScreen);
	delete m_pStateMachine;
	PointLightMgr->Release();

	NumberEffect.Release();

	GameUIMgr->Rerease();

	//SAFE_DELETE(com);
}

//******************************************************************
//		����
//******************************************************************

void sceneMain::Update()
{

//	com->Update();

	stopTimer++;
	//if (stopTimer > 60 * 60)
	//{
	//	return true;
	//}

	// �t�F�[�h�X�V
	Fade::Update();

	// �J�����X�V
	CameraMgr->Update();

	// �X�e�[�W�X�V
	m_pStage->Update();

	// �v���C���[�X�V
	//PlayerMgr->Update();

	// �p�[�e�B�N���X�V
	ParticleManager::Update();

	static float lightAngle = 2.14f;
	// �A���O��
	if (KeyBoard(KB_R)) { lightAngle -= 0.05f; }
	if (KeyBoard(KB_T)) { lightAngle += 0.05f; }

	m_dirLight.x = sinf(lightAngle);
	m_dirLight.z = cosf(lightAngle);
	m_dirLight.y = -0.8f;

	// �u���[�X�V
	DeferredManagerEx.RadialBlurUpdate();

	// �|�C���g���C�g�X�V
	PointLightMgr->Update();

	// 
	NumberEffect.Update();

	// �G���^�[�ŃG�t�F�N�g�J�����������Ă݂�
	if (KeyBoardTRG(KB_ENTER))
	{
		EFFECT_CAMERA_INFO eci;
		eci.scriptID = 0;
		MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
	}

	if (KeyBoardTRG(KB_L))
	{
	//	PointLightManager::GetInstance()->AddPointLight(Vector3(10, 3, 0), Vector3(0, 1, 1), 100, 4, 60, 20, 40);

		//g_eff->Action(0,0);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::BURN);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::NOTICE);

		m_panel->AddEffect(Vector3(0, 5, -5), PANEL_EFFECT_TYPE::GLASS);

		//g_uvEffect->AddEffectRoop(Vector3(5, 5, 0), UV_EFFECT_TYPE::GUARD, 1.0f, 1.0f);

		//g_uvEffect->AddEffect(Vector3(-4, 0, -5), UV_EFFECT_TYPE::CONV,2,2
		//	, Vector3(0, 0, 0), Vector3(0, 0, 0));

		//g_uvEffect->AddEffect(Vector3(4, 0, -5), UV_EFFECT_TYPE::CONV2, 2, 2
		//	, Vector3(0, 0, 0), Vector3(0, 0, 0));

		//g_uvEffect->AddEffect(Vector3(10, 0, -5), UV_EFFECT_TYPE::CONV3, 1, 1
		//	, Vector3(0, 0, 0), Vector3(0, 0, 0));
	}
	if (KeyBoardTRG(KB_NUMPAD1))
	{
		g_uvEffect->AddEffect(Vector3(-2, -5,0 ), UV_EFFECT_TYPE::CONV, 2, 2
			, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}
	if (KeyBoardTRG(KB_NUMPAD2))
	{
		g_uvEffect->AddEffect(Vector3(2, -5, 0), UV_EFFECT_TYPE::CONV2, 2, 2
			, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}
	if (KeyBoardTRG(KB_NUMPAD3))
	{
		g_uvEffect->AddEffect(Vector3(0, -5, 0), UV_EFFECT_TYPE::CONV3, 2, 2
			, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}
	if (KeyBoardTRG(KB_NUMPAD7))
	{
		g_uvEffect->AddEffect(Vector3(0, -5, 0), UV_EFFECT_TYPE::CONV4, 2, 2
			, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}
	if (KeyBoardTRG(KB_NUMPAD8))
	{
		g_uvEffect->AddEffect(Vector3(0, 0, 0), UV_EFFECT_TYPE::BURST_BALL, 1, 2
			, Vector3(0, 0, 0), Vector3(0, 0, 0));

	}

	if (KeyBoardTRG(KB_I))
	{		
		g_uvEffect->AddEffect(Vector3(10, 5, -5), UV_EFFECT_TYPE::CONV, 2, 2
			, Vector3(1.14f, 0, 0), Vector3(1.14f, 0, 0));
	}

	if (KeyBoardTRG(KB_M))
	{
		//g_uvEffect->StopEffectRoop(UV_EFFECT_TYPE::GUARD);
	}

	if (KeyBoardTRG(KB_K))
	{
		if (m_bShaderFlag)
		{
			m_bShaderFlag = false;
		}
		else
		{
			m_bShaderFlag = true;
		}
	}

	//g_eff->Update();
	//EffectMgr.Update();
	m_panel->Update();
	g_uvEffect->Update();


	// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
	m_pStateMachine->Update();
}

//******************************************************************
//		�`��
//******************************************************************

void sceneMain::Render()
{
	// �J����
	CameraMgr->Activate();

	if (m_bShaderFlag)
	{
		// G_Buffer�N���A
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();


		// �V�F�[�_�X�V
		DeferredManagerEx.G_Update(CameraMgr->m_pos);

		// �e
		RenderShadow();

		// �V�F�[�_
		DeferredManagerEx.G_Begin();
		// �X�e�[�W�`��
		m_pStage->RenderDeferred();
		// �v���C���[
		PlayerMgr->RenderDeferred();

		// �V�F�[�_�I���
		DeferredManagerEx.G_End();


		DeferredManagerEx.DirLight(m_dirLight, Vector3(0.8f, 0.72f, 0.72f));
		DeferredManagerEx.HemiLight(Vector3(0.6f, 0.5f, 0.5f), Vector3(0.45f, 0.43f, 0.43f));

		// �|�C���g���C�g�`��
		DeferredManagerEx.GpuPointLightRender();

		RenderStage();// �����ŃX�e�[�W�������܂Ƃ߂ĕ`��

		// �Ō�̏���
		{
			DeferredManagerEx.FinalBegin();

			//int dim= PlayerMgr->GetOverDriveDim();
			//m_stageScreen->SetARGB(255, dim, dim, dim);
			m_stageScreen->Render(0, 0, RS::COPY_NOZ);// ��Z�l�l�������ĂȂ����R�́��̊G��`�悷�邽��

			// �v���C���[
			PlayerMgr->Render();

			// �p�[�e�B�N��
			ParticleManager::Render();

			m_panel->Render();
			m_panel->Render3D();
			g_uvEffect->Render();

			DeferredManagerEx.FinalEnd();
		}

		// �u���[��
		DeferredManagerEx.BloomRender();


		// UI
		NumberEffect.Render();	
		PlayerMgr->RenderUI();
		GameUIMgr->Render();

		// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
		//m_pStateMachine->Render();

		if (KeyBoard(KB_J))
		{
			SurfaceRender();
		}

		// �t�F�[�h�`��
		Fade::Render();
	}
	else
	{

		// �X�e�[�W�`��
		m_pStage->Render(shader, "copy");
		// �v���C���[
		PlayerMgr->Render(shader, "copy");

		m_panel->Render();
		m_panel->Render3D();
		g_uvEffect->Render();
	}

	//com->Render(400, 400);

	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", CameraMgr->m_pos.x, CameraMgr->m_pos.y, CameraMgr->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", CameraMgr->m_target.x, CameraMgr->m_target.y, CameraMgr->m_target.z);


}

void sceneMain::RenderStage()
{
	Surface* save;
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	m_stageScreen->RenderTarget(0); //�؂�ւ�
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->RenderTarget(1); //�؂�ւ�

	// �X�e�[�W�p�̃p�����[�^
	shaderM->SetValue("g_OverDriveDim", PlayerMgr->GetOverDriveDim());

	// �X�e�[�W�`��
	m_pStage->Render(shaderM, "Stage");


	tdnSystem::GetDevice()->SetRenderTarget(0, save);//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

void sceneMain::RenderShadow()
{
	if (DeferredManagerEx.GetShadowFlag() == false)return;

	DeferredManagerEx.CreateShadowMatrix
		(m_dirLight, Vector3(0,0,0), Vector3(0, 0, 1), 400);

	{
		DeferredManagerEx.ShadowBegin();

		m_pStage->RenderShadow();

		DeferredManagerEx.ShadowEnd();
	}
}

void sceneMain::SurfaceRender()
{
	enum {
		X = 320/2, Y = 180/2 
	};

	int texX = 0;
	int texY = 0;

	DeferredManagerEx.GetTex(SURFACE_NAME_EX::NORMAL_DEPTH)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::ROUGHNESS)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SPEC)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::FORWARD)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SCREEN)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX = 0;
	texY++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::POINT_LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	if (DeferredManagerEx.GetShadowFlag() == false)return;
	texX++;
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::SHADOW_MAP)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	texX = 0;
	//texY ++;
	//DeferredManager.GetTex(SURFACE_NAME::MRT_NORMAL)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::MRT_ROUGHNESS)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::LIGHT)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);

	//texX++;
	//DeferredManager.GetTex(SURFACE_NAME::SPECULAR)->Render(X*texX, Y*texY, X, Y, 0, 0, 1280, 720);
}