#pragma once
#include "TDNLIB.h"
#include "../BaseEntity/Entity/EntityEnum.h"

//+-------------------------
//	ラウンドコール
//+-------------------------

//enum CALL_TYPE
//{
//
//};

// ベースラウンドコール
class BaseRoundCall
{
public:
	BaseRoundCall() 
	{
		m_bActionFlag = false;
	};
	virtual ~BaseRoundCall()
	{

	}

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Action(){}
	virtual void Action(int roundnum){}
	virtual void Action(ENTITY_ID WinnerID){}

protected:		
	// アクションフラグ
	bool m_bActionFlag;

};

// 最初のラウンドコール
class FirstRoundCall :public BaseRoundCall
{
public:
	FirstRoundCall();
	~FirstRoundCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();
	void Action(int roundnum) {  Action(); };



protected:
	// ラウンドナンバー
	tdn2DAnim* m_pNumber;
	

	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	// ラウンドロール
	//tdn2DAnim* m_pRollBig;
	//float m_fRollBigAngle;
	Pic m_pRollBig;

	//tdn2DAnim* m_pRollMini;
	//float m_fRollMiniAngle;
	Pic m_pRollMini;

	enum { ROLL_START = 12, ROLL_END = 112 };

	Pic m_pBlackLine;

	Pic m_pFirstFont;

	// 光のリング
	tdn2DAnim* m_pLightRing;

	// 光の線
	tdn2DAnim* m_pLightLine;

	// 
	int m_iFrame;


};


// リベルコール
class RebelCall :public BaseRoundCall
{
public:
	RebelCall();
	~RebelCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();
	void Action(int roundNum);
	void MaskScreen();


protected:


	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	// Rebel_Circle
	Pic m_pCenterCircle;

	Pic m_pBG;

	enum { REBEL_START = 12, REBEL_END = 119 };

	// Pic
	// 黒い線
	Pic m_pBlackLineL;
	Pic m_pBlackLineR;

	Pic m_pFirstFont;

	// 光のリング
	tdn2DAnim* m_pLightRing;

	// 光の線
	tdn2DAnim* m_pLightLine;

	// ラウンドナンバー
	tdn2DAnim* m_pNumber;
	// ラウンドの数字
	int m_iRoundNumber;

	// マスク用レンダーターゲット
	tdn2DObj* m_pMaskScreen;
	tdn2DObj* m_pMaskPic;

	// 
	int m_iFrame;
	


};



// アクションコール
class ActionCall :public BaseRoundCall
{
public:
	ActionCall();
	~ActionCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();
	void Action(int roundnum) {  Action(); };

protected:


	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};
	
	Pic m_pBG;
	Pic m_pBGRip;

	enum { ACTION_START = 1, ACTION_END = 34, END_FRAME = 42 };

	Pic m_pFirstFont;

	// 光のリング
	tdn2DAnim* m_pLightRing;

	// 光の線
	tdn2DAnim* m_pLightLine;

	// 
	int m_iFrame;



};



// フィニッシュコール
class FinishCall :public BaseRoundCall
{
public:
	FinishCall();
	~FinishCall();

	void FrameMove();
	void Update();
	void Render();
	void Action(ENTITY_ID WinnerID);

protected:

	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { FINISH_START = 8, FINISH_END = 138, END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// 光の線
	tdn2DAnim* m_pLightLine;
	
	// 
	int m_iFrame;

	// 勝ったやつのID
	ENTITY_ID m_WinnerID;
};



// オーバードライブフィニッシュコール
class OverDriveFinishCall :public BaseRoundCall
{
public:
	OverDriveFinishCall();
	~OverDriveFinishCall();

	void FrameMove();
	void Update();
	void Render();
	void Action(ENTITY_ID WinnerID);

protected:

	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { OVER_FINISH_START = 18, OVER_FINISH_END = 138, OVER_END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// 光の線
	tdn2DAnim* m_pLightLine;

	Pic m_pSircle;

	// 
	int m_iFrame;

	// 勝ったやつのID
	ENTITY_ID m_WinnerID;
};

// タイムアップコール
class TimeUpCall :public BaseRoundCall
{
public:
	TimeUpCall();
	~TimeUpCall();

	void FrameMove();
	void Update();
	void Render();
	void Action(ENTITY_ID WinnerID);

protected:

	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { OVER_FINISH_START = 18, OVER_FINISH_END = 138, OVER_END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// 光の線
	tdn2DAnim* m_pLightLine;

	Pic m_pSircle;

	// 
	int m_iFrame;

	// 勝ったやつのID
	ENTITY_ID m_WinnerID;
};


// ドローコール
class DrawCall :public BaseRoundCall
{
public:
	DrawCall();
	~DrawCall();

	void FrameMove();
	void Update();
	void Render();
	void Action();

protected:

	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		Vector2 pos;
		Vector2 uv;
		float rotate;
		float scale;
		void PicClean()
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			rotate = 0.0f;
			scale = 1.0f;
		}
	};

	enum { OVER_FINISH_START = 18, OVER_FINISH_END = 138, OVER_END_FRAME = 150 };

	Pic m_pBG;
	Pic m_pBGRip;
	Pic m_pFont;

	// 光の線
	tdn2DAnim* m_pLightLine;

	Pic m_pSircle;

	// 
	int m_iFrame;
};