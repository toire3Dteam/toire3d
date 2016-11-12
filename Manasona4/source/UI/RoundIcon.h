#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "HpGage.h"

//+------------------------------
//		ラウンドアイコン
//+------------------------------

class RoundIcon
{
public:
	RoundIcon(BasePlayer* pPlayer,int iRoundNum );
	~RoundIcon();

	void Update();
	void Render();
	void Action(int waitFrame);

	// Winアイコン出現
	void AppIcon();
		

private:
	// WinIcon
	struct WinIcon
	{
		// アイコン
		tdn2DAnim* pIconTable;
		tdn2DAnim* pWinIcon;
		tdn2DAnim* pWinIconRip;

		Vector2 vPos;
		bool bWinFlag;
	};
	int m_iMaxIcons;
	std::vector<WinIcon> m_sIcons;

	SIDE m_sSideFlag;

	// プレイヤーを参照用
	BasePlayer* m_pPlayerReferences;

	// アニメーション用
	int m_iAnimTimer;
};

