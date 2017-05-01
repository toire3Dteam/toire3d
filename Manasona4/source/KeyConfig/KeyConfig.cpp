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
	AddIconData("□攻撃");
	AddIconData("△スキル");
	AddIconData("×投げ");
	AddIconData("○回避");

	AddIconData("▽必殺");
	AddIconData("▽無敵");
	AddIconData("▽パートナー");
	AddIconData("▽中段");
	AddIconData("▽バースト");

	// 二回戻り防止
	m_bBackPush = false;

	// 位置微調整用
	m_iAbjustHeight = -1;

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


		// 40ピクセルずらして描画していく
		y += 40 + m_iAbjustHeight;
	}

}

bool KeyConfig::Ctrl(int DeviceID)
{
	// 起動していなかったらハジく
	if (m_bActive == false)return false;

	// 
	// 押したときのSE
	// 決定
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		se->Play("決定1");
	}

	// 戻る
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		se->Play("キャンセル1");
	}

	// ↑
	if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
	{
		se->Play("カーソル1");
	}

	// ↓
	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
	{
		se->Play("カーソル1");
	}


	// 決定を押してない時
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0)
	{

		// 選択切り替え
		if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
		{
			m_iSelectNo--;
			// 選択時の演出
			//m_pSelect->Action();
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
			//m_pSelect->Action();
			// 最初に戻る
			if (m_iSelectNo >= (int)m_aIconData.size())
			{
				m_iSelectNo = 0;
			}
		}

		// 選択してるパラメータの操作
		//CtrlParamSetting(m_iSelectNo, DeviceID);

	
	}

	return false;


}

void KeyConfig::Action(Vector2 vPos)
{
	m_vPos = vPos;

	m_iChoiceState = CHOICE_STATE::HOLD;
	// ウィンドウ起動
	m_bActive = true;
}

void KeyConfig::Stop()
{
	// ウィンドウ終了
	m_bActive = false;
	m_iChoiceState = CHOICE_STATE::HOLD;// (12/12) ここにもHOLDの処理
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
