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
	else if (pMain->GetTagType() == TAG_TYPE::MOVIE)
	{
		// �R���N�g�������o
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		return;
	}
	else if (pMain->GetTagType() == TAG_TYPE::RECORD)
	{
		// �R���N�g�������o
		pMain->RecordFirstAction();
		pMain->GetFSM()->ChangeState(RecordStep::GetInstance());
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

	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �������o
		pMain->RecordFirstAction();
		pMain->GetFSM()->ChangeState(RecordStep::GetInstance());
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
		pMain->SetCtrlDevice(DeviceID); // ���̃{�^�����������f�o�C�X��ۑ�

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

	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �R���N�g�������o
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		bChangedState = true;
	}

	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �R���N�g�������o
		pMain->MovieFirstAction();
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
	if (tdnInput::KeyGet(KEYCODE::KEY_C, DeviceID) == 3||
		tdnInput::KeyGet(KEYCODE::KEY_D, DeviceID) == 3)
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

		// SE�̍Đ�
		se->Play("�w��");
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
//					���[�r�[�̑I���X�e�[�g
/*******************************************************/

void SceneCollectState::MovieStep::Enter(sceneCollect *pMain)
{
	// ���݂̃^�O�ԍ��ۑ�
	pMain->SetTagType(TAG_TYPE::MOVIE);

	// �摜�ӏܑO�̍����t�F�[�h
	pMain->m_bBlackRect = false;
	//pMain->m_iBlackRectAlpha = 0;
}

void SceneCollectState::MovieStep::Execute(sceneCollect *pMain)
{

	// ���[�r�[�X�V
	pMain->MovieUpdate();

	// �����t�F�[�h�n���t���O��ON�ɂȂ��Ă���
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// �ӏ܃X�e�[�g��
			pMain->GetFSM()->ChangeState(MovieWatch::GetInstance());
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

bool SceneCollectState::MovieStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// ���[�r�[�̑���
	pMain->MovieCtrl(DeviceID);


	// ����Ń��[�r�[�ӏ܃X�e�[�g�ցI
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // ���̃{�^�����������f�o�C�X��ۑ�

		// �������܂��������Ă��Ȃ�������
		if (PlayerDataMgr->m_SecretData.iAllMovie[pMain->m_tagMI.iSelectNo] == 0)
		{
			//�����Ŕ�����Ȃ� + ����������΁y���[�r�[�w���X�e�[�g�z��
			if (pMain->GetMovieMgr()->GetMovie(pMain->m_tagMI.iSelectNo)->isRock() == false &&
				(int)PlayerDataMgr->m_PlayerInfo.coin >= pMain->GetMovieMgr()->GetMovie(pMain->m_tagMI.iSelectNo)->GetPrice())
			{

				// SE�̍Đ�
				se->Play("����1");

				// ���[�r�[�w���X�e�[�g��
				pMain->GetFSM()->ChangeState(BuyMovieStep::GetInstance());
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
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �������o
		pMain->RecordFirstAction();
		pMain->GetFSM()->ChangeState(RecordStep::GetInstance());
		bChangedState = true;
	}

	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
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

void SceneCollectState::MovieStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::MovieStep::Render(sceneCollect *pMain)
{
	// ���[�r�[�̊G
	pMain->MovieRender();


	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


	// �t�F�[�h
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "MovieStep");

#endif // _DEBUG

}

bool SceneCollectState::MovieStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//					���[�r�[�ӏ܃X�e�[�g
/*******************************************************/

void SceneCollectState::MovieWatch::Enter(sceneCollect *pMain)
{
	// BGM�t�F�[�h�A�E�g
	bgm->StopStreamIn(true);

	// �ӏܗp���񏉊���
	pMain->MovieInitExe();

}

void SceneCollectState::MovieWatch::Execute(sceneCollect *pMain)
{

	// �����t�F�[�h�n���t���O��ON�ɂȂ��Ă���
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// �O�̃X�e�[�g�ɖ߂�
			pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
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
		pMain->m_iBlackRectAlpha -= (int)(255 / 6);
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

		// �N�����[�r�[�X�V
		if (pMain->GetMovieMgr()->UpdateExe() == true)
		{
			// �Ō�܂ŗ�����
			pMain->m_bBlackRect = true;
			return;
		}
		
	}


}

bool SceneCollectState::MovieWatch::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);




	// ���ӏ܃��[�r�[�̑���
	pMain->GetMovieMgr()->CtrlExe(DeviceID);



	// ���b�Z�[�W��\��
	if (tdnInput::KeyGet(KEYCODE::KEY_C, DeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_D, DeviceID) == 3)
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

void SceneCollectState::MovieWatch::Exit(sceneCollect *pMain)
{
	// �摜�̉��
	pMain->GetMovieMgr()->RereaseExe();

	// BGM�t�F�[�h�C��
	bgm->PlayStreamIn("DATA/Sound/BGM/System/Collect.ogg");
}

void SceneCollectState::MovieWatch::Render(sceneCollect *pMain)
{
	// �G
	pMain->MovieRenderExe();
	//pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);

	// �t�F�[�h
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

	//�Ƃ肠�����ӏ܂܂ł̃V�[���J�ڂł������Ƃ͊��摜�̊g��Ƃ��ړ��Ƃ��摜�ǂݍ��݂Ƃ�
	//	��
	//	�w���֘A�ȏ�

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "MovieWatch");

#endif // _DEBUG

}

bool SceneCollectState::MovieWatch::OnMessage(sceneCollect *pMain, const Message & msg)
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

void SceneCollectState::BuyMovieStep::Enter(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->Action();

}

void SceneCollectState::BuyMovieStep::Execute(sceneCollect *pMain)
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

bool SceneCollectState::BuyMovieStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::OK)
	{
		// ���̃R���e���c�����
		MOVIE_TYPE eType;
		eType = pMain->GetMovieMgr()->GetSelectType();
		MsgMgr->Dispatch(0, ENTITY_ID::MOVIE_MGR, ENTITY_ID::MOVIE_MGR, MESSAGE_TYPE::MOVIE_GET, &eType);

		// �l�i�������Ă���R�C�������炷
		int l_iPrice = pMain->GetMovieMgr()->GetMovie(pMain->m_tagMI.iSelectNo)->GetPrice();
		PlayerDataMgr->m_PlayerInfo.coin -= l_iPrice;


		// �߂�
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

		// SE�̍Đ�
		se->Play("�w��");
	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::CANCEL)
	{
		// �߂�
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}

	return bChangedState;
}

void SceneCollectState::BuyMovieStep::Exit(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->End();
}

void SceneCollectState::BuyMovieStep::Render(sceneCollect *pMain)
{
	// ���[�r�[�X�e�b�v���痈���ꍇ
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::MovieStep::GetInstance()))
	{
		// ���[�r�[�̊G
		pMain->MovieRender();

	}

	// ������
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BuyMovieStep");

#endif // _DEBUG

}

bool SceneCollectState::BuyMovieStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//					���R�[�h�̑I���X�e�[�g
/*******************************************************/

void SceneCollectState::RecordStep::Enter(sceneCollect *pMain)
{
	// ���݂̃^�O�ԍ��ۑ�
	pMain->SetTagType(TAG_TYPE::RECORD);
}

void SceneCollectState::RecordStep::Execute(sceneCollect *pMain)
{
	// ���R�[�h�̍X�V
	pMain->RecordUpdate();

	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}

}

bool SceneCollectState::RecordStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);


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

		// �������o
		pMain->TrophyFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		bChangedState = true;
	}
	// ���X�e�b�v�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�J�[�\��2");

		// �������o
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		bChangedState = true;
	}




	return bChangedState;
}

void SceneCollectState::RecordStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::RecordStep::Render(sceneCollect *pMain)
{

	// ���R�[�h�̊G
	pMain->RecordRender();


	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


	// �t�F�[�h
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "RecordStep");

#endif // _DEBUG

}

bool SceneCollectState::RecordStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
		MainFrameEX->ChangeScene(new sceneMenu());
		return;

	}

}

bool SceneCollectState::BackMenuStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::SELECT_STATE::OK)
	{
		// �t�F�[�h���ă`���[�g���A����
		// �t�F�[�h�A�E�g
		Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::SELECT_STATE::CANCEL)
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
	// ���[�r�[�X�e�b�v���痈���ꍇ
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::MovieStep::GetInstance()))
	{
		// ���[�r�[�̊G
		pMain->MovieRender();

	}
	// ���R�[�h�X�e�b�v���痈���ꍇ
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::RecordStep::GetInstance()))
	{
		// ���R�[�h�̊G
		pMain->RecordRender();

	}


	// ������
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// ����^�C�g��
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BackMenuStep");
	tdnText::Draw(0, 100, 0xffffffff, "���_%d", (int)pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState());

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
