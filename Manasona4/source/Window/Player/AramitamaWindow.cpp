#include "AramitamaWindow.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+-------------------------------
//	アラミタマウィンドウ
//+-------------------------------

AramitamaWindow::AramitamaWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/CommandList.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/SoundSettingWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//選択アイコンの長さ
	m_iSelectLength = 53;

	// オプションウィンドウに存在するアイコンを詰めていく
	AddIconData("■ローリング�@", "転がって相手を連続で攻撃をする。");
	AddIconData("■チャージ�A", "ゲージを溜めます。");
	AddIconData("■車輪�B", "空中から横に回転しながら攻撃する。");
	AddIconData("■空中回避�B", "後ろへ飛びながら回避します。");
	
	AddIconData("■トスミタマ", "味方が相手を上方向に吹っ飛ばします。");
	AddIconData("■隕石ミタマ", "味方が空中から相手に突進します。");
	AddIconData("■Dミタマ", "味方が下から上へ回転しながら攻撃します。");

	AddIconData("▲月影", "相手を吹き飛ばし、切り替えし攻撃をします。");
	AddIconData("●ミタマキャノン", "上方向にビームを放つ必殺技。");

	AddCommandData("△", 365);
	AddCommandData("↓＋△", 365);
	AddCommandData("空中で△", 365);
	AddCommandData("空中で↓＋△", 365);

	AddCommandData("�@後に自動で", 365);
	AddCommandData("�A後に自動で", 365);
	AddCommandData("�B後に自動で", 365);
	AddCommandData("��", 365);
	AddCommandData("SP50以上で��", 365);

	// 演出
	//m_iAlpha = 0;

}

AramitamaWindow::~AramitamaWindow()
{

}


bool AramitamaWindow::Update()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 更新だけほかの子のWindowが起動していたら
	// 操作できないようにする。

	m_pSelect->Update();

	// 透明度更新
	m_iAlpha += 255 / 6;
	m_iAlpha = min(255, m_iAlpha);

	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新							 // 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新

	return true;// 起動中
}

void AramitamaWindow::Redner()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return;

	// タイトル
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ★基本的なウィンドウの位置
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		m_cSizeX, 64, 0, 0, m_cSizeX, 64);


	// 中身をブロック単位で描画
	int y = (int)(m_vPos.y + 64.0f) + m_iAbjustHeight;
	int x = (int)m_vPos.x + 18 + (255 - m_iAlpha);
	for (int i = 0; i < (int)m_aIconData.size(); i++)
	{

		// 最後尾ならしっぽを描画
		if (i == (int)m_aIconData.size() - 1)
		{
			m_pWindow->Render(WindowX, y,
				m_cSizeX, 46, 0, 64 + 40, m_cSizeX, 46);


		}
		else// 中
		{
			m_pWindow->Render(WindowX, y,
				m_cSizeX, 40, 0, 64, m_cSizeX, 40);
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
		for (int j = 0; j < 30; j++)
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

		// コマンドの文字
		DWORD fontComCol = 0xff4e84e6;	

		if (m_iSelectNo == i)
		{
			// 紺
			tdnFont::RenderString(m_aIconCommandData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
				27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);
		}
		else
		{
			// 白
			tdnFont::RenderString(m_aIconCommandData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
				27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);

		}

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
bool  AramitamaWindow::Ctrl(int DeviceID)
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

	//// 決定
	//if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	//{
	//	// 選択した番号格納！
	//	m_iChoiceState = (OPTION_STATE)m_iSelectNo;
	//	return true;
	//}

	//// キャンセルで
	//// ゲームに戻るステートを格納
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	//{
	//	// 選択した番号格納！
	//	m_iChoiceState = (OPTION_STATE::BACK);
	//	return true;
	//}

	return false;

}

void AramitamaWindow::Action()
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

void AramitamaWindow::Stop()
{
	BaseWindow::Stop();

}
