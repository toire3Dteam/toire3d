#include "TDNLIB.h"
#include "Stage.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"
#include "../Camera/camera.h"

Stage::Base::Base() : m_pObj(nullptr), m_pBack(nullptr), m_fBottom(0), m_fWidth(200)
{

}

Stage::Base::~Base()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pBack);
}



void Stage::Base::Update()
{

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
}
void Stage::Base::RenderDeferred()
{
	m_pObj->Render(shaderM, "G_Buffer");
}
void Stage::Base::RenderShadow()
{
	m_pObj->Render(shaderM, "ShadowBuf");
}
void Stage::Base::CreateStage(Stage::Base**p,STAGE id, Camera *pCamera)
{
	switch (id)
	{
	case STAGE::SENJO:
		*p = new Stage::Senjo;
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
	Collision::Raypic(this, player, move);
}

void Stage::Base::Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2)
{
	Collision::Sinking(this, pPlayer1, pPlayer2);
}


void Stage::Senjo::Initialize(Camera *pCamera)
{
	//m_pObj = new iexMesh("DATA/Stage/Senjo/pupupu2.IMO");
	m_pObj = new iexMesh("DATA/Stage/Stage/sandStage.IMO");
	//m_pObj->SetScale(2);
	//m_pObj->Update();

	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, 0, 300));
	m_pBack->SetScale(3.5f);
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 150;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Senjo/camera.txt");
}

void Stage::Syuten::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Syuten/final_point.IMO");
	m_pBack = new iexMesh("DATA/Stage/Syuten/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, -50, 0));
	m_pBack->Update();
	m_fBottom = 0;
	m_fWidth = 150;

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