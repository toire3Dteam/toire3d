#include "TDNLIB.h"
#include "sceneTitle.h"
#include "SceneTitleState.h"
#include "SceneMain.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data/SelectData.h"

//+--------------------
// ��ƌ�����
//+--------------------

// ���j���[��UI�Ǘ��n
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()

// ������`�����OP���X�L�b�v�����(�f�o�b�O���Z�p)
//#define OP_SKIP

/*******************************************************/
//					�Q�[���J�n�X�e�[�g
/*******************************************************/

void SceneTitleState::Intro::Enter(sceneTitle *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN,8);

}
void SceneTitleState::Intro::Execute(sceneTitle *pMain)
{
	// �ŏ��̑I����
	if (Fade::isFadeInCompletion())
	{

#ifdef OP_SKIP

		pMain->GetFSM()->ChangeState(TitleStep::GetInstance());
		return;
#endif

		pMain->GetFSM()->ChangeState(LogoStep::GetInstance());
		return;
	}


}

void SceneTitleState::Intro::Exit(sceneTitle *pMain) 
{
	

}

void SceneTitleState::Intro::Render(sceneTitle *pMain)
{

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "Intro");

#endif // _DEBUG

}

bool SceneTitleState::Intro::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					���S�X�e�[�g
/*******************************************************/

void SceneTitleState::LogoStep::Enter(sceneTitle *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	// ���S�̃A�j���[�V�����J�n
	pMain->m_pLogo->Action();

	// ���u�p
	pMain->m_iGameTimer = 0;

}
void SceneTitleState::LogoStep::Execute(sceneTitle *pMain)
{
	// ���S�X�V
	pMain->m_pLogo->Update();

	// 
	if (Fade::isFadeOutCompletion() == false)
	{

		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{

			pMain->m_iGameTimer++;
			if (pMain->m_iGameTimer == 180)
			{
				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 8);
			}

			// �p�b�h���X�V
			int NumDevice(tdnInputManager::GetNumDevice());
			// �p�b�h�����������ĂȂ��Ƃ��p
			if (NumDevice == 0)NumDevice = 1;
			for (int i = 0; i < NumDevice; i++)
			{
				if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
				{
					// �t�F�[�h�A�E�g
					Fade::Set(Fade::FLAG::FADE_OUT, 8);
					return;
				}
			}

		}

	}
	else
	{
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		return;
	}

}

void SceneTitleState::LogoStep::Exit(sceneTitle *pMain)
{

}

void SceneTitleState::LogoStep::Render(sceneTitle *pMain)
{
	//
	pMain->m_pLogo->Render(0, 0);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "LogoStep");

#endif // _DEBUG

}

bool SceneTitleState::LogoStep::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					���[�r�[�X�e�[�g
/*******************************************************/

void SceneTitleState::MovieStep::Enter(sceneTitle *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	// ���[�r�[�Đ�
	pMain->m_pMovie->Play();
}
void SceneTitleState::MovieStep::Execute(sceneTitle *pMain)
{
	// �e�A�����O�h�~�̋���̍�
	pMain->m_pMovie->Update();
	
	// ���[�r�[���Ō�܂ŗ�����
	if (pMain->m_pMovie->isEndPos())
	{
		// ���[�r�[�~�߂�
		pMain->m_pMovie->Stop();
		// ���j���[��
		pMain->GetFSM()->ChangeState(TitleStep::GetInstance());
		return;
	}
	

	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// ���[�r�[�~�߂�
			pMain->m_pMovie->Stop();

			// �����{�^���������烁�j���[��
			pMain->GetFSM()->ChangeState(TitleStep::GetInstance());
			return;
		}
	}

}

void SceneTitleState::MovieStep::Exit(sceneTitle *pMain)
{

}

void SceneTitleState::MovieStep::Render(sceneTitle *pMain)
{
	// ����摜
	pMain->m_pImages[sceneTitle::IMAGE::MOVIE]->Render(0, 0);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "MovieStep");

#endif // _DEBUG

}

bool SceneTitleState::MovieStep::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					�^�C�g���X�e�[�g
/*******************************************************/

void SceneTitleState::TitleStep::Enter(sceneTitle *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8);


	// �G�j�[�L�[�̃A�j���J�n
	pMain->m_pPreaseAnyButton->Action();

	// BG���[�r�[�Đ�
	pMain->m_pMovieBGLine->Play();

	// ���u�p
	pMain->m_iGameTimer = 0;

	pMain->m_fCloudU = 0;


}
void SceneTitleState::TitleStep::Execute(sceneTitle *pMain)
{
	// �e�A�����O�h�~
	pMain->m_pMovieBGLine->Update();

	// �����X�V
	if (pMain->m_pPreaseAnyButton->GetAction()->IsEnd() == true)
	{	
		// �A�j���̃��[�v
		pMain->m_pPreaseAnyButton->Action();
	}
	
	pMain->m_pPreaseAnyButton->Update();

	// �_�̓���
	pMain->m_fCloudU -= 0.25f;


	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// SE�̍Đ�
			se->Play("�^�C�g���X�^�[�g");

			// �����{�^���������烁�j���[��
			pMain->GetFSM()->ChangeState(End::GetInstance());
			return;
		}
	}

	pMain->m_iGameTimer++;
	if (pMain->m_iGameTimer >= 60 * 120)
	{
		pMain->GetFSM()->ChangeState(Intro::GetInstance());
		return;
	}


	//// �ŏ��̑I����
	//if (Fade::isFadeOutCompletion())
	//{
	//	pMain->GetFSM()->ChangeState(End::GetInstance());
	//	return;
	//}
}

void SceneTitleState::TitleStep::Exit(sceneTitle *pMain)
{

}

void SceneTitleState::TitleStep::Render(sceneTitle *pMain)
{
	// �w�i
	pMain->m_pImages[sceneTitle::IMAGE::BG]->Render(0, 0);

	// ���C��
	pMain->m_pBGLine->Render(0, 0, RS::ADD);

	// �_1
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD1]->Render(-100 + (int)pMain->m_fCloudU , 200);

	// �_2
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD2]->Render(750+(int)pMain->m_fCloudU, 150);

	// �^�C�g��
	pMain->m_pImages[sceneTitle::IMAGE::TITLE]->Render(0, 0);

	// �����{�^��������
	pMain->m_pPreaseAnyButton->Render(448, 576);


#ifdef _DEBUG
	
	// ��
	//tdnPolygon::Rect(200, 200, 400);

	tdnText::Draw(0, 0, 0xffffffff, "TitleStep");

#endif // _DEBUG

}

bool SceneTitleState::TitleStep::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					�����i���j���[�ցj
/*******************************************************/

void SceneTitleState::End::Enter(sceneTitle *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);
}

void SceneTitleState::End::Execute(sceneTitle *pMain)
{
	// ���C����
	if (Fade::isFadeOutCompletion())
	{
		MainFrameEX->ChangeScene(new sceneMenu(),true);
		return;
	}

}

bool SceneTitleState::End::PadUpdate(sceneTitle *pMain, int DeviceID)
{
	bool bChangedState(false);

	return bChangedState;
}

void SceneTitleState::End::Exit(sceneTitle *pMain)
{
}

void SceneTitleState::End::Render(sceneTitle *pMain)
{
	// �w�i
	pMain->m_pImages[sceneTitle::IMAGE::BG]->Render(0, 0);

	// �_1
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD1]->Render(-100 + (int)pMain->m_fCloudU, 200);

	// �_2
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD2]->Render(750 + (int)pMain->m_fCloudU, 150);

	// �^�C�g��
	pMain->m_pImages[sceneTitle::IMAGE::TITLE]->Render(0, 0);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "End");

#endif // _DEBUG

}

bool SceneTitleState::End::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					�߂�X�e�[�g
/*******************************************************/

void SceneTitleState::BackMenu::Enter(sceneTitle *pMain)
{

}

void SceneTitleState::BackMenu::Execute(sceneTitle *pMain)
{

	// (�� ��ŏ���)�ŏ��̑I���֖߂�
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iSenderDeviceID) == 3)
	//{
	//	if (pMain->m_pBackMenuTips->GetStep() == TipsCard::STEP::EXECUTE)
	//	{
	//		// ���̃X�e�[�g��
	//		pMain->GetFSM()->ChangeState(SceneTitleState::CharaSelect::GetInstance());
	//		return;
	//	}
	//}

	if (Fade::isFadeOutCompletion())
	{
		// ���j���[��
		MainFrameEX->ChangeScene(new sceneMenu());
		return;
	}

	// (TODO)�t�F�[�h�̏����Ȃ�Ƃ�����@
	if (Fade::GetMode() != Fade::FADE_OUT)
	{

		//if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::OK)
		//{
		//	// �t�F�[�h
		//	Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);
		//	return;
		//}
		//
		//if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::CANCEL)
		//{
		//	// �L�����Z���ɖ߂�
		//	pMain->GetFSM()->ChangeState(SceneTitleState::CharaSelect::GetInstance());
		//	return;
		//}

	}




}

void SceneTitleState::BackMenu::Exit(sceneTitle *pMain) 
{

}

void SceneTitleState::BackMenu::Render(sceneTitle *pMain)
{

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BackMenu");

#endif // _DEBUG

}

bool SceneTitleState::BackMenu::OnMessage(sceneTitle *pMain, const Message & msg)
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

