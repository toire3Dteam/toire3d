#include "GameWindow.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+------------------------
//  ゲームウィンドウ
//+------------------------

GameWindow::GameWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/GameOptionsWindow.png");

	//選択アイコンの長さ
	m_iSelectLength = 54;

	// ポーズウィンドウに存在するアイコンを詰めていく
	AddIconData("CPU難易度", "CPUの難易度の変更を行います。");
	AddIconData("勝利ラウンド数", "勝利条件ラウンド数を設定します。");
	AddIconData("時間制限", "1ラウンドの制限時間を設定します。");
	AddIconData("初期値に戻す", "初期設定に戻します。");
	AddIconData("戻る", "このウィンドウを閉じます。");

	m_pDifficultyParam = new DifficultyParamSetting(1, 275);
	m_pRoundNumTypeParam = new RoundNumTypeParamSetting(2, 275);
	m_pRoundTimeTypeParam = new RoundTimeTypeParamSetting(3, 275);
	//// スピードレベル
	//iSpeedLv = 0;
	//iLeftPushFrame = 0;
	//iRightPushFrame = 0;
}

GameWindow::~GameWindow()
{
	SAFE_DELETE(m_pDifficultyParam);
	SAFE_DELETE(m_pRoundNumTypeParam);
	SAFE_DELETE(m_pRoundTimeTypeParam);
}


bool GameWindow::Update()
{
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

	// パラメーター更新
	m_pDifficultyParam->Update();
	m_pRoundNumTypeParam->Update();
	m_pRoundTimeTypeParam->Update();

	return true;// 起動中
}

void GameWindow::Redner()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return;

	// タイトル
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ★基本的なウィンドウの位置
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		m_cSizeX, 64, 0, 0, m_cSizeX, 64);


	// 中身をブロック単位で描画
	int y = (int)(m_vPos.y + 64.0f);
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

		//+------------------------------------------------
		// ただし戻るなどはいつも通り文字分の枠に　逐一記入
		if (i == GameWindow::BACK|| i == GameWindow::RESET)
		{
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
		}else
		{
			// パラメータ調整系のWindowなので枠は均等に
			for (int j = 0; j < 25/*m_aIconData[i].iStringLength*/; j++)
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


		// 文字
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);


		// パラメーター関連の描画
		RenderParamSetting(i, x + 300 , y);

		// for(ぱらめ)
		// if(i ==　パラメータウィンドウの番号と合っていたら)　描画 
		// switch で作ったやつを↑の番号があっていれば出す


		// 40ピクセルずらして描画していく
		y += 40;
	}



#ifdef _DEBUG
	// 
	//tdnText::Draw(300, 100, 0xffff00aa, "BGMパラメーター%d",PlayerDataMgr->m_ConfigData.iBGMVolume);
	//tdnText::Draw(300, 150, 0xffff00aa, "SEパラメーター%d", PlayerDataMgr->m_ConfigData.iSEVolume);
	//tdnText::Draw(300, 200, 0xffff00aa, "VOICEパラメーター%d", PlayerDataMgr->m_ConfigData.iVoiceVolume);

	//tdnText::Draw(300, 250, 0xffff00aa, "スピードレベル%d", iSpeedLv);
	//tdnText::Draw(300, 300, 0xffff00aa, "左おしっぱ%d", iLeftPushFrame);
	//tdnText::Draw(300, 350, 0xffff00aa, "右おしっぱ%d", iRightPushFrame);

#endif // _DEBUG



}

// 操作
bool  GameWindow::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 操作音
	CtrlSE(DeviceID);

	// (1212)離してすすむと前のステートとけんかして押して次進む->離して戻るができる
	// 決定
	//if ((SOUND_STATE)m_iSelectNo == SOUND_STATE::BACK)
	//{
	//	// ゲームに戻る動作は離して進む
	//	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
	//	{
	//		// 選択した番号格納！
	//		m_iChoiceState = (SOUND_STATE)m_iSelectNo;
	//		return true;
	//	}
	//}
	//else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// 選択した番号格納！
			m_iChoiceState = (GAME_STATE)m_iSelectNo;
		
			// ↑リセットを押したとき
			if (m_iChoiceState == (GAME_STATE::RESET))
			{
				// 決定ボタンで初期設定に戻します。
				PlayerDataMgr->m_ConfigData.iRoundNumType = (int)ROUND_NUM_TYPE::ROUND_2;
				PlayerDataMgr->m_ConfigData.iRoundTimeType = (int)ROUND_TIME_TYPE::SEC_99;
				PlayerDataMgr->m_ConfigData.iDifficultyAI = (int)AI_TYPE::CPU_NORMAL;
			}

			return true;
		}
	}

	// キャンセルで
	// ゲームに戻るステートを格納
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		// 選択した番号格納！
		m_iChoiceState = (GAME_STATE::BACK);
		return true;
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 3)
	{
		// 選択した番号格納！
		//m_iChoiceState = (BATTLE_STATE::BACK);
		return true;
	}


	// 決定を押してない時
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0)
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


		// 選択してるパラメータの操作
		CtrlParamSetting(m_iSelectNo,DeviceID);
	}


	return false;

}


void GameWindow::Action()
{
	BaseWindow::Action();

	// 透明度を最初は透明に
	m_iAlpha = 0;
	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新
}

void GameWindow::Stop()
{
	BaseWindow::Stop();
}

void GameWindow::RenderParamSetting(int No, int x, int y)
{
	bool bSelect = false;
	if (No == m_iSelectNo)bSelect = true;


	switch ((GAME_STATE)No)
	{
	case GameWindow::DIFFICULTY:
		m_pDifficultyParam->Render(x, y, bSelect);
		break;
	case GameWindow::GAME_STATE::ROUND:
		m_pRoundNumTypeParam->Render(x, y, bSelect);
		break;
	case GameWindow::GAME_STATE::TIME:
		m_pRoundTimeTypeParam->Render(x, y, bSelect);
		break;
	case GameWindow::BACK:
		break;
	default:
		break;
	}


}

void GameWindow::CtrlParamSetting(int SelectNo, int DeviceID)
{
	switch ((GAME_STATE)SelectNo)
	{
	case GameWindow::DIFFICULTY:
		m_pDifficultyParam->Ctrl(DeviceID);
		break;
	case GameWindow::GAME_STATE::ROUND:
		m_pRoundNumTypeParam->Ctrl(DeviceID);
		break;
	case GameWindow::GAME_STATE::TIME:
		m_pRoundTimeTypeParam->Ctrl(DeviceID);
		break;
	case GameWindow::RESET:

		break;
	case GameWindow::BACK:
		
		break;
	default:
		break;
	}
}

