#include "TrainingPauseWindow.h"

#include "PauseWindow.h"

//+------------------------
//  �`���[�g���A���|�[�Y�E�B���h�E
//+------------------------

TrainingPauseWindow::TrainingPauseWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/PauseWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/PauseWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//�I���A�C�R���̒���
	m_iSelectLength = 34;

	// �|�[�Y�E�B���h�E�ɑ��݂���A�C�R�����l�߂Ă���
	//AddIconData("�O�̃`���[�g���A����", "��O�̃`���[�g���A���ɖ߂�܂��B");
	AddIconData("�g���[�j���O�ݒ�", "�g���[�j���O�̊�{�ݒ���s���܂��B");
	AddIconData("�g���[�j���O�_�~�[�ݒ�", "�g���[�j���O�_�~�[�̐ݒ���s���܂��B");
	AddIconData("�|�W�V�������Z�b�g","�|�W�V������������Ԃɖ߂��܂��B");
	AddIconData("�R�}���h���X�g", "���삵�Ă���L�����N�^�[�̃R�}���h���X�g���m�F���܂��B");
	AddIconData("�T�E���h�ݒ�", "�e��T�E���h�̐ݒ���s���܂��B");
	AddIconData("���j���[��\��", "���j���[��ʂ��\���ɂ��܂��B");
	AddIconData("�L�����N�^�[�Z���N�g", "�L�����N�^�[�Z���N�g�ɖ߂�܂��B");
	AddIconData("���C�����j���[", "���C�����j���[�ɖ߂�܂��B");
	AddIconData("�߂�", "���̃E�B���h�E����A�Q�[���ɖ߂�܂��B");

	// ���o
	//m_iAlpha = 0;
	//m_iAddX = 0;
	//m_iAddY = 0;

	m_pInfoPlate = new InformationPlate();


	// ���߂�h�~
	m_bBackPush = false;
}

TrainingPauseWindow::~TrainingPauseWindow()
{
	SAFE_DELETE(m_pInfoPlate);

}


bool TrainingPauseWindow::Update()
{
	// �����̉��ɏ悹���
	m_pInfoPlate->Update();

	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	// �X�V�����ق��̎q��Window���N�����Ă�����
	// ����ł��Ȃ��悤�ɂ���B

	m_pSelect->Update();

	// �����x�X�V
	m_iAlpha += 255 / 4;
	m_iAlpha = min(255, m_iAlpha);

	m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V							 // �����x�X�V
	m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V


	return true;// �N����
}

void TrainingPauseWindow::Redner()
{
	// �����̉��ɏ悹���
	m_pInfoPlate->Render();

	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return;

	// �^�C�g��
	int WindowX = (int)m_vPos.x + (255 - m_iAlpha);// ����{�I�ȃE�B���h�E�̈ʒu
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// ���g���u���b�N�P�ʂŕ`��
	int y = (int)(m_vPos.y + 64.0f);
	int x = (int)m_vPos.x + 18 + (255 - m_iAlpha);
	for (int i = 0; i < (int)m_aIconData.size(); i++)
	{

		// �Ō���Ȃ炵���ۂ�`��
		if (i == (int)m_aIconData.size() - 1)
		{
			m_pWindow->Render(WindowX, y,
				SizeX, 46, 0, 64 + 40, SizeX, 46);
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
			m_pFontBox->Render(x + (j * 12) - 4, y, 32, 32, 0, 0, 32, 32);

			// �I�����Ă���^�X�N�Ȃ�
			if (m_iSelectNo == i)
			{
				m_pFontBox->Render(x + (j * 12) - 4, y,
					32, 32, 0, 32, 32, 32);
			}

		}

		// ����
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);


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
bool  TrainingPauseWindow::Ctrl(int DeviceID)
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	// ���쉹
	CtrlSE(DeviceID);

	// ����@�Q�[���ɖ߂�I�����Ȃ�Η����Đi�ނ悤��
	if ((TRAINING_PAUSE_STATE)m_iSelectNo == TRAINING_PAUSE_STATE::BACK || 
		(TRAINING_PAUSE_STATE)m_iSelectNo == TRAINING_PAUSE_STATE::POSITION_RESET)
	{
		// �Q�[���ɖ߂铮��͗����Đi��
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (TRAINING_PAUSE_STATE)m_iSelectNo;
			return true;
		}
	}
	else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (TRAINING_PAUSE_STATE)m_iSelectNo;
			return true;
		}
	}

	// �L�����Z���{�^�����������ޓ��삪�K�v��
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3||
		tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 3)
	{
		m_bBackPush = true;
	}

	// �L�����Z���{�^��
	if (m_bBackPush == true)// ��x�L�����Z���{�^������������ł�����
	{
		// �Q�[���ɖ߂铮��͗����Đi��
		if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 2)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (TRAINING_PAUSE_STATE::BACK);
			return true;
		}
		if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 2)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (TRAINING_PAUSE_STATE::BACK);
			return true;
		}
	}

	// B�ł��ǂ�����肹���ƂɂȂ�
	//	�Ƃ�܊���
	//	��������Window�̐ݒ�Ƃ�
	//	�Z�[�u�f�[�^�Ƀg���[�j���O�悤�̂Ł[�������Ă��炤

	// �����L�����Z���������ĂȂ���
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0 && 
	tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 0)
	{

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

	}


	return false;

}

void TrainingPauseWindow::Action()
{
	if (m_bActive == false)// �����E�B���h�E���o�Ă���Ƃ��͉��K���n�W�N
	{
		// �����x���ŏ��͓�����
		m_iAlpha = 0;
		m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V
		m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
		m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V

		m_pInfoPlate->Action();// (TODO) �x�[�X��
	}

	BaseWindow::Action();

	// ���߂�h�~
	m_bBackPush = false;
}

void TrainingPauseWindow::Stop()
{
	BaseWindow::Stop();

	m_pInfoPlate->Stop();

	// ���߂�h�~
	m_bBackPush = false;
}