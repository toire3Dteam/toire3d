#include "DeferredEx.h"

//+-------------------------------------
//	DeferredEX
//+-------------------------------------

DeferredEx* DeferredEx::m_pInstance = nullptr;

//+-------------------------------------
// �C���X�^���X
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

/* �}�b�v�̓ǂݍ��ݏ�񃁃� */
// N RGB->�@�� 
// L R->Rou G->���t�l�X B->
// H R->AO G->�G�~�b�V�u�ǉ��\��@ B->��

//+-------------------------------------
// �������E���
//+-------------------------------------
DeferredEx::DeferredEx()
{
	// �ɏ��o�b�t�@�̃T�C�Y
	MiniSizeLv = 6;
	MiniSizeX = tdnSystem::GetScreenSize().right / MiniSizeLv;
	MiniSizeY = tdnSystem::GetScreenSize().bottom / MiniSizeLv;

	// �I���x
	m_fExposure = 0.0f;

	// �ڐ�
	m_vViewPos = Vector3(.0f, .0f, .0f);

	// ���˃u���\
	m_blurValue = 2.0f;
	m_blurPower = 0.0f;

	// �T�[�t�F�C�X������
	for (int i = 0; i < (int)SURFACE_NAME_EX::ARRAY_END; i++)
	{
		switch ((SURFACE_NAME_EX)i)
		{
		case SURFACE_NAME_EX::NORMAL_DEPTH:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);	 // RG �@���@BA �[�x;

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
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);// �A���t�@���̂Ă�

			break;
		case SURFACE_NAME_EX::POINT_LIGHT:
			m_pSurface[i] = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::HDR);

			break;
		case SURFACE_NAME_EX::SHADOW_MAP:
			m_pSurface[i] = nullptr;// �����ł͍��Ȃ�

			break;
		default:
			assert(0); // ����������Ă��Ȃ��T�[�t�F�C�X������
			break;
		}

	}

	// BackBuffer�̕ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);
	// ������
	m_pSaveBackBuffer = nullptr;
	m_pSaveStencilSurface = nullptr;		// �X�e���V���ۑ��p
	m_pShadowStencilSurface = nullptr;		// �V���h�E�p�X�e���V��
	m_iShadowSize = 0;						// �V���h�E�}�b�v�̃T�C�Y
	m_fShadowRange = 0.0f;					// Shadow�̃J��������̋���	
	m_bShadowFlag = false;					// �V���h�E�}�b�v�̎g�p�t���O

	// Gpu_PointLight
	m_PLSdata.reserve(PLS_MAX);// �O�����ėe�ʂ��m�� (reserve) ���Ă����΁A�f�[�^�̒ǉ����ɔ�������Ċ��蓖�� (reallocation) ��h�����Ƃ��ł��A�����I�ł��B 
	m_pPLS = new iexMesh("DATA/Shader/PLS_CCW.imo");

	m_bPLSInstancing = false;

	// ToonTex
	m_pToonShadoTex = new tdn2DObj("Data/Shader/toonShadow.png");
	shaderM->SetValue("ToonShadowMap", m_pToonShadoTex);

}

DeferredEx::~DeferredEx()
{
	// �T�[�t�F�C�X���
	for (int i = 0; i < (int)SURFACE_NAME_EX::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pSurface[i]);
	}

	SAFE_DELETE(m_pPLS);
	SAFE_DELETE(m_pToonShadoTex);

	//Surface�̉��	
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
// G_Buffer�̍쐬
//+-------------------------------------
void DeferredEx::G_Begin()
{	
	// BackBuffer�T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);

	//MRT�̊��蓖��
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::ROUGHNESS]->RenderTarget(1);

}

void DeferredEx::G_End()
{
	// �o�b�N�o�b�t�@�T�[�t�F�C�X�̕����EMRT�̏I������
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);

	// ��������e�N�X�`����]��
	shaderM->SetValue("NormalDepthTex", m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]);
	shaderM->SetValue("RoughnessLightMapTex", m_pSurface[(int)SURFACE_NAME_EX::ROUGHNESS]);

}

// ������Update�̓����_�[�ŌĂ�ł�������
// Deferred�̍X�V
void DeferredEx::G_Update(const Vector3 g_vViewPos)
{
	// �g�[���}�b�s���O����
	CtrlExposure();

	// �ڐ�
	m_vViewPos = g_vViewPos;
	shaderM->SetValue("g_vViewPos", m_vViewPos);

	// �v���W�F�N�V�����t�s�񐶐�
	Matrix invproj = matProjection;
	D3DXMatrixInverse(&invproj, nullptr, &invproj);
	shaderM->SetValue("g_mInvPMatrix", invproj);

	// �v���W�F�N�V���������[���h�t�s�񐶐�
	Matrix invview = matView;
	D3DXMatrixInverse(&invview, nullptr, &invview);
	shaderM->SetValue("g_mInvVMatrix", invview);
	shaderM->SetValue("g_mInvVPMatrix", invproj * invview);

	/*************************************/
	//	�ϊ��s����V�F�[�_���ɑ���
	/*************************************/
	// View�ϊ��s�� 
	shaderM->SetValue("g_mVMatrix", matView);
	// Projection�ϊ��s�� 
	shaderM->SetValue("g_mPMatrix", matProjection);

	// View*Proj�ϊ��s��
	shaderM->SetValue("g_mVPMatrix", matView*matProjection);


}

//+-------------------------------------
//				Lighting
//+-------------------------------------
// ���̃T�[�t�F�C�X�N���A
void DeferredEx::ClearLightSurface()
{
	// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	// ���C�g�T�[�t�F�C�X��ʃN���A
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::SPEC]->RenderTarget(1);
	// ���C�g�T�[�t�F�C�X��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	// �����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// ���s��
void DeferredEx::DirLight(const Vector3 dir, const Vector3 color)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ���[���h�̕��s����n��
	Vector3 vWLightDir = dir;
	vWLightDir.Normalize();
	shaderM->SetValue("g_vWLightVec", vWLightDir);

	// �r���[���W�n�ɕϊ�
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;

	LightDir.Normalize();

	// �V�F�[�_�[�ݒ� shader�ɑ���
	shaderM->SetValue("g_vViewLightVec", LightDir);
	shaderM->SetValue("g_vLightColor", (Vector3)color);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget();
	m_pSurface[(int)SURFACE_NAME_EX::SPEC]->RenderTarget(1);

	//	�����_�����O
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->Render(0, 0, shaderM, "DirLightShadow");

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

// �������C�e�B���O
void DeferredEx::HemiLight(const Vector3 g_vSkyColor, const Vector3 g_vGroundColor)
{
	// �V�F�[�_�[�ݒ�
	shaderM->SetValue("g_vSkyColor", (Vector3)g_vSkyColor);
	shaderM->SetValue("g_vGroundColor", (Vector3)g_vGroundColor);

	// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	// �����_�[�^�[�Q�b�g�̐ؑւ�
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget();

	// �����_�����O
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->Render(0, 0, shaderM, "HemiLight");

	// �����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
}

void DeferredEx::AllLight(const Vector3 dir, const Vector3 color, const Vector3 g_vSkyColor, const Vector3 g_vGroundColor)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ���[���h�̕��s����n��
	Vector3 vWLightDir = dir;
	vWLightDir.Normalize();
	shaderM->SetValue("g_vWLightVec", vWLightDir);

	//�r���[���W�n�ɕϊ�
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;


	LightDir.Normalize();

	//	�V�F�[�_�[�ݒ� 
	shaderM->SetValue("g_vViewLightVec", LightDir);
	shaderM->SetValue("g_vLightColor", (Vector3)color);
	shaderM->SetValue("g_vSkyColor", (Vector3)g_vSkyColor);
	shaderM->SetValue("g_vGroundColor", (Vector3)g_vGroundColor);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	m_pSurface[(int)SURFACE_NAME_EX::LIGHT]->RenderTarget();
	m_pSurface[(int)SURFACE_NAME_EX::SPEC]->RenderTarget(1);

	//	�����_�����O
	m_pSurface[(int)SURFACE_NAME_EX::NORMAL_DEPTH]->Render(0, 0, shaderM, "AllLight");

	//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, now);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);



}


//+-------------------------------------
//	FORWARD
//+-------------------------------------

void DeferredEx::ClearForward()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// �t�H���[�h���Z�b�g
	m_pSurface[(int)SURFACE_NAME_EX::FORWARD]->RenderTarget(0);
	// �t�H���[�h��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);
}

// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����ԂɃZ�b�g���邱��
void DeferredEx::ForwardBegin()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// forward�p�T�[�t�F�C�X�ɕς���
	m_pSurface[(int)SURFACE_NAME_EX::FORWARD]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->RenderTarget(1);

};
// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����ԂɃZ�b�g���邱��
void DeferredEx::ForwardEnd()
{
	//diffuse�ɖ߂�
	//diffuse->RenderTarget(0);
	//��������e�N�X�`����]��
	shaderM->SetValue("ForwardTex", m_pSurface[(int)SURFACE_NAME_EX::FORWARD]);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);
}

void DeferredEx::ForwardRender()
{
	// forward�ŕ`�悵���G���ォ�炩�Ԃ���
	m_pSurface[(int)SURFACE_NAME_EX::FORWARD]->Render(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, 0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

}

//+-------------------------------------
//		BLOOM
//+-------------------------------------

void DeferredEx::ClearBloom()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// �Z�b�g
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->RenderTarget(0);
	// ��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);
}

// ToneMap���Ɏ����ō��P�x�̎�����p�ɂ����̂Ł��̏������R�����g�A�E�g
void DeferredEx::BloomRender()
{
	D3DVIEWPORT9 save_viewport;//�r���[�|�[�g���ꎞ�ۑ�
	tdnSystem::GetDevice()->GetViewport(&save_viewport);

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	//�e�N�X�`���̃T�C�Y�̃r���[�|�[�g�ݒ�
	D3DVIEWPORT9 v = { 0, 0, MiniSizeX, MiniSizeY, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&v);

	m_pSurface[(int)SURFACE_NAME_EX::BLOOM]->RenderTarget();
	tdnSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	shaderM->SetValue("g_fTU", (float)1.25f / (float)MiniSizeX);
	shaderM->SetValue("g_fTV", (float)1.25f / (float)MiniSizeY);

	// ��ʂ̖��邢�������u���[���̎�Ƃ��Ē��o
	//m_sBloomScreen->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0,
	//	tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, shader, "make_hdrBloomSeed");

	// �u���[���̎�ɉf���Ă��镨��`��
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->
		Render(0, 0, MiniSizeX, MiniSizeY, 0, 0,
		tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom);

	//�{�P�x�@�񐔂����Ȃ��قǃ{�P�Ȃ� �������͌y���Ȃ�
	for (int i = 0; i < 2; i++)
	{
		m_pSurface[(int)SURFACE_NAME_EX::BLOOM]->Render(0, 0, MiniSizeX, MiniSizeY,
			0, 0, MiniSizeX, MiniSizeY, shaderM, "gaussZ");//���s���֎~
	}

	tdnSystem::GetDevice()->SetViewport(&save_viewport);
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);

	// HDR�u���[������Z�@
	//for (int i = 0; i < 2; i++)// �񐔂��傫���قǃu���[���������Ȃ�
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


	if ((int)m_PLSdata.size() >= PLS_MAX)
	{

	#ifdef _DEBUG
		MyAssert(0, "����ȏド�C�g�͂����Ȃ�");
	#endif // _DEBUG
		return;
	}

	// �z��ɉ�����
	m_PLSdata.push_back(data);



}


void DeferredEx::ClearGpuPointLight()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);
	// �|�C���g���C�g�T�[�t�F�C�X���Z�b�g
	m_pSurface[(int)SURFACE_NAME_EX::POINT_LIGHT]->RenderTarget(0);
	// �|�C���g���C�g��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);

}

void DeferredEx::GpuPointLightRender()
{

	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);

	// GPU�|�C���g���C�g
	m_pSurface[(int)SURFACE_NAME_EX::POINT_LIGHT]->RenderTarget(0);

	// �Î~��p
	if (m_bPLSInstancing == true)
	{
		m_pPLSInstancing->InstancingRender();
	}

	// �����ŕ`��
	for (int i = 0; i < (int)m_PLSdata.size(); i++)
	{
		// �J������ԕϊ������|�W�V����������
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


	// �t�@�C�i�����U���g�ɑ���
	shaderM->SetValue("PLSMap", m_pSurface[(int)SURFACE_NAME_EX::POINT_LIGHT]);


}

// ������
void DeferredEx::InitPointLightInstancing(const int iIndex)
{
#ifdef _DEBUG
	MyAssert((m_bPLSInstancing == false), "���łɐ�������Ă���");
#endif // _DEBUG

	m_bPLSInstancing = true;
	m_pPLSInstancing = new PointLightInstancingMesh("Data/Shader/PLS_CCW.imo", iIndex);

}

//+-------------------------------------
//	�V���h�E
//+-------------------------------------

//�@�V���h�E�̏�����
void DeferredEx::InitShadowMap(const int size )
{
	if (m_bShadowFlag == true) return;	// 2��ȏ�A�V���h�E�}�b�v�𐶐������Ȃ��B

	// �V���h�E�}�b�v�̃T�C�Y���m��
	m_iShadowSize = size;

	// �V���h�E�}�b�v�̐���
	m_pSurface[(int)SURFACE_NAME_EX::SHADOW_MAP]= new tdn2DObj(size, size, TDN2D::FLOAT1);		//r������z�o�b�t�@g������z�o�b�t�@�̓��̒l������

	//�V���h�E�}�b�v�p�X�e���V���o�b�t�@�T�[�t�F�C�X�̍쐬!
	if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pShadowStencilSurface, NULL)))
	{
		MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};

	m_bShadowFlag = true;	// �V���h�E�}�b�v�̗p
}

// �V���h�E�̍s�񐶐�
void DeferredEx::CreateShadowMatrix
(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{
	// �V���h�E�}�b�v����������Ă��Ȃ���Γ]�����Ȃ�
	if (!m_bShadowFlag)
	{
		MessageBox(tdnSystem::GetWindow(), "�V���h�E�}�b�v������Ă��Ȃ�", __FUNCTION__, MB_OK);
		exit(-1);
	}

	// �V���h�E�쐬
	dir.Normalize();
	Vector3 pos = target - dir * dist;
	Vector3 up(.0f, 1.0f, .0f);

	// ���̎w��
	float width = dist;

	// ���_�ƃ��C�g�ʒu��
	Matrix	ShadowMat, work;
	// �r���[�s��̐ݒ�
	Math::LookAtLH(ShadowMat, pos + (playerVec), target + (playerVec), up);
	// ���s���e�s��쐬 �r���[�{�����[��
	Math::OlthoLH(work, width, width, 0.02f, width * 2.5f);	//���s���e�s��
	ShadowMat *= work;
	// ��������
	shaderM->SetValue("g_mShadowProjection", ShadowMat);

	// �A���t�@����
	m_fShadowRange = width * 0.25f;
}

//�@�V���h�E�}�b�v�p�̕`��J�n
void DeferredEx::ShadowBegin()
{

#ifdef _DEBUG
	
	// �V���h�E�}�b�v����������Ă��Ȃ���Ώ����Ȃ�
	if (m_bShadowFlag == false)
	{
		MyAssert(0, "�V���h�E�}�b�v����������Ă��Ȃ���Ԃŉe�������������Ƃ��Ă��܂�");
	}

#endif // _DEBUG


	// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pBackBuffer);
	
	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	tdnSystem::GetDevice()->GetDepthStencilSurface(&m_pSaveStencilSurface);
	
	// �V���h�E�}�b�v�p�X�e���V���o�b�t�@�ɐ؂�ւ�
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pShadowStencilSurface);

	// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
	tdnSystem::GetDevice()->GetViewport(&m_pSaveViewPort);
	
	// �r���[�|�[�g�̍쐬
	D3DVIEWPORT9 vp = { 0, 0, m_iShadowSize, m_iShadowSize, 0, 1.0f };
	tdnSystem::GetDevice()->SetViewport(&vp);

	// �V���h�E�}�b�v�̃����_�[�X�^�[�g
	m_pSurface[(int)SURFACE_NAME_EX::SHADOW_MAP]->RenderTarget();

	// ��ʃN���A
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// ���̊֐����I������e��`�悵�����Ώۂ�`��
}

//�@�V���h�E�}�b�v�p�̕`��I��
void DeferredEx::ShadowEnd()
{

	//shaderD->SetValue("g_fTU", 1.0f / shadowSize); // 2�ł����肩��
	//shaderD->SetValue("g_fTV", 1.0f / shadowSize);
	//// �V���h�E�}�b�v�̉𑜓x�Ńu���[������ω�	
	//for (int i = 0; i < 2; i++)
	//{
	//	shadowMap->Render(0, 0, shadowSize, shadowSize, 0, 0, shadowSize, shadowSize, shaderD, "gaussian");//�ア�ڂ���
	//}
	shaderM->SetValue("ShadowMap", m_pSurface[(int)SURFACE_NAME_EX::SHADOW_MAP]);

	// �����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pBackBuffer);

	// �X�e���V���o�b�t�@�̕���
	tdnSystem::GetDevice()->SetDepthStencilSurface(m_pSaveStencilSurface);

	// �r���[�|�[�g�̕���
	tdnSystem::GetDevice()->SetViewport(&m_pSaveViewPort);
}


//+-------------------------------------
//		DOWNSAMPLE
//+-------------------------------------
void DeferredEx::CtrlExposure()
{

	shaderM->SetValue("g_fExposure", m_fExposure);

#ifdef _DEBUG

	//tdnText::Draw(10, 600, 0xff00ffff, "�I���x%.1f", m_fExposure);

	// �I���x
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
	// �u���\
	m_blurPower = power;
	shaderM->SetValue("BluePower", m_blurPower);

	// �X�N���[�����W�ɕϊ�
	Vector2 screenPos = Math::WorldToProj(pos);
	
	// �ꏊ
	shaderM->SetValue("g_fCenterX", screenPos.x);
	shaderM->SetValue("g_fCenterY", screenPos.y);

}

void DeferredEx::SetRadialBlur(Vector2 pos, float power)
{
	// �u���\
	m_blurPower = power;
	shaderM->SetValue("BluePower", m_blurPower);

	// �ꏊ
	shaderM->SetValue("g_fCenterX", pos.x);
	shaderM->SetValue("g_fCenterY", pos.y);

}

void DeferredEx::RadialBlurUpdate()
{
	m_blurPower -= 0.25f;
	m_blurPower = Math::Clamp(m_blurPower, 0.0f, 30.0f);

	// �u���\
	shaderM->SetValue("g_fBlurPower", m_blurPower);
	shaderM->SetValue("g_fBlurValue", m_blurValue);

}

//+-------------------------------------
//		FINAL
//+-------------------------------------
void DeferredEx::FinalBegin()
{
	// �Ō�̉��Z���ʂ��V�F�[�_�[���֑���
	shaderM->SetValue("LightMap", m_pSurface[(int)SURFACE_NAME_EX::LIGHT]);
	shaderM->SetValue("SpecMap", m_pSurface[(int)SURFACE_NAME_EX::SPEC]);

	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &m_pSaveBackBuffer);

	// �T�[�t�F�C�X�؂�ւ�
	m_pSurface[(int)SURFACE_NAME_EX::SCREEN]->RenderTarget(0);
	m_pSurface[(int)SURFACE_NAME_EX::BLOOM_SEED]->RenderTarget(1);

}


void DeferredEx::FinalEnd()
{
	// �t�H�A�[�h�ŕ`�悵�Ă������̂��������
	//ForwardRender();

	// �t�H�A�[�h�̇@�o�b�N�o�b�t�@�@�A�u���[���@�Ƃ����`�ɂ��܂���
	// ���P�x�̃T�[�t�F�C�X��߂�
	tdnSystem::GetDevice()->SetRenderTarget(0, m_pSaveBackBuffer);//�����_�[�^�[�Q�b�g�̕���
	tdnSystem::GetDevice()->SetRenderTarget(1, nullptr);

	// �`��
	m_pSurface[(int)SURFACE_NAME_EX::SCREEN]->Render(0, 0,shaderM,"RadialBlur");

}

void DeferredEx::SendFinalScreenEnv()
{
	shaderM->SetValue("EnvFullBuf", m_pSurface[(int)SURFACE_NAME_EX::SCREEN]);
}

//+-------------------------------------
//				texture
//+-------------------------------------
// �e�N�X�`��������Ă���
tdn2DObj* DeferredEx::GetTex(const SURFACE_NAME_EX name) 
{
	// �G���[����
	switch (name)
	{
	case SURFACE_NAME_EX::ARRAY_END:
		assert(0); // ������͔z��̏I���ł�
		break;
	}

	return  m_pSurface[(int)name];
}
