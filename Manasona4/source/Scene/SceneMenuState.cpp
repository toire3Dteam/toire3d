#include "TDNLIB.h"
#include "SceneMenu.h"
#include "SceneMenuState.h"
#include "SceneCollect.h"
#include "SceneSelect.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "Window\OptionWindow.h"	// 必要なウィンドウ
#include "../Window/GameWindow.h"
#include "../Window/SystemWindow.h"
#include "../Window/ChallengeWindow.h"
#include "../Window/ChallengeSelectWindow.h"
#include "Challenge\ChallengeManagerManager.h"
#include "SceneVS.h"

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

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "Intro");
#endif // _DEBUG

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
			pMain->GetFSM()->ChangeState(TutorialSelectStep::GetInstance());
			pMain->SetCtrlDevice(DeviceID); // ★チュートリアルを選択したデバイスを保存
			bChangedState = true;	// ★関数分けしたので、これをtrueにしたらreturnするようにした
			break;
		case MENU_ICON_TYPE::BATTLE:
			pMain->GetFSM()->ChangeState(BattleControllerSelectStep::GetInstance());
			//return true;
			bChangedState = true;	// ★関数分けしたので、これをtrueにしたらreturnするようにした
			break;
		case MENU_ICON_TYPE::TRAINING:
			//M_UIMgr->ActionUp();
			pMain->GetFSM()->ChangeState(TrainingControllerSelectStep::GetInstance());
			// ★ここで決定したデバイスを保存
			pMain->SetTrainingCtrlDevice(DeviceID);
			bChangedState = true;	// ★関数分けしたので、これをtrueにしたらreturnするようにした
			break;
		case MENU_ICON_TYPE::CHALLENGE:
			pMain->SetPauseDeviceID(DeviceID);// ★ポーズメニューを押した人保存
			pMain->GetFSM()->ChangeState(ChallengeStep::GetInstance());
			bChangedState = true;	// ★関数分けしたので、これをtrueにしたらreturnするようにした
			break;
		case MENU_ICON_TYPE::COLLECT:
			pMain->SetPauseDeviceID(DeviceID);// ★ポーズメニューを押した人保存
			M_UIMgr->Action();
			bChangedState = true;
			MainFrameEX->ChangeScene(new sceneCollect, true);
			break;
		case MENU_ICON_TYPE::OPTION:
			pMain->SetPauseDeviceID(DeviceID);// ★ポーズメニューを押した人保存
			pMain->GetFSM()->ChangeState(OptionStep::GetInstance());
			bChangedState = true;	// ★関数分けしたので、これをtrueにしたらreturnするようにした
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

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");
#endif // _DEBUG
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

		// SEの再生
		se->Play("決定1");

		//else
		{
			
			//[10/30] ここで↑の情報をもとに誰がどっちサイドなのかを次のシーンに渡す

			// 自分がサイドを選択していて、かつもう片方が空いている状態なら
			// 空いてるAI側に自分のデバイスを入れる
			if (C_UIMgr->IsLeftPlayer() == false)
			{
				// (★)誰も選んでないのでAIフラグをONにする操作デバイスを同じに
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eAIType = (AI_TYPE)PlayerDataMgr->m_ConfigData.iDifficultyAI;// (TODO)
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			}
			else if (C_UIMgr->IsRightPlayer() == false)
			{

				// (★)誰も選んでないのでAIフラグをONにするの操作デバイスを同じに
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = (AI_TYPE)PlayerDataMgr->m_ConfigData.iDifficultyAI;// (TODO)
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;//
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;// 

			}
			else
			{
				// 全員入ったら
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;
				SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			}

		
			// チュートリアルではない
			SelectDataMgr->Get()->bTutorial = false;

			// トレーニングでもない
			SelectDataMgr->Get()->bTraining = false;

			// チャレンジでもない
			SelectDataMgr->Get()->bChallenge= false;

			// (TODO) ラウンド数設定 [12/1] ここでラウンドの設定はしない
			//SelectDataMgr->Get()->iWinRound = 2;

			MainFrameEX->ChangeScene(new sceneSelect, true);
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

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BattleControllerSelectStep");
#endif // _DEBUG
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

/*******************************************************/
//				トレーニングのコントローラー選択
/*******************************************************/

void SceneMenuState::TrainingControllerSelectStep::Enter(sceneMenu *pMain)
{
	
	C_UIMgr->Action();
}

void SceneMenuState::TrainingControllerSelectStep::Execute(sceneMenu *pMain)
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

bool SceneMenuState::TrainingControllerSelectStep::PadUpdate(sceneMenu *pMain, int DeviceID)
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

		// SEの再生
		se->Play("決定1");

		// トレーニングをしようとしていた人が入るまで先へは進ませない
		// どっちもトレーニングしたいコントローラーが入っていない、やり直し
		if (C_UIMgr->GetLeftPlayer().iPlayerDeviceID != pMain->GetTrainingCtrlDevice() &&
			C_UIMgr->GetRightPlayer().iPlayerDeviceID != pMain->GetTrainingCtrlDevice())
		{
			return bChangedState;
		}

		// ★ここまで通ったらどちらかにデバイスが入ったということ
		
		// もし左に練習する人が入ってたら
		if (C_UIMgr->GetLeftPlayer().iPlayerDeviceID == pMain->GetTrainingCtrlDevice())
		{
			SelectDataMgr->Get()->eOrderTrainingSide = SIDE::LEFT;// ★トレーニング主のサイドを左に
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;

			// 右をAIに
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = AI_TYPE::PRACTICE_LAND;// (TODO) 練習用AI
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;
		}
		// もし右に練習する人が入ってたら
		else if (C_UIMgr->GetRightPlayer().iPlayerDeviceID == pMain->GetTrainingCtrlDevice())
		{
			SelectDataMgr->Get()->eOrderTrainingSide = SIDE::RIGHT;// ★トレーニング主のサイドを右に
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;
	
			// 左をAIに
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eAIType = AI_TYPE::PRACTICE_LAND;// (TODO) 練習用AI
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

		}


		//else
		{

			//[10/30] ここで↑の情報をもとに誰がどっちサイドなのかを次のシーンに渡す

			// 自分がサイドを選択していて、かつもう片方が空いている状態なら
			// 空いてるAI側に自分のデバイスを入れる
			//if (C_UIMgr->IsLeftPlayer() == false)
			//{
			//	// (★)誰も選んでないのでAIフラグをONにする操作デバイスを同じに
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = true;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			//}
			//else if (C_UIMgr->IsRightPlayer() == false)
			//{

			//	// (★)誰も選んでないのでAIフラグをONにするの操作デバイスを同じに
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;//
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;// 

			//}
			//else
			//{
			//	// 全員入ったら
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = false;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = C_UIMgr->GetLeftPlayer().iPlayerDeviceID;
			//	SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = C_UIMgr->GetRightPlayer().iPlayerDeviceID;

			//}


			// チュートリアルではない
			SelectDataMgr->Get()->bTutorial = false;

			// トレーニングです
			SelectDataMgr->Get()->bTraining = true;

			// チャレンジではない
			SelectDataMgr->Get()->bChallenge= false;

			// (TODO) ラウンド数設定 [12/1] ここでラウンドの設定はしない
			//SelectDataMgr->Get()->iWinRound = 2;

			MainFrameEX->ChangeScene(new sceneSelect, true);
			bChangedState = true;
		}

	}

	return bChangedState;
}

void SceneMenuState::TrainingControllerSelectStep::Exit(sceneMenu *pMain)
{
	C_UIMgr->Stop();
}

void SceneMenuState::TrainingControllerSelectStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// コントローラーセレクト
	C_UIMgr->Render();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TrainingControllerSelectStep");
#endif // _DEBUG
}

bool SceneMenuState::TrainingControllerSelectStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				チュートリアル選択
/*******************************************************/

void SceneMenuState::TutorialSelectStep::Enter(sceneMenu *pMain)
{
	pMain->GetTips(TIPS_TYPE::TUTORIAL)->Action();

	

}

void SceneMenuState::TutorialSelectStep::Execute(sceneMenu *pMain)
{
	// パッド分更新
	const int NumDevice(tdnInputManager::GetNumDevice());

	// フェード
	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
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
		}

	}else
	{
		// ★(TODO)ここでチュートリアルに移動します
		// その前に自分・相手・ステージ・チュートリアルフラグをここで変えてやりましょう
		SelectDataMgr->Get()->bTutorial = true;
		
		// トレーニングではない
		SelectDataMgr->Get()->bTraining = false;

		// チャレンジではない
		SelectDataMgr->Get()->bChallenge = false;

		SelectDataMgr->Get()->eStage = STAGE::SAND;
		SelectDataMgr->Get()->iBattleMusicID = 6;	// ジャルダンセレステ

		//SelectDataMgr->Get()->iWinRound = 0;
		// トレーニングに登場するキャラクター設定をここで設定する
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::NAZENARA;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter = CHARACTER::TEKI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].ePartner = PARTNER::MOKOI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].ePartner = PARTNER::MAYA;

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = AI_TYPE::PRACTICE_LAND;// (TODO)
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = pMain->GetCtrlDevice();
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = pMain->GetCtrlDevice();

		MainFrameEX->ChangeScene(new sceneVS());
		return;
	}

	// UI
	M_UIMgr->Update();
}

bool SceneMenuState::TutorialSelectStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE::TUTORIAL)->GetSelectState() == TipsCard::OK)
	{
		// フェードしてチュートリアルへ
		// フェードアウト
		Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE::TUTORIAL)->GetSelectState() == TipsCard::CANCEL)
	{
		// 戻る
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		bChangedState = true;

	}

	return bChangedState;
}

void SceneMenuState::TutorialSelectStep::Exit(sceneMenu *pMain)
{
	pMain->GetTips(TIPS_TYPE::TUTORIAL)->End();
}

void SceneMenuState::TutorialSelectStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 黒い板
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TutorialSelectStep");
#endif // _DEBUG
}

bool SceneMenuState::TutorialSelectStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				オプション選択
/*******************************************************/

void SceneMenuState::OptionStep::Enter(sceneMenu *pMain)
{
	// ウィンドウ起動
	pMain->GetWindow(WINDOW_TYPE::OPTION)->Action();
}

void SceneMenuState::OptionStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();
	
	// 戻るボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState() == OptionWindow::OPTION_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::OPTION)->Stop();// ウィンドウを閉じる
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());	// メニューへ戻る
	}

	// システムでボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState()
		== OptionWindow::OPTION_STATE::SYSTEM)
	{
		// システムメニューへ
		pMain->GetFSM()->ChangeState(SystemWindowStep::GetInstance());
		return;
	}

	// ゲームでボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState()
		== OptionWindow::OPTION_STATE::GAME)
	{
		// サウンドメニューへ
		pMain->GetFSM()->ChangeState(GameWindowStep::GetInstance());
		return;
	}

	// サウンドでボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::OPTION)->GetChoiceState()
		== OptionWindow::OPTION_STATE::SOUND)
	{
		// サウンドメニューへ
		pMain->GetFSM()->ChangeState(SoundWindowStep::GetInstance());
		return;
	}

	// (1/18) メニューを開いた人だけが動かせる
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	このステートでの操作
	//+----------------------------------
	// パッド分更新
	// int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}

}

bool SceneMenuState::OptionStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// オプションウィンドウの操作
	pMain->GetWindow(WINDOW_TYPE::OPTION)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::OptionStep::Exit(sceneMenu *pMain)
{	
	// ★ウィンドウはExitで消してはいけない
}

void SceneMenuState::OptionStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 黒い板
	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 選択ウィンドウの説明
	pMain->GetWindow(WINDOW_TYPE::OPTION)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "OptionStep");
#endif // _DEBUG
}

bool SceneMenuState::OptionStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				チャレンジ選択
/*******************************************************/

void SceneMenuState::ChallengeStep::Enter(sceneMenu *pMain)
{
	// ウィンドウ起動
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->Action();
}

void SceneMenuState::ChallengeStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// 戻るボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->Stop();// ウィンドウを閉じる
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());	// メニューへ戻る
	}

	//+--------------------------------------------
	// 各キャラクターでボタンを押したら
	//+--------------------------------------------
	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::AIROU)
	{
		// キャラクターの設定
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::AIROU);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::AIROU;

		// ナンバーセレクトへ
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());	
		return;
	}

	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::TEKI)
	{
		// キャラクターの設定
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::TEKI);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::TEKI;

		// ナンバーセレクトへ
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());
		return;
	}

	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::NAZENARABA)
	{
		// キャラクターの設定
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::NAZENARA);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::NAZENARA;

		// ナンバーセレクトへ
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());
		return;
	}

	if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->GetChoiceState()
		== ChallengeWindow::CHALLENGE_STATE::ARAMITAMA)
	{
		// キャラクターの設定
		ChallengeMgrMgr->SetSelectCharaType(CHARACTER::ARAMITAMA);
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter = CHARACTER::ARAMITAMA;

		// ナンバーセレクトへ
		pMain->GetFSM()->ChangeState(ChallengeNoStep::GetInstance());
		return;
	}


	// (1/18) メニューを開いた人だけが動かせる
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	このステートでの操作
	//+----------------------------------
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}

}

bool SceneMenuState::ChallengeStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// ウィンドウの操作
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::ChallengeStep::Exit(sceneMenu *pMain)
{
	// ★ウィンドウはExitで消してはいけない
}

void SceneMenuState::ChallengeStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 黒い板
	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 選択ウィンドウの説明
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "ChallengeStep");
#endif // _DEBUG
}

bool SceneMenuState::ChallengeStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				チャレンジナンバー選択
/*******************************************************/

void SceneMenuState::ChallengeNoStep::Enter(sceneMenu *pMain)
{
	// ウィンドウ起動
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->Action();

	// ★選択している番号は0番に初期化
	SelectDataMgr->Get()->iChallengeType = 0;

}

void SceneMenuState::ChallengeNoStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// フェード
	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{

			// 戻るボタンを押したら
			if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->GetChoiceState()
				== ChallengeSelectWindow::CHALLENGE_SELECT_STATE::BACK)
			{
				pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->Stop();// ウィンドウを閉じる
				pMain->GetFSM()->ChangeState(ChallengeStep::GetInstance());	// メニューへ戻る
			}

			//+--------------------------------------------
			// 決定ボタンを押したら
			//+--------------------------------------------
			if (pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->GetChoiceState()
				== ChallengeSelectWindow::CHALLENGE_SELECT_STATE::SELECT_CHALLENGE)
			{
				// ★メインへ
				// フェードアウト
				Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);
				return;
			}



			// (1/18) メニューを開いた人だけが動かせる
			int l_iDeviceID = pMain->GetPauseDeviceID();

			//+----------------------------------
			//	このステートでの操作
			//+----------------------------------
			{
				if (PadUpdate(pMain, l_iDeviceID)) return;
			}

		}

	}
	else 
	{
		//+--------------------------------------------
		//		フェード後
		//+--------------------------------------------

		// トレーニング・チュートリアルではない
		SelectDataMgr->Get()->bTraining = false;
		SelectDataMgr->Get()->bTutorial = false;

		// チャレンジです
		SelectDataMgr->Get()->bChallenge = true;

		SelectDataMgr->Get()->eStage = STAGE::SAND;
		SelectDataMgr->Get()->iBattleMusicID = 6;	// ジャルダンセレステ
		
		// チャレンジに登場するキャラクター設定をここで設定する
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter = CHARACTER::TEKI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].ePartner = PARTNER::MOKOI;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].ePartner = PARTNER::MOKOI;

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI = false;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI = true;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eAIType = AI_TYPE::PRACTICE_LAND;
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID = pMain->GetCtrlDevice();
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID = pMain->GetCtrlDevice();

		MainFrameEX->ChangeScene(new sceneVS());

		return;

	}

}

bool SceneMenuState::ChallengeNoStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// ウィンドウの操作
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::ChallengeNoStep::Exit(sceneMenu *pMain)
{
	// ★ウィンドウはExitで消してはいけない
}

void SceneMenuState::ChallengeNoStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 黒い板
	//tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 選択ウィンドウの説明
	pMain->GetWindow(WINDOW_TYPE::CHALLENGE_SELECT)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "ChallengeNoStep");
#endif // _DEBUG
}

bool SceneMenuState::ChallengeNoStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
//				システムウィンドウ選択
/*******************************************************/

void SceneMenuState::SystemWindowStep::Enter(sceneMenu *pMain)
{
	// ウィンドウ起動
	pMain->GetWindow(WINDOW_TYPE::SYSTEM)->Action();
}

void SceneMenuState::SystemWindowStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// 戻るボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::SYSTEM)->GetChoiceState()
		== SystemWindow::SYSTEM_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::SYSTEM)->Stop();	// ウィンドウを閉じる
		pMain->GetFSM()->RevertToPreviousState();		// 前のステートへ戻る
		return;
	}



	// (1/18) メニューを開いた人だけが動かせる
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	このステートでの操作
	//+----------------------------------
	// パッド分更新
	// int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}

}

bool SceneMenuState::SystemWindowStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 選択ウィンドウの操作
	pMain->GetWindow(WINDOW_TYPE::SYSTEM)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::SystemWindowStep::Exit(sceneMenu *pMain)
{

}

void SceneMenuState::SystemWindowStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 選択ウィンドウの説明
	pMain->GetWindow(WINDOW_TYPE::SYSTEM)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "SystemWindowStep");
#endif // _DEBUG
}

bool SceneMenuState::SystemWindowStep::OnMessage(sceneMenu *pMain, const Message & msg)
{
	return false;
}


/*******************************************************/
//				ゲームウィンドウ選択
/*******************************************************/

void SceneMenuState::GameWindowStep::Enter(sceneMenu *pMain)
{
	// サウンドウィンドウ起動
	pMain->GetWindow(WINDOW_TYPE::GAME)->Action();
}

void SceneMenuState::GameWindowStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();

	// 戻るボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::GAME)->GetChoiceState()
		== GameWindow::GAME_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::GAME)->Stop();	// ウィンドウを閉じる
		pMain->GetFSM()->RevertToPreviousState();		// 前のステートへ戻る
		return;
	}



	// (1/18) メニューを開いた人だけが動かせる
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	このステートでの操作
	//+----------------------------------
	// パッド分更新
	// int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}
	
}

bool SceneMenuState::GameWindowStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 選択ウィンドウの操作
	pMain->GetWindow(WINDOW_TYPE::GAME)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::GameWindowStep::Exit(sceneMenu *pMain)
{

}

void SceneMenuState::GameWindowStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 選択ウィンドウの説明
	pMain->GetWindow(WINDOW_TYPE::GAME)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "GameWindowStep");
#endif // _DEBUG
}

bool SceneMenuState::GameWindowStep::OnMessage(sceneMenu *pMain, const Message & msg)
{
	return false;
}


/*******************************************************/
//				サウンドウィンドウ選択
/*******************************************************/

void SceneMenuState::SoundWindowStep::Enter(sceneMenu *pMain)
{
	// サウンドウィンドウ起動
	pMain->GetWindow(WINDOW_TYPE::SOUND)->Action();
}

void SceneMenuState::SoundWindowStep::Execute(sceneMenu *pMain)
{
	// UI
	M_UIMgr->Update();
	
	// 戻るボタンを押したら
	if (pMain->GetWindow(WINDOW_TYPE::SOUND)->GetChoiceState()
		== SoundWindow::SOUND_STATE::BACK)
	{
		pMain->GetWindow(WINDOW_TYPE::SOUND)->Stop();	// ウィンドウを閉じる
		pMain->GetFSM()->RevertToPreviousState();		// 前のステートへ戻る
		return;
	}



	// (1/18) メニューを開いた人だけが動かせる
	int l_iDeviceID = pMain->GetPauseDeviceID();

	//+----------------------------------
	//	このステートでの操作
	//+----------------------------------
	// パッド分更新
	// int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	// if (NumDevice == 0)NumDevice = 1;
	// for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, l_iDeviceID)) return;
	}
	
}

bool SceneMenuState::SoundWindowStep::PadUpdate(sceneMenu *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 選択ウィンドウの操作
	pMain->GetWindow(WINDOW_TYPE::SOUND)->Ctrl(DeviceID);

	return bChangedState;
}

void SceneMenuState::SoundWindowStep::Exit(sceneMenu *pMain)
{

}

void SceneMenuState::SoundWindowStep::Render(sceneMenu *pMain)
{
	// アイコンUI
	M_UIMgr->Render();

	// 選択ウィンドウの説明
	pMain->GetWindow(WINDOW_TYPE::SOUND)->RednerInfo();

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "SoundWindowStep");
#endif // _DEBUG
}

bool SceneMenuState::SoundWindowStep::OnMessage(sceneMenu *pMain, const Message & msg)
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
