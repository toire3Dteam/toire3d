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
//	封印されし魂の演出
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
//	ぶんぶんの演出
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

	//m_movieMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	//m_movieTex = new tdn2DObj(m_movieMgr->GetTexture());



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

	//SAFE_DELETE(m_movieMgr);
	//SAFE_DELETE(m_movieTex);

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

//+-------------------------------
//	テキの演出
//+-------------------------------

TekiResultPerformance::TekiResultPerformance()
{
	m_winner.PicClean();
	m_winner.pic = new tdn2DAnim("DATA/Result/teki/Winner.png");
	m_winner.pic->OrderShake(3, 0, 63, 3);
	//m_winner.pic->SetScale(1.15f);

	m_winnerDon.PicClean();
	m_winnerDon.pic = new tdn2DAnim("DATA/Result/teki/Winner.png");
	m_winnerDon.pic->OrderRipple(6, 1.15f, 0.025f);
	//m_winnerDon.pic->SetScale(1.15f);

	m_edge.PicClean();
	m_edge.pic = new tdn2DAnim("DATA/Result/teki/Edge.png");
	m_edge.pic->OrderShake(3, 0, 63, 3);
	//m_edge.pic->SetScale(1.25f);

	m_back.PicClean();
	m_back.pic = new tdn2DAnim("DATA/Result/teki/back.png");
	//m_back.pic->SetScale(1.5f);
	m_back.pic->OrderNone();

	m_font.PicClean();
	m_font.pic = new tdn2DAnim("DATA/Result/teki/KO.png");
	m_font.pos.x = -150;
	m_font.pos.y = 0;
	//m_font.pic->SetScale(2.0f);
	m_font.pic->OrderShrink(8, 1, 3);
	m_iFontAnimFrame = 0;

	//m_fontKill.PicClean();
	//m_fontKill.pic = new tdn2DAnim("DATA/Result/teki/Kill.png");
	//m_fontKill.pos.x = 420;
	//m_fontKill.pos.y = 300;
	//m_fontKill.pic->SetScale(2.0f);
	//m_fontKill.pic->OrderMoveAppeared(12, (int)m_fontKill.pos.x, -200);

	// リング
	m_pFlontRing.PicClean();
	m_pFlontRing.pic = new tdn2DAnim("DATA/Result/teki/FlontRing.png");
	m_pFlontRing.uv.x = 0.0f;
	m_pFlontRing.uv.y = 0.0f;
	m_pFlontRing.pic->OrderMoveAppeared(10, -1000, 500);

	m_pBackRing.PicClean();
	m_pBackRing.pic = new tdn2DAnim("DATA/Result/teki/BackRing.png");
	m_pBackRing.uv.x = 0.0f;
	m_pBackRing.uv.y = 0.0f;
	m_pBackRing.pic->OrderMoveAppeared(10, 1280, 500);

	// 上のリングの光
	m_pTopLight.PicClean();
	m_pTopLight.pic = new tdn2DAnim("DATA/Result/teki/TopLight.png");
	m_pTopLight.uv.x = 0.0f;
	m_pTopLight.uv.y = 0.0f;
	m_pTopLight.pic->OrderMoveAppeared(10, -800, 100);


	m_blackInk.PicClean();
	m_blackInk.pic = new tdn2DAnim("DATA/Result/teki/blackInk.png");
	m_blackInk.uv.x = 0.0f;
	m_blackInk.uv.y = 0.0f;
	m_blackInk.pic->OrderMoveAppeared(10, -1000, -100);


	m_whiteSoul.PicClean();
	m_whiteSoul.pic = new tdn2DAnim("DATA/Result/Aramitama/WhiteSplash.png");
	m_whiteSoul.pic->OrderMoveAppeared(8, -650, -500);

	//m_movieMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	//m_movieTex = new tdn2DObj(m_movieMgr->GetTexture());

	
}

TekiResultPerformance::~TekiResultPerformance()
{
	SAFE_DELETE(m_winner.pic);
	SAFE_DELETE(m_winnerDon.pic);
	SAFE_DELETE(m_edge.pic);
	SAFE_DELETE(m_back.pic);
	SAFE_DELETE(m_font.pic);
	//SAFE_DELETE(m_fontKill.pic);
	SAFE_DELETE(m_pFlontRing.pic);
	SAFE_DELETE(m_pBackRing.pic);
	SAFE_DELETE(m_pTopLight.pic);
	SAFE_DELETE(m_whiteSoul.pic);
	SAFE_DELETE(m_blackInk.pic);

	//SAFE_DELETE(m_movieMgr);
	//SAFE_DELETE(m_movieTex);


}

void TekiResultPerformance::Update()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	//m_back.uv.x -= 0.2f;
	m_back.pic->Update();

	/*m_soul.pos.x += 0.05f;*/
	m_font.pos.x += 0.1f;
	m_font.pos.y += -0.05f;

	m_blackInk.pos.x += 0.1f;
	m_blackInk.pos.y += -0.05f;

	m_font.pic->Update();


	// 後ろの白魂
	m_whiteSoul.pos.x += 0.05f;
	m_whiteSoul.pic->Update();


	m_blackInk.pic->Update();

	// リング
	m_pBackRing.pos.x += -0.3f;// 左にわー
	m_pBackRing.pos.y += +0.125f;
	m_pBackRing.pic->Update();

	m_pFlontRing.pos.x += 0.4f;// 右にわー
	m_pFlontRing.pos.y += -0.125f;
	m_pFlontRing.pic->Update();

	// 上の光
	m_pTopLight.pic->Update();

	// 手前のプレイヤー
	m_winner.pos.x += 0.2f;
	m_winner.pos.y -= 0.1f;
	m_winner.pic->Update();

	m_winnerDon.pic->Update();

	// 
	m_edge.pic->Update();
	m_edge.pos.x += 0.2f;
	m_edge.pos.y -= 0.1f;

};

void TekiResultPerformance::BackRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	// 自力
	m_back.pic->Render(0, 0, m_back.pic->GetWidth(), m_back.pic->GetHeight(),
		(int)m_back.uv.x, (int)m_back.uv.y, m_back.pic->GetWidth(), m_back.pic->GetHeight());

	// 後ろのライト
	//m_pTopLight.pic->Render((int)m_pTopLight.pos.x, (int)m_pTopLight.pos.y);

	// 後ろのリング
	m_pBackRing.pic->Render((int)m_pBackRing.pos.x, (int)m_pBackRing.pos.y);
	
	// 墨
	m_blackInk.pic->RenderSpecial((int)m_blackInk.pos.x, (int)m_blackInk.pos.y);
	// タイトル
	m_font.pic->Render((int)m_font.pos.x, (int)m_font.pos.y);

};

void TekiResultPerformance::FrontRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	// 手前のリング
	m_pFlontRing.pic->Render((int)m_pFlontRing.pos.x, (int)m_pFlontRing.pos.y);

	m_edge.pic->Render((int)m_edge.pos.x, (int)m_edge.pos.y);
	m_winner.pic->Render((int)m_winner.pos.x, (int)m_winner.pos.y);
	m_winnerDon.pic->Render((int)m_winnerDon.pos.x, (int)m_winnerDon.pos.y, RS::ADD);


	//m_fontKill.pic->Render((int)m_fontKill.pos.x, (int)m_fontKill.pos.y);

};

void TekiResultPerformance::Action()
{
	m_bAction = true;

	// 初期化＆始動
	m_winner.pos.x = 0;
	m_winner.pos.y = 20;
	m_winner.pic->Action();

	m_winnerDon.pos.x = 0;
	m_winnerDon.pos.y = 20;
	m_winnerDon.pic->Action();

	m_edge.pos.x = -10;
	m_edge.pos.y = 15;
	m_edge.pic->Action();

	m_back.pic->Action();

	// K.O!
	m_font.pos.y = -150.0f;
	m_font.pos.x = -150.0f;
	m_font.pic->Action(8);


	m_pFlontRing.pos.x = -150;
	m_pFlontRing.pos.y = 250;
	m_pFlontRing.pic->Action(4);

	m_pBackRing.pos.x = -50;
	m_pBackRing.pos.y = 300;
	m_pBackRing.pic->Action(4);

	m_pTopLight.pos.x = 0;
	m_pTopLight.pos.y = 0;
	m_pTopLight.pic->Action(4);

	// 初期位置
	m_blackInk.pos.x = 0;
	m_blackInk.pos.y = 0;
	m_blackInk.pic->Action(4);

	m_whiteSoul.pos.x = 0;
	m_whiteSoul.pos.y = 0;
	m_whiteSoul.pic->Action(10);



	// ムービープレイ
	//m_movieMgr->SetTime(0);
	//m_movieMgr->Play();
}



//+-------------------------------
//	(∵)の演出
//+-------------------------------

NazenarabaResultPerformance::NazenarabaResultPerformance()
{
	m_winner.PicClean();
	m_winner.pic = new tdn2DAnim("DATA/Result/nazenaraba/Winner.png");
	m_winner.pic->OrderShake(3, 0, 63, 3);
	m_winner.pic->SetScale(1.1f);

	m_winnerDon.PicClean();
	m_winnerDon.pic = new tdn2DAnim("DATA/Result/nazenaraba/Winner.png");
	m_winnerDon.pic->OrderRipple(6, 1.15f, 0.025f);
	m_winnerDon.pic->SetScale(1.1f);

	m_edge.PicClean();
	m_edge.pic = new tdn2DAnim("DATA/Result/nazenaraba/Edge.png");
	m_edge.pic->OrderShake(3, 0, 63, 3);
	m_edge.pic->SetScale(1.1f);

	m_back.PicClean();
	m_back.pic = new tdn2DAnim("DATA/Result/nazenaraba/back.png");
	m_back.pic->SetScale(1.5f);
	m_back.pic->OrderNone();

	m_back2.PicClean();
	m_back2.pic = new tdn2DAnim("DATA/Result/nazenaraba/back2.png");
	m_back2.pic->SetScale(1.5f);
	m_back2.pic->OrderNone();


	m_font.PicClean();
	m_font.pic = new tdn2DAnim("DATA/Result/nazenaraba/font.png");
	m_font.pos.x = -150;
	m_font.pos.y = 0;
	//m_font.pic->SetScale(2.0f);
	m_font.pic->OrderShrink(8, 1, 3);
	m_iFontAnimFrame = 0;

	//m_fontKill.PicClean();
	//m_fontKill.pic = new tdn2DAnim("DATA/Result/teki/Kill.png");
	//m_fontKill.pos.x = 420;
	//m_fontKill.pos.y = 300;
	//m_fontKill.pic->SetScale(2.0f);
	//m_fontKill.pic->OrderMoveAppeared(12, (int)m_fontKill.pos.x, -200);

	// リング
	m_pFlontRing.PicClean();
	m_pFlontRing.pic = new tdn2DAnim("DATA/Result/teki/FlontRing.png");
	m_pFlontRing.uv.x = 0.0f;
	m_pFlontRing.uv.y = 0.0f;
	m_pFlontRing.pic->OrderMoveAppeared(10, -800, 600);

	m_pBackRing.PicClean();
	m_pBackRing.pic = new tdn2DAnim("DATA/Result/teki/BackRing.png");
	m_pBackRing.uv.x = 0.0f;
	m_pBackRing.uv.y = 0.0f;
	m_pBackRing.pic->OrderMoveAppeared(10, 1280, 600);

	// 上のリングの光
	m_pTopLight.PicClean();
	m_pTopLight.pic = new tdn2DAnim("DATA/Result/teki/TopLight.png");
	m_pTopLight.uv.x = 0.0f;
	m_pTopLight.uv.y = 0.0f;
	m_pTopLight.pic->OrderMoveAppeared(10, -800, 100);


	m_blackInk.PicClean();
	m_blackInk.pic = new tdn2DAnim("DATA/Result/teki/blackInk.png");
	m_blackInk.uv.x = 0.0f;
	m_blackInk.uv.y = 0.0f;
	m_blackInk.pic->OrderMoveAppeared(10, -1000, -100);


	m_whiteSoul.PicClean();
	m_whiteSoul.pic = new tdn2DAnim("DATA/Result/Aramitama/WhiteSplash.png");
	m_whiteSoul.pic->OrderMoveAppeared(8, -650, -500);

	//m_movieMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	//m_movieTex = new tdn2DObj(m_movieMgr->GetTexture());

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		std::string sFont;
		sFont = "DATA/Result/nazenaraba/nort/nort";
		sFont += std::to_string(i);
		sFont += ".png";

		m_pNort[i].PicClean();
		m_pNort[i].pic = new tdn2DAnim(sFont.c_str());
		
		m_pNort[i].pic->OrderJump(18, 1.0f, 0.25f);

	}

	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		std::string sFont;
		sFont = "DATA/Result/nazenaraba/star/star";
		sFont += std::to_string(i);
		sFont += ".png";

		m_pStar[i].PicClean();
		m_pStar[i].pic = new tdn2DAnim(sFont.c_str());

		m_pStar[i].pic->OrderMoveAppeared(12, 1280, 720);

	}

	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		std::string sFont;
		sFont = "DATA/Result/nazenaraba/hart/hart";
		sFont += std::to_string(i);
		sFont += ".png";

		m_pHart[i].PicClean();
		m_pHart[i].pic = new tdn2DAnim(sFont.c_str());

		m_pHart[i].pic->OrderMoveAppeared(12, 1280, 720);

	}

	// 吹き出し
	m_pBalloon.PicClean();
	m_pBalloon.pic = new tdn2DAnim("DATA/Result/nazenaraba/Balloon.png");
	m_pBalloon.pic->OrderMoveAppeared(8, +650, +500);

}

NazenarabaResultPerformance::~NazenarabaResultPerformance()
{
	SAFE_DELETE(m_winner.pic);
	SAFE_DELETE(m_winnerDon.pic);
	SAFE_DELETE(m_edge.pic);
	SAFE_DELETE(m_back.pic);
	SAFE_DELETE(m_back2.pic);
	SAFE_DELETE(m_font.pic);
	//SAFE_DELETE(m_fontKill.pic);
	SAFE_DELETE(m_pFlontRing.pic);
	SAFE_DELETE(m_pBackRing.pic);
	SAFE_DELETE(m_pTopLight.pic);
	SAFE_DELETE(m_whiteSoul.pic);
	SAFE_DELETE(m_blackInk.pic);

	SAFE_DELETE(m_pBalloon.pic);
	//SAFE_DELETE(m_movieMgr);
	//SAFE_DELETE(m_movieTex);

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		SAFE_DELETE(m_pNort[i].pic);
	}
	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		SAFE_DELETE(m_pStar[i].pic);
	}
	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		SAFE_DELETE(m_pHart[i].pic);
	}
}

void NazenarabaResultPerformance::Update()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	//m_back.uv.x -= 0.2f;
	m_back.pic->Update();
	m_back2.pic->Update();

	/*m_soul.pos.x += 0.05f;*/
	m_font.pos.x += 0.1f;
	m_font.pos.y += -0.05f;

	m_blackInk.pos.x += 0.1f;
	m_blackInk.pos.y += -0.05f;

	m_font.pic->Update();


	// 後ろの白魂
	m_whiteSoul.pos.x += 0.05f;
	m_whiteSoul.pic->Update();


	m_blackInk.pic->Update();

	// リング
	m_pBackRing.pos.x += -0.25f;// 左にわー
	m_pBackRing.pos.y += +0.05f;
	m_pBackRing.pic->Update();

	m_pFlontRing.pos.x += 0.25f;// 右にわー
	m_pFlontRing.pos.y += -0.05f;
	m_pFlontRing.pic->Update();

	// 上の光
	m_pTopLight.pic->Update();

	// 手前のプレイヤー
	m_winner.pos.x += 0.2f;
	m_winner.pos.y -= 0.1f;
	m_winner.pic->Update();

	m_winnerDon.pic->Update();

	// 
	m_edge.pic->Update();
	m_edge.pos.x += 0.2f;
	m_edge.pos.y -= 0.1f;

	// 吹き出し
	m_pBalloon.pic->Update();

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		m_pNort[i].pos += m_pNort[i].vMove;
		m_pNort[i].pic->Update();
	}
	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		m_pStar[i].pos += m_pStar[i].vMove;
		m_pStar[i].rotate += 0.01f;
		m_pStar[i].pic->SetAngle(m_pStar[i].rotate);
		m_pStar[i].pic->Update();
	}
	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		m_pHart[i].pos += m_pHart[i].vMove;
		m_pHart[i].pic->Update();
	}

};

void NazenarabaResultPerformance::BackRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	// 真っ白に
	tdnPolygon::Rect(0, 0, 1280, 720,RS::COPY, 0xffffffff);

	// 背景の後ろ
	m_back2.pic->Render(0, 0, m_back2.pic->GetWidth(), m_back2.pic->GetHeight(),
		(int)m_back2.uv.x, (int)m_back2.uv.y, m_back2.pic->GetWidth(), m_back2.pic->GetHeight());

	// ピンク背景
	m_back.pic->Render(0, 0, m_back.pic->GetWidth(), m_back.pic->GetHeight(),
		(int)m_back.uv.x, (int)m_back.uv.y, m_back.pic->GetWidth(), m_back.pic->GetHeight());

	// 吹き出し
	m_pBalloon.pic->Render((int)m_pBalloon.pos.x, (int)m_pBalloon.pos.y);

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		m_pNort[i].pic->Render((int)m_pNort[i].pos.x, (int)m_pNort[i].pos.y);
	}
	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		m_pStar[i].pic->Render((int)m_pStar[i].pos.x, (int)m_pStar[i].pos.y);
	}
	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		m_pHart[i].pic->Render((int)m_pHart[i].pos.x, (int)m_pHart[i].pos.y);
	}

	// 後ろのライト
	//m_pTopLight.pic->Render((int)m_pTopLight.pos.x, (int)m_pTopLight.pos.y);

	// 後ろのリング
	//m_pBackRing.pic->Render((int)m_pBackRing.pos.x, (int)m_pBackRing.pos.y);

	// 墨
	//m_blackInk.pic->RenderSpecial((int)m_blackInk.pos.x, (int)m_blackInk.pos.y);
	
	// タイトル
	m_font.pic->Render((int)m_font.pos.x, (int)m_font.pos.y);

};

void NazenarabaResultPerformance::FrontRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

								  // 手前のリング
	//m_pFlontRing.pic->Render((int)m_pFlontRing.pos.x, (int)m_pFlontRing.pos.y);


	m_edge.pic->Render((int)m_edge.pos.x, (int)m_edge.pos.y);
	m_winner.pic->Render((int)m_winner.pos.x, (int)m_winner.pos.y);
	m_winnerDon.pic->Render((int)m_winnerDon.pos.x, (int)m_winnerDon.pos.y, RS::ADD);

	

	//m_fontKill.pic->Render((int)m_fontKill.pos.x, (int)m_fontKill.pos.y);

};

void NazenarabaResultPerformance::Action()
{
	m_bAction = true;

	// 初期化＆始動
	m_winner.pos.x = 0;
	m_winner.pos.y = 0;
	m_winner.pic->Action();

	m_winnerDon.pos.x = 0;
	m_winnerDon.pos.y = 0;
	m_winnerDon.pic->Action();

	m_edge.pos.x = -10;
	m_edge.pos.y = -5;
	m_edge.pic->Action();

	m_back.pic->Action();
	m_back2.pic->Action();


	// GoodNight!
	m_font.pos.y = 0.0f;
	m_font.pos.x = 0.0f;
	m_font.pic->Action(8);


	m_pFlontRing.pos.x = -50;
	m_pFlontRing.pos.y = 300;
	m_pFlontRing.pic->Action(4);

	m_pBackRing.pos.x = -50;
	m_pBackRing.pos.y = 300;
	m_pBackRing.pic->Action(4);

	m_pTopLight.pos.x = 0;
	m_pTopLight.pos.y = 0;
	m_pTopLight.pic->Action(4);

	// 初期位置
	m_blackInk.pos.x = 0;
	m_blackInk.pos.y = 0;
	m_blackInk.pic->Action(4);

	m_whiteSoul.pos.x = 0;
	m_whiteSoul.pos.y = 0;
	m_whiteSoul.pic->Action(10);

	m_pBalloon.pos.x = -100;
	m_pBalloon.pos.y = -100;
	m_pBalloon.pic->Action();

	// ノーツを個別に設定
	m_pNort[0].pos.x = 0;
	m_pNort[0].pos.y = 0;
	m_pNort[0].pic->Action();

	m_pNort[1].pos.x = 200;
	m_pNort[1].pos.y = 300;
	m_pNort[1].pic->Action();

	m_pNort[2].pos.x = 800;
	m_pNort[2].pos.y = 100;
	m_pNort[2].pic->Action();


	Vector2 vS;
	Vector2 vMoveVec = VECTOR2_ZERO;
	vS.x = 1280;
	vS.y = 720;
	

	// スターを個別に設定
	m_pStar[0].pos.x = 300;
	m_pStar[0].pos.y = 0;
	m_pStar[0].pic->Action(2);	

	m_pStar[1].pos.x = 140;
	m_pStar[1].pos.y = 500;
	m_pStar[1].pic->Action(4);
	   
	m_pStar[2].pos.x = 848;
	m_pStar[2].pos.y = 64;
	m_pStar[2].pic->Action(6);

	m_pStar[3].pos.x = 641;
	m_pStar[3].pos.y = 448;
	m_pStar[3].pic->Action(8);

	for (int i = 0; i < STAR_MAX; i++)
	{
		vMoveVec = m_pStar[i].pos - vS;
		vMoveVec.Normalize();
		m_pStar[i].vMove = vMoveVec*0.25f;
	}

	// ハートを個別に設定
	m_pHart[0].pos.x = 1152;
	m_pHart[0].pos.y = 0;
	m_pHart[0].pic->Action(2);
	   
	m_pHart[1].pos.x = -32;
	m_pHart[1].pos.y = 224;
	m_pHart[1].pic->Action(4);
	   
	m_pHart[2].pos.x = 496;
	m_pHart[2].pos.y = 448;
	m_pHart[2].pic->Action(6);

	m_pHart[3].pos.x = 528;
	m_pHart[3].pos.y = 224;
	m_pHart[3].pic->Action(8);

	for (int i = 0; i < HART_MAX; i++)
	{
		vMoveVec = m_pHart[i].pos - vS;
		vMoveVec.Normalize();
		m_pHart[i].vMove = vMoveVec*0.25f;
	}

	// ムービープレイ
	//m_movieMgr->SetTime(0);
	//m_movieMgr->Play();
}


//+-------------------------------
//	兄貴の演出
//+-------------------------------

AnikiResultPerformance::AnikiResultPerformance()
{
	m_winner.PicClean();
	m_winner.pic = new tdn2DAnim("DATA/Result/Aniki/Winner.png");
	m_winner.pic->OrderShake(3, 0, 63, 3);

	m_winnerDon.PicClean();
	m_winnerDon.pic = new tdn2DAnim("DATA/Result/Aniki/Winner.png");
	m_winnerDon.pic->OrderRipple(6, 1.15f, 0.025f);

	m_edge.PicClean();
	m_edge.pic = new tdn2DAnim("DATA/Result/Aniki/Edge.png");
	m_edge.pic->OrderShake(3, 0, 63, 3);

	m_back.PicClean();
	m_back.pic = new tdn2DAnim("DATA/Result/Aniki/back.png");
	m_back.pic->OrderNone();

	m_cloud.PicClean();
	m_cloud.pic = new tdn2DAnim("DATA/Result/Aniki/cloud.png");
	m_cloud.pic->OrderMoveAppeared(12, -300, 0);


	m_font.PicClean();
	m_font.pic = new tdn2DAnim("DATA/Result/Aniki/Reputation.png");
	//m_font.pic->SetScale(2.0f);
	m_font.pic->OrderMoveAppeared(12, -400, 100);
	m_iFontAnimFrame = 0;

	//m_fontKill.PicClean();
	//m_fontKill.pic = new tdn2DAnim("DATA/Result/teki/Kill.png");
	//m_fontKill.pos.x = 420;
	//m_fontKill.pos.y = 300;
	//m_fontKill.pic->SetScale(2.0f);
	//m_fontKill.pic->OrderMoveAppeared(12, (int)m_fontKill.pos.x, -200);

	// リング
	m_pFlontRing.PicClean();
	m_pFlontRing.pic = new tdn2DAnim("DATA/Result/teki/FlontRing.png");
	m_pFlontRing.uv.x = 0.0f;
	m_pFlontRing.uv.y = 0.0f;
	m_pFlontRing.pic->OrderMoveAppeared(10, -800, 600);

	m_pBackRing.PicClean();
	m_pBackRing.pic = new tdn2DAnim("DATA/Result/teki/BackRing.png");
	m_pBackRing.uv.x = 0.0f;
	m_pBackRing.uv.y = 0.0f;
	m_pBackRing.pic->OrderMoveAppeared(10, 1280, 600);

	// 薔薇
	m_petal.PicClean();
	m_petal.pic = new tdn2DAnim("DATA/Result/Aniki/petal.png");
	m_petal.uv.x = 0.0f;
	m_petal.uv.y = 0.0f;
	m_petal.pic->OrderMoveAppeared(8, -800, 100);


	m_blackInk.PicClean();
	m_blackInk.pic = new tdn2DAnim("DATA/Result/teki/blackInk.png");
	m_blackInk.uv.x = 0.0f;
	m_blackInk.uv.y = 0.0f;
	m_blackInk.pic->OrderMoveAppeared(10, -1000, -100);


	m_whiteSoul.PicClean();
	m_whiteSoul.pic = new tdn2DAnim("DATA/Result/Aramitama/WhiteSplash.png");
	m_whiteSoul.pic->OrderMoveAppeared(8, -650, -500);

	//m_movieMgr = new tdnMovie("DATA/Result/Aramitama/backmovie.wmv");
	//m_movieTex = new tdn2DObj(m_movieMgr->GetTexture());

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		std::string sFont;
		sFont = "DATA/Result/nazenaraba/nort/nort";
		sFont += std::to_string(i);
		sFont += ".png";

		m_pNort[i].PicClean();
		m_pNort[i].pic = new tdn2DAnim(sFont.c_str());

		m_pNort[i].pic->OrderJump(18, 1.0f, 0.25f);

	}

	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		std::string sFont;
		sFont = "DATA/Result/nazenaraba/star/star";
		sFont += std::to_string(i);
		sFont += ".png";

		m_pStar[i].PicClean();
		m_pStar[i].pic = new tdn2DAnim(sFont.c_str());

		m_pStar[i].pic->OrderMoveAppeared(12, 1280, 720);

	}

	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		std::string sFont;
		sFont = "DATA/Result/nazenaraba/hart/hart";
		sFont += std::to_string(i);
		sFont += ".png";

		m_pHart[i].PicClean();
		m_pHart[i].pic = new tdn2DAnim(sFont.c_str());

		m_pHart[i].pic->OrderMoveAppeared(12, 1280, 720);

	}

	// 吹き出し
	m_pRose.PicClean();
	m_pRose.pic = new tdn2DAnim("DATA/Result/Aniki/Rose.png");
	m_pRose.pic->OrderShrink(12, 1, 3);

}

AnikiResultPerformance::~AnikiResultPerformance()
{
	SAFE_DELETE(m_winner.pic);
	SAFE_DELETE(m_winnerDon.pic);
	SAFE_DELETE(m_edge.pic);
	SAFE_DELETE(m_back.pic);
	SAFE_DELETE(m_cloud.pic);
	SAFE_DELETE(m_font.pic);
	//SAFE_DELETE(m_fontKill.pic);
	SAFE_DELETE(m_pFlontRing.pic);
	SAFE_DELETE(m_pBackRing.pic);
	SAFE_DELETE(m_petal.pic);
	SAFE_DELETE(m_whiteSoul.pic);
	SAFE_DELETE(m_blackInk.pic);

	SAFE_DELETE(m_pRose.pic);
	//SAFE_DELETE(m_movieMgr);
	//SAFE_DELETE(m_movieTex);

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		SAFE_DELETE(m_pNort[i].pic);
	}
	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		SAFE_DELETE(m_pStar[i].pic);
	}
	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		SAFE_DELETE(m_pHart[i].pic);
	}
}

void AnikiResultPerformance::Update()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	m_back.pic->Update();

	m_cloud.pos.x += 1.0f;
	m_cloud.pos.y = 0.0f;
	m_cloud.pic->Update();

	/*m_soul.pos.x += 0.05f;*/
	//m_font.pos.x += 0.25f;
	//m_font.pos.y += -0.125f;

	m_blackInk.pos.x += 0.025f;
	m_blackInk.pos.y += -0.05f;

	m_font.pos.x += 0.1f;
	m_font.pos.y += -0.05f;
	m_font.pic->Update();


	// 後ろの白魂
	m_whiteSoul.pos.x += 0.05f;
	m_whiteSoul.pic->Update();


	m_blackInk.pic->Update();

	// リング
	m_pBackRing.pos.x += -0.25f;// 左にわー
	m_pBackRing.pos.y += +0.05f;
	m_pBackRing.pic->Update();

	m_pFlontRing.pos.x += 0.25f;// 右にわー
	m_pFlontRing.pos.y += -0.05f;
	m_pFlontRing.pic->Update();

	// 薔薇
	m_petal.pos.x += 0.25f;
	m_petal.pic->Update();

	// 手前のプレイヤー
	m_winner.pos.x += 0.2f;
	m_winner.pos.y -= 0.1f;
	m_winner.pic->Update();

	m_winnerDon.pic->Update();

	// 
	m_edge.pic->Update();
	m_edge.pos.x += 0.2f;
	m_edge.pos.y -= 0.1f;

	// 吹き出し
	m_pRose.pic->Update();

	// ノーツ
	for (int i = 0; i < NORT_MAX; i++)
	{
		m_pNort[i].pos += m_pNort[i].vMove;
		m_pNort[i].pic->Update();
	}
	// スター
	for (int i = 0; i < STAR_MAX; i++)
	{
		m_pStar[i].pos += m_pStar[i].vMove;
		m_pStar[i].rotate += 0.01f;
		m_pStar[i].pic->SetAngle(m_pStar[i].rotate);
		m_pStar[i].pic->Update();
	}
	// ハート
	for (int i = 0; i < HART_MAX; i++)
	{
		m_pHart[i].pos += m_pHart[i].vMove;
		m_pHart[i].pic->Update();
	}

};

void AnikiResultPerformance::BackRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

								  // 真っ白に
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xffffffff);

	// 背景
	m_back.pic->Render(0, 0, m_back.pic->GetWidth(), m_back.pic->GetHeight(),
		(int)m_back.uv.x, (int)m_back.uv.y, m_back.pic->GetWidth(), m_back.pic->GetHeight());

	// 雲
	m_cloud.pic->Render(0, 0, m_cloud.pic->GetWidth(), m_cloud.pic->GetHeight(),
		(int)m_cloud.uv.x, (int)m_cloud.uv.y, m_cloud.pic->GetWidth(), m_cloud.pic->GetHeight());


	// 歪みネェナ
	m_font.pic->Render((int)m_font.pos.x, (int)m_font.pos.y);


	// バラ
	m_pRose.pic->Render((int)m_pRose.pos.x, (int)m_pRose.pos.y);

	//// ノーツ
	//for (int i = 0; i < NORT_MAX; i++)
	//{
	//	m_pNort[i].pic->Render((int)m_pNort[i].pos.x, (int)m_pNort[i].pos.y);
	//}
	//// スター
	//for (int i = 0; i < STAR_MAX; i++)
	//{
	//	m_pStar[i].pic->Render((int)m_pStar[i].pos.x, (int)m_pStar[i].pos.y);
	//}
	//// ハート
	//for (int i = 0; i < HART_MAX; i++)
	//{
	//	m_pHart[i].pic->Render((int)m_pHart[i].pos.x, (int)m_pHart[i].pos.y);
	//}


	// 後ろのリング
	//m_pBackRing.pic->Render((int)m_pBackRing.pos.x, (int)m_pBackRing.pos.y);

	// 墨
	//m_blackInk.pic->RenderSpecial((int)m_blackInk.pos.x, (int)m_blackInk.pos.y);


};

void AnikiResultPerformance::FrontRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

								  // 手前のリング
								  //m_pFlontRing.pic->Render((int)m_pFlontRing.pos.x, (int)m_pFlontRing.pos.y);


	m_edge.pic->Render((int)m_edge.pos.x, (int)m_edge.pos.y);
	m_winner.pic->Render((int)m_winner.pos.x, (int)m_winner.pos.y);
	m_winnerDon.pic->Render((int)m_winnerDon.pos.x, (int)m_winnerDon.pos.y, RS::ADD);


	// 薔薇
	m_petal.pic->Render((int)m_petal.pos.x, (int)m_petal.pos.y);

	//m_fontKill.pic->Render((int)m_fontKill.pos.x, (int)m_fontKill.pos.y);

};

void AnikiResultPerformance::Action()
{
	m_bAction = true;

	// 初期化＆始動
	m_winner.pos.x = 495;
	m_winner.pos.y = -81;
	m_winner.pic->Action();

	m_winnerDon.pos.x = 495;
	m_winnerDon.pos.y = -81;
	m_winnerDon.pic->Action();

	m_edge.pos.x = 15+ 495;
	m_edge.pos.y = -6 - 81;
	m_edge.pic->Action();

	m_back.pic->Action();
	m_cloud.pic->Action();
	m_cloud.pos.x = 0.0f;
	m_cloud.pos.y = 0.0f;

	// 文字!
	m_font.pos.y = 0.0f;
	m_font.pos.x = 0.0f;
	m_font.pic->Action(8);


	m_pFlontRing.pos.x = -50;
	m_pFlontRing.pos.y = 300;
	m_pFlontRing.pic->Action(4);

	m_pBackRing.pos.x = -50;
	m_pBackRing.pos.y = 300;
	m_pBackRing.pic->Action(4);

	m_petal.pos.x = 0;
	m_petal.pos.y = 0;
	m_petal.pic->Action(4);

	// 初期位置
	m_blackInk.pos.x = 0;
	m_blackInk.pos.y = 0;
	m_blackInk.pic->Action(4);

	m_whiteSoul.pos.x = 0;
	m_whiteSoul.pos.y = 0;
	m_whiteSoul.pic->Action(10);

	m_pRose.pic->Action();

	// ノーツを個別に設定
	m_pNort[0].pos.x = 0;
	m_pNort[0].pos.y = 0;
	m_pNort[0].pic->Action();

	m_pNort[1].pos.x = 200;
	m_pNort[1].pos.y = 300;
	m_pNort[1].pic->Action();

	m_pNort[2].pos.x = 800;
	m_pNort[2].pos.y = 100;
	m_pNort[2].pic->Action();


	Vector2 vS;
	Vector2 vMoveVec = VECTOR2_ZERO;
	vS.x = 1280;
	vS.y = 720;


	// スターを個別に設定
	m_pStar[0].pos.x = 300;
	m_pStar[0].pos.y = 0;
	m_pStar[0].pic->Action(2);

	m_pStar[1].pos.x = 140;
	m_pStar[1].pos.y = 500;
	m_pStar[1].pic->Action(4);

	m_pStar[2].pos.x = 848;
	m_pStar[2].pos.y = 64;
	m_pStar[2].pic->Action(6);

	m_pStar[3].pos.x = 641;
	m_pStar[3].pos.y = 448;
	m_pStar[3].pic->Action(8);

	for (int i = 0; i < STAR_MAX; i++)
	{
		vMoveVec = m_pStar[i].pos - vS;
		vMoveVec.Normalize();
		m_pStar[i].vMove = vMoveVec*0.25f;
	}

	// ハートを個別に設定
	m_pHart[0].pos.x = 1152;
	m_pHart[0].pos.y = 0;
	m_pHart[0].pic->Action(2);

	m_pHart[1].pos.x = -32;
	m_pHart[1].pos.y = 224;
	m_pHart[1].pic->Action(4);

	m_pHart[2].pos.x = 496;
	m_pHart[2].pos.y = 448;
	m_pHart[2].pic->Action(6);

	m_pHart[3].pos.x = 528;
	m_pHart[3].pos.y = 224;
	m_pHart[3].pic->Action(8);

	for (int i = 0; i < HART_MAX; i++)
	{
		vMoveVec = m_pHart[i].pos - vS;
		vMoveVec.Normalize();
		m_pHart[i].vMove = vMoveVec*0.25f;
	}

	// ムービープレイ
	//m_movieMgr->SetTime(0);
	//m_movieMgr->Play();
}




//+-------------------------------
//	バランスの演出
//+-------------------------------

BalanceResultPerformance::BalanceResultPerformance()
{
	m_winner.PicClean();
	m_winner.pic = new tdn2DAnim("DATA/Result/Balance/Winner.png");
	m_winner.pic->OrderShake(3, 0, 63, 3);

	m_winnerDon.PicClean();
	m_winnerDon.pic = new tdn2DAnim("DATA/Result/Balance/Winner.png");
	m_winnerDon.pic->OrderRipple(6, 1.15f, 0.025f);

	m_edge.PicClean();
	m_edge.pic = new tdn2DAnim("DATA/Result/Balance/Edge.png");
	m_edge.pic->OrderShake(3, 0, 63, 3);

	m_back.PicClean();
	m_back.pic = new tdn2DAnim("DATA/Result/Balance/back.png");
	m_back.pic->OrderNone();


	m_font.PicClean();
	m_font.pic = new tdn2DAnim("DATA/Result/Balance/font.png");
	//m_font.pic->SetScale(2.0f);
	m_font.pic->OrderMoveAppeared(12, -400, 100);
	//m_font.pic->OrderShrink(12, 1, 3);
	m_iFontAnimFrame = 0;

	//m_fontKill.PicClean();
	//m_fontKill.pic = new tdn2DAnim("DATA/Result/teki/Kill.png");
	//m_fontKill.pos.x = 420;
	//m_fontKill.pos.y = 300;
	//m_fontKill.pic->SetScale(2.0f);
	//m_fontKill.pic->OrderMoveAppeared(12, (int)m_fontKill.pos.x, -200);

	
	// 薔薇
	m_petal.PicClean();
	m_petal.pic = new tdn2DAnim("DATA/Result/Balance/font.png");
	m_petal.uv.x = 0.0f;
	m_petal.uv.y = 0.0f;
	m_petal.pic->OrderMoveAppeared(8, -800, 100);


	m_pRose.PicClean();
	m_pRose.pic = new tdn2DAnim("DATA/Result/Balance/flame.png");
	m_pRose.pic->OrderShrink(12, 1, 3);

	m_pNinjin.PicClean();
	m_pNinjin.pic = new tdn2DAnim("DATA/Result/Balance/Ninjin.png");
	m_pNinjin.pic->OrderShrink(12, 1, 3);

	m_pSaku.PicClean();
	m_pSaku.pic = new tdn2DAnim("DATA/Result/Balance/Saku.png");
	m_pSaku.pic->OrderMoveAppeared(12, 0, 720);

}

BalanceResultPerformance::~BalanceResultPerformance()
{
	SAFE_DELETE(m_winner.pic);
	SAFE_DELETE(m_winnerDon.pic);
	SAFE_DELETE(m_edge.pic);
	SAFE_DELETE(m_back.pic);
	SAFE_DELETE(m_font.pic);
	//SAFE_DELETE(m_fontKill.pic);
	SAFE_DELETE(m_petal.pic);
	SAFE_DELETE(m_pRose.pic);
	SAFE_DELETE(m_pNinjin.pic);
	SAFE_DELETE(m_pSaku.pic);

	//SAFE_DELETE(m_movieMgr);
	//SAFE_DELETE(m_movieTex);

}

void BalanceResultPerformance::Update()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

	m_back.pic->Update();

	m_font.pos.x += 0.1f;
	m_font.pos.y += -0.05f;
	m_font.pic->Update();

	// 薔薇
	m_petal.pos.x += 0.25f;
	m_petal.pic->Update();

	// 手前のプレイヤー
	m_winner.pos.x += 0.25f;
	//m_winner.pos.y -= 0.1f;
	m_winner.pic->Update();

	m_winnerDon.pic->Update();

	// 
	m_edge.pic->Update();
	m_edge.pos.x += 0.25f;
	//m_edge.pos.y -= 0.1f;

	// 吹き出し
	m_pRose.pic->Update();

	m_pNinjin.pic->Update();
	m_pSaku.pic->Update();

};

void BalanceResultPerformance::BackRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

								  // 真っ白に
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xffffffff);

	// 背景
	m_back.pic->Render(0, 0, m_back.pic->GetWidth(), m_back.pic->GetHeight(),
		(int)m_back.uv.x, (int)m_back.uv.y, m_back.pic->GetWidth(), m_back.pic->GetHeight());

	// 歪みネェナ
	m_font.pic->Render((int)m_font.pos.x, (int)m_font.pos.y);

	// バラ
	m_pRose.pic->Render((int)m_pRose.pos.x, (int)m_pRose.pos.y);


};

void BalanceResultPerformance::FrontRender()
{
	if (m_bAction == false)return;// アクションフラグが立つまでハジク

								  // 手前のリング
								  //m_pFlontRing.pic->Render((int)m_pFlontRing.pos.x, (int)m_pFlontRing.pos.y);


	m_edge.pic->Render((int)m_edge.pos.x, (int)m_edge.pos.y);
	m_winner.pic->Render((int)m_winner.pos.x, (int)m_winner.pos.y);
	m_winnerDon.pic->Render((int)m_winnerDon.pos.x, (int)m_winnerDon.pos.y, RS::ADD);

	//m_pNinjin.pic->Render((int)m_pNinjin.pos.x, (int)m_pNinjin.pos.y);

	m_pSaku.pic->Render((int)m_pSaku.pos.x, (int)m_pSaku.pos.y);
	
	// 薔薇
	//m_petal.pic->Render((int)m_petal.pos.x, (int)m_petal.pos.y);

	//m_fontKill.pic->Render((int)m_fontKill.pos.x, (int)m_fontKill.pos.y);

};

void BalanceResultPerformance::Action()
{
	m_bAction = true;

	// 初期化＆始動
	m_winner.pos.x = 495;
	m_winner.pos.y = -81;
	m_winner.pic->Action();

	m_winnerDon.pos.x = 495;
	m_winnerDon.pos.y = -81;
	m_winnerDon.pic->Action();

	m_edge.pos.x = -25 + 495;
	m_edge.pos.y = -16 - 81;
	m_edge.pic->Action();

	m_back.pic->Action();

	// 文字!
	m_font.pos.y = 0.0f;
	m_font.pos.x = 0.0f;
	m_font.pic->Action(8);



	m_petal.pos.x = 0;
	m_petal.pos.y = 0;
	m_petal.pic->Action(4);

	m_pRose.pic->Action();

	m_pNinjin.pos.x = 1280 - 252;
	m_pNinjin.pos.y = 720 - 380;
	m_pNinjin.pic->Action();

	m_pSaku.pos.x = 0;
	m_pSaku.pos.y = 720 - 144;
	m_pSaku.pic->Action(4);

}