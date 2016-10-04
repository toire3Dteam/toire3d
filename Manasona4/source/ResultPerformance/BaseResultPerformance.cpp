#include "BaseResultPerformance.h"
#include "../system/System.h"


/*****************************/
//		ベースの演出
/****************************/

//void BaseResultPerformance::PicClean(Pic* pic)
//{
//	pic->pic = nullptr;
//	pic->x = pic->y = 0;
//	pic->uvX= pic->uvY = 0;
//	pic->rotate = 0.0f;
//}


/*****************************/
//	アラミタマの演出
/****************************/

AramitamaResultPerformance::AramitamaResultPerformance()
{
	m_winner.PicClean();
	m_winner.pic = new tdn2DAnim("DATA/Result/Aramitama/Winner.png");
	m_winner.pic->OrderShake(3, 0, 63, 3);

	m_winnerDon.PicClean();
	m_winnerDon.pic = new tdn2DAnim("DATA/Result/Aramitama/Winner.png");
	m_winnerDon.pic->OrderRipple(6, 1, 0.025f);

	m_edge.PicClean();
	m_edge.pic = new tdn2DAnim("DATA/Result/Aramitama/edge.png");
	m_edge.pic->OrderShake(3, 0, 63, 3);
	

	m_back.PicClean();
	m_back.pic = new tdn2DAnim("DATA/Result/Aramitama/back.png");
	m_back.pic->OrderNone();

	m_soul.PicClean();
	m_soul.pic = new tdn2DAnim("DATA/Result/Aramitama/soul.png");
	m_soul.pos.x = 100;
	m_soul.pos.y = 100;
	m_soul.pic->OrderShrink(8, 1, 3);

	m_whiteInk.PicClean();
	m_whiteInk.pic = new tdn2DAnim("DATA/Result/Aramitama/whiteInk_Screen.png");
	m_whiteInk.uv.x = 0.0f;
	m_whiteInk.uv.y = 0.0f;
	m_whiteInk.pic->OrderMoveAppeared(10, -900, 550);

	m_blackInk.PicClean();
	m_blackInk.pic = new tdn2DAnim("DATA/Result/Aramitama/blackInk.png");
	m_blackInk.uv.x = 0.0f;
	m_blackInk.uv.y = 0.0f;
	m_blackInk.pic->OrderMoveAppeared(10, -1000, -100);
	
	m_blackStar.PicClean();
	m_blackStar.pic = new tdn2DAnim("DATA/Result/Aramitama/blackStar.png");
	m_blackStar.pic->OrderNone();

	
	m_whiteSoul.PicClean();
	m_whiteSoul.pic = new tdn2DAnim("DATA/Result/Aramitama/WhiteSplash.png");
	m_whiteSoul.pic->OrderMoveAppeared(8, -650, -500);

	m_movieMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	m_movieTex = new tdn2DObj(m_movieMgr->GetTexture());

}

AramitamaResultPerformance::~AramitamaResultPerformance()
{
	SAFE_DELETE(m_winner.pic);
	SAFE_DELETE(m_winnerDon.pic);
	SAFE_DELETE(m_edge.pic);
	SAFE_DELETE(m_back.pic);
	SAFE_DELETE(m_soul.pic);
	SAFE_DELETE(m_whiteInk.pic);
	SAFE_DELETE(m_whiteSoul.pic);
	SAFE_DELETE(m_blackInk.pic);
	SAFE_DELETE(m_blackStar.pic);

	SAFE_DELETE(m_movieMgr);
	SAFE_DELETE(m_movieTex);
}

void AramitamaResultPerformance::Update()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	m_back.uv.x -= 0.2f;
	m_back.pic->Update();
	
	m_soul.pos.x += 0.05f;
	m_soul.pic->Update();

	// 後ろの白魂
	m_whiteSoul.pos.x += 0.05f;
	m_whiteSoul.pic->Update();

	m_blackStar.pos.x += -0.1f;
	m_blackStar.rotate += 0.0005f;
	m_blackStar.pic->SetAngle(m_blackStar.rotate);
	m_blackStar.pic->Update();

	// インク
	m_blackInk.pic->Update();	
	m_whiteInk.pic->Update();
	
	// 手前のプレイヤー
	m_winner.pos.x += 0.2f;
	m_winner.pos.y -= 0.1f;
	m_winner.pic->Update();

	m_winnerDon.pic->Update();

	// 
	m_edge.pic->Update();
	m_edge.pos.x += 0.4f;
	m_edge.pos.y -= 0.2f;
};

void AramitamaResultPerformance::BackRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	// ムービー
	//m_movieTex->Render(0, 0);

	// 自力
	m_back.pic->Render(0, 0, m_back.pic->GetWidth(), m_back.pic->GetHeight(),
		(int)m_back.uv.x, (int)m_back.uv.y, m_back.pic->GetWidth(), m_back.pic->GetHeight());
	
	m_whiteSoul.pic->RenderSpecial((int)m_whiteSoul.pos.x, (int)m_whiteSoul.pos.y);

	m_soul.pic->Render((int)m_soul.pos.x, (int)m_soul.pos.y);

	m_blackInk.pic->RenderSpecial((int)m_blackInk.pos.x, (int)m_blackInk.pos.y);

	m_blackStar.pic->Render((int)m_blackStar.pos.x, (int)m_blackStar.pos.y);


};

void AramitamaResultPerformance::FrontRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	m_edge.pic->Render((int)m_edge.pos.x, (int)m_edge.pos.y);
	m_winner.pic->Render((int)m_winner.pos.x, (int)m_winner.pos.y);
	m_winnerDon.pic->Render((int)m_winnerDon.pos.x, (int)m_winnerDon.pos.y,RS::ADD);
	m_whiteInk.pic->RenderSpecial((int)m_whiteInk.pos.x, (int)m_whiteInk.pos.y);
};

void AramitamaResultPerformance::Action()
{
	m_bAction = true;

	// 初期化＆始動
	m_winner.pos.x = 0;
	m_winner.pos.y = 0;
	m_winner.pic->Action();

	m_winnerDon.pos.x = 0;
	m_winnerDon.pos.y = 0;
	m_winnerDon.pic->Action();

	m_edge.pos.x = -20;
	m_edge.pos.y = -10;
	m_edge.pic->Action();

	m_back.pic->Action();

	m_soul.pos.y = 0;
	m_soul.pos.x = 200;
	m_soul.pic->Action(8);

	m_blackStar.pos.x = 600;
	m_blackStar.pos.y = -180;
	m_blackStar.rotate = 0;
	m_blackStar.pic->Action();

	m_whiteInk.pos.x = 0;
	m_whiteInk.pos.y = 0;
	m_whiteInk.pic->Action(4);

	m_blackInk.pos.x = -250;
	m_blackInk.pos.y = -300;
	m_blackInk.pic->Action(4);

	m_whiteSoul.pos.x = 0;
	m_whiteSoul.pos.y = 0;
	m_whiteSoul.pic->Action(10);

	// ムービープレイ
	//m_movieMgr->SetTime(0);
	//m_movieMgr->Play();
}