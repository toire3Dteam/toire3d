#include "TDNLIB.h"
#include "SelectUI.h"
#include "SelectUIState.h"
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
//					�ŏ��̉��o
/*******************************************************/

void SelectUIState::Intro::Enter(SelectUI *pMain)
{
	// �C���t�H���[�V�����p�̘g��z�u
	pMain->ActionCharaPlate();
	pMain->m_iWaitFrame = 0;
}
void SelectUIState::Intro::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;
	// �ŏ��̑I����
	if (pMain->m_iWaitFrame >= 30)
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		return;
	}
}
void SelectUIState::Intro::Exit(SelectUI *pMain) 
{

}
void SelectUIState::Intro::Render(SelectUI *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
}

bool SelectUIState::Intro::OnMessage(SelectUI *pMain, const Message & msg)
{
	return false;
}


/*******************************************************/
//					�ŏ��̑I���X�e�[�g(�L�����Z��)
/*******************************************************/

void SelectUIState::FirstStep::Enter(SelectUI *pMain)
{

	// ���o
	pMain->ActionChara();	

}
void SelectUIState::FirstStep::Execute(SelectUI *pMain)
{
	// �L�����Z���N�g�̑���
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, pMain->m_iDeviceID) == 3) 
	{
		pMain->m_iSelectCharacterNo--;
		
		
		// �����鏈��
		if (pMain->m_iSelectCharacterNo <= -1)
		{
			pMain->m_iSelectCharacterNo = (int)CHARACTER::END - 1;
		}

		// ���o
		pMain->ActionChara();

	}
	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectCharacterNo++;
		
		// �����鏈��
		if (pMain->m_iSelectCharacterNo >= (int)CHARACTER::END)
		{
			pMain->m_iSelectCharacterNo = 0;
		}

		// ���o
		pMain->ActionChara();

	}
	/**********************************************************/
	

	// �L��������
	if (tdnInput::KeyGet(KEYCODE::KEY_B, pMain->m_iDeviceID) == 3)
	{
		pMain->GetFSM()->ChangeState(MiddleStep::GetInstance());
		return;
	}


}

void SelectUIState::FirstStep::Exit(SelectUI *pMain)
{
	pMain->StopFirstStep();
}

void SelectUIState::FirstStep::Render(SelectUI *pMain)
{

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
}

bool SelectUIState::FirstStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			���Ԃ̑I���X�e�[�g(�L�����ƃp�[�g�i�[�̊�)
/*******************************************************/

void SelectUIState::MiddleStep::Enter(SelectUI *pMain)
{
	// ���o
	pMain->MiddleAction();
	pMain->m_iWaitFrame = 0;

}

void SelectUIState::MiddleStep::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;

	if (pMain->m_iWaitFrame >= 24)
	{
		pMain->m_iWaitFrame = 0;
		pMain->GetFSM()->ChangeState(SecondStep::GetInstance());
		return;
	}

}

void SelectUIState::MiddleStep::Exit(SelectUI *pMain)
{

}

void SelectUIState::MiddleStep::Render(SelectUI *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "MiddleStep");
}

bool SelectUIState::MiddleStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			��ڂ̑I���X�e�[�g(�p�[�g�i�[)
/*******************************************************/

void SelectUIState::SecondStep::Enter(SelectUI *pMain)
{

	// ���o
	pMain->ActionPartner();
	pMain->ActionSecondStep();

}

void SelectUIState::SecondStep::Execute(SelectUI *pMain)
{
	// �y���\�i�Z���N�g�̑���
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectPartnerNo--;


		// �����鏈��
		if (pMain->m_iSelectPartnerNo <= -1)
		{
			pMain->m_iSelectPartnerNo = (int)PARTNER::END - 1;
		}

		// ���o
		pMain->ActionPartner();

	}
	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectPartnerNo++;

		// �����鏈��
		if (pMain->m_iSelectPartnerNo >= (int)PARTNER::END)
		{
			pMain->m_iSelectPartnerNo = 0;
		}

		// ���o
		pMain->ActionPartner();

	}
	/**********************************************************/

	// �L��������
	if (tdnInput::KeyGet(KEYCODE::KEY_B, pMain->m_iDeviceID) == 3)
	{

		pMain->GetFSM()->ChangeState(SecondToOKStep::GetInstance());
		return;
	}
	
	// �߂�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
	{
		pMain->BackSecondStep();
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		return;
	}

}

void SelectUIState::SecondStep::Exit(SelectUI *pMain)
{

}

void SelectUIState::SecondStep::Render(SelectUI *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "SecondStep");
}

bool SelectUIState::SecondStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			SecondToOKStep�I
/*******************************************************/

void SelectUIState::SecondToOKStep::Enter(SelectUI *pMain)
{
	// ���o
	pMain->SecondToOKAction();
	pMain->m_iWaitFrame = 0;
}
void SelectUIState::SecondToOKStep::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;

	if (pMain->m_iWaitFrame >= 24)
	{
		pMain->m_iWaitFrame = 0;
		pMain->GetFSM()->ChangeState(OKStep::GetInstance());
		return;
	}

}

void SelectUIState::SecondToOKStep::Exit(SelectUI *pMain)
{

}

void SelectUIState::SecondToOKStep::Render(SelectUI *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "SecondToOKStep");
}

bool SelectUIState::SecondToOKStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			���������I
/*******************************************************/

void SelectUIState::OKStep::Enter(SelectUI *pMain)
{
	pMain->ActionOKStep();

	// ���������t���O��ON�I
	pMain->m_bOK = true;

}
void SelectUIState::OKStep::Execute(SelectUI *pMain)
{
	// �߂�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
	{
		pMain->BackOKStep();
		pMain->GetFSM()->ChangeState(SecondStep::GetInstance());
		return;
	}

}

void SelectUIState::OKStep::Exit(SelectUI *pMain)
{
	// ���������t���O��OFF
	pMain->m_bOK = false;
}

void SelectUIState::OKStep::Render(SelectUI *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "OKStep");
}

bool SelectUIState::OKStep::OnMessage(SelectUI *pMain, const Message & msg)
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
