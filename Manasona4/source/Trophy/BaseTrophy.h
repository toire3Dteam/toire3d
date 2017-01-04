#pragma once
#include "TDNLIB.h"

//+------------------------------
//	基本のトロフィー
//+------------------------------

struct TROPHY_DATA
{
	tdn2DObj* pIcon;	// トロフィーのアイコン
	std::string sTitle;	// トロフィーのタイトル
	std::string sText;	// トロフィーの文字
	Vector2 vPos;		// 場所
	float iAlpha;		// 透明度

	TROPHY_DATA();
};

class BaseTrophy
{
public:
	BaseTrophy();
	virtual ~BaseTrophy();

	virtual void Update();
	virtual void Render();
	virtual void Action(int iDelayFrame = 0);

	void RenderRoom(int iX, int iY);		//	閲覧用

	// アクセサ
	TROPHY_DATA GetTrophy() { return m_tagTrophy; }// トロフィーの構造体

	// 閲覧用のポジション
	Vector2 GetRoomPos() { return m_vRoomPos; };
	void SetRoomPos(Vector2 vPos) { m_vRoomPos = vPos; }

protected:
	bool m_bAction;
	int m_iActiveFrame;		//	動いてる時間

	TROPHY_DATA m_tagTrophy;
	
	tdn2DObj* m_pPlate;
	tdn2DAnim* m_pIconRip;// トロフィーのアイコンの波紋

	int m_iDelayFrame;

	Vector2 m_vRoomPos;		//	閲覧用の場所

};

//+---------------------------------
//	初プレイのトロフィー
//+---------------------------------

class FirstTrophy :public BaseTrophy
{
public:
	FirstTrophy();
	~FirstTrophy();

private:

};


//+---------------------------------
//	初めて対戦をしたのトロフィー
//+---------------------------------

class FirstBattleTrophy :public BaseTrophy
{
public:
	FirstBattleTrophy();
	~FirstBattleTrophy();

private:

};



