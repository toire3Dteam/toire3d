#pragma once
#include "TDNLIB.h"
#include "Effect\UVEffect\BaseUVEffect.h"


//+------------------------------
//	オーバードライブ用のステージ
//+------------------------------

class OverDriveStage
{
public:
	OverDriveStage();
	~OverDriveStage();

	void Update();
	void Render();

	// アクセサ
	tdn2DObj* GetScreen() { return m_pScreen; }


private:
	tdn2DObj* m_pBG;

	tdn2DObj* m_pScreen;
	Surface* m_pStencilSurface;//Z

	// スピード線
	BaseUVEffect* m_pSpeedLine;

	// 平行線
	tdn2DObj* m_pSpeedLine2D;
	float m_fSpeed;
};

