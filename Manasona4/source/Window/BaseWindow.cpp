#include "BaseWindow.h"

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
}

void BaseWindow::Action()
{
	// ウィンドウ起動
	m_bActive = true;
	m_iChoiceState = CHOICE_STATE::HOLD;
}

void BaseWindow::Stop()
{
	// ウィンドウ終了
	m_bActive = false;
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
