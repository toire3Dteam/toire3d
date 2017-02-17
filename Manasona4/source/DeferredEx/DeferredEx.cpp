#include "DeferredEx.h"

//+-------------------------------------
//	DeferredEX
//+-------------------------------------

DeferredEx* DeferredEx::m_pInstance = nullptr;

//+-------------------------------------
// インスタンス
//+-------------------------------------
DeferredEx& DeferredEx::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new DeferredEx();
	}
	return *m_pInstance;
}

void DeferredEx::Release()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/* マップの読み込み情報メモ */
// N RGB->法線 
// L R->Rou G->ラフネス B->
// H R->AO G->エミッシブ追加予定　 B->空白

//+-------------------------------------
// 初期化・解放
//+-------------------------------------
DeferredEx::DeferredEx()
{
	// 極小バッファのサイズ
	MiniSizeLv = 6;
	MiniSizeX = tdnSystem::GetScreenSize().right / MiniSizeLv;
	MiniSizeY = tdnSystem::GetScreenSize().bottom / MiniSizeLv;

	// 露光度
	m_fExposure = 0.0f;

	// 目線
	m_vViewPos = Vector3(.0f, .0f, .0f);

	// 放射ブラ―
	m_blurValue = 2.0f;
	m_blurPower = 0.0f;

	// サーフェイス初期化
	for (int i = 0; i < (int)SURFACE_NAME_EX::ARRAY_END; i++)
	{
		switch ((SURFACE_NAME_EX)i)
		{
		case SURFACE_NAME_EX::NORMAL_DEPTH:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);	 // RG 法線　BA 深度;

			break;
		case SURFACE_NAME_EX::ROUGHNESS:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);	

			break;
		case SURFACE_NAME_EX::LIGHT:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

			break;
		case SURFACE_NAME_EX::SPEC:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

			break;
		case SURFACE_NAME_EX::FORWARD:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);

			break;
		case SURFACE_NAME_EX::BLOOM:
			m_pSurface[i] = new tdn2DObj(MiniSizeX, MiniSizeY, TDN2D::HDR);

			break;
		case SURFACE_NAME_EX::BLOOM_SEED:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);

			break;
		case SURFACE_NAME_EX::SCREEN:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);// アルファを捨てる

			break;
		case SURFACE_NAME_EX::POINT_LIGHT:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

			break;
		case SURFACE_NAME_EX::SHADOW_MAP:
			m_pSurface[i] = nullptr;// ここでは作らない

			break;
		default:
			assert(0); // 初期化されていないサーフェイスがある
			break;
		}

	}

	// BackBufferの保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);
	// 初期化
	m_pSaveBackBuffer = nullptr;
	m_pSaveStencilSurface = nullptr;		// ステンシル保存用
	m_pShadowStencilSurface = nullptr;		// シャドウ用ステンシル
	m_iShadowSize = 0;						// シャドウマップのサイズ
	m_fShadowRange = 0.0f;					// Shadowのカメラからの距離	
	m_bShadowFlag = false;					// シャドウマップの使用フラグ

	// Gpu_PointLight
	m_PLSdata.reserve(PLS_MAX);// 前もって容量を確保 (reserve) しておけば、データの追加時に発生する再割り当て (reallocation) を防ぐことができ、効率的です。 
	m_pPLS = new iexMesh("DATA/Shader/PLS_CCW.imo");

	m_bPLSInstancing = false;

	// ToonTex
	m_pToonShadoTex = new tdn2DObj("Data/Shader/toonShadow.png");
	shaderM->SetValue("ToonShadowMap", m_pToonShadoTex);

}

DeferredEx::~DeferredEx()
{
	// サーフェイス解放
	for (int i = 0; i < (int)SURFACE_NAME_EX::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pSurface[i]);
	}

	SAFE_DELETE(m_pPLS);
	SAFE_DELETE(m_pToonShadoTex);

	//Surfaceの解放	
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pSaveBackBuffer);
	SAFE_RELEASE(m_pSaveStencilSurface);
	SAFE_RELEASE(m_pShadowStencilSurface);

	if (m_bPLSInstancing == true)
	{
		SAFE_DELETE(m_pPLSInstancing);
	}

}

//+-------------------------------------
// G_Bufferの作成
//+-------------------------------------
void DeferredEx::G_Begin()
{	
	// BackBufferサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);

	//MRTの割り当て
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::ROUGHNESS]->RenderTarget(1);

}

void DeferredEx::G_End()
{
	// バックバッファサーフェイスの復元・MRTの終了処理
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);

	// 作った情報テクスチャを転送
	shaderM->SetValue("NormalDepthTex", m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]);
	shaderM->SetValue("RoughnessLightMapTex", m_pSurface[(int)SURFACE_NAME_EX::ROUGHNESS]);

}

// ※このUpdateはレンダーで呼んでください
// Deferredの更新
void DeferredEx::G_Update(const Vector3 g_vViewPos)
{
	// トーンマッピング操作
	CtrlExposure();

	// 目線
	m_vViewPos = g_vViewPos;
	shaderM->SetValue("g_vViewPos", m_vViewPos);

	// プロジェクション逆行列生成
	Matrix invproj = matProjection;
	D3DXMatrixInverse(&invproj, nullptr, &invproj);
	shaderM->SetValue("g_mInvPMatrix", invproj);

	// プロジェクション→ワールド逆行列生成
	Matrix invview = matView;
	D3DXMatrixInverse(&invview, nullptr, &invview);
	shaderM->SetValue("g_mInvVMatrix", invview);
	shaderM->SetValue("g_mInvVPMatrix", invproj * invview);

	/*************************************/
	//	変換行列をシェーダ側に贈る
	/*************************************/
	// View変換行列 
	shaderM->SetValue("g_mVMatrix", matView);
	// Projection変換行列 
	shaderM->SetValue("g_mPMatrix", matProjection);

	// View*Proj変換行列
	shaderM->SetValue("g_mVPMatrix", matView*matProjection);


}

//+-------------------------------------
//				Lighting
//+-------------------------------------
// 光のサーフェイスクリア
void DeferredEx::ClearLightSurface()
{
	// 現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	// ライトサーフェイス画面クリア
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::SPEC]->RenderTarget(1);
	// ライトサーフェイス画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	// レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// 平行光
void DeferredEx::DirLight(const Vector3 dir, const Vector3 color)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ワールドの平行光を渡す
	Vector3 vWLightDir = dir;
	vWLightDir.Normalize();
	shaderM->SetValue("g_vWLightVec", vWLightDir);

	// ビュー座標系に変換
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;

	LightDir.Normalize();

	// シェーダー設定 shaderに送る
	shaderM->SetValue("g_vViewLightVec", LightDir);
	shaderM->SetValue("g_vLightColor", (Vector3)color);

	//現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//レンダーターゲットの切替え
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget();
	m_pSurface[(int)SURFACE_NAME_EX::SPEC]->RenderTarget(1);

	//	レンダリング
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->Render(0, 0, shaderM, "DirLightShadow");

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// 半球ライティング
void DeferredEx::HemiLight(const Vector3 g_vSkyColor, const Vector3 g_vGroundColor)
{
	// シェーダー設定
	shaderM->SetValue("g_vSkyColor", (Vector3)g_vSkyColor);
	shaderM->SetValue("g_vGroundColor", (Vector3)g_vGroundColor);

	// 現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	// レンダーターゲットの切替え
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget();

	// レンダリング
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->Render(0, 0, shaderM, "HemiLight");

	// レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
}

void DeferredEx::AllLight(const Vector3 dir, const Vector3 color, const Vector3 g_vSkyColor, const Vector3 g_vGroundColor)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ワールドの平行光を渡す
	Vector3 vWLightDir = dir;
	vWLightDir.Normalize();
	shaderM->SetValue("g_vWLightVec", vWLightDir);

	//ビュー座標系に変換
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;


	LightDir.Normalize();

	//	シェーダー設定 
	shaderM->SetValue("g_vViewLightVec", LightDir);
	shaderM->SetValue("g_vLightColor", (Vector3)color);
	shaderM->SetValue("g_vSkyColor", (Vector3)g_vSkyColor);
	shaderM->SetValue("g_vGroundColor", (Vector3)g_vGroundColor);

	//現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//レンダーターゲットの切替え
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget();
	m_pSurface[(int)SURFACE_NAME_EX::SPEC]->RenderTarget(1);

	//	レンダリング
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->Render(0, 0, shaderM, "AllLight");

	//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);



}


//+-------------------------------------
//	FORWARD
//+-------------------------------------

void DeferredEx::ClearForward()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// フォワードをセット
	m_pSurface[(int)SURFACE_NAME_EX::FORWARD]->RenderTarget(0);
	// フォワード画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);
}

// G_Buffe対象のZバッファが残っている状態にセットすること
void DeferredEx::ForwardBegin()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// forward用サーフェイスに変える
	m_pSurface[(int)SURFACE_NAME_EX::FORWARD]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->RenderTarget(1);

};
// G_Buffe対象のZバッファが残っている状態にセットすること
void DeferredEx::ForwardEnd()
{
	//diffuseに戻す
	//diffuse->RenderTarget(0);
	//作った情報テクスチャを転送
	shaderM->SetValue("ForwardTex", m_pSurface[(int)SURFACE_NAME_EX::FORWARD]);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

void DeferredEx::ForwardRender()
{
	// forwardで描画した絵を上からかぶせる
	m_pSurface[(int)SURFACE_NAME_EX::FORWARD]->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

}

//+-------------------------------------
//		BLOOM
//+-------------------------------------

void DeferredEx::ClearBloom()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// セット
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->RenderTarget(0);
	// 画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);
}

// ToneMap時に自動で高輝度の種を作る用にしたので↑の処理をコメントアウト
void DeferredEx::BloomRender()
{
	D3DVIEWPORT9 save_viewport;//ビューポートを一時保存
	tdnSystem::GetDevice()->GetViewport(&save_viewport);

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	//テクスチャのサイズのビューポート設定
	D3DVIEWPORT9 v = { 0, 0, MiniSizeX, MiniSizeY, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&v);

	m_pSurface[(int)SURFACE_NAME_EX::BLOOM]->RenderTarget();
	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	shaderM->SetValue("g_fTU", (float)1.25f / (float)MiniSizeX);
	shaderM->SetValue("g_fTV", (float)1.25f / (float)MiniSizeY);

	// 画面の明るい部分をブルームの種として抽出
	//m_sBloomScreen->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0,
	//	tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, shader, "make_hdrBloomSeed");

	// ブルームの種に映ってある物を描画
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->
		Render(0, 0, MiniSizeX, MiniSizeY, 0, 0,
		tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

	//ボケ度　回数が少ないほどボケない が処理は軽くなる
	for (int i = 0; i < 2; i++)
	{
		m_pSurface[(int)SURFACE_NAME_EX::BLOOM]->Render(0, 0, MiniSizeX, MiniSizeY,
			0, 0, MiniSizeX, MiniSizeY, shaderM, "gaussZ");//奥行を禁止
	}

	tdnSystem::GetDevice()->SetViewport(&save_viewport);
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);

	// HDRブルームを乗算　
	//for (int i = 0; i < 2; i++)// 回数が大きいほどブルームが強くなる
	{
		m_pSurface[(int)SURFACE_NAME_EX::BLOOM]->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom,
			0, 0, MiniSizeX, MiniSizeY, shaderM, "add_hdrBloom");
	}
};


//+-------------------------------------
//		GPU_POINTLIGHT
//+-------------------------------------

void  DeferredEx::ClearPLSdata()
{
	m_PLSdata.clear();
}

void  DeferredEx::AddPLSdata(const Vector3 pos, const Vector3 color, const float range, const float power)
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


	if ((int)m_PLSdata.size() >= PLS_MAX)
	{

	#ifdef _DEBUG
		MyAssert(0, "それ以上ライトはおけない");
	#endif // _DEBUG
		return;
	}

	// 配列に加える
	m_PLSdata.push_back(data);



}


void DeferredEx::ClearGpuPointLight()
{
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// ポイントライトサーフェイスをセット
	m_pSurface[(int)SURFACE_NAME_EX::POINT_LIGHT]->RenderTarget(0);
	// ポイントライト画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);

}

void DeferredEx::GpuPointLightRender()
{

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);

	// GPUポイントライト
	m_pSurface[(int)SURFACE_NAME_EX::POINT_LIGHT]->RenderTarget(0);

	// 静止画用
	if (m_bPLSInstancing == true)
	{
		m_pPLSInstancing->InstancingRender();
	}

	// ここで描画
	for (int i = 0; i < (int)m_PLSdata.size(); i++)
	{
		// カメラ空間変換したポジションを入れる
		shaderM->SetValue("g_vPLSpos", m_PLSdata[i].pos);
		shaderM->SetValue("g_vPLScolor", m_PLSdata[i].color);
		shaderM->SetValue("g_fPLSrange", m_PLSdata[i].range);
		shaderM->SetValue("g_fPLSpower", m_PLSdata[i].power);

		//PointLightBall->SetScale(30.0f);
		m_pPLS->SetPos(m_PLSdata[i].wpos);
		m_pPLS->Update();
		m_pPLS->Render(shaderM, "PointLightSphere");

	}

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);


	// ファイナルリザルトに贈る
	shaderM->SetValue("PLSMap", m_pSurface[(int)SURFACE_NAME_EX::POINT_LIGHT]);


}

// 初期化
void DeferredEx::InitPointLightInstancing(const int iIndex)
{
#ifdef _DEBUG
	MyAssert((m_bPLSInstancing == false), "すでに生成されている");
#endif // _DEBUG

	m_bPLSInstancing = true;
	m_pPLSInstancing = new PointLightInstancingMesh("Data/Shader/PLS_CCW.imo", iIndex);

}

//+-------------------------------------
//	シャドウ
//+-------------------------------------

//　シャドウの初期化
void DeferredEx::InitShadowMap(const int size )
{
	if (m_bShadowFlag == true) return;	// 2回以上、シャドウマップを生成させない。

	// シャドウマップのサイズを確保
	m_iShadowSize = size;

	// シャドウマップの生成
	m_pSurface[(int)SURFACE_NAME_EX::SHADOW_MAP]= new tdn2DObj(size, size, TDN2D::FLOAT1);		//r成分にzバッファg成分にzバッファの二乗の値が入る

	//シャドウマップ用ステンシルバッファサーフェイスの作成!
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pShadowStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "深度バッファが作成できなかった", "ERROR", MB_OK);
	};

	m_bShadowFlag = true;	// シャドウマップ採用
}

// シャドウの行列生成
void DeferredEx::CreateShadowMatrix
(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{
	// シャドウマップが生成されていなければ転送しない
	if (!m_bShadowFlag)
	{
		MessageBox(tdnSystem::GetWindow(), "シャドウマップを作っていない", __FUNCTION__, MB_OK);
		exit(-1);
	}

	// シャドウ作成
	dir.Normalize();
	Vector3 pos = target - dir * dist;
	Vector3 up(.0f, 1.0f, .0f);

	// 幅の指定
	float width = dist;

	// 視点とライト位置へ
	Matrix	ShadowMat, work;
	// ビュー行列の設定
	Math::LookAtLH(ShadowMat, pos + (playerVec), target + (playerVec), up);
	// 平行投影行列作成 ビューボリューム
	Math::OlthoLH(work, width, width, 0.02f, width * 2.5f);	//平行投影行列
	ShadowMat *= work;
	// 合成する
	shaderM->SetValue("g_mShadowProjection", ShadowMat);

	// アルファ調整
	m_fShadowRange = width * 0.25f;
}

//　シャドウマップ用の描画開始
void DeferredEx::ShadowBegin()
{

#ifdef _DEBUG
	
	// シャドウマップが生成されていなければ書かない
	if (m_bShadowFlag == false)
	{
		MyAssert(0, "シャドウマップが生成されていない状態で影を書き込もうとしています");
	}

#endif // _DEBUG


	// 現在のサーフェイスを一時保管
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);
	
	// 現在のステンシルバッファを一時保管
	tdnSystem::GetDevice()->GetDepthStencilSurface(&m_pSaveStencilSurface);
	
	// シャドウマップ用ステンシルバッファに切り替え
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pShadowStencilSurface);

	// 現在のビューポートの一時保管
	tdnSystem::GetDevice()->GetViewport(&m_pSaveViewPort);
	
	// ビューポートの作成
	D3DVIEWPORT9 vp = { 0, 0, m_iShadowSize, m_iShadowSize, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);

	// シャドウマップのレンダースタート
	m_pSurface[(int)SURFACE_NAME_EX::SHADOW_MAP]->RenderTarget();

	// 画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// この関数を終えたら影を描画したい対象を描画
}

//　シャドウマップ用の描画終了
void DeferredEx::ShadowEnd()
{

	//shaderD->SetValue("g_fTU", 1.0f / shadowSize); // 2でもありかも
	//shaderD->SetValue("g_fTV", 1.0f / shadowSize);
	//// シャドウマップの解像度でブラー処理を変化	
	//for (int i = 0; i < 2; i++)
	//{
	//	shadowMap->Render(0, 0, shadowSize, shadowSize, 0, 0, shadowSize, shadowSize, shaderD, "gaussian");//弱いぼかし
	//}
	shaderM->SetValue("ShadowMap", m_pSurface[(int)SURFACE_NAME_EX::SHADOW_MAP]);

	// レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

	// ステンシルバッファの復元
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pSaveStencilSurface);

	// ビューポートの復元
	tdnSystem::GetDevice()->SetViewport(&m_pSaveViewPort);
}


//+-------------------------------------
//		DOWNSAMPLE
//+-------------------------------------
void DeferredEx::CtrlExposure()
{

	shaderM->SetValue("g_fExposure", m_fExposure);

#ifdef _DEBUG

	//tdnText::Draw(10, 600, 0xff00ffff, "露光度%.1f", m_fExposure);

	// 露光度
	if (KeyBoard(KB_C))
	{
		if (KeyBoard(KB_LEFT))
		{
			m_fExposure -= 0.1f;
		}
		else if (KeyBoard(KB_RIGHT))
		{
			m_fExposure += 0.1f;
		}

	}
	if (KeyBoard(KB_P))
	{
		m_fExposure -= 0.05f;
	}
	else if (KeyBoard(KB_O))
	{
		m_fExposure += 0.05f;
	}
#endif // _DEBUG

}

//+-------------------------------------
//		RADIAL_BLUR
//+-------------------------------------
void DeferredEx::SetRadialBlur(Vector3 pos, float power)
{
	// ブラ―
	m_blurPower = power;
	shaderM->SetValue("BluePower", m_blurPower);

	// スクリーン座標に変換
	Vector2 screenPos = Math::WorldToProj(pos);
	
	// 場所
	shaderM->SetValue("g_fCenterX", screenPos.x);
	shaderM->SetValue("g_fCenterY", screenPos.y);

}

void DeferredEx::SetRadialBlur(Vector2 pos, float power)
{
	// ブラ―
	m_blurPower = power;
	shaderM->SetValue("BluePower", m_blurPower);

	// 場所
	shaderM->SetValue("g_fCenterX", pos.x);
	shaderM->SetValue("g_fCenterY", pos.y);

}

void DeferredEx::RadialBlurUpdate()
{
	m_blurPower -= 0.25f;
	m_blurPower = Math::Clamp(m_blurPower, 0.0f, 30.0f);

	// ブラ―
	shaderM->SetValue("g_fBlurPower", m_blurPower);
	shaderM->SetValue("g_fBlurValue", m_blurValue);

}

//+-------------------------------------
//		FINAL
//+-------------------------------------
void DeferredEx::FinalBegin()
{
	// 最後の演算結果をシェーダー側へ送る
	shaderM->SetValue("LightMap", m_pSurface[(int)SURFACE_NAME_EX::LIGHT]);
	shaderM->SetValue("SpecMap", m_pSurface[(int)SURFACE_NAME_EX::SPEC]);

	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);

	// サーフェイス切り替え
	m_pSurface[(int)SURFACE_NAME_EX::SCREEN]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->RenderTarget(1);

}


void DeferredEx::FinalEnd()
{
	// フォアードで描画していたものを乗っける
	//ForwardRender();

	// フォアードの①バックバッファ　②ブルーム　という形にしました
	// 高輝度のサーフェイスを戻す
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);//レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);

	// 描画
	m_pSurface[(int)SURFACE_NAME_EX::SCREEN]->Render(0, 0,shaderM,"RadialBlur");

}

void DeferredEx::SendFinalScreenEnv()
{
	shaderM->SetValue("EnvFullBuf", m_pSurface[(int)SURFACE_NAME_EX::SCREEN]);
}

//+-------------------------------------
//				texture
//+-------------------------------------
// テクスチャを取ってくる
tdn2DObj* DeferredEx::GetTex(const SURFACE_NAME_EX name) 
{
	// エラー処理
	switch (name)
	{
	case SURFACE_NAME_EX::ARRAY_END:
		assert(0); // ↑これは配列の終わりです
		break;
	}

	return  m_pSurface[(int)name];
}
