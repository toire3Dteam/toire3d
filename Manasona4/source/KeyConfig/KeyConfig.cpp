#include "KeyConfig.h"
#include "Sound\SoundManager.h"

//+--------------------------------------------
//	�L�[�R���t�B�O
//+--------------------------------------------

KeyConfig::KeyConfig()
{
	m_pWindow = new tdn2DObj("Data/UI/Window/KeyConfigWindow.png");
	
	//�I���A�C�R��
	m_pSelect = new tdn2DAnim("Data/UI/Window/fontBox.png");
	m_pSelect->OrderJump(6, 1, 0.15f);
	m_iSelectLength = 34;

	m_vPos = VECTOR2_ZERO;
	m_bActive = false;
	m_iSelectNo = 0;
	m_iChoiceState = CHOICE_STATE::HOLD;

	m_aIconData.clear();
	m_aIconData.reserve(64);

	// ���݂���p�����[�^���ǉ�
	AddIconData("�߂�");
	AddIconData("���U��");
	AddIconData("���X�L��");
	AddIconData("�~����");
	AddIconData("�����");

	AddIconData("���K�E");
	AddIconData("�����G");
	AddIconData("���p�[�g�i�[");
	AddIconData("�����i");
	AddIconData("���o�[�X�g");

	AddIconData("���X�^�[�g");
	AddIconData("���Z���N�g");

	// ���߂�h�~
	m_bBackPush = false;

	// �ʒu�������p
	m_iAbjustHeight = -1;

	m_iDeviceID = -1;
}

KeyConfig::~KeyConfig()
{
	SAFE_DELETE(m_pWindow);
	SAFE_DELETE(m_pSelect);

	m_aIconData.clear();
}

bool KeyConfig::Update()
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	m_pSelect->Update();

	return true;
}

void KeyConfig::Render()
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return;

	// �^�C�g��
	int WindowX = (int)m_vPos.x + (255 /*- m_iAlpha*/);// ����{�I�ȃE�B���h�E�̈ʒu
	m_pWindow->Render(WindowX, (int)m_vPos.y,
		SizeX, 64, 0, 0, SizeX, 64);


	// ���g���u���b�N�P�ʂŕ`��
	int y = (int)(m_vPos.y + 64.0f) + m_iAbjustHeight;
	int x = (int)m_vPos.x + 18 + (255 /*- m_iAlpha*/);
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
			//m_pFontBox->Render(x + (j * 11) - 4, y, 32, 32, 0, 0, 32, 32);
			//
			//// �I�����Ă���^�X�N�Ȃ�
			//if (m_iSelectNo == i)
			//{
			//	m_pFontBox->Render(x + (j * 11) - 4, y,
			//		32, 32, 0, 32, 32, 32);
			//}


		}

		// ����
		DWORD fontCol = 0xff030a58;
		//if (m_iSelectNo == i)fontCol = 0xff030a58;
		//tdnFont::RenderString(m_aIconData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
		//	24, x, y + 2, ARGB(/*m_iAlpha*/255, 255, 255, 255
		//		), RS::COPY);


		if (m_iSelectNo == i)
		{
			// ��
			//tdnFont::RenderString(m_aIconCommandData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
			//	27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 3, 10, 88), RS::COPY);
			tdnFont::RenderString(m_aIconData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
				24, x, y + 2, ARGB(/*m_iAlpha*/255, 3, 10, 88), RS::COPY);
		}
		else
		{
			// ��
			//tdnFont::RenderString(m_aIconCommandData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
			//	27, x + m_aIconCommandData[i].iWidthSize, y + 2, ARGB(m_iAlpha, 255, 255, 255), RS::COPY);
			
			tdnFont::RenderString(m_aIconData[i].pString, "HG�޼��E",// HGP�n�p��ھ�ݽEB
				24, x, y + 2, ARGB(/*m_iAlpha*/255, 255, 255, 255), RS::COPY);

		}

		if (i != (int)KEY_CONFIG_STATE::BACK)
		{
			std::string l_strSting = "";
			l_strSting = std::to_string(*GetKeyConfigButton((KEY_CONFIG_STATE)i));

			tdnFont::RenderString(l_strSting.c_str(), "HG�޼��E",// HGP�n�p��ھ�ݽEB
				24, x + 240, y + 2, ARGB(255, 255, 255, 255), RS::COPY);
		}

		// 40�s�N�Z�����炵�ĕ`�悵�Ă���
		y += 40 + m_iAbjustHeight;
	}

}

bool KeyConfig::Ctrl()
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;


	if (tdnInput::KeyGet(KEYCODE::KEY_A, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_A);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_B, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_B);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_C, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_C);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_D, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_D);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_L1, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_L1);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_L2, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_L2);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_R1, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_R1);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_R2, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_R2);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_R3, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_R3);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_START, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_START);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_SELECT, m_iDeviceID) == 3)
	{
		SetConfig(KEYCODE::KEY_SELECT);
	}


	// ����܂��͖߂��
	if ((KEY_CONFIG_STATE)m_iSelectNo == KEY_CONFIG_STATE::BACK)
	{
		// �Q�[���ɖ߂铮��͗����Đi��
		if (tdnInput::KeyGet(KEYCODE::KEY_B, m_iDeviceID) == 3 ||
			tdnInput::KeyGet(KEYCODE::KEY_A, m_iDeviceID) == 3)
		{
			// �I�������ԍ��i�[�I
			m_iChoiceState = (KEY_CONFIG_STATE)m_iSelectNo;
			return true;
		}
	}
	

	// ����������ĂȂ���
	//if (tdnInput::KeyGet(KEYCODE::KEY_B, m_iDeviceID) == 0 &&
	//	tdnInput::KeyGet(KEYCODE::KEY_A, m_iDeviceID) == 0)
	{


		// �I��؂�ւ�
		if (tdnInput::KeyGet(KEYCODE::KEY_UP, m_iDeviceID) == 3)
		{
			m_iSelectNo--;
			// �I�����̉��o
			//m_pSelect->Action();
			// �Ō�֐i��
			if (m_iSelectNo <= -1)
			{
				m_iSelectNo = ((int)m_aIconData.size() - 1);
			}
			se->Play("�J�[�\��1");
		}
		if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, m_iDeviceID) == 3)
		{
			m_iSelectNo++;
			// �I�����̉��o
			//m_pSelect->Action();
			// �ŏ��ɖ߂�
			if (m_iSelectNo >= (int)m_aIconData.size())
			{
				m_iSelectNo = 0;
			}
			se->Play("�J�[�\��1");
		}

		// �I�����Ă�p�����[�^�̑���
		//CtrlParamSetting(m_iSelectNo, DeviceID);

	
	}

	return false;


}

void KeyConfig::Action(Vector2 vPos, int iDeviceID)
{
	// ���݂̃R���g���[���[�̃R���t�B�O�̃f�[�^���Ƃ��Ă���
	memcpy_s(&m_tagConfigData, sizeof(KEY_CONFIG_DATA), tdnInputManager::GetConfigDatas(iDeviceID), sizeof(KEY_CONFIG_DATA));
	memcpy_s(&m_tagOrgConfigData, sizeof(KEY_CONFIG_DATA), tdnInputManager::GetConfigDatas(iDeviceID), sizeof(KEY_CONFIG_DATA));

	m_vPos = vPos;
	m_iDeviceID = iDeviceID;

	m_iChoiceState = CHOICE_STATE::HOLD;
	// �E�B���h�E�N��
	m_bActive = true;
}

void KeyConfig::Stop()
{
	// �E�B���h�E�I��
	m_bActive = false;
	m_iChoiceState = CHOICE_STATE::HOLD;// (12/12) �����ɂ�HOLD�̏���

	// �L�[�R���t�B�O����ۑ��I
	tdnInputManager::SetConfig(m_tagConfigData, m_iDeviceID);

	m_iDeviceID = -1;

}

void KeyConfig::AddIconData(LPSTR string)
{
	IconData data;
	data.pString = string;
	//data.pInfoString = infoString;

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

void KeyConfig::SetConfig(KEYCODE PushedKey)
{
	BYTE *l_pSelectKey = GetKeyConfigButton((KEY_CONFIG_STATE)m_iSelectNo);

	// �l�����ĂȂ�������(�߂�{�^��)�o�Ă������I�I
	if (!l_pSelectKey) return;

	// L1-R1-L2-R2-L3-R3�@���@L1-L2-L3-R1-R2-R3
	BYTE Push = m_tagOrgConfigData.tagPadSet.AllDatas[(BYTE)PushedKey];
	switch (PushedKey)
	{
	case KEYCODE::KEY_L1:
		Push = m_tagOrgConfigData.tagPadSet.L1;
		break;
	case KEYCODE::KEY_L2:
		Push = m_tagOrgConfigData.tagPadSet.L2;
		break;
	case KEYCODE::KEY_L3:
		Push = m_tagOrgConfigData.tagPadSet.L3;
		break;
	case KEYCODE::KEY_R1:
		Push = m_tagOrgConfigData.tagPadSet.R1;
		break;
	case KEYCODE::KEY_R2:
		Push = m_tagOrgConfigData.tagPadSet.R2;
		break;
	case KEYCODE::KEY_R3:
		Push = m_tagOrgConfigData.tagPadSet.R3;
		break;
	}

	// �������L�[�Ɛݒ肳��Ă��铯���{�^����������o�Ă������I�I
	if (Push == *l_pSelectKey) return;

	//BYTE a(tdnInputManager::GetConfigDatas(m_iDeviceID)->tagPadSet.AllDatas[(BYTE)PushedKey]), b(*l_pSelectKey), c(m_tagConfigData.tagPadSet.AllDatas[(BYTE)PushedKey]);

	// �L�[����
	for (int i = 4; i < 16; i++)
	{
		if (Push == m_tagConfigData.tagPadSet.AllDatas[i])
		{
			// �O�̂�Ɠ���ւ���
			BYTE temp = *l_pSelectKey;
			*l_pSelectKey = Push;
			m_tagConfigData.tagPadSet.AllDatas[i] = temp;
		}
	}

	se->Play("����1");
}

BYTE *KeyConfig::GetKeyConfigButton(KEY_CONFIG_STATE eState)
{
	switch (eState)
	{
	case KeyConfig::ATTACK:
		return &m_tagConfigData.tagPadSet.C;
		break;
	case KeyConfig::SKILL:
		return &m_tagConfigData.tagPadSet.D;
		break;
	case KeyConfig::THROW:
		return &m_tagConfigData.tagPadSet.A;
		break;
	case KeyConfig::ESCAPE:
		return &m_tagConfigData.tagPadSet.B;
		break;
	case KeyConfig::HEAVEHO_DRIVE:
		return &m_tagConfigData.tagPadSet.R2;
		break;
	case KeyConfig::INVINCIBLE:
		return &m_tagConfigData.tagPadSet.L2;
		break;
	case KeyConfig::PARTNER:
		return &m_tagConfigData.tagPadSet.L1;
		break;
	case KeyConfig::DOKKOI:
		return &m_tagConfigData.tagPadSet.R1;
		break;
	case KeyConfig::OVER_DRIVE:
		return &m_tagConfigData.tagPadSet.R3;
		break;
	case KeyConfig::START:
		return &m_tagConfigData.tagPadSet.START;
		break;
	case KeyConfig::SELECT:
		return &m_tagConfigData.tagPadSet.SELECT;
		break;
	default:
		return nullptr;
		break;
	}
}
