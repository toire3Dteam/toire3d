#include "TDNLIB.h"
#include "SelectUI.h"
#include "SelectUIState.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "BaseEntity\Message\Message.h"
#include "BaseEntity\Message\MessageDispatcher.h"
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
	if (pMain->m_iWaitFrame >= 24)
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
//	tdnText::Draw(0, 0, 0xffffffff, "Intro");
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

		// SE�̍Đ�
		se->Play("�J�[�\��2");
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

		// SE�̍Đ�
		se->Play("�J�[�\��2");
	}

	// �J���[�ύX
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectCharacterColor--;

		// �����鏈��
		if (pMain->m_iSelectCharacterColor <= -1)
		{
			pMain->m_iSelectCharacterColor = (int)COLOR_TYPE::MIRROR;
		}

		// ���o
		//pMain->ActionChara();

		// SE�̍Đ�
		se->Play("�J�[�\��2");
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectCharacterColor++;

		// �����鏈��
		if (pMain->m_iSelectCharacterColor > (int)COLOR_TYPE::MIRROR)
		{
			pMain->m_iSelectCharacterColor = 0;
		}

		// ���o
		//pMain->ActionChara();

		// SE�̍Đ�
		se->Play("�J�[�\��2");
	}

	/**********************************************************/
	
	// �~�����ĂȂ�������
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 0)
	{
		// �L��������
		if (tdnInput::KeyGet(KEYCODE::KEY_B, pMain->m_iDeviceID) == 3)
		{
			// SE�̍Đ�
			se->Play("����1");

			// �B���R�}���h
			if (tdnInput::KeyGet(KEYCODE::KEY_L2, pMain->m_iDeviceID) == 1)
			{
				pMain->m_iSelectCharacterNo = (int)(CHARACTER::BALANCE);
				// ���o
				pMain->ActionChara();
			}

			pMain->GetFSM()->ChangeState(MiddleStep::GetInstance());
			return;
		}

		// ���Ń����_���Z���N�g
		if (tdnInput::KeyGet(KEYCODE::KEY_D, pMain->m_iDeviceID) == 3)
		{
			pMain->m_iSelectCharacterNo = tdnRandom::Get(0, (int)(CHARACTER::END) - 1);
			
			// ���o(�L�����̖��O�Ƃ�)
			pMain->ActionChara();

			// SE�̍Đ�
			se->Play("����1");

			pMain->GetFSM()->ChangeState(MiddleStep::GetInstance());
			return;
		}

	}

		// ���j���[�ɖ߂肽���v���C���[�����邱�Ƃ�
		// �V�[�����ɓ`����
		int senderID = pMain->m_iDeviceID;
		
		if (pMain->IsAI() == false)
		{
			// �y�~�����āz�Ń��j���[�ɖ߂�
			if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
			{
				// SE�̍Đ�
				se->Play("�L�����Z��1");

				MsgMgr->Dispatch(0, pMain->GetID(), ENTITY_ID::SCENE_SELECT, MESSAGE_TYPE::BACK_MENU, &senderID);
			}
		}
		else // AI�Ȃ��
		{	
			// �y�~�����z�Ď����̑��삷��v���C���[�֖߂�
			if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
			{
				// SE�̍Đ�
				se->Play("�L�����Z��1");

				// �t�T�C�h�ɑ���@��O�ɖ߂炷����������
				//ENTITY_ID id;
				if (pMain->GetID() == ENTITY_ID::SELECT_UI_LEFT)
				{
					// ���Ȃ�E��
					MsgMgr->Dispatch(0, pMain->GetID(), ENTITY_ID::SELECT_UI_RIGHT, MESSAGE_TYPE::BACK_MENU, &senderID);
				}
				else
				{
					// �E�Ȃ獶
					MsgMgr->Dispatch(0, pMain->GetID(), ENTITY_ID::SELECT_UI_LEFT, MESSAGE_TYPE::BACK_MENU, &senderID);
				}
			}
		}
	
}

void SelectUIState::FirstStep::Exit(SelectUI *pMain)
{
	pMain->StopFirstStep();
}

void SelectUIState::FirstStep::Render(SelectUI *pMain)
{

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");

	tdnText::Draw(0, 200, 0xffaaaaaa, "SelectCharacterColor->%d", pMain->m_iSelectCharacterColor);

#endif // _DEBUG

	
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
	//tdnText::Draw(0, 0, 0xffffffff, "MiddleStep");
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
		// SE�̍Đ�
		se->Play("�J�[�\��2");

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
		// SE�̍Đ�
		se->Play("�J�[�\��2");

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
		// SE�̍Đ�
		se->Play("����1");

		pMain->GetFSM()->ChangeState(SecondToOKStep::GetInstance());
		return;
	}
	
	// ���Ń����_���Z���N�g
	if (tdnInput::KeyGet(KEYCODE::KEY_D, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectPartnerNo = tdnRandom::Get(0, (int)(PARTNER::END) - 1);
		
		// ���o(�p�[�g�i�[�̖��O)
		pMain->ActionPartner();

		// SE�̍Đ�
		se->Play("����1");

		pMain->GetFSM()->ChangeState(SecondToOKStep::GetInstance());
		return;
	}

	// �߂�
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
	{
		// SE�̍Đ�
		se->Play("�L�����Z��1");

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
	//tdnText::Draw(0, 0, 0xffffffff, "SecondStep");
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
	//tdnText::Draw(0, 0, 0xffffffff, "SecondToOKStep");
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
	//tdnText::Draw(0, 0, 0xffffffff, "OKStep");
}

bool SelectUIState::OKStep::OnMessage(SelectUI *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::BACK_MENU:
		// AI����߂�Ƃ������b�Z�[�W���Ƃǂ�����B
		pMain->BackOKStep();
		pMain->GetFSM()->ChangeState(OKToSecondStep::GetInstance());

		return true;
		break;
	default:
		break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//			������������p�[�g�i�[�I�тɖ߂�
/*******************************************************/

void SelectUIState::OKToSecondStep::Enter(SelectUI *pMain)
{
	pMain->m_iWaitFrame = 0;
}
void SelectUIState::OKToSecondStep::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;
	// �p�[�g�i�[�̑I����
	if (pMain->m_iWaitFrame >= 2)
	{
		pMain->GetFSM()->ChangeState(SecondStep::GetInstance());
		return;
	}
}

void SelectUIState::OKToSecondStep::Exit(SelectUI *pMain)
{
}

void SelectUIState::OKToSecondStep::Render(SelectUI *pMain)
{
	//tdnText::Draw(0, 0, 0xffffffff, "SecondToOKStep");
}

bool SelectUIState::OKToSecondStep::OnMessage(SelectUI *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::BACK_MENU:
	//	// AI����߂�Ƃ������b�Z�[�W���Ƃǂ�����B
	//	pMain->BackOKStep();
	//	pMain->GetFSM()->ChangeState(SecondStep::GetInstance());

	//	return true;
	//	break;
	//default:
	//	break;
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}
