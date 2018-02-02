#include "ControllerSelectUI.h"

//����Ƃ̉�]�͂˂�����������������ׂ�����
//���낢�悱���񂢂���
ControllerSelectUI::ControllerSelectUI()
{
	m_pInfoBox = new tdn2DObj("Data/UI/Menu/Information.png");

	m_pInfo = new tdn2DAnim("Data/UI/Menu/Info/ControllerSelectinfo.png");
	m_pInfo->OrderMoveAppeared(12, 128, 592);

	// �����S����
	for (int i = 0; i < PAD_MAX; i++)
	{
		m_pPad[i].pPic = new tdn2DAnim("Data/UI/Menu/ControllerSelect/pad.png");
		m_pPad[i].pAroow = new tdn2DAnim("Data/UI/Menu/ControllerSelect/padAroow.png");
		m_pPad[i].vPos = Vector2((float)552, (float)(96 + (i * 64)));
		m_pPad[i].vNextPos = Vector2((float)552, (float)(96 + (i * 64)));
		m_pPad[i].vFirstPos = Vector2((float)552, (float)(96 + (i * 64)));
		m_pPad[i].pPic->OrderMoveAppeared(8, (int)(m_pPad[i].vPos.x + 128), (int)m_pPad[i].vPos.y);
		m_pPad[i].pAroow->OrderMoveAppeared(8, (int)(m_pPad[i].vFirstPos.x - 60), (int)m_pPad[i].vFirstPos.y);

	}

	// �M�A
	m_pGia.pPic = new tdn2DAnim("Data/UI/Menu/ControllerSelect/gia.png");
	m_pGia.pPic->OrderGrow(8, 0, (float)(1.0f / 8.0f));// ���ō��͂��̃I�[�_�[
	m_pGia.fAngle = 0.0f;
	m_pGia.fNextAngle = 0.0f;

	m_pGiaFont.pPic = new tdn2DAnim("Data/UI/Menu/ControllerSelect/giaFont.png");
	m_pGiaFont.pPic->OrderGrow(8, 0, (float)(1.0f / 8.0f));// ���ō��͂��̃I�[�_�[
	m_pGiaFont.fAngle = -2.5f;
	m_pGiaFont.fNextAngle = -2.5f;

	// �v���C���[�J�[�h
	m_pPlayerCardLeft.pPic=new tdn2DAnim("Data/UI/Menu/ControllerSelect/PlayerCard1.png");
	m_pPlayerCardLeft.pPic->OrderMoveAppeared(8, 96 - 384 , 128);
	m_pPlayerCardLeft.pFont = new tdn2DAnim("Data/UI/Menu/ControllerSelect/Player1CardFont.png");
	//m_pPlayerCardLeft.pFont->OrderMoveAppeared(8, 96, 128 - 64);
	//m_pPlayerCardLeft.pFont->OrderShake(8, 6, 24, 3);
	m_pPlayerCardLeft.pFont->OrderJump(6, 1, 0.4f);
	m_pPlayerCardLeft.bPlayerFlag = false;
	m_pPlayerCardLeft.iPlayerDeviceID = -1;

	m_pPlayerCardRight.pPic = new tdn2DAnim("Data/UI/Menu/ControllerSelect/PlayerCard2.png");
	m_pPlayerCardRight.pPic->OrderMoveAppeared(8, 800 + 384, 128);
	m_pPlayerCardRight.pFont = new tdn2DAnim("Data/UI/Menu/ControllerSelect/Player2CardFont.png");
	//m_pPlayerCardRight.pFont->OrderMoveAppeared(8, 800 , 128 - 64);
	m_pPlayerCardRight.pFont->OrderJump(6, 1, 0.4f);
	m_pPlayerCardRight.bPlayerFlag = false;
	m_pPlayerCardRight.iPlayerDeviceID = -1;

	// ��̂��
	m_pPlayerToPlayer= new tdn2DAnim("Data/UI/Menu/ControllerSelect/Player1orPlayer2.png");
	m_pPlayerToPlayer->OrderMoveAppeared(8, 384, -128);

	m_pTopUI = new tdn2DObj("Data/UI/Menu/ControllerSelect/TopUI.png");
	//m_pTopUI->OrderMoveAppeared(8, 384, -128);
}

ControllerSelectUI::~ControllerSelectUI()
{
	SAFE_DELETE(m_pInfo);

	for (int i = 0; i < PAD_MAX; i++)
	{
		SAFE_DELETE(m_pPad[i].pPic);
		SAFE_DELETE(m_pPad[i].pAroow);
	}

	SAFE_DELETE(m_pGia.pPic);
	SAFE_DELETE(m_pGiaFont.pPic);

	SAFE_DELETE(m_pInfoBox);

	// �v���C���[�J�[�h
	SAFE_DELETE(m_pPlayerCardLeft.pPic);
	SAFE_DELETE(m_pPlayerCardLeft.pFont);
	SAFE_DELETE(m_pPlayerCardRight.pPic);
	SAFE_DELETE(m_pPlayerCardRight.pFont);

	SAFE_DELETE(m_pPlayerToPlayer);
	SAFE_DELETE(m_pTopUI);

}

void ControllerSelectUI::Update()
{
	m_pInfo->Update();

	// PAD
	for (int i = 0; i < PAD_MAX; i++)
	{
		// �ɋ}�̂��铮��
		m_pPad[i].vPos = m_pPad[i].vNextPos * 0.3f + m_pPad[i].vPos* 0.7f;

		// 0.1�ȉ��̌덷�̓����𖳂�������
		if (0.9f >= abs(m_pPad[i].vPos.x - m_pPad[i].vNextPos.x) &&
			0.9f >= abs(m_pPad[i].vPos.y - m_pPad[i].vNextPos.y))
		{
			m_pPad[i].vPos = m_pPad[i].vNextPos;
		}


		m_pPad[i].pPic->Update();
		m_pPad[i].pAroow->Update();

	}

	m_pGia.fAngle -= 0.004f;
	m_pGia.pPic->SetAngle(m_pGia.fAngle);
	m_pGia.pPic->Update();
	
	// �ɋ}�̂��铮��
	m_pGiaFont.fAngle = (m_pGiaFont.fAngle*0.6f) + (m_pGiaFont.fNextAngle*0.4f);
		
	m_pGiaFont.pPic->SetAngle(m_pGiaFont.fAngle);
	m_pGiaFont.pPic->Update();

	// �v���C���[�J�[�h
	m_pPlayerCardLeft.pPic->Update();
	m_pPlayerCardLeft.pFont->Update();
	m_pPlayerCardRight.pPic->Update();
	m_pPlayerCardRight.pFont->Update();

	m_pPlayerToPlayer->Update();
}

void ControllerSelectUI::Render()
{
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);
	
	//m_pTopUI->Render(0, 0);
	
	// �M�A
	//.pPic->Render(896-32, 320 - 32);
	//m_pGiaFont.pPic->Render(896 - 32, 320 - 32);

	// �v���C���[�J�[�h
	m_pPlayerCardLeft.pPic->Render(96, 128, 384, 256, 0, m_pPlayerCardLeft.bPlayerFlag * 256, 384, 256);
	m_pPlayerCardLeft.pFont->Render(96, 128, 384, 256, 0, m_pPlayerCardLeft.bPlayerFlag * 256, 384, 256);

	m_pPlayerCardRight.pPic->Render(800, 128, 384, 256, 0, m_pPlayerCardRight.bPlayerFlag * 256, 384, 256);
	m_pPlayerCardRight.pFont->Render(800, 128, 384, 256, 0, m_pPlayerCardRight.bPlayerFlag * 256, 384, 256);

	static int a = 0;
	a++;
	static int b = 0;
	if (a >= 4)
	{
		a = 0;
		b++;
	}

	// �p�b�h���`��
	for (int i = 0; i < tdnInputManager::GetNumDevice(); i++)
	{
		m_pPad[i].pPic->Render((int)m_pPad[i].vPos.x, (int)m_pPad[i].vPos.y, 180, 48, 0, (48 * i), 180, 48);
		
		//if (IsLeftPlayer() == false && IsRightPlayer() == false)
		m_pPad[i].pAroow->Render((int)m_pPad[i].vPos.x-60, (int)m_pPad[i].vPos.y, 286, 48, (248 * b), 0, 248, 48);

	}

	// ������
	m_pInfoBox->Render(0, 592);
	m_pInfo->Render(0, 592);

	m_pPlayerToPlayer->Render(384, 0);
}

void ControllerSelectUI::Action()
{
	m_pInfo->Action();
	
	// �M�A������
	m_pGia.pPic->Action();

	m_pGiaFont.fAngle = -4.5f;
	m_pGiaFont.fNextAngle = 0.2f;
	m_pGiaFont.pPic->Action(0);

	// �v���C���[������
	m_pPlayerCardLeft.pPic->Action();
	m_pPlayerCardLeft.pFont->Action();
	m_pPlayerCardLeft.bPlayerFlag = false;
	m_pPlayerCardLeft.iPlayerDeviceID = -1;

	m_pPlayerCardRight.pPic->Action();
	m_pPlayerCardRight.pFont->Action();
	m_pPlayerCardRight.bPlayerFlag = false;
	m_pPlayerCardRight.iPlayerDeviceID = -1;

	// PAD
	for (int i = 0; i < PAD_MAX; i++)
	{
		m_pPad[i].vPos = Vector2((float)552, (float)(96 + (i * 64)));
		m_pPad[i].vNextPos = Vector2((float)552, (float)(96 + (i * 64)));
		m_pPad[i].pPic->Action(i * 4);
		m_pPad[i].pAroow->Action((i * 4) + 8);
	}

	// ���UI
	m_pPlayerToPlayer->Action();
}

void ControllerSelectUI::Stop()
{
	m_pInfo->Stop();
	
	for (int i = 0; i < PAD_MAX; i++)
	{
		m_pPad[i].pPic->Stop();
		m_pPad[i].pAroow->Stop();
	}

}

void ControllerSelectUI::PadMoveLeftSide(int padNo)
{
	// ���̃p�b�h���t�T�C�h�ɂ���Ȃ�܂��߂�
	if (m_pPlayerCardRight.iPlayerDeviceID == padNo)
	{
		PadMoveBack(padNo);
		return;
	}

	// �����v���C���[�������Ă���n�W�N
	if (m_pPlayerCardLeft.bPlayerFlag == true)return;

	// ���T�C�h�Ƀv���C���[������܂����B
	m_pPlayerCardLeft.bPlayerFlag = true;

	// Pad��ID��ۑ�
	m_pPlayerCardLeft.iPlayerDeviceID = padNo;
	
	// �����Ɉړ����Ă�
	m_pPad[padNo].vNextPos = Vector2(102, 224);

	// ���o
	m_pPlayerCardLeft.pFont->Action();

	m_pPad[padNo].pAroow->Stop();
}

void ControllerSelectUI::PadMoveRightSide(int padNo)
{
	// ���̃p�b�h���t�T�C�h�ɂ���Ȃ�܂��߂�
	if (m_pPlayerCardLeft.iPlayerDeviceID == padNo)
	{
		PadMoveBack(padNo);
		return;
	}

	// �����v���C���[�������Ă���n�W�N
	if (m_pPlayerCardRight.bPlayerFlag == true)return;

	// ���T�C�h�Ƀv���C���[������܂����B
	m_pPlayerCardRight.bPlayerFlag = true;

	// Pad��ID��ۑ�
	m_pPlayerCardRight.iPlayerDeviceID = padNo;

	// �����Ɉړ����Ă�
	m_pPad[padNo].vNextPos = Vector2(996, 224);

	// ���o
	m_pPlayerCardRight.pFont->Action();
	
	m_pPad[padNo].pAroow->Stop();
}

void ControllerSelectUI::PadMoveBack(int padNo)
{
	// ���������Ė߂�

	if (m_pPlayerCardLeft.iPlayerDeviceID == padNo)
	{
		m_pPlayerCardLeft.bPlayerFlag = false;
		m_pPlayerCardLeft.iPlayerDeviceID = -1;

		// �����Ɉړ����Ă�
		m_pPad[padNo].vNextPos = m_pPad[padNo].vFirstPos;
	}
	
	if (m_pPlayerCardRight.iPlayerDeviceID == padNo)
	{
		m_pPlayerCardRight.bPlayerFlag = false;
		m_pPlayerCardRight.iPlayerDeviceID = -1;

		// �����Ɉړ����Ă�
		m_pPad[padNo].vNextPos = m_pPad[padNo].vFirstPos;
	}

	// ���o
	m_pPad[padNo].pAroow->Action(8);
}

