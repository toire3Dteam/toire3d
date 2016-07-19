#pragma once
#include "TDNLIB.h"
#include "../system/System.h"

/*
	Deferred
*/

//　シャドウマップのサイズ
#define SHADOWSIZE 1024

enum  SURFACE_NAME {
	MRT_DIFFUSE, MRT_NORMAL, MRT_ROUGHNESS, MRT_DEPTH,		//G-Buffer
	LIGHT, SPECULAR, DOF, SHADOW, SHADOWMAP, SHADOWMAPL, FOG,
	BLOOMSCREEN, BLOOM, GLOWSCREEN, GLOW, FORWARD, DOWNSAMPLE, INFOPL, SKIN, BLOOMSEED, CLOSS_GLARE_X, CLOSS_GLARE_Y,
	GPUPL,//GPUPARTICLE,
	GODRAY,
	SCREEN
};

class Deferred
{
private:
	//初期化関数
	void Init();

	Deferred();
	~Deferred();
	static Deferred* pInstance;
public:
	// Instance
	static Deferred& GetInstance();
	static void Release();

	//***************************
	//		Deferred_Set
	//***************************
	void Begin();
	void End();
	void Update(const Vector3 ViewPos);

	//***************************
	//		CubeMap
	//***************************
	void CubeSet(char* name);


	//***************************
	//		Lighting
	//***************************
	// 平行光
	void DirLight(const Vector3 dir, const Vector3 color);
	// 半球ライティング
	void HemiLight(const Vector3 SkyColor, const Vector3 GroundColor);
	// フォグ
	void Fog(const float FogNear, const float FogFar, const Vector3 FogColor);

	//****************************
	//		ShadowMaps
	//****************************
	void CreateShadowMap(const int size = SHADOWSIZE);
	void CreateShadowMapL(const int size = SHADOWSIZE);
	void CreateShadowMatrix(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist = 100.0f);
	void CreateShadowMatrixL(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist = 100.0f);
	void ShadowBegin();
	void ShadowEnd();
	void ShadowBeginL();
	void ShadowEndL();
	void ShadowSetting(const float ShadowStrength, const float AdjustValue, const float DistAlpha = .75f);

	//****************************
	//		FORWARD
	//****************************
	// G_Buffe対象のZバッファが残っている状態で描画すること
	void ClearForward();
	void ForwardBegin();
	void ForwardEnd();
	void ForwardRender();

	//****************************
	//		BLOOM
	//****************************
	//void ClearBloom();
	//void BeginDrawBloom();
	//void EndDrawBloom();
	void BloomRender();

	//****************************
	//		SKIN
	//****************************
	void SkinMask();

	//****************************
	//		GPU_POINTLIGHT
	//****************************
	void ClearPLSdata();
	void AddPLSdata(const Vector3 pos, const Vector3 color, const float range = 100.0f, const float power = 2.0f);
	void ClearGpuPointLight();
	void GpuPointLightRender();

	//****************************
	//		スポットライト
	//****************************
	void SpotLight();


	//****************************
	//		DOWNSAMPLE
	//****************************
	float GetExposure() { return m_fExposure; }
	void SetExposure(float exposure) { this->m_fExposure = exposure; }
	void AddExposure(float exposure) { this->m_fExposure += exposure; }
	void CtrlExposure();

	//****************************
	//		DeferredRender
	//****************************
	void FinalResultDeferred();	// G_Bufferを合わせる
	void DefaultRender();// デフォルト
	void RenderShadow();// 

	void Render();

	//ゲッター・セッター
	tdn2DObj* GetTex(const int type); //テクスチャを取ってくる


private:

	// G-Buffer　MRT (マルチレンダーターゲット)
	tdn2DObj* m_mDiffuse;
	tdn2DObj* m_mNormal;
	tdn2DObj* m_mRoughness;
	tdn2DObj* m_mDepth;

	// G-Bufferを利用するSecondSurface
	tdn2DObj* m_sLight;			// ライト用サーフェイス	
	tdn2DObj* m_sSpecular;			// スペキュラ用サーフェイス
	tdn2DObj* m_sFog;			// フォグ用サーフェイス
	tdn2DObj* m_sShadow;		// シャドウ用サーフェイス



    // FinalPassSurface
	tdn2DObj* m_sScreen;			// 演算結果をまとめたサーフェイス	
	tdn2DObj* m_sBloomScreen;
	tdn2DObj* m_sBloom;
	tdn2DObj* m_sBloomSeed;			// 高輝度の種

	tdn2DObj* m_sCrossGlareX;			// クロスグレア
	tdn2DObj* m_sCrossGlareY;			// クロスグレア

	// ForwardSurface
	tdn2DObj* m_sForward;			// deferredで通さず直接描画するサーフェイス

	// GpuPointLight
	tdn2DObj* m_sGpuPointLight;		// Gpuポイントライト


	// BackBufferSurface 		
	Surface* backbuffer;	// 最終的に画面に映すサーフェイス
	// バックステンシルバッファサーフェイス
	Surface* m_backbufferZ;

	// カメラの位置
	Vector3 m_vViewPos;

	// キューブテクスチャ
	//CubeTex* m_cubeTex;

	// 保存用サーフェイス
	Surface* m_savebackbuffer;
	D3DVIEWPORT9	m_saveViewport;// ビューポート保存用


	/*************************/
	//	PointLight用
	/*************************/
	typedef struct
	{
		Vector3 pos;
		float range;
		Vector3 color;
		float power;
		Vector3 wpos;
	} PLSData;
	std::vector<PLSData> PLSdata;	// ポイントライトスフィアのデータ
	static const int PLS_MAX = 64;	// ポイントライトスフィアの限界値
	iexMesh* m_PLS;

	//****************************
	//		ShadowMap
	//****************************
	//シャドウマップ書き込み時ステンシルバッファサーフェイス
	Surface*		m_shadowZ;
	Surface*		m_orgZ;		// Zバッファ保存用
	D3DVIEWPORT9	m_orgViewport;// ビューポート保存用

	tdn2DObj*	m_sShadowMap;		// ｚ値テクスチャ
	tdn2DObj*	m_sShadowMapL;		// 遠距離用マップ
	tdn2DObj*	m_sSoftShadowMap;	// ソフトシャドウマップ

	int		m_iShadowSize;			// シャドウマップのサイズ
	int		m_iShadowSizeL;		// シャドウマップのサイズL
	float	m_fShadowRange;		// Shadowのカメラからの距離

								//シャドウマップの使用フラグ
	bool m_bShadowFlag;
	bool m_bCascadeFlag;

	// 露光レベル
	float m_fExposure;
};

#define DeferredManager (Deferred::GetInstance())
