#include "BaseWindow.h"
#include "Sound\SoundManager.h"

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

	// 
	m_aIconCommandData.clear();
	m_aIconCommandData.reserve(64);

	// ���o�p��
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

// ���ɕ\�L��������`��
void BaseWindow::RednerInfo()
{
	// ����
	DWORD fontCol = 0xffffffff;
	//if (m_iSelectNo == i)fontCol = 0xff030a58;
	tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS�n�p�p�޼��UB",// HGP�n�p��ھ�ݽEB
		24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
}

// ���쉹
void BaseWindow::CtrlSE(int DeviceID)
{
	// �������Ƃ���SE

	// ����
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		se->Play("����1");
	}

	// �߂�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		se->Play("�L�����Z��1");
	}

	// ��
	if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
	{
		se->Play("�J�[�\��1");
	}

	// ��
	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
	{
		se->Play("�J�[�\��1");
	}

}

void BaseWindow::Action()
{
	m_iChoiceState = CHOICE_STATE::HOLD;
	// �E�B���h�E�N��
	m_bActive = true;

}

void BaseWindow::Stop()
{
	// �E�B���h�E�I��
	m_bActive = false;
	m_iChoiceState = CHOICE_STATE::HOLD;// (12/12) �����ɂ�HOLD�̏���
}

void BaseWindow::ChoiceClear()
{
	m_iChoiceState = CHOICE_STATE::HOLD; // ���I����
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

void BaseWindow::AddCommandData(LPSTR string, int iWidthSize)
{
	IconCommandData data;
	data.pString = string;
	data.iWidthSize = iWidthSize;

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

	m_aIconCommandData.push_back(data);

}
