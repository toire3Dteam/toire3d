#include "TDNLIB.h"
#include "../system/Framework.h"
#include	"SceneMain.h"
#include	"../Stage/StageMNG.h"
#include	"../Sound/SoundManager.h"
#include	"../PostEffect/PostEffect.h"
#include	"../particle_2d/particle_2d.h"
#include	"../Effect/EffectManager.h"

//******************************************************************
//		初期化・解放
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
//		処理
//******************************************************************

bool sceneMain::Update()
{
	return true;
}

//******************************************************************
//		描画
//******************************************************************

void sceneMain::Render()
{
	tdnView::Clear(0x000000);
	tdnView::Activate();

	tdnText::Draw(0, 30, 0xffffffff, "sceneMain");
}