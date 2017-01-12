#include "BaseWindow.h"
#include "Sound\SoundManager.h"

//+------------------------
// ウィンドウ画面のベース
//+------------------------

// [メモ]　別シーンへ移動する処理はこの中でせずメッセージで送ってやりましょう

BaseWindow::BaseWindow(Vector2 vPos)
{	
	// ウィンドウの場所
	m_vPos = vPos;

	m_pWindow = nullptr;
	m_pTop = nullptr;
	m_pMiddle= nullptr;
	m_pBottom= nullptr;
	m_pSelect = new tdn2DAnim("Data/UI/Window/fontBox.png");
	m_pSelect->OrderJump(6, 1, 0.15f);
	m_iSelectLength = 32;
	m_pFontBox = new tdn2DObj("Data/UI/Window/fontBox.png");

	m_bActive = false;
	m_iSelectNo = 0;
	m_aIconData.clear();
	m_aIconData.reserve(64);
	m_iChoiceState = CHOICE_STATE::HOLD;

	// 
	m_aIconCommandData.clear();
	m_aIconCommandData.reserve(64);

	// 演出用に
	m_iAlpha = 0;
	m_iAddX = 0;
	m_iAddY = 0;
}

BaseWindow::~BaseWindow()
{
	SAFE_DELETE(m_pWindow);
	SAFE_DELETE(m_pTop);
	SAFE_DELETE(m_pMiddle);
	SAFE_DELETE(m_pBottom);
	SAFE_DELETE(m_pSelect);
	SAFE_DELETE(m_pFontBox);
	m_aIconData.clear();
	m_aIconCommandData.clear();
}

// 下に表記する説明描画
void BaseWindow::RednerInfo()
{
	// 文字
	DWORD fontCol = 0xffffffff;
	//if (m_iSelectNo == i)fontCol = 0xff030a58;
	tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS創英角ｺﾞｼｯｸUB",// HGP創英ﾌﾟﾚｾﾞﾝｽEB
		24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
}

// 操作音
void BaseWindow::CtrlSE(int DeviceID)
{
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

}

void BaseWindow::Action()
{
	m_iChoiceState = CHOICE_STATE::HOLD;
	// ウィンドウ起動
	m_bActive = true;

}

void BaseWindow::Stop()
{
	// ウィンドウ終了
	m_bActive = false;
	m_iChoiceState = CHOICE_STATE::HOLD;// (12/12) ここにもHOLDの処理
}

void BaseWindow::ChoiceClear()
{
	m_iChoiceState = CHOICE_STATE::HOLD; // 未選択へ
}

void BaseWindow::AddIconData(LPSTR string, LPSTR infoString)
{
	IconData data;
	data.pString = string;
	data.pInfoString = infoString;

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

void BaseWindow::AddCommandData(LPSTR string, int iWidthSize)
{
	IconCommandData data;
	data.pString = string;
	data.iWidthSize = iWidthSize;

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

	m_aIconCommandData.push_back(data);

}
