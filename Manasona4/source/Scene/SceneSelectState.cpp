#include "TDNLIB.h"
#include "SceneSelect.h"
#include "SceneSelectState.h"
#include "SceneCollect.h"
#include "SceneMain.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"

//+--------------------
// ��ƌ�����
//+--------------------

// ���j���[��UI�Ǘ��n
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()

/*******************************************************/
//					�Q�[���J�n�X�e�[�g
/*******************************************************/

void SceneSelectState::Intro::Enter(sceneSelect *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0xFFFFFFFF);

	// �����̉��o
	pMain->FirstAction();

}
void SceneSelectState::Intro::Execute(sceneSelect *pMain)
{
	// �ŏ��̑I����
	if (Fade::isFadeInCompletion())
	{
		pMain->GetFSM()->ChangeState(CharaSelect::GetInstance());
		return;
	}


}
void SceneSelectState::Intro::Exit(sceneSelect *pMain) {}
void SceneSelectState::Intro::Render(sceneSelect *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
}

bool SceneSelectState::Intro::OnMessage(sceneSelect *pMain, const Message & msg)
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
//					Chara�̑I���X�e�[�g
/*******************************************************/

void SceneSelectState::CharaSelect::Enter(sceneSelect *pMain)
{
	
}

void SceneSelectState::CharaSelect::Execute(sceneSelect *pMain)
{




	// �����őS��OK�ɂȂ�����X�e�[�W&BGM��
	if (pMain->GetSelectUIMgr()->IsOK())
	{
		pMain->GetFSM()->ChangeState(SceneSelectState::StageAndBGM::GetInstance());
		return;
	}
}

bool SceneSelectState::CharaSelect::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);



	return bChangedState;
}

void SceneSelectState::CharaSelect::Exit(sceneSelect *pMain)
{
	//M_UIMgr->InfoStop();
}

void SceneSelectState::CharaSelect::Render(sceneSelect *pMain)
{
	// �A�C�R��UI
	//M_UIMgr->Render();

	tdnText::Draw(0, 0, 0xffffffff, "CharaSelect");
}

bool SceneSelectState::CharaSelect::OnMessage(sceneSelect *pMain, const Message & msg)
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
//					Stage&BGM�̑I���X�e�[�g
/*******************************************************/

void SceneSelectState::StageAndBGM::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;

}

void SceneSelectState::StageAndBGM::Execute(sceneSelect *pMain)
{

	// �Ó]�p
	pMain->m_iRectAlpha += 18;
	if (pMain->m_iRectAlpha >= 156 )
	{
		pMain->m_iRectAlpha = 156;
	}

	// �N�����߂����������x�L�����Z����
	if (pMain->GetSelectUIMgr()->IsOK() == false)
	{
		pMain->GetFSM()->ChangeState(SceneSelectState::CharaSelect::GetInstance());
		return;
	}

	/**********************/
	//	Stage��BGM����
	/**********************/

	// �p�b�h���X�V
	const int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)
	{
		if (PadUpdate(pMain, 0)) return;
	}
	else
	{
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}
	}
}

bool SceneSelectState::StageAndBGM::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// Stage��BGM�̂ǂ��炩�؂�ւ�
	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
	{	
		pMain->m_tagSecondSelect.bBGMSelect = true;
	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
	{
		pMain->m_tagSecondSelect.bBGMSelect = false;
	}


	if (pMain->m_tagSecondSelect.bBGMSelect == true)
	{
		// ��BGM�̑I��
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
		{
			pMain->m_iSelectBGMNo--;
			pMain->m_tagSecondSelect.pBGMLeftRip->Action();
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
		{
			pMain->m_iSelectBGMNo++;
			pMain->m_tagSecondSelect.pBGMRightRip->Action();
		}
	}
	else
	{
		// ���X�e�[�W�̑I��
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
		{
			pMain->m_iSelectStageNo--;
			pMain->m_tagSecondSelect.pStageLeftRip->Action();
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
		{
			pMain->m_iSelectStageNo++;
			pMain->m_tagSecondSelect.pStageRightRip->Action();
		}
	}

	// ���Ŋ���
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// ���̃X�e�[�g��
		pMain->GetFSM()->ChangeState(SceneSelectState::End::GetInstance());
		bChangedState = true;
	}

	return bChangedState;
}

void SceneSelectState::StageAndBGM::Exit(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
}

void SceneSelectState::StageAndBGM::Render(sceneSelect *pMain)
{
	pMain->StageAndBGMRender();
	tdnText::Draw(0, 0, 0xffffffff, "StageAndBGM");
	
	tdnText::Draw(68, 128, 0xffffffff, "m_iSelectStageNo->%d",pMain->m_iSelectStageNo);
	tdnText::Draw(68, 156, 0xffffffff, "m_iSelectBGMNo->%d",pMain->m_iSelectBGMNo);
}

bool SceneSelectState::StageAndBGM::OnMessage(sceneSelect *pMain, const Message & msg)
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
//					�����i���C���ցj
/*******************************************************/

void SceneSelectState::End::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);

}

void SceneSelectState::End::Execute(sceneSelect *pMain)
{
	// ���C����
	if (Fade::isFadeOutCompletion())
	{
		MainFrameEx->ChangeScene(new sceneMain());
		return;
	}

}

bool SceneSelectState::End::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);

	return bChangedState;
}

void SceneSelectState::End::Exit(sceneSelect *pMain)
{
}

void SceneSelectState::End::Render(sceneSelect *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "End");
}

bool SceneSelectState::End::OnMessage(sceneSelect *pMain, const Message & msg)
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

