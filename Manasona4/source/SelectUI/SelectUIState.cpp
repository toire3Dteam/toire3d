#include "TDNLIB.h"
#include "SelectUI.h"
#include "SelectUIState.h"
#include "../BaseEntity/Message/Message.h"
#include "../Fade/Fade.h"
#include "../Sound/SoundManager.h"
#include "Data\SelectData.h"
#include "BaseEntity\Message\Message.h"
#include "BaseEntity\Message\MessageDispatcher.h"
//+--------------------
// 作業効率化
//+--------------------

// メニューのUI管理系
//#define M_UIMgr pMain->GetMenuUIMgr()
//#define C_UIMgr pMain->GetCtrlSelectUI()



/*******************************************************/
//					最初の演出
/*******************************************************/

void SelectUIState::Intro::Enter(SelectUI *pMain)
{
	// インフォメーション用の枠を配置
	pMain->ActionCharaPlate();
	pMain->m_iWaitFrame = 0;
}
void SelectUIState::Intro::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;
	// 最初の選択へ
	if (pMain->m_iWaitFrame >= 24)
	{
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		return;
	}
}
void SelectUIState::Intro::Exit(SelectUI *pMain) 
{

}
void SelectUIState::Intro::Render(SelectUI *pMain)
{
//	tdnText::Draw(0, 0, 0xffffffff, "Intro");
}

bool SelectUIState::Intro::OnMessage(SelectUI *pMain, const Message & msg)
{
	return false;
}


/*******************************************************/
//					最初の選択ステート(キャラセレ)
/*******************************************************/

void SelectUIState::FirstStep::Enter(SelectUI *pMain)
{

	// 演出
	pMain->ActionChara();	

}
void SelectUIState::FirstStep::Execute(SelectUI *pMain)
{
	// キャラセレクトの操作
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, pMain->m_iDeviceID) == 3) 
	{
		pMain->m_iSelectCharacterNo--;
		
		
		// 一周回る処理
		if (pMain->m_iSelectCharacterNo <= -1)
		{
			pMain->m_iSelectCharacterNo = (int)CHARACTER::END - 1;
		}

		// 演出
		pMain->ActionChara();

		// SEの再生
		se->Play("カーソル2");
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectCharacterNo++;
		
		// 一周回る処理
		if (pMain->m_iSelectCharacterNo >= (int)CHARACTER::END)
		{
			pMain->m_iSelectCharacterNo = 0;
		}

		// 演出
		pMain->ActionChara();

		// SEの再生
		se->Play("カーソル2");
	}

	// カラー変更
	if (tdnInput::KeyGet(KEYCODE::KEY_L1, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectCharacterColor--;

		// 一周回る処理
		if (pMain->m_iSelectCharacterColor <= -1)
		{
			pMain->m_iSelectCharacterColor = (int)COLOR_TYPE::MIRROR;
		}

		// 演出
		//pMain->ActionChara();

		// SEの再生
		se->Play("カーソル2");
	}
	if (tdnInput::KeyGet(KEYCODE::KEY_R1, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectCharacterColor++;

		// 一周回る処理
		if (pMain->m_iSelectCharacterColor > (int)COLOR_TYPE::MIRROR)
		{
			pMain->m_iSelectCharacterColor = 0;
		}

		// 演出
		//pMain->ActionChara();

		// SEの再生
		se->Play("カーソル2");
	}

	/**********************************************************/
	
	// ×押してなかったら
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 0)
	{
		// キャラ決定
		if (tdnInput::KeyGet(KEYCODE::KEY_B, pMain->m_iDeviceID) == 3)
		{
			// SEの再生
			se->Play("決定1");

			// 隠しコマンド
			if (tdnInput::KeyGet(KEYCODE::KEY_L2, pMain->m_iDeviceID) == 1)
			{
				pMain->m_iSelectCharacterNo = (int)(CHARACTER::BALANCE);
				// 演出
				pMain->ActionChara();
			}

			pMain->GetFSM()->ChangeState(MiddleStep::GetInstance());
			return;
		}

		// △でランダムセレクト
		if (tdnInput::KeyGet(KEYCODE::KEY_D, pMain->m_iDeviceID) == 3)
		{
			pMain->m_iSelectCharacterNo = tdnRandom::Get(0, (int)(CHARACTER::END) - 1);
			
			// 演出(キャラの名前とか)
			pMain->ActionChara();

			// SEの再生
			se->Play("決定1");

			pMain->GetFSM()->ChangeState(MiddleStep::GetInstance());
			return;
		}

	}

		// メニューに戻りたいプレイヤーがいることを
		// シーン側に伝える
		int senderID = pMain->m_iDeviceID;
		
		if (pMain->IsAI() == false)
		{
			// 【×押して】でメニューに戻る
			if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
			{
				// SEの再生
				se->Play("キャンセル1");

				MsgMgr->Dispatch(0, pMain->GetID(), ENTITY_ID::SCENE_SELECT, MESSAGE_TYPE::BACK_MENU, &senderID);
			}
		}
		else // AIならば
		{	
			// 【×離し】て自分の操作するプレイヤーへ戻る
			if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
			{
				// SEの再生
				se->Play("キャンセル1");

				// 逆サイドに送り　一つ前に戻らす処理をする
				//ENTITY_ID id;
				if (pMain->GetID() == ENTITY_ID::SELECT_UI_LEFT)
				{
					// 左なら右へ
					MsgMgr->Dispatch(0, pMain->GetID(), ENTITY_ID::SELECT_UI_RIGHT, MESSAGE_TYPE::BACK_MENU, &senderID);
				}
				else
				{
					// 右なら左
					MsgMgr->Dispatch(0, pMain->GetID(), ENTITY_ID::SELECT_UI_LEFT, MESSAGE_TYPE::BACK_MENU, &senderID);
				}
			}
		}
	
}

void SelectUIState::FirstStep::Exit(SelectUI *pMain)
{
	pMain->StopFirstStep();
}

void SelectUIState::FirstStep::Render(SelectUI *pMain)
{

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "FirstStep");

	tdnText::Draw(0, 200, 0xffaaaaaa, "SelectCharacterColor->%d", pMain->m_iSelectCharacterColor);

#endif // _DEBUG

	
}

bool SelectUIState::FirstStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			中間の選択ステート(キャラとパートナーの間)
/*******************************************************/

void SelectUIState::MiddleStep::Enter(SelectUI *pMain)
{
	// 演出
	pMain->MiddleAction();
	pMain->m_iWaitFrame = 0;

}

void SelectUIState::MiddleStep::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;

	if (pMain->m_iWaitFrame >= 24)
	{
		pMain->m_iWaitFrame = 0;
		pMain->GetFSM()->ChangeState(SecondStep::GetInstance());
		return;
	}

}

void SelectUIState::MiddleStep::Exit(SelectUI *pMain)
{

}

void SelectUIState::MiddleStep::Render(SelectUI *pMain)
{
	//tdnText::Draw(0, 0, 0xffffffff, "MiddleStep");
}

bool SelectUIState::MiddleStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			二つ目の選択ステート(パートナー)
/*******************************************************/

void SelectUIState::SecondStep::Enter(SelectUI *pMain)
{

	// 演出
	pMain->ActionPartner();
	pMain->ActionSecondStep();

}

void SelectUIState::SecondStep::Execute(SelectUI *pMain)
{
	// ペルソナセレクトの操作
	if (tdnInput::KeyGet(KEYCODE::KEY_LEFT, pMain->m_iDeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		pMain->m_iSelectPartnerNo--;


		// 一周回る処理
		if (pMain->m_iSelectPartnerNo <= -1)
		{
			pMain->m_iSelectPartnerNo = (int)PARTNER::END - 1;
		}

		// 演出
		pMain->ActionPartner();

	}
	if (tdnInput::KeyGet(KEYCODE::KEY_RIGHT, pMain->m_iDeviceID) == 3)
	{
		// SEの再生
		se->Play("カーソル2");

		pMain->m_iSelectPartnerNo++;

		// 一周回る処理
		if (pMain->m_iSelectPartnerNo >= (int)PARTNER::END)
		{
			pMain->m_iSelectPartnerNo = 0;
		}

		// 演出
		pMain->ActionPartner();

	}
	/**********************************************************/

	// キャラ決定
	if (tdnInput::KeyGet(KEYCODE::KEY_B, pMain->m_iDeviceID) == 3)
	{
		// SEの再生
		se->Play("決定1");

		pMain->GetFSM()->ChangeState(SecondToOKStep::GetInstance());
		return;
	}
	
	// △でランダムセレクト
	if (tdnInput::KeyGet(KEYCODE::KEY_D, pMain->m_iDeviceID) == 3)
	{
		pMain->m_iSelectPartnerNo = tdnRandom::Get(0, (int)(PARTNER::END) - 1);
		
		// 演出(パートナーの名前)
		pMain->ActionPartner();

		// SEの再生
		se->Play("決定1");

		pMain->GetFSM()->ChangeState(SecondToOKStep::GetInstance());
		return;
	}

	// 戻る
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
	{
		// SEの再生
		se->Play("キャンセル1");

		pMain->BackSecondStep();
		pMain->GetFSM()->ChangeState(FirstStep::GetInstance());
		return;
	}

}

void SelectUIState::SecondStep::Exit(SelectUI *pMain)
{

}

void SelectUIState::SecondStep::Render(SelectUI *pMain)
{
	//tdnText::Draw(0, 0, 0xffffffff, "SecondStep");
}

bool SelectUIState::SecondStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			SecondToOKStep！
/*******************************************************/

void SelectUIState::SecondToOKStep::Enter(SelectUI *pMain)
{
	// 演出
	pMain->SecondToOKAction();
	pMain->m_iWaitFrame = 0;
}
void SelectUIState::SecondToOKStep::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;

	if (pMain->m_iWaitFrame >= 24)
	{
		pMain->m_iWaitFrame = 0;
		pMain->GetFSM()->ChangeState(OKStep::GetInstance());
		return;
	}

}

void SelectUIState::SecondToOKStep::Exit(SelectUI *pMain)
{

}

void SelectUIState::SecondToOKStep::Render(SelectUI *pMain)
{
	//tdnText::Draw(0, 0, 0xffffffff, "SecondToOKStep");
}

bool SelectUIState::SecondToOKStep::OnMessage(SelectUI *pMain, const Message & msg)
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
//			準備完了！
/*******************************************************/

void SelectUIState::OKStep::Enter(SelectUI *pMain)
{
	pMain->ActionOKStep();

	// 準備完了フラグをON！
	pMain->m_bOK = true;

}
void SelectUIState::OKStep::Execute(SelectUI *pMain)
{
	// 戻る
	if (tdnInput::KeyGet(KEYCODE::KEY_A, pMain->m_iDeviceID) == 3)
	{
		pMain->BackOKStep();
		pMain->GetFSM()->ChangeState(SecondStep::GetInstance());
		return;
	}

}

void SelectUIState::OKStep::Exit(SelectUI *pMain)
{
	// 準備完了フラグをOFF
	pMain->m_bOK = false;
}

void SelectUIState::OKStep::Render(SelectUI *pMain)
{
	//tdnText::Draw(0, 0, 0xffffffff, "OKStep");
}

bool SelectUIState::OKStep::OnMessage(SelectUI *pMain, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::BACK_MENU:
		// AIから戻れというメッセージがとどいたよ。
		pMain->BackOKStep();
		pMain->GetFSM()->ChangeState(OKToSecondStep::GetInstance());

		return true;
		break;
	default:
		break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//			準備完了からパートナー選びに戻る
/*******************************************************/

void SelectUIState::OKToSecondStep::Enter(SelectUI *pMain)
{
	pMain->m_iWaitFrame = 0;
}
void SelectUIState::OKToSecondStep::Execute(SelectUI *pMain)
{
	pMain->m_iWaitFrame++;
	// パートナーの選択へ
	if (pMain->m_iWaitFrame >= 2)
	{
		pMain->GetFSM()->ChangeState(SecondStep::GetInstance());
		return;
	}
}

void SelectUIState::OKToSecondStep::Exit(SelectUI *pMain)
{
}

void SelectUIState::OKToSecondStep::Render(SelectUI *pMain)
{
	//tdnText::Draw(0, 0, 0xffffffff, "SecondToOKStep");
}

bool SelectUIState::OKToSecondStep::OnMessage(SelectUI *pMain, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::BACK_MENU:
	//	// AIから戻れというメッセージがとどいたよ。
	//	pMain->BackOKStep();
	//	pMain->GetFSM()->ChangeState(SecondStep::GetInstance());

	//	return true;
	//	break;
	//default:
	//	break;
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}
