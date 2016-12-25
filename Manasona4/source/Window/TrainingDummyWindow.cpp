#include "TrainingDummyWindow.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "Sound\SoundManager.h"

//+-----------------------------------
//  �g���[�j���O�_�~�[�E�B���h�E
//+-----------------------------------

TrainingDummyWindow::TrainingDummyWindow(Vector2 vPos) :BaseWindow(vPos)
{
	m_pWindow = new tdn2DObj("Data/UI/Window/TrainingOption.png");

	//�I���A�C�R���̒���
	m_iSelectLength = 70;

	// �|�[�Y�E�B���h�E�ɑ��݂���A�C�R�����l�߂Ă���
	AddIconData("����̏��", "����̏�Ԃ�ݒ肵�܂��B");
	AddIconData("�K�[�h�̊�{�ݒ�", "�g���[�j���O�_�~�[�̃K�[�h������ݒ肵�܂��B");
	AddIconData("�K�[�h�̎����؂�ւ�", "�����K�[�h�Ƃ��Ⴊ�݃K�[�h�̎����؂�ւ���ݒ肵�܂��B");
	AddIconData("�󂯐g", "�g���[�j���O�_�~�[�̎󂯐g�̐ݒ���s���܂��B");
	AddIconData("��������", "�g���[�j���O�_�~�[�̓���������ݒ肵�܂��B");;
	
	AddIconData("�����l�ɖ߂�", "�����ݒ�ɖ߂��܂��B");
	AddIconData("�߂�", "���̃E�B���h�E����܂��B");


	m_pEStateParam = new EnemyStateParamSetting(1, 400);
	m_pEGuardParam = new EnemyGuardParamSetting(2, 400);
	m_pEGuardSwitchParam = new EnemyGuardSwitchParamSetting(3, 400);
	m_pETechParam = new TechParamSetting(4, 400);
	m_pEThrowTechParam = new ThrowTechParamSetting(5, 400);

}

TrainingDummyWindow::~TrainingDummyWindow()
{
	SAFE_DELETE(m_pEStateParam);
	SAFE_DELETE(m_pEGuardParam);
	SAFE_DELETE(m_pEGuardSwitchParam);
	SAFE_DELETE(m_pETechParam);
	SAFE_DELETE(m_pEThrowTechParam);

}


bool TrainingDummyWindow::Update()
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
	m_pEStateParam->Update();
	m_pEGuardParam->Update();
	m_pEGuardSwitchParam->Update();
	m_pETechParam->Update();
	m_pEThrowTechParam->Update();
	
		
	return true;// �N����
}

void TrainingDummyWindow::Redner()
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
		if (i == TrainingDummyWindow::BACK || i == TrainingDummyWindow::RESET)
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
		}
		else
		{
			// �p�����[�^�����n��Window�Ȃ̂Řg�͋ϓ���
			for (int j = 0; j < 28/*m_aIconData[i].iStringLength*/; j++)
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
		RenderParamSetting(i, x + 330, y);

		// for(�ς��)
		// if(i ==�@�p�����[�^�E�B���h�E�̔ԍ��ƍ����Ă�����)�@�`�� 
		// switch �ō����������̔ԍ��������Ă���Ώo��


		// 40�s�N�Z�����炵�ĕ`�悵�Ă���
		y += 40;
	}



#ifdef _DEBUG
	// 
	//tdnText::Draw(300, 100, 0xffff00aa, "BGM�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iBGMVolume);
	//tdnText::Draw(300, 150, 0xffff00aa, "SE�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iSEVolume);
	//tdnText::Draw(300, 200, 0xffff00aa, "VOICE�p�����[�^�[%d", PlayerDataMgr->m_ConfigData.iVoiceVolume);

	//tdnText::Draw(300, 250, 0xffff00aa, "�X�s�[�h���x��%d", iSpeedLv);
	//tdnText::Draw(300, 300, 0xffff00aa, "����������%d", iLeftPushFrame);
	//tdnText::Draw(300, 350, 0xffff00aa, "�E��������%d", iRightPushFrame);

#endif // _DEBUG



}

// ����
bool  TrainingDummyWindow::Ctrl(int DeviceID)
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	// ���쉹
	CtrlSE(DeviceID);

	// (1212)�����Ă����ނƑO�̃X�e�[�g�Ƃ��񂩂��ĉ����Ď��i��->�����Ė߂邪�ł���
	// ����
	//if ((TRAINING_DUMMY_STATE)m_iSelectNo == TRAINING_DUMMY_STATE::BACK)
	//{
	//	// �Q�[���ɖ߂铮��͗����Đi��
	//	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 2)
	//	{
	//		// �I�������ԍ��i�[�I
	//		m_iChoiceState = (TRAINING_DUMMY_STATE)m_iSelectNo;
	//		return true;
	//	}
	//}
	//else
	{
		if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (TRAINING_DUMMY_STATE)m_iSelectNo;

			// �����Z�b�g���������Ƃ�
			if (m_iChoiceState == (TRAINING_DUMMY_STATE::RESET))
			{
				// ����{�^���ŏ����ݒ�ɖ߂��܂��B
				PlayerDataMgr->m_TrainingData.iEnemyState = (int)ENEMY_STATE_TYPE::STAND;
				PlayerDataMgr->m_TrainingData.iEnemyGuard = (int)ENEMY_GUARD_TYPE::NO;
				PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch = (int)ENEMY_GUARD_SWITCH_TYPE::OK;
				PlayerDataMgr->m_TrainingData.iEnemyTech = (int)ENEMY_TECH_TYPE::ALL;
				PlayerDataMgr->m_TrainingData.iEnemyThrowTech= (int)ENEMY_THROW_TECH_TYPE::NO;
			}

			return true;
		}
	}

	// �L�����Z����
	// �Q�[���ɖ߂�X�e�[�g���i�[
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		// �I�������ԍ��i�[�I
		m_iChoiceState = (TRAINING_DUMMY_STATE::BACK);
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
		CtrlParamSetting(m_iSelectNo, DeviceID);


	}


	return false;

}


void TrainingDummyWindow::Action()
{
	BaseWindow::Action();

	// �����x���ŏ��͓�����
	m_iAlpha = 0;
	m_pWindow->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pFontBox->SetAlpha(m_iAlpha);			// �����x�X�V
	m_pSelect->SetAlpha(m_iAlpha);			// �����x�X�V
}

void TrainingDummyWindow::Stop()
{
	BaseWindow::Stop();
}

void TrainingDummyWindow::RenderParamSetting(int No, int x, int y)
{
	bool bSelect = false;
	if (No == m_iSelectNo)bSelect = true;


	switch ((TRAINING_DUMMY_STATE)No)
	{
	case TRAINING_DUMMY_STATE::ENEMY_STATE:
		m_pEStateParam->Render(x, y, bSelect);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_GUARD:
		m_pEGuardParam->Render(x, y, bSelect);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_GUARD_SWITCH:
		m_pEGuardSwitchParam->Render(x, y, bSelect);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_TECH:
		m_pETechParam->Render(x, y, bSelect);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_THROW_TECH:
		m_pEThrowTechParam->Render(x, y, bSelect);
		break;
	case TRAINING_DUMMY_STATE::BACK:
		break;
	default:
		break;
	}


}

void TrainingDummyWindow::CtrlParamSetting(int SelectNo, int DeviceID)
{
	switch ((TRAINING_DUMMY_STATE)SelectNo)
	{
	case TRAINING_DUMMY_STATE::ENEMY_STATE:
		m_pEStateParam->Ctrl(DeviceID);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_GUARD:
		m_pEGuardParam->Ctrl(DeviceID);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_GUARD_SWITCH:
		m_pEGuardSwitchParam->Ctrl(DeviceID);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_TECH:
		m_pETechParam->Ctrl(DeviceID);
		break;
	case TRAINING_DUMMY_STATE::ENEMY_THROW_TECH:
		m_pEThrowTechParam->Ctrl(DeviceID);
		break;
	case TRAINING_DUMMY_STATE::BACK:
		break;
	default:
		break;
	}
}

