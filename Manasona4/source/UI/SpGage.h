#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "HpGage.h"

//+------------------------------
//		SPゲージ
//+------------------------------

class SpGage
{
public:
	SpGage(BasePlayer* pPlayer, SIDE bSide);
	~SpGage();

	void Update();
	void Render();

private:	
	// SPゲージ
	tdn2DObj* m_pGage;	
	Vector2 m_vPos;
	float m_fRate;
	int m_iGageWidth;
	int m_iGageHeight;

	SIDE m_sSideFlag;

	// プレイヤーを参照用
	BasePlayer* m_pPlayerReferences;

};