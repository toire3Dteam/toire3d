#include "TDNLIB.h"
#include "../system/Framework.h"
#include	"SceneMain.h"
#include	"../Stage/StageMNG.h"
#include	"../Sound/SoundManager.h"
#include	"../PostEffect/PostEffect.h"
#include	"../particle_2d/particle_2d.h"
#include	"../Effect/EffectManager.h"

//******************************************************************
//		‰Šú‰»E‰ğ•ú
//******************************************************************

bool sceneMain::Initialize()
{
	tdnMouse::Initialize(FALSE);
	tdnView::Init();
	
	return true;
}

sceneMain::~sceneMain()
{

}

//******************************************************************
//		ˆ—
//******************************************************************

bool sceneMain::Update()
{
	return true;
}

//******************************************************************
//		•`‰æ
//******************************************************************

void sceneMain::Render()
{
	tdnView::Clear(0x000000);
	tdnView::Activate();

	tdnText::Draw(0, 30, 0xffffffff, "sceneMain");
}