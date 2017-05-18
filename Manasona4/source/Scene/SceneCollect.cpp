#include "TDNLIB.h"
#include "../Sound/SoundManager.h"
#include "system/FrameworkEx.h"
#include "../Data/PlayerData.h"
#include "SceneMenu.h"
#include "SceneCollect.h"
#include "../Fade/Fade.h"
#include "Trophy\TrophyManager.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "MenuUI\CoinUI.h"

// �O���錾

int sceneCollect::m_iCtrlDevice(0);// ���삵�Ă����f�o�C�X
TAG_TYPE sceneCollect::m_eTagType(TAG_TYPE::TROPHY);// �I�𒆂̃^�u

//=============================================================================================
//		��	��	��	��	�J	��
sceneCollect::sceneCollect() :BaseGameEntity(ENTITY_ID::SCENE_COLLECT) {}

bool sceneCollect::Initialize()
{
	// ����
	m_pMovie = new tdnMovie("DATA/UI/Collect/CollectMovie.wmv");
	m_pBackMovie = new tdn2DObj(m_pMovie->GetTexture());
	m_pMovie->Play();

	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	m_fLoadPercentage = .5f;	// ���[�h����

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE_COLLECT::ARRAY_END; i++)
	{
		switch ((TIPS_TYPE_COLLECT)i)
		{
		case TIPS_TYPE_COLLECT::BACK_MENU:
			m_pTips[i] = new TipsCard("���j���[��ʂɖ߂�܂����H", true);
			break;
		case TIPS_TYPE_COLLECT::CONTENT_BUY:
			m_pTips[i] = new TipsCard("�R���e���c���w�����܂����H", true);
			break;
		default:
			MyAssert(0, "����ȃq���g�͂Ȃ�");
			break;
		}
	}

	// �^�O�̏ꏊ
	m_vTagPos.x = 448;
	m_vTagPos.y = 48;
	
	// �g���t�B�[�R���v�p�[�Z���g�̏ꏊ
	m_vTrophyPercentPos.x = 986;
	m_vTrophyPercentPos.y = 34;

	// �摜������
	m_pImages[IMAGE::BACK] = new tdn2DAnim("DATA/UI/Collect/bg.png");
	m_pImages[IMAGE::SCROLL_BAR] = new tdn2DAnim("DATA/UI/Collect/ScrollBar.png");
	m_pImages[IMAGE::SCROLL_ARROW] = new tdn2DAnim("DATA/UI/Collect/ScrollArrow.png");
	m_pImages[IMAGE::TITLE] = new tdn2DAnim("DATA/UI/Collect/title.png");
	m_pImages[IMAGE::INFO_PLATE] = new tdn2DAnim("DATA/UI/Collect/Information.png");
	m_pImages[IMAGE::INFO_PLATE_WATCH] = new tdn2DAnim("DATA/UI/Collect/InformationWatch.png");
	m_pImages[IMAGE::TAG_TROPHY] = new tdn2DAnim("DATA/UI/Collect/TagTrophy.png");
	m_pImages[IMAGE::TAG_PICTURE] = new tdn2DAnim("DATA/UI/Collect/TagPicture.png");
	m_pImages[IMAGE::TAG_MOVIE] = new tdn2DAnim("DATA/UI/Collect/TagMovie.png");
	m_pImages[IMAGE::TAG_RECORD] = new tdn2DAnim("DATA/UI/Collect/TagRecord.png");
	m_pImages[IMAGE::TROPHY_COMP_PERCENT] = new tdn2DAnim("DATA/UI/Collect/CompleteTrophyPercent.png");
	m_pImages[IMAGE::PRICE_PLATE] = new tdn2DAnim("DATA/UI/Collect/Price.png");
	m_pImages[IMAGE::NUMBER] = new tdn2DAnim("DATA/UI/Collect/Number.png");
	m_pImages[IMAGE::NUMBER_TROPHY] = new tdn2DAnim("DATA/UI/Collect/TrophyNumber.png");
	m_pImages[IMAGE::RECORD_PLATE] = new tdn2DAnim("DATA/UI/Collect/RecordPlate.png");
	m_pImages[IMAGE::ARROW_LEFT] = new tdn2DAnim("DATA/UI/Collect/ArrowL.png");
	m_pImages[IMAGE::ARROW_RIGHT] = new tdn2DAnim("DATA/UI/Collect/ArrowR.png");

	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i]->OrderNone();
		m_pImages[i]->Action();
	}
	m_pImages[IMAGE::TITLE]->OrderMoveAppeared(16, 0, -200);
	m_pImages[IMAGE::INFO_PLATE]->OrderMoveAppeared(24, 0, 592 + (200 ));
	m_pImages[IMAGE::INFO_PLATE_WATCH]->OrderNone();
	m_pImages[IMAGE::INFO_PLATE_WATCH]->Action();
	m_pImages[IMAGE::TAG_TROPHY]->OrderMoveAppeared(12, (int)m_vTagPos.x - 100 , (int)m_vTagPos.y);
	m_pImages[IMAGE::TAG_PICTURE]->OrderMoveAppeared(12, (int)m_vTagPos.x - 100, (int)m_vTagPos.y);
	m_pImages[IMAGE::TAG_MOVIE]->OrderMoveAppeared(12, (int)m_vTagPos.x - 100, (int)m_vTagPos.y);
	m_pImages[IMAGE::TAG_RECORD]->OrderMoveAppeared(12, (int)m_vTagPos.x - 100, (int)m_vTagPos.y);

	m_pImages[IMAGE::TROPHY_COMP_PERCENT]->OrderMoveAppeared(12, (int)m_vTrophyPercentPos.x , (int)m_vTrophyPercentPos.y);

	m_pImages[IMAGE::PRICE_PLATE]->OrderNone();
	m_pImages[IMAGE::PRICE_PLATE]->Action();
	m_pImages[IMAGE::NUMBER]->OrderNone();
	m_pImages[IMAGE::NUMBER]->Action();

	m_pImages[IMAGE::ARROW_LEFT]->OrderShrink(8, 1, 2);
	m_pImages[IMAGE::ARROW_RIGHT]->OrderShrink(8, 1, 2);

	m_pImages[IMAGE::NUMBER_TROPHY]->OrderShrink(8, 1 , 1.5f);

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

	//+-------------------------------
	//	�C���X�g�ŕK�v�ȕϐ��̏�����
	m_pPictureMgr = new PictureManager();
	// �C���X�g�A�C�R��
	PictureInit();

	//+-------------------------------
	//	���[�r�[�ŕK�v�ȕϐ��̏�����
	m_pMovieMgr = new MovieManager();
	// ���[�r�[�A�C�R��
	MovieInit();

	// �摜�ӏܑO�̍����t�F�[�h
	m_bBlackRect = false;
	m_iBlackRectAlpha = 0;

	// 
	m_bWatchInfo = true;

	//
	m_pCoinUI = new CoinUI(Vector2(980.0f, 40), true);// Vector2(15.0f, 500.0f)

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
	SAFE_DELETE(m_pMovie);
	SAFE_DELETE(m_pBackMovie);

	bgm->StopStreamIn();
	FOR(IMAGE::ARRAY_END) SAFE_DELETE(m_pImages[i]);
	
	SAFE_DELETE(m_pCoinUI);

	SAFE_DELETE(m_tagTI.pSelectMark);
	SAFE_DELETE(m_tagTI.pSelectMarkEdge);

	SAFE_DELETE(m_tagPI.pSelectMark);
	SAFE_DELETE(m_tagPI.pSelectMarkEdge);

	SAFE_DELETE(m_tagMI.pSelectMark);
	SAFE_DELETE(m_tagMI.pSelectMarkEdge);

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE_COLLECT::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTips[i]);
	}

	// �ӏܒ��ɐ؂�ꂽ�ꍇ
	if (m_pStateMachine->
		isInState(*SceneCollectState::PictureWatch::GetInstance()))
	{
		// �ӏܒ��p�摜���
		m_pPictureMgr->RereaseExe();
	}
	SAFE_DELETE(m_pPictureMgr);

	// �ӏܒ��ɐ؂�ꂽ�ꍇ
	if (m_pStateMachine->
		isInState(*SceneCollectState::MovieWatch::GetInstance()))
	{
		// �ӏܒ��p�摜���
		m_pMovieMgr->RereaseExe();
	}
	SAFE_DELETE(m_pMovieMgr);

	// ���X�e�[�g�}�V���͍Ō��
	SAFE_DELETE(m_pStateMachine);
}
//=============================================================================================


//=============================================================================================
//		�X			�V
void sceneCollect::Update()
{
	static bool bEnd(false);
	
	// ����̃��[�v�X�V
	m_pMovie->Update();

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE_COLLECT::ARRAY_END; i++)
	{
		m_pTips[i]->Update(m_iCtrlDevice);
	}

	// �R���e���c������
	int l_iOwnedContent = m_pPictureMgr->GetPictureOwned();// �C���X�g
	l_iOwnedContent+= m_pMovieMgr->GetMovieOwned();// ���[�r�[���܂߂�

	// �R���e���c�������g���t�B�[�������ł��邩�m�F
	TrophyMgr->CheakBuyManyContent(l_iOwnedContent);

	// �g���t�B
	TrophyMgr->Update();

	// �X�V
	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i]->Update();
	}

	// �R�C��
	m_pCoinUI->Update();


	// �t�F�[�h
	Fade::Update();



	// ���X�e�[�g�}�V���X�V(���̂����ɏ������Ƃ����ƁA���ŃV�[���`�F���W�̏������s���Ă��邽��)
	m_pStateMachine->Update();


	// Y�Ńg���t�B�[���b�N
	if (KeyBoard(KB_Y) == 2)
	{
		TrophyMgr->AllReset();
	}

	if (KeyBoard(KB_I) == 2)
	{
		m_pPictureMgr->AllReset();
	}

	if (KeyBoard(KB_K) == 2)
	{
		m_pMovieMgr->AllReset();
	}
	if (KeyBoard(KB_L) == 2)
	{
		PlayerDataMgr->m_PlayerInfo.coin = 0;
	}

#ifdef _DEBUG

	// T�Ńg���t�B�[����
	if (KeyBoard(KB_T) == 2)
	{
		TROPHY_TYPE eType;
		eType = TROPHY_TYPE::FIRST;
		MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

	}
	if (KeyBoard(KB_U) == 2)
	{
		TROPHY_TYPE eType;
		eType = TROPHY_TYPE::FIRST_BATTLE;
		MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

	}


	// 
	if (KeyBoard(KB_O) == 2)
	{
		PICTURE_TYPE eType;
		eType = PICTURE_TYPE::AIROU;
		MsgMgr->Dispatch(0, ENTITY_ID::PICTURE_MGR, ENTITY_ID::PICTURE_MGR, MESSAGE_TYPE::PICTURE_GET, &eType);

	}
	// �v���C�񐔏�����
	if (KeyBoard(KB_P) == 2)
	{
		PlayerDataMgr->m_PlayerInfo.BattleCount = 0;
	}


	if (bEnd)
	{
		// �t�F�[�h�I�������A��
		if (Fade::isFadeStop())
		{
			bEnd = false;
			MainFrameEX->ChangeScene(new sceneMenu);
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
	//m_pImages[IMAGE::BACK]->Render(0,0);
	m_pBackMovie->Render(0, 0);

	// tdnPolygon::Rect((int)(1280 * .25f) / 2, 96, (int)(1280 * .75f), (int)(720 * .75f), RS::COPY, 0xff808080);

	// 
	//tdnText::Draw(400, 720, 0xffffffff, "�����Ă�l: %.1f", m_iScrollPosY);

	
	// �������ɃX�e�[�g�}�V���`��(����2D�֌W�������񂶂�Ȃ�����)
	m_pStateMachine->Render();

	// �q���g�J�[�h��
	for (int i = 0; i < (int)TIPS_TYPE_COLLECT::ARRAY_END; i++)
	{
		m_pTips[i]->Render();
	}




	// �g���t�B
	TrophyMgr->Render();

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

	// �X�N���[���Ԋu
	m_tagTI.iScrollAddX = 156;
	m_tagTI.iScrollAddY = 156;

	// +�A�C�R���̏����ʒu�����߂�
	Vector2 l_vIconPos = VECTOR2_ZERO;
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		// �J�E���g����𒴂�����
		if (l_iCount >= m_tagTI.iRowNum)
		{
			m_tagTI.iMaxHeight++;// �ő啝�X�V			
			l_iCount = 0;

			l_vIconPos.y += m_tagTI.iScrollAddY;
			l_vIconPos.x = 0;
		}

		// �|�W�V�����ݒ�I�I
		TrophyMgr->GetTrophy(i)->SetRoomPos(l_vIconPos);

		// ���ɂ��炷
		l_vIconPos.x += m_tagTI.iScrollAddX;

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

// �g���t�B�[���[���̍ŏ��̉��o
void sceneCollect::TrophyFirstAction()
{
	// �^�O�̃A�j��
	m_pImages[IMAGE::TAG_TROPHY]->Action();
	m_pImages[IMAGE::ARROW_LEFT]->Action(8);
	m_pImages[IMAGE::ARROW_RIGHT]->Action(8);

	// %�̃A�j��+����
	m_pImages[IMAGE::TROPHY_COMP_PERCENT]->Action();
	m_pImages[IMAGE::NUMBER_TROPHY]->Action();

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
	m_tagTI.vScrollNextPos.y = (float)(m_tagTI.iTop * m_tagTI.iScrollAddY);

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
	int l_iX = 150;
	int l_iY = 100 - (int)m_tagTI.vScrollPos.y;

	// �g���t�B�[�`��
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{	
		TrophyMgr->RenderRoom(i, l_iX, l_iY);
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

	// ���g���t�B�[�p�̃X�N���[���o�[
	enum { ABUJ = 1 };// �ő�̍������{�P���Ă���̂�
	float l_fArrowRate = (float)(m_tagTI.iBottom + ABUJ) / (float)(m_tagTI.iMaxHeight);

	// �X�N���[������K�v������Ȃ�X�N���[���o�[��`��
	if (l_fArrowRate < 1.0f)
	{
		float l_iScrollY = 384 * (float)(m_tagTI.iTop) / (float)(m_tagTI.iMaxHeight);

		m_pImages[sceneCollect::IMAGE::SCROLL_BAR]->Render((int)m_vScrollPos.x, (int)m_vScrollPos.y);
		m_pImages[sceneCollect::IMAGE::SCROLL_ARROW]->Render((int)m_vScrollPos.x,(int)m_vScrollPos.y + (int)l_iScrollY,
				32, (int)(384 * l_fArrowRate), 0, 0, 32, 384);
	}

	// �^�O
	m_pImages[IMAGE::TAG_TROPHY]->Render(448, 48);

	m_pImages[IMAGE::ARROW_LEFT]->Render(377, 53);
	m_pImages[IMAGE::ARROW_RIGHT]->Render(839, 53);

	// �g���t�B�[�R���v��
	m_pImages[IMAGE::TROPHY_COMP_PERCENT]->Render((int)m_vTrophyPercentPos.x, (int)m_vTrophyPercentPos.y);
	
	// ���l�`��
	float l_iPercent = (float)(TrophyMgr->GetTrophyOwned()) / (float)(TrophyMgr->GetMaxTrophyNum());
	int l_iNumber = (int)(l_iPercent * 100);

	//if (l_iNumber >= 100)
	//{
	//	std::string l_strFont = std::to_string(l_iNumber);
	//	tdnFont::RenderString(l_strFont.c_str(), "HG�޼��E", 42,
	//		(int)m_vTrophyPercentPos.x + 150- 24, (int)m_vTrophyPercentPos.y, 0xffffffff, RS::COPY);

	//}
	//else if (l_iNumber >= 10)
	//{
	//	std::string l_strFont = std::to_string(l_iNumber);
	//	tdnFont::RenderString(l_strFont.c_str(), "HG�޼��E", 42,
	//		(int)m_vTrophyPercentPos.x + 150, (int)m_vTrophyPercentPos.y, 0xffffffff, RS::COPY);

	//}
	//else
	//{
	//	std::string l_strFont = std::to_string(l_iNumber);
	//	tdnFont::RenderString(l_strFont.c_str(), "HG�޼��E", 42,
	//		(int)m_vTrophyPercentPos.x + 150 + 24, (int)m_vTrophyPercentPos.y, 0xffffffff, RS::COPY);

	//}

	int number = (int)(l_iPercent * 100);

	for (int count = 0;; ++count)
	{
		int digitNum = number % 10;	// ��ԏ������������
		number = (int)(number / 10);// ���l�̈�ԏ�������������

		m_pImages[IMAGE::NUMBER_TROPHY]->Render(
			(int)m_vTrophyPercentPos.x + 160 - ((count * (42 / 2))),
			(int)m_vTrophyPercentPos.y,
			38, 38, digitNum * 38, 0, 38, 38);// �����`��

		if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
	}

	//for (int count = 0;; ++count)
	//{

	//	int l_iDigitNum = l_iNumber % 10;	// ��ԏ������������
	//	l_iNumber = (int)(l_iNumber / 10);// ���l�̈�ԏ�������������

	//	//
	//	//m_pNumber->Render((int)m_vTrophyPercentPos.x - ((count)* 22), (int)m_vTrophyPercentPos.y,
	//	//	32, 32, l_iDigitNum * 32, 0, 32, 32);

	//	if (l_iNumber <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
	//}

}

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



//------------------------------------------------------
//	�C���X�g
//------------------------------------------------------
void sceneCollect::PictureInit()
{
	//�@�����̐擪
	m_tagPI.iTop = 0;
	m_tagPI.iBottom = 2;

	// ���񂩂�ݒ�
	m_tagPI.iRowNum = 5;
	// �����Ɂ�������킹�����ʁA�c���̑傫���𒲂ׂ�
	int l_iCount = 0;
	m_tagPI.iMaxHeight = 1;// ���܂��Œ��1�͂���

	// �X�N���[���Ԋu
	m_tagPI.iScrollAddX = 154+32;
	m_tagPI.iScrollAddY = 92 + 24/*+32*/;

	// +�A�C�R���̏����ʒu�����߂�
	Vector2 l_vIconPos = VECTOR2_ZERO;
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		// �J�E���g����𒴂�����
		if (l_iCount >= m_tagPI.iRowNum)
		{
			m_tagPI.iMaxHeight++;// �ő啝�X�V			
			l_iCount = 0;

			l_vIconPos.y += m_tagPI.iScrollAddY;
			l_vIconPos.x = 0;
		}

		// �|�W�V�����ݒ�I�I
		m_pPictureMgr->GetPicture(i)->SetIconPos(l_vIconPos);

		// ���ɂ��炷
		l_vIconPos.x += m_tagPI.iScrollAddX;

		l_iCount++;
	}

	// ���ɏ����ɑI�����Ă���ԍ�
	m_tagPI.iSelectNo = 0;
	// �������̑I�����Ă���ԍ����珉���|�W�V�����擾
	m_tagPI.vSelectPos = m_pPictureMgr->GetPicture(m_tagPI.iSelectNo)->GetPictureIcon().vPos;
	m_tagPI.vNextPos = m_pPictureMgr->GetPicture(m_tagPI.iSelectNo)->GetPictureIcon().vPos;

	// �}�[�N�G
	m_tagPI.pSelectMark = new tdn2DAnim("Data/UI/Collect/Icon/Select.png");
	//m_tagPI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagPI.pSelectMark->OrderAlphaMove(210, 90, 120);
	//m_tagPI.pSelectMark->OrderNone();
	m_tagPI.pSelectMark->Action();

	m_tagPI.pSelectMarkEdge = new tdn2DAnim("Data/UI/Collect/Icon/SelectEdge.png");
	//m_tagPI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagPI.pSelectMarkEdge->OrderNone();
	m_tagPI.pSelectMarkEdge->Action();

	// �X�N���[�������ʒu
	m_tagPI.vScrollPos = VECTOR2_ZERO;
	m_tagPI.vScrollNextPos = VECTOR2_ZERO;

}

// �C���X�g�̍ŏ��̉��o
void sceneCollect::PictureFirstAction()
{
	// �^�O�̃A�j��
	m_pImages[IMAGE::TAG_PICTURE]->Action();
	m_pImages[IMAGE::ARROW_LEFT]->Action(8);
	m_pImages[IMAGE::ARROW_RIGHT]->Action(8);

	m_pCoinUI->Action();
	// %�̃A�j��
	//m_pImages[IMAGE::TROPHY_COMP_PERCENT]->Action();

}

void sceneCollect::PictureUpdate()
{
	// �I�����Ă���A�C�R���̓���
	if (m_pPictureMgr->GetSelectType() != (PICTURE_TYPE)m_tagPI.iSelectNo)
	{
		m_pPictureMgr->SetSelectType((PICTURE_TYPE)m_tagPI.iSelectNo);
	}


	// �I���A�C�R���̉摜�̓�������

	// �ړI�n�X�V
	m_tagPI.vNextPos = m_pPictureMgr->GetPicture(m_tagPI.iSelectNo)->GetPictureIcon().vPos;

	// �ꏊ��Ԃ̍X�V
	m_tagPI.vSelectPos = m_tagPI.vNextPos * 0.5f + m_tagPI.vSelectPos  * 0.5f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_tagPI.vSelectPos.x - m_tagPI.vNextPos.x) &&
		0.9f >= abs(m_tagPI.vSelectPos.y - m_tagPI.vNextPos.y))
	{
		m_tagPI.vSelectPos = m_tagPI.vNextPos;
	}

	//+------------------------------
	//	�X�N���[������
	//+------------------------------
	// ���݂̑I�����Ă�g���t�B�[�̍���
	int l_iNowHeight = m_tagPI.iSelectNo / m_tagPI.iRowNum;

	// ��Ɠ����ꏊ�E����ȏ�Ȃ�
	if (l_iNowHeight <= m_tagPI.iTop)
	{
		// ��ԏ�ȊO�Ȃ���
		if (m_tagPI.iTop >= 1)
		{
			m_tagPI.iTop--;
		}

	}

	// ���ւƓ����ꏊ�E����ȏ�Ȃ�
	// �����݂̃{�g�����擾
	int m_iNowBottom = m_tagPI.iTop + m_tagPI.iBottom;

	if (l_iNowHeight >= m_iNowBottom)
	{
		// ��ԉ��ȊO�Ȃ�
		if (m_tagPI.iTop < m_tagPI.iMaxHeight - (m_tagPI.iBottom + 1))
		{
			m_tagPI.iTop++;
		}
	}

	// �ړI�n�X�V
	m_tagPI.vScrollNextPos.y = (float)(m_tagPI.iTop *  m_tagPI.iScrollAddY);

	// �ꏊ��Ԃ̍X�V
	m_tagPI.vScrollPos = m_tagPI.vScrollNextPos * 0.5f + m_tagPI.vScrollPos* 0.5f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_tagPI.vScrollPos.x - m_tagPI.vScrollNextPos.x) &&
		0.9f >= abs(m_tagPI.vScrollPos.y - m_tagPI.vScrollNextPos.y))
	{
		m_tagPI.vScrollPos = m_tagPI.vScrollNextPos;
	}

	// �I���A�C�R��
	m_tagPI.pSelectMark->Update();
	if (m_tagPI.pSelectMark->GetAction()->IsEnd() == true)
	{
		m_tagPI.pSelectMark->Action();
	}

	m_tagPI.pSelectMarkEdge->Update();

}

void sceneCollect::PictureRender()
{
	// �A�C�R���̊G
	int l_iX = 150;
	int l_iY = 100 - (int)m_tagPI.vScrollPos.y;

	// �A�C�R���̕`��
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		m_pPictureMgr->RenderIcon(i, l_iX, l_iY);
	}

	// �Z���N�g�A�C�R��
	m_tagPI.pSelectMark->Render((int)m_tagPI.vSelectPos.x + l_iX, (int)m_tagPI.vSelectPos.y + l_iY, RS::ADD);
	m_tagPI.pSelectMarkEdge->Render((int)m_tagPI.vSelectPos.x + l_iX, (int)m_tagPI.vSelectPos.y + l_iY, RS::ADD);

	// �����p�v���[�g
	m_pImages[IMAGE::INFO_PLATE]->Render(0, 592 - 128);

	// �A�C�R���̃^�C�g���Ɛ���
	m_pPictureMgr->RenderIconInfo(m_tagPI.iSelectNo, 60, 580);

	// ���A�C�R���p�̃X�N���[���o�[
	enum { ABUJ = 1 };// �ő�̍������{�P���Ă���̂�
	float l_fArrowRate = (float)(m_tagPI.iBottom + ABUJ) / (float)(m_tagPI.iMaxHeight);

	// �X�N���[������K�v������Ȃ�X�N���[���o�[��`��
	if (l_fArrowRate < 1.0f)
	{
		float l_iScrollY = 384 * (float)(m_tagPI.iTop) / (float)(m_tagPI.iMaxHeight);

		m_pImages[sceneCollect::IMAGE::SCROLL_BAR]->Render((int)m_vScrollPos.x, (int)m_vScrollPos.y);
		m_pImages[sceneCollect::IMAGE::SCROLL_ARROW]->Render((int)m_vScrollPos.x, (int)m_vScrollPos.y + (int)l_iScrollY,
			32, (int)(384 * l_fArrowRate), 0, 0, 32, 384);
	}

	// �^�O
	m_pImages[IMAGE::TAG_PICTURE]->Render(448, 48);

	m_pImages[IMAGE::ARROW_LEFT]->Render(377, 53);
	m_pImages[IMAGE::ARROW_RIGHT]->Render(839, 53);

	// ���l�i
	// �܂���ɓ���ĂȂ��ă��b�N����Ă��Ȃ�������
	if (PlayerDataMgr->m_SecretData.iAllPic[m_tagPI.iSelectNo] == 0 && 
		GetPictureMgr()->GetPicture(m_tagPI.iSelectNo)->isRock() == false)
	{
		m_pImages[IMAGE::PRICE_PLATE]->Render(910,516);

		int number = GetPictureMgr()->GetPicture(m_tagPI.iSelectNo)->GetPrice();

		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pImages[IMAGE::NUMBER]->Render(1148 - ((count * (36 / 2))), 525, 32, 32, digitNum * 32, 0, 32, 32);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}
	}


	// ���R�C��
	m_pCoinUI->Render();
	
}

void sceneCollect::PictureInitExe()
{
	// �摜�ӏܑO�̍����t�F�[�h
	m_bBlackRect = false;

	// �摜�̏�����
	GetPictureMgr()->InitExe((PICTURE_TYPE)m_tagPI.iSelectNo);

	// ����������
	m_bWatchInfo = true;


}

void sceneCollect::PictureRenderExe()
{
	// �摜
	GetPictureMgr()->RenderExe();

	if (m_bWatchInfo == true)
	{
		// �����p�v���[�g
		m_pImages[IMAGE::INFO_PLATE_WATCH]->Render(0, 592 - 128);
		// �摜�̐���
		GetPictureMgr()->RenderExeInfo(m_tagPI.iSelectNo, 50, 580+14);
	}
}

void sceneCollect::PictureCtrl(int iDeviceID)
{
	// ��{�҂҂ґ���
	PiPiPiCtrl(iDeviceID);

	// �������Ă���
	if (m_bLeftPush)
	{
		m_tagPI.iSelectNo--;
		if (m_tagPI.iSelectNo < 0)
		{
			m_tagPI.iSelectNo = m_pPictureMgr->GetMaxPictureNum() - 1;
		}

		// �ړ����o
		m_tagPI.pSelectMark->Action();

	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		m_tagPI.iSelectNo++;
		if (m_tagPI.iSelectNo >= m_pPictureMgr->GetMaxPictureNum())
		{
			m_tagPI.iSelectNo = 0;
		}

		// �ړ����o
		m_tagPI.pSelectMark->Action();

	}
	// �㉟���Ă���
	if (m_bUpPush)
	{
		int l_iNo = m_tagPI.iSelectNo - m_tagPI.iRowNum;
		if (l_iNo < 0)
		{
			// �ђʂ����ꍇ
			// ��𒲂ׂ�
			int l_iColumn = m_tagPI.iSelectNo%m_tagPI.iRowNum;

			// �񕶂܂킷
			int l_iJumpNo = m_pPictureMgr->GetMaxPictureNum() - 1;// ��ԍŌ�̐����擾
			for (int i = 0; i < m_tagPI.iRowNum; i++)
			{
				// �񂪓��������ׂ�
				if (l_iColumn == l_iJumpNo%m_tagPI.iRowNum)
				{
					// �����Ȃ�΂��̗�̏ꏊ�֔�΂�
					m_tagPI.iSelectNo = l_iJumpNo;
				}

				// �Ȃ���Ύ��̏ꏊ��
				l_iJumpNo--;

			}

		}
		else
		{
			// �񕪑���
			m_tagPI.iSelectNo -= m_tagPI.iRowNum;
		}

		// �ړ����o
		m_tagPI.pSelectMark->Action();

	}
	// �������Ă���
	if (m_bDownPush)
	{
		int l_iNo = m_tagPI.iSelectNo + m_tagPI.iRowNum;
		if (l_iNo >= m_pPictureMgr->GetMaxPictureNum())
		{
			// �ђʂ����ꍇ
			// ��𒲂ׂ�
			int l_iColumn = m_tagPI.iSelectNo%m_tagPI.iRowNum;
			// ���̗�̏ꏊ�փ��[�v
			m_tagPI.iSelectNo = l_iColumn;

		}
		else
		{

			// �񕪑���
			m_tagPI.iSelectNo += m_tagPI.iRowNum;
		}

		// �ړ����o
		m_tagPI.pSelectMark->Action();
	}

}



//------------------------------------------------------
//	���[�r�[
//------------------------------------------------------
void sceneCollect::MovieInit()
{
	//�@�����̐擪
	m_tagMI.iTop = 0;
	m_tagMI.iBottom = 3;

	// ���񂩂�ݒ�
	m_tagMI.iRowNum = 5;
	// �����Ɂ�������킹�����ʁA�c���̑傫���𒲂ׂ�
	int l_iCount = 0;
	m_tagMI.iMaxHeight = 1;// ���܂��Œ��1�͂���

						   // �X�N���[���Ԋu
	m_tagMI.iScrollAddX = 154 + 32;
	m_tagMI.iScrollAddY = 92 + 32;

	// +�A�C�R���̏����ʒu�����߂�
	Vector2 l_vIconPos = VECTOR2_ZERO;
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		// �J�E���g����𒴂�����
		if (l_iCount >= m_tagMI.iRowNum)
		{
			m_tagMI.iMaxHeight++;// �ő啝�X�V			
			l_iCount = 0;

			l_vIconPos.y += m_tagMI.iScrollAddY;
			l_vIconPos.x = 0;
		}

		// �|�W�V�����ݒ�I�I
		m_pMovieMgr->GetMovie(i)->SetIconPos(l_vIconPos);

		// ���ɂ��炷
		l_vIconPos.x += m_tagMI.iScrollAddX;

		l_iCount++;
	}

	// ���ɏ����ɑI�����Ă���ԍ�
	m_tagMI.iSelectNo = 0;
	// �������̑I�����Ă���ԍ����珉���|�W�V�����擾
	m_tagMI.vSelectPos = m_pMovieMgr->GetMovie(m_tagMI.iSelectNo)->GetMovieIcon().vPos;
	m_tagMI.vNextPos = m_pMovieMgr->GetMovie(m_tagMI.iSelectNo)->GetMovieIcon().vPos;

	// �}�[�N�G
	m_tagMI.pSelectMark = new tdn2DAnim("Data/UI/Collect/Icon/Select.png");
	//m_tagMI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagMI.pSelectMark->OrderAlphaMove(210, 90, 120);
	//m_tagMI.pSelectMark->OrderNone();
	m_tagMI.pSelectMark->Action();

	m_tagMI.pSelectMarkEdge = new tdn2DAnim("Data/UI/Collect/Icon/SelectEdge.png");
	//m_tagMI.pSelectMark->OrderRipple(42, 1.0f, 0.015f);
	m_tagMI.pSelectMarkEdge->OrderNone();
	m_tagMI.pSelectMarkEdge->Action();

	// �X�N���[�������ʒu
	m_tagMI.vScrollPos = VECTOR2_ZERO;
	m_tagMI.vScrollNextPos = VECTOR2_ZERO;

}

// ���[�r�[�̍ŏ��̉��o
void sceneCollect::MovieFirstAction()
{
	// �^�O�̃A�j��
	m_pImages[IMAGE::TAG_MOVIE]->Action();
	m_pImages[IMAGE::ARROW_LEFT]->Action(8);
	m_pImages[IMAGE::ARROW_RIGHT]->Action(8);

	m_pCoinUI->Action();
	// %�̃A�j��
	//m_pImages[IMAGE::TROPHY_COMP_PERCENT]->Action();

}

void sceneCollect::MovieUpdate()
{
	// �I�����Ă���A�C�R���̓���
	if (m_pMovieMgr->GetSelectType() != (MOVIE_TYPE)m_tagMI.iSelectNo)
	{
		m_pMovieMgr->SetSelectType((MOVIE_TYPE)m_tagMI.iSelectNo);
	}


	// �I���A�C�R���̉摜�̓�������

	// �ړI�n�X�V
	m_tagMI.vNextPos = m_pMovieMgr->GetMovie(m_tagMI.iSelectNo)->GetMovieIcon().vPos;

	// �ꏊ��Ԃ̍X�V
	m_tagMI.vSelectPos = m_tagMI.vNextPos * 0.5f + m_tagMI.vSelectPos  * 0.5f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_tagMI.vSelectPos.x - m_tagMI.vNextPos.x) &&
		0.9f >= abs(m_tagMI.vSelectPos.y - m_tagMI.vNextPos.y))
	{
		m_tagMI.vSelectPos = m_tagMI.vNextPos;
	}

	//+------------------------------
	//	�X�N���[������
	//+------------------------------
	// ���݂̑I�����Ă�g���t�B�[�̍���
	int l_iNowHeight = m_tagMI.iSelectNo / m_tagMI.iRowNum;

	// ��Ɠ����ꏊ�E����ȏ�Ȃ�
	if (l_iNowHeight <= m_tagMI.iTop)
	{
		// ��ԏ�ȊO�Ȃ���
		if (m_tagMI.iTop >= 1)
		{
			m_tagMI.iTop--;
		}

	}

	// ���ւƓ����ꏊ�E����ȏ�Ȃ�
	// �����݂̃{�g�����擾
	int m_iNowBottom = m_tagMI.iTop + m_tagMI.iBottom;

	if (l_iNowHeight >= m_iNowBottom)
	{
		// ��ԉ��ȊO�Ȃ�
		if (m_tagMI.iTop < m_tagMI.iMaxHeight - (m_tagMI.iBottom + 1))
		{
			m_tagMI.iTop++;
		}
	}

	// �ړI�n�X�V
	m_tagMI.vScrollNextPos.y = (float)(m_tagMI.iTop *  m_tagMI.iScrollAddY);

	// �ꏊ��Ԃ̍X�V
	m_tagMI.vScrollPos = m_tagMI.vScrollNextPos * 0.5f + m_tagMI.vScrollPos* 0.5f;

	// 0.1�ȉ��̌덷�͖���������
	if (0.9f >= abs(m_tagMI.vScrollPos.x - m_tagMI.vScrollNextPos.x) &&
		0.9f >= abs(m_tagMI.vScrollPos.y - m_tagMI.vScrollNextPos.y))
	{
		m_tagMI.vScrollPos = m_tagMI.vScrollNextPos;
	}

	// �I���A�C�R��
	m_tagMI.pSelectMark->Update();
	if (m_tagMI.pSelectMark->GetAction()->IsEnd() == true)
	{
		m_tagMI.pSelectMark->Action();
	}

	m_tagMI.pSelectMarkEdge->Update();

}

void sceneCollect::MovieRender()
{
	// �A�C�R���̊G
	int l_iX = 150;
	int l_iY = 100 - (int)m_tagMI.vScrollPos.y;

	// �A�C�R���̕`��
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		m_pMovieMgr->RenderIcon(i, l_iX, l_iY);
	}

	// �Z���N�g�A�C�R��
	m_tagMI.pSelectMark->Render((int)m_tagMI.vSelectPos.x + l_iX, (int)m_tagMI.vSelectPos.y + l_iY, RS::ADD);
	m_tagMI.pSelectMarkEdge->Render((int)m_tagMI.vSelectPos.x + l_iX, (int)m_tagMI.vSelectPos.y + l_iY, RS::ADD);

	// �����p�v���[�g
	m_pImages[IMAGE::INFO_PLATE]->Render(0, 592 - 128);

	// �A�C�R���̃^�C�g���Ɛ���
	m_pMovieMgr->RenderIconInfo(m_tagMI.iSelectNo, 60, 580);

	// ���A�C�R���p�̃X�N���[���o�[
	enum { ABUJ = 1 };// �ő�̍������{�P���Ă���̂�
	float l_fArrowRate = (float)(m_tagMI.iBottom + ABUJ) / (float)(m_tagMI.iMaxHeight);

	// �X�N���[������K�v������Ȃ�X�N���[���o�[��`��
	if (l_fArrowRate < 1.0f)
	{
		float l_iScrollY = 384 * (float)(m_tagMI.iTop) / (float)(m_tagMI.iMaxHeight);

		m_pImages[sceneCollect::IMAGE::SCROLL_BAR]->Render((int)m_vScrollPos.x, (int)m_vScrollPos.y);
		m_pImages[sceneCollect::IMAGE::SCROLL_ARROW]->Render((int)m_vScrollPos.x, (int)m_vScrollPos.y + (int)l_iScrollY,
			32, (int)(384 * l_fArrowRate), 0, 0, 32, 384);
	}

	// �^�O
	m_pImages[IMAGE::TAG_MOVIE]->Render(448, 48);

	m_pImages[IMAGE::ARROW_LEFT]->Render(377, 53);
	m_pImages[IMAGE::ARROW_RIGHT]->Render(839, 53);

	// ���l�i
	// �܂���ɓ���ĂȂ��ă��b�N����Ă��Ȃ�������
	if (PlayerDataMgr->m_SecretData.iAllMovie[m_tagMI.iSelectNo] == 0 &&
		GetMovieMgr()->GetMovie(m_tagMI.iSelectNo)->isRock() == false)
	{
		m_pImages[IMAGE::PRICE_PLATE]->Render(910, 516);

		int number = GetMovieMgr()->GetMovie(m_tagMI.iSelectNo)->GetPrice();

		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pImages[IMAGE::NUMBER]->Render(1148 - ((count * (36 / 2))), 525, 32, 32, digitNum * 32, 0, 32, 32);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}
	}


	// ���R�C��
	m_pCoinUI->Render();

}

void sceneCollect::MovieInitExe()
{
	// �摜�ӏܑO�̍����t�F�[�h
	m_bBlackRect = false;

	// �摜�̏�����
	GetMovieMgr()->InitExe((MOVIE_TYPE)m_tagMI.iSelectNo);

	// ����������
	m_bWatchInfo = true;


}

void sceneCollect::MovieRenderExe()
{
	// �摜
	GetMovieMgr()->RenderExe();

	if (m_bWatchInfo == true)
	{
		// �����p�v���[�g
		m_pImages[IMAGE::INFO_PLATE_WATCH]->Render(0, 592 - 128);
		// �摜�̐���
		GetMovieMgr()->RenderExeInfo(m_tagMI.iSelectNo, 50, 580 + 14);
	}
}

void sceneCollect::MovieCtrl(int iDeviceID)
{
	// ��{�҂҂ґ���
	PiPiPiCtrl(iDeviceID);

	// �������Ă���
	if (m_bLeftPush)
	{
		m_tagMI.iSelectNo--;
		if (m_tagMI.iSelectNo < 0)
		{
			m_tagMI.iSelectNo = m_pMovieMgr->GetMaxMovieNum() - 1;
		}

		// �ړ����o
		m_tagMI.pSelectMark->Action();

	}
	// �E�����Ă���
	if (m_bRightPush)
	{
		m_tagMI.iSelectNo++;
		if (m_tagMI.iSelectNo >= m_pMovieMgr->GetMaxMovieNum())
		{
			m_tagMI.iSelectNo = 0;
		}

		// �ړ����o
		m_tagMI.pSelectMark->Action();

	}
	// �㉟���Ă���
	if (m_bUpPush)
	{
		int l_iNo = m_tagMI.iSelectNo - m_tagMI.iRowNum;
		if (l_iNo < 0)
		{
			// �ђʂ����ꍇ
			// ��𒲂ׂ�
			int l_iColumn = m_tagMI.iSelectNo%m_tagMI.iRowNum;

			// �񕶂܂킷
			int l_iJumpNo = m_pMovieMgr->GetMaxMovieNum() - 1;// ��ԍŌ�̐����擾
			for (int i = 0; i < m_tagMI.iRowNum; i++)
			{
				// �񂪓��������ׂ�
				if (l_iColumn == l_iJumpNo%m_tagMI.iRowNum)
				{
					// �����Ȃ�΂��̗�̏ꏊ�֔�΂�
					m_tagMI.iSelectNo = l_iJumpNo;
				}

				// �Ȃ���Ύ��̏ꏊ��
				l_iJumpNo--;

			}

		}
		else
		{
			// �񕪑���
			m_tagMI.iSelectNo -= m_tagMI.iRowNum;
		}

		// �ړ����o
		m_tagMI.pSelectMark->Action();

	}
	// �������Ă���
	if (m_bDownPush)
	{
		int l_iNo = m_tagMI.iSelectNo + m_tagMI.iRowNum;
		if (l_iNo >= m_pMovieMgr->GetMaxMovieNum())
		{
			// �ђʂ����ꍇ
			// ��𒲂ׂ�
			int l_iColumn = m_tagMI.iSelectNo%m_tagMI.iRowNum;
			// ���̗�̏ꏊ�փ��[�v
			m_tagMI.iSelectNo = l_iColumn;

		}
		else
		{

			// �񕪑���
			m_tagMI.iSelectNo += m_tagMI.iRowNum;
		}

		// �ړ����o
		m_tagMI.pSelectMark->Action();
	}

}


//------------------------------------------------------
//	���R�[�h
//------------------------------------------------------

void sceneCollect::RecordInit()
{


}

// ���R�[�h���[���̍ŏ��̉��o
void sceneCollect::RecordFirstAction()
{
	// �^�O�̃A�j��
	m_pImages[IMAGE::TAG_RECORD]->Action();
	m_pImages[IMAGE::ARROW_LEFT]->Action(8);
	m_pImages[IMAGE::ARROW_RIGHT]->Action(8);

	// %�̃A�j��+����
	m_pImages[IMAGE::TROPHY_COMP_PERCENT]->Action();
	m_pImages[IMAGE::NUMBER_TROPHY]->Action();

}

void sceneCollect::RecordUpdate()
{

}

void sceneCollect::RecordRender()
{
	// ���R�[�h�v���[�g
	m_pImages[IMAGE::RECORD_PLATE]->Render(0, 0);

	// ���R�[�h���e�`��

	int l_iY = 0;

	// �v���C��
	tdnText::Draw(256, 144, 0xffffffff, "�v���C��: %d", m_pPlayerInfo->PlayCount);

	l_iY += 30;

	// �v���C����
	const int minutes(m_pPlayerInfo->PlayTime % 60), hour(m_pPlayerInfo->PlayTime / 60);
	tdnText::Draw(256  , 144 + l_iY, 0xffffffff, "�v���C����: %d���� %d��", hour, minutes);

	l_iY += 30;

	// �R�C��
	tdnText::Draw(256 , 144 + l_iY, 0xffffffff, "�R�C��: %d", m_pPlayerInfo->coin);

	l_iY += 30;

	// �ΐ��
	tdnText::Draw(256 , 144 + l_iY, 0xffffffff, "�ΐ��: %d", m_pPlayerInfo->BattleCount);


	// �����p�v���[�g
	m_pImages[IMAGE::INFO_PLATE]->Render(0, 592 - 128);

	// �^�C�g��
	tdnFont::RenderString("����܂ł̋L�^���m�F���܂��B", "HG�޼��E",
		22, 50, 580 , 0xffffffff, RS::COPY);
	
	// ����
	//tdnFont::RenderString("����܂ł̋L�^���m�F���܂��B", "HG�޼��E",
	//	19, iX, iY + 30, 0xffffffff, RS::COPY);

	// �^�O
	m_pImages[IMAGE::TAG_RECORD]->Render(448, 48);

	m_pImages[IMAGE::ARROW_LEFT]->Render(377, 53);
	m_pImages[IMAGE::ARROW_RIGHT]->Render(839, 53);


}


//
//=============================================================================================

// �A�C�R���̏�񂪋l�܂����\���̂̏�����
sceneCollect::IconDesc::IconDesc()
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

	iScrollAddX = 156;
	iScrollAddY = 156;
}
