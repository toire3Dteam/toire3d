#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"

//+----------------------------
//	オーバードライブカットイン
//+----------------------------

class OverDriveCutin
{
public:
	OverDriveCutin(BasePlayer* pPlayer);
	~OverDriveCutin();

	void Update();
	void Render();

	void Action();

private:
	// 背景
	tdn2DObj* m_pBG;
	// プレイヤーを参照用
	BasePlayer* m_pPlayerData;
	Vector2 m_vPos;

	bool m_bRender;
	int m_iFrame;			//	
	bool m_bTurnOverFlag;

	tdn2DObj* m_pScreen;
	//Surface* m_pStencilSurface;//Z
	
							   
	tdn2DObj* m_pScreenMask;// マスク
	float m_fMaskRate;
	float m_fMaskPow;

	// スクロール文字
	struct ScrollFont
	{
		tdn2DAnim* pPic;
		Vector2 vPos;
	};
	ScrollFont m_pFont1;
	ScrollFont m_pFont2;
	ScrollFont m_pFont3;


	tdn2DObj* m_pPostLight;
};
//やったぜ
//色々微調整
//フォンtきゃらさいず
//ひかり
//
//うらのえんしゅつ
