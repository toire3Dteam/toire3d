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
#include "Window\OptionWindow.h"	// 必要なウィンドウ
#include "../Window/GameWindow.h"

//+--------------------
// 作業効率化
//+--------------------


/*******************************************************/
//					ゲーム開始ステート
/*******************************************************/

void SceneCollectState::Intro::Enter(sceneCollect *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	// 初期演出
	pMain->FirstAction();

}
void SceneCollectState::Intro::Execute(sceneCollect *pMain)
{
	// 最初の選択へ
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
//					トロフィーの選択ステート
/*******************************************************/

void SceneCollectState::TrophyStep::Enter(sceneCollect *pMain)
{
}

void SceneCollectState::TrophyStep::Execute(sceneCollect *pMain)
{

	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}

	// トロフィー更新
	pMain->TrophyUpdate();


}

bool SceneCollectState::TrophyStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// トロフィーの操作
	pMain->TrophyCtrl(DeviceID);


	// 決定
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// SEの再生
		se->Play("決定1");
	}

	return bChangedState;
}

void SceneCollectState::TrophyStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::TrophyStep::Render(sceneCollect *pMain)
{
	// トロフィーの絵
	pMain->TrophyRender();

	// ★トロフィー用のスクロールバー
	enum { ABUJ = 1 };// 最大の高さを＋１しているので
	float l_fArrowRate = (float)(pMain->m_tagTI.iBottom + ABUJ) / (float)(pMain->m_tagTI.iMaxHeight);
	
	// スクロールする必要があるならスクロールバーを描画
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
	// メッセージタイプ
	//switch (msg.Msg)
	//{

	//default:
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

