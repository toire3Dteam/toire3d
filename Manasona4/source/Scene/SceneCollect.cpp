#include "TDNLIB.h"
#include "../Sound/SoundManager.h"
#include "system/FrameworkEx.h"
#include "../Data/PlayerData.h"
#include "SceneMenu.h"
#include "SceneCollect.h"
#include "../Fade/Fade.h"
#include "Trophy\TrophyManager.h"


//=============================================================================================
//		��	��	��	��	�J	��
sceneCollect::sceneCollect() :BaseGameEntity(ENTITY_ID::SCENE_COLLECT) {}

bool sceneCollect::Initialize()
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	m_fLoadPercentage = .5f;	// ���[�h����

	// �摜������
	m_pImages[IMAGE::BACK] = new tdn2DAnim("DATA/UI/Collect/bg.png");
	m_pImages[IMAGE::SCROLL_BAR] = new tdn2DAnim("DATA/UI/Collect/ScrollBar.png");
	m_pImages[IMAGE::SCROLL_ARROW] = new tdn2DAnim("DATA/UI/Collect/ScrollArrow.png");
	m_pImages[IMAGE::TITLE] = new tdn2DAnim("DATA/UI/Collect/title.png");
	m_pImages[IMAGE::INFO_PLATE] = new tdn2DAnim("DATA/UI/Collect/Information.png");
	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i]->OrderNone();
		m_pImages[i]->Action();
	}
	m_pImages[IMAGE::TITLE]->OrderMoveAppeared(16, 0, -200);
	m_pImages[IMAGE::INFO_PLATE]->OrderMoveAppeared(24, 0, 592 + (200 ));

	m_fLoadPercentage = 1.0f;	// ���[�h����

	m_pPlayerInfo = &PlayerDataMgr->m_PlayerInfo;

	bgm->PlayStreamIn("DATA/Sound/BGM/System/Collect.ogg");

	// �X�N���[���o�[
	m_iScrollHeight = 1440;
	m_iScrollScreenSize= 720/2;
	m_vScrollPos = VECTOR2_ZERO;
	m_vScrollPos.x = 1200;
	m_vScrollPos.y = 128;

	// �g���t�B�[�A�C�R���\����
	TrophyInit();

	// ����֘A
	m_iSpeedLv = 0;
	m_iUpPushFrame = 0;
	m_iDownPushFrame = 0;
	m_iLeftPushFrame = 0;
	m_iRightPushFrame = 0;

	m_bUpPush = false;
	m_bDownPush = false;
	m_bLeftPush = false;
	m_bRightPush = false;

	/* �X�e�[�g�}�V�������� */
	m_pStateMachine = new StateMachine<sceneCollect>(this);
	m_pStateMachine->SetCurrentState(SceneCollectState::Intro::GetInstance());	// �X�e�[�g�̐ݒ�




	return true;
}

sceneCollect::~sceneCollect()
{
	bgm->StopStreamIn();
	FOR(IMAGE::ARRAY_END) SAFE_DELETE(m_pImages[i]);
	
	SAFE_DELETE(m_pStateMachine);

	SAFE_DELETE(m_tagTI.pSelectMark);
	SAFE_DELETE(m_tagTI.pSelectMarkEdge);
}
//=============================================================================================


//=============================================================================================
//		�X			�V
void sceneCollect::Update()
{
	static bool bEnd(false);

	// �X�V
	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i]->Update();
	}

	// �t�F�[�h
	Fade::Update();


	//if (KEY(KEYCODE::KEY_DOWN, 0) == 1)
	//{
	//	m_iScrollPosY -= 10;
	//}

	//if (KEY(KEYCODE::KEY_UP, 0) == 1)
	//{
	//	m_iScrollPosY += 10;
	//}

	//if (KEY(KEYCODE::KEY_SPACE, 0) == 1)
	//{
	//	m_iScrollPosY = 720;
	//}
	//if (KEY(KEYCODE::KEY_C, 0) == 1)
	//{
	//	m_iScrollPosY = m_iScrollHeight;
	//}

	// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
	m_pStateMachine->Update();



#ifdef _DEBUG

	if (bEnd)
	{
		// �t�F�[�h�I�������A��
		if (Fade::isFadeStop())
		{
			bEnd = false;
			MainFrameEx->ChangeScene(new sceneMenu);
		}
	}
	// �X�y�[�X�L�[�Ŕ�����
	else if (KEY(KEYCODE::KEY_ENTER, 0) == 3)
	{
		//se->Play("�߂�");

		// �t�F�[�h�A�E�g�ݒ�
		Fade::Set(Fade::FLAG::FADE_OUT, 4);
		bEnd = true;
	}

#endif // _DEBUG


}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneCollect::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	// �w�i
	m_pImages[IMAGE::BACK]->Render(0,0);

	// tdnPolygon::Rect((int)(1280 * .25f) / 2, 96, (int)(1280 * .75f), (int)(720 * .75f), RS::COPY, 0xff808080);

	// �v���C��
	tdnText::Draw(400, 400, 0xffffffff, "�v���C��: %d", m_pPlayerInfo->PlayCount);

	// �v���C����
	const int minutes(m_pPlayerInfo->PlayTime % 60), hour(m_pPlayerInfo->PlayTime / 60);
	tdnText::Draw(400, 460, 0xffffffff, "�v���C����: %d���� %d��", hour, minutes);

	// �R�C��
	tdnText::Draw(400, 520, 0xffffffff, "�R�C��: %d", m_pPlayerInfo->coin);
	
	// 
	//tdnText::Draw(400, 720, 0xffffffff, "�����Ă�l: %.1f", m_iScrollPosY);

	
	// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
	m_pStateMachine->Render();

	m_pImages[IMAGE::TITLE]->Render(0, 0);


	// �����͐V�����ǉ���Trophy�p��InfoMation��`��
	//����łƂ肠�����I���A�C�R���͍�ꂽ����X�e�[�g��Ctrl�֐������
	//	 �I���𓮂�����悤�ɍ��B�ړ������̓��j���[��ʂ���R�s�y

	//	 �I���΂������ɂ�����I�[�������@RenderRoom�ňă��b�N�Ȃ�΃��b�N�ւɃ`�F���W���Y�ꂸ��

#ifdef _DEBUG

	tdnText::Draw(10, 200, 0xffffffff, "��%d", m_tagTI.iRowNum);
	tdnText::Draw(10, 250, 0xffffffff, "�ő�̍���%d", m_tagTI.iMaxHeight);
	tdnText::Draw(10, 300, 0xffffffff, "���݂̃g�b�v%d", m_tagTI.iTop);
	tdnText::Draw(10, 350, 0xffffffff, "�{�g����%d", m_tagTI.iBottom);
	tdnText::Draw(10, 400, 0xffffffff, "�I��No%d", m_tagTI.iSelectNo);

#endif // _DEBUG



	// �t�F�[�h
	Fade::Render();
}

void sceneCollect::FirstAction()
{
	m_pImages[IMAGE::TITLE]->Action();
	m_pImages[IMAGE::INFO_PLATE]->Action(6);
}

// �s�s�s����
void sceneCollect::PiPiPiCtrl(int iDeviceID)
{


	// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b�̂��߂̏���
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDeviceID) == 1)
	{
		m_iLeftPushFrame++;
		if (m_iSpeedLv == 0 && m_iLeftPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iLeftPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;
		m_iDownPushFrame = 0;
		m_iUpPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDeviceID) == 1)
	{
		m_iRightPushFrame++;
		if (m_iSpeedLv == 0 && m_iRightPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iRightPushFrame >= 77)m_iSpeedLv = 2;

		m_iDownPushFrame = 0;
		m_iLeftPushFrame = 0;
		m_iUpPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_UP, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_UP, iDeviceID) == 1)
	{
		m_iUpPushFrame++;
		if (m_iSpeedLv == 0 && m_iUpPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iUpPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;
		m_iDownPushFrame = 0;
		m_iLeftPushFrame = 0;

	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDeviceID) == 1)
	{
		m_iDownPushFrame++;
		if (m_iSpeedLv == 0 && m_iDownPushFrame >= 16)m_iSpeedLv = 1;
		if (m_iSpeedLv == 1 && m_iDownPushFrame >= 77)m_iSpeedLv = 2;

		m_iRightPushFrame = 0;
		m_iLeftPushFrame = 0;
		m_iUpPushFrame = 0;

	}
	else
	{
		m_iLeftPushFrame = 0;
		m_iRightPushFrame = 0;
		m_iUpPushFrame = 0;
		m_iDownPushFrame = 0;
		m_iSpeedLv = 0;
	}

	//+------------------------------------------------
	//		�������Ƃ��̔���
	//+------------------------------------------------

	int iInterval = 16;
	if (m_iSpeedLv == 1)iInterval = 6;
	if (m_iSpeedLv == 2)iInterval = 4;

	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, iDeviceID) == 3 ||
		(m_iLeftPushFrame % (iInterval) == 0 && m_iLeftPushFrame != 0))
	{
		// ��������
		m_bLeftPush = true;
		// �������� 
		se->Play("�J�[�\��2");
		// ���̖��A�j��

	}
	else
	{
		// �����ĂȂ�������t���O��OFF
		m_bLeftPush = false;
	}



	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, iDeviceID) == 3 ||
		(m_iRightPushFrame % (iInterval) == 0 && m_iRightPushFrame != 0))
	{
		// �E������
		m_bRightPush = true;
		// �������� 
		se->Play("�J�[�\��2");
		// �E�̖��A�j��

	}
	else
	{
		// �����ĂȂ�������t���O��OFF
		m_bRightPush = false;
	}

	if (tdnInput::KeyGet(KEYCODE::KEY_UP, iDeviceID) == 3 ||
		(m_iUpPushFrame % (iInterval) == 0 && m_iUpPushFrame != 0))
	{
		// �㉟����
		m_bUpPush = true;
		// �������� 
		se->Play("�J�[�\��2");

	}
	else
	{
		// �����ĂȂ�������t���O��OFF
		m_bUpPush = false;
	}

	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, iDeviceID) == 3 ||
		(m_iDownPushFrame % (iInterval) == 0 && m_iDownPushFrame != 0))
	{
		// ��������
		m_bDownPush = true;
		// �������� 
		se->Play("�J�[�\��2");

	}
	else
	{
		// �����ĂȂ�������t���O��OFF
		m_bDownPush = false;
	}


}

//------------------------------------------------------
//	�g���t�B�[
//------------------------------------------------------

void sceneCollect::TrophyInit()
{

	//�@�����̐擪
	m_tagTI.iTop = 0;
	m_tagTI.iBottom = 2;
	
	// ���񂩂�ݒ�
	m_tagTI.iRowNum = 6;
	// �g���t�B�[�����Ɂ�������킹�����ʁA�c���̑傫���𒲂ׂ�
	int l_iCount = 0;
	m_tagTI.iMaxHeight = 1;// ���܂��Œ��1�͂���
	// +�A�C�R���̏����ʒu�����߂�
	Vector2 l_vIconPos = VECTOR2_ZERO;
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		// �J�E���g����𒴂�����
		if (l_iCount >= m_tagTI.iRowNum)
		{
			m_tagTI.iMaxHeight++;// �ő啝�X�V			
			l_iCount = 0;

			l_vIconPos.y += 156;
			l_vIconPos.x = 0;
		}

		// �|�W�V�����ݒ�I�I
		TrophyMgr->GetTrophy(i)->SetRoomPos(l_vIconPos);

		// ���ɂ��炷
		l_vIconPos.x += 156;

		l_iCount++;
	}

	// ���ɏ����ɑI�����Ă���ԍ�
	m_tagTI.iSelectNo = 0;
	// �������̑I�����Ă���ԍ����珉���|�W�V�����擾
	m_tagTI.vSelectPos = TrophyMgr->GetTrophy(m_tagTI.iSelectNo)->GetRoomPos();
	m_tagTI.vNextPos= TrophyMgr->GetTrophy(m_tagTI.iSelectNo)->GetRoomPos();

	// �}�[�N�G
	m_tagTI.pSelectMark = new tdn2DAnim("Data/Trophy/Select.png");
	//m_tagTI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagTI.pSelectMark->OrderAlphaMove(210, 90, 120);
	//m_tagTI.pSelectMark->OrderNone();
	m_tagTI.pSelectMark->Action();

	m_tagTI.pSelectMarkEdge = new tdn2DAnim("Data/Trophy/SelectEdge.png");
	//m_tagTI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagTI.pSelectMarkEdge->OrderNone();
	m_tagTI.pSelectMarkEdge->Action();

	// �X�N���[�������ʒu
	m_tagTI.vScrollPos		= VECTOR2_ZERO;
	m_tagTI.vScrollNextPos	= VECTOR2_ZERO;

}

void sceneCollect::TrophyUpdate()
{
	// �I���A�C�R���̉摜�̓�������
	
	// �ړI�n�X�V
	m_tagTI.vNextPos = TrophyMgr->GetTrophy(m_tagTI.iSelectNo)->GetRoomPos();

	// �ꏊ��Ԃ̍X�V
	m_tagTI.vSelectPos = m_tagTI.vNextPos * 0.5f + m_tagTI.vSelectPos  * 0.5f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_tagTI.vSelectPos.x - m_tagTI.vNextPos.x) &&
		0.9f >= abs(m_tagTI.vSelectPos.y - m_tagTI.vNextPos.y))
	{
		m_tagTI.vSelectPos = m_tagTI.vNextPos;
	}

	//+------------------------------
	//	�X�N���[������
	//+------------------------------
	// ���݂̑I�����Ă�g���t�B�[�̍���
	int l_iNowHeight = m_tagTI.iSelectNo / m_tagTI.iRowNum;

	// ��Ɠ����ꏊ�E����ȏ�Ȃ�
	if (l_iNowHeight <= m_tagTI.iTop)
	{
		// ��ԏ�ȊO�Ȃ���
		if (m_tagTI.iTop >= 1)
		{
			m_tagTI.iTop--;
		}

	}

	// ���ւƓ����ꏊ�E����ȏ�Ȃ�
	// �����݂̃{�g�����擾
	int m_iNowBottom = m_tagTI.iTop + m_tagTI.iBottom;

	if (l_iNowHeight >= m_iNowBottom)
	{
		// ��ԉ��ȊO�Ȃ�
		if (m_tagTI.iTop < m_tagTI.iMaxHeight - (m_tagTI.iBottom + 1))
		{
			m_tagTI.iTop++;
		}
	}

	// �ړI�n�X�V
	m_tagTI.vScrollNextPos.y = (float)(m_tagTI.iTop * 156);

	// �ꏊ��Ԃ̍X�V
	m_tagTI.vScrollPos = m_tagTI.vScrollNextPos * 0.5f + m_tagTI.vScrollPos* 0.5f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_tagTI.vScrollPos.x - m_tagTI.vScrollNextPos.x) &&
		0.9f >= abs(m_tagTI.vScrollPos.y - m_tagTI.vScrollNextPos.y))
	{
		m_tagTI.vScrollPos = m_tagTI.vScrollNextPos;
	}

	// �I���A�C�R��
	m_tagTI.pSelectMark->Update();
	if (m_tagTI.pSelectMark->GetAction()->IsEnd() == true)
	{
		m_tagTI.pSelectMark->Action();
	}

	m_tagTI.pSelectMarkEdge->Update();

}

void sceneCollect::TrophyRender()
{
	// (��)�@�A�C�R���̊G
	int l_iX = 200;
	int l_iY = 100 - (int)m_tagTI.vScrollPos.y;

	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		// �J�E���g����𒴂�����
		//if (l_iCount >= m_tagTI.iRowNum)
		//{
		//	l_iY += 156;
		//	l_iX = 100;

		//	l_iCount = 0;
		//}

		//l_iX += 156;

		// ����Lib�`��@��ŃX�P�[����A���t�@�Ȃǂ��������H���������_�[�����
		//TrophyMgr->GetTrophy(i).pIcon->Render(l_iX, l_iY);
		TrophyMgr->RenderRoom(i, l_iX, l_iY);

		//l_iCount++;
	}

	//tdnText::Draw(m_tagTI.vSelectPos.x + l_iX, m_tagTI.vSelectPos.y + l_iY, 0xffff00ff, "�I�𒆁I�I");
	// �Z���N�g�A�C�R��
	m_tagTI.pSelectMark->Render((int)m_tagTI.vSelectPos.x + l_iX, (int)m_tagTI.vSelectPos.y + l_iY,RS::ADD);
	m_tagTI.pSelectMarkEdge->Render((int)m_tagTI.vSelectPos.x + l_iX, (int)m_tagTI.vSelectPos.y + l_iY, RS::ADD);

	// �����p�v���[�g
	m_pImages[IMAGE::INFO_PLATE]->Render(0, 592 - 128);

	// �g���t�B�[�̃^�C�g���Ɛ���
	TrophyMgr->RenderInfo(m_tagTI.iSelectNo, 60, 580);

	//tdnFont::RenderString(TrophyMgr->GetTrophyData(m_tagTI.iSelectNo).sTitle.c_str(), "HG�޼��E",
	//	22, 60, 580, 0xffffffff, RS::COPY);
	//// 
	//tdnFont::RenderString(TrophyMgr->GetTrophyData(m_tagTI.iSelectNo).sText.c_str(), "HG�޼��E",
	//	19,  60, 610, 0xffffffff, RS::COPY);


}

// �����͂����܂�
// �������͗��̓���������
// �g���t�B�[�̉��o�V�[���ς�������������
// �c��̃��[�U�[�C���^�[�t�F�[�X���
// �~�ŃA�C�R�����o�Ă��ă��j���[�ɖ߂�Ƃ��̃V�[���X�e�[�g�֘A

void sceneCollect::TrophyCtrl(int iDeviceID)
{
	// ��{�҂҂ґ���
	 PiPiPiCtrl(iDeviceID);

	// �������Ă���
	if ( m_bLeftPush)
	{
		 m_tagTI.iSelectNo--;
		 if (m_tagTI.iSelectNo < 0)
		 {
			 m_tagTI.iSelectNo = TrophyMgr->GetMaxTrophyNum() - 1;
		 }

		 // �ړ����o
		 m_tagTI.pSelectMark->Action();

	}
	// �E�����Ă���
	if ( m_bRightPush)
	{
		 m_tagTI.iSelectNo++;
		 if (m_tagTI.iSelectNo >= TrophyMgr->GetMaxTrophyNum())
		 {
			 m_tagTI.iSelectNo = 0;
		 }

		 // �ړ����o
		 m_tagTI.pSelectMark->Action();

	}
	// �㉟���Ă���
	if ( m_bUpPush)
	{
		int l_iNo = m_tagTI.iSelectNo - m_tagTI.iRowNum;
		if (l_iNo < 0)
		{
			// �ђʂ����ꍇ
			// ��𒲂ׂ�
			int l_iColumn = m_tagTI.iSelectNo%m_tagTI.iRowNum;
			
			// �񕶂܂킷
			int l_iJumpNo = TrophyMgr->GetMaxTrophyNum() - 1;// ��ԍŌ�̐����擾
			for (int i = 0; i < m_tagTI.iRowNum; i++)
			{
				// �񂪓��������ׂ�
				if (l_iColumn == l_iJumpNo%m_tagTI.iRowNum)
				{
					// �����Ȃ�΂��̗�̏ꏊ�֔�΂�
					m_tagTI.iSelectNo = l_iJumpNo;
				}
			
				// �Ȃ���Ύ��̏ꏊ��
				l_iJumpNo--;
			
			}

		}
		else
		{
			// �񕪑���
			m_tagTI.iSelectNo -= m_tagTI.iRowNum;
		}

		// �ړ����o
		m_tagTI.pSelectMark->Action();

	}
	// �������Ă���
	if ( m_bDownPush)
	{
		int l_iNo = m_tagTI.iSelectNo + m_tagTI.iRowNum;
		if (l_iNo >= TrophyMgr->GetMaxTrophyNum())
		{
			// �ђʂ����ꍇ
			// ��𒲂ׂ�
			int l_iColumn = m_tagTI.iSelectNo%m_tagTI.iRowNum;
			// ���̗�̏ꏊ�փ��[�v
			m_tagTI.iSelectNo = l_iColumn;

		}
		else
		{

			// �񕪑���
			m_tagTI.iSelectNo += m_tagTI.iRowNum;
		}

		// �ړ����o
		m_tagTI.pSelectMark->Action();


	}

	


}

//
//=============================================================================================

// �g���t�B�[�̃A�C�R���̏�񂪋l�܂����\���̂̏�����
sceneCollect::TrophyIconDesc::TrophyIconDesc()
{
	iTop = 0;
	iBottom = 0;
	iMaxHeight = 0;
	iRowNum = 0;

	iSelectNo = 0;
	vSelectPos = VECTOR2_ZERO;
	vNextPos = VECTOR2_ZERO;

	pSelectMark = nullptr;
	pSelectMarkEdge = nullptr;

	vScrollPos = VECTOR2_ZERO;
	vScrollNextPos = VECTOR2_ZERO;

}
