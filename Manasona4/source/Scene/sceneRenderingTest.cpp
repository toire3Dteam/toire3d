#include "TDNLIB.h"
#include "../system/Framework.h"
#include "SceneRenderingTest.h"
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

#include "UI\HeaveHoFinish\HeaveHoFinish.h"
#include "ResultPerformance\ScoreUI\ScoreUI.h"

#include "Data\ResultData.h"

// 実験の為に作られた場所
// 確認さえできればいいので適当

//******************************************************************
//		初期化・解放
//******************************************************************

ScoreUI* g_scoreUI;

iexMesh* g_stage;
iexMesh* g_sky;

tdn3DObj* g_airou;

tdn2DObj* g_ScrennShot;

//Effect2D* g_effect;

int g_fram;
Surface* StencilSurface;		// ステンシル保存用
int size = 1024;

tdn2DObj* g_DamageEF;

BaseUVEffect* m_pAIROU_EF;

LocusEffect* m_pLocus;

BaseUVEffect* m_pAIROU_EF_IMP;

BaseUVEffect* m_pSpeedLine;

BaseUVEffect* m_pWave;

BaseUVEffect* m_pWind;

BasePanelEffect* m_pPanel;

// デリートしたいとき
#define NO_MEMORY_LEAK


bool sceneRenderingTest::Initialize()
{
	ResultData data;
	g_scoreUI = new ScoreUI(data);


	m_pPanel = new AirouWindEffect();

	m_pSpeedLine = new SpeedLineGreenEffect();
	m_pSpeedLine->ActionRoop(Vector3(0, 0, 0), 0.5f, 0.5f);


	m_pLocus = new LocusEffect("DATA/UVeffect/Airou/Locus.png", 6,2);


	m_pAIROU_EF = new AirouBoostEffect();
	m_pAIROU_EF->ActionRoop();


	m_pAIROU_EF_IMP = new BreakImpactEffect();
	m_pAIROU_EF_IMP->Action();

	m_pWave = new FireRingEffect();
	m_pWave->Action();

	m_pWind = new WaveEffect();
	m_pWind->Action();

	g_fram = 0;

	//g_ScrennShot = new tdn2DObj(size, size, TDN2D::USEALPHA);
	//シャドウマップ用ステンシルバッファサーフェイスの作成!
	//if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &StencilSurface, NULL)))
	//{
	//	MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	//};

	//	ビュー設定
	tdnView::Init();

	//Effect2DMgr;
	//Effect2DMgr.AddNode();
	
	g_stage = new iexMesh("Data/Stage/Sister/iwym.IMO");
	g_sky = new iexMesh("Data/Stage/senjo/Skydome.IMO");
	g_sky->SetScale(2.5f);
	g_sky->SetPos(Vector3(0, -30, 0));
	g_sky->Update();
	g_airou = new tdn3DObj("DATA/CHR/Airou/airou.TDNSM", "DATA/CHR/Airou/airou.TDNMo");
	//g_airou->GetSkinMesh()->OutPutTDNSM("DATA/CHR/airou.TDNSM");
	g_airou->SetMotion(0);

	m_dirLight = Vector3(1, -1, 1);

	m_fLoadPercentage = .25f;	// ロード割合
	m_fLoadPercentage = .5f;	// ロード割合
	m_fLoadPercentage = 1.0f;	// ロード割合

	// パーティクル初期化
	//ParticleManager::Initialize("DATA/Effect/particle.png", 2048);


	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	m_bShaderFlag = true;

	PointLightMgr;

	m_camera.pos = Vector3(0, 20, -100);
	m_camera.target = Vector3(0, 15, 0);

	g_DamageEF = new tdn2DObj("DATA/Effect/DamageEffect.png");

	HeaveHoFinishUI;

	return true;
}

sceneRenderingTest::~sceneRenderingTest()
{
	//Effect2DMgr.Release();

	//SAFE_DELETE(g_ScrennShot);
	SAFE_DELETE(g_stage);
	SAFE_DELETE(g_sky);
	delete g_airou;	// SAFE_DELETEだとなぜかメモリリークが発生(deleteされなかった)

	SAFE_DELETE(g_DamageEF);

	//ParticleManager::Release();
	DeferredManagerEx.Release();

	PointLightMgr->Release();

	HeaveHoFinishUI->Rerease();

#ifdef NO_MEMORY_LEAK
	delete m_pPanel;
	delete m_pSpeedLine;
	delete m_pAIROU_EF;
	delete m_pAIROU_EF_IMP;
	delete m_pLocus;
	delete m_pWave;
	delete m_pWind;
	delete g_DamageEF;
	delete g_scoreUI;
#endif
}

//******************************************************************
//		処理
//******************************************************************

void sceneRenderingTest::Update()
{
	if (KeyBoardTRG(KB_7)) 
	{
		g_scoreUI->Action();
	}

	g_scoreUI->Update();

	// カメラ
	static float cameraAngle = 0;
	static float cameraRange= 100;

	// アングル
	if (KeyBoard(KB_A)) { cameraAngle -= 0.1f; }
	if (KeyBoard(KB_D)) { cameraAngle += 0.1f; }

	if (KeyBoard(KB_W)) { cameraRange -= 1; }
	if (KeyBoard(KB_S)) { cameraRange += 1; }

	if (KeyBoard(KB_Q)) { m_camera.pos.y -= 1.0f; }
	if (KeyBoard(KB_E)) { m_camera.pos.y += 1.0f; }

	m_camera.pos.x = sinf(cameraAngle) * cameraRange;
	m_camera.pos.z = cosf(cameraAngle) * cameraRange;

	tdnView::Set(m_camera.pos, m_camera.target);

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



	if (KeyBoardTRG(KB_L))
	{
		PointLightManager::GetInstance()->AddPointLight(Vector3(10, 3, 0), Vector3(0, 1, 1), 100, 4, 60, 20, 40);
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

	//
	g_stage->Update();

	g_airou->Update();
	g_airou->Animation(1.0f);




	static Vector3 locusPos = VECTOR_ZERO;
	static Vector3 locusPos2 = VECTOR_ZERO;
	locusPos.y = 10;
	locusPos2.y = 5;

	m_pAIROU_EF->Update();
	m_pAIROU_EF->SetPos(locusPos-Vector3(0,5,0));

	m_pAIROU_EF_IMP->Update();

	m_pWave->Update();

	m_pWind->Update();

	m_pSpeedLine->Update();

	m_pPanel->Update();

	if (KeyBoardTRG(KB_C))
	{
		HeaveHoFinishUI->Action();
	}

	HeaveHoFinishUI->Update();

	//m_pLocus->Update(locusPos + Vector3(0, 3, 0), locusPos + Vector3(0, -3, 0));
	m_pLocus->Update();

	if (KeyBoard(KB_V))
	{
		locusPos.x -= 5;
		locusPos2.x -= 5;

		m_pAIROU_EF->SetAngleAnimation(Vector3(0, 0, 1.57f) , Vector3(0, 0, 1.57f));

	}
	if (KeyBoard(KB_B))
	{
		locusPos.x += 5;
		locusPos2.x += 5;

		m_pAIROU_EF->SetAngleAnimation(Vector3(0, 0, -1.57f), Vector3(0, 0, -1.57f));

	}
	if (KeyBoardTRG(KB_N))
	{
		m_pLocus->Action(&(locusPos), &(locusPos2));
	}
	if (KeyBoardTRG(KB_M))
	{
		m_pLocus->Stop();
	}

	if (KeyBoardTRG(KB_H)==1)
	{
		// かんぺき！　つぎ風戦
		m_pAIROU_EF_IMP->Action(Vector3(10, 0, 0), .5, 1.0f);
		PointLightManager::GetInstance()->AddPointLight(Vector3(10, 10, 0), Vector3(1, 0.5, 0), 125, 2, 10, 2, 6);
		m_pWave->Action(Vector3(10, 5, 0), 0.5, 1);

		m_pWind->Action(Vector3(10, 10, 0), 2.5, 3.5, Vector3(0, 0, 0), Vector3(0, 0, 0));

		DeferredManagerEx.SetRadialBlur(Vector2(0, 0), 5);


		for (int i = 0; i < 8; i++)
		{
			PointLightManager::GetInstance()->AddPointLight(Vector3(i * 30.0f ,5, 0), Vector3(1, 0.5f, 0), 65, 2, 180, 2, 170);
			PointLightManager::GetInstance()->AddPointLight(Vector3(i * 30.0f, 5, 40), Vector3(0, 1.0f, .7f), 65, 3, 180, 2, 170);

		}
	}

	if (KeyBoardTRG(KB_J) == 1)
	{
		m_pPanel->Action(Vector3(0, 10, 0));
	}


	//
	//if (KeyBoard(KB_8) == 1)
	//{
	//	Effect2DMgr.GetEffectNode(0)->LoadTexture("DATA/Effect/DamageEffect.png");
	//}
	//if (KeyBoard(KB_9) == 1)
	//{
	//	//Effect2DMgr.GetEffectNode(1)->LoadTexture("DATA/Effect/star.png");
	//	Effect2DMgr.m_iSelectNodeNo++;
	//}
	//if (KeyBoard(KB_0) == 1)
	//{
	//	//Effect2DMgr.GetEffectNode(0)->LoadTexture("DATA/Effect/star.png");
	//	Effect2DMgr.m_iSelectNodeNo--;
	//}
	//if (KeyBoard(KB_5) == 1)
	//{
	//	Effect2DMgr.Save("さべ.hef");
	//}
	//if (KeyBoard(KB_4) == 1)
	//{
	//	Effect2DMgr.Load("さべ.hef");
	//}

	//if (KeyBoard(KB_T) == 1)
	//{
	//	Effect2DMgr.ChangeScreenSize(1024, 400);
	//}
	//if (KeyBoard(KB_Y) == 1)
	//{
	//	Effect2DMgr.ChangeScreenSize(256, 512);
	//}

	//Effect2DMgr.Update();

	//g_fram++;
	//if (g_fram > 120)
	//{
	//	g_fram = 0;
	//}


}

//******************************************************************
//		描画
//******************************************************************

void sceneRenderingTest::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	if (m_bShaderFlag)
	{
		// G_Bufferクリア
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();

		// シェーダ更新
		DeferredManagerEx.G_Update(m_camera.pos);

		// 影
		RenderShadow();

		// シェーダ
		DeferredManagerEx.G_Begin();

		// ステージ描画
		g_stage->Render(shaderM, "G_Buffer");


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
			g_stage->Render(shaderM, "DefaultLighting");
			g_sky->Render();
			g_airou->Render(shaderM,"linecopy");

			//m_pAIROU_EF->Render();

			m_pAIROU_EF_IMP->Render();

			m_pWave->Render();

			m_pWind->Render();

			m_pLocus->Render();

			m_pSpeedLine->Render();

			m_pPanel->Render3D();


			HeaveHoFinishUI->Render();

			// パーティクル
			ParticleManager::Render();

			DeferredManagerEx.FinalEnd();
		}

		// ブルーム
		DeferredManagerEx.BloomRender();

		DeferredManagerEx.GetTex(SURFACE_NAME_EX::POINT_LIGHT)->Render(0,0,1280/4, 720/4, 0, 0, 1280, 720);
		//DeferredManagerEx.GetTex(SURFACE_NAME_EX::ROUGHNESS)->Render(1280 / 4, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);


		///*******************************/
		//// まずは今のサーフェイスを保存
		///*******************************/
		//Surface* surface;
		//tdnSystem::GetDevice()->GetRenderTarget(0, &surface);
		//

		////シャドウマップ用ステンシルバッファサーフェイスの作成!
		////if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pShadowStencilSurface, NULL)))
		////{
		////	MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
		////};


		//D3DVIEWPORT9 viewPort;
		//// 現在のビューポートの一時保管
		//tdnSystem::GetDevice()->GetViewport(&viewPort);

		//// ビューポートの作成
		//D3DVIEWPORT9 vp = { 0, 0, 1024, 1024, 0, 1.0f };
		//tdnSystem::GetDevice()->SetViewport(&vp);

		//// 現在のステンシルバッファを一時保管
		//Surface* SaveStencilSurface;
		//tdnSystem::GetDevice()->GetDepthStencilSurface(&SaveStencilSurface);

		//// ステンシルバッファに切り替え
		//tdnSystem::GetDevice()->SetDepthStencilSurface(StencilSurface);

		//g_ScrennShot->RenderTarget();
		////　画面クリア
		//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		//DeferredManagerEx.GetTex(SURFACE_NAME_EX::SCREEN)->Render(0, 0);
		//SurfaceRender();
		//tdnPolygon::Rect(128, 128, 512);

		//tdnSystem::GetDevice()->SetRenderTarget(0, surface);//レンダーターゲットの復元		
		//tdnSystem::GetDevice()->SetDepthStencilSurface(SaveStencilSurface);	//ステンシルバッファの復元														
		//tdnSystem::GetDevice()->SetViewport(&viewPort);//ビューポートの復元


		//if (KeyBoard(KB_J))
		//{
		//	g_ScrennShot->Render(0, 200,1280/2,720/2,0,0,1280,720);			
		//}

		//if (KeyBoard(KB_NUMPAD1) == 1)
		//{
		//	Effect2DMgr.OutputPng("Effect/b.png", 1);
		//}
		//if (KeyBoard(KB_NUMPAD2) == 1)
		//{
		//	Effect2DMgr.OutputPngLine("Effect/a.p", 2,4);
		//}

		//g_DamageEF->Render(300, 0);
	}
	else
	{

	}

	g_scoreUI->Render();

	//if (KeyBoardTRG(KB_6))
	//{
	//	// スクショ出力
	//	D3DXSaveTextureToFile("screenshot.png", D3DXIFF_PNG, DeferredManagerEx.GetTex(SURFACE_NAME_EX::BLOOM)->GetTexture() , NULL);
	//	D3DXSaveTextureToFile("screenshot2.png", D3DXIFF_PNG, g_ScrennShot->GetTexture(), NULL);

	//}

	// 

	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_camera.pos.x, m_camera.pos.y, m_camera.pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_camera.target.x, m_camera.target.y, m_camera.target.z);

	//Effect2DMgr.Render();
}

void sceneRenderingTest::RenderShadow()
{
	if (DeferredManagerEx.GetShadowFlag() == false)return;

	DeferredManagerEx.CreateShadowMatrix
		(m_dirLight, Vector3(0, 0, 0), Vector3(0, 0, 1), 400);

	{
		DeferredManagerEx.ShadowBegin();

		g_stage->Render(shaderM, "ShadowBuf");

		DeferredManagerEx.ShadowEnd();
	}
}

void sceneRenderingTest::SurfaceRender()
{
	enum {
		X = 320 , Y = 180
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