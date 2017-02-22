#include "TDNLIB.h"
#include "SceneCollect.h"
#include "SceneCollectState.h"
#include "SceneCollect.h"
#include "SceneMenu.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"
#include "Window\OptionWindow.h"	// 必要なウィンドウ
#include "../Window/GameWindow.h"
#include "Collect\PictureManager.h"	// イラスト用
#include "BaseEntity\Message\MessageDispatcher.h"

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
	// タグがトロフィーを選択してたら
	if (pMain->GetTagType() == TAG_TYPE::TROPHY)
	{
		// トロフィー初期演出
		pMain->TrophyFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		return;
	}
	else if (pMain->GetTagType() == TAG_TYPE::PICTURE)
	{
		// コレクト初期演出
		pMain->PictureFirstAction();
		pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
		return;
	}
	else if (pMain->GetTagType() == TAG_TYPE::MOVIE)
	{
		// コレクト初期演出
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		return;
	}
	else if (pMain->GetTagType() == TAG_TYPE::RECORD)
	{
		// コレクト初期演出
		pMain->RecordFirstAction();
		pMain->GetFSM()->ChangeState(RecordStep::GetInstance());
		return;
	}
	else
	{
		MyAssert(0, "そんなタグはない");
	}


}
void SceneCollectState::Intro::Exit(sceneCollect *pMain) {}
void SceneCollectState::Intro::Render(sceneCollect *pMain)
{

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
#endif // _DEBUG

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
	// 現在のタグ番号保存
	pMain->SetTagType(TAG_TYPE::TROPHY);
}

void SceneCollectState::TrophyStep::Execute(sceneCollect *pMain)
{

	// トロフィー更新
	pMain->TrophyUpdate();


	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}



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

	// もどる
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存
		// メニューに戻る
		pMain->GetFSM()->ChangeState(BackMenuStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// コレクト初期演出
		pMain->PictureFirstAction();
		pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// 初期演出
		pMain->RecordFirstAction();
		pMain->GetFSM()->ChangeState(RecordStep::GetInstance());
		bChangedState = true;
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

	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "TrophyStep");

#endif // _DEBUG

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


/*******************************************************/
//					イラストの選択ステート
/*******************************************************/

void SceneCollectState::PictureStep::Enter(sceneCollect *pMain)
{
	// 現在のタグ番号保存
	pMain->SetTagType(TAG_TYPE::PICTURE);

	// 画像鑑賞前の黒いフェード
	pMain->m_bBlackRect = false;
	//pMain->m_iBlackRectAlpha = 0;
}

void SceneCollectState::PictureStep::Execute(sceneCollect *pMain)
{

	// イラスト更新
	pMain->PictureUpdate();

	// 黒いフェード始動フラグがONになってたら
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// 鑑賞ステートへ
			pMain->GetFSM()->ChangeState(PictureWatch::GetInstance());
			return;
		}

		// フェード速度
		pMain->m_iBlackRectAlpha += (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha >= 255) { pMain->m_iBlackRectAlpha = 255; }

	}
	else
	{
		//+-------------------------------------
		// フェード速度
		pMain->m_iBlackRectAlpha -= (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha <= 0) { pMain->m_iBlackRectAlpha = 0; }
		//+-------------------------------------

		// パッド分更新
		int NumDevice(tdnInputManager::GetNumDevice());
		// パッド何もささってないとき用
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}

	}





}

bool SceneCollectState::PictureStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// イラストの操作
	pMain->PictureCtrl(DeviceID);


	// 決定でイラスト鑑賞ステートへ！
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存

		// しかしまだ解放されていなかったら
		if (PlayerDataMgr->m_SecretData.iAllPic[pMain->m_tagPI.iSelectNo] == 0)
		{
			//お金で買えるなら + お金があれば【イラスト購入ステート】へ
			if (pMain->GetPictureMgr()->GetPicture(pMain->m_tagPI.iSelectNo)->isRock() == false &&
				(int)PlayerDataMgr->m_PlayerInfo.coin >=  pMain->GetPictureMgr()->GetPicture(pMain->m_tagPI.iSelectNo)->GetPrice())
			{

				// SEの再生
				se->Play("決定1");

				// イラスト購入ステートへ
				pMain->GetFSM()->ChangeState(BuyPictureStep::GetInstance());
				bChangedState = true;
			}

		}
		else 
		{
			// SEの再生
			se->Play("決定1");
			// 鑑賞ステートへ
			pMain->m_bBlackRect = true;
			bChangedState = true;
		}

	}

	// もどる
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存
										// メニューに戻る
		pMain->GetFSM()->ChangeState(BackMenuStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// トロフィー初期演出
		pMain->TrophyFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// コレクト初期演出
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// コレクト初期演出
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		bChangedState = true;
	}
	return bChangedState;
}

void SceneCollectState::PictureStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::PictureStep::Render(sceneCollect *pMain)
{
	// イラストの絵
	pMain->PictureRender();


	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


	// フェード
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "PictureStep");

#endif // _DEBUG

}

bool SceneCollectState::PictureStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//					イラスト鑑賞ステート
/*******************************************************/

void SceneCollectState::PictureWatch::Enter(sceneCollect *pMain)
{
	// 鑑賞用毎回初期化
	pMain->PictureInitExe();

}

void SceneCollectState::PictureWatch::Execute(sceneCollect *pMain)
{

	// 黒いフェード始動フラグがONになってたら
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// 前のステートに戻る
			pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
			return;
		}

		//+-------------------------------------
		// フェード速度
		pMain->m_iBlackRectAlpha += (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha >= 255) { pMain->m_iBlackRectAlpha = 255; }
		//+-------------------------------------
	}
	else 
	{
		//+-------------------------------------
		// フェード速度
		pMain->m_iBlackRectAlpha -= (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha <= 0) { pMain->m_iBlackRectAlpha = 0; }
		//+-------------------------------------

		// パッド分更新
		int NumDevice(tdnInputManager::GetNumDevice());
		// パッド何もささってないとき用
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}

		// 起動イラスト更新
		pMain->GetPictureMgr()->UpdateExe();
	}


}

bool SceneCollectState::PictureWatch::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);




	// ★鑑賞イラストの操作
	pMain->GetPictureMgr()->CtrlExe(DeviceID);



	// メッセージ非表示
	if (tdnInput::KeyGet(KEYCODE::KEY_C, DeviceID) == 3||
		tdnInput::KeyGet(KEYCODE::KEY_D, DeviceID) == 3)
	{
		if (pMain->m_bWatchInfo == true)
		{
			pMain->m_bWatchInfo = false;
		}
		else
		{
			pMain->m_bWatchInfo = true;
		}
	}


	// もどる
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存
											
		// 前のステートへ
		pMain->m_bBlackRect = true;
		bChangedState = true;
	}


	return bChangedState;
}

void SceneCollectState::PictureWatch::Exit(sceneCollect *pMain)
{
	// 画像の解放
	pMain->GetPictureMgr()->RereaseExe();

}

void SceneCollectState::PictureWatch::Render(sceneCollect *pMain)
{
	// 絵
	pMain->PictureRenderExe();
	//pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);

	// フェード
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

	//とりあえず鑑賞までのシーン遷移できたあとは干渉画像の拡大とか移動とか画像読み込みとか
	//	と
	//	購入関連以上

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "PictureWatch");

#endif // _DEBUG

}

bool SceneCollectState::PictureWatch::OnMessage(sceneCollect *pMain, const Message & msg)
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
//				コンテンツ購入確認
/*******************************************************/

void SceneCollectState::BuyPictureStep::Enter(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->Action();

}

void SceneCollectState::BuyPictureStep::Execute(sceneCollect *pMain)
{
	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());

	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}
		
}

bool SceneCollectState::BuyPictureStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::OK)
	{	
		// そのコンテンツを解放
		PICTURE_TYPE eType;
		eType = pMain->GetPictureMgr()->GetSelectType();
		MsgMgr->Dispatch(0, ENTITY_ID::PICTURE_MGR, ENTITY_ID::PICTURE_MGR, MESSAGE_TYPE::PICTURE_GET, &eType);

		// 値段分持っているコインを減らす
		int l_iPrice = pMain->GetPictureMgr()->GetPicture(pMain->m_tagPI.iSelectNo)->GetPrice();
		PlayerDataMgr->m_PlayerInfo.coin -= l_iPrice;


		// 戻る
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

		// SEの再生
		se->Play("購入");
	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::CANCEL)
	{
		// 戻る
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}

	return bChangedState;
}

void SceneCollectState::BuyPictureStep::Exit(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->End();
}

void SceneCollectState::BuyPictureStep::Render(sceneCollect *pMain)
{
	// イラストステップから来た場合
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::PictureStep::GetInstance()))
	{
		// イラストの絵
		pMain->PictureRender();

	}

	// 黒い板
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BuyPictureStep");

#endif // _DEBUG

}

bool SceneCollectState::BuyPictureStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//					ムービーの選択ステート
/*******************************************************/

void SceneCollectState::MovieStep::Enter(sceneCollect *pMain)
{
	// 現在のタグ番号保存
	pMain->SetTagType(TAG_TYPE::MOVIE);

	// 画像鑑賞前の黒いフェード
	pMain->m_bBlackRect = false;
	//pMain->m_iBlackRectAlpha = 0;
}

void SceneCollectState::MovieStep::Execute(sceneCollect *pMain)
{

	// ムービー更新
	pMain->MovieUpdate();

	// 黒いフェード始動フラグがONになってたら
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// 鑑賞ステートへ
			pMain->GetFSM()->ChangeState(MovieWatch::GetInstance());
			return;
		}

		// フェード速度
		pMain->m_iBlackRectAlpha += (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha >= 255) { pMain->m_iBlackRectAlpha = 255; }

	}
	else
	{
		//+-------------------------------------
		// フェード速度
		pMain->m_iBlackRectAlpha -= (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha <= 0) { pMain->m_iBlackRectAlpha = 0; }
		//+-------------------------------------

		// パッド分更新
		int NumDevice(tdnInputManager::GetNumDevice());
		// パッド何もささってないとき用
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}

	}





}

bool SceneCollectState::MovieStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// ムービーの操作
	pMain->MovieCtrl(DeviceID);


	// 決定でムービー鑑賞ステートへ！
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存

		// しかしまだ解放されていなかったら
		if (PlayerDataMgr->m_SecretData.iAllMovie[pMain->m_tagMI.iSelectNo] == 0)
		{
			//お金で買えるなら + お金があれば【ムービー購入ステート】へ
			if (pMain->GetMovieMgr()->GetMovie(pMain->m_tagMI.iSelectNo)->isRock() == false &&
				(int)PlayerDataMgr->m_PlayerInfo.coin >= pMain->GetMovieMgr()->GetMovie(pMain->m_tagMI.iSelectNo)->GetPrice())
			{

				// SEの再生
				se->Play("決定1");

				// ムービー購入ステートへ
				pMain->GetFSM()->ChangeState(BuyMovieStep::GetInstance());
				bChangedState = true;
			}

		}
		else
		{
			// SEの再生
			se->Play("決定1");
			// 鑑賞ステートへ
			pMain->m_bBlackRect = true;
			bChangedState = true;
		}

	}

	// もどる
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存
										// メニューに戻る
		pMain->GetFSM()->ChangeState(BackMenuStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// 初期演出
		pMain->RecordFirstAction();
		pMain->GetFSM()->ChangeState(RecordStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// コレクト初期演出
		pMain->PictureFirstAction();
		pMain->GetFSM()->ChangeState(PictureStep::GetInstance());
		bChangedState = true;
	}


	return bChangedState;
}

void SceneCollectState::MovieStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::MovieStep::Render(sceneCollect *pMain)
{
	// ムービーの絵
	pMain->MovieRender();


	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


	// フェード
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "MovieStep");

#endif // _DEBUG

}

bool SceneCollectState::MovieStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//					ムービー鑑賞ステート
/*******************************************************/

void SceneCollectState::MovieWatch::Enter(sceneCollect *pMain)
{
	// BGMフェードアウト
	bgm->StopStreamIn(true);

	// 鑑賞用毎回初期化
	pMain->MovieInitExe();

}

void SceneCollectState::MovieWatch::Execute(sceneCollect *pMain)
{

	// 黒いフェード始動フラグがONになってたら
	if (pMain->m_bBlackRect == true)
	{
		if (pMain->m_iBlackRectAlpha >= 255)
		{
			// 前のステートに戻る
			pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
			return;
		}

		//+-------------------------------------
		// フェード速度
		pMain->m_iBlackRectAlpha += (int)(255 / 24);
		// クランプ
		if (pMain->m_iBlackRectAlpha >= 255) { pMain->m_iBlackRectAlpha = 255; }
		//+-------------------------------------
	}
	else
	{
		//+-------------------------------------
		// フェード速度
		pMain->m_iBlackRectAlpha -= (int)(255 / 6);
		// クランプ
		if (pMain->m_iBlackRectAlpha <= 0) { pMain->m_iBlackRectAlpha = 0; }
		//+-------------------------------------

		// パッド分更新
		int NumDevice(tdnInputManager::GetNumDevice());
		// パッド何もささってないとき用
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			if (PadUpdate(pMain, i)) return;
		}

		// 起動ムービー更新
		if (pMain->GetMovieMgr()->UpdateExe() == true)
		{
			// 最後まで来たら
			pMain->m_bBlackRect = true;
			return;
		}
		
	}


}

bool SceneCollectState::MovieWatch::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);




	// ★鑑賞ムービーの操作
	pMain->GetMovieMgr()->CtrlExe(DeviceID);



	// メッセージ非表示
	if (tdnInput::KeyGet(KEYCODE::KEY_C, DeviceID) == 3 ||
		tdnInput::KeyGet(KEYCODE::KEY_D, DeviceID) == 3)
	{
		if (pMain->m_bWatchInfo == true)
		{
			pMain->m_bWatchInfo = false;
		}
		else
		{
			pMain->m_bWatchInfo = true;
		}
	}


	// もどる
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存

										// 前のステートへ
		pMain->m_bBlackRect = true;
		bChangedState = true;
	}


	return bChangedState;
}

void SceneCollectState::MovieWatch::Exit(sceneCollect *pMain)
{
	// 画像の解放
	pMain->GetMovieMgr()->RereaseExe();

	// BGMフェードイン
	bgm->PlayStreamIn("DATA/Sound/BGM/System/Collect.ogg");
}

void SceneCollectState::MovieWatch::Render(sceneCollect *pMain)
{
	// 絵
	pMain->MovieRenderExe();
	//pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);

	// フェード
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));

	//とりあえず鑑賞までのシーン遷移できたあとは干渉画像の拡大とか移動とか画像読み込みとか
	//	と
	//	購入関連以上

#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "MovieWatch");

#endif // _DEBUG

}

bool SceneCollectState::MovieWatch::OnMessage(sceneCollect *pMain, const Message & msg)
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
//				コンテンツ購入確認
/*******************************************************/

void SceneCollectState::BuyMovieStep::Enter(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->Action();

}

void SceneCollectState::BuyMovieStep::Execute(sceneCollect *pMain)
{
	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());

	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}

}

bool SceneCollectState::BuyMovieStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::OK)
	{
		// そのコンテンツを解放
		MOVIE_TYPE eType;
		eType = pMain->GetMovieMgr()->GetSelectType();
		MsgMgr->Dispatch(0, ENTITY_ID::MOVIE_MGR, ENTITY_ID::MOVIE_MGR, MESSAGE_TYPE::MOVIE_GET, &eType);

		// 値段分持っているコインを減らす
		int l_iPrice = pMain->GetMovieMgr()->GetMovie(pMain->m_tagMI.iSelectNo)->GetPrice();
		PlayerDataMgr->m_PlayerInfo.coin -= l_iPrice;


		// 戻る
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

		// SEの再生
		se->Play("購入");
	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->GetSelectState() == TipsCard::CANCEL)
	{
		// 戻る
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}

	return bChangedState;
}

void SceneCollectState::BuyMovieStep::Exit(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::CONTENT_BUY)->End();
}

void SceneCollectState::BuyMovieStep::Render(sceneCollect *pMain)
{
	// ムービーステップから来た場合
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::MovieStep::GetInstance()))
	{
		// ムービーの絵
		pMain->MovieRender();

	}

	// 黒い板
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BuyMovieStep");

#endif // _DEBUG

}

bool SceneCollectState::BuyMovieStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//					レコードの選択ステート
/*******************************************************/

void SceneCollectState::RecordStep::Enter(sceneCollect *pMain)
{
	// 現在のタグ番号保存
	pMain->SetTagType(TAG_TYPE::RECORD);
}

void SceneCollectState::RecordStep::Execute(sceneCollect *pMain)
{
	// レコードの更新
	pMain->RecordUpdate();

	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());
	// パッド何もささってないとき用
	if (NumDevice == 0)NumDevice = 1;
	for (int i = 0; i < NumDevice; i++)
	{
		if (PadUpdate(pMain, i)) return;
	}

}

bool SceneCollectState::RecordStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);


	// もどる
	if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	{
		pMain->SetCtrlDevice(DeviceID); // このボタンを押したデバイスを保存
										// メニューに戻る
		pMain->GetFSM()->ChangeState(BackMenuStep::GetInstance());
		bChangedState = true;
	}

	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// 初期演出
		pMain->TrophyFirstAction();
		pMain->GetFSM()->ChangeState(TrophyStep::GetInstance());
		bChangedState = true;
	}
	// ★ステップ変更
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, DeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		// 初期演出
		pMain->MovieFirstAction();
		pMain->GetFSM()->ChangeState(MovieStep::GetInstance());
		bChangedState = true;
	}




	return bChangedState;
}

void SceneCollectState::RecordStep::Exit(sceneCollect *pMain)
{

}

void SceneCollectState::RecordStep::Render(sceneCollect *pMain)
{

	// レコードの絵
	pMain->RecordRender();


	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


	// フェード
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, ARGB(pMain->m_iBlackRectAlpha, 0, 0, 0));


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "RecordStep");

#endif // _DEBUG

}

bool SceneCollectState::RecordStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
//				メニューに戻る
/*******************************************************/

void SceneCollectState::BackMenuStep::Enter(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->Action();

}

void SceneCollectState::BackMenuStep::Execute(sceneCollect *pMain)
{
	// パッド分更新
	int NumDevice(tdnInputManager::GetNumDevice());

	// フェード
	if (Fade::isFadeOutCompletion() == false)
	{
		if (Fade::GetMode() != Fade::FLAG::FADE_OUT)
		{
			// パッド何もささってないとき用
			if (NumDevice == 0)NumDevice = 1;
			for (int i = 0; i < NumDevice; i++)
			{
				if (PadUpdate(pMain, i)) return;
			}
		}

	}
	else
	{
		// ★ここでメニューに移動します
		MainFrameEX->ChangeScene(new sceneMenu());
		return;

	}

}

bool SceneCollectState::BackMenuStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::SELECT_STATE::OK)
	{
		// フェードしてチュートリアルへ
		// フェードアウト
		Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::SELECT_STATE::CANCEL)
	{
		// 戻る
		pMain->GetFSM()->RevertToPreviousState();
		bChangedState = true;

	}

	return bChangedState;
}

void SceneCollectState::BackMenuStep::Exit(sceneCollect *pMain)
{
	pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->End();
}

void SceneCollectState::BackMenuStep::Render(sceneCollect *pMain)
{
	// トロフィーステップから来た場合
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::TrophyStep::GetInstance()))
	{	
		// トロフィーの絵
		pMain->TrophyRender();

	}
	// イラストステップから来た場合
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::PictureStep::GetInstance()))
	{
		// イラストの絵
		pMain->PictureRender();

	}
	// ムービーステップから来た場合
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::MovieStep::GetInstance()))
	{
		// ムービーの絵
		pMain->MovieRender();

	}
	// レコードステップから来た場合
	if (pMain->GetFSM()->isPrevState(*SceneCollectState::RecordStep::GetInstance()))
	{
		// レコードの絵
		pMain->RecordRender();

	}


	// 黒い板
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BackMenuStep");
	tdnText::Draw(0, 100, 0xffffffff, "状態_%d", (int)pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState());

#endif // _DEBUG

}

bool SceneCollectState::BackMenuStep::OnMessage(sceneCollect *pMain, const Message & msg)
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
