#include "Deferred.h"
/*
*	Deferred
*/


// サーフェイスを綺麗にリリース
#define ReleaseSurface(p) { if(p != nullptr) delete p; p = nullptr;}



// 極小バッファのサイズ
static const int Reduction_lvl = 4;
static const int Reduction_x = 1280 / Reduction_lvl, Reduction_y = 720 / Reduction_lvl;

static const int MiniTex_lvl = 6;
static const int MiniTex_x = 1280 / MiniTex_lvl, MiniTex_y = 720 / MiniTex_lvl;

// ダウンサンプル用
static const int downSampleSize = 4;

// ポイントライト用
static const int InfoPL_X = 32;
static const int InfoPL_Y = 2;



Deferred* Deferred::pInstance = nullptr;

// インスタンス
Deferred& Deferred::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new Deferred;
	}
	return *pInstance;
}

void Deferred::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

/* マップの読み込み情報 */
// N RGB->法線 
// S R->スペキュラ G->ブルームレート B->リムライト　A→エミッシブ
// H R->AO G->エミッシブ追加予定　 B->空白

// 初期化
void Deferred::Init()
{
	// G_Bufferを作成
	m_mDiffuse = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA); // RGB アルべド
	m_mNormal = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);	 // RGB 法線　A アンビエントレート
	m_mRoughness= new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);	 // R スペキュラ G ブルーム　B リムライト A　エミッシブ
	m_mDepth = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::FLOAT1);		 // R デプス

	// G-Bufferを利用するサーフェイス
	m_sLight= new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);
	m_sSpecular = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);
	m_sFog= new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);
	m_sShadow = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);
	
	// FinalPassSurface
	m_sScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);
	m_sBloomScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);
	m_sBloom = new tdn2DObj(MiniTex_x, MiniTex_y, TDN2D::USEALPHA);
	m_sBloomSeed = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);

	m_sCrossGlareX = new tdn2DObj(MiniTex_x, MiniTex_y, TDN2D::USEALPHA);
		m_sCrossGlareY = new tdn2DObj(MiniTex_x, MiniTex_y, TDN2D::USEALPHA);

	// ForwardSurface
	m_sForward = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);		//	上から被せるため透明度も採用	

	// BackBuffer
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_backbufferZ);
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	tdnSystem::GetDevice()->GetViewport(&m_saveViewport);// 現在のビューポートの一時保管

	//ShadowMaps												 //ShadowMaps										 //ShadowMaps
	m_shadowZ = nullptr;
	m_sShadowMap = nullptr;
	m_sShadowMapL = nullptr;
	m_sSoftShadowMap = nullptr;
	m_iShadowSize = 0;
	m_fShadowRange = 0.0f;
	m_bShadowFlag = false;
	m_bCascadeFlag = false;

	//　目線
	m_vViewPos = Vector3(.0f, .0f, .0f);

	// キューブテクスチャ
	//m_cubeTex = nullptr;

	//
	m_fExposure = 0.0f;

	// Gpu_PointLight
	PLSdata.reserve(PLS_MAX);// 前もって容量を確保 (reserve) しておけば、データの追加時に発生する再割り当て (reallocation) を防ぐことができ、効率的です。 
	m_sGpuPointLight = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);
	
	m_PLS = new iexMesh("DATA/Shader/PLS_CCW.imo");

}

Deferred::Deferred()
{
	Init();
}

// 解放
Deferred::~Deferred()
{
	// G-Buffer
	ReleaseSurface(m_mDiffuse);
	ReleaseSurface(m_mNormal);
	ReleaseSurface(m_mRoughness);
	ReleaseSurface(m_mDepth);

	// SecondPassSurface
	ReleaseSurface(m_sLight);
	ReleaseSurface(m_sSpecular);
	ReleaseSurface(m_sFog);
	ReleaseSurface(m_sShadow);

	// FinalPassSurface
	ReleaseSurface(m_sScreen);
	ReleaseSurface(m_sBloomScreen);
	ReleaseSurface(m_sBloom);
	ReleaseSurface(m_sBloomSeed);
	ReleaseSurface(m_sCrossGlareX);
	ReleaseSurface(m_sCrossGlareY);
	// ForwardSurface
	ReleaseSurface(m_sForward);

	// GpuPointLight
	ReleaseSurface(m_sGpuPointLight);
	SAFE_DELETE(m_PLS);

	//BackBufferSurface
	backbuffer->Release();
	backbuffer = nullptr;

	m_backbufferZ->Release();
	m_backbufferZ = nullptr;

	m_savebackbuffer->Release();
	m_savebackbuffer = nullptr;

	// キューブ
	//if (m_cubeTex != nullptr)
	//{
	//	m_cubeTex->Release();
	//}
	
	if (m_bShadowFlag)
	{
		ReleaseSurface(m_sShadowMap);
	}
	if (m_bCascadeFlag)
	{
		ReleaseSurface(m_sShadowMapL);
		ReleaseSurface(m_sSoftShadowMap);
	}
}

// G_Buffer作成開始 &前回使用したのサーフェイスのクリア
void Deferred::Begin()
{
	//バックバッファサーフェイスの保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer); // ? これ何回もする必要ある？

	//ライトサーフェイス画面クリア
	m_sLight->RenderTarget();
	m_sSpecular->RenderTarget(1);
	m_sFog->RenderTarget(2);
	//if (ssaoFlag == true)ssao->RenderTarget(2);


	//ライトサーフェイス画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	//MRTの割り当て
	m_mDiffuse->RenderTarget(SURFACE_NAME::MRT_DIFFUSE);
	m_mNormal->RenderTarget(SURFACE_NAME::MRT_NORMAL);
	m_mRoughness->RenderTarget(SURFACE_NAME::MRT_ROUGHNESS);
	m_mDepth->RenderTarget(SURFACE_NAME::MRT_DEPTH);

	//G_Buffer画面クリア
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);



}

// G_Buffer作成終了
void Deferred::End()
{
	//バックバッファサーフェイスの復元・MRTの終了処理
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
	tdnSystem::GetDevice()->SetRenderTarget(2, nullptr);
	tdnSystem::GetDevice()->SetRenderTarget(3, nullptr);


	//作った情報テクスチャを転送
	shader->SetValue("DepthBuf", m_mDepth);
	shader->SetValue("SpecularBuf", m_mRoughness);
	shader->SetValue("NormalBuf",  m_mNormal);

	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);
}

// ※Updateはレンダーで呼んでください
// Deferredの更新
void Deferred::Update(const Vector3 ViewPos)
{
	// トーンマッピング
	CtrlExposure();

	// 目線
	m_vViewPos = ViewPos;
	shader->SetValue("ViewPos", m_vViewPos);

	//プロジェクション逆行列生成
	Matrix invproj = matProjection;
	D3DXMatrixInverse(&invproj, nullptr, &invproj);
	shader->SetValue("InvPMatrix", invproj);

	//プロジェクション→ビュー逆行列生成
	Matrix invview = matView;
	D3DXMatrixInverse(&invview, nullptr, &invview);
	shader->SetValue("InvVPMatrix", invproj * invview);

	/*************************************/
	//	変換行列をシェーダ側に贈る
	/*************************************/
	// View変換行列 
	shader->SetValue("VMatrix", matView);
	// Projection変換行列 
	shader->SetValue("PMatrix", matProjection);
}

// キューブテクスチャをセット
void Deferred::CubeSet(char * name)
{
	HRESULT		hr;
	CubeTex  *cubeTex = NULL;
	LPCTSTR str = name;

	hr = D3DXCreateCubeTextureFromFile(tdnSystem::GetDevice(), str, &cubeTex);

	if (FAILED(hr))
	{

		if (hr == D3DERR_NOTAVAILABLE)
		{
			assert(0);
		}
		if (hr == D3DERR_OUTOFVIDEOMEMORY)
		{
			assert(0);
		}
		if (hr == D3DXERR_INVALIDDATA)
		{
			assert(0);
		}
		if (hr == E_OUTOFMEMORY)
		{
			assert(0);
		}
		if (hr == D3DERR_INVALIDCALL)
		{
			assert(0);

		}
	}
	shader->SetValue("cubeTex", cubeTex);

	cubeTex->Release();
}

//**************************************************
//				Lighting
//**************************************************
// 平行光
void Deferred::DirLight(const Vector3 dir, const Vector3 color)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ワールドの平行光を渡す
	Vector3 wLightDir = dir;
	wLightDir.Normalize();
	//shader->SetValue("wLightVec", wLightDir);

	//ビュー座標系に変換
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;


	LightDir.Normalize();

	//	シェーダー設定 shaderに送る
	shader->SetValue("ViewLightVec", LightDir);
	shader->SetValue("LightColor", (Vector3)color);

	//現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//レンダーターゲットの切替え
	m_sLight->RenderTarget();
	m_sSpecular->RenderTarget(1);

	//	レンダリング
	m_mNormal->Render(0,0,shader, "DirLight");

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// 半球ライティング
void Deferred::HemiLight(const Vector3 SkyColor, const Vector3 GroundColor)
{
	//	シェーダー設定
	shader->SetValue("SkyColor", (Vector3)SkyColor);
	shader->SetValue("GroundColor", (Vector3)GroundColor);

	//現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//レンダーターゲットの切替え
	m_sLight->RenderTarget();

	//	レンダリング
	m_mNormal->Render(0,0,shader, "HemiLight");

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
}


void Deferred::Fog(const float FogNear, const float FogFar, const Vector3 FogColor)
{
	//	GPUへ送る情報
	shader->SetValue("FogNear", FogNear);
	shader->SetValue("FogFar", FogFar);
	shader->SetValue("FogColor", (Vector3)FogColor);

	//現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//レンダーターゲットの切替え
	m_sFog->RenderTarget();

	//  深度を使いたいので
	//	depthでレンダリング
	m_mDepth->Render(0, 0, shader, "Fog");

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
}


//*****************************************************
//					ShadowMaps
//*****************************************************

// シャドウマップの初期化　
void Deferred::CreateShadowMap(const int size)
{
	// 2回以上、シャドウマップを生成させない。
	if (m_bShadowFlag) return;

	// シャドウマップのサイズを確保
	m_iShadowSize = size;

	// シャドウマップの生成
	m_sShadowMap = new tdn2DObj(size, size, TDN2D::FLOAT2);		//r成分にzバッファg成分にzバッファの二乗の値が入る

	//シャドウマップ用ステンシルバッファサーフェイスの作成
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_shadowZ, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};

	m_bShadowFlag = true;	// シャドウマップ採用
}

void Deferred::CreateShadowMapL(const int size)
{
	//シャドウマップLのサイズを確保
	m_iShadowSizeL = size;
	//ブラー処理シャドウマップの生成
	m_sSoftShadowMap = new tdn2DObj(size, size, TDN2D::FLOAT2);
	m_sShadowMapL = new tdn2DObj(size, size, TDN2D::FLOAT2);

	m_bCascadeFlag = true;	// 二重シャドウマップ採用
}

//　シャドウの更新
void Deferred::CreateShadowMatrix(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{

#ifdef _DEBUG
	//シャドウマップが生成されていなければ転送しない
	if (!m_bShadowFlag) 
	{
		MessageBox(tdnSystem::GetWindow(), "シャドウマップを作っていない", __FUNCTION__, MB_OK);
		exit(-1);
	}

#endif

	//	シャドウ作成
	dir.Normalize();
	Vector3 pos = target - dir * dist;
	Vector3 up(.0f, 1.0f, .0f);

	//幅の指定
	float width = dist;

	//	視点とライト位置へ
	Matrix	ShadowMat, work;
	// ビュー行列の設定
	Math::LookAtLH(ShadowMat, pos + (playerVec), target + (playerVec), up);
	// 平行投影行列作成 ビューボリューム
	Math::OlthoLH(work, width, width, 0.02f, width * 2.5f);	//平行投影行列
	ShadowMat *= work;
	// 合成する
	shader->SetValue("ShadowProjection", ShadowMat);

	// アルファ調整
	m_fShadowRange = width * 0.25f;

	////cascadeするならそっちの行列も作成する
	//if (!cascadeFlag) return;
	//pos = target  - dir * dist * 3.0f;			//近距離シャドウに比べ3倍ほど離す
	//up = Vector3(.0f, 1.0f, .0f);

	//D3DXMatrixIdentity(&ShadowMat);
	//D3DXMatrixIdentity(&work);
	//LookAtLH(ShadowMat, pos + (playerVec * 210), target + (playerVec * 210), up);
	//OlthoLH(work, width * 3.0f, width * 3.0f, 0.02f, width * 7.5f);
	//ShadowMat *= work;

	//shaderD->SetValue("ShadowProjectionL", ShadowMat);
}

//　シャドウの更新
void Deferred::CreateShadowMatrixL(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{

	if (!m_bCascadeFlag) return;

	//	シャドウ作成
	dir.Normalize();
	Vector3 pos = target - dir * dist;
	Vector3 up(.0f, 1.0f, .0f);

	//幅の指定
	float width = dist;

	//	視点とライト位置へ
	Matrix	ShadowMat, work;

	//cascadeするならそっちの行列も作成する	
	//pos = target - dir * dist;			//近距離シャドウに比べ3倍ほど離す
	//up = Vector3(.0f, 1.0f, .0f);
	D3DXMatrixIdentity(&ShadowMat);
	D3DXMatrixIdentity(&work);
	float MulPower = 3.0f; // ★遠距離はくそ広くてＯＫ？
	Math::LookAtLH(ShadowMat, pos + (playerVec)*MulPower, target + (playerVec)*MulPower, up);
	Math::OlthoLH(work, width*MulPower, width*MulPower, 0.02f, width * 2.5f);
	ShadowMat *= work;

	shader->SetValue("ShadowProjectionL", ShadowMat);
}

//　シャドウマップ用の描画開始
void Deferred::ShadowBegin()
{
#ifdef _DEBUG
	// シャドウマップが生成されていなければ書かない
	if (!m_bShadowFlag) {
		MessageBox(tdnSystem::GetWindow(), "シャドウマップが作られていない", __FUNCTION__, MB_OK);
		exit(-1);
	}
#endif


	// 現在のサーフェイスを一時保管
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);

	// 現在のステンシルバッファを一時保管
	tdnSystem::GetDevice()->GetDepthStencilSurface(&m_backbufferZ);
	// シャドウマップ用ステンシルバッファに切り替え
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_shadowZ);

	// 現在のビューポートの一時保管
	tdnSystem::GetDevice()->GetViewport(&m_orgViewport);
	// ビューポートの作成
	D3DVIEWPORT9 vp = { 0, 0, m_iShadowSize, m_iShadowSize, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);

	// シャドウマップのレンダースタート
	m_sShadowMap->RenderTarget();

	//　画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// この関数を終えたら影を描画したい対象を描画
}
//　シャドウマップ用の描画開始_L
void Deferred::ShadowBeginL()
{
#ifdef _DEBUG
	// シャドウマップが生成されていなければ書かない
	if (!m_bCascadeFlag) {
		MessageBox(tdnSystem::GetWindow(), "遠距離シャドウマップが作られていない！", __FUNCTION__, MB_OK);
		exit(-1);
	}

#endif

	// 現在のサーフェイスを一時保管
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);

	// 現在のステンシルバッファを一時保管
	tdnSystem::GetDevice()->GetDepthStencilSurface(&m_backbufferZ);
	// シャドウマップ用ステンシルバッファに切り替え
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_shadowZ);

	// 現在のビューポートの一時保管
	tdnSystem::GetDevice()->GetViewport(&m_orgViewport);
	// ビューポートの作成
	D3DVIEWPORT9 vp = { 0, 0, m_iShadowSizeL, m_iShadowSizeL, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);

	// シャドウマップLのレンダースタート
	m_sShadowMapL->RenderTarget();

	//　画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// この関数を終えたら影を描画したい対象を描画（遠距離）
}

//　シャドウマップ用の描画終了
void Deferred::ShadowEnd()
{

	//shaderD->SetValue("TU", 1.0f / shadowSize); // 2でもありかも
	//shaderD->SetValue("TV", 1.0f / shadowSize);
	//// シャドウマップの解像度でブラー処理を変化	
	//for (int i = 0; i < 2; i++)
	//{
	//	shadowMap->Render(0, 0, shadowSize, shadowSize, 0, 0, shadowSize, shadowSize, shaderD, "gaussian");//弱いぼかし
	//}

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);

	//ステンシルバッファの復元
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_backbufferZ);

	//ビューポートの復元
	tdnSystem::GetDevice()->SetViewport(&m_orgViewport);
}
//　シャドウマップ用の描画終了_L
void Deferred::ShadowEndL()
{

	// ソフトシャドウマップにシャドウマップの結果を書き込み
	/*_____________________________________________________________________________*/
	m_sSoftShadowMap->RenderTarget();
	shader->SetValue("TU", 1.0f / m_iShadowSizeL); // 2でもありかも
	shader->SetValue("TV", 1.0f / m_iShadowSizeL);

	// シャドウマップの解像度でブラー処理を変化	
	//for (int i = 0; i < 1; i++)
	{
		m_sShadowMapL->Render(0, 0, m_iShadowSizeL, m_iShadowSizeL, 0, 0, m_iShadowSizeL, m_iShadowSizeL, shader, "gaussian");//奥行を禁止
	}

	/*_____________________________________________________________________________*/

	// レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);

	// ステンシルバッファの復元
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_backbufferZ);

	//ビューポートの復元
	tdnSystem::GetDevice()->SetViewport(&m_orgViewport);
}

// シャドウの描画
void Deferred::RenderShadow()
{
	//シェーダーへ転送
	shader->SetValue("DepthTex", m_mDepth);			// 奥行用のサーフェイスを送る
	shader->SetValue("ShadowMap", m_sShadowMap);
	//shaderD->SetValue("ShadowMap", softShadowMap);	// ソフトシャドウのサーフェイスをシャドウマップとして送る
	shader->SetValue("ShadowRange", m_fShadowRange);	// 近距離の幅

	m_sShadow->RenderTarget();

	//画面クリア
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);// (?)　ZBufferを消さないようにしました?

																					// 影によりスペキュラも弱めるため2番目に設定
	m_sSpecular->RenderTarget(1);

	// シャドウマップを二つ利用しているかの判定
	if (!m_bCascadeFlag)
	{
		m_sShadow->Render(0, 0, shader, "shadow");
	}
	else
	{
		shader->SetValue("ShadowMapL", m_sSoftShadowMap);// 遠距離マップを送る(ソフト)
													   //	shaderD->SetValue("ShadowMapL", shadowMapL);// 遠距離マップを送る
		m_sShadow->Render(0, 0, shader, "DualShadow");
	}

	// 計算し作った影を後でからめる為シェーダーへ
	shader->SetValue("ShadowTex", m_sShadow);

	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

void Deferred::ShadowSetting(const float ShadowStrength, const float AdjustValue, const float DistAlpha)
{
	shader->SetValue("Shadow", 1.0f - ShadowStrength);
	shader->SetValue("AdjustValue", AdjustValue);
	shader->SetValue("DistAlpha", 1.0f - DistAlpha);
}


//****************************
//	FORWARD
//****************************

void Deferred::ClearForward()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	// フォワードをセット
	m_sForward->RenderTarget(0);
	// フォワード画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
}

// G_Buffe対象のZバッファが残っている状態にセットすること
void Deferred::ForwardBegin()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	// forward用サーフェイスに変える
	m_sForward->RenderTarget(0);

};
// G_Buffe対象のZバッファが残っている状態にセットすること
void Deferred::ForwardEnd()
{
	//diffuseに戻す
	//diffuse->RenderTarget(0);
	//作った情報テクスチャを転送
	shader->SetValue("ForwardTex", m_sForward);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
}

void Deferred::ForwardRender()
{
	// forwardで描画した絵を上からかぶせる
	m_sForward->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

}

//****************************
//		BLOOM
//****************************
//void Deferred::ClearBloom()
//{
//	// 今のサーフェイスを保存
//	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
//	// Bloomを描画する元の
//	m_sBloomScreen->RenderTarget();
//	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
//
//	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
//};
//void Deferred::BeginDrawBloom()
//{
//	// まずは今のサーフェイスを保存
//	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
//	m_sBloomScreen->RenderTarget();//
//
//	//tdnSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//Zバッファに書き込まない
//	// ↓から映すであろう一枚絵は次から描画するものの為に
//	//　Zバッファに書き込まないようにする
//
//	// ※IEXのRenderがDefaltで書き込むように設定してるので　描画の対象に　RS_COPY_NOZとするしかない
//};
//void Deferred::EndDrawBloom()
//{
//	// ↑でZバッファをOFFにしていたので戻す
//	//tdnSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);//Zバッファに書き込む
//
//	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
//};

// ToneMap時に自動で高輝度の種を作る用にしたので↑の処理をコメントアウト
void Deferred::BloomRender()
{
	D3DVIEWPORT9 save_viewport;//ビューポートを一時保存
	tdnSystem::GetDevice()->GetViewport(&save_viewport);

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	//テクスチャのサイズのビューポート設定
	D3DVIEWPORT9 v = { 0, 0, MiniTex_x, MiniTex_y, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&v);

	m_sBloom->RenderTarget();
	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	shader->SetValue("TU", (float)1.25f / (float)MiniTex_x);
	shader->SetValue("TV", (float)1.25f / (float)MiniTex_y);

	// 画面の明るい部分をブルームの種として抽出
	//m_sBloomScreen->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0,
	//	tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, shader, "make_hdrBloomSeed");

	m_sBloomSeed->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0,
			tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

	//ボケ度　回数が少ないほどボケない
	for (int i = 0; i < 2; i++)
	{
		m_sBloom->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0, MiniTex_x, MiniTex_y, shader, "gaussZ");//奥行を禁止
	}


	tdnSystem::GetDevice()->SetViewport(&save_viewport);
	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);

	// HDRブルームを乗算　
	m_sBloom->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom,
		0, 0, MiniTex_x, MiniTex_y, shader, "add_hdrBloom");

};


//****************************
//		GPU_POINTLIGHT
//****************************

void  Deferred::ClearPLSdata()
{
	PLSdata.clear();
}

void  Deferred::AddPLSdata(const Vector3 pos, const Vector3 color, const float range, const float power)
{
	PLSData data;

	// 今のままだとカリングが意味をなさない
	Matrix mWV = matView;// ビュー行列
						 // 視錐台カリング処理
	Vector3 flont(mWV._13, mWV._23, mWV._33);
	flont.Normalize();
	Vector3 dir = pos - (m_vViewPos - flont * range);			//レンジの値分下げて、カリングを緩める。
	dir.Normalize();
	float dot = Vector3Dot(dir, flont);
	if (dot < 0.2f)
	{
		return;			// 見えていないポイントライトを消去
	}

	// ワールド座標のデータも詰める
	data.wpos = pos;

	// カメラ空間変換
	Matrix mat = matView;
	data.pos.x = pos.x * mat._11 + pos.y * mat._21 + pos.z * mat._31 + mat._41;
	data.pos.y = pos.x * mat._12 + pos.y * mat._22 + pos.z * mat._32 + mat._42;
	data.pos.z = pos.x * mat._13 + pos.y * mat._23 + pos.z * mat._33 + mat._43;

	data.range = range;
	data.color = color;
	data.power = power;


	if ((int)PLSdata.size() >= PLS_MAX)assert(0);

	// 配列に加える
	PLSdata.push_back(data);



}


void Deferred::ClearGpuPointLight()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	// フォワードをセット
	m_sGpuPointLight->RenderTarget(0);
	// フォワード画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);

}

void Deferred::GpuPointLightRender()
{

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);

	// GPUポイントライト
	m_sGpuPointLight->RenderTarget(0);

	// ここで描画
	for (int i = 0; i < (int)PLSdata.size(); i++)
	{
		// カメラ空間変換したポジションを入れる
		shader->SetValue("PLSpos", PLSdata[i].pos);
		shader->SetValue("PLScolor", PLSdata[i].color);
		shader->SetValue("PLSrange", PLSdata[i].range);
		shader->SetValue("PLSpower", PLSdata[i].power);

		//PointLightBall->SetScale(30.0f);
		m_PLS->SetPos(PLSdata[i].wpos);
		m_PLS->Update();
		m_PLS->Render(shader, "PointLightSphere");

	}

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);


	// ファイナルリザルトに贈る
	shader->SetValue("PLSTex", m_sGpuPointLight);


}


//****************************
//		DOWNSAMPLE
//****************************
void Deferred::CtrlExposure()
{
	// 露光度
	if (KeyBoard(KB_C))
	{
		// 動く
		if (KeyBoard(KB_LEFT))
		{
			m_fExposure -= 0.1f;
		}
		else if(KeyBoard(KB_RIGHT))
		{
			m_fExposure += 0.1f;
		}

	}
	// 動く
	if (KeyBoard(KB_P))
	{
		m_fExposure -= 0.05f;
	}
	else if (KeyBoard(KB_O))
	{
		m_fExposure += 0.05f;
	}


	tdnText::Draw(10, 600, 0xff00ffff, "露光度%.1f", m_fExposure);
	shader->SetValue("exposure", m_fExposure);
}

//***********************************
//				Rendering
//***********************************

// 最後のスクリーンを作る
void Deferred::FinalResultDeferred()
{
	//現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//シェーダーへ転送
	//shader->SetValue("LightTex", light);
	//shader->SetValue("SpecTex", spec);

	// 通常描画
	DefaultRender();

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);

}


// デフォルト　平行光＋スペキュラ
void Deferred::DefaultRender()
{
	// FinalPassSurfaceに今までのSurfaceの情報を全て合わせる
	m_sScreen->RenderTarget();

	// 画面クリア
	// しかし毎回全て描画するため問題なさそうなのでコメントアウト
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	// 仮　ディフーズ
	m_mDiffuse->Render(0, 0);

	// 中でトーンマッピング
	m_sScreen->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

}

//  Deferred
void Deferred::Render()
{
	// 最後の演算結果をシェーダー側へ送る
	shader->SetValue("LightTex", m_sLight);
	shader->SetValue("SpecTex", m_sSpecular);

	// シャドウサーフェイスの更新
	if (m_bShadowFlag == true)
	{
		RenderShadow();
	}

	// FinalPassSurfaceに今までのSurfaceの情報を全て合わせる
	m_sScreen->RenderTarget();
	m_sBloomSeed->RenderTarget(1); // BloomSeed

	if (m_bShadowFlag == true)
	{
		m_mDiffuse->Render(0, 0, shader, "finalPut_Shadow");		//通常描画
	}
	else
	{
		m_mDiffuse->Render(0, 0, shader, "finalPut");		//影なし描画
	}


	// ここで描画
	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);
	
	// ディファードのスクリーン
	m_sScreen->Render(0, 0);

	// フォアード（エフェクト）
	m_sForward->Render(0, 0, shader, "ToneMap");

	// フォアードの①バックバッファ　②ブルーム　という形にしました
	// 高輝度のサーフェイスを戻す
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);

	////ライトサーフェイス画面クリア
	//m_sLight->RenderTarget();
	//m_sSpecular->RenderTarget(1);
	////バックバッファサーフェイスの復元・MRTの終了処理
	//tdnSystem::GetDevice()->SetRenderTarget(SURFACE_NAME::MRT_NORMAL, nullptr);
	//tdnSystem::GetDevice()->SetRenderTarget(SURFACE_NAME::MRT_SPECULAR, nullptr);

	// LOD
	m_sFog->Render(0, 0, RS::ADD);// フォグを重ねる

	tdnText::Draw(100, 600,0xff00ffff, "m_fExposure%.2f", m_fExposure);
	//m_sSpecular->Render(0, 0, shader, "ToneMap");
}

//******************************************
//				texture
//******************************************

tdn2DObj* Deferred::GetTex(const int type)
{
	tdn2DObj* ret = nullptr;

	switch (type) {
	case MRT_DIFFUSE:
		ret = m_mDiffuse;
		break;
	case MRT_NORMAL:
		ret = m_mNormal;
		break;
	case MRT_ROUGHNESS:
		ret = m_mRoughness;
		break;
	case MRT_DEPTH:
		ret = m_mDepth;
		break;
	case LIGHT:
		ret = m_sLight;
		break;
	case SPECULAR:
		ret = m_sSpecular;
		break;
	case FOG:
		ret = m_sFog;
		break;
	case SHADOW:
		ret = m_sShadow;
		break;
	case SHADOWMAP:
		ret = m_sShadowMap;
		break;
	case SHADOWMAPL:
		ret = m_sShadowMapL;
		break;
	case FORWARD:
		ret = m_sForward;
		break;
	case BLOOM:
		ret = m_sBloom;
		break;
	case BLOOMSEED:
		ret = m_sBloomSeed;
		break;
	case GPUPL:
		ret = m_sGpuPointLight;
		break;
	case SCREEN:
		ret = m_sScreen;
		break;
	default:
		MessageBox(tdnSystem::GetWindow(), "サーフェイスがなかった", "Deferred::GetTex", MB_OK);
		break;
	}
	return ret;
}

