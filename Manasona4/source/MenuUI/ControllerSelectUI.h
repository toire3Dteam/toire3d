#pragma once
#include "TDNLIB.h"

//+------------------------
//	コントローラーセレクト
//+------------------------

class ControllerSelectUI
{
private:

	// コントローラ
	struct Pad
	{
		tdn2DAnim* pPic;
		tdn2DAnim* pAroow;
		Vector2 vPos;
		Vector2 vNextPos;
		Vector2 vFirstPos;
	};

	// ギア
	struct Gia
	{
		tdn2DAnim* pPic;
		float fAngle;
		float fNextAngle;
	};

	// プレイヤーカード
	struct PlayerCard
	{
		tdn2DAnim* pPic;
		tdn2DAnim* pFont;
		bool bPlayerFlag;
		int iPlayerDeviceID;
	};


public:
	ControllerSelectUI();
	~ControllerSelectUI();

	void Update();
	void Render();
	void Action();
	void Stop();

	// パッドを動かす処理
	void PadMoveLeftSide(int padNo);
	void PadMoveRightSide(int padNo);
	void PadMoveBack(int padNo);

	PlayerCard GetLeftPlayer() { return m_pPlayerCardLeft; }
	PlayerCard GetRightPlayer() { return m_pPlayerCardRight; }
	bool IsLeftPlayer() 
	{
		return  (m_pPlayerCardLeft.bPlayerFlag == true) ? true : false;
	}

	bool IsRightPlayer()
	{
		return  (m_pPlayerCardRight.bPlayerFlag == true) ? true : false;
	}

private:	
	tdn2DObj* m_pInfoBox;
	tdn2DAnim* m_pInfo;

	static const int PAD_MAX = 4;

	Pad m_pPad[PAD_MAX];

	Gia m_pGia;
	Gia m_pGiaFont;

	PlayerCard m_pPlayerCardLeft;
	PlayerCard m_pPlayerCardRight;

	// 上のやつ
	tdn2DAnim* m_pPlayerToPlayer;

	// 上のやつ
	tdn2DObj* m_pTopUI;
};
