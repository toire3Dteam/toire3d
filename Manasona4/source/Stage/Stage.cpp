#include "TDNLIB.h"
#include "Stage.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"


Stage::Base::Base() : m_pObj(nullptr), m_pBack(nullptr), m_fBottom(0), m_fWidth(200)
{
	// 壁
	m_pAreWall = new AreaWallEffect();
	m_pAreWall->ActionRoop();

	// ステージ毎にシェーダに渡すパラメーター
	// デフォルト
	m_tagShaderParam.vDirLightVec = Vector3(0.84f, -0.99f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);

	m_tagShaderParam.vSkyColor = Vector3(0.6f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor= Vector3(0.45f, 0.43f, 0.43f);

}

Stage::Base::~Base()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pAreWall);
}



void Stage::Base::Update()
{
	// 壁
	//m_pAreWall->GetUV()->ScaleAnimation(m_fWidth, m_fWidth);
	m_pAreWall->Update();
}

void Stage::Base::Render()
{
	if (m_pBack) m_pBack->Render();
	m_pObj->Render(shader,"copy");
}
void Stage::Base::Render(tdnShader* shader, char* name)
{
	if (m_pBack) m_pBack->Render(shaderM,"sky");
	m_pObj->Render(shader, name);
	m_pAreWall->RenderAreaWall();// 壁
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
	case STAGE::SEA:
		*p = new Stage::Sea;
		break;
	case STAGE::GARDEN:
		*p = new Stage::Garden;
		break;
	case STAGE::SYUTEN:
		*p = new Stage::Syuten;
		break;
	case STAGE::A:
		*p = new Stage::A;
		break;
	}

	// ★ここでInitializeを呼ぶ
	(*p)->Initialize(pCamera);
}

void Stage::Base::Collision(BasePlayer *player, Vector3 *move)
{
	// 落下死判定
	if (player->GetPos().y < 0)
	{
		player->SetPos(Vector3(0, 20, 0));
		//player->Set_die(true);
		return;
	}

	// ステージとのあたり判定
	Collision::Raypic(player, move);
}

//void Stage::Base::Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2)
//{
//	Collision::Sinking(this, pPlayer1, pPlayer2);
//}


void Stage::Sand::Initialize(Camera *pCamera)
{
	//m_pObj = new iexMesh("DATA/Stage/Senjo/pupupu2.IMO");
	m_pObj = new iexMesh("DATA/Stage/Stage/sandStage.IMO");
	//m_pObj->SetScale(2);
	//m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetScale(3.5f);
	//m_pBack->SetPos(Vector3(0, 0, 300));
	//m_pBack->SetScale(5.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Stage/camera.txt");
}

void Stage::Sea::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Sister/stage.IMO");

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// ステージ毎にシェーダに渡すパラメーター
	m_tagShaderParam.vDirLightVec = Vector3(0.84f, -0.99f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);
	m_tagShaderParam.vSkyColor = Vector3(0.6f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor = Vector3(0.45f, 0.43f, 0.43f);

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Sister/camera.txt");
}

void Stage::Garden::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Garden/Garden.IMO");// nanasato/stage
	//m_pObj->SetAngle(3.14f);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// ステージ毎にシェーダに渡すパラメーター
	m_tagShaderParam.vDirLightVec = Vector3(-0.84f, -0.99f, -0.53f);
	m_tagShaderParam.vDirLightColor = Vector3(0.8f, 0.72f, 0.72f);
	m_tagShaderParam.vSkyColor = Vector3(0.6f, 0.5f, 0.5f);
	m_tagShaderParam.vGroundColor = Vector3(0.45f, 0.43f, 0.43f);

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Sister/camera.txt");
}

void Stage::Syuten::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Syuten/final_point.IMO");
	m_pBack = new iexMesh("DATA/Stage/Syuten/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, -50, 0));
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Syuten/camera.txt");
}


void Stage::Syuten::Update()
{
	if (m_pBack)
	{
		static float a = .0f;
		m_pBack->SetAngle((a += .001f));
		m_pBack->Update();
	}
}

void Stage::Syuten::Render()
{
	if (m_pBack) m_pBack->Render();
	
	static float v = 0;
	v += 0.01f;
	if (v > 1.0f)v -= 1.0f;
	shader->SetValue("tuAnime", 0.0f);
	shader->SetValue("tvAnime", v);
	shader->SetValue("alphaUV", 1.0f);

	m_pObj->Render(shader,"uvAnime");
}

void Stage::A::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/A/stage1.IMO");
	m_pObj->SetAngle(3.14f);
	m_pObj->SetScale(0.9f);
	m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/A/stage2.IMO");
	m_pBack->SetAngle(3.14f);
	m_pBack->SetScale(0.9f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 200;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/A/camera.txt");
}