#include "TDNLIB.h"
#include "Stage.h"
#include "../Player/BasePlayer.h"
#include "../Collision/Collision.h"
#include "../Camera/camera.h"

Stage::Base::Base() : m_pObj(nullptr), m_pBack(nullptr),m_DeadLineY(0)
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
	m_pObj->Render();
}

void Stage::Base::CreateStage(Stage::Base**p,STAGE_ID id, Camera *pCamera)
{
	switch (id)
	{
	case STAGE_ID::SENJO:
		*p = new Stage::Senjo;
		break;
	case STAGE_ID::SYUTEN:
		*p = new Stage::Syuten;
		break;
	}

	// ★ここでInitializeを呼ぶ
	(*p)->Initialize(pCamera);
}

void Stage::Base::Collision(BasePlayer *player)
{
	// 落下死判定
	if (player->GetPos().y < m_DeadLineY)
	{
		player->SetPos(Vector3(0, 20, 0));
		//player->Set_die(true);
		return;
	}

	// ステージとのあたり判定
	Collision::Raypic(m_pObj, player);
}

void Stage::Senjo::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Senjo/pupupu2.IMO");
	m_pBack = new iexMesh("DATA/Stage/Senjo/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, -50, 0));
	m_pBack->Update();
	m_DeadLineY = -100;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Senjo/camera.txt");
}

void Stage::Syuten::Initialize(Camera *pCamera)
{
	m_pObj = new iexMesh("DATA/Stage/Syuten/final_point.IMO");
	m_pBack = new iexMesh("DATA/Stage/Syuten/Skydome.IMO");
	m_pBack->SetPos(Vector3(0, -50, 0));
	m_pBack->Update();
	m_DeadLineY = -100;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Syuten/camera.txt");
}

void Stage::Syuten::Update()
{
	//static float v = 0;
	//v += 0.01f;
	//if (v > 1.0f)v -= 1.0f;
	//shader->SetValue("V_move", v);

	static float a = .0f;
	m_pBack->SetAngle((a += .001f));
	m_pBack->Update();
}

