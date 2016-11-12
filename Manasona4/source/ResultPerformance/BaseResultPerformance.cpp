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


//+-------------------------------
//	アラミタマの演出
//+-------------------------------

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




//+-------------------------------
//	アイル―の演出
//+-------------------------------

AirouResultPerformance::AirouResultPerformance()
{
	m_winner.PicClean();
	m_winner.pic = new tdn2DAnim("DATA/Result/Airou/Winner.png");
	m_winner.pic->OrderShake(3, 0, 63, 3);
	m_winner.pic->SetScale(1.15f);

	m_winnerDon.PicClean();
	m_winnerDon.pic = new tdn2DAnim("DATA/Result/Airou/Winner.png");
	m_winnerDon.pic->OrderRipple(6, 1.15f, 0.025f);
	m_winnerDon.pic->SetScale(1.15f);

	m_edge.PicClean();
	m_edge.pic = new tdn2DAnim("DATA/Result/Airou/edge.png");
	m_edge.pic->OrderShake(3, 0, 63, 3);
	m_edge.pic->SetScale(1.25f);

	m_back.PicClean();
	m_back.pic = new tdn2DAnim("DATA/Result/Airou/back.png");
	m_back.pic->SetScale(1.5f);
	m_back.pic->OrderNone();

	m_font.PicClean();
	m_font.pic = new tdn2DAnim("DATA/Result/Airou/no_kill.png");
	m_font.pos.x = -150;
	m_font.pos.y = 0;
	//m_font.pic->SetScale(2.0f);
	m_font.pic->OrderShrink(8, 1, 3);
	m_iFontAnimFrame = 0;

	m_fontKill.PicClean();
	m_fontKill.pic = new tdn2DAnim("DATA/Result/Airou/Kill.png");
	m_fontKill.pos.x = 420;
	m_fontKill.pos.y = 300;
	//m_fontKill.pic->SetScale(2.0f);
	m_fontKill.pic->OrderMoveAppeared(12, (int)m_fontKill.pos.x, -200);
	
	m_danger1.PicClean();
	m_danger1.pic = new tdn2DAnim("DATA/Result/Airou/Danger.png");
	m_danger1.uv.x = 0.0f;
	m_danger1.uv.y = 0.0f;
	m_danger1.rotate = PI / 8;
	m_danger1.pic->SetAngle(m_danger1.rotate);
	m_danger1.pic->OrderMoveAppeared(10, 1280, 250);

	m_danger2.PicClean();
	m_danger2.pic = new tdn2DAnim("DATA/Result/Airou/Danger.png");
	m_danger2.uv.x = 0.0f;
	m_danger2.uv.y = 0.0f;
	m_danger2.rotate = PI / 3;
	m_danger2.pic->SetAngle(m_danger2.rotate);
	m_danger2.pic->SetScale(0.75f);
	m_danger2.pic->OrderMoveAppeared(10, 940, 0);


	m_blackInk.PicClean();
	m_blackInk.pic = new tdn2DAnim("DATA/Result/Aramitama/blackInk.png");
	m_blackInk.uv.x = 0.0f;
	m_blackInk.uv.y = 0.0f;
	m_blackInk.pic->OrderMoveAppeared(10, -1000, -100);

	m_susi.PicClean();
	m_susi.pic = new tdn2DAnim("DATA/Result/Airou/susi.png");
	m_susi.pic->OrderNone();


	m_whiteSoul.PicClean();
	m_whiteSoul.pic = new tdn2DAnim("DATA/Result/Aramitama/WhiteSplash.png");
	m_whiteSoul.pic->OrderMoveAppeared(8, -650, -500);

	m_movieMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	m_movieTex = new tdn2DObj(m_movieMgr->GetTexture());



	m_eccmanBody.PicClean();
	m_eccmanBody.pic = new tdn2DAnim("DATA/Result/Airou/eccman_Body.png");
	m_eccmanBody.pic->OrderNone();
	
	m_eccmanHead.PicClean();
	m_eccmanHead.pic = new tdn2DAnim("DATA/Result/Airou/eccman_Head.png");
	m_eccmanHead.pic->OrderShake(180, 0, -15, 2);

	m_earth.PicClean();
	m_earth.pic = new tdn2DAnim("DATA/Result/Airou/earth.png");
	m_earth.pic->OrderNone();
	m_earth.pos.x = 650;
	m_earth.pos.y = 0;
}

AirouResultPerformance::~AirouResultPerformance()
{
	SAFE_DELETE(m_winner.pic);
	SAFE_DELETE(m_winnerDon.pic);
	SAFE_DELETE(m_edge.pic);
	SAFE_DELETE(m_back.pic);
	SAFE_DELETE(m_font.pic);	
	SAFE_DELETE(m_fontKill.pic);
	SAFE_DELETE(m_danger1.pic);
	SAFE_DELETE(m_danger2.pic);
	SAFE_DELETE(m_whiteSoul.pic);
	SAFE_DELETE(m_blackInk.pic);
	SAFE_DELETE(m_susi.pic);

	SAFE_DELETE(m_movieMgr);
	SAFE_DELETE(m_movieTex);

	SAFE_DELETE(m_eccmanBody.pic);
	SAFE_DELETE(m_eccmanHead.pic);

	SAFE_DELETE(m_earth.pic);
}

void AirouResultPerformance::Update()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	m_back.uv.x -= 0.2f;
	m_back.pic->Update();

	/*m_soul.pos.x += 0.05f;*/
	m_font.pic->Update();
	
	//m_iFontAnimFrame++;
	//if (m_iFontAnimFrame >= 6)
	//{
	//	m_iFontAnimFrame = 0;
	//	m_font.uv.x += 512;
	//}

	m_fontKill.pos.x += 0.25f;
	m_fontKill.pic->Update();

	// 後ろの白魂
	m_whiteSoul.pos.x += 0.05f;
	m_whiteSoul.pic->Update();

	m_susi.pos.x += -15.1f;
	m_susi.rotate += 0.25f;
	m_susi.pic->SetAngle(m_susi.rotate);
	m_susi.pic->Update();

	
	m_blackInk.pic->Update();
	
	// デンジャー
	m_danger1.uv.x += 0.5f;
	m_danger1.pic->Update();

	m_danger2.uv.x += 0.5f;
	m_danger2.pic->Update();

	// 手前のプレイヤー
	m_winner.pos.x += 0.2f;
	m_winner.pos.y -= 0.1f;
	m_winner.pic->Update();

	m_winnerDon.pic->Update();

	// 
	m_edge.pic->Update();
	m_edge.pos.x += 0.2f;
	m_edge.pos.y -= 0.1f;

	m_eccmanBody.pic->Update();
	m_eccmanHead.pic->Update();

	m_earth.rotate += 0.01f;
	m_earth.pic->SetAngle(m_earth.rotate);
	m_earth.pic->Update();
};

void AirouResultPerformance::BackRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	// ムービー
	//m_movieTex->Render(0, 0);

	// 自力
	m_back.pic->Render(0, 0, m_back.pic->GetWidth(), m_back.pic->GetHeight(),
		(int)m_back.uv.x, (int)m_back.uv.y, m_back.pic->GetWidth(), m_back.pic->GetHeight());

	//m_whiteSoul.pic->RenderSpecial((int)m_whiteSoul.pos.x, (int)m_whiteSoul.pos.y);

	m_susi.pic->Render((int)m_susi.pos.x, (int)m_susi.pos.y);

	m_earth.pic->Render((int)m_earth.pos.x, (int)m_earth.pos.y);

	//m_font.pic->Render((int)m_font.pos.x, (int)m_font.pos.y, 512, 512, (int)m_font.uv.x, 0, 512, 512);
	m_font.pic->Render((int)m_font.pos.x, (int)m_font.pos.y);


	//m_blackInk.pic->RenderSpecial((int)m_blackInk.pos.x, (int)m_blackInk.pos.y);

	//m_eccmanBody.pic->Render(0, 350);
	//m_eccmanHead.pic->Render(0, 350);

	//m_eccmanBody.pic->Render(300, 350);
	//m_eccmanHead.pic->Render(300, 350);

	//m_eccmanBody.pic->Render(600, 350);
	//m_eccmanHead.pic->Render(600, 350);
};

void AirouResultPerformance::FrontRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	m_edge.pic->Render((int)m_edge.pos.x, (int)m_edge.pos.y);
	m_winner.pic->Render((int)m_winner.pos.x, (int)m_winner.pos.y);
	m_winnerDon.pic->Render((int)m_winnerDon.pos.x, (int)m_winnerDon.pos.y, RS::ADD);
	m_danger2.pic->Render((int)m_danger2.pos.x, (int)m_danger2.pos.y, 1024, 96, (int)m_danger2.uv.x, 0, 1024, 96);
	m_danger1.pic->Render((int)m_danger1.pos.x, (int)m_danger1.pos.y, 1024, 96, (int)m_danger1.uv.x, 0, 1024, 96);

	//m_fontKill.pic->Render((int)m_fontKill.pos.x, (int)m_fontKill.pos.y);

};

void AirouResultPerformance::Action()
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

	m_font.pic->Action(8);
	
	m_fontKill.pos.x = 420;
	m_fontKill.pos.y = 100;
	m_fontKill.pic->Action();

	m_susi.pos.x = 500;
	m_susi.pos.y = 100;
	m_susi.rotate = 0;
	m_susi.pic->Action();

	m_danger1.pos.x = 420;
	m_danger1.pos.y = 620;
	m_danger1.pic->Action(4);

	m_danger2.pos.x = 620;
	m_danger2.pos.y = 570;
	m_danger2.pic->Action(4);

	m_blackInk.pos.x = -250;
	m_blackInk.pos.y = -300;
	m_blackInk.pic->Action(4);

	m_whiteSoul.pos.x = 0;
	m_whiteSoul.pos.y = 0;
	m_whiteSoul.pic->Action(10);

	
	m_eccmanBody.pic->Action();
	m_eccmanHead.pic->Action();

	m_earth.pic->Action();

	// ムービープレイ
	//m_movieMgr->SetTime(0);
	//m_movieMgr->Play();
}







