
#include	"sceneMain.h"
#include	"iextreme.h"
#include	"system/system.h"
#include "test/motion_blend_airou.h"
#include "text_loader/character_loader.h"
#include "collision/collision.h"

bool sceneMain::Initialize()
{
	//	環境設定
	iexLight::SetFog( 800, 1000, 0 );


	//	平行光設定							
	Vector3	 dir ( 1.0f, -1.0f, 1.0f );
	iexLight::DirLight ( 0, &dir, 1.0f, 1.0f, 1.0f );
	//	環境光設定							
	iexLight::SetAmbient ( 0x202020 );

	//	カメラ設定
	view = new iexView();
	view->Set( Vector3(0,10.0f,-20.0f), Vector3(0,0,0) );

	//	ステージ
	lpStage = new iexMesh("DATA\\BG\\STAGE\\STAGE01.X");

	airou = new TEST_airou;
	airou->Set_pos(Vector3(0, 0, 0));

	Character_loader loader;
	loader.Open("./DATA/character/airou/airou.txt");
	loader.Load(airou);
	loader.Close();

	collision = new Collision;

	return true;
}

sceneMain::~sceneMain()
{
	delete  lpStage;
	delete	view;

	delete airou;

	delete collision;
}

void sceneMain::Camera_update()
{
	view->Set(airou->Get_pos() + Vector3(0, 25, -50), airou->Get_pos());
}

void	sceneMain::Update()
{
	airou->Update();

	collision->Check(lpStage, airou);

	airou->Update_pos();

	Camera_update();
}

void	sceneMain::Render()
{
	//	画面クリア
	view->Activate();
	view->Clear();

	//	ステージ
	lpStage->Render ();

	airou->Render();
}
