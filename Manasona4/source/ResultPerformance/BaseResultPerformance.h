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
			rotate = 0.0f;
		}
	};


};



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

	// バックムービー
	tdnMovie*  m_movieMgr;
	tdn2DObj* m_movieTex;

};
//tdn2DobjwoAnimnisuru 
//SpaeadとかいみわからんｍにゃちうのなんまえをScareとかにもこのさいｓかえる