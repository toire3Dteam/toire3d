#include "KeyConfig.h"
#include "Sound\SoundManager.h"

//+--------------------------------------------
//	キーコンフィグ
//+--------------------------------------------

KeyConfig::KeyConfig()
{
	m_pWindow = new tdn2DObj("Data/UI/Window/KeyConfigWindow.png");
	
	//選択アイコン
	m_pSelect = new tdn2DAnim("Data/UI/Window/fontBox.png");
	m_pSelect->OrderJump(6, 1, 0.15f);
	m_iSelectLength = 34;

	m_vPos = VECTOR2_ZERO;
	m_bActive = false;
	m_iSelectNo = 0;
	m_iChoiceState = CHOICE_STATE::HOLD;

	m_aIconData.clear();
	m_aIconData.reserve(64);

	// 存在するパラメータ分追加
	AddIconData("戻る");
	AddIconData("□攻撃");
	AddIconData("△スキル");
	AddIconData("×投げ");
	AddIconData("○回避");

	AddIconData("▽必殺");
	AddIconData("▽無敵");
	AddIconData("▽パートナー");
	AddIconData("▽中段");
	AddIconData("▽バースト");

	AddIconData("▽スタート");
	AddIconData("▽セレクト");

	// 二回戻り防止
	m_bBackPush = false;

	// 位置微調整用
	m_iAbjustHeight = -1;

	m_iDeviceID = -1;
}

KeyConfig::~KeyConfig()
{
	SAFE_DELETE(m_pWindow);
	SAFE_DELETE(m_pSelect);

	m_aIconData.clear();
}

bool KeyConfig::Update()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	m_pSelect->Update();

	return true;
}

void KeyConfig::Render()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return;

	// タイトル
	int WindowX = (int)m_vPos.x + (255 /*- m_iAlpha*/);// ★基本的なウィンドウの位置
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// 中身をブロック単位で描画
	int y = (int)(m_vPos.y + 64.0f) + m_iAbjustHeight;
	int x = (int)m_vPos.x + 18 + (255 /*- m_iAlpha*/);
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
			//m_pFontBox->Render(x + (j * 11) - 4, y, 32, 32, 0, 0, 32, 32);
			//
			//// 選択しているタスクなら
			//if (m_iSelectNo == i)
			//{
			//	m_pFontBox->Render(x + (j * 11) - 4, y,
			//		32, 32, 0, 32, 32, 32);
			//}


		}

		// 文字
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		//tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
		//	24, x, y + 2, ARGB(/*m_iAlpha*/255, 255, 255, 255
		//		), RS::COPY);


		if (m_iSelectNo == i)
		{
			// 紺
			//tdnFont::RenderString(m_aIconCommandData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
			//	27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);
			tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
				24, x, y + 2, ARGB(/*m_iAlpha*/255, 3, 10, 88), RS::COPY);
		}
		else
		{
			// 白
			//tdnFont::RenderString(m_aIconCommandData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
			//	27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
			
			tdnFont::RenderString(m_aIconData[i].pString, "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
				24, x, y + 2, ARGB(/*m_iAlpha*/255, 255, 255, 255), RS::COPY);

		}

		if (i != (int)KEY_CONFIG_STATE::BACK)
		{
			std::string l_strSting = "";
			l_strSting = std::to_string(*GetKeyConfigButton((KEY_CONFIG_STATE)i));

			tdnFont::RenderString(l_strSting.c_str(), "HGｺﾞｼｯｸE",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
				24, x + 240, y + 2, ARGB(255, 255, 255, 255), RS::COPY);
		}

		// 40ピクセルずらして描画していく
		y += 40 + m_iAbjustHeight;
	}

}

bool KeyConfig::Ctrl()
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;


	if (tdnInput::KeyGet(KEYCODE::KEY_A, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_A);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_B, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_B);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_C, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_C);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_D, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_D);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_L1, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_L1);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_L2, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_L2);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_R1, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_R1);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_R2, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_R2);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_R3, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_R3);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_START, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_START);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_SELECT);
	}


	// 決定または戻るで
	if ((KEY_CONFIG_STATE)m_iSelectNo == KEY_CONFIG_STATE::BACK)
	{
		// ゲームに戻る動作は離して進む
		if (tdnInput::KeyGet(KEYCODE::KEY_B, m_iDeviceID) == 3 ||
			tdnInput::KeyGet(KEYCODE::KEY_A, m_iDeviceID) == 3)
		{
			// 選択した番号格納！
			m_iChoiceState = (KEY_CONFIG_STATE)m_iSelectNo;
			return true;
		}
	}
	

	// 決定を押してない時
	//if (tdnInput::KeyGet(KEYCODE::KEY_B, m_iDeviceID) == 0 &&
	//	tdnInput::KeyGet(KEYCODE::KEY_A, m_iDeviceID) == 0)
	{


		// 選択切り替え
		if (tdnInput::KeyGet(KEYCODE::KEY_UP, m_iDeviceID) == 3)
		{
			m_iSelectNo--;
			// 選択時の演出
			//m_pSelect->Action();
			// 最後へ進む
			if (m_iSelectNo <= -1)
			{
				m_iSelectNo = ((int)m_aIconData.size() - 1);
			}
			se->Play("カーソル1");
		}
		if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, m_iDeviceID) == 3)
		{
			m_iSelectNo++;
			// 選択時の演出
			//m_pSelect->Action();
			// 最初に戻る
			if (m_iSelectNo >= (int)m_aIconData.size())
			{
				m_iSelectNo = 0;
			}
			se->Play("カーソル1");
		}

		// 選択してるパラメータの操作
		//CtrlParamSetting(m_iSelectNo, DeviceID);

	
	}

	return false;


}

void KeyConfig::Action(Vector2 vPos, int iDeviceID)
{
	// 現在のコントローラーのコンフィグのデータをとってくる
	memcpy_s(&m_tagConfigData, sizeof(KEY_CONFIG_DATA), tdnInputManager::GetConfigDatas(iDeviceID), sizeof(KEY_CONFIG_DATA));
	memcpy_s(&m_tagOrgConfigData, sizeof(KEY_CONFIG_DATA), tdnInputManager::GetConfigDatas(iDeviceID), sizeof(KEY_CONFIG_DATA));

	m_vPos = vPos;
	m_iDeviceID = iDeviceID;

	m_iChoiceState = CHOICE_STATE::HOLD;
	// ウィンドウ起動
	m_bActive = true;
}

void KeyConfig::Stop()
{
	// ウィンドウ終了
	m_bActive = false;
	m_iChoiceState = CHOICE_STATE::HOLD;// (12/12) ここにもHOLDの処理

	// キーコンフィグ情報を保存！
	tdnInputManager::SetConfig(m_tagConfigData, m_iDeviceID);

	m_iDeviceID = -1;

}

void KeyConfig::AddIconData(LPSTR string)
{
	IconData data;
	data.pString = string;
	//data.pInfoString = infoString;

	// 文字の長さを調べる
	UINT	myByte = 0;
	UINT	addByte = 0;
	//	終端文字までループ
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	文字のバイト数を調べる	
		myByte = _mbclen((BYTE*)&string[i]);
		addByte += myByte;
	}
	data.iStringLength = addByte;

	m_aIconData.push_back(data);
}

void KeyConfig::SetConfig(KEYCODE PushedKey)
{
	BYTE *l_pSelectKey = GetKeyConfigButton((KEY_CONFIG_STATE)m_iSelectNo);

	// 値入ってなかったら(戻るボタン)出ていけぇ！！
	if (!l_pSelectKey) return;

	// L1-R1-L2-R2-L3-R3　→　L1-L2-L3-R1-R2-R3
	BYTE Push = m_tagOrgConfigData.tagPadSet.AllDatas[(BYTE)PushedKey];
	switch (PushedKey)
	{
	case KEYCODE::KEY_L1:
		Push = m_tagOrgConfigData.tagPadSet.L1;
		break;
	case KEYCODE::KEY_L2:
		Push = m_tagOrgConfigData.tagPadSet.L2;
		break;
	case KEYCODE::KEY_L3:
		Push = m_tagOrgConfigData.tagPadSet.L3;
		break;
	case KEYCODE::KEY_R1:
		Push = m_tagOrgConfigData.tagPadSet.R1;
		break;
	case KEYCODE::KEY_R2:
		Push = m_tagOrgConfigData.tagPadSet.R2;
		break;
	case KEYCODE::KEY_R3:
		Push = m_tagOrgConfigData.tagPadSet.R3;
		break;
	}

	// 押したキーと設定されている同じボタンだったら出ていけぇ！！
	if (Push == *l_pSelectKey) return;

	//BYTE a(tdnInputManager::GetConfigDatas(m_iDeviceID)->tagPadSet.AllDatas[(BYTE)PushedKey]), b(*l_pSelectKey), c(m_tagConfigData.tagPadSet.AllDatas[(BYTE)PushedKey]);

	// キー検索
	for (int i = 4; i < 16; i++)
	{
		if (Push == m_tagConfigData.tagPadSet.AllDatas[i])
		{
			// 前のやつと入れ替える
			BYTE temp = *l_pSelectKey;
			*l_pSelectKey = Push;
			m_tagConfigData.tagPadSet.AllDatas[i] = temp;
		}
	}

	se->Play("決定1");
}

BYTE *KeyConfig::GetKeyConfigButton(KEY_CONFIG_STATE eState)
{
	switch (eState)
	{
	case KeyConfig::ATTACK:
		return &m_tagConfigData.tagPadSet.C;
		break;
	case KeyConfig::SKILL:
		return &m_tagConfigData.tagPadSet.D;
		break;
	case KeyConfig::THROW:
		return &m_tagConfigData.tagPadSet.A;
		break;
	case KeyConfig::ESCAPE:
		return &m_tagConfigData.tagPadSet.B;
		break;
	case KeyConfig::HEAVEHO_DRIVE:
		return &m_tagConfigData.tagPadSet.R2;
		break;
	case KeyConfig::INVINCIBLE:
		return &m_tagConfigData.tagPadSet.L2;
		break;
	case KeyConfig::PARTNER:
		return &m_tagConfigData.tagPadSet.L1;
		break;
	case KeyConfig::DOKKOI:
		return &m_tagConfigData.tagPadSet.R1;
		break;
	case KeyConfig::OVER_DRIVE:
		return &m_tagConfigData.tagPadSet.R3;
		break;
	case KeyConfig::START:
		return &m_tagConfigData.tagPadSet.START;
		break;
	case KeyConfig::SELECT:
		return &m_tagConfigData.tagPadSet.SELECT;
		break;
	default:
		return nullptr;
		break;
	}
}
