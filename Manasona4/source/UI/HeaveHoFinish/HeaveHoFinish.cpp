#include "HeaveHoFinish.h"

//+-----------------------------
//	ヒーホーフィニッシュの演出
//+-----------------------------

// 実体
HeaveHoFinish* HeaveHoFinish::m_pInstance = nullptr;


HeaveHoFinish::HeaveHoFinish()
{
	m_pHeaveho = new tdn2DAnim("Data/UI/Game/HeaveHo/HeaveHo2.png");
	m_pHeaveho->OrderShrink(8, 1, 6);
	//m_pHeaveho->OrderShrink(12, 1, 3);


	m_pHeavehoShink = new tdn2DAnim("Data/UI/Game/HeaveHo/HeaveHo2.png");
	//m_pHeavehoShink->OrderShrink(12, 1, 3);


	m_pBlueRing = new tdn2DAnim("Data/UI/Game/HeaveHo/BlueRing.png");
	m_pBlueRing->OrderRipple(24, 1, 0.1f);

	m_pBlueRingNext = new tdn2DAnim("Data/UI/Game/HeaveHo/BlueRing.png");
	
	m_pLight = new tdn2DAnim("Data/UI/Game/HeaveHo/light.png");
	m_pLight->OrderRipple(24, 1, 0.2f);

	m_pStarDust = new tdn2DAnim("Data/UI/Game/HeaveHo/StarDust.png");
	m_pStarDust->OrderRipple(8, 1, 0.03f);
	m_pStarDustPos.x = m_pStarDustPos.y = 0.0f;

	// アクションフラグ
	m_bActionFlag = false;
	m_bRippleFlag = false;

	// フレーム
	m_iFrame = 0;

	m_fPicAngle = 0.0f;
}

HeaveHoFinish::~HeaveHoFinish()
{
	SAFE_DELETE(m_pHeaveho);
	SAFE_DELETE(m_pHeavehoShink);
	SAFE_DELETE(m_pBlueRing);
	SAFE_DELETE(m_pBlueRingNext);
	SAFE_DELETE(m_pLight);
	SAFE_DELETE(m_pStarDust);

}

void HeaveHoFinish::Update()
{
	if (m_bActionFlag == false)return;


	if (m_pHeavehoShink->GetAction()->IsEnd() == true &&
		m_bRippleFlag == false)
	{
		// 止めてリップルに
		//m_pHeavehoShink->Stop();
		m_pHeavehoShink->OrderRipple(8, 1, 0.02f);
		m_pHeavehoShink->Action();
		m_bRippleFlag = true;
	}

	// 引きがね
	Trigger();

	m_pHeaveho->SetAngle(m_fPicAngle);
	m_pHeaveho->Update();
	m_pHeavehoShink->Update();
	m_pBlueRing->Update();
	//m_pBlueRingNext->Update();
	m_pLight->Update();
	m_pStarDust->Update();

	
	m_iFrame++;
	if (m_iFrame >= 150)
	{
		m_iFrame = 0;
		m_bActionFlag = false;
	}
}

void HeaveHoFinish::Render()
{
	if (m_bActionFlag == false)return;
	
		//m_pBlueRingNext->Render(0, 0, RS::ADD);

	m_pStarDust->Render((int)m_pStarDustPos.x, (int)m_pStarDustPos.y, RS::ADD);
		m_pBlueRing->Render(0, 0, RS::ADD);
		m_pHeaveho->Render(0, 0);
		m_pHeavehoShink->Render(0, 0, RS::ADD);
		m_pHeavehoShink->Render(0, 0, RS::ADD);
		m_pLight->Render(0, 0, RS::ADD);

}

void HeaveHoFinish::Action()
{
	m_pHeaveho->OrderShrink(8, 1, 6);
	m_pHeaveho->Action();

	m_pHeavehoShink->OrderShrink(8, 1, 6);
	m_pHeavehoShink->Action(3);

	m_pBlueRing->Action(8);


	m_pLight->Action(12);

	//m_pBlueRingNext->OrderRipple(24, 1, 0.1f);
	//m_pBlueRingNext->Action(12);

	m_pStarDustPos.x = m_pStarDustPos.y = 0.0f;

	m_pStarDust->Action(20);



	// 初期化
	m_bActionFlag = true;
	m_bRippleFlag = false;
	m_iFrame = 0;
	m_fPicAngle = 0.0f;
}

void HeaveHoFinish::Trigger()
{

	if (m_iFrame == 135)
	{
		m_pHeaveho->OrderRipple(14, 1, 0.1f);
		m_pHeaveho->Action();
	}

	/***********************/
	//	動き
	/***********************/
	//if (m_iFrame >= 60)
	//{
		//m_pStarDustPos.x += 10.1f;
		//m_pStarDustPos.y -= 10.1f;
	//}

	if (m_iFrame >= 135)
	{
		m_fPicAngle += 0.3f;
	}
}