#include "TrainingOptionWindow.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "Sound\SoundManager.h"

//+-----------------------------------
//  トレーニングオプションウィンドウ
//+-----------------------------------

TrainingOptionWindow::TrainingOptionWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/TrainingOption.png");

	//選択アイコンの長さ
	m_iSelectLength = 70;

	// ポーズウィンドウに存在するアイコンを詰めていく
	AddIconData("ヒットポイントの回復", "ヒットポイントの自動回復の設定を行います。");
	AddIconData("ヒットポイントの最大値(1P)", "1Pのヒットポイントの最大値を設定します。");
	AddIconData("ヒットポイントの最大値(2P)", "2Pのヒットポイントの最大値を設定します。");
	AddIconData("SPゲージ", "SPゲージの設定を行います。");
	AddIconData("パートナーゲージ", "パートナーゲージの設定を行います。");
	AddIconData("情報表示", "入力覆歴やダメージ情報の表示を設定します。");
	AddIconData("初期値に戻す", "初期設定に戻します。");
	AddIconData("戻る", "このウィンドウを閉じます。");


	m_pHpRecoveryParam		= new HpRecoveryParamSetting(1, 400);
	m_pHp1PParam			= new Hp1pParamSetting(2, 400);
	m_pHp2PParam			= new Hp2pParamSetting(3, 400);
	m_pSpGageParam			= new SpGageParamSetting(4, 400);
	m_pPartnerRecoveryParam = new PartnerRecoveryParamSetting(5, 400);
	m_pInfoParam			= new InfoParamSetting(6, 400);

}

TrainingOptionWindow::~TrainingOptionWindow()
{
	SAFE_DELETE(m_pHpRecoveryParam);
	SAFE_DELETE(m_pHp1PParam);
	SAFE_DELETE(m_pHp2PParam);
	SAFE_DELETE(m_pSpGageParam);
	SAFE_DELETE(m_pPartnerRecoveryParam);
	SAFE_DELETE(m_pInfoParam);

}


bool TrainingOptionWindow::Update()
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
	m_pHpRecoveryParam->Update();
	m_pHp1PParam->Update();
	m_pHp2PParam->Update();
	m_pSpGageParam->Update();
	m_pPartnerRecoveryParam->Update();
	m_pInfoParam->Update();
		
	return true;// 起動中
}

void TrainingOptionWindow::Redner()
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

		//+------------------------------------------------
		// ただし戻るなどはいつも通り文字分の枠に　逐一記入
		if (i == TrainingOptionWindow::BACK || i == TrainingOptionWindow::RESET)
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
		}
		else
		{
			// パラメータ調整系のWindowなので枠は均等に
			for (int j = 0; j < 28/*m_aIconData[i].iStringLength*/; j++)
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
		RenderParamSetting(i, x + 330, y);

		// for(ぱらめ)
		// if(i ==　パラメータウィンドウの番号と合っていたら)　描画 
		// switch で作ったやつを↑の番号があっていれば出す


		// 40ピクセルずらして描画していく
		y += 40 + m_iAbjustHeight;
	}



#ifdef _DEBUG
	// 
	tdnText::Draw(300, 100, 0xffff00aa, "BGMパラメーター%d", PlayerDataMgr->m_ConfigData.iBGMVolume);
	tdnText::Draw(300, 150, 0xffff00aa, "SEパラメーター%d", PlayerDataMgr->m_ConfigData.iSEVolume);
	tdnText::Draw(300, 200, 0xffff00aa, "VOICEパラメーター%d", PlayerDataMgr->m_ConfigData.iVoiceVolume);

	//tdnText::Draw(300, 250, 0xffff00aa, "スピードレベル%d", iSpeedLv);
	//tdnText::Draw(300, 300, 0xffff00aa, "左おしっぱ%d", iLeftPushFrame);
	//tdnText::Draw(300, 350, 0xffff00aa, "右おしっぱ%d", iRightPushFrame);

#endif // _DEBUG



}

// 操作
bool  TrainingOptionWindow::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 操作音
	CtrlSE(DeviceID);

	// (1212)離してすすむと前のステートとけんかして押して次進む->離して戻るができる
	// 決定
	//if ((TRAINING_OPTION_STATE)m_iSelectNo == TRAINING_OPTION_STATE::BACK)
	//{
	//	// ゲームに戻る動作は離して進む
	//	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
	//	{
	//		// 選択した番号格納！
	//		m_iChoiceState = (TRAINING_OPTION_STATE)m_iSelectNo;
	//		return true;
	//	}
	//}
	//else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// 選択した番号格納！
			m_iChoiceState = (TRAINING_OPTION_STATE)m_iSelectNo;

			// ↑リセットを押したとき
			if (m_iChoiceState == (TRAINING_OPTION_STATE::RESET))
			{
				// 決定ボタンで初期設定に戻します。
				PlayerDataMgr->m_TrainingData.iHpRecovery = (int)HP_RECOVERY_TYPE::AUTO_RECOVERY;
				PlayerDataMgr->m_TrainingData.iHp1P = (int)100;
				PlayerDataMgr->m_TrainingData.iHp2P = (int)100;
				PlayerDataMgr->m_TrainingData.iSpGage = (int)100;
				PlayerDataMgr->m_TrainingData.iPartnerRecovery = (int)PARTNER_RECOVERY_TYPE::MAX;
				PlayerDataMgr->m_TrainingData.iInfo = (int)TRAINING_INFO_TYPE::DAMEGE;
			}

			return true;
		}
	}

	// キャンセルで
	// ゲームに戻るステートを格納
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		// 選択した番号格納！
		m_iChoiceState = (TRAINING_OPTION_STATE::BACK);
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
		CtrlParamSetting(m_iSelectNo, DeviceID);


	}


	return false;

}


void TrainingOptionWindow::Action()
{
	BaseWindow::Action();

	// 透明度を最初は透明に
	m_iAlpha = 0;
	m_pWindow->SetAlpha(m_iAlpha);			// 透明度更新
	m_pFontBox->SetAlpha(m_iAlpha);			// 透明度更新
	m_pSelect->SetAlpha(m_iAlpha);			// 透明度更新
}

void TrainingOptionWindow::Stop()
{
	BaseWindow::Stop();
}

void TrainingOptionWindow::RenderParamSetting(int No, int x, int y)
{
	bool bSelect = false;
	if (No == m_iSelectNo)bSelect = true;


	switch ((TRAINING_OPTION_STATE)No)
	{
	case TRAINING_OPTION_STATE::HP_RECOVERY:
		m_pHpRecoveryParam->Render(x, y, bSelect);
		break;
	case TRAINING_OPTION_STATE::MAXHP_1P:
		m_pHp1PParam->Render(x, y, bSelect);
		break;
	case TRAINING_OPTION_STATE::MAXHP_2P:
		m_pHp2PParam->Render(x, y, bSelect);
		break;
	case TRAINING_OPTION_STATE::SP_GAGE:
		m_pSpGageParam->Render(x, y, bSelect);
		break;
	case TRAINING_OPTION_STATE::PARTNER_GAGE:
		m_pPartnerRecoveryParam->Render(x, y, bSelect);
		break;
	case TRAINING_OPTION_STATE::INFO:
		m_pInfoParam->Render(x, y, bSelect);
		break;
	case TRAINING_OPTION_STATE::BACK:
		break;
	default:
		break;
	}


}

void TrainingOptionWindow::CtrlParamSetting(int SelectNo, int DeviceID)
{
	switch ((TRAINING_OPTION_STATE)SelectNo)
	{
	case TRAINING_OPTION_STATE::HP_RECOVERY:
		m_pHpRecoveryParam->Ctrl(DeviceID);
		break;
	case TRAINING_OPTION_STATE::MAXHP_1P:
		m_pHp1PParam->Ctrl(DeviceID);
		break;
	case TRAINING_OPTION_STATE::MAXHP_2P:
		m_pHp2PParam->Ctrl(DeviceID);
		break;
	case TRAINING_OPTION_STATE::SP_GAGE:
		m_pSpGageParam->Ctrl(DeviceID);
		break;
	case TRAINING_OPTION_STATE::PARTNER_GAGE:
		m_pPartnerRecoveryParam->Ctrl(DeviceID);
		break;
	case TRAINING_OPTION_STATE::INFO:
		m_pInfoParam->Ctrl(DeviceID);
		break;
	case TRAINING_OPTION_STATE::BACK:
		break;
	default:
		break;
	}
}

