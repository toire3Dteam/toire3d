#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"

//+-----------------------
//		顔グラ
//+-----------------------

class FacePic
{
public:
	FacePic(BasePlayer* pPlayer);
	~FacePic();

	void Update();
	void Render();
	void Action(int waitFrame);

private:
	
	// 顔グラ用
	Vector2 m_vPos;

	SIDE m_sSideFlag;
	// プレイヤーを参照用
	BasePlayer* m_pPlayerReferences;

	// パネルアニメーション
	BasePanelEffect* m_pPanelEF;

};
