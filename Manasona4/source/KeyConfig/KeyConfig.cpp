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
	AddIconData("���U��");
	AddIconData("���X�L��");
	AddIconData("�~����");
	AddIconData("�����");

	AddIconData("���K�E");
	AddIconData("�����G");
	AddIconData("���p�[�g�i�[");
	AddIconData("�����i");
	AddIconData("���o�[�X�g");

	// ���߂�h�~
	m_bBackPush = false;

	// �ʒu�������p
	m_iAbjustHeight = -1;

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


		// 40�s�N�Z�����炵�ĕ`�悵�Ă���
		y += 40 + m_iAbjustHeight;
	}

}

bool KeyConfig::Ctrl(int DeviceID)
{
	// �N�����Ă��Ȃ�������n�W��
	if (m_bActive == false)return false;

	// 
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


	// ����������ĂȂ���
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 0)
	{

		// �I��؂�ւ�
		if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
		{
			m_iSelectNo--;
			// �I�����̉��o
			//m_pSelect->Action();
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
			//m_pSelect->Action();
			// �ŏ��ɖ߂�
			if (m_iSelectNo >= (int)m_aIconData.size())
			{
				m_iSelectNo = 0;
			}
		}

		// �I�����Ă�p�����[�^�̑���
		//CtrlParamSetting(m_iSelectNo, DeviceID);

	
	}

	return false;


}

void KeyConfig::Action(Vector2 vPos)
{
	m_vPos = vPos;

	m_iChoiceState = CHOICE_STATE::HOLD;
	// �E�B���h�E�N��
	m_bActive = true;
}

void KeyConfig::Stop()
{
	// �E�B���h�E�I��
	m_bActive = false;
	m_iChoiceState = CHOICE_STATE::HOLD;// (12/12) �����ɂ�HOLD�̏���
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
