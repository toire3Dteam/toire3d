#include "TDNLIB.h"
#include "SceneSelect.h"
#include "SceneSelectState.h"
#include "SceneCollect.h"
#include "SceneVS.h"
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

void SceneSelectState::Intro::Enter(sceneSelect *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 8, 0xFFFFFFFF);

	// 初期の演出
	pMain->FirstAction();

}
void SceneSelectState::Intro::Execute(sceneSelect *pMain)
{
	// 最初の選択へ
	if (Fade::isFadeInCompletion())
	{
		pMain->GetFSM()->ChangeState(CharaSelect::GetInstance());
		return;
	}


}

void SceneSelectState::Intro::Exit(sceneSelect *pMain) 
{

}

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
//					Charaの選択ステート
/*******************************************************/

void SceneSelectState::CharaSelect::Enter(sceneSelect *pMain)
{
	// 説明文
	pMain->m_pPic[sceneSelect::PIC_TYPE::SELECT_INFO]->Action();
}

void SceneSelectState::CharaSelect::Execute(sceneSelect *pMain)
{

	// ここで全員OKになったらステージ&BGMへ
	if (pMain->GetSelectUIMgr()->IsOK())
	{
		pMain->GetFSM()->ChangeState(SceneSelectState::StageAndBGM::GetInstance());
		return;
	}

	/**********************/
	//	キャラセレの操作
	/**********************/

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

}

bool SceneSelectState::CharaSelect::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);

	//// 戻るボタンを押したコントローラーが初期状態じゃなかったら返す
	//if (pMain->GetSelectUIMgr()->isFirstStep(DeviceID) == false)return bChangedState;
	//
	//// ×でメニューに戻る
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, DeviceID) == 3)
	//{
	//	
	//	// 次のステートへ
	//	pMain->GetFSM()->ChangeState(SceneSelectState::BackMenu::GetInstance());
	//	bChangedState = true;
	//}


	return bChangedState;
}

void SceneSelectState::CharaSelect::Exit(sceneSelect *pMain)
{
	pMain->m_pPic[sceneSelect::PIC_TYPE::SELECT_INFO]->Stop();
}

void SceneSelectState::CharaSelect::Render(sceneSelect *pMain)
{
	// アイコンUI
	//M_UIMgr->Render();

	tdnText::Draw(0, 0, 0xffffffff, "CharaSelect");
}

bool SceneSelectState::CharaSelect::OnMessage(sceneSelect *pMain, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::BACK_MENU:
	{
		int *senderID = (int*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
		pMain->m_iSenderDeviceID = *senderID;
		// バックメニューステートへ
		pMain->GetFSM()->ChangeState(SceneSelectState::BackMenu::GetInstance());

		return true;
	}	break;
	default:
		break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					Stage&BGMの選択ステート
/*******************************************************/

void SceneSelectState::StageAndBGM::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
	
	// 説明文
	pMain->m_pPic[sceneSelect::PIC_TYPE::SECOND_SELECT_INFO]->Action();
}

void SceneSelectState::StageAndBGM::Execute(sceneSelect *pMain)
{

	// 暗転用
	pMain->m_iRectAlpha += 18;
	if (pMain->m_iRectAlpha >= 156 )
	{
		pMain->m_iRectAlpha = 156;
	}

	// 誰かが戻ったらもう一度キャラセレへ
	if (pMain->GetSelectUIMgr()->IsOK() == false)
	{
		pMain->GetFSM()->ChangeState(SceneSelectState::CharaSelect::GetInstance());
		return;
	}

	/**********************/
	//	StageとBGM操作
	/**********************/

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
}

bool SceneSelectState::StageAndBGM::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);

	// StageとBGMのどちらか切り替え
	if (tdnInput::KeyGet(KEYCODE::KEY_DOWN, DeviceID) == 3)
	{	
		pMain->m_tagSecondSelect.bBGMSelect = true;
	}
	else if (tdnInput::KeyGet(KEYCODE::KEY_UP, DeviceID) == 3)
	{
		pMain->m_tagSecondSelect.bBGMSelect = false;
	}


	if (pMain->m_tagSecondSelect.bBGMSelect == true)
	{
		// ★BGMの選択
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
		{
			pMain->m_iSelectBGMNo--;
			pMain->m_tagSecondSelect.pBGMLeftRip->Action();
			// -1になれば最後の曲にする
			if (pMain->m_iSelectBGMNo < 0)
			{
				pMain->m_iSelectBGMNo = BattleMusicMgr->GetNumMusic() - 1;
			}
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
		{
			pMain->m_iSelectBGMNo++;
			pMain->m_tagSecondSelect.pBGMRightRip->Action();
			// 最大曲数まできたら最初に戻す
			if (pMain->m_iSelectBGMNo >=  BattleMusicMgr->GetNumMusic())
			{
				pMain->m_iSelectBGMNo = 0;
			}
		}
	}
	else
	{
		// ★ステージの選択
		if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, DeviceID) == 3)
		{
			pMain->m_iSelectStageNo--;
			pMain->m_tagSecondSelect.pStageLeftRip->Action();
			// -1になれば最後のステージにする
			if (pMain->m_iSelectStageNo < 0)
			{
				pMain->m_iSelectStageNo = (int)STAGE::MAX;	// -1にしないのはRANDOMを含めているから
			}
		}
		else if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, DeviceID) == 3)
		{
			pMain->m_iSelectStageNo++;
			pMain->m_tagSecondSelect.pStageRightRip->Action();

			// (1/2) ↓ここの変数間違ってたから直した。
			// 最後のステージまできたら最初に戻す
			if (pMain->m_iSelectStageNo > (int)STAGE::MAX)
			{
				pMain->m_iSelectStageNo = 0;
			}
		}
	}

	// ○で完了
	if (tdnInput::KeyGet(KEYCODE::KEY_B, DeviceID) == 3)
	{
		// 次のステートへ
		pMain->GetFSM()->ChangeState(SceneSelectState::End::GetInstance());
		bChangedState = true;
	}

	return bChangedState;
}

void SceneSelectState::StageAndBGM::Exit(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;

	pMain->m_pPic[sceneSelect::PIC_TYPE::SECOND_SELECT_INFO]->Stop();
}

void SceneSelectState::StageAndBGM::Render(sceneSelect *pMain)
{
	pMain->StageAndBGMRender();
	tdnText::Draw(0, 0, 0xffffffff, "StageAndBGM");
	
	//tdnText::Draw(68, 128, 0xffffffff, "m_iSelectStageNo->%d",pMain->m_iSelectStageNo);
	//tdnText::Draw(68, 156, 0xffffffff, "m_iSelectBGMNo->%d",pMain->m_iSelectBGMNo);
	//tdnText::Draw(68, 256, 0xffffffff, "曲めい->%s", BattleMusicMgr->GetMusicName(pMain->m_iSelectBGMNo).c_str());
}

bool SceneSelectState::StageAndBGM::OnMessage(sceneSelect *pMain, const Message & msg)
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
//					おわり（メインへ）
/*******************************************************/

void SceneSelectState::End::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);

}

void SceneSelectState::End::Execute(sceneSelect *pMain)
{
	// メインへ
	if (Fade::isFadeOutCompletion())
	{
		// (TODO) ↑で選択したキャラクター・パートナー・ステージ・BGMを設定する
		
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter
			= pMain->GetSelectUIMgr()->GetSelectCharacter(SIDE::LEFT);

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter
			= pMain->GetSelectUIMgr()->GetSelectCharacter(SIDE::RIGHT);				

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].ePartner
			= pMain->GetSelectUIMgr()->GetSelectPartner(SIDE::LEFT);

		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].ePartner
			= pMain->GetSelectUIMgr()->GetSelectPartner(SIDE::RIGHT);

		SelectDataMgr->Get()->iBattleMusicID=pMain->m_iSelectBGMNo;

		// ステージの登録(★RANDOMを0にしているので、実際のenumに合わす必要がある)
		if (pMain->m_iSelectStageNo == 0)
		{
			int r;
			// なぜかランダムで0～MAX-1でない値が入っていたので、その値が出るまで無限ループにする(なんでこうなったんだろう？)
			//do
			//{
				r = tdnRandom::Get(0, (int)STAGE::MAX - 1);
				// ランダム処理
				SelectDataMgr->Get()->eStage = (STAGE)r;

				switch ((STAGE)r)
				{
				case STAGE::SAND:
					break;
				case STAGE::SYUTEN:
					break;
				case STAGE::SEA:
					break;
				case STAGE::GARDEN:
					break;
				case STAGE::NANASATO_SITY:
					break;
				default:
					MyAssert(0, "ステージに不正な値%d", r);
					break;
				}
			//} while (r < 0 || r >= (int)STAGE::MAX);
		}
		else
		{
			SelectDataMgr->Get()->eStage = (STAGE)(pMain->m_iSelectStageNo - 1);	// RANDOMぶんを引いている
		}

		MainFrameEX->ChangeScene(new sceneVS);
		return;
	}

}

bool SceneSelectState::End::PadUpdate(sceneSelect *pMain, int DeviceID)
{
	bool bChangedState(false);

	return bChangedState;
}

void SceneSelectState::End::Exit(sceneSelect *pMain)
{
}

void SceneSelectState::End::Render(sceneSelect *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "End");
}

bool SceneSelectState::End::OnMessage(sceneSelect *pMain, const Message & msg)
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

void SceneSelectState::BackMenu::Enter(sceneSelect *pMain)
{
	pMain->m_iRectAlpha = 0;
	pMain->m_pBackMenuTips->Action();

}

void SceneSelectState::BackMenu::Execute(sceneSelect *pMain)
{
	// 暗転用
	pMain->m_iRectAlpha += 18;
	if (pMain->m_iRectAlpha >= 156)
	{
		pMain->m_iRectAlpha = 156;
	}

	// (仮 後で消す)最初の選択へ戻る
	//if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iSenderDeviceID) == 3)
	//{
	//	if (pMain->m_pBackMenuTips->GetStep() == TipsCard::STEP::EXECUTE)
	//	{
	//		// 次のステートへ
	//		pMain->GetFSM()->ChangeState(SceneSelectState::CharaSelect::GetInstance());
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

		if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::OK)
		{
			// フェード
			Fade::Set(Fade::FLAG::FADE_OUT, 8, 0xFF000000);
			return;
		}

		if (pMain->GetBackMenuTips()->GetSelectState() == TipsCard::SELECT_STATE::CANCEL)
		{
			// キャラセレに戻る
			pMain->GetFSM()->ChangeState(SceneSelectState::CharaSelect::GetInstance());
			return;
		}

	}




}

void SceneSelectState::BackMenu::Exit(sceneSelect *pMain) 
{
	pMain->m_iRectAlpha = 0;
	pMain->m_pBackMenuTips->End();
}

void SceneSelectState::BackMenu::Render(sceneSelect *pMain)
{
	tdnText::Draw(0, 0, 0xffffffff, "BackMenu");
}

bool SceneSelectState::BackMenu::OnMessage(sceneSelect *pMain, const Message & msg)
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

