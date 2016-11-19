#include "TDNLIB.h"
#include "SceneSelect.h"
#include "SceneSelectState.h"
#include "SceneCollect.h"
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
	pMain->GetSelectUIMgr()->FirstAction();

}
void SceneSelectState::Intro::Execute(sceneSelect *pMain)
{
	// �ŏ��̑I����
	if (Fade::isFadeInCompletion())
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
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
//					�ŏ��̑I���X�e�[�g
/*******************************************************/

void SceneSelectState::FirstStep::Enter(sceneSelect *pMain)
{
	
}

void SceneSelectState::FirstStep::Execute(sceneSelect *pMain)
{




	// �����őS��OK�ɂȂ�����X�e�[�W��

}

bool SceneSelectState::FirstStep::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);



	return bChangedState;
}

void SceneSelectState::FirstStep::Exit(sceneSelect *pMain)
{
	//M_UIMgr->InfoStop();
}

void SceneSelectState::FirstStep::Render(sceneSelect *pMain) 
{
	// �A�C�R��UI
	//M_UIMgr->Render();

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
}

bool SceneSelectState::FirstStep::OnMessage(sceneSelect *pMain, const Message & msg)
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

