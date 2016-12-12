#include "SoundWindow.h"
#include "Data\PlayerData.h"
#include "Sound\SoundManager.h"

//+------------------------
//  サウンドウィンドウ
//+------------------------

SoundWindow::SoundWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/PauseWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/PauseWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//選択アイコンの長さ
	m_iSelectLength = 34;

	// ポーズウィンドウに存在するアイコンを詰めていく
	AddIconData("BGM", "BGMの調整を行います。");
	AddIconData("SE" , "SEの調整を行います。");
	AddIconData("VOICE", "VOICEの調整を行います。");
	AddIconData("戻る", "このウィンドウを閉じます。");

	// 演出
	m_iAlpha = 0;
	m_iAddX = 0;
	m_iAddY = 0;

	// スピードレベル
	iSpeedLv = 0;
	iLeftPushFrame = 0;
	iRightPushFrame = 0;
}

SoundWindow::~SoundWindow()
{

}


bool SoundWindow::Update()
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


	return true;// 起動中
}

void SoundWindow::Redner()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return;

	// タイトル
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ★基本的なウィンドウの位置
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// 中身をブロック単位で描画
	int y = (int)(m_vPos.y + 64.0f);
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
		y += 40;
	}

	// 文字
	DWORD fontCol = 0xffffffff;
	//if (m_iSelectNo == i)fontCol = 0xff030a58;
	tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS創英角ｺﾞｼｯｸUB",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
		24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);


#ifdef _DEBUG
	// 
	tdnText::Draw(300, 100, 0xffff00aa, "BGMパラメーター%d",PlayerDataMgr->m_ConfigData.iBGMVolume);
	tdnText::Draw(300, 150, 0xffff00aa, "SEパラメーター%d", PlayerDataMgr->m_ConfigData.iSEVolume);
	tdnText::Draw(300, 200, 0xffff00aa, "VOICEパラメーター%d", PlayerDataMgr->m_ConfigData.iVoiceVolume);

	tdnText::Draw(300, 250, 0xffff00aa, "スピードレベル%d", iSpeedLv);
	tdnText::Draw(300, 300, 0xffff00aa, "左おしっぱ%d", iLeftPushFrame);
	tdnText::Draw(300, 350, 0xffff00aa, "右おしっぱ%d", iRightPushFrame);

#endif // _DEBUG



}

// 操作
bool  SoundWindow::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;


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
			m_iChoiceState = (SOUND_STATE)m_iSelectNo;
			return true;
		}
	}

	// キャンセル
	// ゲームに戻る動作は離して進む
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 2)
	{
		// 選択した番号格納！
		m_iChoiceState = (SOUND_STATE::BACK);
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

		//+------------------------------------------------- 
		//	サウンドのボリューム切り替え
		//+-------------------------------------------------
		
		// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッのための処理
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3||
			tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 1)
		{
			iLeftPushFrame++;
			if (iSpeedLv == 0 && iLeftPushFrame >= 12)iSpeedLv = 1;
			if (iSpeedLv == 1 && iLeftPushFrame >= 77)iSpeedLv = 2;
		
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3 ||
				 tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 1)
		{
			iRightPushFrame++;
			if (iSpeedLv == 0 && iRightPushFrame >= 12)iSpeedLv = 1;
			if (iSpeedLv == 1 && iRightPushFrame >= 77)iSpeedLv = 2;		
		
		}else
		{
			iLeftPushFrame = 0;
			iRightPushFrame = 0;
			iSpeedLv = 0;
		}

		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3 || 
			(iLeftPushFrame % ((12 + iSpeedLv) - (iSpeedLv * 6)) == 0 && iLeftPushFrame != 0))
		{
			// サウンドステート
			switch ((SOUND_STATE)m_iSelectNo)
			{
			case SoundWindow::BGM:
				PlayerDataMgr->m_ConfigData.iBGMVolume--;

				break;
			case SoundWindow::SE:
				PlayerDataMgr->m_ConfigData.iSEVolume--;

				break;
			case SoundWindow::VOICE:
				PlayerDataMgr->m_ConfigData.iVoiceVolume--;

				break;
			case SoundWindow::BACK:
				
				break;
			default:
				break;
			}
			
			// -1以下にさせない
			PlayerDataMgr->m_ConfigData.iBGMVolume = max(0, PlayerDataMgr->m_ConfigData.iBGMVolume);
			PlayerDataMgr->m_ConfigData.iSEVolume = max(0, PlayerDataMgr->m_ConfigData.iSEVolume);
			PlayerDataMgr->m_ConfigData.iVoiceVolume = max(0, PlayerDataMgr->m_ConfigData.iVoiceVolume);

			// 選択時の演出
			m_pSelect->Action();

			// パラメーターさわってる音 
			se->Play("カーソル2");

			// サウンドに反映
			SoundManager::SetBaseVolume(PlayerDataMgr->m_ConfigData.iBGMVolume, PlayerDataMgr->m_ConfigData.iSEVolume, PlayerDataMgr->m_ConfigData.iVoiceVolume);


		}
		if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3 ||
			(iRightPushFrame % ((12 + iSpeedLv) - (iSpeedLv * 6)) == 0 && iRightPushFrame != 0))
		{
			// サウンドステート
			switch ((SOUND_STATE)m_iSelectNo)
			{
			case SoundWindow::BGM:
				PlayerDataMgr->m_ConfigData.iBGMVolume++;

				break;
			case SoundWindow::SE:
				PlayerDataMgr->m_ConfigData.iSEVolume++;

				break;
			case SoundWindow::VOICE:
				PlayerDataMgr->m_ConfigData.iVoiceVolume++;

				break;
			case SoundWindow::BACK:

				break;
			default:
				break;
			}

			// 100以上にさせない
			PlayerDataMgr->m_ConfigData.iBGMVolume = min(100, PlayerDataMgr->m_ConfigData.iBGMVolume);
			PlayerDataMgr->m_ConfigData.iSEVolume = min(100, PlayerDataMgr->m_ConfigData.iSEVolume);
			PlayerDataMgr->m_ConfigData.iVoiceVolume = min(100, PlayerDataMgr->m_ConfigData.iVoiceVolume);

			// 選択時の演出
			m_pSelect->Action();

			// サウンドに反映
			SoundManager::SetBaseVolume(PlayerDataMgr->m_ConfigData.iBGMVolume, PlayerDataMgr->m_ConfigData.iSEVolume, PlayerDataMgr->m_ConfigData.iVoiceVolume);

		}
	}


	return false;

}

void SoundWindow::Action()
{
	BaseWindow::Action();

	// 透明度を最初は透明に
	m_iAlpha = 0;
	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新
}

void SoundWindow::Stop()
{
	BaseWindow::Stop();
}

