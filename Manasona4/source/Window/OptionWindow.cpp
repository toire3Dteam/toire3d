#include "OptionWindow.h"

//+------------------------
// オプションウィンドウ
//+------------------------

OptionWindow::OptionWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/OptionWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/OptionWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");
					
	//選択アイコンの長さ
	m_iSelectLength = 30;

	// オプションウィンドウに存在するアイコンを詰めていく
	AddIconData("システム設定", "システム設定を行います。");
	AddIconData("ゲーム設定", "ゲーム内の各種設定変更を行います。");
	AddIconData("サウンド設定", "各種サウンドの音量の設定変更を行います。");
	AddIconData("コントローラ設定", "コントローラの設定変更を行います。");
	AddIconData("戻る", "このウィンドウを閉じます。");

	// 演出
	//m_iAlpha = 0;

}

OptionWindow::~OptionWindow()
{

}


bool OptionWindow::Update()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 更新だけほかの子のWindowが起動していたら
	// 操作できないようにする。

	m_pSelect->Update();

	// 透明度更新
	m_iAlpha += 255 / 6;
	m_iAlpha = min(255, m_iAlpha);

	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新

	//// パッド分更新
	//const int NumDevice = (tdnInputManager::GetNumDevice());

	//// パッド何もささってないとき用
	//if (NumDevice == 0)
	//{
	//	if (Ctrl(0))
	//	{
	//		// 決定を押したらここに入る
	//		
	//		return true;
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < NumDevice; i++)
	//	{
	//		if (Ctrl(i))
	//		{
	//			// 決定を押したらここに入る

	//			return true;
	//		}
	//	}
	//}


	return true;// 起動中
}

void OptionWindow::Redner()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return;

	// タイトル
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ★基本的なウィンドウの位置
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// 中身をブロック単位で描画
	int y = (int)(m_vPos.y + 64.0f) + m_iAbjustHeight;
	int x = (int)m_vPos.x + 18 + (255 - m_iAlpha);
	for (int i = 0; i < (int)m_aIconData.size(); i++)
	{

		// 最後尾ならしっぽを描画
		if (i == (int)m_aIconData.size() - 1)
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 46, 0, 64+40, SizeX, 46);


		}
		else// 中
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 40, 0, 64, SizeX, 40);
		}



		//+-----------------------
		// 選択している番号なら
		if (i == m_iSelectNo)
		{
			// (TUDO) 選択している所を目立たせる演出
			
			//+-----------------------
			// 選択してる黄色い長い枠
			for (int j = 0; j < m_iSelectLength; j++)
			{
				m_pSelect->Render(x + (j * 11) - 4, y, 32, 32, 0, 32, 32, 32);
			}

		}

		//+-----------------------
		// 枠
		for (int j = 0; j < m_aIconData[i].iStringLength; j++)
		{
			m_pFontBox->Render(x + (j * 11) - 4, y, 32, 32, 0, 0, 32, 32);

			// 選択しているタスクなら
			if (m_iSelectNo == i)
			{
				m_pFontBox->Render(x + (j * 11) - 4, y,
					32, 32, 0, 32, 32, 32);
			}

		}

		// 文字
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);


		// 40ピクセルずらして描画していく
		y += 40 + m_iAbjustHeight;
	}

	//// 文字
	//DWORD fontCol = 0xffffffff;
	////if (m_iSelectNo == i)fontCol = 0xff030a58;
	//tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS創英角ｺﾞｼｯｸUB",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
	//	24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
}

// 操作
bool  OptionWindow::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	CtrlSE(DeviceID);// 操作音

	// 選択切り替え
	if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
	{
		m_iSelectNo--;
		// 選択時の演出
		m_pSelect->Action();
		// 最後へ進む
		if (m_iSelectNo <= -1)
		{
			m_iSelectNo = ((int)m_aIconData.size() - 1);
		}
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
	{
		m_iSelectNo++;
		// 選択時の演出
		m_pSelect->Action();
		// 最初に戻る
		if (m_iSelectNo >= (int)m_aIconData.size())
		{
			m_iSelectNo = 0;
		}
	}

	// 決定
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// 選択した番号格納！
		m_iChoiceState = (OPTION_STATE)m_iSelectNo;
		return true;
	}

	// キャンセルで
	// ゲームに戻るステートを格納
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		// 選択した番号格納！
		m_iChoiceState = (OPTION_STATE::BACK);
		return true;
	}

	return false;

}

void OptionWindow::Action()
{
	if (m_bActive == false)// もうウィンドウが出ているときは演習をハジク
	{
		// 透明度を最初は透明に
		m_iAlpha = 0;
		m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
		m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
		m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新
	}

	BaseWindow::Action();
	
}

void OptionWindow::Stop()
{
	BaseWindow::Stop();

}
