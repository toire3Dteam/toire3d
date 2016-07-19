#include "Deferred.h"
/*
*	Deferred
*/


// �T�[�t�F�C�X���Y��Ƀ����[�X
#define ReleaseSurface(p) { if(p != nullptr) delete p; p = nullptr;}



// �ɏ��o�b�t�@�̃T�C�Y
static const int Reduction_lvl = 4;
static const int Reduction_x = 1280 / Reduction_lvl, Reduction_y = 720 / Reduction_lvl;

static const int MiniTex_lvl = 6;
static const int MiniTex_x = 1280 / MiniTex_lvl, MiniTex_y = 720 / MiniTex_lvl;

// �_�E���T���v���p
static const int downSampleSize = 4;

// �|�C���g���C�g�p
static const int InfoPL_X = 32;
static const int InfoPL_Y = 2;



Deferred* Deferred::pInstance = nullptr;

// �C���X�^���X
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

/* �}�b�v�̓ǂݍ��ݏ�� */
// N RGB->�@�� 
// S R->�X�y�L���� G->�u���[�����[�g B->�������C�g�@A���G�~�b�V�u
// H R->AO G->�G�~�b�V�u�ǉ��\��@ B->��

// ������
void Deferred::Init()
{
	// G_Buffer���쐬
	m_mDiffuse = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA); // RGB �A���׃h
	m_mNormal = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);	 // RGB �@���@A �A���r�G���g���[�g
	m_mRoughness= new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);	 // R �X�y�L���� G �u���[���@B �������C�g A�@�G�~�b�V�u
	m_mDepth = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::FLOAT1);		 // R �f�v�X

	// G-Buffer�𗘗p����T�[�t�F�C�X
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
	m_sForward = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);		//	�ォ��킹�邽�ߓ����x���̗p	

	// BackBuffer
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_backbufferZ);
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	tdnSystem::GetDevice()->GetViewport(&m_saveViewport);// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�

	//ShadowMaps												 //ShadowMaps										 //ShadowMaps
	m_shadowZ = nullptr;
	m_sShadowMap = nullptr;
	m_sShadowMapL = nullptr;
	m_sSoftShadowMap = nullptr;
	m_iShadowSize = 0;
	m_fShadowRange = 0.0f;
	m_bShadowFlag = false;
	m_bCascadeFlag = false;

	//�@�ڐ�
	m_vViewPos = Vector3(.0f, .0f, .0f);

	// �L���[�u�e�N�X�`��
	//m_cubeTex = nullptr;

	//
	m_fExposure = 0.0f;

	// Gpu_PointLight
	PLSdata.reserve(PLS_MAX);// �O�����ėe�ʂ��m�� (reserve) ���Ă����΁A�f�[�^�̒ǉ����ɔ�������Ċ��蓖�� (reallocation) ��h�����Ƃ��ł��A�����I�ł��B 
	m_sGpuPointLight = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);
	
	m_PLS = new iexMesh("DATA/Shader/PLS_CCW.imo");

}

Deferred::Deferred()
{
	Init();
}

// ���
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

	// �L���[�u
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

// G_Buffer�쐬�J�n &�O��g�p�����̃T�[�t�F�C�X�̃N���A
void Deferred::Begin()
{
	//�o�b�N�o�b�t�@�T�[�t�F�C�X�̕ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer); // ? ���ꉽ�������K�v����H

	//���C�g�T�[�t�F�C�X��ʃN���A
	m_sLight->RenderTarget();
	m_sSpecular->RenderTarget(1);
	m_sFog->RenderTarget(2);
	//if (ssaoFlag == true)ssao->RenderTarget(2);


	//���C�g�T�[�t�F�C�X��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	//MRT�̊��蓖��
	m_mDiffuse->RenderTarget(SURFACE_NAME::MRT_DIFFUSE);
	m_mNormal->RenderTarget(SURFACE_NAME::MRT_NORMAL);
	m_mRoughness->RenderTarget(SURFACE_NAME::MRT_ROUGHNESS);
	m_mDepth->RenderTarget(SURFACE_NAME::MRT_DEPTH);

	//G_Buffer��ʃN���A
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);



}

// G_Buffer�쐬�I��
void Deferred::End()
{
	//�o�b�N�o�b�t�@�T�[�t�F�C�X�̕����EMRT�̏I������
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
	tdnSystem::GetDevice()->SetRenderTarget(2, nullptr);
	tdnSystem::GetDevice()->SetRenderTarget(3, nullptr);


	//��������e�N�X�`����]��
	shader->SetValue("DepthBuf", m_mDepth);
	shader->SetValue("SpecularBuf", m_mRoughness);
	shader->SetValue("NormalBuf",  m_mNormal);

	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);
}

// ��Update�̓����_�[�ŌĂ�ł�������
// Deferred�̍X�V
void Deferred::Update(const Vector3 ViewPos)
{
	// �g�[���}�b�s���O
	CtrlExposure();

	// �ڐ�
	m_vViewPos = ViewPos;
	shader->SetValue("ViewPos", m_vViewPos);

	//�v���W�F�N�V�����t�s�񐶐�
	Matrix invproj = matProjection;
	D3DXMatrixInverse(&invproj, nullptr, &invproj);
	shader->SetValue("InvPMatrix", invproj);

	//�v���W�F�N�V�������r���[�t�s�񐶐�
	Matrix invview = matView;
	D3DXMatrixInverse(&invview, nullptr, &invview);
	shader->SetValue("InvVPMatrix", invproj * invview);

	/*************************************/
	//	�ϊ��s����V�F�[�_���ɑ���
	/*************************************/
	// View�ϊ��s�� 
	shader->SetValue("VMatrix", matView);
	// Projection�ϊ��s�� 
	shader->SetValue("PMatrix", matProjection);
}

// �L���[�u�e�N�X�`�����Z�b�g
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
// ���s��
void Deferred::DirLight(const Vector3 dir, const Vector3 color)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ���[���h�̕��s����n��
	Vector3 wLightDir = dir;
	wLightDir.Normalize();
	//shader->SetValue("wLightVec", wLightDir);

	//�r���[���W�n�ɕϊ�
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;


	LightDir.Normalize();

	//	�V�F�[�_�[�ݒ� shader�ɑ���
	shader->SetValue("ViewLightVec", LightDir);
	shader->SetValue("LightColor", (Vector3)color);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	m_sLight->RenderTarget();
	m_sSpecular->RenderTarget(1);

	//	�����_�����O
	m_mNormal->Render(0,0,shader, "DirLight");

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// �������C�e�B���O
void Deferred::HemiLight(const Vector3 SkyColor, const Vector3 GroundColor)
{
	//	�V�F�[�_�[�ݒ�
	shader->SetValue("SkyColor", (Vector3)SkyColor);
	shader->SetValue("GroundColor", (Vector3)GroundColor);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	m_sLight->RenderTarget();

	//	�����_�����O
	m_mNormal->Render(0,0,shader, "HemiLight");

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
}


void Deferred::Fog(const float FogNear, const float FogFar, const Vector3 FogColor)
{
	//	GPU�֑�����
	shader->SetValue("FogNear", FogNear);
	shader->SetValue("FogFar", FogFar);
	shader->SetValue("FogColor", (Vector3)FogColor);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	m_sFog->RenderTarget();

	//  �[�x���g�������̂�
	//	depth�Ń����_�����O
	m_mDepth->Render(0, 0, shader, "Fog");

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
}


//*****************************************************
//					ShadowMaps
//*****************************************************

// �V���h�E�}�b�v�̏������@
void Deferred::CreateShadowMap(const int size)
{
	// 2��ȏ�A�V���h�E�}�b�v�𐶐������Ȃ��B
	if (m_bShadowFlag) return;

	// �V���h�E�}�b�v�̃T�C�Y���m��
	m_iShadowSize = size;

	// �V���h�E�}�b�v�̐���
	m_sShadowMap = new tdn2DObj(size, size, TDN2D::FLOAT2);		//r������z�o�b�t�@g������z�o�b�t�@�̓��̒l������

	//�V���h�E�}�b�v�p�X�e���V���o�b�t�@�T�[�t�F�C�X�̍쐬
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_shadowZ, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};

	m_bShadowFlag = true;	// �V���h�E�}�b�v�̗p
}

void Deferred::CreateShadowMapL(const int size)
{
	//�V���h�E�}�b�vL�̃T�C�Y���m��
	m_iShadowSizeL = size;
	//�u���[�����V���h�E�}�b�v�̐���
	m_sSoftShadowMap = new tdn2DObj(size, size, TDN2D::FLOAT2);
	m_sShadowMapL = new tdn2DObj(size, size, TDN2D::FLOAT2);

	m_bCascadeFlag = true;	// ��d�V���h�E�}�b�v�̗p
}

//�@�V���h�E�̍X�V
void Deferred::CreateShadowMatrix(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{

#ifdef _DEBUG
	//�V���h�E�}�b�v����������Ă��Ȃ���Γ]�����Ȃ�
	if (!m_bShadowFlag) 
	{
		MessageBox(tdnSystem::GetWindow(), "�V���h�E�}�b�v������Ă��Ȃ�", __FUNCTION__, MB_OK);
		exit(-1);
	}

#endif

	//	�V���h�E�쐬
	dir.Normalize();
	Vector3 pos = target - dir * dist;
	Vector3 up(.0f, 1.0f, .0f);

	//���̎w��
	float width = dist;

	//	���_�ƃ��C�g�ʒu��
	Matrix	ShadowMat, work;
	// �r���[�s��̐ݒ�
	Math::LookAtLH(ShadowMat, pos + (playerVec), target + (playerVec), up);
	// ���s���e�s��쐬 �r���[�{�����[��
	Math::OlthoLH(work, width, width, 0.02f, width * 2.5f);	//���s���e�s��
	ShadowMat *= work;
	// ��������
	shader->SetValue("ShadowProjection", ShadowMat);

	// �A���t�@����
	m_fShadowRange = width * 0.25f;

	////cascade����Ȃ炻�����̍s����쐬����
	//if (!cascadeFlag) return;
	//pos = target  - dir * dist * 3.0f;			//�ߋ����V���h�E�ɔ��3�{�قǗ���
	//up = Vector3(.0f, 1.0f, .0f);

	//D3DXMatrixIdentity(&ShadowMat);
	//D3DXMatrixIdentity(&work);
	//LookAtLH(ShadowMat, pos + (playerVec * 210), target + (playerVec * 210), up);
	//OlthoLH(work, width * 3.0f, width * 3.0f, 0.02f, width * 7.5f);
	//ShadowMat *= work;

	//shaderD->SetValue("ShadowProjectionL", ShadowMat);
}

//�@�V���h�E�̍X�V
void Deferred::CreateShadowMatrixL(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{

	if (!m_bCascadeFlag) return;

	//	�V���h�E�쐬
	dir.Normalize();
	Vector3 pos = target - dir * dist;
	Vector3 up(.0f, 1.0f, .0f);

	//���̎w��
	float width = dist;

	//	���_�ƃ��C�g�ʒu��
	Matrix	ShadowMat, work;

	//cascade����Ȃ炻�����̍s����쐬����	
	//pos = target - dir * dist;			//�ߋ����V���h�E�ɔ��3�{�قǗ���
	//up = Vector3(.0f, 1.0f, .0f);
	D3DXMatrixIdentity(&ShadowMat);
	D3DXMatrixIdentity(&work);
	float MulPower = 3.0f; // ���������͂����L���Ăn�j�H
	Math::LookAtLH(ShadowMat, pos + (playerVec)*MulPower, target + (playerVec)*MulPower, up);
	Math::OlthoLH(work, width*MulPower, width*MulPower, 0.02f, width * 2.5f);
	ShadowMat *= work;

	shader->SetValue("ShadowProjectionL", ShadowMat);
}

//�@�V���h�E�}�b�v�p�̕`��J�n
void Deferred::ShadowBegin()
{
#ifdef _DEBUG
	// �V���h�E�}�b�v����������Ă��Ȃ���Ώ����Ȃ�
	if (!m_bShadowFlag) {
		MessageBox(tdnSystem::GetWindow(), "�V���h�E�}�b�v������Ă��Ȃ�", __FUNCTION__, MB_OK);
		exit(-1);
	}
#endif


	// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);

	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&m_backbufferZ);
	// �V���h�E�}�b�v�p�X�e���V���o�b�t�@�ɐ؂�ւ�
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_shadowZ);

	// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
	tdnSystem::GetDevice()->GetViewport(&m_orgViewport);
	// �r���[�|�[�g�̍쐬
	D3DVIEWPORT9 vp = { 0, 0, m_iShadowSize, m_iShadowSize, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);

	// �V���h�E�}�b�v�̃����_�[�X�^�[�g
	m_sShadowMap->RenderTarget();

	//�@��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// ���̊֐����I������e��`�悵�����Ώۂ�`��
}
//�@�V���h�E�}�b�v�p�̕`��J�n_L
void Deferred::ShadowBeginL()
{
#ifdef _DEBUG
	// �V���h�E�}�b�v����������Ă��Ȃ���Ώ����Ȃ�
	if (!m_bCascadeFlag) {
		MessageBox(tdnSystem::GetWindow(), "�������V���h�E�}�b�v������Ă��Ȃ��I", __FUNCTION__, MB_OK);
		exit(-1);
	}

#endif

	// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);

	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&m_backbufferZ);
	// �V���h�E�}�b�v�p�X�e���V���o�b�t�@�ɐ؂�ւ�
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_shadowZ);

	// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
	tdnSystem::GetDevice()->GetViewport(&m_orgViewport);
	// �r���[�|�[�g�̍쐬
	D3DVIEWPORT9 vp = { 0, 0, m_iShadowSizeL, m_iShadowSizeL, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);

	// �V���h�E�}�b�vL�̃����_�[�X�^�[�g
	m_sShadowMapL->RenderTarget();

	//�@��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// ���̊֐����I������e��`�悵�����Ώۂ�`��i�������j
}

//�@�V���h�E�}�b�v�p�̕`��I��
void Deferred::ShadowEnd()
{

	//shaderD->SetValue("TU", 1.0f / shadowSize); // 2�ł����肩��
	//shaderD->SetValue("TV", 1.0f / shadowSize);
	//// �V���h�E�}�b�v�̉𑜓x�Ńu���[������ω�	
	//for (int i = 0; i < 2; i++)
	//{
	//	shadowMap->Render(0, 0, shadowSize, shadowSize, 0, 0, shadowSize, shadowSize, shaderD, "gaussian");//�ア�ڂ���
	//}

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);

	//�X�e���V���o�b�t�@�̕���
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_backbufferZ);

	//�r���[�|�[�g�̕���
	tdnSystem::GetDevice()->SetViewport(&m_orgViewport);
}
//�@�V���h�E�}�b�v�p�̕`��I��_L
void Deferred::ShadowEndL()
{

	// �\�t�g�V���h�E�}�b�v�ɃV���h�E�}�b�v�̌��ʂ���������
	/*_____________________________________________________________________________*/
	m_sSoftShadowMap->RenderTarget();
	shader->SetValue("TU", 1.0f / m_iShadowSizeL); // 2�ł����肩��
	shader->SetValue("TV", 1.0f / m_iShadowSizeL);

	// �V���h�E�}�b�v�̉𑜓x�Ńu���[������ω�	
	//for (int i = 0; i < 1; i++)
	{
		m_sShadowMapL->Render(0, 0, m_iShadowSizeL, m_iShadowSizeL, 0, 0, m_iShadowSizeL, m_iShadowSizeL, shader, "gaussian");//���s���֎~
	}

	/*_____________________________________________________________________________*/

	// �����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);

	// �X�e���V���o�b�t�@�̕���
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_backbufferZ);

	//�r���[�|�[�g�̕���
	tdnSystem::GetDevice()->SetViewport(&m_orgViewport);
}

// �V���h�E�̕`��
void Deferred::RenderShadow()
{
	//�V�F�[�_�[�֓]��
	shader->SetValue("DepthTex", m_mDepth);			// ���s�p�̃T�[�t�F�C�X�𑗂�
	shader->SetValue("ShadowMap", m_sShadowMap);
	//shaderD->SetValue("ShadowMap", softShadowMap);	// �\�t�g�V���h�E�̃T�[�t�F�C�X���V���h�E�}�b�v�Ƃ��đ���
	shader->SetValue("ShadowRange", m_fShadowRange);	// �ߋ����̕�

	m_sShadow->RenderTarget();

	//��ʃN���A
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);// (?)�@ZBuffer�������Ȃ��悤�ɂ��܂���?

																					// �e�ɂ��X�y�L��������߂邽��2�Ԗڂɐݒ�
	m_sSpecular->RenderTarget(1);

	// �V���h�E�}�b�v�����p���Ă��邩�̔���
	if (!m_bCascadeFlag)
	{
		m_sShadow->Render(0, 0, shader, "shadow");
	}
	else
	{
		shader->SetValue("ShadowMapL", m_sSoftShadowMap);// �������}�b�v�𑗂�(�\�t�g)
													   //	shaderD->SetValue("ShadowMapL", shadowMapL);// �������}�b�v�𑗂�
		m_sShadow->Render(0, 0, shader, "DualShadow");
	}

	// �v�Z��������e����ł���߂�׃V�F�[�_�[��
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
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	// �t�H���[�h���Z�b�g
	m_sForward->RenderTarget(0);
	// �t�H���[�h��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
}

// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����ԂɃZ�b�g���邱��
void Deferred::ForwardBegin()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	// forward�p�T�[�t�F�C�X�ɕς���
	m_sForward->RenderTarget(0);

};
// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����ԂɃZ�b�g���邱��
void Deferred::ForwardEnd()
{
	//diffuse�ɖ߂�
	//diffuse->RenderTarget(0);
	//��������e�N�X�`����]��
	shader->SetValue("ForwardTex", m_sForward);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
}

void Deferred::ForwardRender()
{
	// forward�ŕ`�悵���G���ォ�炩�Ԃ���
	m_sForward->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

}

//****************************
//		BLOOM
//****************************
//void Deferred::ClearBloom()
//{
//	// ���̃T�[�t�F�C�X��ۑ�
//	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
//	// Bloom��`�悷�錳��
//	m_sBloomScreen->RenderTarget();
//	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
//
//	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
//};
//void Deferred::BeginDrawBloom()
//{
//	// �܂��͍��̃T�[�t�F�C�X��ۑ�
//	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
//	m_sBloomScreen->RenderTarget();//
//
//	//tdnSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//Z�o�b�t�@�ɏ������܂Ȃ�
//	// ������f���ł��낤�ꖇ�G�͎�����`�悷����ׂ̂̈�
//	//�@Z�o�b�t�@�ɏ������܂Ȃ��悤�ɂ���
//
//	// ��IEX��Render��Defalt�ŏ������ނ悤�ɐݒ肵�Ă�̂Ł@�`��̑ΏۂɁ@RS_COPY_NOZ�Ƃ��邵���Ȃ�
//};
//void Deferred::EndDrawBloom()
//{
//	// ����Z�o�b�t�@��OFF�ɂ��Ă����̂Ŗ߂�
//	//tdnSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);//Z�o�b�t�@�ɏ�������
//
//	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);
//};

// ToneMap���Ɏ����ō��P�x�̎�����p�ɂ����̂Ł��̏������R�����g�A�E�g
void Deferred::BloomRender()
{
	D3DVIEWPORT9 save_viewport;//�r���[�|�[�g���ꎞ�ۑ�
	tdnSystem::GetDevice()->GetViewport(&save_viewport);

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	//�e�N�X�`���̃T�C�Y�̃r���[�|�[�g�ݒ�
	D3DVIEWPORT9 v = { 0, 0, MiniTex_x, MiniTex_y, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&v);

	m_sBloom->RenderTarget();
	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	shader->SetValue("TU", (float)1.25f / (float)MiniTex_x);
	shader->SetValue("TV", (float)1.25f / (float)MiniTex_y);

	// ��ʂ̖��邢�������u���[���̎�Ƃ��Ē��o
	//m_sBloomScreen->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0,
	//	tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, shader, "make_hdrBloomSeed");

	m_sBloomSeed->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0,
			tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

	//�{�P�x�@�񐔂����Ȃ��قǃ{�P�Ȃ�
	for (int i = 0; i < 2; i++)
	{
		m_sBloom->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0, MiniTex_x, MiniTex_y, shader, "gaussZ");//���s���֎~
	}


	tdnSystem::GetDevice()->SetViewport(&save_viewport);
	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);

	// HDR�u���[������Z�@
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

	// ���̂܂܂��ƃJ�����O���Ӗ����Ȃ��Ȃ�
	Matrix mWV = matView;// �r���[�s��
						 // ������J�����O����
	Vector3 flont(mWV._13, mWV._23, mWV._33);
	flont.Normalize();
	Vector3 dir = pos - (m_vViewPos - flont * range);			//�����W�̒l�������āA�J�����O���ɂ߂�B
	dir.Normalize();
	float dot = Vector3Dot(dir, flont);
	if (dot < 0.2f)
	{
		return;			// �����Ă��Ȃ��|�C���g���C�g������
	}

	// ���[���h���W�̃f�[�^���l�߂�
	data.wpos = pos;

	// �J������ԕϊ�
	Matrix mat = matView;
	data.pos.x = pos.x * mat._11 + pos.y * mat._21 + pos.z * mat._31 + mat._41;
	data.pos.y = pos.x * mat._12 + pos.y * mat._22 + pos.z * mat._32 + mat._42;
	data.pos.z = pos.x * mat._13 + pos.y * mat._23 + pos.z * mat._33 + mat._43;

	data.range = range;
	data.color = color;
	data.power = power;


	if ((int)PLSdata.size() >= PLS_MAX)assert(0);

	// �z��ɉ�����
	PLSdata.push_back(data);



}


void Deferred::ClearGpuPointLight()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);
	// �t�H���[�h���Z�b�g
	m_sGpuPointLight->RenderTarget(0);
	// �t�H���[�h��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_savebackbuffer);

}

void Deferred::GpuPointLightRender()
{

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_savebackbuffer);

	// GPU�|�C���g���C�g
	m_sGpuPointLight->RenderTarget(0);

	// �����ŕ`��
	for (int i = 0; i < (int)PLSdata.size(); i++)
	{
		// �J������ԕϊ������|�W�V����������
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


	// �t�@�C�i�����U���g�ɑ���
	shader->SetValue("PLSTex", m_sGpuPointLight);


}


//****************************
//		DOWNSAMPLE
//****************************
void Deferred::CtrlExposure()
{
	// �I���x
	if (KeyBoard(KB_C))
	{
		// ����
		if (KeyBoard(KB_LEFT))
		{
			m_fExposure -= 0.1f;
		}
		else if(KeyBoard(KB_RIGHT))
		{
			m_fExposure += 0.1f;
		}

	}
	// ����
	if (KeyBoard(KB_P))
	{
		m_fExposure -= 0.05f;
	}
	else if (KeyBoard(KB_O))
	{
		m_fExposure += 0.05f;
	}


	tdnText::Draw(10, 600, 0xff00ffff, "�I���x%.1f", m_fExposure);
	shader->SetValue("exposure", m_fExposure);
}

//***********************************
//				Rendering
//***********************************

// �Ō�̃X�N���[�������
void Deferred::FinalResultDeferred()
{
	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//�V�F�[�_�[�֓]��
	//shader->SetValue("LightTex", light);
	//shader->SetValue("SpecTex", spec);

	// �ʏ�`��
	DefaultRender();

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);

}


// �f�t�H���g�@���s���{�X�y�L����
void Deferred::DefaultRender()
{
	// FinalPassSurface�ɍ��܂ł�Surface�̏���S�č��킹��
	m_sScreen->RenderTarget();

	// ��ʃN���A
	// ����������S�ĕ`�悷�邽�ߖ��Ȃ������Ȃ̂ŃR�����g�A�E�g
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	// ���@�f�B�t�[�Y
	m_mDiffuse->Render(0, 0);

	// ���Ńg�[���}�b�s���O
	m_sScreen->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

}

//  Deferred
void Deferred::Render()
{
	// �Ō�̉��Z���ʂ��V�F�[�_�[���֑���
	shader->SetValue("LightTex", m_sLight);
	shader->SetValue("SpecTex", m_sSpecular);

	// �V���h�E�T�[�t�F�C�X�̍X�V
	if (m_bShadowFlag == true)
	{
		RenderShadow();
	}

	// FinalPassSurface�ɍ��܂ł�Surface�̏���S�č��킹��
	m_sScreen->RenderTarget();
	m_sBloomSeed->RenderTarget(1); // BloomSeed

	if (m_bShadowFlag == true)
	{
		m_mDiffuse->Render(0, 0, shader, "finalPut_Shadow");		//�ʏ�`��
	}
	else
	{
		m_mDiffuse->Render(0, 0, shader, "finalPut");		//�e�Ȃ��`��
	}


	// �����ŕ`��
	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);
	
	// �f�B�t�@�[�h�̃X�N���[��
	m_sScreen->Render(0, 0);

	// �t�H�A�[�h�i�G�t�F�N�g�j
	m_sForward->Render(0, 0, shader, "ToneMap");

	// �t�H�A�[�h�̇@�o�b�N�o�b�t�@�@�A�u���[���@�Ƃ����`�ɂ��܂���
	// ���P�x�̃T�[�t�F�C�X��߂�
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);

	////���C�g�T�[�t�F�C�X��ʃN���A
	//m_sLight->RenderTarget();
	//m_sSpecular->RenderTarget(1);
	////�o�b�N�o�b�t�@�T�[�t�F�C�X�̕����EMRT�̏I������
	//tdnSystem::GetDevice()->SetRenderTarget(SURFACE_NAME::MRT_NORMAL, nullptr);
	//tdnSystem::GetDevice()->SetRenderTarget(SURFACE_NAME::MRT_SPECULAR, nullptr);

	// LOD
	m_sFog->Render(0, 0, RS::ADD);// �t�H�O���d�˂�

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
		MessageBox(tdnSystem::GetWindow(), "�T�[�t�F�C�X���Ȃ�����", "Deferred::GetTex", MB_OK);
		break;
	}
	return ret;
}

