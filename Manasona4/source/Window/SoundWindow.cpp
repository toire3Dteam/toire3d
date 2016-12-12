#include "SoundWindow.h"
#include "Data\PlayerData.h"
#include "Sound\SoundManager.h"

//+------------------------
//  �T�E���h�E�B���h�E
//+------------------------

SoundWindow::SoundWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/PauseWindow.png");
	//m_pTop =    new tdn2DAnim("Data/UI/Window/PauseWindow.png");
	//m_pMiddle = new tdn2DAnim("");
	//m_pBottom = new tdn2DAnim("");
	//m_pSelect = new tdn2DAnim("");

	//�I���A�C�R���̒���
	m_iSelectLength = 34;

	// �|�[�Y�E�B���h�E�ɑ��݂���A�C�R�����l�߂Ă���
	AddIconData("BGM", "BGM�̒������s���܂��B");
	AddIconData("SE" , "SE�̒������s���܂��B");
	AddIconData("VOICE", "VOICE�̒������s���܂��B");
	AddIconData("�߂�", "���̃E�B���h�E����܂��B");

	// ���o
	m_iAlpha = 0;
	m_iAddX = 0;
	m_iAddY = 0;

	// �X�s�[�h���x��
	iSpeedLv = 0;
	iLeftPushFrame = 0;
	iRightPushFrame = 0;
}

SoundWindow::~SoundWindow()
{

}


bool SoundWindow::Update()
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


	return true;// �N����
}

void SoundWindow::Redner()
{
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
		y += 40;
	}

	// ����
	DWORD fontCol = 0xffffffff;
	//if (m_iSelectNo == i)fontCol = 0xff030a58;
	tdnFont::RenderStringCentering(m_aIconData[m_iSelectNo].pInfoString, "HGS�n�p�p�޼��UB",// HGP�n�p��ھ�ݽEB
		24, 1280 / 2 + (255 - m_iAlpha), 652, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);


#ifdef _DEBUG
	// 
	tdnText::Draw(300, 100, 0xffff00aa, "BGM�p�����[�^�[%d",PlayerDataMgr->m_ConfigData.iBGMVolume);
	tdnText::Draw(300, 150, 0xffff00aa, "SE�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iSEVolume);
	tdnText::Draw(300, 200, 0xffff00aa, "VOICE�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iVoiceVolume);

	tdnText::Draw(300, 250, 0xffff00aa, "�X�s�[�h���x��%d", iSpeedLv);
	tdnText::Draw(300, 300, 0xffff00aa, "����������%d", iLeftPushFrame);
	tdnText::Draw(300, 350, 0xffff00aa, "�E��������%d", iRightPushFrame);

#endif // _DEBUG



}

// ����
bool  SoundWindow::Ctrl(int DeviceID)
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;


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
			m_iChoiceState = (SOUND_STATE)m_iSelectNo;
			return true;
		}
	}

	// �L�����Z��
	// �Q�[���ɖ߂铮��͗����Đi��
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 2)
	{
		// �I�������ԍ��i�[�I
		m_iChoiceState = (SOUND_STATE::BACK);
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

		//+------------------------------------------------- 
		//	�T�E���h�̃{�����[���؂�ւ�
		//+-------------------------------------------------
		
		// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b�̂��߂̏���
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3||
			tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 1)
		{
			iLeftPushFrame++;
			if (iSpeedLv == 0 && iLeftPushFrame >= 12)iSpeedLv = 1;
			if (iSpeedLv == 1 && iLeftPushFrame >= 77)iSpeedLv = 2;
		
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3 ||
				 tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 1)
		{
			iRightPushFrame++;
			if (iSpeedLv == 0 && iRightPushFrame >= 12)iSpeedLv = 1;
			if (iSpeedLv == 1 && iRightPushFrame >= 77)iSpeedLv = 2;		
		
		}else
		{
			iLeftPushFrame = 0;
			iRightPushFrame = 0;
			iSpeedLv = 0;
		}

		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3 || 
			(iLeftPushFrame % ((12 + iSpeedLv) - (iSpeedLv * 6)) == 0 && iLeftPushFrame != 0))
		{
			// �T�E���h�X�e�[�g
			switch ((SOUND_STATE)m_iSelectNo)
			{
			case SoundWindow::BGM:
				PlayerDataMgr->m_ConfigData.iBGMVolume--;

				break;
			case SoundWindow::SE:
				PlayerDataMgr->m_ConfigData.iSEVolume--;

				break;
			case SoundWindow::VOICE:
				PlayerDataMgr->m_ConfigData.iVoiceVolume--;

				break;
			case SoundWindow::BACK:
				
				break;
			default:
				break;
			}
			
			// -1�ȉ��ɂ����Ȃ�
			PlayerDataMgr->m_ConfigData.iBGMVolume = max(0, PlayerDataMgr->m_ConfigData.iBGMVolume);
			PlayerDataMgr->m_ConfigData.iSEVolume = max(0, PlayerDataMgr->m_ConfigData.iSEVolume);
			PlayerDataMgr->m_ConfigData.iVoiceVolume = max(0, PlayerDataMgr->m_ConfigData.iVoiceVolume);

			// �I�����̉��o
			m_pSelect->Action();

			// �p�����[�^�[������Ă鉹 
			se->Play("�J�[�\��2");

			// �T�E���h�ɔ��f
			SoundManager::SetBaseVolume(PlayerDataMgr->m_ConfigData.iBGMVolume, PlayerDataMgr->m_ConfigData.iSEVolume, PlayerDataMgr->m_ConfigData.iVoiceVolume);


		}
		if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3 ||
			(iRightPushFrame % ((12 + iSpeedLv) - (iSpeedLv * 6)) == 0 && iRightPushFrame != 0))
		{
			// �T�E���h�X�e�[�g
			switch ((SOUND_STATE)m_iSelectNo)
			{
			case SoundWindow::BGM:
				PlayerDataMgr->m_ConfigData.iBGMVolume++;

				break;
			case SoundWindow::SE:
				PlayerDataMgr->m_ConfigData.iSEVolume++;

				break;
			case SoundWindow::VOICE:
				PlayerDataMgr->m_ConfigData.iVoiceVolume++;

				break;
			case SoundWindow::BACK:

				break;
			default:
				break;
			}

			// 100�ȏ�ɂ����Ȃ�
			PlayerDataMgr->m_ConfigData.iBGMVolume = min(100, PlayerDataMgr->m_ConfigData.iBGMVolume);
			PlayerDataMgr->m_ConfigData.iSEVolume = min(100, PlayerDataMgr->m_ConfigData.iSEVolume);
			PlayerDataMgr->m_ConfigData.iVoiceVolume = min(100, PlayerDataMgr->m_ConfigData.iVoiceVolume);

			// �I�����̉��o
			m_pSelect->Action();

			// �T�E���h�ɔ��f
			SoundManager::SetBaseVolume(PlayerDataMgr->m_ConfigData.iBGMVolume, PlayerDataMgr->m_ConfigData.iSEVolume, PlayerDataMgr->m_ConfigData.iVoiceVolume);

		}
	}


	return false;

}

void SoundWindow::Action()
{
	BaseWindow::Action();

	// �����x���ŏ��͓�����
	m_iAlpha = 0;
	m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V
}

void SoundWindow::Stop()
{
	BaseWindow::Stop();
}

