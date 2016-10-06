#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "HpGage.h"

//+------------------------------
//		SP�Q�[�W
//+------------------------------

class SpGage
{
public:
	SpGage(BasePlayer* pPlayer, SIDE bSide);
	~SpGage();

	void Update();
	void Render();

private:	
	// SP�Q�[�W
	tdn2DObj* m_pGage;	
	Vector2 m_vPos;
	float m_fRate;
	int m_iGageWidth;
	int m_iGageHeight;

	SIDE m_sSideFlag;

	// �v���C���[���Q�Ɨp
	BasePlayer* m_pPlayerReferences;

};