#include "SceneEffectTool.h"

bool sceneEffectTool::Initialize()
{
	Effect2DMgr;

	return true;
}

sceneEffectTool::~sceneEffectTool()
{
	Effect2DMgr.Release();
}

void sceneEffectTool::Update()
{
	if (KeyBoard(KB_Q) == 1)
	{
		Effect2DMgr.AddNode();
	}
	if (KeyBoard(KB_W) == 1)
	{
		if (Effect2DMgr.GetNodeMAX() == 0)
		{
			MyAssert(0, "ƒm[ƒh‚ª‚È‚¢");
		}
		
		Effect2DMgr.GetEffectNode()->LoadTexture("DATA/Effect/DamageEffect.png");
	}
	
	if (KeyBoard(KB_E) == 1)
	{
		Effect2DMgr.m_iSelectNodeNo++;
	}

	if (KeyBoard(KB_R) == 1)
	{
		Effect2DMgr.m_iSelectNodeNo--;
	}
	
	if (KeyBoard(KB_T) == 1)
	{
		Effect2DMgr.EraseNode(Effect2DMgr.m_iSelectNodeNo);
	}

	if (KeyBoard(KB_Y) == 1)
	{
		SetEffectAngle(3.14f, END);
	}

	if (KeyBoard(KB_U) == 1)
	{
		SetMiddleFlag(true);
	}
	if (KeyBoard(KB_I) == 1)
	{
		SetEffectFrame(15, MIDDLE);
		SetEffectCol(255,0,255,255, MIDDLE);
		//SetEffectCol(255, 255, 255, 255, END);
	}
	if (KeyBoard(KB_O) == 1)
	{
		SetMiddleFlag(false);
	}

	if (KeyBoard(KB_P) == 1)
	{
		SetRenderFlag(false);
	}

	if (KeyBoard(KB_F) == 1)
	{
		float a=GetEffectAngle();
		a = a;
	}
	if (KeyBoard(KB_G) == 1)
	{
		bool a = GetMiddleFlag();
		a = a;
	}
	if (KeyBoard(KB_5) == 1)
	{
		Effect2DMgr.Save("‚³‚×.hef");
	}
	if (KeyBoard(KB_4) == 1)
	{
		Effect2DMgr.Load("‚³‚×.hef");
	}

	Effect2DMgr.Update();
}

void sceneEffectTool::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff666666);

	tdnPolygon::Rect(1000, 0, 100);

	Effect2DMgr.Render();

}
