#include "TDNLIB.h"
#include "sceneTitle.h"
#include "SceneTitleState.h"
#include "SceneMain.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "../Sound/BattleMusic.h"

//+--------------------
// 作業効率化
//+--------------------

// メニューのUI管理系
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()

/*******************************************************/
//					ゲーム開始ステート
/*******************************************************/

void SceneTitleState::Intro::Enter(sceneTitle *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN,8);

}
void SceneTitleState::Intro::Execute(sceneTitle *pMain)
{
	// 最初の選択へ
	if (Fade::isFadeInCompletion())
	{
		pMain->GetFSM()->ChangeState(LogoStep::GetInstance());
		return;
	}


}

void SceneTitleState::Intro::Exit(sceneTitle *pMain) 
{

}

void SceneTitleState::Intro::Render(sceneTitle *pMain)
{

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "Intro");

#endif // _DEBUG

}

bool SceneTitleState::Intro::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					ロゴステート
/*******************************************************/

void SceneTitleState::LogoStep::Enter(sceneTitle *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8);

	// ロゴのアニメーション開始
	pMain->m_pLogo->Action();

}
void SceneTitleState::LogoStep::Execute(sceneTitle *pMain)
{
	// ロゴ更新
	pMain->m_pLogo->Update();

	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// 何かボタン押したらメニューへ
			pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
			return;
		}
	}

}

void SceneTitleState::LogoStep::Exit(sceneTitle *pMain)
{

}

void SceneTitleState::LogoStep::Render(sceneTitle *pMain)
{
	//
	pMain->m_pLogo->Render(0, 0);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "LogoStep");

#endif // _DEBUG

}

bool SceneTitleState::LogoStep::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					ムービーステート
/*******************************************************/

void SceneTitleState::MovieStep::Enter(sceneTitle *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8);

}
void SceneTitleState::MovieStep::Execute(sceneTitle *pMain)
{
	// テアリング防止の苦肉の策
	pMain->m_pMovie->Update();
	

	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// 何かボタン押したらメニューへ
			pMain->GetFSM()->ChangeState(TitleStep::GetInstance());
			return;
		}
	}

}

void SceneTitleState::MovieStep::Exit(sceneTitle *pMain)
{

}

void SceneTitleState::MovieStep::Render(sceneTitle *pMain)
{
	// 動画画像
	pMain->m_pImages[sceneTitle::IMAGE::BACK]->Render(0, 0);

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "MovieStep");

#endif // _DEBUG

}

bool SceneTitleState::MovieStep::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					タイトルステート
/*******************************************************/

void SceneTitleState::TitleStep::Enter(sceneTitle *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8);


}
void SceneTitleState::TitleStep::Execute(sceneTitle *pMain)
{


	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// 何かボタン押したらメニューへ
			pMain->GetFSM()->ChangeState(End::GetInstance());
			return;
		}
	}


	//// 最初の選択へ
	//if (Fade::isFadeOutCompletion())
	//{
	//	pMain->GetFSM()->ChangeState(End::GetInstance());
	//	return;
	//}
}

void SceneTitleState::TitleStep::Exit(sceneTitle *pMain)
{

}

void SceneTitleState::TitleStep::Render(sceneTitle *pMain)
{
	

#ifdef _DEBUG
	
	// □
	tdnPolygon::Rect(200, 200, 400);

	tdnText::Draw(0, 0, 0xffffffff, "TitleStep");

#endif // _DEBUG

}

bool SceneTitleState::TitleStep::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					おわり（メニューへ）
/*******************************************************/

void SceneTitleState::End::Enter(sceneTitle *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);
}

void SceneTitleState::End::Execute(sceneTitle *pMain)
{
	// メインへ
	if (Fade::isFadeOutCompletion())
	{
		MainFrameEx->ChangeScene(new sceneMenu(),true);
		return;
	}

}

bool SceneTitleState::End::PadUpdate(sceneTitle *pMain, int DeviceID)
{
	bool bChangedState(false);

	return bChangedState;
}

void SceneTitleState::End::Exit(sceneTitle *pMain)
{
}

void SceneTitleState::End::Render(sceneTitle *pMain)
{

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "End");

#endif // _DEBUG

}

bool SceneTitleState::End::OnMessage(sceneTitle *pMain, const Message & msg)
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
//					戻るステート
/*******************************************************/

void SceneTitleState::BackMenu::Enter(sceneTitle *pMain)
{

}

void SceneTitleState::BackMenu::Execute(sceneTitle *pMain)
{

	// (仮 後で消す)最初の選択へ戻る
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iSenderDeviceID) == 3)
	//{
	//	if (pMain->m_pBackMenuTips->GetStep() == TipsCard::STEP::EXECUTE)
	//	{
	//		// 次のステートへ
	//		pMain->GetFSM()->ChangeState(SceneTitleState::CharaSelect::GetInstance());
	//		return;
	//	}
	//}

	if (Fade::isFadeOutCompletion())
	{
		// メニューへ
		MainFrameEx->ChangeScene(new sceneMenu());
		return;
	}

	// (TODO)フェードの処理なんとかする　
	if (Fade::GetMode() != Fade::FADE_OUT)
	{

		//if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::OK)
		//{
		//	// フェード
		//	Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);
		//	return;
		//}
		//
		//if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::CANCEL)
		//{
		//	// キャラセレに戻る
		//	pMain->GetFSM()->ChangeState(SceneTitleState::CharaSelect::GetInstance());
		//	return;
		//}

	}




}

void SceneTitleState::BackMenu::Exit(sceneTitle *pMain) 
{

}

void SceneTitleState::BackMenu::Render(sceneTitle *pMain)
{

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BackMenu");

#endif // _DEBUG

}

bool SceneTitleState::BackMenu::OnMessage(sceneTitle *pMain, const Message & msg)
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

