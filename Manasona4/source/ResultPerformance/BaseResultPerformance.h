#pragma once
#include "TDNLIB.h"

/*************************/
//	BaseResultPerformace
/*************************/

class BaseResultPerformance
{
public:
	BaseResultPerformance()
	{
		m_bAction = false; 
		//PicClean(&m_winner);
		//m_winner.PicClean();
	};

	virtual ~BaseResultPerformance()
	{
		
	};

	virtual void Update() = 0;
	virtual void BackRender() = 0;
	virtual void FrontRender() = 0;
	virtual void Action() = 0;

	protected:
		bool m_bAction;

	// イラストの構造体
	struct Pic
	{
		tdn2DAnim* pic;
		//Vector2 pos;
		//int x, y;
		//Vector2 move;
		//int uvX, uvY;
		//Vector2 moveUV;

		Vector2 pos;
		Vector2 vMove;
		Vector2 uv;
		float rotate;

		void PicClean()
		{
			pic = nullptr;
			//x = y = 0;
			//move.x = move.y = 0.0f;
			//uvX= uvY = 0;
			//moveUV.x = moveUV.y = 0.0f;

			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			vMove.x = vMove.y = 0.0f;
			rotate = 0.0f;
		}

		Pic() 
		{
			pic = nullptr;
			pos.x = pos.y = 0.0f;
			uv.x = uv.y = 0.0f;
			vMove.x = vMove.y = 0.0f;
			rotate = 0.0f;
		}
	};


};


//+-------------------------------
//	封印されし魂の演出
//+-------------------------------
class AramitamaResultPerformance : public BaseResultPerformance
{
public:
	AramitamaResultPerformance();
	~AramitamaResultPerformance();

	void Update();
	void BackRender();
	void FrontRender();
	void Action();

private:
	// 必要なやつ
	Pic m_winner;
	Pic m_winnerDon;

	Pic m_edge;

	Pic m_back;
	Pic m_soul;
	Pic m_whiteInk;
	Pic m_blackInk;
	Pic m_blackStar;
	Pic m_whiteSoul;
};


//+-------------------------------
//	ぶんぶんの演出
//+-------------------------------
class AirouResultPerformance : public BaseResultPerformance
{
public:
	AirouResultPerformance();
	~AirouResultPerformance();

	void Update();
	void BackRender();
	void FrontRender();
	void Action();

private:
	// 必要なやつ
	Pic m_winner;
	Pic m_winnerDon;

	Pic m_edge;

	Pic m_back;
	
	Pic m_font;
	int m_iFontAnimFrame;
	Pic m_fontKill;

	Pic m_danger1;
	Pic m_danger2;

	Pic m_blackInk;
	Pic m_susi;
	Pic m_whiteSoul;
	
	// ECC
	Pic m_eccmanBody;
	Pic m_eccmanHead;

	// 地球
	Pic m_earth;

	// バックムービー
	tdnMovie*  m_movieMgr;
	tdn2DObj* m_movieTex;

};


//+-------------------------------
//	TEKIの演出
//+-------------------------------
class TekiResultPerformance : public BaseResultPerformance
{
public:
	TekiResultPerformance();
	~TekiResultPerformance();

	void Update();
	void BackRender();
	void FrontRender();
	void Action();

private:
	// 必要なやつ
	Pic m_winner;
	Pic m_winnerDon;
	Pic m_edge;
	Pic m_back;


	Pic m_font;
	int m_iFontAnimFrame;
	//Pic m_fontKill;

	Pic m_pTopLight;
	Pic m_pFlontRing;
	Pic m_pBackRing;

	Pic m_blackInk;
	Pic m_whiteSoul;

	// バックムービー
	tdnMovie*  m_movieMgr;
	tdn2DObj* m_movieTex;

};


//+-------------------------------
//	Nazenarabaの演出
//+-------------------------------
class NazenarabaResultPerformance : public BaseResultPerformance
{
public:
	NazenarabaResultPerformance();
	~NazenarabaResultPerformance();

	void Update();
	void BackRender();
	void FrontRender();
	void Action();

private:
	// 必要なやつ
	Pic m_winner;
	Pic m_winnerDon;
	Pic m_edge;
	Pic m_back;
	Pic m_back2;

	Pic m_font;
	int m_iFontAnimFrame;
	//Pic m_fontKill;

	Pic m_pTopLight;
	Pic m_pFlontRing;
	Pic m_pBackRing;

	Pic m_blackInk;
	Pic m_whiteSoul;

	Pic m_pBalloon;// 吹き出し

	static const int NORT_MAX = 3;
	Pic m_pNort[NORT_MAX];

	static const int STAR_MAX = 4;
	Pic m_pStar[STAR_MAX];

	static const int HART_MAX = 4;
	Pic m_pHart[HART_MAX];

	 // 
	//Pic m_pNort2; // 
	//Pic m_pNort3; // 


	// バックムービー
	tdnMovie*  m_movieMgr;
	tdn2DObj* m_movieTex;

};


//+-------------------------------
//	兄貴の演出
//+-------------------------------
class AnikiResultPerformance : public BaseResultPerformance
{
public:
	AnikiResultPerformance();
	~AnikiResultPerformance();

	void Update();
	void BackRender();
	void FrontRender();
	void Action();

private:
	// 必要なやつ
	Pic m_winner;
	Pic m_winnerDon;
	Pic m_edge;
	Pic m_back;
	Pic m_cloud;

	Pic m_font;
	int m_iFontAnimFrame;
	//Pic m_fontKill;

	Pic m_petal;
	Pic m_pFlontRing;
	Pic m_pBackRing;

	Pic m_blackInk;
	Pic m_whiteSoul;

	Pic m_pRose;// 

	static const int NORT_MAX = 3;
	Pic m_pNort[NORT_MAX];

	static const int STAR_MAX = 4;
	Pic m_pStar[STAR_MAX];

	static const int HART_MAX = 4;
	Pic m_pHart[HART_MAX];

	// 
	//Pic m_pNort2; // 
	//Pic m_pNort3; // 


	// バックムービー
	tdnMovie*  m_movieMgr;
	tdn2DObj* m_movieTex;

};


//+-------------------------------
//	バランスの演出
//+-------------------------------
class BalanceResultPerformance : public BaseResultPerformance
{
public:
	BalanceResultPerformance();
	~BalanceResultPerformance();

	void Update();
	void BackRender();
	void FrontRender();
	void Action();

private:
	// 必要なやつ
	Pic m_winner;
	Pic m_winnerDon;
	Pic m_edge;
	Pic m_back;

	Pic m_font;
	int m_iFontAnimFrame;
	//Pic m_fontKill;

	Pic m_petal;
	Pic m_pRose;// 
	Pic m_pNinjin;// 
	Pic m_pSaku;

	// バックムービー
	tdnMovie*  m_movieMgr;
	tdn2DObj* m_movieTex;

};
