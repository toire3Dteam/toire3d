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
	if (tdnInput::KeyGet(KEYCODE::KEY_C, DeviceID) == 3)
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
		MainFrameEx->ChangeScene(new sceneMenu());
		return;

	}

}

bool SceneCollectState::BackMenuStep::PadUpdate(sceneCollect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// 
	if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::OK)
	{
		// フェードしてチュートリアルへ
		// フェードアウト
		Fade::Set(Fade::FLAG::FADE_OUT, 16, 0x00000000);

		bChangedState = true;

	}
	else if (pMain->GetTips(TIPS_TYPE_COLLECT::BACK_MENU)->GetSelectState() == TipsCard::CANCEL)
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

	// 黒い板
	tdnPolygon::Rect(0, 0, 1280, 720, RS::COPY, 0xaa000000);

	// 左上タイトル
	pMain->m_pImages[sceneCollect::IMAGE::TITLE]->Render(0, 0);


#ifdef _DEBUG

	tdnText::Draw(0, 0, 0xffffffff, "BackMenuStep");

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
