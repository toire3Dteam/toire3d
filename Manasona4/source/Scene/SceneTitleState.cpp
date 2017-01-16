#include "TDNLIB.h"
#include "sceneTitle.h"
#include "SceneTitleState.h"
#include "SceneMain.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "../Sound/BattleMusic.h"

//+--------------------
// ��ƌ�����
//+--------------------

// ���j���[��UI�Ǘ��n
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()

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

}
void SceneTitleState::LogoStep::Execute(sceneTitle *pMain)
{
	// ���S�X�V
	pMain->m_pLogo->Update();

	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// �����{�^���������烁�j���[��
			pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
			return;
		}
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

}
void SceneTitleState::MovieStep::Execute(sceneTitle *pMain)
{
	// �e�A�����O�h�~�̋���̍�
	pMain->m_pMovie->Update();
	

	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
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
	pMain->m_pImages[sceneTitle::IMAGE::BACK]->Render(0, 0);

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


}
void SceneTitleState::TitleStep::Execute(sceneTitle *pMain)
{


	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// �����{�^���������烁�j���[��
			pMain->GetFSM()->ChangeState(End::GetInstance());
			return;
		}
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
	

#ifdef _DEBUG
	
	// ��
	tdnPolygon::Rect(200, 200, 400);

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
		MainFrameEx->ChangeScene(new sceneMenu(),true);
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
		MainFrameEx->ChangeScene(new sceneMenu());
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

