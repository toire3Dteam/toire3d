#include "RoundCall.h"
#include "system\System.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../UI/GameUI.h"
#include "../Sound/SoundManager.h"


// ������`����ƃ��E���h�R�[���̐����X�L�b�v�����(�f�o�b�O���Z�p)
//#define ROUND_CALL_ON

//+-------------------------------
//	�x�[�X���E���h�R�[��
//+-------------------------------




//+-------------------------------
//	�t�@�[�X�g���E���h�R�[��
//+-------------------------------

FirstRoundCall::FirstRoundCall()
{
	// ������
	m_iFrame = 0;

	// �i���o�[
	m_pNumber = new tdn2DAnim("Data/UI/Game/RoundCall/RoundNum.png");
	m_pNumber->OrderAlphaMove(120, ROLL_START, ROLL_END);
		
	// ���E���h���[��
	m_pRollBig.PicClean();
	m_pRollBig.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundRollBig.png");
	m_pRollBig.pic->OrderAlphaMove(120, ROLL_START, ROLL_END);
	
	m_pRollMini.PicClean();
	m_pRollMini.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundRollMini.png");
	m_pRollMini.pic->OrderAlphaMove(120, ROLL_START, ROLL_END);

	// ������
	m_pBlackLine.PicClean();
	m_pBlackLine.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundBlackLineWidth.png");
	m_pBlackLine.pic->OrderAlphaMove(120, ROLL_START, ROLL_END);

	// ����
	m_pFirstFont.PicClean();
	m_pFirstFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/theWheelofFateisTurning.png");
	m_pFirstFont.pic->OrderMoveAppeared(ROLL_START, 0, -90);

	// ��
	m_pLightRing = new tdn2DAnim("Data/UI/Game/RoundCall/BlueRing.png");
	m_pLightRing->OrderRipple(24, 1, 0.1f);
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue.png");
	m_pLightLine->OrderAlphaMove(130, ROLL_START, ROLL_END);

}

FirstRoundCall::~FirstRoundCall()
{
	SAFE_DELETE(m_pNumber);
	SAFE_DELETE(m_pRollBig.pic);
	SAFE_DELETE(m_pRollMini.pic);
	SAFE_DELETE(m_pBlackLine.pic);
	SAFE_DELETE(m_pFirstFont.pic);
	SAFE_DELETE(m_pLightRing);
	SAFE_DELETE(m_pLightLine);
}

void FirstRoundCall::FrameMove()
{
	if (m_iFrame == 4)
	{
#ifdef ROUND_CALL_ON
#else
		se->Play("�t�F�C�g�C�Y�^�[�j���O");
#endif
	}
	if (m_iFrame == ROLL_START)
	{
		#ifdef ROUND_CALL_ON
		se->Play("�t�F�C�g�C�Y�^�[�j���O");
		#endif

		// ���Q�[��UI����
		GameUIMgr->Action();
	}

	// ���[���̓���
	// ��]�̃X�s�[�h
	if (m_iFrame <= ROLL_START)
	{
		m_pRollBig.rotate -= 0.06f;
		m_pRollMini.rotate -= 0.06f;

		m_pRollBig.scale += 0.075f;
		m_pRollMini.scale += 0.075f;

		// ������
		m_pBlackLine.scale += 0.1f;

		// font
		m_pFirstFont.scale += 0.0005f;
		m_pFirstFont.pic->SetScale(m_pFirstFont.scale);
	}
	else if (m_iFrame <= ROLL_END)// ����
	{
		m_pRollBig.rotate -= 0.005f;
		m_pRollMini.rotate -= 0.019f;

		// ������
		m_pBlackLine.scale += 0.1f;
		
		// font
		m_pFirstFont.scale += 0.0005f;
		m_pFirstFont.pic->SetScale(m_pFirstFont.scale);

	}
	else // ���X�g
	{
		m_pRollBig.rotate -= 0.08f;
		m_pRollMini.rotate -= 0.18f;

		m_pRollBig.scale -= 0.075f;
		m_pRollMini.scale -= 0.075f;

		m_pBlackLine.rotate -= 0.15f;

	}


	if (m_iFrame == ROLL_END)// Font���o�؂�ւ�
	{
		m_pFirstFont.pic->OrderRipple(12, m_pFirstFont.scale, -0.03f);
		m_pFirstFont.pic->Action();
	}


}

void FirstRoundCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;
	
	// �I�菈��
	if (m_iFrame >= 132)
	{
		m_bActionFlag = false;
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// �X�V
	m_pRollBig.pic->SetAngle(m_pRollBig.rotate);
	m_pRollBig.pic->SetScale(m_pRollBig.scale);
	m_pRollMini.pic->SetAngle(m_pRollMini.rotate);
	m_pRollMini.pic->SetScale(m_pRollMini.scale);

	m_pBlackLine.pic->SetAngle(m_pBlackLine.rotate);
	m_pBlackLine.pic->SetScale(m_pBlackLine.scale);


	m_pNumber->Update();
	m_pRollBig.pic->Update();
	m_pRollMini.pic->Update();
	m_pBlackLine.pic->Update();
	m_pFirstFont.pic->Update();
	m_pLightRing->Update();
	m_pLightLine->Update();

}

void FirstRoundCall::Render()
{
	if (m_bActionFlag == false) return;

	//m_pNumber->Render(0,0);
	m_pBlackLine.pic->Render(0, 340);
	//m_pRollBig.pic->Render(0, 0,RS::ADD);
	//m_pRollMini.pic->Render(0, 0,RS::ADD);
	//m_pLightLine->Render((1280/2)/2, (720/2)/2, RS::ADD);
	m_pLightLine->Render(0, 0, RS::ADD);
	m_pFirstFont.pic->Render(0, 0);
	m_pLightRing->Render((1280/2)-256, (720 / 2) - 256, RS::ADD);
	

}

void FirstRoundCall::Action()
{
	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�
	m_pRollBig.rotate = -2.2f;
	m_pRollBig.scale = 0.4f;
	m_pRollMini.rotate = -1.5f;
	m_pRollMini.scale = 0.4f;

	m_pBlackLine.rotate = 0.0f;
	m_pBlackLine.scale = 1.0f;// line 
	m_pFirstFont.scale = 1.0f;// font

	m_pNumber->Action(0);
	m_pRollBig.pic->Action();
	m_pRollMini.pic->Action();
	m_pBlackLine.pic->Action();

	m_pFirstFont.pic->OrderMoveAppeared(ROLL_START, 0, -90);
	m_pFirstFont.pic->Action();
	m_pLightRing->Action();
	m_pLightLine->Action();
}


//+-------------------------------
//	���x���R�[��
//+-------------------------------

RebelCall::RebelCall()
{
	// ������
	m_iFrame = 0;

	// �i���o�[
	m_pNumber = new tdn2DAnim("Data/UI/Game/RoundCall/RoundNum.png");
	m_pNumber->OrderShrink(12, 1, 3);
	m_iRoundNumber = 1;

	// 
	m_pCenterCircle.PicClean();
	m_pCenterCircle.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundCenter.png");
	m_pCenterCircle.pic->OrderAlphaMove(120, REBEL_START, REBEL_END);

	m_pBG.PicClean();
	m_pBG.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundCallEx2.png");
	m_pBG.pic->OrderAlphaMove(120, REBEL_START, REBEL_END);
	m_pBG.scale = 1.0f;

	// ������
	m_pBlackLineL.PicClean();
	m_pBlackLineL.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundBlackLineHeight.png");
	m_pBlackLineL.pic->OrderAlphaMove(36, REBEL_START, 20);
	
	m_pBlackLineR.PicClean();
	m_pBlackLineR.pic = new tdn2DAnim("Data/UI/Game/RoundCall/RoundBlackLineHeight.png");
	m_pBlackLineR.pic->OrderAlphaMove(36, REBEL_START, 20);

	// ����
	m_pFirstFont.PicClean();
	m_pFirstFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/rebel.png");
	m_pFirstFont.pic->OrderMoveAppeared(14, 0, -30);

	// ��
	m_pLightRing = new tdn2DAnim("Data/UI/Game/RoundCall/BlueRing.png");
	m_pLightRing->OrderRipple(24, 1, 0.1f);
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue.png");
	m_pLightLine->OrderAlphaMove(130, REBEL_START, REBEL_END);

	// �}�X�N
	m_pMaskScreen = new tdn2DObj(1280, 720, TDN2D::RENDERTARGET);
	m_pMaskPic = new tdn2DObj("Data/UI/Game/RoundCall/mask.png");

}

RebelCall::~RebelCall()
{
	SAFE_DELETE(m_pNumber);
	SAFE_DELETE(m_pCenterCircle.pic);
	//m_pBG.pic->GetObj()->GetTexture()->
	//shader2D->SetMaskScreenNULL();
	SAFE_DELETE(m_pBG.pic);
	SAFE_DELETE(m_pBlackLineL.pic);
	SAFE_DELETE(m_pBlackLineR.pic);
	SAFE_DELETE(m_pFirstFont.pic);
	SAFE_DELETE(m_pLightRing);
	SAFE_DELETE(m_pLightLine);
	SAFE_DELETE(m_pMaskScreen);
	SAFE_DELETE(m_pMaskPic);

}

void RebelCall::FrameMove()
{

	if (m_iFrame == 2)
	{
		// ���ŏ���҂Ń��x���R�[��������
//#ifdef ROUND_CALL_ON
		se->Play("���x��");
//#endif
	}

	// ��]�̃X�s�[�h
	if (m_iFrame <= REBEL_START)
	{
		m_pCenterCircle.rotate -= 0.06f;

		m_pCenterCircle.scale += 0.075f;

		// ������
		m_pBlackLineL.pos.x -= 16;
		m_pBlackLineR.pos.x += 16;

		// font
		m_pFirstFont.scale += 0.0005f;
		m_pFirstFont.pic->SetScale(m_pFirstFont.scale);
	}
	else if (m_iFrame <= REBEL_END)// ����
	{
		m_pCenterCircle.rotate -= 0.005f;

		// ������
		m_pBlackLineL.pos.x -= 16;
		m_pBlackLineR.pos.x += 16;

		// font
		m_pFirstFont.scale += 0.0005f;
		m_pFirstFont.pic->SetScale(m_pFirstFont.scale);

	}
	else // ���X�g
	{
		m_pCenterCircle.rotate -= 0.08f;

		m_pCenterCircle.scale -= 0.075f;


	}

#ifdef ROUND_CALL_ON
	// �������o��u��
	if (m_iFrame == 48)
	{
		if (m_iRoundNumber == 1)
		{
			se->Play("����");
		}
		else if (m_iRoundNumber == 2)
		{
			se->Play("�c�[");
		}
		else if (m_iRoundNumber == 3)
		{
			se->Play("�X���[");
		}
		else if (m_iRoundNumber == 4)
		{

		}
		
	}
#endif

	if (m_iFrame == REBEL_END)// Font���o�؂�ւ�
	{
		m_pFirstFont.pic->Stop();
		m_pBG.pic->Stop();
		m_pNumber->Stop();
	}

}

void RebelCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;

	// �I�菈��
	if (m_iFrame >= 130)
	{
		m_bActionFlag = false;
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// ���}�X�N�摜�̃X�P�[�����グ��
	if (m_pMaskPic->GetScale() < 15.0f)
	{
		m_pMaskPic->SetScale(m_pMaskPic->GetScale() + 0.3f);
	}


	// �X�V
	m_pCenterCircle.pic->SetAngle(m_pCenterCircle.rotate);
	m_pCenterCircle.pic->SetScale(m_pCenterCircle.scale);
	//m_pBG.pic->SetAngle(m_pBG.rotate);
	m_pBG.pic->SetScale(m_pBG.scale);

	m_pBlackLineL.pic->SetAngle(m_pBlackLineL.rotate);
	m_pBlackLineL.pic->SetScale(m_pBlackLineL.scale);
	//m_pBlackLineL.pic->Setpos(m_pBlackLineL.pos);


	m_pNumber->Update();
	m_pCenterCircle.pic->Update();
	m_pBG.pic->Update();
	//m_pBlackLineL.pic->Update();
	//m_pBlackLineR.pic->Update();
	m_pFirstFont.pic->Update();
	m_pLightRing->Update();
	m_pLightLine->Update();

}

void RebelCall::Render()
{

	
	if (m_bActionFlag == false) return;

	// �}�X�N�p
	MaskScreen();

	
	m_pBG.pic->Render(0, 0,shader2D, "Mask_Add");
	//m_pCenterCircle.pic->Render(0, 0, RS::ADD);
	
	m_pBlackLineL.pic->Render((int)m_pBlackLineL.pos.x, (int)m_pBlackLineL.pos.y);
	m_pBlackLineR.pic->Render((int)m_pBlackLineR.pos.x, (int)m_pBlackLineR.pos.y);

	//m_pLightLine->Render((1280/2)/2, (720/2)/2, RS::ADD);
	//m_pLightLine->Render(0, 0, RS::ADD);
	m_pFirstFont.pic->Render(0, 0);
	m_pNumber->Render((1280 / 2) - 64, (720 / 2), 128, 128, (128 * m_iRoundNumber) , 0, 128, 128);

	//m_pLightRing->Render((1280 / 2) - 256, (720 / 2) - 256, RS::ADD);
	//m_pMaskScreen->Render(0, 0, 1280 / 4, 720 / 4, 0, 0, 1280, 720);

}

void RebelCall::Action()
{
	// ���E���h�̐���
	m_iRoundNumber = 1;

	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�
	m_pCenterCircle.rotate = -2.2f;
	m_pCenterCircle.scale = 0.4f;
	//m_pBG.rotate = -1.5f;
	//m_pBG.scale = 0.4f;

	m_pFirstFont.scale = 1.0f;// font

	m_pBlackLineL.pos.x = (1280 / 2) - 96;
	m_pBlackLineR.pos.x = (1280 / 2) - 32;

	m_pNumber->Action(48);
	m_pCenterCircle.pic->Action();
	m_pBG.pic->Action();
	m_pBlackLineL.pic->Action();
	m_pBlackLineR.pic->Action();

	m_pFirstFont.pic->Action(6);
	m_pLightRing->Action();
	m_pLightLine->Action();

	// �}�X�N�摜
	m_pMaskPic->SetScale(1);


}

void RebelCall::Action(int roundNum)
{
	// ���E���h�̐���
	m_iRoundNumber = roundNum;

	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�
	m_pCenterCircle.rotate = -2.2f;
	m_pCenterCircle.scale = 0.4f;
	//m_pBG.rotate = -1.5f;
	//m_pBG.scale = 0.4f;

	m_pFirstFont.scale = 1.0f;// font

	m_pBlackLineL.pos.x = (1280 / 2) - 96;
	m_pBlackLineR.pos.x = (1280 / 2) -32;

	m_pNumber->Action(48);
	m_pCenterCircle.pic->Action();
	m_pBG.pic->Action();
	m_pBlackLineL.pic->Action();
	m_pBlackLineR.pic->Action();

	m_pFirstFont.pic->Action(6);
	m_pLightRing->Action();
	m_pLightLine->Action();

	// �}�X�N�摜
	m_pMaskPic->SetScale(1);


}

void RebelCall::MaskScreen()
{

	Surface* saveSurface;
	// �܂��͍��̃T�[�t�F�C�X��ۑ�
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	// �}�X�N�ɐ؂�ւ�
	m_pMaskScreen->RenderTarget(0);
	{
		// ��ʃN���A
		tdnView::Clear();
		m_pMaskPic->Render(0, 0);

	}
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//�����_�[�^�[�Q�b�g�̕���

	// ���œ���ꂽ�����}�X�N�Ƃ��đ���
	shader2D->SetMaskScreen(m_pMaskScreen);

}


//+-------------------------------
//	�A�N�V�����R�[��
//+-------------------------------

ActionCall::ActionCall()
{
	// ������
	m_iFrame = 0;
	
	m_pBG.PicClean();
	m_pBG.pic = new tdn2DAnim("Data/UI/Game/RoundCall/ActionBG.png");
	//m_pBG.pic->OrderAlphaMove(END_FRAME, ACTION_START, ACTION_END);
	m_pBG.scale = 1.0f;

	//
	m_pBGRip.PicClean();
	m_pBGRip.pic = new tdn2DAnim("Data/UI/Game/RoundCall/ActionBG.png");
	//m_pBGRip.pic->OrderShrink(12, 1, 2);
	m_pBGRip.pic->OrderRipple(32, 1, 0.01f);
	m_pBGRip.scale = 1.0f;

	// ����
	m_pFirstFont.PicClean();
	m_pFirstFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/action.png");

	// ��
	m_pLightRing = new tdn2DAnim("Data/UI/Game/RoundCall/BlueRing.png");
	m_pLightRing->OrderRipple(24, 1, 0.1f);
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue.png");
	m_pLightLine->OrderAlphaMove(130, ACTION_START, ACTION_END);


}

ActionCall::~ActionCall()
{
	SAFE_DELETE(m_pBG.pic);
	SAFE_DELETE(m_pBGRip.pic);
	SAFE_DELETE(m_pFirstFont.pic);
	SAFE_DELETE(m_pLightRing);
	SAFE_DELETE(m_pLightLine);

}

void ActionCall::FrameMove()
{

	if (m_iFrame == ACTION_START)
	{
//#ifdef ROUND_CALL_ON
		se->Play("�A�N�V����");
//#endif
	}
	else if (m_iFrame <= ACTION_END)// ����
	{

	}
	else // ���X�g
	{

	}

	// �����@�L�����N�^�[�����Ă��ꂦ���I�I�I�I�I�Ă������b�Z�[�W�������ő���
	if (m_iFrame == ACTION_END)
	{

		m_pBG.pic->OrderRipple(8, 1, 0.05f);
		m_pBG.pic->Action();
		
		m_pFirstFont.pic->OrderRipple(8, 1, 0.05f);
		m_pFirstFont.pic->Action();

		// �V�[�����C���Ƀ��b�Z�[�W���M
		MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_ROUNDCALL, nullptr);
	}

}

void ActionCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;

	// �I�菈��
	if (m_iFrame >= END_FRAME)
	{
		m_bActionFlag = false;
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// �X�V
	m_pBG.pic->Update();
	m_pBGRip.pic->Update();

	m_pFirstFont.pic->Update();
	m_pLightRing->Update();
	m_pLightLine->Update();
	
}

void ActionCall::Render()
{
	if (m_bActionFlag == false) return;

	//m_pBG.pic->Render(0, 0, RS::ADD);
	m_pBGRip.pic->Render(0, 0, RS::ADD);
	m_pFirstFont.pic->Render(0, 0);

}

void ActionCall::Action()
{
	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�

	m_pBG.pic->OrderShrink(8, 1, 2);
	m_pBG.pic->Action();

	m_pBGRip.pic->Action(8);

	m_pFirstFont.pic->OrderShrink(8, 1, 10);
	m_pFirstFont.pic->Action();

	m_pLightRing->Action();
	m_pLightLine->Action();

}


//+-------------------------------
//	�t�B�j�b�V���R�[��
//+-------------------------------

FinishCall::FinishCall()
{
	// ������
	m_iFrame = 0;

	m_pBG.PicClean();
	m_pBG.pic = new tdn2DAnim("Data/UI/Game/RoundCall/FinishBG.png");
	//m_pBG.pic->OrderAlphaMove(END_FRAME, ACTION_START, ACTION_END);
	m_pBG.scale = 1.0f;

	//
	m_pBGRip.PicClean();
	m_pBGRip.pic = new tdn2DAnim("Data/UI/Game/RoundCall/FinishBG.png");
	m_pBGRip.pic->OrderRipple(60, 1, 0.01f);
	m_pBGRip.scale = 1.0f;

	// ����
	m_pFont.PicClean();
	m_pFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/Finish.png");
	
	// ���̐�
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue.png");
	m_pLightLine->OrderAlphaMove(END_FRAME, FINISH_START, FINISH_END-8);

}

FinishCall::~FinishCall()
{
	SAFE_DELETE(m_pBG.pic);
	SAFE_DELETE(m_pBGRip.pic);
	SAFE_DELETE(m_pFont.pic);
	SAFE_DELETE(m_pLightLine);
}

void FinishCall::FrameMove()
{

	if (m_iFrame <= FINISH_START)
	{

	}
	else if (m_iFrame <= FINISH_END)// ����
	{

	}
	else // ���X�g
	{

	}

	if (m_iFrame == FINISH_END)
	{

		m_pBG.pic->OrderRipple(4, 1, 0.1f);
		m_pBG.pic->Action();

		m_pFont.pic->OrderRipple(4, 1, 0.1f);
		m_pFont.pic->Action();
	
	}

}

void FinishCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;

	// �I�菈��
	if (m_iFrame >= END_FRAME)
	{
		m_bActionFlag = false;

		// �V�[�����C���ɏI�������ƃ��b�Z�[�W�𑗂�
		MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_FINISHCALL, &m_WinnerID);
		//MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, m_WinnerID, MESSAGE_TYPE::END_FINISHCALL, nullptr);
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// �X�V
	m_pBG.pic->Update();
	m_pBGRip.pic->Update();

	m_pFont.pic->Update();

	m_pLightLine->Update();
}

void FinishCall::Render()
{
	if (m_bActionFlag == false) return;

	m_pBG.pic->Render(0, 0);
	m_pBGRip.pic->Render(0, 0, RS::ADD);
	m_pLightLine->Render(0, 0, RS::ADD);

	m_pFont.pic->Render(0, 0);

}

void FinishCall::Action(ENTITY_ID WinnerID)
{
	m_WinnerID = WinnerID;

	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�

	m_pBG.pic->OrderShrink(8, 1, 2);
	m_pBG.pic->Action();

	m_pBGRip.pic->Action(8);

	m_pFont.pic->OrderShrink(8, 1, 10);
	m_pFont.pic->Action();

	m_pLightLine->Action();

}


//+-------------------------------
//	�I�[�o�[�h���C�u�t�B�j�b�V���R�[��
//+-------------------------------

OverDriveFinishCall::OverDriveFinishCall()
{
	// ������
	m_iFrame = 0;

	m_pBG.PicClean();
	m_pBG.pic = new tdn2DAnim("Data/UI/Game/RoundCall/OverDriveBG.png");
	//m_pBG.pic->OrderAlphaMove(END_FRAME, ACTION_START, ACTION_END);
	m_pBG.scale = 1.0f;

	//
	m_pBGRip.PicClean();
	m_pBGRip.pic = new tdn2DAnim("Data/UI/Game/RoundCall/OverDriveBG.png");
	m_pBGRip.pic->OrderRipple(60, 1, 0.01f);
	m_pBGRip.scale = 1.0f;

	// ����
	m_pFont.PicClean();
	m_pFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/OverDrive.png");

	// ���̐�
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue2.png");
	//m_pLightLine->OrderAlphaMove(OVER_END_FRAME, OVER_FINISH_START, OVER_FINISH_END - 8);
	m_pLightLine->OrderRipple(24, 2, 0.1f);

	// �T�[�N��
	m_pSircle.pic = new tdn2DAnim("Data/UI/Game/RoundCall/OverDriveSircle.png");
	m_pSircle.pic->OrderRipple(24, 1, 0.1f);

}

OverDriveFinishCall::~OverDriveFinishCall()
{
	SAFE_DELETE(m_pBG.pic);
	SAFE_DELETE(m_pBGRip.pic);
	SAFE_DELETE(m_pFont.pic);
	SAFE_DELETE(m_pLightLine);
	SAFE_DELETE(m_pSircle.pic);
}

void OverDriveFinishCall::FrameMove()
{

	if (m_iFrame == OVER_FINISH_START)
	{
		m_pFont.pic->SetScale(1);
		m_pFont.pic->OrderShake(8, 14, 28, 2);
		m_pFont.pic->Action();
	}

	if (m_iFrame <= OVER_FINISH_START)
	{
	}
	else if (m_iFrame <= OVER_FINISH_END)// ����
	{

	}
	else // ���X�g
	{

	}

	if (m_iFrame == OVER_FINISH_END)
	{

		m_pBG.pic->OrderRipple(4, 1, 0.1f);
		m_pBG.pic->Action();

		m_pFont.pic->OrderRipple(4, 1, 0.1f);
		m_pFont.pic->Action();

	}

}

void OverDriveFinishCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;

	// �I�菈��
	if (m_iFrame >= OVER_END_FRAME)
	{
		m_bActionFlag = false;

		// �V�[�����C���ɏI�������ƃ��b�Z�[�W�𑗂�
		MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_FINISHCALL, &m_WinnerID);
		//MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, m_WinnerID, MESSAGE_TYPE::END_FINISHCALL, nullptr);
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// �X�V
	m_pBG.pic->Update();
	m_pBGRip.pic->Update();

	m_pFont.pic->Update();

	m_pLightLine->Update();
	m_pSircle.pic->Update();

}

void OverDriveFinishCall::Render()
{
	if (m_bActionFlag == false) return;

	m_pBG.pic->Render(0, 0);
	m_pBGRip.pic->Render(0, 0, RS::ADD);
	
	m_pLightLine->Render(0, 0, RS::ADD);
	m_pFont.pic->Render(0, 0);
	//m_pFont.pic->Render(0, 0, RS::ADD);
	m_pSircle.pic->Render(128, -96, RS::ADD);

}

void OverDriveFinishCall::Action(ENTITY_ID WinnerID)
{
	m_WinnerID = WinnerID;

	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�

	m_pBG.pic->OrderShrink(18, 1, 12);
	m_pBG.pic->Action();

	m_pBGRip.pic->Action(18);

	m_pFont.pic->OrderShrink(18, 1, 10);
	m_pFont.pic->Action();

	m_pLightLine->Action();

	m_pSircle.pic->Action();

}



//+-------------------------------
//	�^�C���A�b�v�R�[��
//+-------------------------------

TimeUpCall::TimeUpCall()
{
	// ������
	m_iFrame = 0;

	m_pBG.PicClean();
	m_pBG.pic = new tdn2DAnim("Data/UI/Game/RoundCall/TimeUpBG.png");
	//m_pBG.pic->OrderAlphaMove(END_FRAME, ACTION_START, ACTION_END);
	m_pBG.scale = 1.0f;

	//
	m_pBGRip.PicClean();
	m_pBGRip.pic = new tdn2DAnim("Data/UI/Game/RoundCall/TimeUpBG.png");
	m_pBGRip.pic->OrderRipple(60, 1, 0.01f);
	m_pBGRip.scale = 1.0f;

	// ����
	m_pFont.PicClean();
	m_pFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/TimeUp.png");

	// ���̐�
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue2.png");
	//m_pLightLine->OrderAlphaMove(OVER_END_FRAME, OVER_FINISH_START, OVER_FINISH_END - 8);
	m_pLightLine->OrderRipple(24, 2, 0.1f);

	// �T�[�N��
	m_pSircle.pic = new tdn2DAnim("Data/UI/Game/RoundCall/OverDriveSircle.png");
	m_pSircle.pic->OrderRipple(24, 1, 0.1f);

}

TimeUpCall::~TimeUpCall()
{
	SAFE_DELETE(m_pBG.pic);
	SAFE_DELETE(m_pBGRip.pic);
	SAFE_DELETE(m_pFont.pic);
	SAFE_DELETE(m_pLightLine);
	SAFE_DELETE(m_pSircle.pic);
}

void TimeUpCall::FrameMove()
{

	if (m_iFrame == OVER_FINISH_START)
	{
		m_pFont.pic->SetScale(1);
		m_pFont.pic->OrderShake(8, 14, 28, 2);
		m_pFont.pic->Action();
	}

	if (m_iFrame <= OVER_FINISH_START)
	{
	}
	else if (m_iFrame <= OVER_FINISH_END)// ����
	{

	}
	else // ���X�g
	{

	}

	if (m_iFrame == OVER_FINISH_END)
	{

		m_pBG.pic->OrderRipple(4, 1, 0.1f);
		m_pBG.pic->Action();

		m_pFont.pic->OrderRipple(4, 1, 0.1f);
		m_pFont.pic->Action();

	}

}

void TimeUpCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;

	// �I�菈��
	if (m_iFrame >= OVER_END_FRAME)
	{
		m_bActionFlag = false;

		// �V�[�����C���ɏI�������ƃ��b�Z�[�W�𑗂�
		MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_FINISHCALL, &m_WinnerID);
		//MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, m_WinnerID, MESSAGE_TYPE::END_FINISHCALL, nullptr);
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// �X�V
	m_pBG.pic->Update();
	m_pBGRip.pic->Update();

	m_pFont.pic->Update();

	m_pLightLine->Update();
	m_pSircle.pic->Update();

}

void TimeUpCall::Render()
{
	if (m_bActionFlag == false) return;

	m_pBG.pic->Render(0, 0);
	m_pBGRip.pic->Render(0, 0, RS::ADD);

	m_pLightLine->Render(0, 0, RS::ADD);
	m_pFont.pic->Render(0, 0);
	//m_pFont.pic->Render(0, 0, RS::ADD);
	m_pSircle.pic->Render(128, -96, RS::ADD);

}

void TimeUpCall::Action(ENTITY_ID WinnerID)
{
	m_WinnerID = WinnerID;

	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�

	m_pBG.pic->OrderShrink(18, 1, 12);
	m_pBG.pic->Action();

	m_pBGRip.pic->Action(18);

	m_pFont.pic->OrderShrink(18, 1, 10);
	m_pFont.pic->Action();

	m_pLightLine->Action();

	m_pSircle.pic->Action();

}


//+-------------------------------
//	�h���[�R�[��
//+-------------------------------

DrawCall::DrawCall()
{
	// ������
	m_iFrame = 0;

	m_pBG.PicClean();
	m_pBG.pic = new tdn2DAnim("Data/UI/Game/RoundCall/DrawBG.png");
	//m_pBG.pic->OrderAlphaMove(END_FRAME, ACTION_START, ACTION_END);
	m_pBG.scale = 1.0f;

	//
	m_pBGRip.PicClean();
	m_pBGRip.pic = new tdn2DAnim("Data/UI/Game/RoundCall/DrawBG.png");
	m_pBGRip.pic->OrderRipple(60, 1, 0.01f);
	m_pBGRip.scale = 1.0f;

	// ����
	m_pFont.PicClean();
	m_pFont.pic = new tdn2DAnim("Data/UI/Game/RoundCall/Draw.png");

	// ���̐�
	m_pLightLine = new tdn2DAnim("Data/UI/Game/RoundCall/LightBlue2.png");
	//m_pLightLine->OrderAlphaMove(OVER_END_FRAME, OVER_FINISH_START, OVER_FINISH_END - 8);
	m_pLightLine->OrderRipple(24, 2, 0.1f);

	// �T�[�N��
	m_pSircle.pic = new tdn2DAnim("Data/UI/Game/RoundCall/OverDriveSircle.png");
	m_pSircle.pic->OrderRipple(24, 1, 0.1f);

}

DrawCall::~DrawCall()
{
	SAFE_DELETE(m_pBG.pic);
	SAFE_DELETE(m_pBGRip.pic);
	SAFE_DELETE(m_pFont.pic);
	SAFE_DELETE(m_pLightLine);
	SAFE_DELETE(m_pSircle.pic);
}

void DrawCall::FrameMove()
{

	if (m_iFrame == OVER_FINISH_START)
	{
		m_pFont.pic->SetScale(1);
		m_pFont.pic->OrderShake(8, 14, 28, 2);
		m_pFont.pic->Action();
	}

	if (m_iFrame <= OVER_FINISH_START)
	{
	}
	else if (m_iFrame <= OVER_FINISH_END)// ����
	{

	}
	else // ���X�g
	{

	}

	if (m_iFrame == OVER_FINISH_END)
	{

		m_pBG.pic->OrderRipple(4, 1, 0.1f);
		m_pBG.pic->Action();

		m_pFont.pic->OrderRipple(4, 1, 0.1f);
		m_pFont.pic->Action();

	}

}

void DrawCall::Update()
{
	if (m_bActionFlag == false) return;
	m_iFrame++;

	// �I�菈��
	if (m_iFrame >= OVER_END_FRAME)
	{
		m_bActionFlag = false;

		// �V�[�����C���ɏI�������ƃ��b�Z�[�W�𑗂�
		ENTITY_ID l_eNoID = ENTITY_ID::ID_ERROR;
		MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_FINISHCALL, &l_eNoID);
		//MsgMgr->Dispatch(0, ENTITY_ID::ROUND_CALL_MGR, m_WinnerID, MESSAGE_TYPE::END_FINISHCALL, nullptr);
	}

	// �t���[���ɂ��X�V
	FrameMove();

	// �X�V
	m_pBG.pic->Update();
	m_pBGRip.pic->Update();

	m_pFont.pic->Update();

	m_pLightLine->Update();
	m_pSircle.pic->Update();

}

void DrawCall::Render()
{
	if (m_bActionFlag == false) return;

	m_pBG.pic->Render(0, 0);
	m_pBGRip.pic->Render(0, 0, RS::ADD);

	m_pLightLine->Render(0, 0, RS::ADD);
	m_pFont.pic->Render(0, 0);
	//m_pFont.pic->Render(0, 0, RS::ADD);
	m_pSircle.pic->Render(128, -96, RS::ADD);

}

void DrawCall::Action()
{
	m_bActionFlag = true;
	m_iFrame = 0;

	// �����ݒ�

	m_pBG.pic->OrderShrink(18, 1, 12);
	m_pBG.pic->Action();

	m_pBGRip.pic->Action(18);

	m_pFont.pic->OrderShrink(18, 1, 10);
	m_pFont.pic->Action();

	m_pLightLine->Action();

	m_pSircle.pic->Action();

}