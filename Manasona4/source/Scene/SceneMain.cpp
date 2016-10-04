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
//		初期化・解放
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
	m_fLoadPercentage = .05f;	// ロード割合
	g_uvEffect = new UVEffectManager();

	m_fLoadPercentage = .1f;	// ロード割合
	// カメラ初期化
	//CameraMgr = new Camera();

	m_fLoadPercentage = .25f;	// ロード割合

	// ステージ初期化
	Stage::Base::CreateStage(&m_pStage, STAGE_ID::SENJO, CameraMgr);	// 関数の中で作ってもらう

	m_fLoadPercentage = .5f;	// ロード割合

	// プレイヤー初期化
	//m_pPlayerMgr = new PlayerManager(4, m_pStage);
	PlayerMgr->Initialize(2, m_pStage);

	m_fLoadPercentage = .75f;	// ロード割合

	// プレイヤーの座標のアドレスをカメラに渡してあげる(いじることは絶対に無く、ただ参照するだけ)
	CameraMgr->SetPlayersPos();

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

	NumberEffect;
	
	GameUIMgr;

	// タイム初期化
	TimeMgr->Reset(60);

	m_stageScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

	/* ステートマシン初期化 */
	m_pStateMachine = new StateMachine<sceneMain>(this);
	m_pStateMachine->SetCurrentState(SceneMainState::Intro::GetInstance());	// ステートの設定

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
//		処理
//******************************************************************

void sceneMain::Update()
{

//	com->Update();

	stopTimer++;
	//if (stopTimer > 60 * 60)
	//{
	//	return true;
	//}

	// フェード更新
	Fade::Update();

	// カメラ更新
	CameraMgr->Update();

	// ステージ更新
	m_pStage->Update();

	// プレイヤー更新
	//PlayerMgr->Update();

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

	// 
	NumberEffect.Update();

	// エンターでエフェクトカメラ発動してみる
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


	// ★ステートマシン更新(何故ここに書くかというと、中でシーンチェンジの処理を行っているため)
	m_pStateMachine->Update();
}

//******************************************************************
//		描画
//******************************************************************

void sceneMain::Render()
{
	// カメラ
	CameraMgr->Activate();

	if (m_bShaderFlag)
	{
		// G_Bufferクリア
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();


		// シェーダ更新
		DeferredManagerEx.G_Update(CameraMgr->m_pos);

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

		RenderStage();// ここでステージだけをまとめて描画

		// 最後の処理
		{
			DeferredManagerEx.FinalBegin();

			//int dim= PlayerMgr->GetOverDriveDim();
			//m_stageScreen->SetARGB(255, dim, dim, dim);
			m_stageScreen->Render(0, 0, RS::COPY_NOZ);// ※Z値考慮させてない理由は↓の絵を描画するため

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


		// UI
		NumberEffect.Render();	
		PlayerMgr->RenderUI();
		GameUIMgr->Render();

		// ★ここにステートマシン描画(多分2D関係が多いんじゃないかと)
		//m_pStateMachine->Render();

		if (KeyBoard(KB_J))
		{
			SurfaceRender();
		}

		// フェード描画
		Fade::Render();
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

	//com->Render(400, 400);

	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", CameraMgr->m_pos.x, CameraMgr->m_pos.y, CameraMgr->m_pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", CameraMgr->m_target.x, CameraMgr->m_target.y, CameraMgr->m_target.z);


}

void sceneMain::RenderStage()
{
	Surface* save;
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &save);

	m_stageScreen->RenderTarget(0); //切り替え
	DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM_SEED)->RenderTarget(1); //切り替え

	// ステージ用のパラメータ
	shaderM->SetValue("g_OverDriveDim", PlayerMgr->GetOverDriveDim());

	// ステージ描画
	m_pStage->Render(shaderM, "Stage");


	tdnSystem::GetDevice()->SetRenderTarget(0, save);//レンダーターゲットの復元
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