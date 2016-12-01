#include "Timer.h"
#include "Data\SelectData.h"

////+-----------------------
////	タイマー
////+-----------------------
//
//TimeManager::TimeManager()
//{
//	m_frame = 0;
//
//}
//
//void  TimeManager::Init()
//{
//	m_vPos.x = 574;
//	m_vPos.y = 58;
//
//	m_pic = new tdn2DAnim("Data/UI/Game/timer.png");
//	m_pic->OrderShrink(12, 1, 1.5);
//	m_pic->Action();
//};
//
//// 更新(タイム減算処理&終了判定を返す)
//bool TimeManager::Update()
//{
//	m_pic->Update();
//
//	bool bEnd(--m_frame <= 0);
//	return bEnd;
//
//}
//
//// 描画
//void TimeManager::Render()
//{
//	const int second = m_frame / 60,
//		minutes = m_frame / 60;
//
//	m_pic->Render((int)m_vPos.x, (int)m_vPos.y, 64, 64, (second / 10) * 64, 0, 64, 64);			// 分
//	m_pic->Render((int)m_vPos.x + 64, (int)m_vPos.y, 64, 64, (second % 10) * 64, 0, 64, 64);	// 
//	
//
//}


//+-------------------------------------------
//	newタイマー
//+-------------------------------------------

Timer::Timer()
{
	m_iFrame = 0;
	
	m_pPic = new tdn2DAnim("Data/UI/Game/timer.png");
	m_pPic->OrderShrink(12, 1, 2.5);

	m_vPos = Vector2(574, 58);
	m_bStop = true;

	m_iElapsedTime = 0;
	m_iElapsedFrame = 0;
	m_iRoundElapsedTime = 0;

} 

Timer::~Timer()
{
	SAFE_DELETE(m_pPic);
}

//+-------------------------------------------
//	更新・描画
//+-------------------------------------------
void Timer::Update()
{
	// 止めるフラグが立っていなかったら
	if (m_bStop == false) 
	{
		m_iFrame--;
		if (m_iFrame <= 0)m_iFrame = 0;

		// 経過フレーム
		m_iElapsedFrame++;
		// 現在の経過時間
		m_iRoundElapsedTime = m_iElapsedFrame / 60;

		// 過去最低のタイムを保存
		if (m_iElapsedTime < m_iRoundElapsedTime)
		{
			m_iElapsedTime = m_iRoundElapsedTime;
		}

	}
	
	// アニメーション
	m_pPic->Update();
}

void Timer::Render()
{
	const int second = m_iFrame / 60;

	if (m_iFrame <= 15 * 60)
	{
		m_pPic->SetARGB(256,256, 0, 0);
	}

	m_pPic->Render((int)m_vPos.x, (int)m_vPos.y, 64, 64, (second / 10) * 64, 0, 64, 64);			// 秒
	m_pPic->Render((int)m_vPos.x + 64, (int)m_vPos.y, 64, 64, (second % 10) * 64, 0, 64, 64);	// 

}

//+-------------------------------------------
//	開幕演出 
//+-------------------------------------------
void Timer::Action(int delayFrame)
{
	Reset(); // タイマー初期化
	m_pPic->Action(delayFrame);
}

//+-------------------------------------------
//	時間を動かす・止める
//+-------------------------------------------
void Timer::Start()
{
	m_bStop = false;
}

void Timer::Stop()
{
	m_bStop = true;
}

void  Timer::Reset()
{
	/* (TODO) 残り時間はデータから取得 */
	m_iFrame = SelectDataMgr->Get()->iRoundTime * 60;

	//m_iElapsedTime = 0;
	m_iElapsedFrame = 0;
	m_iRoundElapsedTime = 0;

}

