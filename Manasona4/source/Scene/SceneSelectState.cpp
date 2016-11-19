#include "TDNLIB.h"
#include "SceneSelect.h"
#include "SceneSelectState.h"
#include "SceneCollect.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"

//+--------------------
// 作業効率化
//+--------------------

// メニューのUI管理系
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()

/*******************************************************/
//					ゲーム開始ステート
/*******************************************************/

void SceneSelectState::Intro::Enter(sceneSelect *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0xFFFFFFFF);

	// 初期の演出
	pMain->GetSelectUIMgr()->FirstAction();

}
void SceneSelectState::Intro::Execute(sceneSelect *pMain)
{
	// 最初の選択へ
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
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//default:
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					最初の選択ステート
/*******************************************************/

void SceneSelectState::FirstStep::Enter(sceneSelect *pMain)
{
	
}

void SceneSelectState::FirstStep::Execute(sceneSelect *pMain)
{




	// ここで全員OKになったらステージへ

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
	// アイコンUI
	//M_UIMgr->Render();

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
}

bool SceneSelectState::FirstStep::OnMessage(sceneSelect *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

