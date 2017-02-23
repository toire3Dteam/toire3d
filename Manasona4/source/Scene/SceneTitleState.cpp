#include "TDNLIB.h"
#include "sceneTitle.h"
#include "SceneTitleState.h"
#include "SceneMain.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data/SelectData.h"

//+--------------------
// 作業効率化
//+--------------------

// メニューのUI管理系
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()

// これを定義するとOPがスキップされる(デバッグ時短用)
//#define OP_SKIP

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

#ifdef OP_SKIP

		pMain->GetFSM()->ChangeState(TitleStep::GetInstance());
		return;
#endif

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

	// 放置用
	pMain->m_iGameTimer = 0;

}
void SceneTitleState::LogoStep::Execute(sceneTitle *pMain)
{
	// ロゴ更新
	pMain->m_pLogo->Update();

	// 
	if (Fade::isFadeOutCompletion() == false)
	{

		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{

			pMain->m_iGameTimer++;
			if (pMain->m_iGameTimer == 180)
			{
				// フェードアウト
				Fade::Set(Fade::FLAG::FADE_OUT, 8);
			}

			// パッド分更新
			int NumDevice(tdnInputManager::GetNumDevice());
			// パッド何もささってないとき用
			if (NumDevice == 0)NumDevice = 1;
			for (int i = 0; i < NumDevice; i++)
			{
				if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
				{
					// フェードアウト
					Fade::Set(Fade::FLAG::FADE_OUT, 8);
					return;
				}
			}

		}

	}
	else
	{
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		return;
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

	// ムービー再生
	pMain->m_pMovie->Play();
}
void SceneTitleState::MovieStep::Execute(sceneTitle *pMain)
{
	// テアリング防止の苦肉の策
	pMain->m_pMovie->Update();
	
	// ムービーが最後まで来たら
	if (pMain->m_pMovie->isEndPos())
	{
		// ムービー止める
		pMain->m_pMovie->Stop();
		// メニューへ
		pMain->GetFSM()->ChangeState(TitleStep::GetInstance());
		return;
	}
	

	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// ムービー止める
			pMain->m_pMovie->Stop();

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
	pMain->m_pImages[sceneTitle::IMAGE::MOVIE]->Render(0, 0);

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


	// エニーキーのアニメ開始
	pMain->m_pPreaseAnyButton->Action();

	// BGムービー再生
	pMain->m_pMovieBGLine->Play();

	// 放置用
	pMain->m_iGameTimer = 0;

	pMain->m_fCloudU = 0;


}
void SceneTitleState::TitleStep::Execute(sceneTitle *pMain)
{
	// テアリング防止
	pMain->m_pMovieBGLine->Update();

	// 動き更新
	if (pMain->m_pPreaseAnyButton->GetAction()->IsEnd() == true)
	{	
		// アニメのループ
		pMain->m_pPreaseAnyButton->Action();
	}
	
	pMain->m_pPreaseAnyButton->Update();

	// 雲の動き
	pMain->m_fCloudU -= 0.25f;


	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (KEY(KEY_A, i) == 3 || KEY(KEY_B, i) == 3 || KEY(KEY_C, i) == 3 || KEY(KEY_D, i) == 3)
		{
			// SEの再生
			se->Play("タイトルスタート");

			// 何かボタン押したらメニューへ
			pMain->GetFSM()->ChangeState(End::GetInstance());
			return;
		}
	}

	pMain->m_iGameTimer++;
	if (pMain->m_iGameTimer >= 60 * 120)
	{
		pMain->GetFSM()->ChangeState(Intro::GetInstance());
		return;
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
	// 背景
	pMain->m_pImages[sceneTitle::IMAGE::BG]->Render(0, 0);

	// ライン
	pMain->m_pBGLine->Render(0, 0, RS::ADD);

	// 雲1
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD1]->Render(-100 + (int)pMain->m_fCloudU , 200);

	// 雲2
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD2]->Render(750+(int)pMain->m_fCloudU, 150);

	// タイトル
	pMain->m_pImages[sceneTitle::IMAGE::TITLE]->Render(0, 0);

	// 何かボタン押して
	pMain->m_pPreaseAnyButton->Render(448, 576);


#ifdef _DEBUG
	
	// □
	//tdnPolygon::Rect(200, 200, 400);

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
		MainFrameEX->ChangeScene(new sceneMenu(),true);
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
	// 背景
	pMain->m_pImages[sceneTitle::IMAGE::BG]->Render(0, 0);

	// 雲1
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD1]->Render(-100 + (int)pMain->m_fCloudU, 200);

	// 雲2
	pMain->m_pImages[sceneTitle::IMAGE::CLOUD2]->Render(750 + (int)pMain->m_fCloudU, 150);

	// タイトル
	pMain->m_pImages[sceneTitle::IMAGE::TITLE]->Render(0, 0);

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
		MainFrameEX->ChangeScene(new sceneMenu());
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

