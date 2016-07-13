#include "TDNLIB.h"
#include "Stage.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"
#include "../Camera/camera.h"

Stage::Base::Base() : m_pObj(nullptr), m_DeadLineY(0)
{

}

Stage::Base::~Base()
{
	SAFE_DELETE(m_pObj);
}



void Stage::Base::Update()
{

}

void Stage::Base::Render()
{
	m_pObj->Render();
}

void Stage::Base::CreateStage(Stage::Base**p,STAGE_ID id, Camera *pCamera)
{
	switch (id)
	{
	case STAGE_ID::SENJO:
		*p = new Stage::Senjo;
		break;
	}

	// ★ここでInitializeを呼ぶ
	(*p)->Initialize(pCamera);
}

void Stage::Base::Collision(Fighter::Base *player)
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
	m_DeadLineY = -100;

	// ★ここでステージごとのスマブラカメラのテキストパスを与え、情報を設定する
	pCamera->SetStageCameraInfo("DATA/Stage/Senjo/camera.txt");
}