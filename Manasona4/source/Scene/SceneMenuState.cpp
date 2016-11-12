#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneCollect.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"

//+--------------------
// 作業効率化
//+--------------------

// メニューのUI管理系
#define M_UIMgr pMain->GetMenuUIMgr()
#define C_UIMgr pMain->GetCtrlSelectUI()

/*******************************************************/
//					ゲーム開始ステート
/*******************************************************/

void SceneMenuState::Intro::Enter(sceneMenu *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0xFFFFFFFF);


}
void SceneMenuState::Intro::Execute(sceneMenu *pMain)
{
	// 最初の選択へ
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

void SceneMenuState::FirstStep::Enter(sceneMenu *pMain)
{
	M_UIMgr->InfoAction();
}
void SceneMenuState::FirstStep::Execute(sceneMenu *pMain)
{
	// でばぐ
	if (tdnInput::KeyGet(KEYCODE::KEY_SPACE, 0) == 3)
	{
		M_UIMgr->Action();
	}

	// パッド分更新
	const int NumDevice(tdnInputManager::GetNumDevice());

	// パッド何もささってないとき用
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

	// アイコン切り替え
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
	{
		if (--pMain->m_iSelectNo <= -1)
		{
			pMain->m_iSelectNo = 0;
		}
		else
		{
			pMain->m_pMenuUIMgr->ChangeSelectNo(pMain->m_iSelectNo);

			// SEの再生
			se->Play("カーソル1");
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

			// SEの再生
			se->Play("カーソル1");
		}
	}


	// 決定
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		switch (M_UIMgr->GetIconType())
		{
		case MENU_ICON_TYPE::TUTORIAL:

			break;
		case MENU_ICON_TYPE::BATTLE:
			pMain->GetFSM()->ChangeState(BattleControllerSelectStep::GetInstance());
			//return true;
			bChangedState = true;	// ★関数分けしたので、これをtrueにしたらreturnするようにした
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

		// SEの再生
		se->Play("決定1");
	}

	return bChangedState;
}

void SceneMenuState::FirstStep::Exit(sceneMenu *pMain)
{
	M_UIMgr->InfoStop();
}

void SceneMenuState::FirstStep::Render(sceneMenu *pMain) 
{
	// アイコンUI
	M_UIMgr->Render();

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
}

bool SceneMenuState::FirstStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				バトルのコントローラー選択
/*******************************************************/

void SceneMenuState::BattleControllerSelectStep::Enter(sceneMenu *pMain)
{
	
	C_UIMgr->Action();
}

void SceneMenuState::BattleControllerSelectStep::Execute(sceneMenu *pMain)
{
	
	// パッド分更新
	const int NumDevice(tdnInputManager::GetNumDevice());

	// パッド何もささってないとき用
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

	// コントローラーセレクト
	C_UIMgr->Update();

}

bool SceneMenuState::BattleControllerSelectStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// アイコン切り替え
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
	{
		C_UIMgr->PadMoveLeftSide(DeviceID);
	}

	else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
	{
		C_UIMgr->PadMoveRightSide(DeviceID);
	}

	// もどる
	else if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		bChangedState = true;
	}

	// 仮決定
	else if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// 誰も何も選択していなかったら←サイドに移動させるやさしさ
		if (C_UIMgr->IsLeftPlayer() == false && C_UIMgr->IsRightPlayer() == false)
		{
			C_UIMgr->PadMoveLeftSide(DeviceID);
			return bChangedState;
		}

		// 自分がサイドを選択していて、かつもう片方が空いている状態(デバイスIDが被るのを防ぐ)
		else if(C_UIMgr->IsLeftPlayer() == false)
		{
			// 0だったら0じゃないようにする
			C_UIMgr->PadMoveLeftSide((DeviceID == 0) ? 1 : 0);

			// (★)誰も選んでないのでAIフラグをONにする
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
		}
		else if (C_UIMgr->IsRightPlayer() == false)
		{
			// 0だったら0じゃないようにする
			C_UIMgr->PadMoveRightSide((DeviceID == 0) ? 1 : 0);

			// (★)誰も選んでないのでAIフラグをONにする
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
		}

		// SEの再生
		se->Play("決定1");

		//else
		{
			//[10/30] ここで↑の情報をもとに誰がどっちサイドなのかを次のシーンに渡す
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;//←このゲッターを使ってください
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
	// アイコンUI
	M_UIMgr->Render();

	// コントローラーセレクト
	C_UIMgr->Render();

	tdnText::Draw(0, 0, 0xffffffff, "BattleControllerSelectStep");
}

bool SceneMenuState::BattleControllerSelectStep::OnMessage(sceneMenu *pMain, const Message & msg)
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