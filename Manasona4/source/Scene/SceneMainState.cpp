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
//					ゲーム開始ステート
/*******************************************************/

void SceneMainState::Intro::Enter(sceneMain *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0x00ffffff);
}
void SceneMainState::Intro::Execute(sceneMain *pMain)
{
	// プレイヤー更新
	PlayerMgr->Update(false);

	// フェード終わったらメインにいく(仮)
	if (Fade::isFadeStop())
	{
		pMain->GetFSM()->ChangeState(Main::GetInstance());
	}
}
void SceneMainState::Intro::Exit(sceneMain *pMain){}
void SceneMainState::Intro::Render(sceneMain *pMain){}

bool SceneMainState::Intro::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					メインステート
/*******************************************************/

void SceneMainState::Main::Enter(sceneMain *pMain){}
void SceneMainState::Main::Execute(sceneMain *pMain)
{
	// プレイヤー更新
	PlayerMgr->Update(true);

	// タイマー更新&タイマーが終了なら終了ステートに行く
	if (TimeMgr->Update())
	{
		pMain->GetFSM()->ChangeState(End::GetInstance());
	}

}
void SceneMainState::Main::Exit(sceneMain *pMain){}
void SceneMainState::Main::Render(sceneMain *pMain){}

bool SceneMainState::Main::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/*******************************************************/
//					終了時ステート
/*******************************************************/

void SceneMainState::End::Enter(sceneMain *pMain)
{
	Fade::Set(Fade::FLAG::FADE_OUT, 4, 0x00ffffff);
}
void SceneMainState::End::Execute(sceneMain *pMain)
{
	// プレイヤー更新
	PlayerMgr->Update(false);

	// フェード終わったらリザルトに行く
	if (Fade::isFadeStop())
	{
		MainFrameEx->ChangeScene(new sceneResult);
	}
}
void SceneMainState::End::Exit(sceneMain *pMain){}
void SceneMainState::End::Render(sceneMain *pMain){}
bool SceneMainState::End::OnMessage(sceneMain *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

