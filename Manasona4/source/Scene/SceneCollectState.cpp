#include "TDNLIB.h"
#include "SceneCollect.h"
#include "SceneCollectState.h"
#include "SceneCollect.h"
#include "SceneSelect.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "Window\OptionWindow.h"	// �K�v�ȃE�B���h�E
#include "../Window/GameWindow.h"

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
	// �ŏ��̑I����
	//if ()
	{
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		return;
	}


}
void SceneCollectState::Intro::Exit(sceneCollect *pMain) {}
void SceneCollectState::Intro::Render(sceneCollect *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
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
}

void SceneCollectState::TrophyStep::Execute(sceneCollect *pMain)
{

	// �p�b�h���X�V
	int NumDevice(tdnInputManager::GetNumDevice());
	// �p�b�h�����������ĂȂ��Ƃ��p
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}

	// �g���t�B�[�X�V
	pMain->TrophyUpdate();


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

	return bChangedState;
}

void SceneCollectState::TrophyStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::TrophyStep::Render(sceneCollect *pMain)
{
	// �g���t�B�[�̊G
	pMain->TrophyRender();

	// ���g���t�B�[�p�̃X�N���[���o�[
	enum { ABUJ = 1 };// �ő�̍������{�P���Ă���̂�
	float l_fArrowRate = (float)(pMain->m_tagTI.iBottom + ABUJ) / (float)(pMain->m_tagTI.iMaxHeight);
	
	// �X�N���[������K�v������Ȃ�X�N���[���o�[��`��
	if (l_fArrowRate < 1.0f)
	{
		int l_iScrollY = 384 * (float)(pMain->m_tagTI.iTop) / (float)(pMain->m_tagTI.iMaxHeight);

		pMain->m_pImages[sceneCollect::IMAGE::SCROLL_BAR]->Render((int)pMain->m_vScrollPos.x, (int)pMain->m_vScrollPos.y);
		pMain->m_pImages[sceneCollect::IMAGE::SCROLL_ARROW]->Render
			((int)pMain->m_vScrollPos.x,
				(int)pMain->m_vScrollPos.y + l_iScrollY,
				32, (int)(384 * l_fArrowRate), 0, 0, 32, 384);
	}

	tdnText::Draw(0, 0, 0xffffffff, "TrophyStep");
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

