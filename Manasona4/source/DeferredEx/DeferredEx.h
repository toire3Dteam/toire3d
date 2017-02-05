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

	ARRAY_END// �z��̏I���
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
	// �V���O���g��
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
	// ���s��
	void DirLight(const Vector3 dir, const Vector3 color);
	// �������C�e�B���O
	void HemiLight(const Vector3 g_vSkyColor, const Vector3 g_vGroundColor);
	// ���s���{�������C�e�B���O
	void AllLight(const Vector3 dir, const Vector3 color, const Vector3 g_vSkyColor, const Vector3 g_vGroundColor);
	
	//****************************
	//		FORWARD
	//****************************
	// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����Ԃŕ`�悷�邱��
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
	// �ŏI���ʂ̃X�N���[�������}�b�v�Ƃ��ăV�F�[�_���ɑ��� 
	void SendFinalScreenEnv() 
	{
		shaderM->SetValue("EnvFullBuf", m_pSurface[(int)SURFACE_NAME_EX::SCREEN]);
	}

	//�@�A�N�Z�T�[
	tdn2DObj* GetTex(const SURFACE_NAME_EX name); //�e�N�X�`��������Ă���
	bool  GetShadowFlag(){ return m_bShadowFlag; }
	Surface* GetBackBuffer() { return m_pBackBuffer; }

private:

	// �T�[�t�F�C�X
	tdn2DObj* m_pSurface[(int)SURFACE_NAME_EX::ARRAY_END];

	// �o�b�N�o�b�t�@
	Surface* m_pBackBuffer;

	// �J�����̈ʒu
	Vector3 m_vViewPos;

	// �I�����x��
	float m_fExposure;

	// �ۑ��p�T�[�t�F�C�X
	Surface* m_pSaveBackBuffer;
	D3DVIEWPORT9	m_pSaveViewPort;// �r���[�|�[�g�ۑ��p

	// �ɏ��o�b�t�@�̃T�C�Y
	int MiniSizeLv;
	int MiniSizeX;
	int MiniSizeY;

	// ���˃u���\
	float m_blurValue;
	float m_blurPower;

	// ToonTexture
	tdn2DObj* m_pToonShadoTex;

	/*************************/
	//	PointLight�p
	/*************************/
	typedef struct
	{
		Vector3 pos;
		float range;
		Vector3 color;
		float power;
		Vector3 wpos;
	} PLSData;
	std::vector<PLSData> m_PLSdata;	// �|�C���g���C�g�X�t�B�A�̃f�[�^
	static const int PLS_MAX = 64;	// �|�C���g���C�g�X�t�B�A�̌��E�l
	iexMesh* m_pPLS;

	//****************************
	//		ShadowMap�p
	//****************************
	Surface* m_pSaveStencilSurface;		// �X�e���V���ۑ��p
	Surface* m_pShadowStencilSurface;	// �V���h�E�p�X�e���V��

	int		m_iShadowSize;			// �V���h�E�}�b�v�̃T�C�Y
	float	m_fShadowRange;			// Shadow�̃J��������̋���	
	bool	m_bShadowFlag;			// �V���h�E�}�b�v�̎g�p�t���O

};


#define DeferredManagerEx (DeferredEx::GetInstance())
