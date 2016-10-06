#include "TDNLIB.h"
#include "SceneMainState.h"
#include "../BaseEntity/Message/Message.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../Stage/Stage.h"
#include "../Timer/Timer.h"
#include "../Fade/Fade.h"
#include "../system/FrameworkEx.h"
#include "sceneResult.h"

/*******************************************************/
//					�Q�[���J�n�X�e�[�g
/*******************************************************/

void SceneMainState::Intro::Enter(sceneMain *pMain)
{
	// �t�F�[�h������
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00ffffff);
}
void SceneMainState::Intro::Execute(sceneMain *pMain)
{
	// �v���C���[�X�V
	PlayerMgr->Update(false);

	// �t�F�[�h�I������烁�C���ɂ���(��)
	if (Fade::isFadeStop())
	{
		pMain->GetFSM()->ChangeState(Main::GetInstance());
	}
}
void SceneMainState::Intro::Exit(sceneMain *pMain){}
void SceneMainState::Intro::Render(sceneMain *pMain){}

bool SceneMainState::Intro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					���C���X�e�[�g
/*******************************************************/

void SceneMainState::Main::Enter(sceneMain *pMain){}
void SceneMainState::Main::Execute(sceneMain *pMain)
{
	// �v���C���[�X�V
	PlayerMgr->Update(true);

	// �^�C�}�[�X�V&�^�C�}�[���I���Ȃ�I���X�e�[�g�ɍs��
	if (TimeMgr->Update())
	{
		pMain->GetFSM()->ChangeState(End::GetInstance());
	}

}
void SceneMainState::Main::Exit(sceneMain *pMain){}
void SceneMainState::Main::Render(sceneMain *pMain){}

bool SceneMainState::Main::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					�I�����X�e�[�g
/*******************************************************/

void SceneMainState::End::Enter(sceneMain *pMain)
{
	Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00ffffff);
}
void SceneMainState::End::Execute(sceneMain *pMain)
{
	// �v���C���[�X�V
	PlayerMgr->Update(false);

	// �t�F�[�h�I������烊�U���g�ɍs��
	if (Fade::isFadeStop())
	{
		MainFrameEx->ChangeScene(new sceneResult);
	}
}
void SceneMainState::End::Exit(sceneMain *pMain){}
void SceneMainState::End::Render(sceneMain *pMain){}
bool SceneMainState::End::OnMessage(sceneMain *pMain, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

