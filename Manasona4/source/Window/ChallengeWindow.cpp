#include "ChallengeWindow.h"

//+------------------------
// �`�������W�E�B���h�E
//+------------------------

ChallengeWindow::ChallengeWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/ChallengeWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/ChallengeWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");
					
	//�I���A�C�R���̒���
	m_iSelectLength = 30;

	// �I�v�V�����E�B���h�E�ɑ��݂���A�C�R�����l�߂Ă���
	AddIconData("�A�C���\", "�A�C���[�Ń`�������W���[�h���n�߂܂��B");
	AddIconData("�e�L", "�e�L�Ń`�������W���[�h���n�߂܂��B");
	AddIconData("(��)", "(��)�Ń`�������W���[�h���n�߂܂��B");
	AddIconData("�A���~�^�}", "�A���~�^�}�Ń`�������W���[�h���n�߂܂��B");
	AddIconData("�߂�", "���̃E�B���h�E����܂��B");

	// ���o
	//m_iAlpha = 0;

}

ChallengeWindow::~ChallengeWindow()
{

}


bool ChallengeWindow::Update()
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	// �X�V�����ق��̎q��Window���N�����Ă�����
	// ����ł��Ȃ��悤�ɂ���B

	m_pSelect->Update();

	// �����x�X�V
	m_iAlpha += 255 / 6;
	m_iAlpha = min(255, m_iAlpha);

	m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V							 // �����x�X�V
	m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V

	//// �p�b�h���X�V
	//const int NumDevice = (tdnInputManager::GetNumDevice());

	//// �p�b�h�����������ĂȂ��Ƃ��p
	//if (NumDevice == 0)
	//{
	//	if (Ctrl(0))
	//	{
	//		// ������������炱���ɓ���
	//		
	//		return true;
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < NumDevice; i++)
	//	{
	//		if (Ctrl(i))
	//		{
	//			// ������������炱���ɓ���

	//			return true;
	//		}
	//	}
	//}


	return true;// �N����
}

void ChallengeWindow::Redner()
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return;

	// �^�C�g��
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ����{�I�ȃE�B���h�E�̈ʒu
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// ���g���u���b�N�P�ʂŕ`��
	int y = (int)(m_vPos.y + 64.0f) + m_iAbjustHeight;
	int x = (int)m_vPos.x + 18 + (255 - m_iAlpha);
	for (int i = 0; i < (int)m_aIconData.size(); i++)
	{

		// �Ō���Ȃ炵���ۂ�`��
		if (i == (int)m_aIconData.size() - 1)
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 46, 0, 64+40, SizeX, 46);


		}
		else// ��
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 40, 0, 64, SizeX, 40);
		}



		//+-----------------------
		// �I�����Ă���ԍ��Ȃ�
		if (i == m_iSelectNo)
		{
			// (TUDO) �I�����Ă��鏊��ڗ������鉉�o
			
			//+-----------------------
			// �I�����Ă鉩�F�������g
			for (int j = 0; j < m_iSelectLength; j++)
			{
				m_pSelect->Render(x + (j * 11) - 4, y, 32, 32, 0, 32, 32, 32);
			}

		}

		//+-----------------------
		// �g
		for (int j = 0; j < m_aIconData[i].iStringLength; j++)
		{
			m_pFontBox->Render(x + (j * 11) - 4, y, 32, 32, 0, 0, 32, 32);

			// �I�����Ă���^�X�N�Ȃ�
			if (m_iSelectNo == i)
			{
				m_pFontBox->Render(x + (j * 11) - 4, y,
					32, 32, 0, 32, 32, 32);
			}

		}

		// ����
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);


		// 40�s�N�Z�����炵�ĕ`�悵�Ă���
		y += 40 + m_iAbjustHeight;
	}

	//// ����
	//DWORD fontCol = 0xffffffff;
	////if (m_iSelectNo == i)fontCol = 0xff030a58;
	//tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS�n�p�p�޼��UB",// HGP�n�p��ھ�ݽEB
	//	24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
}

// ����
bool  ChallengeWindow::Ctrl(int DeviceID)
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	CtrlSE(DeviceID);// ���쉹

	// �I��؂�ւ�
	if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
	{
		m_iSelectNo--;
		// �I�����̉��o
		m_pSelect->Action();
		// �Ō�֐i��
		if (m_iSelectNo <= -1)
		{
			m_iSelectNo = ((int)m_aIconData.size() - 1);
		}
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
	{
		m_iSelectNo++;
		// �I�����̉��o
		m_pSelect->Action();
		// �ŏ��ɖ߂�
		if (m_iSelectNo >= (int)m_aIconData.size())
		{
			m_iSelectNo = 0;
		}
	}

	// ����
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// �I�������ԍ��i�[�I
		m_iChoiceState = (CHALLENGE_STATE)m_iSelectNo;
		return true;
	}

	// �L�����Z����
	// �Q�[���ɖ߂�X�e�[�g���i�[
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		// �I�������ԍ��i�[�I
		m_iChoiceState = (CHALLENGE_STATE::BACK);
		return true;
	}

	return false;

}

void ChallengeWindow::Action()
{
	if (m_bActive == false)// �����E�B���h�E���o�Ă���Ƃ��͉��K���n�W�N
	{
		// �����x���ŏ��͓�����
		m_iAlpha = 0;
		m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V
		m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
		m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V
	}

	BaseWindow::Action();
	
}

void ChallengeWindow::Stop()
{
	BaseWindow::Stop();

}
