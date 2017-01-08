#include "TDNLIB.h"
#include "SceneCollect.h"
#include "SceneCollectState.h"
#include "SceneCollect.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "Window\OptionWindow.h"	// �K�v�ȃE�B���h�E
#include "../Window/GameWindow.h"
#include "Collect\PictureManager.h"	// �C���X�g�p
#include "BaseEntity\Message\MessageDispatcher.h"

//+--------------------
// ��ƌ�����
//+--------------------


/*******************************************************/
//					�Q�[���J�n�X�e�[�g
/*******************************************************/

void SceneCollectState::Intro::Enter(sceneCollect *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	// �������o
	pMain->FirstAction();

}

void SceneCollectState::Intro::Execute(sceneCollect *pMain)
{
	// �^�O���g���t�B�[��I�����Ă���
	if (pMain->GetTagType() == TAG_TYPE::TROPHY)
	{
		// �g���t�B�[�������o
		pMain->TrophyFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		return;
	}
	else if (pMain->GetTagType() == TAG_TYPE::PICTURE)
	{
		// �R���N�g�������o
		pMain->PictureFirstAction();
		pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
		return;
	}
	else
	{
		MyAssert(0, "����ȃ^�O�͂Ȃ�");
	}


}
void SceneCollectState::Intro::Exit(sceneCollect *pMain) {}
void SceneCollectState::Intro::Render(sceneCollect *pMain)
{

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
#endif // _DEBUG

}

bool SceneCollectState::Intro::OnMessage(sceneCollect *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�g���t�B�[�̑I���X�e�[�g
/*******************************************************/

void SceneCollectState::TrophyStep::Enter(sceneCollect *pMain)
{
	// ���݂̃^�O�ԍ��ۑ�
	pMain->SetTagType(TAG_TYPE::TROPHY);
}

void SceneCollectState::TrophyStep::Execute(sceneCollect *pMain)
{

	// �g���t�B�[�X�V
	pMain->TrophyUpdate();


	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}



}

bool SceneCollectState::TrophyStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �g���t�B�[�̑���
	pMain->TrophyCtrl(DeviceID);


	// ����
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("����1");
	}

	// ���ǂ�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // ���̃{�^�����������f�o�C�X��ۑ�
		// ���j���[�ɖ߂�
		pMain->GetFSM()->ChangeState(BackMenuStep::GetInstance());
		bChangedState = true;
	}

	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �R���N�g�������o
		pMain->PictureFirstAction();
		pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
		bChangedState = true;
	}

	return bChangedState;
}

void SceneCollectState::TrophyStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::TrophyStep::Render(sceneCollect *pMain)
{
	// �g���t�B�[�̊G
	pMain->TrophyRender();

	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TrophyStep");

#endif // _DEBUG

}

bool SceneCollectState::TrophyStep::OnMessage(sceneCollect *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�C���X�g�̑I���X�e�[�g
/*******************************************************/

void SceneCollectState::PictureStep::Enter(sceneCollect *pMain)
{
	// ���݂̃^�O�ԍ��ۑ�
	pMain->SetTagType(TAG_TYPE::PICTURE);

	// �摜�ӏܑO�̍����t�F�[�h
	pMain->m_bBlackRect = false;
	//pMain->m_iBlackRectAlpha = 0;
}

void SceneCollectState::PictureStep::Execute(sceneCollect *pMain)
{

	// �C���X�g�X�V
	pMain->PictureUpdate();

	// �����t�F�[�h�n���t���O��ON�ɂȂ��Ă���
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// �ӏ܃X�e�[�g��
			pMain->GetFSM()->ChangeState(PictureWatch::GetInstance());
			return;
		}

		// �t�F�[�h���x
		pMain->m_iBlackRectAlpha += (int)(255 / 24);
		// �N�����v
		if (pMain->m_iBlackRectAlpha >= 255) { pMain->m_iBlackRectAlpha = 255; }

	}
	else
	{
		//+-------------------------------------
		// �t�F�[�h���x
		pMain->m_iBlackRectAlpha -= (int)(255 / 24);
		// �N�����v
		if (pMain->m_iBlackRectAlpha <= 0) { pMain->m_iBlackRectAlpha = 0; }
		//+-------------------------------------

		// �p�b�h���X�V
		int NumDevice(tdnInputManager::GetNumDevice());
		// �p�b�h�����������ĂȂ��Ƃ��p
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}

	}





}

bool SceneCollectState::PictureStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �C���X�g�̑���
	pMain->PictureCtrl(DeviceID);


	// ����ŃC���X�g�ӏ܃X�e�[�g�ցI
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// �������܂��������Ă��Ȃ�������
		if (PlayerDataMgr->m_SecretData.iAllPic[pMain->m_tagPI.iSelectNo] == 0)
		{
			//�����Ŕ�����Ȃ� + ����������΁y�C���X�g�w���X�e�[�g�z��
			if (pMain->GetPictureMgr()->GetPicture(pMain->m_tagPI.iSelectNo)->isRock() == false &&
				(int)PlayerDataMgr->m_PlayerInfo.coin >=  pMain->GetPictureMgr()->GetPicture(pMain->m_tagPI.iSelectNo)->GetPrice())
			{

				// SE�̍Đ�
				se->Play("����1");

				// �C���X�g�w���X�e�[�g��
				pMain->GetFSM()->ChangeState(BuyPictureStep::GetInstance());
				bChangedState = true;
			}

		}
		else 
		{
			// SE�̍Đ�
			se->Play("����1");
			// �ӏ܃X�e�[�g��
			pMain->m_bBlackRect = true;
			bChangedState = true;
		}

	}

	// ���ǂ�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // ���̃{�^�����������f�o�C�X��ۑ�
										// ���j���[�ɖ߂�
		pMain->GetFSM()->ChangeState(BackMenuStep::GetInstance());
		bChangedState = true;
	}

	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �g���t�B�[�������o
		pMain->TrophyFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		bChangedState = true;
	}


	return bChangedState;
}

void SceneCollectState::PictureStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::PictureStep::Render(sceneCollect *pMain)
{
	// �C���X�g�̊G
	pMain->PictureRender();


	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


	// �t�F�[�h
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "PictureStep");

#endif // _DEBUG

}

bool SceneCollectState::PictureStep::OnMessage(sceneCollect *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					�C���X�g�ӏ܃X�e�[�g
/*******************************************************/

void SceneCollectState::PictureWatch::Enter(sceneCollect *pMain)
{
	// �ӏܗp���񏉊���
	pMain->PictureInitExe();

}

void SceneCollectState::PictureWatch::Execute(sceneCollect *pMain)
{

	// �����t�F�[�h�n���t���O��ON�ɂȂ��Ă���
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// �O�̃X�e�[�g�ɖ߂�
			pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
			return;
		}

		//+-------------------------------------
		// �t�F�[�h���x
		pMain->m_iBlackRectAlpha += (int)(255 / 24);
		// �N�����v
		if (pMain->m_iBlackRectAlpha >= 255) { pMain->m_iBlackRectAlpha = 255; }
		//+-------------------------------------
	}
	else 
	{
		//+-------------------------------------
		// �t�F�[�h���x
		pMain->m_iBlackRectAlpha -= (int)(255 / 24);
		// �N�����v
		if (pMain->m_iBlackRectAlpha <= 0) { pMain->m_iBlackRectAlpha = 0; }
		//+-------------------------------------

		// �p�b�h���X�V
		int NumDevice(tdnInputManager::GetNumDevice());
		// �p�b�h�����������ĂȂ��Ƃ��p
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}

		// �N���C���X�g�X�V
		pMain->GetPictureMgr()->UpdateExe();
	}


}

bool SceneCollectState::PictureWatch::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);




	// ���ӏ܃C���X�g�̑���
	pMain->GetPictureMgr()->CtrlExe(DeviceID);



	// ���b�Z�[�W��\��
	if (tdnInput::KeyGet(KEYCODE::KEY_C, DeviceID) == 3)
	{
		if (pMain->m_bWatchInfo == true)
		{
			pMain->m_bWatchInfo = false;
		}
		else
		{
			pMain->m_bWatchInfo = true;
		}
	}


	// ���ǂ�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // ���̃{�^�����������f�o�C�X��ۑ�
											
		// �O�̃X�e�[�g��
		pMain->m_bBlackRect = true;
		bChangedState = true;
	}


	return bChangedState;
}

void SceneCollectState::PictureWatch::Exit(sceneCollect *pMain)
{
	// �摜�̉��
	pMain->GetPictureMgr()->RereaseExe();

}

void SceneCollectState::PictureWatch::Render(sceneCollect *pMain)
{
	// �G
	pMain->PictureRenderExe();
	//pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);

	// �t�F�[�h
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

	//�Ƃ肠�����ӏ܂܂ł̃V�[���J�ڂł������Ƃ͊��摜�̊g��Ƃ��ړ��Ƃ��摜�ǂݍ��݂Ƃ�
	//	��
	//	�w���֘A�ȏ�

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "PictureWatch");

#endif // _DEBUG

}

bool SceneCollectState::PictureWatch::OnMessage(sceneCollect *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				�R���e���c�w���m�F
/*******************************************************/

void SceneCollectState::BuyPictureStep::Enter(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->Action();

}

void SceneCollectState::BuyPictureStep::Execute(sceneCollect *pMain)
{
	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());

	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}
		
}

bool SceneCollectState::BuyPictureStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::OK)
	{	
		// ���̃R���e���c�����
		PICTURE_TYPE eType;
		eType = pMain->GetPictureMgr()->GetSelectType();
		MsgMgr->Dispatch(0, ENTITY_ID::PICTURE_MGR, ENTITY_ID::PICTURE_MGR, MESSAGE_TYPE::PICTURE_GET, &eType);

		// �l�i�������Ă���R�C�������炷
		int l_iPrice = pMain->GetPictureMgr()->GetPicture(pMain->m_tagPI.iSelectNo)->GetPrice();
		PlayerDataMgr->m_PlayerInfo.coin -= l_iPrice;


		// �߂�
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::CANCEL)
	{
		// �߂�
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}

	return bChangedState;
}

void SceneCollectState::BuyPictureStep::Exit(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->End();
}

void SceneCollectState::BuyPictureStep::Render(sceneCollect *pMain)
{
	// �C���X�g�X�e�b�v���痈���ꍇ
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::PictureStep::GetInstance()))
	{
		// �C���X�g�̊G
		pMain->PictureRender();

	}

	// ������
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BuyPictureStep");

#endif // _DEBUG

}

bool SceneCollectState::BuyPictureStep::OnMessage(sceneCollect *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//				���j���[�ɖ߂�
/*******************************************************/

void SceneCollectState::BackMenuStep::Enter(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->Action();

}

void SceneCollectState::BackMenuStep::Execute(sceneCollect *pMain)
{
	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());

	// �t�F�[�h
	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// �p�b�h�����������ĂȂ��Ƃ��p
			if (NumDevice == 0)NumDevice = 1;
			for (int i = 0; i < NumDevice; i++)
			{
				if (PadUpdate(pMain, i)) return;
			}
		}

	}
	else
	{
		// �������Ń��j���[�Ɉړ����܂�
		MainFrameEx->ChangeScene(new sceneMenu());
		return;

	}

}

bool SceneCollectState::BackMenuStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::OK)
	{
		// �t�F�[�h���ă`���[�g���A����
		// �t�F�[�h�A�E�g
		Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::CANCEL)
	{
		// �߂�
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}

	return bChangedState;
}

void SceneCollectState::BackMenuStep::Exit(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->End();
}

void SceneCollectState::BackMenuStep::Render(sceneCollect *pMain)
{
	// �g���t�B�[�X�e�b�v���痈���ꍇ
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::TrophyStep::GetInstance()))
	{	
		// �g���t�B�[�̊G
		pMain->TrophyRender();

	}
	// �C���X�g�X�e�b�v���痈���ꍇ
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::PictureStep::GetInstance()))
	{
		// �C���X�g�̊G
		pMain->PictureRender();

	}

	// ������
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BackMenuStep");

#endif // _DEBUG

}

bool SceneCollectState::BackMenuStep::OnMessage(sceneCollect *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}
