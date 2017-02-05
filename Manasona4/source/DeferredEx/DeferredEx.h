#pragma once
#include "TDNLIB.h"
#include "../system/System.h"

enum class SURFACE_NAME_EX
{
	NORMAL_DEPTH, ROUGHNESS,
	LIGHT,SPEC,
	FORWARD,
	BLOOM, BLOOM_SEED,
	SCREEN,
	POINT_LIGHT,
	SHADOW_MAP,

	ARRAY_END// 配列の終わり
};

class DeferredEx
{
private:
	DeferredEx();
	~DeferredEx();
	DeferredEx(const DeferredEx&){}
	DeferredEx& operator=(const DeferredEx&){}
	static DeferredEx* pInstance;

public:
	// シングルトン
	static DeferredEx& GetInstance();
	static void Release();

	//***************************
	//		G_Buffer
	//***************************
	void G_Begin();
	void G_End();
	void G_Update(const Vector3 g_vViewPos);

	//***************************
	//		Lighting
	//***************************
	void ClearLightSurface();
	// 平行光
	void DirLight(const Vector3 dir, const Vector3 color);
	// 半球ライティング
	void HemiLight(const Vector3 g_vSkyColor, const Vector3 g_vGroundColor);
	// 平行光＋半球ライティング
	void AllLight(const Vector3 dir, const Vector3 color, const Vector3 g_vSkyColor, const Vector3 g_vGroundColor);
	
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
	void ClearBloom();
	void BloomRender();

	//****************************
	//		GPU_POINTLIGHT
	//****************************
	void ClearPLSdata();
	void AddPLSdata(const Vector3 pos, const Vector3 color, const float range = 100.0f, const float power = 2.0f);
	void ClearGpuPointLight();
	void GpuPointLightRender();

	//****************************
	//		SHADOW
	//****************************
	void InitShadowMap(const int size = 1024);
	void CreateShadog_mWMatrix(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist = 100.0f);

	void ShadowBegin();
	void ShadowEnd();


	//****************************
	//		DOWNSAMPLE
	//****************************
	float GetExposure() { return m_fExposure; }
	void SetExposure(float g_fExposure) { this->m_fExposure = g_fExposure; }
	void AddExposure(float g_fExposure) { this->m_fExposure += g_fExposure; }
	void CtrlExposure();

	//****************************
	//		RADIAL_BLUR
	//****************************
	void SetRadialBlur(Vector3 pos, float power);
	void SetRadialBlur(Vector2 pos, float power);
	void RadialBlurUpdate();
	//void RadialBlurRender();

	//****************************
	//		FINAL
	//****************************
	void FinalBegin();
	void FinalEnd();

	//****************************
	//		ENVIRONMENT
	//****************************
	// 最終結果のスクリーンを環境マップとしてシェーダ側に送る 
	void SendFinalScreenEnv() 
	{
		shaderM->SetValue("EnvFullBuf", m_pSurface[(int)SURFACE_NAME_EX::SCREEN]);
	}

	//　アクセサー
	tdn2DObj* GetTex(const SURFACE_NAME_EX name); //テクスチャを取ってくる
	bool  GetShadowFlag(){ return m_bShadowFlag; }
	Surface* GetBackBuffer() { return m_pBackBuffer; }

private:

	// サーフェイス
	tdn2DObj* m_pSurface[(int)SURFACE_NAME_EX::ARRAY_END];

	// バックバッファ
	Surface* m_pBackBuffer;

	// カメラの位置
	Vector3 m_vViewPos;

	// 露光レベル
	float m_fExposure;

	// 保存用サーフェイス
	Surface* m_pSaveBackBuffer;
	D3DVIEWPORT9	m_pSaveViewPort;// ビューポート保存用

	// 極小バッファのサイズ
	int MiniSizeLv;
	int MiniSizeX;
	int MiniSizeY;

	// 放射ブラ―
	float m_blurValue;
	float m_blurPower;

	// ToonTexture
	tdn2DObj* m_pToonShadoTex;

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
	std::vector<PLSData> m_PLSdata;	// ポイントライトスフィアのデータ
	static const int PLS_MAX = 64;	// ポイントライトスフィアの限界値
	iexMesh* m_pPLS;

	//****************************
	//		ShadowMap用
	//****************************
	Surface* m_pSaveStencilSurface;		// ステンシル保存用
	Surface* m_pShadowStencilSurface;	// シャドウ用ステンシル

	int		m_iShadowSize;			// シャドウマップのサイズ
	float	m_fShadowRange;			// Shadowのカメラからの距離	
	bool	m_bShadowFlag;			// シャドウマップの使用フラグ

};


#define DeferredManagerEx (DeferredEx::GetInstance())
