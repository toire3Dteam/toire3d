#include "GameWindow.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+------------------------
//  �Q�[���E�B���h�E
//+------------------------

GameWindow::GameWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/GameOptionsWindow.png");

	//�I���A�C�R���̒���
	m_iSelectLength = 54;

	// �|�[�Y�E�B���h�E�ɑ��݂���A�C�R�����l�߂Ă���
	AddIconData("CPU��Փx", "CPU�̓�Փx�̕ύX���s���܂��B");
	AddIconData("�������E���h��", "�����������E���h����ݒ肵�܂��B");
	AddIconData("���Ԑ���", "1���E���h�̐������Ԃ�ݒ肵�܂��B");
	AddIconData("�����l�ɖ߂�", "�����ݒ�ɖ߂��܂��B");
	AddIconData("�߂�", "���̃E�B���h�E����܂��B");

	m_pDifficultyParam = new DifficultyParamSetting(1, 275);
	m_pRoundNumTypeParam = new RoundNumTypeParamSetting(2, 275);
	m_pRoundTimeTypeParam = new RoundTimeTypeParamSetting(3, 275);
	//// �X�s�[�h���x��
	//iSpeedLv = 0;
	//iLeftPushFrame = 0;
	//iRightPushFrame = 0;
}

GameWindow::~GameWindow()
{
	SAFE_DELETE(m_pDifficultyParam);
	SAFE_DELETE(m_pRoundNumTypeParam);
	SAFE_DELETE(m_pRoundTimeTypeParam);
}


bool GameWindow::Update()
{
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

	// �p�����[�^�[�X�V
	m_pDifficultyParam->Update();
	m_pRoundNumTypeParam->Update();
	m_pRoundTimeTypeParam->Update();

	return true;// �N����
}

void GameWindow::Redner()
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

		//+------------------------------------------------
		// �������߂�Ȃǂ͂����ʂ蕶�����̘g�Ɂ@����L��
		if (i == GameWindow::BACK|| i == GameWindow::RESET)
		{
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
		}else
		{
			// �p�����[�^�����n��Window�Ȃ̂Řg�͋ϓ���
			for (int j = 0; j < 25/*m_aIconData[i].iStringLength*/; j++)
			{
				m_pFontBox->Render(x + (j * 11) - 4, y, 32, 32, 0, 0, 32, 32);

				// �I�����Ă���^�X�N�Ȃ�
				if (m_iSelectNo == i)
				{
					m_pFontBox->Render(x + (j * 11) - 4, y,
						32, 32, 0, 32, 32, 32);
				}

			}
		}


		// ����
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		tdnFont::RenderString(m_aIconData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
			24, x, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);


		// �p�����[�^�[�֘A�̕`��
		RenderParamSetting(i, x + 300 , y);

		// for(�ς��)
		// if(i ==�@�p�����[�^�E�B���h�E�̔ԍ��ƍ����Ă�����)�@�`�� 
		// switch �ō����������̔ԍ��������Ă���Ώo��


		// 40�s�N�Z�����炵�ĕ`�悵�Ă���
		y += 40;
	}



#ifdef _DEBUG
	// 
	//tdnText::Draw(300, 100, 0xffff00aa, "BGM�p�����[�^�[%d",PlayerDataMgr->m_ConfigData.iBGMVolume);
	//tdnText::Draw(300, 150, 0xffff00aa, "SE�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iSEVolume);
	//tdnText::Draw(300, 200, 0xffff00aa, "VOICE�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iVoiceVolume);

	//tdnText::Draw(300, 250, 0xffff00aa, "�X�s�[�h���x��%d", iSpeedLv);
	//tdnText::Draw(300, 300, 0xffff00aa, "����������%d", iLeftPushFrame);
	//tdnText::Draw(300, 350, 0xffff00aa, "�E��������%d", iRightPushFrame);

#endif // _DEBUG



}

// ����
bool  GameWindow::Ctrl(int DeviceID)
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	// ���쉹
	CtrlSE(DeviceID);

	// (1212)�����Ă����ނƑO�̃X�e�[�g�Ƃ��񂩂��ĉ����Ď��i��->�����Ė߂邪�ł���
	// ����
	//if ((SOUND_STATE)m_iSelectNo == SOUND_STATE::BACK)
	//{
	//	// �Q�[���ɖ߂铮��͗����Đi��
	//	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
	//	{
	//		// �I�������ԍ��i�[�I
	//		m_iChoiceState = (SOUND_STATE)m_iSelectNo;
	//		return true;
	//	}
	//}
	//else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (GAME_STATE)m_iSelectNo;
		
			// �����Z�b�g���������Ƃ�
			if (m_iChoiceState == (GAME_STATE::RESET))
			{
				// ����{�^���ŏ����ݒ�ɖ߂��܂��B
				PlayerDataMgr->m_ConfigData.iRoundNumType = (int)ROUND_NUM_TYPE::ROUND_2;
				PlayerDataMgr->m_ConfigData.iRoundTimeType = (int)ROUND_TIME_TYPE::SEC_99;
				PlayerDataMgr->m_ConfigData.iDifficultyAI = (int)AI_TYPE::CPU_NORMAL;
			}

			return true;
		}
	}

	// �L�����Z����
	// �Q�[���ɖ߂�X�e�[�g���i�[
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		// �I�������ԍ��i�[�I
		m_iChoiceState = (GAME_STATE::BACK);
		return true;
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, DeviceID) == 3)
	{
		// �I�������ԍ��i�[�I
		//m_iChoiceState = (BATTLE_STATE::BACK);
		return true;
	}


	// ����������ĂȂ���
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0)
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


		// �I�����Ă�p�����[�^�̑���
		CtrlParamSetting(m_iSelectNo,DeviceID);
	}


	return false;

}


void GameWindow::Action()
{
	BaseWindow::Action();

	// �����x���ŏ��͓�����
	m_iAlpha = 0;
	m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V
}

void GameWindow::Stop()
{
	BaseWindow::Stop();
}

void GameWindow::RenderParamSetting(int No, int x, int y)
{
	bool bSelect = false;
	if (No == m_iSelectNo)bSelect = true;


	switch ((GAME_STATE)No)
	{
	case GameWindow::DIFFICULTY:
		m_pDifficultyParam->Render(x, y, bSelect);
		break;
	case GameWindow::GAME_STATE::ROUND:
		m_pRoundNumTypeParam->Render(x, y, bSelect);
		break;
	case GameWindow::GAME_STATE::TIME:
		m_pRoundTimeTypeParam->Render(x, y, bSelect);
		break;
	case GameWindow::BACK:
		break;
	default:
		break;
	}


}

void GameWindow::CtrlParamSetting(int SelectNo, int DeviceID)
{
	switch ((GAME_STATE)SelectNo)
	{
	case GameWindow::DIFFICULTY:
		m_pDifficultyParam->Ctrl(DeviceID);
		break;
	case GameWindow::GAME_STATE::ROUND:
		m_pRoundNumTypeParam->Ctrl(DeviceID);
		break;
	case GameWindow::GAME_STATE::TIME:
		m_pRoundTimeTypeParam->Ctrl(DeviceID);
		break;
	case GameWindow::RESET:

		break;
	case GameWindow::BACK:
		
		break;
	default:
		break;
	}
}

