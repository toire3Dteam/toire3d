#include "TDNLIB.h"
#include "../system/FrameworkEx.h"
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

#include "Window\BaseWindow.h"
#include "Window\SoundWindow.h"

#include "PointLight\PointLightInstancingMesh.h"

#include "BattleLoading\BattleLoading.h"
#include "Fade\Fade.h"
#include "Effect\ExPlosion.h"

#include "KeyConfig\KeyConfig.h"


SoundWindow* g_pSoundWindow;

KeyConfig* keyconfig;

/*****************************/
// 描画テスト用
// ① 水を描画する				◎
// ② オブジェクト反転させる	◎
// ③ 水面に繁栄				◎
// ④ 


iexMesh* g_pStage;
iexMesh* g_pSky;

iexMesh* g_pPlayer;
Vector3	g_vPlayerPos;

iexMesh* g_pTarget;

// 海
iexMesh* g_pSea;
tdn2DObj* g_pEnv;

float g_fY = 0;;

//+-------------
//	水
//+-------------
struct Water
{
	iexMesh* pObj;
	Vector3 vPos;
};
Water g_tagWater;
float g_fUvWater = 0.0f;

//+-------------
//	メッシュ
//+-------------
tdnMesh* g_pBox;
//PointLightInstancingMesh* g_pObj;

iex3DObj* g_pSkinObj;
float g_fSleap = 0;
int m_iSquat = 10;
//
//BaseUVEffect *m_pExplosion;			// 爆発UVエフェクト
//BaseUVEffect *m_pFireRing;
//BaseUVEffect *m_pFireWave;

CExplosion *eeeee;

bool sceneRenderingTest2::Initialize()
{
	keyconfig = new KeyConfig();

	eeeee = new CExplosion();

	g_pSkinObj= new iex3DObj("Data/CHR/teki/teki.iem");
	g_pSkinObj->SetMotion(0);

	//g_pObj = new PointLightInstancingMesh("Data/Shader/PLS_CCW.imo", 3);
	//Matrix* mat = new Matrix[g_pObj->GetMaxInstancingNum()];
	//for (int i = 0; i < g_pObj->GetMaxInstancingNum(); i++)
	//{
	//	D3DXMatrixIdentity(&mat[i]);
	//	mat[i]._11 = 1.0f;
	//	mat[i]._22 = 1.0f;
	//	mat[i]._33 = 1.0f;
	//	mat[i]._41 = (float)(rand() % 50 / (float)5) - 0.5 - 5;
	//	mat[i]._42 = (float)(rand() % 50 / (float)5) - 1.0f;
	//	mat[i]._43 = (float)(rand() % 50 / (float)5) - 0.5;

	//}
	//g_pObj->Update(mat, g_pObj->GetMaxInstancingNum());
	//g_pObj->GetInstancingData(1)->vPos.x = 10;
	//g_pObj->GetInstancingData(1)->fPower = 2;
	//g_pObj->GetInstancingData(1)->fScale = 50;
	//
	//g_pObj->GetInstancingData(2)->vPos.y = 10;
	//g_pObj->GetInstancingData(2)->vPos.x = -10;	
	//g_pObj->GetInstancingData(2)->fScale = 2;
	//
	//g_pObj->GetInstancingData(0)->vPos.x = -10;
	//
	//g_pObj->InstancingUpdate();
	//delete mat;

	g_pBox = new tdnMesh();
	Vector3 posList[]
	{
		{ -2, -2, 0 },
		{ -2, 2, 0 },
		{ 0, 0, 0 },
		{ 2, -2, 0 },
		{ 2, 2, 0 },
	};
	g_pBox->CreateCube(1, 1, 1, 0xFFFFFFFF, posList, 5);


	//	ビュー設定
	tdnView::Init();

	m_dirLight = Vector3(1, -1, 1);

	DeferredManagerEx;
	DeferredManagerEx.InitShadowMap(1024);
	DeferredManagerEx.InitPointLightInstancing(5);

	// ワールド座標のデータも詰める
	Vector3 wpos = Vector3(150, 20, -10);
	InstancingData data;

	for (int i = 0; i < DeferredManagerEx.GetPLSInstancing()->GetMaxInstancingNum(); i++)
	{
		wpos.x -= 50;

		data.vPos = wpos;
		data.fPower = 6;
		data.fScale = 35;
		data.vColor = Vector3(1.5f, 0.75f, 0.4f);

		DeferredManagerEx.GetPLSInstancing()->SetInstancingData(i, data);

	}
	DeferredManagerEx.GetPLSInstancing()->InstancingUpdate();


	//Matrix* mat = new Matrix[g_pObj->GetMaxInstancingNum()];
	//for (int i = 0; i < g_pObj->GetMaxInstancingNum(); i++)
	//{
	//	D3DXMatrixIdentity(&mat[i]);
	//	mat[i]._11 = 1.0f;
	//	mat[i]._22 = 1.0f;
	//	mat[i]._33 = 1.0f;
	//	mat[i]._41 = (float)(rand() % 50 / (float)5) - 0.5 - 5;
	//	mat[i]._42 = (float)(rand() % 50 / (float)5) - 1.0f;
	//	mat[i]._43 = (float)(rand() % 50 / (float)5) - 0.5;

	//}

	m_bShaderFlag = false;
	PointLightMgr;
	m_camera.pos = Vector3(0, 20, -100);
	m_camera.target = Vector3(0, 15, 0);

	// 必要な奴
	g_pStage = new iexMesh("Data/Stage/Sister/stage.imo");
	g_pSky =new iexMesh("Data/Stage/nanasato/skydome_night.imo");//Sister
	//g_pSky->SetScale(3.0f);
	g_pSky->Update();
	g_pPlayer= new iexMesh("Data/Stage/Nanasato/NanasatoSity.imo");//Data/CHR/aramitama/aramitama_toire.IEM
	g_vPlayerPos = VECTOR_ZERO;//anasato/NanasatoSity.imo
	g_vPlayerPos.y = 0;

	g_pTarget = new iexMesh("Data/CHR/teki/teki.imo");


	// 水
	g_tagWater.pObj = new iexMesh("Data/Water/water2.imo");
	g_tagWater.vPos = VECTOR_ZERO;
	g_tagWater.vPos.y = -28;

	g_pSoundWindow = new SoundWindow(Vector2(500, 100));

	// 海
	g_pSea = new iexMesh("Data/Stage/Sister/Water/Water.imo");
	g_pEnv = new tdn2DObj("Data/Stage/Sister/Water/EnvSky.png");
	shaderM->SetValue("EnvMap", g_pEnv);

	// 奥行・スクリーン
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	}
	m_pWaterEnvScreen = new tdn2DObj(1280, 720, TDN2D::USEALPHA);


	// PLの場所
	m_aPos.clear();
	m_aPos.reserve(32);

	// 兄貴
	m_pAniki = new iex3DObj("Data/CHR/Aniki/Aniki.iem");

	//BattleLoadInst;

	return true;
}

sceneRenderingTest2::~sceneRenderingTest2()
{
	//SAFE_DELETE(g_pObj);

	DeferredManagerEx.Release();
	PointLightMgr->Release();
	
	SAFE_DELETE(g_pStage);
	SAFE_DELETE(g_pSky);
	SAFE_DELETE(g_tagWater.pObj);
	SAFE_DELETE(g_pSea);
	SAFE_DELETE(g_pEnv);

	//SAFE_DELETE(m_pStencilSurface);
	SAFE_RELEASE(m_pStencilSurface);
	SAFE_DELETE(m_pWaterEnvScreen);
	SAFE_DELETE(g_pPlayer);
	SAFE_DELETE(g_pTarget);

	// ウィンドウ
	SAFE_DELETE(g_pSoundWindow);

	SAFE_DELETE(m_pAniki);

	SAFE_DELETE(g_pBox);

	//BattleLoadInst->Rerease();

	SAFE_DELETE(g_pSkinObj);

	SAFE_DELETE(eeeee);

	SAFE_DELETE(keyconfig);
}

//******************************************************************
//		処理
//******************************************************************

void sceneRenderingTest2::Update()
{
	if (KeyBoardTRG(KB_F))
	{
		keyconfig->Action(Vector2(100,100));
	}
	keyconfig->Ctrl(0);

	keyconfig->Update();


	if (KeyBoardTRG(KB_B)) 
	{
		eeeee->Action(VECTOR_ZERO);

	}

	eeeee->Update();

	ParticleManager::Update();


	if (KeyBoard(KB_H)) {
		g_pSkinObj->SetMotion(0);
	}
	if (KeyBoard(KB_J)) {
		g_pSkinObj->SetMotion(10);
	}

	g_pSkinObj->Animation();
	//g_pSkinObj->UpdateSkinMeshFrame(,);
	g_pSkinObj->Update(0.25f);


	//BattleLoadInst->Update();

	//// ワールド座標のデータも詰める
	//Vector3 wpos = Vector3(0, 15, 0);

	//// カメラ空間変換
	//Matrix mat = matView;
	//Vector3 vpos;
	//vpos.x = wpos.x * mat._11 + wpos.y * mat._21 + wpos.z * mat._31 + mat._41;
	//vpos.y = wpos.x * mat._12 + wpos.y * mat._22 + wpos.z * mat._32 + mat._42;
	//vpos.z = wpos.x * mat._13 + wpos.y * mat._23 + wpos.z * mat._33 + mat._43;

	//DeferredManagerEx.GetPLSInstancing()->GetInstancingData(0)->vPos = wpos;
	//DeferredManagerEx.GetPLSInstancing()->GetInstancingData(0)->vVpos = vpos;
	//DeferredManagerEx.GetPLSInstancing()->GetInstancingData(0)->fScale = 90;

	//DeferredManagerEx.GetPLSInstancing()->GetInstancingData(0)->vColor = Vector3(1.5f, 0.75f, 0.4f);
	////DeferredManagerEx.GetPLSInstancing()->GetInstancingData(0)->fRange = 100;
	//DeferredManagerEx.GetPLSInstancing()->GetInstancingData(0)->fPower = 4;

	//DeferredManagerEx.GetPLSInstancing()->InstancingUpdate();

	g_pBox->UpdateWorldMatrix();

	// カメラ
	static float cameraAngle = 0;
	static float cameraRange = 100;

	// アングル
	if (KeyBoard(KB_A)) { cameraAngle -= 0.05f; }
	if (KeyBoard(KB_D)) { cameraAngle += 0.05f; }

	if (KeyBoard(KB_W)) 
	{
		m_camera.target.x -= sinf(cameraAngle) * 1;
		m_camera.target.z -= cosf(cameraAngle) * 1;
	}
	if (KeyBoard(KB_S)) 
	{
		m_camera.target.x += sinf(cameraAngle) * 1;
		m_camera.target.z += cosf(cameraAngle) * 1;
	}

	if (KeyBoard(KB_Q)) { m_camera.pos.y = m_camera.target.y -= 0.5f; }
	if (KeyBoard(KB_E)) { m_camera.pos.y = m_camera.target.y += 0.5f; }

	if (KeyBoard(KB_R)) { g_fY -= 1.5f; }
	if (KeyBoard(KB_T)) { g_fY += 1.5f; }


	m_camera.pos.x =m_camera.target.x + sinf(cameraAngle) * cameraRange;
	m_camera.pos.z =m_camera.target.z + cosf(cameraAngle) * cameraRange;
	m_camera.pos.y = m_camera.target.y + g_fY;// Y軸補正

	g_pTarget->SetPos(m_camera.target);
	g_pTarget->Update();



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

	// ステージ
	g_pStage->Update();
	
	// プレイヤーの高さを変える
	//if (KeyBoard(KB_U)) { g_vPlayerPos.y += 1.0f; }
	//if (KeyBoard(KB_J)) { g_vPlayerPos.y -= 1.0f; }
	
	g_pPlayer->SetPos(g_vPlayerPos);
	//g_pPlayer->Animation();
	g_pPlayer->Update();


	// 水の高さを変える
	if (KeyBoard(KB_Y)) { g_tagWater.vPos.y += 1.0f; }
	if (KeyBoard(KB_I)) { g_tagWater.vPos.y -= 1.0f; }
	if (KeyBoard(KB_U)) { g_tagWater.vPos.z += 1.0f; }
	if (KeyBoard(KB_J)) { g_tagWater.vPos.z -= 1.0f; }
	if (KeyBoard(KB_H)) { g_tagWater.vPos.x += 1.0f; }
	if (KeyBoard(KB_K)) { g_tagWater.vPos.x -= 1.0f; }

	g_tagWater.pObj->SetPos(g_tagWater.vPos);
	g_tagWater.pObj->Update();

	g_pSea->SetPos(g_tagWater.vPos + Vector3(0, 10, 0));
	g_pSea->Update();

	// 他のウィンドウを触っているときに別ウィンドウを動かしたくないときは
	// m_iChoiceStateをつかうといいかも

	// ウィンドウ更新
	g_pSoundWindow->Update();
	
	if (KeyBoardTRG(KB_0)) 
	{ 
		g_pSoundWindow->Action();
	}


	g_pSoundWindow->Ctrl(0);



	// ポイントライトフェス
	Vector3 LightCol = Vector3(1.5f, 0.75f, 0.4f);

	 //PointLightMgr->AddPointLight(Vector3(0,20,0), LightCol, 90, 4, 1, 1, 1);
	 //PointLightMgr->AddPointLight(Vector3(-60, 20, 0), LightCol, 90, 4, 1, 1, 1);
	 //PointLightMgr->AddPointLight(Vector3(60, 20, 0), LightCol, 90, 4, 1, 1, 1);
	 //PointLightMgr->AddPointLight(Vector3(120, 20, 0), LightCol, 90, 4, 1, 1, 1);

	// ライトを置く
	if (KeyBoardTRG(KB_L))
	{
		m_aPos.push_back(m_camera.target);
	}
	if (KeyBoardTRG(KB_K))
	{
		m_aPos.pop_back();
	}

	// ポイントライト
	for (int i = 0; i < (int)m_aPos.size(); i++)
	{
		PointLightMgr->AddPointLight(m_aPos[i], LightCol, 90*1, 4*1, 1, 1, 1);
	}

	m_pAniki->Animation();
	m_pAniki->Update();

	static float uvWater = 0;
	uvWater += 0.00025f;
	shaderM->SetValue("m_fUvSea", uvWater);

	// フェード
	Fade::Update();
}

//******************************************************************
//		描画
//******************************************************************

void sceneRenderingTest2::Render()
{
	tdnView::Set(m_camera.pos, m_camera.target);

	tdnView::Activate();
	tdnView::Clear();

	tdnStopWatch::Start();

	// 水用の環境光
	WaterEnvRender();

	// シェーダ更新
	DeferredManagerEx.G_Update(m_camera.pos);

	if (m_bShaderFlag)
	{
		// G_Bufferクリア
		DeferredManagerEx.ClearLightSurface();
		DeferredManagerEx.ClearBloom();
		DeferredManagerEx.ClearGpuPointLight();

	
		// 影
		RenderShadow();

		// シェーダ
		DeferredManagerEx.G_Begin();

		// ステージ描画
		//g_pStage->Render(shaderM, "G_Buffer");

		g_pPlayer->Render(shaderM, "G_Buffer");
		//g_tagWater.pObj->Render(shaderM, "G_Buffer");
		g_pTarget->Render(shaderM, "G_Buffer");



		// シェーダ終わり
		DeferredManagerEx.G_End();


		//DeferredManagerEx.DirLight(m_dirLight, Vector3(0.4f, 0.32f, 0.72f));
		//DeferredManagerEx.HemiLight(Vector3(0.3f, 0.1f, 0.1f), Vector3(0.35f, 0.23f, 0.23f));
		DeferredManagerEx.AllLight(m_dirLight, Vector3(0.8f, 0.72f, 0.72f)
			, Vector3(0.6f, 0.5f, 0.5f), Vector3(0.45f, 0.43f, 0.43f));

		// ポイントライト描画
		DeferredManagerEx.GpuPointLightRender();

		//g_pObj->InstancingPointLightRender();

		// 最後の処理
		{
			DeferredManagerEx.FinalBegin();

			// ステージ描画
			//g_pStage->Render(shaderM, "Stage");
			g_pSky->Render(shaderM, "nightsky");
			// プレイヤー
			g_pPlayer->Render(shaderM, "DefaultLighting");
			// ターゲット
			g_pTarget->Render(shaderM, "DefaultLighting");

			// 海
			g_pSea->Render(shaderM, "DefaultLighting");

		
			// パーティクル
			ParticleManager::Render();

			DeferredManagerEx.FinalEnd();
		}

		// 最終結果のスクリーンを環境マップとしてシェーダ側に送る 
		DeferredManagerEx.SendFinalScreenEnv();

		// 水
		g_fUvWater += 0.001f;
		shaderM->SetValue("g_fUvWater", g_fUvWater);
		g_tagWater.pObj->Render(shaderM, "CrystalWater");//CrystalWater
	

		// 海
		g_pSea->Render(shaderM, "Sea");

		// ブルーム
		DeferredManagerEx.BloomRender();

		DeferredManagerEx.GetTex(SURFACE_NAME_EX::POINT_LIGHT)->Render(0, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);
		//DeferredManagerEx.GetTex(SURFACE_NAME_EX::ROUGHNESS)->Render(1280 / 4, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);

	}else
	{
		g_pSkinObj->Render();

		//g_pStage->Render(shaderM, "copy");
		g_pSky->Render();
		g_tagWater.pObj->Render(shaderM, "copy");

		eeeee->Render();
		ParticleManager::Render();

		//m_pAniki->Render(shaderM, "copy");

		//g_pBox->Render(shaderM, "linecopy");
		
		//g_pObj->Render(shaderM, "copy");
		//g_pObj->InstancingRender();
		DeferredManagerEx.GetPLSInstancing()->InstancingRender();

	}

	//BattleLoadInst->Render();
	
	float len = Math::Length(Vector3(10, 10, 10), Vector3(0, 0, 0));

	// サーフェイス描画
	if (KeyBoard(KB_SPACE))
	{
		SurfaceRender();

		m_pWaterEnvScreen->SetAlpha(64);
		m_pWaterEnvScreen->Render(0, 0, 1280, 720, 0, 0, 1280, 720, RS::ADD);
		m_pWaterEnvScreen->SetAlpha(256);

		//m_pWaterEnvScreen->Render(0, 720/4, 1280 / 4, 720 / 4, 0, 0, 1280, 720);
		

	}

	//  WindowUI
	g_pSoundWindow->Redner();


	keyconfig->Render();

	tdnText::Draw(0, 30, 0xffffffff, "CameraPos    : %.1f %.1f %.1f", m_camera.pos.x, m_camera.pos.y, m_camera.pos.z);
	tdnText::Draw(0, 60, 0xffffffff, "CameraTarget : %.1f %.1f %.1f", m_camera.target.x, m_camera.target.y, m_camera.target.z);
	tdnText::Draw(0, 90, 0xff00ffff, "水の高さ	   : %.1f %.1f %.1f", g_tagWater.vPos.x, g_tagWater.vPos.y, g_tagWater.vPos.z);
	tdnText::Draw(0, 120, 0xff00ffff,"プレイヤー　: %.1f %.1f %.1f", g_vPlayerPos.x, g_vPlayerPos.y, g_vPlayerPos.z);
	tdnText::Draw(0, 150, 0xff00ffff, "目の順応度: %.1f", DeferredManagerEx.GetExposure());

	for (int i = 0; i < (int)m_aPos.size(); i++)
	{
		tdnText::Draw(400, 30 + (i * 32) , 0xffffffff, "PlPos  : %.1f %.1f %.1f", m_aPos[i].x, m_aPos[i].y, m_aPos[i].z);
	}

	tdnStopWatch::End();


	// フェード
	Fade::Render();
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
	enum 
	{
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

// 水の環境マップ
void sceneRenderingTest2::WaterEnvRender()
{
	Surface* saveBuffer;
	Surface* saveZ;
	// 現在のサーフェイスを一時保管
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveBuffer);
	// 現在のステンシルバッファを一時保管
	tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);
	// Zバッファを切り替え
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	// レンダーターゲット切り替え
	m_pWaterEnvScreen->RenderTarget();
	
	//　画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);


	// オブジェクト反転

	// 水の高さ
	float l_fWaterHeight = g_tagWater.vPos.y;	
	// シェーダ
	shaderM->SetValue("g_fWaterHeight", g_tagWater.vPos.y);

	// 現在のプレイヤー高さと水の幅
	float l_vPPHY = g_vPlayerPos.y - l_fWaterHeight;

	// まずは水の高さまで引きづり落とす
	Vector3 l_vNextPos = g_vPlayerPos;
	l_vNextPos.y = l_fWaterHeight - (g_vPlayerPos.y - l_fWaterHeight);
	Vector3 l_vPrevPos = g_vPlayerPos;

	



	// オブジェを水の元へ
	g_pPlayer->SetPos(l_vNextPos);
	g_pPlayer->Update();
	
	g_pPlayer->Render(shaderM,"CrystalWaterReflect");
	g_pSky->Render(shaderM, "CrystalWaterReflect");

	// オブジェの位置を元の場所へ
	g_pPlayer->SetPos(l_vPrevPos);
	g_pPlayer->Update();

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, saveBuffer);

	//ステンシルバッファの復元
	tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ);

	// 生成したリフレクションマップをシェーダに
	shaderM->SetValue("WaterReflectBuf", m_pWaterEnvScreen);

}
