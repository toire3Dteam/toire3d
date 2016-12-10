#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneRenderingTest2.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../Effect/Particle.h"

#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"

#include "../DeferredEx/DeferredEx.h"
#include "../PointLight/PointLight.h"

#include "Effect\LocusEffect.h"

/*****************************/
// �`��e�X�g�p
// �@ ����`�悷��				��
// �A �I�u�W�F�N�g���]������	��
// �B ���ʂɔɉh				��
// �C 


iexMesh* g_pStage;
iexMesh* g_pSky;

iexMesh* g_pPlayer;
Vector3 g_vPlayerPos;

//+-------------
//	��
//+-------------
struct Water
{
	iexMesh* pObj;
	Vector3 vPos;
};
Water g_tagWater;

float g_fUvWater = 0.0f;

bool sceneRenderingTest2::Initialize()
{
	//	�r���[�ݒ�
	tdnView::Init();

	m_dirLight = Vector3(1, -1, 1);

	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	m_bShaderFlag = true;
	PointLightMgr;
	m_camera.pos = Vector3(0, 20, -100);
	m_camera.target = Vector3(0, 15, 0);

	// �K�v�ȓz
	g_pStage = new iexMesh("Data/Stage/Sister/stage.imo");
	g_pSky =new iexMesh("Data/Stage/Sister/skydome.imo");
	g_pSky->SetScale(3.0f);
	g_pSky->Update();
	g_pPlayer= new iexMesh("Data/Stage/Garden/Garden.imo");//Data/CHR/aramitama/aramitama_toire.IEM
	g_vPlayerPos = VECTOR_ZERO;
	g_vPlayerPos.y = 5;

	// ��
	g_tagWater.pObj = new iexMesh("Data/Water/water.imo");
	g_tagWater.vPos = VECTOR_ZERO;
	g_tagWater.vPos.y = 10;

	// ���s�E�X�N���[��
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	}
	m_pWaterEnvScreen = new tdn2DObj(1280, 720, TDN2D::USEALPHA);


	return true;
}

sceneRenderingTest2::~sceneRenderingTest2()
{
	DeferredManagerEx.Release();
	PointLightMgr->Release();
	
	SAFE_DELETE(g_pStage);
	SAFE_DELETE(g_pSky);
	SAFE_DELETE(g_tagWater.pObj);
	
	//SAFE_DELETE(m_pStencilSurface);
	SAFE_RELEASE(m_pStencilSurface);
	SAFE_DELETE(m_pWaterEnvScreen);
	SAFE_DELETE(g_pPlayer);

}

//******************************************************************
//		����
//******************************************************************

void sceneRenderingTest2::Update()
{

	// �J����
	static float cameraAngle = 0;
	static float cameraRange = 100;

	// �A���O��
	if (KeyBoard(KB_A)) { cameraAngle -= 0.1f; }
	if (KeyBoard(KB_D)) { cameraAngle += 0.1f; }

	if (KeyBoard(KB_W)) { cameraRange -= 1; }
	if (KeyBoard(KB_S)) { cameraRange += 1; }

	if (KeyBoard(KB_Q)) { m_camera.pos.y -= 1.0f; }
	if (KeyBoard(KB_E)) { m_camera.pos.y += 1.0f; }

	m_camera.pos.x = sinf(cameraAngle) * cameraRange;
	m_camera.pos.z = cosf(cameraAngle) * cameraRange;

	tdnView::Set(m_camera.pos, m_camera.target);

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

	static Vector3 locusPos = VECTOR_ZERO;
	static Vector3 locusPos2 = VECTOR_ZERO;
	locusPos.y = 10;
	locusPos2.y = 5;

	if (KeyBoardTRG(KB_ENTER))
	{
		if (m_bShaderFlag == true)
		{
			m_bShaderFlag = false;
		}
		else
		{
			m_bShaderFlag = true;
		}
	}

	// �X�e�[�W
	g_pStage->Update();
	
	// �v���C���[�̍�����ς���
	if (KeyBoard(KB_U)) { g_vPlayerPos.y += 1.0f; }
	if (KeyBoard(KB_J)) { g_vPlayerPos.y -= 1.0f; }
	
	g_pPlayer->SetPos(g_vPlayerPos);
	//g_pPlayer->Animation();
	g_pPlayer->Update();


	// ���̍�����ς���
	if (KeyBoard(KB_I)) { g_tagWater.vPos.y += 1.0f; }
	if (KeyBoard(KB_K)) { g_tagWater.vPos.y -= 1.0f; }

	g_tagWater.pObj->SetPos(g_tagWater.vPos);
	g_tagWater.pObj->Update();

}

//******************************************************************
//		�`��
//******************************************************************

void sceneRenderingTest2::Render()
{
	tdnView::Activate();
	tdnView::Clear();
	
	// ���p�̊���
	WaterEnvRender();

	if (m_bShaderFlag)
	{
		// G_Buffer�N���A
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();

		// �V�F�[�_�X�V
		DeferredManagerEx.G_Update(m_camera.pos);



		// �e
		RenderShadow();

		// �V�F�[�_
		DeferredManagerEx.G_Begin();

		// �X�e�[�W�`��
		//g_pStage->Render(shaderM, "G_Buffer");

		g_pPlayer->Render(shaderM, "G_Buffer");
		//g_tagWater.pObj->Render(shaderM, "G_Buffer");

		// �V�F�[�_�I���
		DeferredManagerEx.G_End();


		//DeferredManagerEx.DirLight(m_dirLight, Vector3(0.8f, 0.72f, 0.72f));
		//DeferredManagerEx.HemiLight(Vector3(0.6f, 0.5f, 0.5f), Vector3(0.45f, 0.43f, 0.43f));
		DeferredManagerEx.AllLight(m_dirLight, Vector3(0.8f, 0.72f, 0.72f)
			, Vector3(0.6f, 0.5f, 0.5f), Vector3(0.45f, 0.43f, 0.43f));

		// �|�C���g���C�g�`��
		DeferredManagerEx.GpuPointLightRender();

		// �Ō�̏���
		{
			DeferredManagerEx.FinalBegin();

			// �X�e�[�W�`��
			//g_pStage->Render(shaderM, "Stage");
			g_pSky->Render(shaderM, "sky");
			// �v���C���[
			g_pPlayer->Render(shaderM, "DefaultLighting");

			// �p�[�e�B�N��
			ParticleManager::Render();

			DeferredManagerEx.FinalEnd();
		}

		// �ŏI���ʂ̃X�N���[�������}�b�v�Ƃ��ăV�F�[�_���ɑ��� 
		DeferredManagerEx.SendFinalScreenEnv();

		// ��
		tdnStopWatch::Start();
		g_fUvWater += 0.001f;
		shaderM->SetValue("g_fUvWater", g_fUvWater);
		g_tagWater.pObj->Render(shaderM, "CrystalWater");//CrystalWater
		tdnStopWatch::End();

		// �u���[��
		DeferredManagerEx.BloomRender();

		DeferredManagerEx.GetTex(SURFACE_NAME_EX::POINT_LIGHT)->Render(0, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);
		//DeferredManagerEx.GetTex(SURFACE_NAME_EX::ROUGHNESS)->Render(1280 / 4, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);

	}else
	{
		//g_pStage->Render(shaderM, "copy");
		g_pSky->Render();
		g_tagWater.pObj->Render(shaderM, "copy");

	}
	
	float len = Math::Length(Vector3(10, 10, 10), Vector3(0, 0, 0));

	// �T�[�t�F�C�X�`��
	if (KeyBoard(KB_SPACE))
	{
		SurfaceRender();

		m_pWaterEnvScreen->SetAlpha(64);
		m_pWaterEnvScreen->Render(0, 0, 1280, 720, 0, 0, 1280, 720, RS::ADD);
		m_pWaterEnvScreen->SetAlpha(256);

		m_pWaterEnvScreen->Render(0, 720/4, 1280 / 4, 720 / 4, 0, 0, 1280, 720);
		

	}


	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_camera.pos.x, m_camera.pos.y, m_camera.pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_camera.target.x, m_camera.target.y, m_camera.target.z);
	tdnText::Draw(0, 90, 0xff00ffff, "���̍���	   : %.1f", g_tagWater.vPos.y);


}

void sceneRenderingTest2::RenderShadow()
{
	if (DeferredManagerEx.GetShadowFlag() == false)return;

	DeferredManagerEx.CreateShadowMatrix
		(m_dirLight, Vector3(0, 0, 0), Vector3(0, 0, 1), 400);

	{
		DeferredManagerEx.ShadowBegin();

		//g_pStage->Render(shaderM, "ShadowBuf");
		g_pPlayer->Render(shaderM, "ShadowBuf");

		DeferredManagerEx.ShadowEnd();
	}
}

void sceneRenderingTest2::SurfaceRender()
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

// ���̊��}�b�v
void sceneRenderingTest2::WaterEnvRender()
{
	Surface* saveBuffer;
	Surface* saveZ;
	// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveBuffer);
	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);
	// Z�o�b�t�@��؂�ւ�
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	// �����_�[�^�[�Q�b�g�؂�ւ�
	m_pWaterEnvScreen->RenderTarget();
	
	//�@��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);


	// �I�u�W�F�N�g���]

	// ���̍���
	float l_fWaterHeight = g_tagWater.vPos.y;	
	// �V�F�[�_
	shaderM->SetValue("g_fWaterHeight", g_tagWater.vPos.y);

	// ���݂̃v���C���[�����Ɛ��̕�
	float l_vPPHY = g_vPlayerPos.y - l_fWaterHeight;

	// �܂��͐��̍����܂ň����Â藎�Ƃ�
	Vector3 l_vNextPos = g_vPlayerPos;
	l_vNextPos.y = l_fWaterHeight - (g_vPlayerPos.y - l_fWaterHeight);
	Vector3 l_vPrevPos = g_vPlayerPos;

	



	// �I�u�W�F�𐅂̌���
	g_pPlayer->SetPos(l_vNextPos);
	g_pPlayer->Update();
	
	g_pPlayer->Render(shaderM,"CrystalWaterReflect");
	g_pSky->Render(shaderM, "CrystalWaterReflect");

	// �I�u�W�F�̈ʒu�����̏ꏊ��
	g_pPlayer->SetPos(l_vPrevPos);
	g_pPlayer->Update();

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, saveBuffer);

	//�X�e���V���o�b�t�@�̕���
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ);

	// �����������t���N�V�����}�b�v���V�F�[�_��
	shaderM->SetValue("WaterReflectBuf", m_pWaterEnvScreen);

}