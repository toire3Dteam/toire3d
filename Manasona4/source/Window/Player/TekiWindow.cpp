#include "TekiWindow.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+-------------------------------
//	�e�L�E�B���h�E
//+-------------------------------

TekiWindow::TekiWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/CommandList.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/SoundSettingWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//�I���A�C�R���̒���
	m_iSelectLength = 53;

	// �I�v�V�����E�B���h�E�ɑ��݂���A�C�R�����l�߂Ă���
	AddIconData("���L�����b�V���@", "����ɋ߂Â��Ă���A���i�U��������B");
	AddIconData("���_�b�L���O�@", "�f�����X�e�b�v�ő���ɋ߂Â��B");
	AddIconData("���A�T���g�_�C�u", "�󒆂���^���ɑ���𗎂Ƃ��Ă����U���B");
	AddIconData("���R�[�N�X�N�����[", "�O���֓ːi���đ���𐁂���΂��܂��B");
	AddIconData("���J�E���^�[", "����������̍U����������΁A�؂�ւ����U�����܂��B");
	AddIconData("���T�C�N�����A�b�p�[", "��O�̑���������ł��グ��K�E�Z�B");

	AddCommandData("��", 365);
	AddCommandData("���{��", 365);
	AddCommandData("�󒆂Ł�", 365);
	AddCommandData("�@��Ɂ�", 365);
	AddCommandData("��", 365);
	AddCommandData("SP50�ȏ�Ň�", 365);

	// ���o
	//m_iAlpha = 0;

}

TekiWindow::~TekiWindow()
{

}


bool TekiWindow::Update()
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

	return true;// �N����
}

void TekiWindow::Redner()
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return;

	// �^�C�g��
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ����{�I�ȃE�B���h�E�̈ʒu
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		m_cSizeX, 64, 0, 0, m_cSizeX, 64);


	// ���g���u���b�N�P�ʂŕ`��
	int y = (int)(m_vPos.y + 64.0f);
	int x = (int)m_vPos.x + 18 + (255 - m_iAlpha);
	for (int i = 0; i < (int)m_aIconData.size(); i++)
	{

		// �Ō���Ȃ炵���ۂ�`��
		if (i == (int)m_aIconData.size() - 1)
		{
			m_pWindow->Render(WindowX, y,
				m_cSizeX, 46, 0, 64 + 40, m_cSizeX, 46);


		}
		else// ��
		{
			m_pWindow->Render(WindowX, y,
				m_cSizeX, 40, 0, 64, m_cSizeX, 40);
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
		for (int j = 0; j < 30; j++)
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

		// �R�}���h�̕���
		DWORD fontComCol = 0xff4e84e6;	

		if (m_iSelectNo == i)
		{
			// ��
			tdnFont::RenderString(m_aIconCommandData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
				27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);
		}
		else
		{
			// ��
			tdnFont::RenderString(m_aIconCommandData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
				27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);

		}

		// 40�s�N�Z�����炵�ĕ`�悵�Ă���
		y += 40;
	}

	//// ����
	//DWORD fontCol = 0xffffffff;
	////if (m_iSelectNo == i)fontCol = 0xff030a58;
	//tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS�n�p�p�޼��UB",// HGP�n�p��ھ�ݽEB
	//	24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
}

// ����
bool  TekiWindow::Ctrl(int DeviceID)
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

	//// ����
	//if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	//{
	//	// �I�������ԍ��i�[�I
	//	m_iChoiceState = (OPTION_STATE)m_iSelectNo;
	//	return true;
	//}

	//// �L�����Z����
	//// �Q�[���ɖ߂�X�e�[�g���i�[
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	//{
	//	// �I�������ԍ��i�[�I
	//	m_iChoiceState = (OPTION_STATE::BACK);
	//	return true;
	//}

	return false;

}

void TekiWindow::Action()
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

void TekiWindow::Stop()
{
	BaseWindow::Stop();

}
