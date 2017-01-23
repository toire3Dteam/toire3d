#include "ChallengePauseWindow.h"

#include "PauseWindow.h"

//+------------------------
//  チュートリアルポーズウィンドウ
//+------------------------

ChallengePauseWindow::ChallengePauseWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/ChallengePauseWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/PauseWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//選択アイコンの長さ
	m_iSelectLength = 60;

	// ポーズウィンドウに存在するアイコンを詰めていく
	//AddIconData("前のチュートリアルへ", "一つ前のチュートリアルに戻ります。");
	AddIconData("チャレンジセレクト", "チャレンジ状態を変更できます。");
	AddIconData("ポジションリセット", "ポジションを初期状態に戻します。");
	AddIconData("コマンドリスト", "操作しているキャラクターのコマンドリストを確認します。");
	AddIconData("サウンド設定", "各種サウンドの設定を行います。");
	AddIconData("メニュー非表示", "メニュー画面を非表示にします。");
	AddIconData("メニューに戻る", "メニュー画面に戻ります。");
	AddIconData("戻る", "このウィンドウを閉じます。");

	// 演出
	//m_iAlpha = 0;
	//m_iAddX = 0;
	//m_iAddY = 0;

	m_pInfoPlate = new InformationPlate();

	m_ChallengeSelectParam = new ChallengeSelectParamSetting(1, 325);


	// 二回戻り防止
	m_bBackPush = false;
}

ChallengePauseWindow::~ChallengePauseWindow()
{
	SAFE_DELETE(m_pInfoPlate);

	SAFE_DELETE(m_ChallengeSelectParam);

}


bool ChallengePauseWindow::Update()
{
	// 説明の下に乗せる板
	m_pInfoPlate->Update();

	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 更新だけほかの子のWindowが起動していたら
	// 操作できないようにする。

	m_pSelect->Update();

	// 透明度更新
	m_iAlpha += 255 / 4;
	m_iAlpha = min(255, m_iAlpha);

	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新							 // 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新

	// パラメータ更新
	m_ChallengeSelectParam->Update();

	return true;// 起動中
}

void ChallengePauseWindow::Redner()
{
	// 説明の下に乗せる板
	m_pInfoPlate->Render();

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
				SizeX, 46, 0, 64 + 40, SizeX, 46);
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
		//+------------------------------------------------	
		if (i == ChallengePauseWindow::CHALLENGE_PAUSE_STATE::SELECT_CHALLENGE)
		{
			// パラメータ調整系のWindowなので枠は均等に
			for (int j = 0; j < 25; j++)
			{
				m_pFontBox->Render(x + (j * 11) - 4, y, 32, 32, 0, 0, 32, 32);

				// 選択しているタスクなら
				if (m_iSelectNo == i)
				{
					m_pFontBox->Render(x + (j * 11) - 4, y,
						32, 32, 0, 32, 32, 32);
				}

			}
		}
		else
		{
			// 戻るなどはいつも通り文字分の枠に　逐一記入
			for (int j = 0; j < m_aIconData[i].iStringLength; j++)
			{
				m_pFontBox->Render(x + (j * 12) - 4, y, 32, 32, 0, 0, 32, 32);

				// 選択しているタスクなら
				if (m_iSelectNo == i)
				{
					m_pFontBox->Render(x + (j * 12) - 4, y,
						32, 32, 0, 32, 32, 32);
				}

			}

		}

		// 文字
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);

		
		// パラメーター関連の描画
		RenderParamSetting(i, x + 300, y);


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
bool  ChallengePauseWindow::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 操作音
	CtrlSE(DeviceID);

	// 決定
	if ((CHALLENGE_PAUSE_STATE)m_iSelectNo == CHALLENGE_PAUSE_STATE::BACK)
	{
		// ゲームに戻る動作は離して進む
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
		{
			// 選択した番号格納！
			m_iChoiceState = (CHALLENGE_PAUSE_STATE)m_iSelectNo;
			return true;
		}
	}
	else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// 選択した番号格納！
			m_iChoiceState = (CHALLENGE_PAUSE_STATE)m_iSelectNo;
			return true;
		}
	}

	// キャンセルボタンを押し込む動作が必要に
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		m_bBackPush = true;
	}

	// キャンセルボタン
	if (m_bBackPush == true)// 一度キャンセルボタンを押し込んでいたら
	{
		// ゲームに戻る動作は離して進む
		if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 2)
		{
			// 選択した番号格納！
			m_iChoiceState = (CHALLENGE_PAUSE_STATE::BACK);
			return true;
		}
	}

	if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 3)
	{
		// 選択した番号格納！
		m_iChoiceState = (CHALLENGE_PAUSE_STATE::BACK);
		return true;
	}

	// 決定やキャンセルを押してない時
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0 &&
		tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 0)
	{

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

	}

	// 選択してるパラメータの操作
	CtrlParamSetting(m_iSelectNo, DeviceID);

	return false;

}

void ChallengePauseWindow::Action()
{
	if (m_bActive == false)// もうウィンドウが出ているときは演習をハジク
	{
		// 透明度を最初は透明に
		m_iAlpha = 0;
		m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
		m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
		m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新

		m_pInfoPlate->Action();// (TODO) ベースで
	}

	BaseWindow::Action();

	// 二回戻り防止
	m_bBackPush = false;
}

void ChallengePauseWindow::Stop()
{
	BaseWindow::Stop();

	m_pInfoPlate->Stop();

	// 二回戻り防止
	m_bBackPush = false;
}


void ChallengePauseWindow::RenderParamSetting(int No, int x, int y)
{
	bool bSelect = false;
	if (No == m_iSelectNo)bSelect = true;


	switch ((CHALLENGE_PAUSE_STATE)No)
	{
	case CHALLENGE_PAUSE_STATE::SELECT_CHALLENGE:
		m_ChallengeSelectParam->Render(x, y, bSelect);
		break;
	case CHALLENGE_PAUSE_STATE::BACK:
		
		break;
	default:
		break;
	}


}

void ChallengePauseWindow::CtrlParamSetting(int SelectNo, int DeviceID)
{
	switch ((CHALLENGE_PAUSE_STATE)SelectNo)
	{
	case CHALLENGE_PAUSE_STATE::SELECT_CHALLENGE:
		m_ChallengeSelectParam->Ctrl(DeviceID);
		break;
	case CHALLENGE_PAUSE_STATE::BACK:

		break;
	default:
		break;
	}

}

