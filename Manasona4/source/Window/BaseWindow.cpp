#include "BaseWindow.h"

//+------------------------
// �E�B���h�E��ʂ̃x�[�X
//+------------------------

// [����]�@�ʃV�[���ֈړ����鏈���͂��̒��ł������b�Z�[�W�ő����Ă��܂��傤

BaseWindow::BaseWindow(Vector2 vPos)
{	
	// �E�B���h�E�̏ꏊ
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
	// �E�B���h�E�N��
	m_bActive = true;
	m_iChoiceState = CHOICE_STATE::HOLD;
}

void BaseWindow::Stop()
{
	// �E�B���h�E�I��
	m_bActive = false;
}

void BaseWindow::AddIconData(LPSTR string, LPSTR infoString)
{
	IconData data;
	data.pString = string;
	data.pInfoString = infoString;

	// �����̒����𒲂ׂ�
	UINT	myByte = 0;
	UINT	addByte = 0;
	//	�I�[�����܂Ń��[�v
	for (UINT i = 0; string[i] != '\0'; i += myByte)
	{
		//	�����̃o�C�g���𒲂ׂ�	
		myByte = _mbclen((BYTE*)&string[i]);
		addByte += myByte;
	}
	data.iStringLength = addByte;

	m_aIconData.push_back(data);
}
