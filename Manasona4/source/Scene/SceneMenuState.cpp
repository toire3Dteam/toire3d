#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneCollect.h"
#include "SceneSelect.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "Window\OptionWindow.h"	// �K�v�ȃE�B���h�E
#include "../Window/GameWindow.h"
#include "../Window/SystemWindow.h"
#include "../Window/ChallengeWindow.h"
#include "../Window/ChallengeSelectWindow.h"
#include "Challenge\ChallengeManagerManager.h"
#include "SceneVS.h"

//+--------------------
// ��ƌ�����
//+--------------------

// ���j���[��UI�Ǘ��n
#define M_UIMgr pMain->GetMenuUIMgr()
#define C_UIMgr pMain->GetCtrlSelectUI()

/*******************************************************/
//					�Q�[���J�n�X�e�[�g
/*******************************************************/

void SceneMenuState::Intro::Enter(sceneMenu *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0xFFFFFFFF);


}
void SceneMenuState::Intro::Execute(sceneMenu *pMain)
{
	// �ŏ��̑I����
	//if ()
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		return;
	}


}
void SceneMenuState::Intro::Exit(sceneMenu *pMain) {}
void SceneMenuState::Intro::Render(sceneMenu *pMain)
{

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "Intro");
#endif // _DEBUG

}

bool SceneMenuState::Intro::OnMessage(sceneMenu *pMain, const Message & msg)
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
//					�ŏ��̑I���X�e�[�g
/*******************************************************/

void SceneMenuState::FirstStep::Enter(sceneMenu *pMain)
{
	M_UIMgr->InfoAction();
}
void SceneMenuState::FirstStep::Execute(sceneMenu *pMain)
{
	// �ł΂�
	if (tdnInput::KeyGet(KEYCODE::KEY_SPACE, 0) == 3)
	{
		M_UIMgr->Action();
	}

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

	// UI
	M_UIMgr->Update();
}

bool SceneMenuState::FirstStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �A�C�R���؂�ւ�
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
	{
		if (--pMain->m_iSelectNo <= -1)
		{
			pMain->m_iSelectNo = 0;
		}
		else
		{
			pMain->m_pMenuUIMgr->ChangeSelectNo(pMain->m_iSelectNo);

			// SE�̍Đ�
			se->Play("�J�[�\��1");
		}
	}


	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
	{
		if (++pMain->m_iSelectNo >= pMain->m_pMenuUIMgr->GetMaxIcon())
		{
			pMain->m_iSelectNo = pMain->m_pMenuUIMgr->GetMaxIcon() - 1;
		}
		else
		{
			pMain->m_pMenuUIMgr->ChangeSelectNo(pMain->m_iSelectNo);

			// SE�̍Đ�
			se->Play("�J�[�\��1");
		}
	}


	// ����
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		switch (M_UIMgr->GetIconType())
		{
		case MENU_ICON_TYPE::TUTORIAL:
			pMain->GetFSM()->ChangeState(TutorialSelectStep::GetInstance());
			pMain->SetCtrlDevice(DeviceID); // ���`���[�g���A����I�������f�o�C�X��ۑ�
			bChangedState = true;	// ���֐����������̂ŁA�����true�ɂ�����return����悤�ɂ���
			break;
		case MENU_ICON_TYPE::BATTLE:
			pMain->GetFSM()->ChangeState(BattleControllerSelectStep::GetInstance());
			//return true;
			bChangedState = true;	// ���֐����������̂ŁA�����true�ɂ�����return����悤�ɂ���
			break;
		case MENU_ICON_TYPE::TRAINING:
			//M_UIMgr->ActionUp();
			pMain->GetFSM()->ChangeState(TrainingControllerSelectStep::GetInstance());
			// �������Ō��肵���f�o�C�X��ۑ�
			pMain->SetTrainingCtrlDevice(DeviceID);
			bChangedState = true;	// ���֐����������̂ŁA�����true�ɂ�����return����悤�ɂ���
			break;
		case MENU_ICON_TYPE::CHALLENGE:
			pMain->SetPauseDeviceID(DeviceID);// ���|�[�Y���j���[���������l�ۑ�
			pMain->GetFSM()->ChangeState(ChallengeStep::GetInstance());
			bChangedState = true;	// ���֐����������̂ŁA�����true�ɂ�����return����悤�ɂ���
			break;
		case MENU_ICON_TYPE::COLLECT:
			pMain->SetPauseDeviceID(DeviceID);// ���|�[�Y���j���[���������l�ۑ�
			M_UIMgr->Action();
			bChangedState = true;
			MainFrameEX->ChangeScene(new sceneCollect, true);
			break;
		case MENU_ICON_TYPE::OPTION:
			pMain->SetPauseDeviceID(DeviceID);// ���|�[�Y���j���[���������l�ۑ�
			pMain->GetFSM()->ChangeState(OptionStep::GetInstance());
			bChangedState = true;	// ���֐����������̂ŁA�����true�ɂ�����return����悤�ɂ���
			break;
		case MENU_ICON_TYPE::ARRAY_END:
			break;
		default:
			break;
		}

		// SE�̍Đ�
		se->Play("����1");
	}

	return bChangedState;
}

void SceneMenuState::FirstStep::Exit(sceneMenu *pMain)
{
	M_UIMgr->InfoStop();
}

void SceneMenuState::FirstStep::Render(sceneMenu *pMain) 
{
	// �A�C�R��UI
	M_UIMgr->Render();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
#endif // _DEBUG
}

bool SceneMenuState::FirstStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�o�g���̃R���g���[���[�I��
/*******************************************************/

void SceneMenuState::BattleControllerSelectStep::Enter(sceneMenu *pMain)
{
	
	C_UIMgr->Action();
}

void SceneMenuState::BattleControllerSelectStep::Execute(sceneMenu *pMain)
{
	
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

	// UI
	M_UIMgr->Update();

	// �R���g���[���[�Z���N�g
	C_UIMgr->Update();

}

bool SceneMenuState::BattleControllerSelectStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �A�C�R���؂�ւ�
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
	{
		C_UIMgr->PadMoveLeftSide(DeviceID);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
	{
		C_UIMgr->PadMoveRightSide(DeviceID);
	}

	// ���ǂ�
	else if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		bChangedState = true;
	}

	// ������
	else if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// �N�������I�����Ă��Ȃ������灩�T�C�h�Ɉړ�������₳����
		if (C_UIMgr->IsLeftPlayer() == false && C_UIMgr->IsRightPlayer() == false)
		{
			C_UIMgr->PadMoveLeftSide(DeviceID);
			return bChangedState;
		}

		// SE�̍Đ�
		se->Play("����1");

		//else
		{
			
			//[10/30] �����Ł��̏������ƂɒN���ǂ����T�C�h�Ȃ̂������̃V�[���ɓn��

			// �������T�C�h��I�����Ă��āA�������Е����󂢂Ă����ԂȂ�
			// �󂢂Ă�AI���Ɏ����̃f�o�C�X������
			if (C_UIMgr->IsLeftPlayer() == false)
			{
				// (��)�N���I��łȂ��̂�AI�t���O��ON�ɂ��鑀��f�o�C�X�𓯂���
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eAIType = (AI_TYPE)PlayerDataMgr->m_ConfigData.iDifficultyAI;// (TODO)
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			}
			else if (C_UIMgr->IsRightPlayer() == false)
			{

				// (��)�N���I��łȂ��̂�AI�t���O��ON�ɂ���̑���f�o�C�X�𓯂���
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = (AI_TYPE)PlayerDataMgr->m_ConfigData.iDifficultyAI;// (TODO)
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;//
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;// 

			}
			else
			{
				// �S����������
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			}

		
			// �`���[�g���A���ł͂Ȃ�
			SelectDataMgr->Get()->bTutorial = false;

			// �g���[�j���O�ł��Ȃ�
			SelectDataMgr->Get()->bTraining = false;

			// �`�������W�ł��Ȃ�
			SelectDataMgr->Get()->bChallenge= false;

			// (TODO) ���E���h���ݒ� [12/1] �����Ń��E���h�̐ݒ�͂��Ȃ�
			//SelectDataMgr->Get()->iWinRound = 2;

			MainFrameEX->ChangeScene(new sceneSelect, true);
			bChangedState = true;
		}

	}

	return bChangedState;
}

void SceneMenuState::BattleControllerSelectStep::Exit(sceneMenu *pMain)
{
	C_UIMgr->Stop();
}

void SceneMenuState::BattleControllerSelectStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// �R���g���[���[�Z���N�g
	C_UIMgr->Render();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BattleControllerSelectStep");
#endif // _DEBUG
}

bool SceneMenuState::BattleControllerSelectStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�g���[�j���O�̃R���g���[���[�I��
/*******************************************************/

void SceneMenuState::TrainingControllerSelectStep::Enter(sceneMenu *pMain)
{
	
	C_UIMgr->Action();
}

void SceneMenuState::TrainingControllerSelectStep::Execute(sceneMenu *pMain)
{

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

	// UI
	M_UIMgr->Update();

	// �R���g���[���[�Z���N�g
	C_UIMgr->Update();

}

bool SceneMenuState::TrainingControllerSelectStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �A�C�R���؂�ւ�
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
	{
		C_UIMgr->PadMoveLeftSide(DeviceID);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
	{
		C_UIMgr->PadMoveRightSide(DeviceID);
	}

	// ���ǂ�
	else if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		bChangedState = true;
	}

	// ������
	else if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// �N�������I�����Ă��Ȃ������灩�T�C�h�Ɉړ�������₳����
		if (C_UIMgr->IsLeftPlayer() == false && C_UIMgr->IsRightPlayer() == false)
		{
			C_UIMgr->PadMoveLeftSide(DeviceID);
			return bChangedState;
		}

		// SE�̍Đ�
		se->Play("����1");

		// �g���[�j���O�����悤�Ƃ��Ă����l������܂Ő�ւ͐i�܂��Ȃ�
		// �ǂ������g���[�j���O�������R���g���[���[�������Ă��Ȃ��A��蒼��
		if (C_UIMgr->GetLeftPlayer().iPlayerDeviceID != pMain->GetTrainingCtrlDevice() &&
			C_UIMgr->GetRightPlayer().iPlayerDeviceID != pMain->GetTrainingCtrlDevice())
		{
			return bChangedState;
		}

		// �������܂Œʂ�����ǂ��炩�Ƀf�o�C�X���������Ƃ�������
		
		// �������ɗ��K����l�������Ă���
		if (C_UIMgr->GetLeftPlayer().iPlayerDeviceID == pMain->GetTrainingCtrlDevice())
		{
			SelectDataMgr->Get()->eOrderTrainingSide = SIDE::LEFT;// ���g���[�j���O��̃T�C�h������
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;

			// �E��AI��
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = AI_TYPE::PRACTICE_LAND;// (TODO) ���K�pAI
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;
		}
		// �����E�ɗ��K����l�������Ă���
		else if (C_UIMgr->GetRightPlayer().iPlayerDeviceID == pMain->GetTrainingCtrlDevice())
		{
			SelectDataMgr->Get()->eOrderTrainingSide = SIDE::RIGHT;// ���g���[�j���O��̃T�C�h���E��
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;
	
			// ����AI��
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eAIType = AI_TYPE::PRACTICE_LAND;// (TODO) ���K�pAI
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

		}


		//else
		{

			//[10/30] �����Ł��̏������ƂɒN���ǂ����T�C�h�Ȃ̂������̃V�[���ɓn��

			// �������T�C�h��I�����Ă��āA�������Е����󂢂Ă����ԂȂ�
			// �󂢂Ă�AI���Ɏ����̃f�o�C�X������
			//if (C_UIMgr->IsLeftPlayer() == false)
			//{
			//	// (��)�N���I��łȂ��̂�AI�t���O��ON�ɂ��鑀��f�o�C�X�𓯂���
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			//}
			//else if (C_UIMgr->IsRightPlayer() == false)
			//{

			//	// (��)�N���I��łȂ��̂�AI�t���O��ON�ɂ���̑���f�o�C�X�𓯂���
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;//
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;// 

			//}
			//else
			//{
			//	// �S����������
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			//}


			// �`���[�g���A���ł͂Ȃ�
			SelectDataMgr->Get()->bTutorial = false;

			// �g���[�j���O�ł�
			SelectDataMgr->Get()->bTraining = true;

			// �`�������W�ł͂Ȃ�
			SelectDataMgr->Get()->bChallenge= false;

			// (TODO) ���E���h���ݒ� [12/1] �����Ń��E���h�̐ݒ�͂��Ȃ�
			//SelectDataMgr->Get()->iWinRound = 2;

			MainFrameEX->ChangeScene(new sceneSelect, true);
			bChangedState = true;
		}

	}

	return bChangedState;
}

void SceneMenuState::TrainingControllerSelectStep::Exit(sceneMenu *pMain)
{
	C_UIMgr->Stop();
}

void SceneMenuState::TrainingControllerSelectStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// �R���g���[���[�Z���N�g
	C_UIMgr->Render();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TrainingControllerSelectStep");
#endif // _DEBUG
}

bool SceneMenuState::TrainingControllerSelectStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�`���[�g���A���I��
/*******************************************************/

void SceneMenuState::TutorialSelectStep::Enter(sceneMenu *pMain)
{
	pMain->GetTips(TIPS_TYPE::TUTORIAL)->Action();

	

}

void SceneMenuState::TutorialSelectStep::Execute(sceneMenu *pMain)
{
	// �p�b�h���X�V
	const int NumDevice(tdnInputManager::GetNumDevice());

	// �t�F�[�h
	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
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

	}else
	{
		// ��(TODO)�����Ń`���[�g���A���Ɉړ����܂�
		// ���̑O�Ɏ����E����E�X�e�[�W�E�`���[�g���A���t���O�������ŕς��Ă��܂��傤
		SelectDataMgr->Get()->bTutorial = true;
		
		// �g���[�j���O�ł͂Ȃ�
		SelectDataMgr->Get()->bTraining = false;

		// �`�������W�ł͂Ȃ�
		SelectDataMgr->Get()->bChallenge = false;

		SelectDataMgr->Get()->eStage = STAGE::SAND;
		SelectDataMgr->Get()->iBattleMusicID = 6;	// �W�����_���Z���X�e

		//SelectDataMgr->Get()->iWinRound = 0;
		// �g���[�j���O�ɓo�ꂷ��L�����N�^�[�ݒ�������Őݒ肷��
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::NAZENARA;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter = CHARACTER::TEKI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].ePartner = PARTNER::MOKOI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].ePartner = PARTNER::MAYA;

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = AI_TYPE::PRACTICE_LAND;// (TODO)
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = pMain->GetCtrlDevice();
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = pMain->GetCtrlDevice();

		MainFrameEX->ChangeScene(new sceneVS());
		return;
	}

	// UI
	M_UIMgr->Update();
}

bool SceneMenuState::TutorialSelectStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE::TUTORIAL)->GetSelectState() == TipsCard::OK)
	{
		// �t�F�[�h���ă`���[�g���A����
		// �t�F�[�h�A�E�g
		Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE::TUTORIAL)->GetSelectState() == TipsCard::CANCEL)
	{
		// �߂�
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		bChangedState = true;

	}

	return bChangedState;
}

void SceneMenuState::TutorialSelectStep::Exit(sceneMenu *pMain)
{
	pMain->GetTips(TIPS_TYPE::TUTORIAL)->End();
}

void SceneMenuState::TutorialSelectStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// ������
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TutorialSelectStep");
#endif // _DEBUG
}

bool SceneMenuState::TutorialSelectStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�I�v�V�����I��
/*******************************************************/

void SceneMenuState::OptionStep::Enter(sceneMenu *pMain)
{
	// �E�B���h�E�N��
	pMain->GetWindow(WINDOW_TYPE::OPTION)->Action();
}

void SceneMenuState::OptionStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();
	
	// �߂�{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState() == OptionWindow::OPTION_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::OPTION)->Stop();// �E�B���h�E�����
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());	// ���j���[�֖߂�
	}

	// �V�X�e���Ń{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState()
		== OptionWindow::OPTION_STATE::SYSTEM)
	{
		// �V�X�e�����j���[��
		pMain->GetFSM()->ChangeState(SystemWindowStep::GetInstance());
		return;
	}

	// �Q�[���Ń{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState()
		== OptionWindow::OPTION_STATE::GAME)
	{
		// �T�E���h���j���[��
		pMain->GetFSM()->ChangeState(GameWindowStep::GetInstance());
		return;
	}

	// �T�E���h�Ń{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState()
		== OptionWindow::OPTION_STATE::SOUND)
	{
		// �T�E���h���j���[��
		pMain->GetFSM()->ChangeState(SoundWindowStep::GetInstance());
		return;
	}

	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}

}

bool SceneMenuState::OptionStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �I�v�V�����E�B���h�E�̑���
	pMain->GetWindow(WINDOW_TYPE::OPTION)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::OptionStep::Exit(sceneMenu *pMain)
{	
	// ���E�B���h�E��Exit�ŏ����Ă͂����Ȃ�
}

void SceneMenuState::OptionStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// ������
	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// �I���E�B���h�E�̐���
	pMain->GetWindow(WINDOW_TYPE::OPTION)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "OptionStep");
#endif // _DEBUG
}

bool SceneMenuState::OptionStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�`�������W�I��
/*******************************************************/

void SceneMenuState::ChallengeStep::Enter(sceneMenu *pMain)
{
	// �E�B���h�E�N��
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->Action();
}

void SceneMenuState::ChallengeStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// �߂�{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->Stop();// �E�B���h�E�����
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());	// ���j���[�֖߂�
	}

	//+--------------------------------------------
	// �e�L�����N�^�[�Ń{�^������������
	//+--------------------------------------------
	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::AIROU)
	{
		// �L�����N�^�[�̐ݒ�
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::AIROU);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::AIROU;

		// �i���o�[�Z���N�g��
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());	
		return;
	}

	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::TEKI)
	{
		// �L�����N�^�[�̐ݒ�
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::TEKI);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::TEKI;

		// �i���o�[�Z���N�g��
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());
		return;
	}

	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::NAZENARABA)
	{
		// �L�����N�^�[�̐ݒ�
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::NAZENARA);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::NAZENARA;

		// �i���o�[�Z���N�g��
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());
		return;
	}

	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::ARAMITAMA)
	{
		// �L�����N�^�[�̐ݒ�
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::ARAMITAMA);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::ARAMITAMA;

		// �i���o�[�Z���N�g��
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());
		return;
	}


	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}

}

bool SceneMenuState::ChallengeStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �E�B���h�E�̑���
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::ChallengeStep::Exit(sceneMenu *pMain)
{
	// ���E�B���h�E��Exit�ŏ����Ă͂����Ȃ�
}

void SceneMenuState::ChallengeStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// ������
	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// �I���E�B���h�E�̐���
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "ChallengeStep");
#endif // _DEBUG
}

bool SceneMenuState::ChallengeStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�`�������W�i���o�[�I��
/*******************************************************/

void SceneMenuState::ChallengeNoStep::Enter(sceneMenu *pMain)
{
	// �E�B���h�E�N��
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->Action();

	// ���I�����Ă���ԍ���0�Ԃɏ�����
	SelectDataMgr->Get()->iChallengeType = 0;

}

void SceneMenuState::ChallengeNoStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// �t�F�[�h
	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{

			// �߂�{�^������������
			if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->GetChoiceState()
				== ChallengeSelectWindow::CHALLENGE_SELECT_STATE::BACK)
			{
				pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->Stop();// �E�B���h�E�����
				pMain->GetFSM()->ChangeState(ChallengeStep::GetInstance());	// ���j���[�֖߂�
			}

			//+--------------------------------------------
			// ����{�^������������
			//+--------------------------------------------
			if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->GetChoiceState()
				== ChallengeSelectWindow::CHALLENGE_SELECT_STATE::SELECT_CHALLENGE)
			{
				// �����C����
				// �t�F�[�h�A�E�g
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);
				return;
			}



			// (1/18) ���j���[���J�����l��������������
			int l_iDeviceID = pMain->GetPauseDeviceID();

			//+----------------------------------
			//	���̃X�e�[�g�ł̑���
			//+----------------------------------
			{
				if (PadUpdate(pMain, l_iDeviceID)) return;
			}

		}

	}
	else 
	{
		//+--------------------------------------------
		//		�t�F�[�h��
		//+--------------------------------------------

		// �g���[�j���O�E�`���[�g���A���ł͂Ȃ�
		SelectDataMgr->Get()->bTraining = false;
		SelectDataMgr->Get()->bTutorial = false;

		// �`�������W�ł�
		SelectDataMgr->Get()->bChallenge = true;

		SelectDataMgr->Get()->eStage = STAGE::SAND;
		SelectDataMgr->Get()->iBattleMusicID = 6;	// �W�����_���Z���X�e
		
		// �`�������W�ɓo�ꂷ��L�����N�^�[�ݒ�������Őݒ肷��
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter = CHARACTER::TEKI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].ePartner = PARTNER::MOKOI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].ePartner = PARTNER::MOKOI;

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = AI_TYPE::PRACTICE_LAND;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = pMain->GetCtrlDevice();
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = pMain->GetCtrlDevice();

		MainFrameEX->ChangeScene(new sceneVS());

		return;

	}

}

bool SceneMenuState::ChallengeNoStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �E�B���h�E�̑���
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::ChallengeNoStep::Exit(sceneMenu *pMain)
{
	// ���E�B���h�E��Exit�ŏ����Ă͂����Ȃ�
}

void SceneMenuState::ChallengeNoStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// ������
	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// �I���E�B���h�E�̐���
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "ChallengeNoStep");
#endif // _DEBUG
}

bool SceneMenuState::ChallengeNoStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				�V�X�e���E�B���h�E�I��
/*******************************************************/

void SceneMenuState::SystemWindowStep::Enter(sceneMenu *pMain)
{
	// �E�B���h�E�N��
	pMain->GetWindow(WINDOW_TYPE::SYSTEM)->Action();
}

void SceneMenuState::SystemWindowStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// �߂�{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::SYSTEM)->GetChoiceState()
		== SystemWindow::SYSTEM_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::SYSTEM)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}

}

bool SceneMenuState::SystemWindowStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �I���E�B���h�E�̑���
	pMain->GetWindow(WINDOW_TYPE::SYSTEM)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::SystemWindowStep::Exit(sceneMenu *pMain)
{

}

void SceneMenuState::SystemWindowStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// �I���E�B���h�E�̐���
	pMain->GetWindow(WINDOW_TYPE::SYSTEM)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "SystemWindowStep");
#endif // _DEBUG
}

bool SceneMenuState::SystemWindowStep::OnMessage(sceneMenu *pMain, const Message & msg)
{
	return false;
}


/*******************************************************/
//				�Q�[���E�B���h�E�I��
/*******************************************************/

void SceneMenuState::GameWindowStep::Enter(sceneMenu *pMain)
{
	// �T�E���h�E�B���h�E�N��
	pMain->GetWindow(WINDOW_TYPE::GAME)->Action();
}

void SceneMenuState::GameWindowStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// �߂�{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::GAME)->GetChoiceState()
		== GameWindow::GAME_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::GAME)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}
	
}

bool SceneMenuState::GameWindowStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �I���E�B���h�E�̑���
	pMain->GetWindow(WINDOW_TYPE::GAME)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::GameWindowStep::Exit(sceneMenu *pMain)
{

}

void SceneMenuState::GameWindowStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// �I���E�B���h�E�̐���
	pMain->GetWindow(WINDOW_TYPE::GAME)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "GameWindowStep");
#endif // _DEBUG
}

bool SceneMenuState::GameWindowStep::OnMessage(sceneMenu *pMain, const Message & msg)
{
	return false;
}


/*******************************************************/
//				�T�E���h�E�B���h�E�I��
/*******************************************************/

void SceneMenuState::SoundWindowStep::Enter(sceneMenu *pMain)
{
	// �T�E���h�E�B���h�E�N��
	pMain->GetWindow(WINDOW_TYPE::SOUND)->Action();
}

void SceneMenuState::SoundWindowStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();
	
	// �߂�{�^������������
	if (pMain->GetWindow(WINDOW_TYPE::SOUND)->GetChoiceState()
		== SoundWindow::SOUND_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::SOUND)->Stop();	// �E�B���h�E�����
		pMain->GetFSM()->RevertToPreviousState();		// �O�̃X�e�[�g�֖߂�
		return;
	}



	// (1/18) ���j���[���J�����l��������������
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	���̃X�e�[�g�ł̑���
	//+----------------------------------
	// �p�b�h���X�V
	// int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}
	
}

bool SceneMenuState::SoundWindowStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// �I���E�B���h�E�̑���
	pMain->GetWindow(WINDOW_TYPE::SOUND)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::SoundWindowStep::Exit(sceneMenu *pMain)
{

}

void SceneMenuState::SoundWindowStep::Render(sceneMenu *pMain)
{
	// �A�C�R��UI
	M_UIMgr->Render();

	// �I���E�B���h�E�̐���
	pMain->GetWindow(WINDOW_TYPE::SOUND)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "SoundWindowStep");
#endif // _DEBUG
}

bool SceneMenuState::SoundWindowStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
