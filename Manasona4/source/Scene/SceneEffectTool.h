#pragma once
#include "TDNLIB.h"
#include "../system/FrameworkEx.h"
#include "EffectTool\Effect2DManager.h"

class sceneEffectTool : public BaseScene
{
public:

	//初期化・解放
	bool Initialize();
	~sceneEffectTool();
	
	//処理
	void Update();
	//描画
	void Render();

public:	// C#の命令用

	
	void AddNode() 
	{
		Effect2DMgr.AddNode();
	}
	void EraseNode()
	{
		Effect2DMgr.EraseNode();
	}
	void SelectNode(int no)
	{
		Effect2DMgr.m_iSelectNodeNo = no;
		Effect2DMgr.CalcSelectNo();
	}

	int GetSelectNode()
	{
		return  Effect2DMgr.m_iSelectNodeNo;
	}

	void LoadTexture(char* fileName)
	{
		Effect2DMgr.GetEffectNode()->LoadTexture(fileName);
	}

	void LoadEffectData(char* fileName)
	{
		Effect2DMgr.Load(fileName);
	}

	void SaveEffectData(char* fileName) 
	{
		Effect2DMgr.Save(fileName);
	}

	void OutPutEffect(char* fileName,int interval)
	{
		Effect2DMgr.OutputPng(fileName, interval);
	}

	void OutPutLineEffect(char* fileName, int interval, int WidthFrameNum)
	{
		Effect2DMgr.OutputPngLine(fileName, interval, WidthFrameNum);
	}

	void ChangeScreenSize256() 
	{
		Effect2DMgr.ChangeScreenSize256();
	}

	void ChangeScreenSize512()
	{
		Effect2DMgr.ChangeScreenSize512();
	}

	enum TYPE 
	{
		START = 0, MIDDLE = 1, END = 2
	};

	void SetEffectFrame(int frame, int type = TYPE::START) 
	{
		switch (type)
		{
		case START:
			 Effect2DMgr.GetEffectNode()->m_sStart.frame= frame;
			break;
		case MIDDLE:
			 Effect2DMgr.GetEffectNode()->m_sMiddle.frame = frame;
			break;
		case END:
			 Effect2DMgr.GetEffectNode()->m_sEnd.frame = frame;
			break;
		default:
			MyAssert("配列外", "err");
			break;
		}
	}

	void SetEffectPos(float x, float y, int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			Effect2DMgr.GetEffectNode()->m_sStart.pos.x = x;
			Effect2DMgr.GetEffectNode()->m_sStart.pos.y = y;
			break;
		case MIDDLE:
			Effect2DMgr.GetEffectNode()->m_sMiddle.pos.x = x;
			Effect2DMgr.GetEffectNode()->m_sMiddle.pos.y = y;
			break;
		case END:
			Effect2DMgr.GetEffectNode()->m_sEnd.pos.x = x;
			Effect2DMgr.GetEffectNode()->m_sEnd.pos.y = y;
			break;
		default:
			MyAssert("配列外", "err");
			break;
		}
	}

	void SetEffectCol(int a, int r, int g, int b, int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			Effect2DMgr.GetEffectNode()->m_sStart.col.a = a;
			Effect2DMgr.GetEffectNode()->m_sStart.col.r = r;
			Effect2DMgr.GetEffectNode()->m_sStart.col.g = g;
			Effect2DMgr.GetEffectNode()->m_sStart.col.b = b;
			break;
		case MIDDLE:
			 Effect2DMgr.GetEffectNode()->m_sMiddle.col.a = a;
			 Effect2DMgr.GetEffectNode()->m_sMiddle.col.r = r;
			 Effect2DMgr.GetEffectNode()->m_sMiddle.col.g = g;
			 Effect2DMgr.GetEffectNode()->m_sMiddle.col.b = b;
			break;
		case END:
			 Effect2DMgr.GetEffectNode()->m_sEnd.col.a = a;
			 Effect2DMgr.GetEffectNode()->m_sEnd.col.r = r;
			 Effect2DMgr.GetEffectNode()->m_sEnd.col.g = g;
			 Effect2DMgr.GetEffectNode()->m_sEnd.col.b = b;
			break;
		default:
			MyAssert("配列外", "err");
			break;
		}

	}
	void SetEffectScale(float scale, int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			Effect2DMgr.GetEffectNode()->m_sStart.scale = scale;
			break;
		case MIDDLE:
			 Effect2DMgr.GetEffectNode()->m_sMiddle.scale = scale;
			break;
		case END:
			 Effect2DMgr.GetEffectNode()->m_sEnd.scale = scale;
			break;
		default:
			MyAssert("配列外", "err");
			break;
		}
	}
	void SetEffectAngle(float angle, int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			 Effect2DMgr.GetEffectNode()->m_sStart.angle= angle;
			break;
		case MIDDLE:
			 Effect2DMgr.GetEffectNode()->m_sMiddle.angle = angle;
			break;
		case END:
			 Effect2DMgr.GetEffectNode()->m_sEnd.angle = angle;
			break;
		default:
			MyAssert("配列外", "err");
			break;
		}
	}

	int   GetEffectFrame(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.frame;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.frame;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.frame;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
		
	}

	float   GetEffectPosX(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.pos.x;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.pos.x;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.pos.x;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}
	float   GetEffectPosY(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.pos.y;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.pos.y;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.pos.y;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}

	int   GetEffectColA(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.col.a;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.col.a;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.col.a;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}
	int   GetEffectColR(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.col.r;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.col.r;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.col.r;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}
	int   GetEffectColG(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.col.g;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.col.g;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.col.g;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}
	int   GetEffectColB(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.col.b;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.col.b;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.col.b;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}

	float GetEffectScale(int type = TYPE::START)
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.scale;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.scale;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.scale;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}
	float GetEffectAngle(int type = TYPE::START) 
	{
		switch (type)
		{
		case START:
			return Effect2DMgr.GetEffectNode()->m_sStart.angle;
			break;
		case MIDDLE:
			return Effect2DMgr.GetEffectNode()->m_sMiddle.angle;
			break;
		case END:
			return Effect2DMgr.GetEffectNode()->m_sEnd.angle;
			break;
		default:
			MyAssert("配列外", "err");
			return -1;
			break;
		}
	}

	// 最大値が必要な時
	int GetNodeMAX() { return Effect2DMgr.GetNodeMAX(); }

	// 中間点を使用するかのフラグ
	void SetMiddleFlag(bool flag) { Effect2DMgr.GetEffectNode()->m_bMiddleFlag = flag; }
	//bool GetMiddleFlag() 
	//{
	//	return Effect2DMgr.GetEffectNode()->m_bMiddleFlag;
	//}
	
	int GetMiddleFlag() 
	{
		if (Effect2DMgr.GetEffectNode()->m_bMiddleFlag == true) return 1;
		else return 0;
	}

	void SetRenderFlag(bool flag) { Effect2DMgr.GetEffectNode()->m_bRenderFlag = flag; };
	//bool GetRenderFlag() {return Effect2DMgr.GetEffectNode()->m_bRenderFlag;}
	int GetRenderFlag() 
	{
		if (Effect2DMgr.GetEffectNode()->m_bRenderFlag == true) return 1;
		else return 0;
	}

	// 名前
	void SetListName(char* listname) 
	{
		sprintf(Effect2DMgr.GetEffectNode()->m_sListName, "%s", listname); // 名前
	}

	std::string GetListName() { return Effect2DMgr.GetEffectNode()->m_sListName; };

	char* GetTexName() { return Effect2DMgr.GetEffectNode()->m_sName; };

	// ゲームフレーム
	int GetGameFrame() 
	{
		return Effect2DMgr.m_iEffectFrame;
	}

	void SetGameFrame(int gageFrame)
	{
		Effect2DMgr.m_iEffectFrame = gageFrame;
	}


	int GetGameFrameMAX()
	{
		return Effect2DMgr.m_iMaxEffectFrame;
	}

	void SetGameFrameMAX(int gageFramemax)
	{
		Effect2DMgr.m_iMaxEffectFrame = gageFramemax;
	}


private:
	

};