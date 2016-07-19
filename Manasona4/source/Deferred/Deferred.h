#pragma once
#include "TDNLIB.h"
#include "../system/System.h"

/*
	Deferred
*/

//�@�V���h�E�}�b�v�̃T�C�Y
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
	//�������֐�
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
	// ���s��
	void DirLight(const Vector3 dir, const Vector3 color);
	// �������C�e�B���O
	void HemiLight(const Vector3 SkyColor, const Vector3 GroundColor);
	// �t�H�O
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
	// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����Ԃŕ`�悷�邱��
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
	//		�X�|�b�g���C�g
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
	void FinalResultDeferred();	// G_Buffer�����킹��
	void DefaultRender();// �f�t�H���g
	void RenderShadow();// 

	void Render();

	//�Q�b�^�[�E�Z�b�^�[
	tdn2DObj* GetTex(const int type); //�e�N�X�`��������Ă���


private:

	// G-Buffer�@MRT (�}���`�����_�[�^�[�Q�b�g)
	tdn2DObj* m_mDiffuse;
	tdn2DObj* m_mNormal;
	tdn2DObj* m_mRoughness;
	tdn2DObj* m_mDepth;

	// G-Buffer�𗘗p����SecondSurface
	tdn2DObj* m_sLight;			// ���C�g�p�T�[�t�F�C�X	
	tdn2DObj* m_sSpecular;			// �X�y�L�����p�T�[�t�F�C�X
	tdn2DObj* m_sFog;			// �t�H�O�p�T�[�t�F�C�X
	tdn2DObj* m_sShadow;		// �V���h�E�p�T�[�t�F�C�X



    // FinalPassSurface
	tdn2DObj* m_sScreen;			// ���Z���ʂ��܂Ƃ߂��T�[�t�F�C�X	
	tdn2DObj* m_sBloomScreen;
	tdn2DObj* m_sBloom;
	tdn2DObj* m_sBloomSeed;			// ���P�x�̎�

	tdn2DObj* m_sCrossGlareX;			// �N���X�O���A
	tdn2DObj* m_sCrossGlareY;			// �N���X�O���A

	// ForwardSurface
	tdn2DObj* m_sForward;			// deferred�Œʂ������ڕ`�悷��T�[�t�F�C�X

	// GpuPointLight
	tdn2DObj* m_sGpuPointLight;		// Gpu�|�C���g���C�g


	// BackBufferSurface 		
	Surface* backbuffer;	// �ŏI�I�ɉ�ʂɉf���T�[�t�F�C�X
	// �o�b�N�X�e���V���o�b�t�@�T�[�t�F�C�X
	Surface* m_backbufferZ;

	// �J�����̈ʒu
	Vector3 m_vViewPos;

	// �L���[�u�e�N�X�`��
	//CubeTex* m_cubeTex;

	// �ۑ��p�T�[�t�F�C�X
	Surface* m_savebackbuffer;
	D3DVIEWPORT9	m_saveViewport;// �r���[�|�[�g�ۑ��p


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
	std::vector<PLSData> PLSdata;	// �|�C���g���C�g�X�t�B�A�̃f�[�^
	static const int PLS_MAX = 64;	// �|�C���g���C�g�X�t�B�A�̌��E�l
	iexMesh* m_PLS;

	//****************************
	//		ShadowMap
	//****************************
	//�V���h�E�}�b�v�������ݎ��X�e���V���o�b�t�@�T�[�t�F�C�X
	Surface*		m_shadowZ;
	Surface*		m_orgZ;		// Z�o�b�t�@�ۑ��p
	D3DVIEWPORT9	m_orgViewport;// �r���[�|�[�g�ۑ��p

	tdn2DObj*	m_sShadowMap;		// ���l�e�N�X�`��
	tdn2DObj*	m_sShadowMapL;		// �������p�}�b�v
	tdn2DObj*	m_sSoftShadowMap;	// �\�t�g�V���h�E�}�b�v

	int		m_iShadowSize;			// �V���h�E�}�b�v�̃T�C�Y
	int		m_iShadowSizeL;		// �V���h�E�}�b�v�̃T�C�YL
	float	m_fShadowRange;		// Shadow�̃J��������̋���

								//�V���h�E�}�b�v�̎g�p�t���O
	bool m_bShadowFlag;
	bool m_bCascadeFlag;

	// �I�����x��
	float m_fExposure;
};

#define DeferredManager (Deferred::GetInstance())
