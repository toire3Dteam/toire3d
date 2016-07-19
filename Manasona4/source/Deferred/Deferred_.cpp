#include "Deferred.h"
/*
*	Deferred
*/

iexMesh* PLS;
static const int PLS_MAX = 64;
Vector3 PLS_POS[PLS_MAX];
Vector3 PLS_COL[PLS_MAX];
float PLS_RANGE[PLS_MAX];

Deferred* Deferred::pInstance = nullptr;

/// �C���X�^���X
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
/// N RGB->�@�� 
/// S R->�X�y�L���� G->�u���[�����[�g B->�������C�g�@A���G�~�b�V�u
/// H R->AO G->�G�~�b�V�u�ǉ��\��@ B->��

/// ������
void Deferred::Init()
{

	PLS = new iexMesh("DATA/PLS/PLS_CCW.imo");
	for (int i = 0; i < PLS_MAX; i++)
	{
		PLS_POS[i].x = rand() % 400 - 200;
		PLS_POS[i].y = 10;
		PLS_POS[i].z = rand() % 400 - 200;
		
		PLS_COL[i].x = rand() % 600 ;
		PLS_COL[i].y = rand() % 600 ;
		PLS_COL[i].z = rand() % 600 ;
		
		PLS_RANGE[i] = rand() % 60;
	}
	
	// G_Buffer���쐬
	diffuse = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET); // RGB �A���׃h
	normal	= new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_USEALPHA);	 // RGB �@���@A �A���r�G���g���[�g
	specular= new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_USEALPHA);	 // R �X�y�L���� G �u���[���@B �������C�g A�@�G�~�b�V�u
	depth	= new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_FLOAT);		 // R �f�v�X

	// G-Buffer�𗘗p����T�[�t�F�C�X
	light	= new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_HDR);
	spec	= new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_HDR);
	dof		= new iex2DObj(Reduction_x, Reduction_y, IEX2D_HDR);			//A8R8G8B8
	shadow	= new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);
	
	// FinalPassSurface
	screen = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_HDR);

	// ForwardSurface
	forward = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_USEALPHA);		//	�ォ��킹�邽�ߓ����x���̗p

	// Bloom HDR�ς݂̂��̂�`�悵�Ă���̂Ł@HDR����USEALPHA��
	bloomScreen = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_USEALPHA);			//�@�Q�[����ʂ�HDR�p�ɂ�����p
	bloom = new iex2DObj(MiniTex_x, MiniTex_y, IEX2D_USEALPHA);		// HDR

	// Glow
	glowScreen = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);	//�@�Q�[����ʂ�Glow�p�ɂ�����p
	glow = new iex2DObj(MiniTex_x, MiniTex_y, IEX2D_HDR);			// Glow

	// DownSample
	downSample = new iex2DObj(downSampleSize, downSampleSize, IEX2D_FLOAT);
	calcDownSample = new iex2DObj(downSampleSize, downSampleSize, IEX2D_SYSTEMMEM);
	
	// �v�Z�pPointLight
	infoPointLight = new iex2DObj(InfoPL_X, InfoPL_Y, IEX2D_FLOAT4_SYSTEMMEM);
	PLdata.reserve(InfoPL_X);// �O�����ėe�ʂ��m�� (reserve) ���Ă����΁A�f�[�^�̒ǉ����ɔ�������Ċ��蓖�� (reallocation) ��h�����Ƃ��ł��A�����I�ł��B 

	// Gpu_PointLight
	gpuPointLight = 
		new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_HDR);
	PLSdata.reserve(PLS_MAX);// �O�����ėe�ʂ��m�� (reserve) ���Ă����΁A�f�[�^�̒ǉ����ɔ�������Ċ��蓖�� (reallocation) ��h�����Ƃ��ł��A�����I�ł��B 

	// Gpu_Particle
	//�@gpuParticle = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);

	// godRay
	godRay = new iex2DObj(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);



	// �I�����x��
	exposure = 0.0f;
	// �P�x���x��
	luminance = 0.0f;

	// BackBuffer
	iexSystem::Device->GetRenderTarget(0, &backbuffer);
	iexSystem::Device->GetDepthStencilSurface(&backbufferZ);
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	iexSystem::Device->GetViewport(&saveViewport);// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�


	//ShadowMaps
	shadowZ = nullptr;
	shadowMap = nullptr;
	shadowMapL = nullptr;
	softShadowMap = nullptr;
	shadowSize = 0;
	shadowRange = 0.0f;
	shadowFlag = false;
	cascadeFlag = false;

	ViewPos = Vector3(.0f, .0f, .0f);

	// ���}�b�v
	EnvMap = new iex2DObj("DATA/Shader/Env.png");
	shaderD->SetValue("EnvMap", EnvMap);

}

Deferred::Deferred()
{
	Init();
}

/// ���
Deferred::~Deferred()
{
	// G-Buffer
	ReleaseSurface(diffuse);
	ReleaseSurface(normal);
	ReleaseSurface(specular);
	ReleaseSurface(depth);

	// SecondPassSurface
	ReleaseSurface(light);
	ReleaseSurface(spec);
	ReleaseSurface(dof);
	ReleaseSurface(shadow);

	// FinalPassSurface
	ReleaseSurface(screen);

	// ForwardSurface
	ReleaseSurface(forward);

	// glowSurface
	ReleaseSurface(bloomScreen);
	ReleaseSurface(bloom);

	// glowSurface
	ReleaseSurface(glow);
	ReleaseSurface(glowScreen);
		
	// downSample
	ReleaseSurface(downSample);
	ReleaseSurface(calcDownSample);

	// infoPointLight
	ReleaseSurface(infoPointLight);

	// gpuPointLight
	ReleaseSurface(gpuPointLight);
	SAFE_DELETE(PLS);

	// gpuParticle
	//ReleaseSurface(gpuParticle);


	//BackBufferSurface
	backbuffer->Release();
	backbuffer = nullptr;

	backbufferZ->Release();
	backbufferZ = nullptr;

	savebackbuffer->Release();
	savebackbuffer = nullptr;


	SAFE_DELETE(EnvMap);

	if (shadowFlag)
	{
		ReleaseSurface(shadowMap);

	}
	if (cascadeFlag)
	{
		ReleaseSurface(shadowMapL);
		ReleaseSurface(softShadowMap);
	}
}

/// G_Buffer�쐬�J�n &�O��g�p�����̃T�[�t�F�C�X�̃N���A
void Deferred::Bigin()
{
	//�o�b�N�o�b�t�@�T�[�t�F�C�X�̕ۑ�
	iexSystem::Device->GetRenderTarget(0, &backbuffer);

	//���C�g�T�[�t�F�C�X��ʃN���A
	light->RenderTarget();
	spec->RenderTarget(1);
	//if (ssaoFlag == true)ssao->RenderTarget(2);


	//���C�g�T�[�t�F�C�X��ʃN���A
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	//MRT�̊��蓖��
	diffuse->RenderTarget(SURFACE_NAME::DIFFUSE);
	normal->RenderTarget(SURFACE_NAME::NORMAL);
	specular->RenderTarget(SURFACE_NAME::SPECULAR);
	depth->RenderTarget(SURFACE_NAME::DEPTH);

	//G_Buffer��ʃN���A
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);



}
/// G_Buffer�쐬�I��
void Deferred::End()
{
	//�o�b�N�o�b�t�@�T�[�t�F�C�X�̕����EMRT�̏I������
	iexSystem::Device->SetRenderTarget(NORMAL, nullptr);
	iexSystem::Device->SetRenderTarget(SPECULAR, nullptr);
	iexSystem::Device->SetRenderTarget(DEPTH, nullptr);


	//��������e�N�X�`����]��
	shaderD->SetValue("DepthBuf", depth);
	shaderD->SetValue("SpecularBuf", specular);
	shaderD->SetValue("NormalBuf", normal);

	iexSystem::Device->SetRenderTarget(0, backbuffer);


}

/// Deferred�̍X�V
void Deferred::Update(const Vector3 ViewPos)
{
	this->ViewPos = ViewPos;
	shaderD->SetValue("ViewPos", this->ViewPos);

	//�v���W�F�N�V�����t�s�񐶐�
	Matrix invproj = matProjection;
	D3DXMatrixInverse(&invproj, nullptr, &invproj);
	shaderD->SetValue("InvProj", invproj);
	
	if (shadowFlag){
		//�r���[�t�s�񐶐�
		Matrix invview = matView;
		D3DXMatrixInverse(&invview, nullptr, &invview);
		shaderD->SetValue("InvViewProj", invproj * invview);
	}

}

//**************************************************
///				Lighting
//**************************************************
/// ���s��
void Deferred::DirLight(const Vector3 dir, const Vector3 color)
{
	Matrix matV = matView;

	Vector3 LightDir;
	// ���[���h�̕��s����n��
	Vector3 wLightDir = dir;
	wLightDir.Normalize();
	shaderD->SetValue("wLightVec", wLightDir);
	
	//�r���[���W�n�ɕϊ�
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;


	LightDir.Normalize();

	//	�V�F�[�_�[�ݒ� shaderD�ɑ���
	shaderD->SetValue("ViewLightVec", LightDir);
	shaderD->SetValue("LightColor", (Vector3)color);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();
	spec->RenderTarget(1);


	//	�����_�����O
	normal->Render(shaderD, "def_DirLight");

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
	iexSystem::Device->SetRenderTarget(1, nullptr);
}

// �S���ꊇ��
void Deferred::AllLight(const Vector3 dir, const Vector3 color, const Vector3 SkyColor, const Vector3 GroundColor, float EmissivePower)
{

	Matrix matV = matView;

	Vector3 LightDir;

	//�r���[���W�n�ɕϊ�
	LightDir.x = dir.x * matV._11 + dir.y * matV._21 + dir.z * matV._31;
	LightDir.y = dir.x * matV._12 + dir.y * matV._22 + dir.z * matV._32;
	LightDir.z = dir.x * matV._13 + dir.y * matV._23 + dir.z * matV._33;


	LightDir.Normalize();

	//	�V�F�[�_�[�ݒ� 
	shaderD->SetValue("LightVec", LightDir);
	shaderD->SetValue("LightColor", (Vector3)color);
	//	�V�F�[�_�[�ݒ�
	shaderD->SetValue("SkyColor", (Vector3)SkyColor);
	shaderD->SetValue("GroundColor", (Vector3)GroundColor);
	//	�V�F�[�_�[�ݒ�
	shaderD->SetValue("EmissivePowerRate", EmissivePower);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();
	spec->RenderTarget(1);


	//	�����_�����O
	normal->Render(shaderD, "def_AllLight");

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
	iexSystem::Device->SetRenderTarget(1, nullptr);
}


//****************************
///		POINTLIGHT
//****************************

void Deferred::ClearPointLight()
{
	PL_NUM = 0;
	PLdata.clear(); 

	// �|�C���g���C�g�̏���������
}
void Deferred::SetInfoPointLight(const Vector3 pos, const Vector3 color, const float range, const float power)
{
	PointLightData data;

	// ���̂܂܂��ƃJ�����O���Ӗ����Ȃ��Ȃ�
	Matrix mWV = matView;// �r���[�s��
	// ������J�����O����
	Vector3 flont(mWV._13, mWV._23, mWV._33);
	flont.Normalize();
	Vector3 dir = pos - (ViewPos - flont * range);			//�����W�̒l�������āA�J�����O���ɂ߂�B
	dir.Normalize();
	float dot = Vector3Dot(dir, flont);
	if (dot < 0.5)
	{
		return;			// �����Ă��Ȃ��|�C���g���C�g������
	}


	// �J������ԕϊ�
	Matrix mat = matView;
	data.pos.x = pos.x * mat._11 + pos.y * mat._21 + pos.z * mat._31 + mat._41;
	data.pos.y = pos.x * mat._12 + pos.y * mat._22 + pos.z * mat._32 + mat._42;
	data.pos.z = pos.x * mat._13 + pos.y * mat._23 + pos.z * mat._33 + mat._43;

	data.range = range;
	data.color = color;
	data.power = power;

	// �z��ɉ�����
	PLdata.push_back(data);

}
//void Deferred::PointLightRender()
//{
//	//// �|�C���g���C�g�̐�
//	//shaderD->SetValue("PL_NUM", (int)PLdata.size());
//
//	//// �|�C���g���C�g�̏��𑗂�
//	//for (int i = 0; i < (int)PLdata.size(); i++)
//	//{
//	//	if (i >= PL_MAX)
//	//	{
//	//		MessageBox(iexSystem::Window, "�|�C���g���C�g�̍ő吔�𒴂��܂���", "ERROR", MB_OK);
//	//	}
//	//	char str[128];
//	//	sprintf(str, "PLpos[%d]", i);
//	//	shaderD->SetValue(str, PLdata[i].pos);
//	//	sprintf(str, "PLrange[%d]", i);
//	//	shaderD->SetValue(str, PLdata[i].range);
//	//	sprintf(str, "PLcolor[%d]", i);
//	//	shaderD->SetValue(str, PLdata[i].color);
//	//	sprintf(str, "PLpower[%d]", i);
//	//	shaderD->SetValue(str, PLdata[i].power);
//	//}
//
//
//	//// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
//	//Surface* now = nullptr;
//	//iexSystem::Device->GetRenderTarget(0, &now);
//
//	//// �����_�[�^�[�Q�b�g�̐ؑւ�
//	//light->RenderTarget();
//	//
//	//// �����_�����O
//	//normal->Render(shaderD, "def_PointLight");
//
//	//// �����_�[�^�[�Q�b�g�̕���
//	//iexSystem::Device->SetRenderTarget(0, now);
//}

//
void Deferred::PointLightInfoDraw()
{
	// �|�C���g���C�g�̐�
	shaderD->SetValue("PL_NUM", (int)PLdata.size());

	// �T�[�t�F�X�����b�N���ăR�s�[
	D3DLOCKED_RECT LockedRect;// �e�N�X�`�� ���\�[�X��̋�`�����b�N����
	// RGBA34�̃|�C���g���C�g
	infoPointLight->GetTexture()->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD);

	// float4�̌^���쐬
	struct SEND_COLOR
	{
		float r, g, b, a;
	};
	SEND_COLOR col4;

	// 0������
	FillMemory(LockedRect.pBits, LockedRect.Pitch * InfoPL_Y, 0);
	// �G��`������
	float* pData;
	for (int y = 0; y < InfoPL_Y; y++)
	{
		/*�������ލs�̐擪�A�h���X�Ɉړ��I*/
		pData = (float*)((LPBYTE)LockedRect.pBits + LockedRect.Pitch * y);// y�̒l�����ړ�

		for (int x = 0; x < InfoPL_X; x++)
		{

			// �|�C���g���C�g�̍ő吔�܂ŗ�����continue
			if (x >= (int)PLdata.size()) continue;

			// memcpy
			// �R�s�[��̃|�C���^
			// �R�s�[���̃|�C���^
			// �R�s�[���̃|�C���^�̃T�C�Y

			col4.r = col4.g = col4.b = col4.a = 0.0f;
			// �e�N�X�`����0�s�ڂ� �|�W�V�����ƃ����W��
			if (y == 0)
			{
				// �|�C���g���C�g�̃|�W�V������RGB�ɋl�߂�I
				col4.r = PLdata[x].pos.x;
				col4.g = PLdata[x].pos.y;
				col4.b = PLdata[x].pos.z;

				// Range�����ɋl�߂�
				col4.a = PLdata[x].range;

				// �e�N�X�`���֏�������
				memcpy((float*)pData + (4 * x), &col4, sizeof(SEND_COLOR));
			}
			// �e�N�X�`����1�s�ڂ� �J���[�Ƌ�����
			else if (y == 1)
			{
				// �|�C���g���C�g�̃J���[��RGB�ɋl�߂�I
				col4.r = PLdata[x].color.x;
				col4.g = PLdata[x].color.y;
				col4.b = PLdata[x].color.z;

				// �|�C���g���C�g�̋��������ɋl�߂�
				col4.a = PLdata[x].power;

				// �e�N�X�`���֏�������
				memcpy((float*)pData + (4 * x), &col4, sizeof(SEND_COLOR));
			}

		}

	}

	// �T�[�t�F�C�X�̃��b�N������
	infoPointLight->GetTexture()->UnlockRect(0);


	// �|�C���g���C�g�̏����l�߂��e�N�X�`����GPU�֑���I
	shaderD->SetValue("InfoPLMap", infoPointLight);




}
void Deferred::PointLightRender()
{


	/*********************************/
	//		�|�C���g���C�g�`��
	/*********************************/

	// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	// �����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();

	// �����_�����O
	normal->Render(shaderD, "def_PointLight");

	// �����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
}


void Deferred::PointLightRenderEx()
{
	// �|�C���g���C�g�̐�
	shaderD->SetValue("PL_NUM", (int)PLdata.size());

	// �T�[�t�F�X�����b�N���ăR�s�[
	D3DLOCKED_RECT LockedRect;// �e�N�X�`�� ���\�[�X��̋�`�����b�N����
	// RGBA34�̃|�C���g���C�g
	infoPointLight->GetTexture()->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD);

	// float4�̌^���쐬
	struct SEND_COLOR
	{
		float r, g, b, a;
	};
	SEND_COLOR col4;

	// 0������
	FillMemory(LockedRect.pBits, LockedRect.Pitch * InfoPL_Y, 0);
	// �G��`������
	float* pData;
	for (int y = 0; y < InfoPL_Y; y++)
	{
		/*�������ލs�̐擪�A�h���X�Ɉړ��I*/
		pData = (float*)((LPBYTE)LockedRect.pBits + LockedRect.Pitch * y);// y�̒l�����ړ�

		for (int x = 0; x < InfoPL_X; x++)
		{

			// �|�C���g���C�g�̍ő吔�܂ŗ�����continue
			if (x >= (int)PLdata.size()) continue;

			// memcpy
			// �R�s�[��̃|�C���^
			// �R�s�[���̃|�C���^
			// �R�s�[���̃|�C���^�̃T�C�Y

			col4.r = col4.g = col4.b = col4.a = 0.0f;
			// �e�N�X�`����0�s�ڂ� �|�W�V�����ƃ����W��
			if (y == 0)
			{
				// �|�C���g���C�g�̃|�W�V������RGB�ɋl�߂�I
				col4.r = PLdata[x].pos.x;
				col4.g = PLdata[x].pos.y;
				col4.b = PLdata[x].pos.z;

				// Range�����ɋl�߂�
				col4.a = PLdata[x].range;

				// �e�N�X�`���֏�������
				memcpy((float*)pData + (4 * x), &col4, sizeof(SEND_COLOR));
			}
			// �e�N�X�`����1�s�ڂ� �J���[�Ƌ�����
			else if (y == 1)
			{
				// �|�C���g���C�g�̃J���[��RGB�ɋl�߂�I
				col4.r = PLdata[x].color.x;
				col4.g = PLdata[x].color.y;
				col4.b = PLdata[x].color.z;

				// �|�C���g���C�g�̋��������ɋl�߂�
				col4.a = PLdata[x].power;

				// �e�N�X�`���֏�������
				memcpy((float*)pData + (4 * x), &col4, sizeof(SEND_COLOR));
			}

		}

	}

	// �T�[�t�F�C�X�̃��b�N������
	infoPointLight->GetTexture()->UnlockRect(0);


	// �|�C���g���C�g�̏����l�߂��e�N�X�`����GPU�֑���I
	shaderD->SetValue("InfoPLMap", infoPointLight);



	/*********************************/
	//		�|�C���g���C�g�`��
	/*********************************/

	// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	// �����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();

	// �����_�����O
	normal->Render(shaderD, "def_PointLight");

	// �����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
}


//****************************
///		GPU_POINTLIGHT
//****************************

void  Deferred::ClearPLSdata()
{
	PLSdata.clear();
}

void  Deferred::AddPLSdata(const Vector3 pos, const Vector3 color, const float range , const float power)
{
	PLSData data;

	// ���̂܂܂��ƃJ�����O���Ӗ����Ȃ��Ȃ�
	Matrix mWV = matView;// �r���[�s��
	// ������J�����O����
	Vector3 flont(mWV._13, mWV._23, mWV._33);
	flont.Normalize();
	Vector3 dir = pos - (ViewPos - flont * range);			//�����W�̒l�������āA�J�����O���ɂ߂�B
	dir.Normalize();
	float dot = Vector3Dot(dir, flont);
	if (dot < 0.5)
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
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	// �t�H���[�h���Z�b�g
	gpuPointLight->RenderTarget(0);
	// �t�H���[�h��ʃN���A
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);

}

void Deferred::GpuPointLightRender()
{

	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);

	// GPU�|�C���g���C�g
	gpuPointLight->RenderTarget(0);
	
	// �����ŕ`��
	for (int i = 0; i < (int)PLSdata.size(); i++)
	{
		// �J������ԕϊ������|�W�V����������
		shaderD->SetValue("PLSpos", PLSdata[i].pos);
		shaderD->SetValue("PLScolor", PLSdata[i].color);
		shaderD->SetValue("PLSrange", PLSdata[i].range);
		shaderD->SetValue("PLSpower", PLSdata[i].power);

		//PointLightBall->SetScale(30.0f);
		PLS->SetPos(PLSdata[i].wpos);
		PLS->Update();
		PLS->Render(shaderD, "PointLightSphere");

	}

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);


	// �t�@�C�i�����U���g�ɑ���
	shaderD->SetValue("PLSTex", gpuPointLight);


}

//void Deferred::PointLight(const Vector3 pos, const Vector3 color, const float range, const float power)
//{
//
//	Matrix mWV = matView;// �r���[�s��
//
//	// ������J�����O����
//	Vector3 flont(mWV._13, mWV._23, mWV._33);
//	flont.Normalize();
//	Vector3 dir = pos - (ViewPos - flont * range);			//�����W�̒l�������āA�J�����O���ɂ߂�B
//	dir.Normalize();
//
//	float dot = Vector3Dot(dir, flont);
//	if (dot < .5f) return;					//�@�킩�����������������I�I�I�|�C���g���C�g��������
//
//	// ���C�g�ʒu���r���[���W�n�ɕϊ�
//	Vector3 PointLight;
//	D3DXVECTOR3 plight = D3DXVECTOR3(pos.x, pos.y, pos.z);
//	D3DXVec3TransformCoord(&plight, &plight, &mWV);
//	PointLight = Vector3(plight.x, plight.y, plight.z);
//
//	// �K�v�p�����[�^�̓]��
//	shaderD->SetValue("LightPos", PointLight);
//	shaderD->SetValue("LightColor", (Vector3)color);
//	shaderD->SetValue("LightRange", range);
//	shaderD->SetValue("LightPower", power);
//
//	// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
//	Surface* now = nullptr;
//	iexSystem::Device->GetRenderTarget(0, &now);
//
//	// �����_�[�^�[�Q�b�g�̐ؑւ�
//	light->RenderTarget();
//	//spec->RenderTarget(1);
//
//
//	// �����_�����O
//	normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	//normal->Render(shaderD, "def_PointLight");
//	
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	//normal->Render(shaderD, "def_Emissive");
//	
//	// �����_�[�^�[�Q�b�g�̕���
//	iexSystem::Device->SetRenderTarget(0, now);
//	//iexSystem::Device->SetRenderTarget(1, nullptr);
//
//}


/// �V���v���|�C���g���C�g
//void Deferred::SimpliPointLight(const Vector3 pos, const Vector3 color, const float range)
//{
//
//	//Matrix mWV = matView;// �r���[�s��
//
//	//// ������J�����O����
//	//Vector3 flont(mWV._13, mWV._23, mWV._33);
//	//flont.Normalize();
//	//Vector3 dir = pos - (ViewPos - flont * range);			//�����W�̒l�������āA�J�����O���ɂ߂�B
//	//dir.Normalize();
//
//	//float dot = Vector3Dot(dir, flont);
//	//if (dot < .5f) return;
//
//	//// ���C�g�ʒu���r���[���W�n�ɕϊ�
//	//Vector3 PointLight;
//	//D3DXVECTOR3 plight = D3DXVECTOR3(pos.x, pos.y, pos.z);
//	//D3DXVec3TransformCoord(&plight, &plight, &mWV);
//	//PointLight = Vector3(plight.x, plight.y, plight.z);
//
//	//// �K�v�p�����[�^�̓]��
//	//shaderD->SetValue("LightPos", PointLight);
//	//shaderD->SetValue("LightColor", (Vector3)color);
//	//shaderD->SetValue("LightRange", range);
//
//	//// ���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
//	//Surface* now = nullptr;
//	//iexSystem::Device->GetRenderTarget(0, &now);
//
//	//// �����_�[�^�[�Q�b�g�̐ؑւ�
//	//light->RenderTarget();
//	//spec->RenderTarget(1);
//
//	//// �����_�����O
//	//normal->Render(shaderD, "def_SimpliPointLight");
//
//	//// �����_�[�^�[�Q�b�g�̕���
//	//iexSystem::Device->SetRenderTarget(0, now);
//	//iexSystem::Device->SetRenderTarget(1, nullptr);
//}
// �������C�e�B���O
void Deferred::HemiLight(const Vector3 SkyColor, const Vector3 GroundColor)
{
	//	�V�F�[�_�[�ݒ�
	shaderD->SetValue("SkyColor", (Vector3)SkyColor);
	shaderD->SetValue("GroundColor", (Vector3)GroundColor);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();

	//	�����_�����O
	normal->Render(shaderD, "def_HemiLight");

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
}

// �G�~�b�V�u
void Deferred::Emissive()
{
	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();

	//  specular.a���g�������̂�
	//	specular�Ń����_�����O
	specular->Render(shaderD, "def_Emissive");

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
}

void Deferred::Emissive(float EmissivePower)
{
	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();

	//	�V�F�[�_�[�ݒ�
	shaderD->SetValue("EmissivePowerRate", EmissivePower);
	
	//  specular.a���g�������̂�
	//	specular�Ń����_�����O
	specular->Render(shaderD, "def_Emissive");

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
}

void Deferred::SetEmissiveRate(float rate)
{
	//	�V�F�[�_�[�ݒ�
	shaderD->SetValue("EmissivePowerRate", rate);
}


void Deferred::Fog(const float FogNear, const float FogFar, const Vector3 FogColor)
{
	//	GPU�֑�����
	shaderD->SetValue("FogNear", FogNear);
	shaderD->SetValue("FogFar",  FogFar);
	shaderD->SetValue("FogColor", (Vector3)FogColor);

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�����_�[�^�[�Q�b�g�̐ؑւ�
	light->RenderTarget();

	//  �[�x���g�������̂�
	//	depth�Ń����_�����O
	depth->Render(shaderD, "def_Fog");

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
}

//****************************
///	FORWARD
//****************************

void Deferred::ClearForward()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	// �t�H���[�h���Z�b�g
	forward->RenderTarget(0);
	// �t�H���[�h��ʃN���A
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
}

// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����ԂɃZ�b�g���邱��
void Deferred::ForwardBigin()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	// forward�p�T�[�t�F�C�X�ɕς���
	forward->RenderTarget(0);



};
// G_Buffe�Ώۂ�Z�o�b�t�@���c���Ă����ԂɃZ�b�g���邱��
void Deferred::ForwardEnd()
{
	//diffuse�ɖ߂�
	//diffuse->RenderTarget(0);
	//��������e�N�X�`����]��
	shaderD->SetValue("ForwardTex", forward);

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
}

void Deferred::ForwardRender()
{
	// forward�ŕ`�悵���G���ォ�炩�Ԃ���
	forward->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight);

}

//****************************
///		BLOOM
//****************************
void Deferred::ClearBloom()
{
	// ���̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	// Bloom��`�悷�錳��
	bloomScreen->RenderTarget();
	iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
};
void Deferred::BeginDrawBloom()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	bloomScreen->RenderTarget();//
	
	//iexSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//Z�o�b�t�@�ɏ������܂Ȃ�
	// ������f���ł��낤�ꖇ�G�͎�����`�悷����ׂ̂̈�
	//�@Z�o�b�t�@�ɏ������܂Ȃ��悤�ɂ���

	// ��IEX��Render��Defalt�ŏ������ނ悤�ɐݒ肵�Ă�̂Ł@�`��̑ΏۂɁ@RS_COPY_NOZ�Ƃ��邵���Ȃ�
};
void Deferred::EndDrawBloom()
{
	// ����Z�o�b�t�@��OFF�ɂ��Ă����̂Ŗ߂�
	//iexSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);//Z�o�b�t�@�ɏ�������

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
};
void Deferred::BloomRender()
{
	D3DVIEWPORT9 save_viewport;//�r���[�|�[�g���ꎞ�ۑ�
	iexSystem::Device->GetViewport(&save_viewport);

	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	//�e�N�X�`���̃T�C�Y�̃r���[�|�[�g�ݒ�
	D3DVIEWPORT9 v = { 0, 0, MiniTex_x, MiniTex_y, 0, 1.0f };
	iexSystem::Device->SetViewport(&v);

	bloom->RenderTarget();
	iexSystem::Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	shaderD->SetValue("TU", (float)1.25f / (float)MiniTex_x);
	shaderD->SetValue("TV", (float)1.25f / (float)MiniTex_y);

	// ��ʂ̖��邢�������u���[���̎�Ƃ��Ē��o
	bloomScreen->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shaderD, "make_hdrBloomSeed");

	//�{�P�x�@�񐔂����Ȃ��قǃ{�P�Ȃ�
	for (int i = 0; i < 4; i++)
	{
		bloom->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0, MiniTex_x, MiniTex_y, shaderD, "gaussZ");//���s���֎~
	}

	iexSystem::Device->SetViewport(&save_viewport);
	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);

	// HDR�u���[������Z�@
	bloom->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, MiniTex_x, MiniTex_y, shaderD, "add_hdrBloom");

};

//****************************
///		GLOW
//****************************
void Deferred::ClearGlow()
{
	// ���̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	// glow��`�悷�錳��
	glowScreen->RenderTarget();
	iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
};
void Deferred::BeginDrawGlow()
{
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	glowScreen->RenderTarget();//
	
};
void Deferred::EndDrawGlow()
{
	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
};
void Deferred::GlowRender()
{
	D3DVIEWPORT9 save_viewport;//�r���[�|�[�g���ꎞ�ۑ�
	iexSystem::Device->GetViewport(&save_viewport);

	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	//�e�N�X�`���̃T�C�Y�̃r���[�|�[�g�ݒ�
	D3DVIEWPORT9 v = { 0, 0, MiniTex_x, MiniTex_y, 0, 1.0f };
	iexSystem::Device->SetViewport(&v);

	glow->RenderTarget();
	iexSystem::Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	shaderD->SetValue("TU", (float)1 / (float)MiniTex_x);
	shaderD->SetValue("TV", (float)1 / (float)MiniTex_y);

	glowScreen->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight);

	//�{�P�x�@�񐔂����Ȃ��قǃ{�P�Ȃ�
	for (int i = 0; i < 2; i++)
	{
		glow->Render(0, 0, MiniTex_x, MiniTex_y, 0, 0, MiniTex_x, MiniTex_y, shaderD, "gaussZ");//���s���֎~
	}

	iexSystem::Device->SetViewport(&save_viewport);
	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);

	//���邳�@�񐔂������قǖ��邭
	for (int i = 0; i < 2; i++)
	{
		glow->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, MiniTex_x, MiniTex_y, RS_ADD);
	}

};

//****************************
///		GOD_RAY
//****************************

void Deferred::ClearGodRay()
{
	// ���̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	// glow��`�悷�錳��
	godRay->RenderTarget();
	iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
};

void  Deferred::BeginGodRay()
{

	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	
	// godRay�p�T�[�t�F�C�X�ɕς���
	godRay->RenderTarget(0);



}

// �����Ńu���\�����ďI��
void  Deferred::EndGodRay()
{
	// ���X�g�Ɏ��g�̃����_�[�^�[�Q�b�g�ɕ��˃u���\���|����
	godRay->Render(0, 0,1280/2,720/2,0,0,1280,720, shaderD, "RadialBlur");
	//godRay->Render(0, 0, 1280/2, 720/2, 0, 0, 1280/2, 720/2);
	//godRay->Render(500, 500, 1280 / 1.5, 720 / 1.5, 0, 0, 1280, 720, shaderD, "RadialBlur");
	//godRay->Render(0, 0,  shaderD, "RadialBlur");

	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);

}



//****************************
///		DOWNSAMPLE
//****************************
//void Deferred::BeginDownSample()
//{
//	// �����_�[�^�[�Q�b�g�؂�ւ�
//	// ���̃T�[�t�F�C�X��ۑ�
//	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
//	downSample->RenderTarget();
//
//	// �r���[�|�[�g�쐬
//	iexSystem::Device->GetViewport(&saveViewport);// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
//	// �r���[�|�[�g�̍쐬
//	D3DVIEWPORT9 vp = { 0, 0, downSampleSize, downSampleSize, 0, 1.0f };
//	iexSystem::Device->SetViewport(&vp);
//
//}
//void Deferred::EndDownSample()
//{
//	// 
//
//	// �T�[�t�F�X�����b�N���ăR�s�[
//	D3DLOCKED_RECT LockedRect;// �e�N�X�`�� ���\�[�X��̋�`�����b�N����
//	calcDownSample->GetTexture()->LockRect(0, &LockedRect, NULL, D3DLOCK_READONLY);
//
//	// ��`���̃o�b�t�@���쐬 
//	float buffer[downSampleSize*downSampleSize];
//
//	// �S�摜�f�[�^���R�s�[
//	for (int y = 0; y < downSampleSize; y++)
//	{
//		CopyMemory(&buffer[downSampleSize * y], (char*)LockedRect.pBits + LockedRect.Pitch * y, downSampleSize * sizeof(float));
//	}
//
//	// �T�[�t�F�C�X�̃��b�N������
//	calcDownSample->GetTexture()->UnlockRect(0);
//
//	// pixel�̐�
//	float maxPixel = downSampleSize*downSampleSize;
//
//	// �Spixel�̋P�x�����Z
//	float sum = 0;
//	for (int i = 0; i < maxPixel; i++)
//	{
//		sum += buffer[i];
//	}
//	// �Ō�ɉ��Z�����������ĕ��ω�
//	sum /= maxPixel;
//
//
//
//	// ����
//	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
//	iexSystem::Device->SetViewport(&saveViewport);
//}


void Deferred::UpdateDownSample(float maxLumi, float minLumi)
{

	// �����_�[�^�[�Q�b�g�؂�ւ�
	// ���̃T�[�t�F�C�X��ۑ�
	iexSystem::GetDevice()->GetRenderTarget(0, &savebackbuffer);
	downSample->RenderTarget();

	// �r���[�|�[�g�쐬
	iexSystem::Device->GetViewport(&saveViewport);// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
	// �r���[�|�[�g�̍쐬
	D3DVIEWPORT9 vp = { 0, 0, downSampleSize, downSampleSize, 0, 1.0f };
	iexSystem::Device->SetViewport(&vp);

	// ���݂̃X�N���[�����_�E���T���v���p�ɕ`��
	//screen->Render(0, 0, downSampleSize, downSampleSize, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shaderD, "ToneMap_DownSample");
	screen->Render(0, 0, downSampleSize, downSampleSize, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shaderD, "DownSample");

	// �G��`���I������̂Ō��ɖ߂�
	iexSystem::GetDevice()->SetRenderTarget(0, savebackbuffer);
	iexSystem::Device->SetViewport(&saveViewport);

	// �����_�����O��e�N�X�`���̓��e���T�[�t�F�X�ɓ]��
	iexSystem::GetDevice()->GetRenderTargetData(
		downSample->GetSurface(),
		calcDownSample->GetSurface());

	// �T�[�t�F�X�����b�N���ăR�s�[
	D3DLOCKED_RECT LockedRect;// �e�N�X�`�� ���\�[�X��̋�`�����b�N����
	calcDownSample->GetTexture()->LockRect(0, &LockedRect, NULL, D3DLOCK_READONLY);

	// ��`���̃o�b�t�@���쐬 
	float buffer[downSampleSize*downSampleSize];

	// �S�摜�f�[�^���R�s�[
	for (int y = 0; y < downSampleSize; y++)
	{
		//Destination�m���́n�R�s�[��̊J�n�A�h���X�ւ̃|�C���^���w�肵�܂��B
		//Source�m���́n�R�s�[���̃������u���b�N�̊J�n�A�h���X�ւ̃|�C���^���w�肵�܂��B
		//Length�m���́n�R�s�[�������������u���b�N�̃o�C�g�����w�肵�܂��B
		CopyMemory(&buffer[downSampleSize * y], (char*)LockedRect.pBits + LockedRect.Pitch * y, downSampleSize * sizeof(float));
	}	

	// �T�[�t�F�C�X�̃��b�N������
	calcDownSample->GetTexture()->UnlockRect(0);

	// pixel�̐�
	float maxPixel = downSampleSize*downSampleSize;

	// �Spixel�̋P�x�����Z
	luminance = 0;
	//for (int i = 0; i < maxPixel; i++)
	//{
	//	luminance += buffer[i];
	//}
	//// �Ō�ɉ��Z�����������ĕ��ω�
	//luminance /= maxPixel;

	//luminance += buffer[5];
	//luminance += buffer[6];
	//luminance += buffer[9];
	//luminance += buffer[10];
	//luminance += buffer[13];
	//luminance += buffer[14];
	//luminance /= 6;

	for (int i = 0; i < maxPixel; i++)
	{
		if (luminance < buffer[i])
		{
			luminance = buffer[i];
		}
	}

	// �Ō�ɋP�x�ɂ��I�����x������
	// �E�����|�C���g�@���炩�ɏ�������悤�ɂ���Ƃ�胊�A����
	
	// �����Ŗ��邭���邩
	if (luminance <= minLumi)
	{
		if (exposure <= -7.0f)
		{
			exposure += 0.005f;
		}

	}
	// �����ňÂ�
	if (luminance >= maxLumi)
	{

	if (exposure >= -10.0f)
	{
		exposure -= 0.008f;
	}


	}

	// ���ʂ𑗂�
	shaderD->SetValue("exposure", exposure);



#ifdef _DEBUG

	// 1�ȏ�̋P�x�͂Ȃ�
	if (maxLumi >= 1.0f || minLumi >= 1.0f)
	{
		assert(false);
	}


#endif

}



//*****************************************************
///						ShadowMaps
//*****************************************************

// �V���h�E�}�b�v�̏������@
void Deferred::CreateShadowMap(const int size)
{
	//2��ȏ�A�V���h�E�}�b�v�𐶐������Ȃ��B
	if (shadowFlag) return;

	//�V���h�E�}�b�v�̃T�C�Y���m��
	shadowSize = size;

	//�V���h�E�}�b�v�̐���
	shadowMap = new iex2DObj(size, size, IEX2D_FLOAT2);			//r������z�o�b�t�@g������z�o�b�t�@�̓��̒l������

	//�V���h�E�}�b�v�p�X�e���V���o�b�t�@�T�[�t�F�C�X�̍쐬
	if (FAILED(iexSystem::Device->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &shadowZ, NULL)))
	{
		MessageBox(iexSystem::Window, "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	};

	shadowFlag = true;	// �V���h�E�}�b�v�̗p
}

void Deferred::CreateShadowMapL(const int size)
{
	//�V���h�E�}�b�vL�̃T�C�Y���m��
	shadowSizeL = size;
	//�u���[�����V���h�E�}�b�v�̐���
	softShadowMap = new iex2DObj(size, size, IEX2D_FLOAT2);
	shadowMapL = new iex2DObj(size, size, IEX2D_FLOAT2);

	cascadeFlag = true;	// ��d�V���h�E�}�b�v�̗p
}

//�@�V���h�E�̍X�V
void Deferred::CreateShadowMatrix(Vector3 dir, Vector3 target, Vector3 playerVec, const float dist)
{

#ifdef _DEBUG
	//�V���h�E�}�b�v����������Ă��Ȃ���Γ]�����Ȃ�
	if (!shadowFlag){
		MessageBox(iexSystem::Window, "�V���h�E�}�b�v������Ă��Ȃ�", __FUNCTION__, MB_OK);
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
	LookAtLH(ShadowMat, pos + (playerVec), target + (playerVec), up);
	// ���s���e�s��쐬 �r���[�{�����[��
	OlthoLH(work, width, width, 0.02f, width * 2.5f);	//���s���e�s��
	ShadowMat *= work;
	// ��������
	shaderD->SetValue("ShadowProjection", ShadowMat);

	// �A���t�@����
	shadowRange = width * .25f;

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

	if (!cascadeFlag) return;

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
	LookAtLH(ShadowMat, pos + (playerVec)*MulPower, target + (playerVec)*MulPower, up);
	OlthoLH(work, width*MulPower, width*MulPower, 0.02f, width * 2.5f);
	ShadowMat *= work;

	shaderD->SetValue("ShadowProjectionL", ShadowMat);
}

//�@�V���h�E�}�b�v�p�̕`��J�n
void Deferred::ShadowBegin()
{
#ifdef _DEBUG
	// �V���h�E�}�b�v����������Ă��Ȃ���Ώ����Ȃ�
	if (!shadowFlag){
		MessageBox(iexSystem::Window, "�V���h�E�}�b�v������Ă��Ȃ�", __FUNCTION__, MB_OK);
		exit(-1);
	}
#endif


	// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	iexSystem::Device->GetRenderTarget(0, &backbuffer);
	
	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	iexSystem::Device->GetDepthStencilSurface(&backbufferZ);
	// �V���h�E�}�b�v�p�X�e���V���o�b�t�@�ɐ؂�ւ�
	iexSystem::Device->SetDepthStencilSurface(shadowZ);

	// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
	iexSystem::Device->GetViewport(&orgViewport);
	// �r���[�|�[�g�̍쐬
	D3DVIEWPORT9 vp = { 0, 0, shadowSize, shadowSize, 0, 1.0f };
	iexSystem::Device->SetViewport(&vp);

	// �V���h�E�}�b�v�̃����_�[�X�^�[�g
	shadowMap->RenderTarget();

	//�@��ʃN���A
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

	// ���̊֐����I������e��`�悵�����Ώۂ�`��
}
//�@�V���h�E�}�b�v�p�̕`��J�n_L
void Deferred::ShadowBeginL()
{
#ifdef _DEBUG
	// �V���h�E�}�b�v����������Ă��Ȃ���Ώ����Ȃ�
	if (!cascadeFlag){
		MessageBox(iexSystem::Window, "�������V���h�E�}�b�v������Ă��Ȃ��I", __FUNCTION__, MB_OK);
		exit(-1);
	}

#endif

	// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	iexSystem::Device->GetRenderTarget(0, &backbuffer);

	// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	iexSystem::Device->GetDepthStencilSurface(&backbufferZ);
	// �V���h�E�}�b�v�p�X�e���V���o�b�t�@�ɐ؂�ւ�
	iexSystem::Device->SetDepthStencilSurface(shadowZ);

	// ���݂̃r���[�|�[�g�̈ꎞ�ۊ�
	iexSystem::Device->GetViewport(&orgViewport);
	// �r���[�|�[�g�̍쐬
	D3DVIEWPORT9 vp = { 0, 0, shadowSizeL, shadowSizeL, 0, 1.0f };
	iexSystem::Device->SetViewport(&vp);

	// �V���h�E�}�b�vL�̃����_�[�X�^�[�g
	shadowMapL->RenderTarget();

	//�@��ʃN���A
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

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
	iexSystem::Device->SetRenderTarget(0, backbuffer);

	//�X�e���V���o�b�t�@�̕���
	iexSystem::Device->SetDepthStencilSurface(backbufferZ);

	//�r���[�|�[�g�̕���
	iexSystem::Device->SetViewport(&orgViewport);
}
//�@�V���h�E�}�b�v�p�̕`��I��_L
void Deferred::ShadowEndL()
{

	// �\�t�g�V���h�E�}�b�v�ɃV���h�E�}�b�v�̌��ʂ���������
	/*_____________________________________________________________________________*/
	softShadowMap->RenderTarget();
	shaderD->SetValue("TU", 1.0f / shadowSizeL); // 2�ł����肩��
	shaderD->SetValue("TV", 1.0f / shadowSizeL);

	// �V���h�E�}�b�v�̉𑜓x�Ńu���[������ω�	
	//for (int i = 0; i < 1; i++)
	{
		shadowMapL->Render(0, 0, shadowSizeL, shadowSizeL, 0, 0, shadowSizeL, shadowSizeL, shaderD, "gaussian");//���s���֎~

	}
	
	/*_____________________________________________________________________________*/

	// �����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, backbuffer);

	// �X�e���V���o�b�t�@�̕���
	iexSystem::Device->SetDepthStencilSurface(backbufferZ);

	///�r���[�|�[�g�̕���
	iexSystem::Device->SetViewport(&orgViewport);
}

// �V���h�E�̕`��
void Deferred::RenderShadow()
{
	//�V�F�[�_�[�֓]��
	shaderD->SetValue("DepthTex", depth);			// ���s�p�̃T�[�t�F�C�X�𑗂�
	shaderD->SetValue("ShadowMap",shadowMap);
	//shaderD->SetValue("ShadowMap", softShadowMap);	// �\�t�g�V���h�E�̃T�[�t�F�C�X���V���h�E�}�b�v�Ƃ��đ���
	shaderD->SetValue("ShadowRange", shadowRange);	// �ߋ����̕�

	shadow->RenderTarget();

	//��ʃN���A
	//iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET, 0xFFFFFFFF, 1.0f, 0);// (?)�@ZBuffer�������Ȃ��悤�ɂ��܂���?

	// �e�ɂ��X�y�L��������߂邽��2�Ԗڂɐݒ�
	spec->RenderTarget(1);

	// �V���h�E�}�b�v�����p���Ă��邩�̔���
	if (!cascadeFlag)
	{
		shadow->Render(shaderD, "shadow");
	}
	else 
	{
		shaderD->SetValue("ShadowMapL", softShadowMap);// �������}�b�v�𑗂�(�\�t�g)
	//	shaderD->SetValue("ShadowMapL", shadowMapL);// �������}�b�v�𑗂�
		shadow->Render(shaderD, "DualShadow");
	}

	// �v�Z��������e����ł���߂�׃V�F�[�_�[��
	shaderD->SetValue("ShadowTex", shadow);

	iexSystem::Device->SetRenderTarget(1, nullptr);
}

void Deferred::ShadowSetting(const float ShadowStrength, const float AdjustValue, const float DistAlpha)
{
	shaderD->SetValue("Shadow", 1.0f - ShadowStrength);
	shaderD->SetValue("AdjustValue", AdjustValue);
	shaderD->SetValue("DistAlpha", 1.0f - DistAlpha);
}


//***********************************
///				Rendering
//***********************************

// �Ō�̃X�N���[�������
void Deferred::FinalResultDeferred()
{

	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//�V�F�[�_�[�֓]��
	shaderD->SetValue("LightTex", light);
	shaderD->SetValue("SpecTex", spec);

	// �ʏ�`��
	DefaultRender();

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);

}

/// �`��
void Deferred::RenderDeferred(const int outX, const int outY, const int W, const int H, const int inX, const int inY)
{
	// FinalPassSurface���ڂ�
	screen->Render(outX, outY, W, H, 0, 0, inX, inY);

}

// ��ʊE�[�x
void Deferred::RenderDOF(const Vector3 target, const float range)
{
	//���݂̃����_�[�^�[�Q�b�g���ꎞ�I�Ɋm��
	Surface* now = nullptr;
	iexSystem::Device->GetRenderTarget(0, &now);

	//********************************************
	///		�ŏI����screen�ɔ�ʊE�[�x��������
	//********************************************
	dof->RenderTarget();

	//target���r���[���W�n�ɕϊ�����
	Matrix mWV = matView;
	D3DXVECTOR3 Vt(target.x, target.y, target.z);
	D3DXVec3TransformCoord(&Vt, &Vt, &mWV);
	Vector3 Target(Vt.x, Vt.y, Vt.z);

	//��ʃN���A		���͔����Ă���
	iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	//�V�F�[�_�[�֕K�v�p�����[�^��]��
	shaderD->SetValue("DOF_target", Target);
	shaderD->SetValue("DOF_Range", range);

	// ToneMap+�߂����̃A���t�@��؂��鏈�� 
	screen->Render(0, 0, Reduction_x, Reduction_y, 0, 0, 1280, 720, shaderD, "def_DOF");	

	//********************************************
	///		���ō쐬�����C���[�W���ڂ���
	//********************************************

	//�V�F�[�_�[��1�e�N�Z���̒l��]��
	shaderD->SetValue("TU", 1.0f / (float)Reduction_x);
	shaderD->SetValue("TV", 1.0f / (float)Reduction_y);

	//�����_�[�^�[�Q�b�g�̕���
	iexSystem::Device->SetRenderTarget(0, now);
	
	//��ʃN���A
	//iexSystem::GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );

	//�k���o�b�t�@���g�p�����̂ň������΂�
	dof->Render(0, 0, 1280, 720, 0, 0, Reduction_x, Reduction_y, shaderD, "gaussian");


}

/// �f�t�H���g�@���s���{�X�y�L����
void Deferred::DefaultRender()
{
	// �V���h�E�T�[�t�F�C�X�̍X�V
	if (shadowFlag==true)
	{
		RenderShadow();
	}

	// FinalPassSurface�ɍ��܂ł�Surface�̏���S�č��킹��
	screen->RenderTarget();

	// ��ʃN���A
	// ����������S�ĕ`�悷�邽�ߖ��Ȃ������Ȃ̂ŃR�����g�A�E�g
	//iexSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	if (shadowFlag == true)
	{
		diffuse->Render(shaderD, "finalPut");		//�ʏ�`��
	}
	else
	{
		diffuse->Render(shaderD, "finalPut_notShadow");		//�e�Ȃ��`��
	}
	
	// ���Ńg�[���}�b�s���O
	screen->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shaderD, "ToneMap");

	//forward->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight,shaderD,"HDR_Lighting");


}

//******************************************
///				texture
//******************************************

iex2DObj* Deferred::GetTex(const int type)
{
	iex2DObj* ret = nullptr;

	switch (type){
	case DIFFUSE:
		ret = diffuse;
		break;
	case NORMAL:
		ret = normal;
		break;
	case SPECULAR:
		ret = specular;
		break;
	case DEPTH:
		ret = depth;
		break;
	case LIGHT:
		ret = light;
		break;
	case SPEC:
		ret = spec;
		break;
	case DOF:
		ret = dof;
		break;
	case SHADOW:
		ret = shadow;
		break;
	case SHADOWMAP:
		if (!shadowFlag)
		MessageBox(iexSystem::Window, "�e������Ă��Ȃ��I", __FUNCTION__, MB_OK);

		ret = shadowMap;
		break;
	case SHADOWMAPL:
		if (!cascadeFlag)
		MessageBox(iexSystem::Window, "��d�e�ɂ��Ă��Ȃ��I", __FUNCTION__, MB_OK);
		
		ret = shadowMapL;
		break;
	case BLOOMSCREEN:
		ret = bloomScreen;
		break;
	case BLOOM:
		ret = bloom;
		break;
	case GLOWSCREEN:
		ret = glowScreen;
		break;
	case GLOW:
		ret = glow;
		break;
	case FORWARD:
		ret = forward;
		break;
	case DOWNSAMPLE:
		ret = downSample;
		break;
	case INFOPL:
		ret = infoPointLight;
		break;
	case SCREEN:
		ret = screen;
		break;
	case GPUPL:
		ret = gpuPointLight;
		break;
	//case GPUPARTICLE:
		//ret = gpuParticle;
	case GODRAY:
		ret = godRay;
		break;
	}

	return ret;
}

// int  Deferred::Gethoge(int a)
//{
//
//	return a;
//}