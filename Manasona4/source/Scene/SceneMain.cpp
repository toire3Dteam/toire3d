#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneMain.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"
#include "../PostEffect/PostEffect.h"
#include "../Effect/Particle.h"

#include "../Stage/Stage.h"
#include "../Camera/camera.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect/UVEffect/UVEffectManager.h"

#include "../DeferredEx/DeferredEx.h"

#include "../PointLight/PointLight.h"


//BaseEffect* g_eff;
//EffectManager;
PanelEffectManager* m_panel;
UVEffectManager* g_uvEffect;

//******************************************************************
//		初期化・解放
//******************************************************************

bool sceneMain::Initialize()
{
	m_dirLight = Vector3(1, -1, 1);

	//g_eff = new HitEffect();
	m_panel	   = new PanelEffectManager();
	g_uvEffect = new UVEffectManager();


	// カメラ初期化
	m_pCamera = new Camera();

	m_fLoadPercentage = .25f;	// ロード割合

	// ステージ初期化
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SENJO, m_pCamera);	// 関数の中で作ってもらう

	m_fLoadPercentage = .5f;	// ロード割合

	// プレイヤー初期化
	//m_pPlayerMgr = new PlayerManager(4, m_pStage);
	PlayerMgr->Initialize(4, m_pStage);

	// プレイヤーの座標のアドレスをカメラに渡してあげる(いじることは絶対に無く、ただ参照するだけ)
	m_pCamera->SetPlayersPos();

	m_fLoadPercentage = 1.0f;	// ロード割合

	// パーティクル初期化
	ParticleManager::Initialize("DATA/Effect/particle.png", 2048);

	// オレ曲初期化
	m_pMyMusicMgr = new MyMusicManager(MY_MUSIC_ID::SENJO);
	m_pMyMusicMgr->Play();

	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	m_bShaderFlag = true;

	PointLightMgr;

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
	ParticleManager::Release();
	DeferredManagerEx.Release();

	PointLightMgr->Release();
}

//******************************************************************
//		処理
//******************************************************************

bool sceneMain::Update()
{
	// カメラ更新
	m_pCamera->Update();

	// ステージ更新
	m_pStage->Update();

	// プレイヤー更新
	PlayerMgr->Update();

	// パーティクル更新
	ParticleManager::Update();

	static float lightAngle = 2.14f;
	// アングル
	if (KeyBoard(KB_R)) { lightAngle -= 0.05f; }
	if (KeyBoard(KB_T)) { lightAngle += 0.05f; }

	m_dirLight.x = sinf(lightAngle);
	m_dirLight.z = cosf(lightAngle);
	m_dirLight.y = -0.8f;

	// ブラー更新
	DeferredManagerEx.RadialBlurUpdate();

	// ポイントライト更新
	PointLightMgr->Update();

	// エンターでエフェクトカメラ発動してみる
	//if (KeyBoardTRG(KB_ENTER))
	//{
	//	EFFECT_CAMERA_INFO eci;
	//	eci.scriptID = 3;
	//	MsgMgr->Dispatch(0, ENTITY_ID::CAMERA_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &eci);
	//}

	if (KeyBoardTRG(KB_L))
	{
		PointLightManager::GetInstance()->AddPointLight(Vector3(10, 3, 0), Vector3(0, 1, 1), 100, 4, 60, 20, 40);

		//g_eff->Action(0,0);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::BURN);
		//EffectMgr.AddEffect(Vector3(0, 0, -5), EFFECT_TYPE::NOTICE);

		m_panel->AddEffect(Vector3(0, 5, -5), PANEL_EFFECT_TYPE::DAMAGE);
		//g_uvEffect->AddEffect(Vector3(0, 0, -5), UV_EFFECT_TYPE::WAVE);
		g_uvEffect->AddEffect(Vector3(5, 5, -5), UV_EFFECT_TYPE::UPPER, 1.0f, 1.5f);
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
	return true;
}

//******************************************************************
//		描画
//******************************************************************

void sceneMain::Render()
{
	// カメラ
	m_pCamera->Activate();

	if (m_bShaderFlag)
	{
		// G_Bufferクリア
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();


		// シェーダ更新
		DeferredManagerEx.G_Update(m_pCamera->m_pos);

		// 影
		RenderShadow();

		// シェーダ
		DeferredManagerEx.G_Begin();
		// ステージ描画
		m_pStage->RenderDeferred();
		// プレイヤー
		PlayerMgr->RenderDeferred();

		// シェーダ終わり
		DeferredManagerEx.G_End();


		DeferredManagerEx.DirLight(m_dirLight, Vector3(0.8f, 0.72f, 0.72f));
		DeferredManagerEx.HemiLight(Vector3(0.6f, 0.5f, 0.5f), Vector3(0.45f, 0.43f, 0.43f));

		// ポイントライト描画
		DeferredManagerEx.GpuPointLightRender();

		// 最後の処理
		{
			DeferredManagerEx.FinalBegin();

			// ステージ描画
			m_pStage->Render(shaderM, "DefaultLighting");
			// プレイヤー
			PlayerMgr->Render();

			// パーティクル
			ParticleManager::Render();

			m_panel->Render();
			m_panel->Render3D();
			g_uvEffect->Render();

			DeferredManagerEx.FinalEnd();
		}

		// ブルーム
		DeferredManagerEx.BloomRender();


		if (KeyBoard(KB_J))
		{
			SurfaceRender();
		}
	}
	else
	{

		// ステージ描画
		m_pStage->Render(shader, "copy");
		// プレイヤー
		PlayerMgr->Render(shader, "copy");

		m_panel->Render();
		m_panel->Render3D();
		g_uvEffect->Render();

	}
	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_pCamera->m_pos.x, m_pCamera->m_pos.y, m_pCamera->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_pCamera->m_target.x, m_pCamera->m_target.y, m_pCamera->m_target.z);
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