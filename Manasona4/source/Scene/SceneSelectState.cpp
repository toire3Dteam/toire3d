#include "TDNLIB.h"
#include "SceneSelect.h"
#include "SceneSelectState.h"
#include "SceneCollect.h"
#include "SceneVS.h"
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

void SceneSelectState::Intro::Exit(sceneSelect *pMain) 
{

}

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
	// ������
	pMain->m_pPic[sceneSelect::PIC_TYPE::SELECT_INFO]->Action();
}

void SceneSelectState::CharaSelect::Execute(sceneSelect *pMain)
{

	// �����őS��OK�ɂȂ�����X�e�[�W&BGM��
	if (pMain->GetSelectUIMgr()->IsOK())
	{
		pMain->GetFSM()->ChangeState(SceneSelectState::StageAndBGM::GetInstance());
		return;
	}

	/**********************/
	//	�L�����Z���̑���
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

bool SceneSelectState::CharaSelect::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);

	//// �߂�{�^�����������R���g���[���[��������Ԃ���Ȃ�������Ԃ�
	//if (pMain->GetSelectUIMgr()->isFirstStep(DeviceID) == false)return bChangedState;
	//
	//// �~�Ń��j���[�ɖ߂�
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	//{
	//	
	//	// ���̃X�e�[�g��
	//	pMain->GetFSM()->ChangeState(SceneSelectState::BackMenu::GetInstance());
	//	bChangedState = true;
	//}


	return bChangedState;
}

void SceneSelectState::CharaSelect::Exit(sceneSelect *pMain)
{
	pMain->m_pPic[sceneSelect::PIC_TYPE::SELECT_INFO]->Stop();
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
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::BACK_MENU:
	{
		int *senderID = (int*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		pMain->m_iSenderDeviceID = *senderID;
		// �o�b�N���j���[�X�e�[�g��
		pMain->GetFSM()->ChangeState(SceneSelectState::BackMenu::GetInstance());

		return true;
	}	break;
	default:
		break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					Stage&BGM�̑I���X�e�[�g
/*******************************************************/

void SceneSelectState::StageAndBGM::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
	
	// ������
	pMain->m_pPic[sceneSelect::PIC_TYPE::SECOND_SELECT_INFO]->Action();
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
			// -1�ɂȂ�΍Ō�̋Ȃɂ���
			if (pMain->m_iSelectBGMNo < 0)
			{
				pMain->m_iSelectBGMNo = BattleMusicMgr->GetNumMusic() - 1;
			}
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
		{
			pMain->m_iSelectBGMNo++;
			pMain->m_tagSecondSelect.pBGMRightRip->Action();
			// �ő�Ȑ��܂ł�����ŏ��ɖ߂�
			if (pMain->m_iSelectBGMNo >=  BattleMusicMgr->GetNumMusic())
			{
				pMain->m_iSelectBGMNo = 0;
			}
		}
	}
	else
	{
		// ���X�e�[�W�̑I��
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
		{
			pMain->m_iSelectStageNo--;
			pMain->m_tagSecondSelect.pStageLeftRip->Action();
			// -1�ɂȂ�΍Ō�̃X�e�[�W�ɂ���
			if (pMain->m_iSelectStageNo < 0)
			{
				pMain->m_iSelectStageNo = (int)STAGE::MAX;	// -1�ɂ��Ȃ��̂�RANDOM���܂߂Ă��邩��
			}
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
		{
			pMain->m_iSelectStageNo++;
			pMain->m_tagSecondSelect.pStageRightRip->Action();

			// (1/2) �������̕ϐ��Ԉ���Ă����璼�����B
			// �Ō�̃X�e�[�W�܂ł�����ŏ��ɖ߂�
			if (pMain->m_iSelectStageNo > (int)STAGE::MAX)
			{
				pMain->m_iSelectStageNo = 0;
			}
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

	pMain->m_pPic[sceneSelect::PIC_TYPE::SECOND_SELECT_INFO]->Stop();
}

void SceneSelectState::StageAndBGM::Render(sceneSelect *pMain)
{
	pMain->StageAndBGMRender();
	tdnText::Draw(0, 0, 0xffffffff, "StageAndBGM");
	
	//tdnText::Draw(68, 128, 0xffffffff, "m_iSelectStageNo->%d",pMain->m_iSelectStageNo);
	//tdnText::Draw(68, 156, 0xffffffff, "m_iSelectBGMNo->%d",pMain->m_iSelectBGMNo);
	//tdnText::Draw(68, 256, 0xffffffff, "�Ȃ߂�->%s", BattleMusicMgr->GetMusicName(pMain->m_iSelectBGMNo).c_str());
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
		// (TODO) ���őI�������L�����N�^�[�E�p�[�g�i�[�E�X�e�[�W�EBGM��ݒ肷��
		
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter
			= pMain->GetSelectUIMgr()->GetSelectCharacter(SIDE::LEFT);

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter
			= pMain->GetSelectUIMgr()->GetSelectCharacter(SIDE::RIGHT);				

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].ePartner
			= pMain->GetSelectUIMgr()->GetSelectPartner(SIDE::LEFT);

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].ePartner
			= pMain->GetSelectUIMgr()->GetSelectPartner(SIDE::RIGHT);

		SelectDataMgr->Get()->iBattleMusicID=pMain->m_iSelectBGMNo;

		// �X�e�[�W�̓o�^(��RANDOM��0�ɂ��Ă���̂ŁA���ۂ�enum�ɍ��킷�K�v������)
		if (pMain->m_iSelectStageNo == 0)
		{
			int r;
			// �Ȃ��������_����0�`MAX-1�łȂ��l�������Ă����̂ŁA���̒l���o��܂Ŗ������[�v�ɂ���(�Ȃ�ł����Ȃ����񂾂낤�H)
			//do
			//{
				r = tdnRandom::Get(0, (int)STAGE::MAX - 1);
				// �����_������
				SelectDataMgr->Get()->eStage = (STAGE)r;

				switch ((STAGE)r)
				{
				case STAGE::SAND:
					break;
				case STAGE::SYUTEN:
					break;
				case STAGE::SEA:
					break;
				case STAGE::GARDEN:
					break;
				case STAGE::NANASATO_SITY:
					break;
				default:
					MyAssert(0, "�X�e�[�W�ɕs���Ȓl%d", r);
					break;
				}
			//} while (r < 0 || r >= (int)STAGE::MAX);
		}
		else
		{
			SelectDataMgr->Get()->eStage = (STAGE)(pMain->m_iSelectStageNo - 1);	// RANDOM�Ԃ�������Ă���
		}

		MainFrameEX->ChangeScene(new sceneVS);
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


/*******************************************************/
//					�߂�X�e�[�g
/*******************************************************/

void SceneSelectState::BackMenu::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
	pMain->m_pBackMenuTips->Action();

}

void SceneSelectState::BackMenu::Execute(sceneSelect *pMain)
{
	// �Ó]�p
	pMain->m_iRectAlpha += 18;
	if (pMain->m_iRectAlpha >= 156)
	{
		pMain->m_iRectAlpha = 156;
	}

	// (�� ��ŏ���)�ŏ��̑I���֖߂�
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iSenderDeviceID) == 3)
	//{
	//	if (pMain->m_pBackMenuTips->GetStep() == TipsCard::STEP::EXECUTE)
	//	{
	//		// ���̃X�e�[�g��
	//		pMain->GetFSM()->ChangeState(SceneSelectState::CharaSelect::GetInstance());
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

		if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::OK)
		{
			// �t�F�[�h
			Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);
			return;
		}

		if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::CANCEL)
		{
			// �L�����Z���ɖ߂�
			pMain->GetFSM()->ChangeState(SceneSelectState::CharaSelect::GetInstance());
			return;
		}

	}




}

void SceneSelectState::BackMenu::Exit(sceneSelect *pMain) 
{
	pMain->m_iRectAlpha = 0;
	pMain->m_pBackMenuTips->End();
}

void SceneSelectState::BackMenu::Render(sceneSelect *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "BackMenu");
}

bool SceneSelectState::BackMenu::OnMessage(sceneSelect *pMain, const Message & msg)
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

