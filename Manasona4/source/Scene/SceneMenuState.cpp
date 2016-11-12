#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneCollect.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"

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
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
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

			break;
		case MENU_ICON_TYPE::BATTLE:
			pMain->GetFSM()->ChangeState(BattleControllerSelectStep::GetInstance());
			//return true;
			bChangedState = true;	// ���֐����������̂ŁA�����true�ɂ�����return����悤�ɂ���
			break;
		case MENU_ICON_TYPE::TRAINING:
			M_UIMgr->ActionUp();
			break;
		case MENU_ICON_TYPE::COLLECT:
			M_UIMgr->Action();
			bChangedState = true;
			MainFrameEx->ChangeScene(new sceneCollect, true);
			break;
		case MENU_ICON_TYPE::OPTION:

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

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
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

		// �������T�C�h��I�����Ă��āA�������Е����󂢂Ă�����(�f�o�C�XID�����̂�h��)
		else if(C_UIMgr->IsLeftPlayer() == false)
		{
			// 0��������0����Ȃ��悤�ɂ���
			C_UIMgr->PadMoveLeftSide((DeviceID == 0) ? 1 : 0);

			// (��)�N���I��łȂ��̂�AI�t���O��ON�ɂ���
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
		}
		else if (C_UIMgr->IsRightPlayer() == false)
		{
			// 0��������0����Ȃ��悤�ɂ���
			C_UIMgr->PadMoveRightSide((DeviceID == 0) ? 1 : 0);

			// (��)�N���I��łȂ��̂�AI�t���O��ON�ɂ���
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
		}

		// SE�̍Đ�
		se->Play("����1");

		//else
		{
			//[10/30] �����Ł��̏������ƂɒN���ǂ����T�C�h�Ȃ̂������̃V�[���ɓn��
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;//�����̃Q�b�^�[���g���Ă�������
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			MainFrameEx->ChangeScene(new sceneMain, true);
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

	tdnText::Draw(0, 0, 0xffffffff, "BattleControllerSelectStep");
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