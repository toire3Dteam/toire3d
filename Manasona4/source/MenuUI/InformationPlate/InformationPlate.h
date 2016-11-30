#pragma once
#include "TDNLIB.h"

//+-----------------------------------
//	説明用のプレート
//+-----------------------------------

class InformationPlate
{
public:
	InformationPlate();
	~InformationPlate();

	void Update();
	void Render();

	void Action();
	void Stop();

private:
	enum 
	{
		START_POS_Y = 598
	};

	// プレートの絵
	tdn2DAnim* m_pPic;

	// 場所
	Vector2 m_vPos;
	float m_fMoveY;

	// フラグ
	bool m_bRender;

};
