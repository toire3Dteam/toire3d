#include "TDNLIB.h"
#include "Stage.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"
#include "PointLight\PointLight.h"
#include "DeferredEx\DeferredEx.h"

Stage::Base::Base() : m_pObj(nullptr), m_pBack(nullptr), m_pBack2(nullptr), m_fBottom(0), m_fWidth(200)
{
	// �X�e�[�W�̏ꏊ
	m_vStagePos = VECTOR_ZERO;

	// ��
	m_pAreWall = new AreaWallEffect();
	m_pAreWall->ActionRoop();

	// �X�e�[�W���ɃV�F�[�_�ɓn���p�����[�^�[
	// �f�t�H���g
	m_tagShaderParam.vDirLightVec = Vector3(0.84f, -0.99f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);

	m_tagShaderParam.vSkyColor = Vector3(0.6f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor= Vector3(0.45f, 0.43f, 0.43f);

	m_bBakeShadow = false;
}

Stage::Base::~Base()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBack2);
	SAFE_DELETE(m_pAreWall);
}



void Stage::Base::Update()
{
	// ��
	//m_pAreWall->GetUV()->ScaleAnimation(m_fWidth, m_fWidth);
	m_pAreWall->Update();
}

void Stage::Base::RenderCopy()
{
	if (m_pBack) m_pBack->Render(shaderM, "copy");
	m_pObj->Render(shaderM,"copy");
}
void Stage::Base::Render(tdnShader* shader, char* name)
{
	if (m_pBack) m_pBack->Render(shaderM,"sky");
	m_pObj->Render(shader, name);
	m_pAreWall->RenderAreaWall();// ��
}
void Stage::Base::RenderDeferred()
{
	m_pObj->Render(shaderM, "G_Buffer");
}
void Stage::Base::RenderShadow()
{
	m_pObj->Render(shaderM, "ShadowBuf");
}
void Stage::Base::ActionWall()
{
	m_pAreWall->ActionRoop();
}
void Stage::Base::StopWall()
{
	m_pAreWall->Stop();
}
void Stage::Base::CreateStage(Stage::Base**p,STAGE id, Camera *pCamera)
{
	switch (id)
	{
	case STAGE::SAND:
		*p = new Stage::Sand;
		break;
	case STAGE::SYUTEN:
		*p = new Stage::Syuten;
		break;
	case STAGE::SEA:
		*p = new Stage::Sea;
		break;
	case STAGE::GARDEN:
		*p = new Stage::Garden;
		break;
	case STAGE::NANASATO_SITY:
		*p = new Stage::NanasatoSity;
		break;
	case STAGE::A:
		*p = new Stage::A;
		break;
	default:
		MyAssert(0, "�X�e�[�W�ɕs���Ȓl�������Ă��܂�%d", (int)id);
		*p = new Stage::NanasatoSity;
		break;
	}

	// ��������Initialize���Ă�
	(*p)->Initialize(pCamera);
}

void Stage::Base::Collision(BasePlayer *player, Vector3 *move)
{
	// ����������
	if (player->GetPos().y < 0)
	{
		player->SetPos(Vector3(0, 20, 0));
		//player->Set_die(true);
		return;
	}

	// �X�e�[�W�Ƃ̂����蔻��
	Collision::Raypic(player, move);
}

//void Stage::Base::Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2)
//{
//	Collision::Sinking(this, pPlayer1, pPlayer2);
//}

//+------------------------------------------------------
//		�e�X�e�[�W�̐ݒ�֘A
//+------------------------------------------------------

//+------------------------------------------------------
//		����
//+------------------------------------------------------
void Stage::Sand::Initialize(Camera *pCamera)
{

	//m_pObj = new iexMesh("DATA/Stage/Senjo/pupupu2.IMO");
	m_pObj = new iexMesh("DATA/Stage/Stage/sandStage.IMO");
	//m_pObj->SetScale(2);
	m_pObj->SetPos(m_vStagePos);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetScale(3.5f);

	//m_pBack->SetPos(Vector3(0, 0, 300));
	//m_pBack->SetScale(5.5f);
	m_pBack->Update();

	m_pBack2 = new iexMesh("DATA/Stage/SkyDome/Skydome.IMO");
	//m_pBack2->SetScale(3.5f);
	m_pBack2->SetPos(Vector3(0, 0, 300));
	m_pBack2->Update();


	m_fBottom = 0;
	m_fWidth = 200;

	FOR(m_cGRASS_MAX) 
	{
		m_pGrass[i].pObj = new iex3DObj("Data/Stage/Stage/kusa/kusa_1.iem");
		m_pGrass[i].vPos = Vector3(0,0,90);
		m_pGrass[i].pObj->SetScale(1.5f);
		m_pGrass[i].pObj->SetAngle(tdnRandom::Get(0.0f, 3.14f));

	}
	FOR(m_cGRASS_MAX2)
	{
		m_pGrass2[i].pObj = new iex3DObj("Data/Stage/Stage/kusa/kusa_2.iem");
		m_pGrass2[i].vPos = Vector3(0, 0, 90);
		m_pGrass2[i].pObj->SetScale(2.25f);
		m_pGrass2[i].pObj->SetAngle(tdnRandom::Get(0.0f, 3.14f));

	}

	//���̈ʒu
	m_pGrass[0].vPos = Vector3(-20, 0, 53);
	m_pGrass[1].vPos = Vector3(-15, 0, 53);
	m_pGrass[2].vPos = Vector3(110, 0, 80);
	m_pGrass[3].vPos = Vector3(105, 0, 80);
	m_pGrass[4].vPos = Vector3(-92, 0, 55);
	m_pGrass[5].vPos = Vector3(-98, 0, 55);
	m_pGrass[6].vPos = Vector3(10, 0, 90);
	m_pGrass[7].vPos = Vector3(15, 0, 90);

	m_pGrass[8].vPos = Vector3(20, 0, 80);
	m_pGrass[9].vPos = Vector3(25, 0, 80);


	m_pGrass2[0].vPos = Vector3(25, 0, 90);
	m_pGrass2[1].vPos = Vector3(110, 0, 80);
	m_pGrass2[2].vPos = Vector3(10, 0, 90);
	m_pGrass2[3].vPos = Vector3(-98, 0, 55);


	// �������ŃX�e�[�W���Ƃ̃X�}�u���J�����̃e�L�X�g�p�X��^���A����ݒ肷��
	pCamera->SetStageCameraInfo("DATA/Stage/Stage/camera.txt");
}


Stage::Sand::~Sand()
{
	FOR(m_cGRASS_MAX)
	{
		SAFE_DELETE(m_pGrass[i].pObj);
	}
	FOR(m_cGRASS_MAX2)
	{
		SAFE_DELETE(m_pGrass2[i].pObj);
	}
}


void Stage::Sand::Update()
{
	// ��
	m_pAreWall->Update();

	// ��
	FOR(m_cGRASS_MAX)
	{
		m_pGrass[i].pObj->SetPos(m_pGrass[i].vPos);
		m_pGrass[i].pObj->Animation();
		m_pGrass[i].pObj->Update();
	}
	FOR(m_cGRASS_MAX2)
	{
		m_pGrass2[i].pObj->SetPos(m_pGrass2[i].vPos);
		m_pGrass2[i].pObj->Animation();
		m_pGrass2[i].pObj->Update();
	}

}

void Stage::Sand::Render()
{
	m_pBack2->Render(shaderM, "sky");
	m_pBack->Render(shaderM, "sky");
	
	FOR(m_cGRASS_MAX)
	{
		m_pGrass[i].pObj->Render(shaderM, "Stage");
	}
	FOR(m_cGRASS_MAX2)
	{
		m_pGrass2[i].pObj->Render(shaderM, "Stage");
	}

	m_pObj->Render(shaderM,"Stage");
	m_pAreWall->RenderAreaWall();// ��
}


//+------------------------------------------------------
//		�C
//+------------------------------------------------------
void Stage::Sea::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Sister/stage.IMO");
	m_pObj->SetPos(m_vStagePos);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, -40, 300));
	m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// �X�e�[�W���ɃV�F�[�_�ɓn���p�����[�^�[
	m_tagShaderParam.vDirLightVec = Vector3(-0.86f, -0.97f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);
	m_tagShaderParam.vSkyColor = Vector3(0.6f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor = Vector3(0.45f, 0.43f, 0.43f);


	// �������ŃX�e�[�W���Ƃ̃X�}�u���J�����̃e�L�X�g�p�X��^���A����ݒ肷��
	pCamera->SetStageCameraInfo("DATA/Stage/Sister/camera.txt");

	m_fUvSea = 0.0f;
	m_pSea = new iexMesh("Data/Stage/Sister/Water/water.IMO");
	m_pSea->SetScale(1.5f);
	m_pSea->SetPos(0, -40, -100);
	m_pSea->SetAngle(-0.02f,0,0);
	m_pSea->Update();

	m_pEnvSea = new tdn2DObj("Data/Stage/Sister/Water/EnvSky.png");
	shaderM->SetValue("EnvMap", m_pEnvSea);

	m_tagShip.pObj = new iex3DObj("DATA/Stage/Sister/Ship/ship.IEM");

}
Stage::Sea::~Sea()
{
	SAFE_DELETE(m_pSea);

	shaderM->SetTextureNULL("EnvMap");
	SAFE_DELETE(m_pEnvSea);
}

Stage::Sea::Ship::Ship() :pObj(nullptr), vPos(0, -35, 800), fAngle(PI * 0.5f), bTurn(false)
{

}

// �D�̓���
void Stage::Sea::Ship::Update()
{
	// �ړ�����
	{
		static const float l_cfSpeed(0.3f);
		vPos += Vector3(sinf(fAngle), 0, cosf(fAngle)) * l_cfSpeed;

		if (bTurn)
		{
			static const float l_cfAngleSpeed(.025f);
			fAngle += l_cfAngleSpeed;
			if (fabs(fAngle - fTurnStartAngle) > PI)
			{
				bTurn = false;
			}
		}
		else
		{
			if (fabs(vPos.x) > 200)
			{
				bTurn = true;
				fTurnStartAngle = fAngle;
			}
		}
	}

	// �I�u�W�F�N�g�̍X�V
	pObj->SetScale(3.5f);
	pObj->SetPos(vPos);
	pObj->SetAngle(fAngle);
	pObj->Animation();
	pObj->Update();
}

void Stage::Sea::Update()
{
	m_pAreWall->Update();

	m_fUvSea += 0.00025f;
	shaderM->SetValue("m_fUvSea", m_fUvSea);
	
	// �X�V
	m_tagShip.Update();

}

void Stage::Sea::Render()
{
	if (m_pBack) m_pBack->Render(shaderM, "sky");
	m_pObj->Render(shaderM, "Stage");

	m_tagShip.Render(shaderM, "Stage");// �D

	
}

void Stage::Sea::RenderDeferred()
{
	m_pObj->Render(shaderM, "G_Buffer");
	m_tagShip.Render(shaderM, "G_Buffer");// �D

}

void Stage::Sea::RenderForward()
{
	// �C
	m_pSea->Render(shaderM,"Sea");

	m_pAreWall->RenderAreaWall();// ��
}

//+------------------------------------------------------
//		�K�[�f��
//+------------------------------------------------------
void Stage::Garden::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Garden/Garden.IMO");// nanasato/stage
	//m_pObj->SetAngle(3.14f);
	m_pObj->SetPos(m_vStagePos);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, -85, 300));
	m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	m_pBack2 = new iexMesh("DATA/Stage/SkyDome/Skydome.IMO");
	//m_pBack2->SetScale(3.5f);
	m_pBack2->SetPos(Vector3(0, 0, 300));
	m_pBack2->Update();


	// �X�e�[�W���ɃV�F�[�_�ɓn���p�����[�^�[
	m_tagShaderParam.vDirLightVec = Vector3(-0.84f, -0.99f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);
	m_tagShaderParam.vSkyColor = Vector3(0.6f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor = Vector3(0.45f, 0.43f, 0.43f);

	// �������ŃX�e�[�W���Ƃ̃X�}�u���J�����̃e�L�X�g�p�X��^���A����ݒ肷��
	pCamera->SetStageCameraInfo("DATA/Stage/Sister/camera.txt");
}

void Stage::Garden::Render()
{
	m_pBack2->Render(shaderM, "sky");
	m_pBack->Render(shaderM, "sky");
	m_pObj->Render(shaderM, "Stage");
	m_pAreWall->RenderAreaWall();// ��
}

//+------------------------------------------------------
//		�����V�e�B
//+------------------------------------------------------
void Stage::NanasatoSity::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/nanasato/NanasatoSity.IMO");// nanasato/stage
	m_pObj->SetAngle(-1.57f / 24);
	m_pObj->SetPos(m_vStagePos);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Nanasato/skydome_night.IMO");
	//m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	//m_pBack->SetPos(Vector3(0, 0, 300));
	//m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// ��
	m_fUvWater = 0.0f;
	m_fWaterHeight = -24;
	m_pWater = new iexMesh("Data/Water/water2.imo");// nanasato/stage

	//// ���s�E�X�N���[��
	//if (FAILED(tdnSystem::GetDevice()->CreateDepthStencilSurface(1280, 720, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pStencilSurface, NULL)))
	//{
	//	MessageBox(tdnSystem::GetWindow(), "�[�x�o�b�t�@���쐬�ł��Ȃ�����", "ERROR", MB_OK);
	//}
	//m_pWaterEnvScreen = new tdn2DObj(1280, 720, TDN2D::USEALPHA);


	// �X�e�[�W���ɃV�F�[�_�ɓn���p�����[�^�[
	m_tagShaderParam.vDirLightVec = Vector3(-0.84f, -0.79f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.4f, 0.32f, 0.72f);
	m_tagShaderParam.vSkyColor = Vector3(0.3f, 0.1f, 0.1f);
	m_tagShaderParam.vGroundColor = Vector3(0.35f, 0.23f, 0.23f);

	// �������ŃX�e�[�W���Ƃ̃X�}�u���J�����̃e�L�X�g�p�X��^���A����ݒ肷��
	pCamera->SetStageCameraInfo("DATA/Stage/Sister/camera.txt");


	// �|�C���g���C�g�z�u
	Vector3 LightCol = Vector3(1.5f, 0.75f, 0.4f);
	float LightSize = 60;

	PointLightMgr->InitPointLightInstancing(8);
	PointLightMgr->SetPointLightInstancing(0,Vector3(11, 19, -8), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(1,Vector3(-34, 34, -8), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(2,Vector3(92, 31, 49), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(3,Vector3(-85, 31, 21), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(4,Vector3(-53, 13, 120), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(5,Vector3(1, 24, 120), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(6,Vector3(-67, 16, 205), LightCol, LightSize, 4);
	PointLightMgr->SetPointLightInstancing(7,Vector3(4, 6, 217), LightCol, LightSize, 4);
	//PointLightMgr->SetPointLightInstancing(8,Vector3(6, 30, 321), LightCol, LightSize , 4);
	//PointLightMgr->SetPointLightInstancing(9,Vector3(-66, 22, 319), LightCol, LightSize, 4 );
	PointLightMgr->UpdatePointLightInstancing();


	// �e�Ă�����
	m_bBakeShadow = true;

}

Stage::NanasatoSity::~NanasatoSity()
{
	SAFE_DELETE(m_pWater);
	//SAFE_RELEASE(m_pStencilSurface);
	//SAFE_DELETE(m_pWaterEnvScreen);
}

void Stage::NanasatoSity::Update()
{
	Base::Update();

	// �|�C���g���C�g�z�u
	//Vector3 LightCol = Vector3(1.5f, 0.75f, 0.4f);
	//float LightSize = 60;
	//PointLightMgr->AddPointLight(Vector3(11, 19, -8), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(-34, 34, -8), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(92, 31, 49), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(-85, 31, 21), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(-53, 13, 120), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(1, 24, 120), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(-67, 16, 205), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(4, 6, 217), LightCol, LightSize, 4, 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(6, 30, 321), LightCol, LightSize , 4 , 1, 1, 1);
	//PointLightMgr->AddPointLight(Vector3(-66, 22, 319), LightCol, LightSize, 4 , 1, 1, 1);


	// ��
	m_fUvWater += 0.001f;
	shaderM->SetValue("g_fUvWater", m_fUvWater);

	//m_pWater->SetAngle(Vector3(0, 0, 1));
	m_pWater->SetPos(Vector3(0, m_fWaterHeight, 200));
	m_pWater->Update();

}

void Stage::NanasatoSity::Render()
{
	if (m_pBack) m_pBack->Render(shaderM, "nightsky");
	m_pObj->Render(shaderM, "StageNanasato");
	m_pAreWall->RenderAreaWall();// ��


}

void Stage::NanasatoSity::Render(tdnShader* shader, char* name)
{
	if (m_pBack) m_pBack->Render(shaderM, "nightsky");
	m_pObj->Render(shader, name);
	m_pAreWall->RenderAreaWall();// ��
}

void Stage::NanasatoSity::RenderForward()
{
	//Surface* saveBuffer;
	//Surface* saveZ;
	//// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	//tdnSystem::GetDevice()->GetRenderTarget(0, &saveBuffer);
	//// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	//tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);
	//// Z�o�b�t�@��؂�ւ�
	//tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	//// �����_�[�^�[�Q�b�g�؂�ւ�
	//m_pWaterEnvScreen->RenderTarget();

	////�@��ʃN���A
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);


	//// �I�u�W�F�N�g���]

	//// ���̍���
	//m_fWaterHeight;
	//// �V�F�[�_
	//shaderM->SetValue("g_fWaterHeight", m_fWaterHeight);

	//// ���݂̃v���C���[�����Ɛ��̕�
	//float l_vPPHY = m_vStagePos.y - m_fWaterHeight;

	//// �܂��͐��̍����܂ň����Â藎�Ƃ�
	//Vector3 l_vNextPos = m_vStagePos;
	//l_vNextPos.y = m_fWaterHeight - (m_vStagePos.y - m_fWaterHeight);
	//Vector3 l_vPrevPos = m_vStagePos;


	//// �I�u�W�F�𐅂̌���
	//m_pObj->SetPos(l_vNextPos);
	//m_pObj->Update();
	//
	//// (01/13) Fps�����肳���邽�ߍ��̓X�e�[�W�𔽎˂����Ȃ�
	//// m_pObj->Render(shaderM, "CrystalWaterReflect");
	//m_pBack->Render(shaderM, "CrystalWaterReflect");

	//// �I�u�W�F�̈ʒu�����̏ꏊ��
	//m_pObj->SetPos(l_vPrevPos);
	//m_pObj->Update();

	////�����_�[�^�[�Q�b�g�̕���
	//tdnSystem::GetDevice()->SetRenderTarget(0, saveBuffer);

	////�X�e���V���o�b�t�@�̕���
	//tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ);

	//// �����������t���N�V�����}�b�v���V�F�[�_��
	//shaderM->SetValue("WaterReflectBuf", m_pWaterEnvScreen);


	tdnStopWatch::Start();

	m_pWater->Render(shaderM, "CrystalWater");// ���`��
	
	tdnStopWatch::End();

}

//+------------------------------------------------------
//		�I�_
//+------------------------------------------------------
void Stage::Syuten::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/nanasato/NanasatoSity.IMO");//= new iexMesh("DATA/Stage/Syuten/final_point.IMO");
	m_pObj->SetAngle(-1.57f / 24);
	m_pObj->SetPos(m_vStagePos);
	m_pObj->Update();

	//m_pBack = new iexMesh("DATA/Stage/Syuten/Skydome.IMO");
	//m_pBack->SetPos(Vector3(0, -50, 0));
	//m_pBack->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	//m_pBack2 = new iexMesh("DATA/Stage/SkyDome/Skydome.IMO");
	//m_pBack2->SetPos(Vector3(0, 0, 300));
	//m_pBack2->Update();

	// ��
	m_fUvWater = 0.0f;
	m_fWaterHeight = -24;
	m_pWater = new iexMesh("Data/Water/water2.imo");// nanasato/stage

													// �X�e�[�W���ɃV�F�[�_�ɓn���p�����[�^�[
	m_tagShaderParam.vDirLightVec = Vector3(0.84f, -0.99f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);
	m_tagShaderParam.vSkyColor = Vector3(0.64f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor = Vector3(0.48f, 0.43f, 0.43f);

	// �������ŃX�e�[�W���Ƃ̃X�}�u���J�����̃e�L�X�g�p�X��^���A����ݒ肷��
	pCamera->SetStageCameraInfo("DATA/Stage/Syuten/camera.txt");

	// �e�Ă�����
	m_bBakeShadow = true;
}

Stage::Syuten::~Syuten()
{
	SAFE_DELETE(m_pWater);
	//SAFE_RELEASE(m_pStencilSurface);
	//SAFE_DELETE(m_pWaterEnvScreen);
}

void Stage::Syuten::Update()
{
	//if (m_pBack)
	//{
	//	static float a = .0f;
	//	m_pBack->SetAngle((a += .001f));
	//	m_pBack->Update();
	//}

	m_pAreWall->Update();

	// ��
	m_fUvWater += 0.001f;
	shaderM->SetValue("g_fUvWater", m_fUvWater);

	//m_pWater->SetAngle(Vector3(0, 0, 1));
	m_pWater->SetPos(Vector3(0, m_fWaterHeight, 200));
	m_pWater->Update();
}

void Stage::Syuten::Render()
{
	m_pBack->Render(shaderM, "sky");
	//m_pBack2->Render(shaderM, "sky");
	m_pObj->Render(shaderM, "Stage");
	
	m_pAreWall->RenderAreaWall();// ��
}

void Stage::Syuten::RenderForward()
{
	//Surface* saveBuffer;
	//Surface* saveZ;
	//// ���݂̃T�[�t�F�C�X���ꎞ�ۊ�
	//tdnSystem::GetDevice()->GetRenderTarget(0, &saveBuffer);
	//// ���݂̃X�e���V���o�b�t�@���ꎞ�ۊ�
	//tdnSystem::GetDevice()->GetDepthStencilSurface(&saveZ);
	//// Z�o�b�t�@��؂�ւ�
	//tdnSystem::GetDevice()->SetDepthStencilSurface(m_pStencilSurface);
	//// �����_�[�^�[�Q�b�g�؂�ւ�
	//m_pWaterEnvScreen->RenderTarget();

	////�@��ʃN���A
	//tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);


	//// �I�u�W�F�N�g���]

	//// ���̍���
	//m_fWaterHeight;
	//// �V�F�[�_
	//shaderM->SetValue("g_fWaterHeight", m_fWaterHeight);

	//// ���݂̃v���C���[�����Ɛ��̕�
	//float l_vPPHY = m_vStagePos.y - m_fWaterHeight;

	//// �܂��͐��̍����܂ň����Â藎�Ƃ�
	//Vector3 l_vNextPos = m_vStagePos;
	//l_vNextPos.y = m_fWaterHeight - (m_vStagePos.y - m_fWaterHeight);
	//Vector3 l_vPrevPos = m_vStagePos;


	//// �I�u�W�F�𐅂̌���
	//m_pObj->SetPos(l_vNextPos);
	//m_pObj->Update();
	//
	//// (01/13) Fps�����肳���邽�ߍ��̓X�e�[�W�𔽎˂����Ȃ�
	//// m_pObj->Render(shaderM, "CrystalWaterReflect");
	//m_pBack->Render(shaderM, "CrystalWaterReflect");

	//// �I�u�W�F�̈ʒu�����̏ꏊ��
	//m_pObj->SetPos(l_vPrevPos);
	//m_pObj->Update();

	////�����_�[�^�[�Q�b�g�̕���
	//tdnSystem::GetDevice()->SetRenderTarget(0, saveBuffer);

	////�X�e���V���o�b�t�@�̕���
	//tdnSystem::GetDevice()->SetDepthStencilSurface(saveZ);

	//// �����������t���N�V�����}�b�v���V�F�[�_��
	//shaderM->SetValue("WaterReflectBuf", m_pWaterEnvScreen);


	tdnStopWatch::Start();

	m_pWater->Render(shaderM, "CrystalWater");// ���`��

	tdnStopWatch::End();

}
//void Stage::Syuten::Render()
//{
//	if (m_pBack) m_pBack->Render();
//	
//	static float v = 0;
//	v += 0.01f;
//	if (v > 1.0f)v -= 1.0f;
//	shader->SetValue("m_fTuAnime", 0.0f);
//	shader->SetValue("m_fTvAnime", v);
//	shader->SetValue("m_fAlphaUV", 1.0f);
//
//	m_pObj->Render(shader,"uvAnime");
//}

void Stage::A::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/A/toire.IMO");
	m_pObj->SetPos(Vector3(0, 0, -20));
	m_pObj->SetAngle(3.14f);
	m_pObj->SetScale(1.0f);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetAngle(3.14f);
	m_pBack->SetScale(3.5f);
	m_pBack->Update();

	m_pBack2 = new iexMesh("DATA/Stage/SkyDome/Skydome.IMO");
	//m_pBack2->SetScale(3.5f);
	m_pBack2->SetPos(Vector3(0, 0, 300));
	m_pBack2->Update();

	m_fBottom = 0;
	m_fWidth = 200;

	// �������ŃX�e�[�W���Ƃ̃X�}�u���J�����̃e�L�X�g�p�X��^���A����ݒ肷��
	pCamera->SetStageCameraInfo("DATA/Stage/A/camera.txt");
}

void Stage::A::Update()
{
	m_pAreWall->Update();
}

void Stage::A::Render()
{
	m_pBack->Render(shaderM, "sky");
	m_pBack2->Render(shaderM, "sky");
	m_pObj->Render(shaderM, "Stage");

	m_pAreWall->RenderAreaWall();// ��
}