#include "TDNLIB.h"
#include "BattleLoading.h"
#include "BattleLoadingState.h"
#include "../Fade/Fade.h"
#include "../BaseEntity/Message/Message.h"
#include "Data\SelectData.h"
#include "system\System.h"

int GetCharaType(SIDE eSide)
{
	int l_iNo = 0;

	if (eSide == SIDE::LEFT)
	{
		l_iNo = (int)SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].eCharacter;
	}
	else
	{
		l_iNo = (int)SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].eCharacter;
	}

	return l_iNo;
}

/*******************************************************/
//					開始ステート
/*******************************************************/

void BattleLoadingState::Intro::Enter(BattleLoading *pMain)
{
	// フェード初期化
	Fade::Set(Fade::FLAG::FADE_IN, 64);

	for (int i = 0; i < BattleLoading::IMAGE::ARRAY_END; i++)
	{
		pMain->m_pImages[i].End();
	}
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		pMain->m_tagSlideChara1P[i].End();
		pMain->m_tagSlideChara2P[i].End();
		pMain->m_tagFinalChara1P[i].End();
		pMain->m_tagFinalChara2P[i].End();
	}

	// 初期演出
	//pMain->FirstAction();

	pMain->m_pImages[BattleLoading::IMAGE::BLACK_LINE].Action();
	

}

void BattleLoadingState::Intro::Execute(BattleLoading *pMain)
{

	// 1Pのスライドへ
	pMain->GetFSM()->ChangeState(BattleLoadingState::Slide1P::GetInstance());

}

void BattleLoadingState::Intro::Exit(BattleLoading *pMain) 
{

}

void BattleLoadingState::Intro::Render(BattleLoading *pMain)
{


#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Intro");
#endif // _DEBUG

}

bool BattleLoadingState::Intro::OnMessage(BattleLoading *pMain, const Message & msg)
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
//					1Pスライドステート
/*******************************************************/

void BattleLoadingState::Slide1P::Enter(BattleLoading *pMain)
{
	// 周りのフレームなどのイラスト
	pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_VS].Action();

	// 1PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_1PLAYER].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].Action();

	// キャラ
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Action();

	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::Slide1P::Execute(BattleLoading *pMain)
{
	// 更新
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 120)
	{
		// 2Pのスライドへ
		pMain->GetFSM()->ChangeState(BattleLoadingState::Slide2P::GetInstance());
		return;
	}

}

void BattleLoadingState::Slide1P::Exit(BattleLoading *pMain)
{
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].End();
	
	// PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_1PLAYER].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].End();


}

void BattleLoadingState::Slide1P::Render(BattleLoading *pMain)
{
	// 描画
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Render();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Render();

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Slide1P");
#endif // _DEBUG

}

bool BattleLoadingState::Slide1P::OnMessage(BattleLoading *pMain, const Message & msg)
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
//					2Pスライドステート
/*******************************************************/

void BattleLoadingState::Slide2P::Enter(BattleLoading *pMain)
{
	// キャラ
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Action();

	// 2PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_2PLAYER].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK].Action();
	
	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::Slide2P::Execute(BattleLoading *pMain)
{
	// 更新
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 120)
	{
		// スライド終りへ
		pMain->GetFSM()->ChangeState(BattleLoadingState::SlideEnd::GetInstance());
		return;
	}

}

void BattleLoadingState::Slide2P::Exit(BattleLoading *pMain)
{
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].End();
	
	// PLAYER
	pMain->m_pImages[BattleLoading::IMAGE::S_2PLAYER].End();
	//pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK].End();// まだ2Pの背景残す

}

void BattleLoadingState::Slide2P::Render(BattleLoading *pMain)
{
	// 描画
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Render();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Render();

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "Slide2P");
#endif // _DEBUG

}

bool BattleLoadingState::Slide2P::OnMessage(BattleLoading *pMain, const Message & msg)
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
//					スライド終りステート
/*******************************************************/

void BattleLoadingState::SlideEnd::Enter(BattleLoading *pMain)
{
	pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].End();
	pMain->m_pImages[BattleLoading::IMAGE::S_VS].End();

	// 最終フレームの演出開始
	pMain->m_pImages[BattleLoading::IMAGE::F_FRAME].Action(6);

	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::SlideEnd::Execute(BattleLoading *pMain)
{

	// 更新
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	// スライド時のUIをはける
	pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].AddPos(Vector2(0, -4.5));
	pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].AddPos(Vector2(0, 4.5));
	pMain->m_pImages[BattleLoading::IMAGE::S_VS].AddPos(Vector2(0, 4));


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 8)
	{
		// 次へ
		pMain->GetFSM()->ChangeState(BattleLoadingState::FinalStep::GetInstance());
		return;
	}

}

void BattleLoadingState::SlideEnd::Exit(BattleLoading *pMain)
{
	// ここで背景消す
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK].End();
}

void BattleLoadingState::SlideEnd::Render(BattleLoading *pMain)
{
	

#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "SlideEnd");
#endif // _DEBUG

}

bool BattleLoadingState::SlideEnd::OnMessage(BattleLoading *pMain, const Message & msg)
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
//					最終ステート
/*******************************************************/

void BattleLoadingState::FinalStep::Enter(BattleLoading *pMain)
{
	// 上下のラインを消す
	pMain->m_pImages[BattleLoading::IMAGE::BLACK_LINE].End();

	// 開始
	pMain->m_pImages[BattleLoading::IMAGE::F_BLACK_CIRCLE].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_BACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_FLASH_BACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_VERSUS].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_FLASH].Action();

	// キャラ
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Action();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Action();


	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::FinalStep::Execute(BattleLoading *pMain)
{

	//// 更新
	//pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	//pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();
	//
	//// スライド時のUIをはける
	//pMain->m_pImages[BattleLoading::IMAGE::S_UP_FRAME].AddPos(Vector2(0, -4.5));
	//pMain->m_pImages[BattleLoading::IMAGE::S_DOWN_FRAME].AddPos(Vector2(0, 4.5));
	//pMain->m_pImages[BattleLoading::IMAGE::S_VS].AddPos(Vector2(0, 4));

	// キャラ
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Update();


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 180)
	{
		// 次へ
		pMain->GetFSM()->ChangeState(BattleLoadingState::FadeChangeStep::GetInstance());
		return;
	}

}

void BattleLoadingState::FinalStep::Exit(BattleLoading *pMain)
{
	// (TODO)
	//pMain->m_pImages[BattleLoading::IMAGE::F_BLACK_CIRCLE].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_BACK].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_VERSUS].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_FRAME].End();

}

void BattleLoadingState::FinalStep::Render(BattleLoading *pMain)
{

	// キャラ
	//shader2D->SetMaskScreen(pMain->m_pMask2P);
	//pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Render();
	//shader2D->SetMaskScreen(pMain->m_pMask2P);

	// マスク
	pMain->MaskRender(pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)]);
	shader2D->SetMaskScreen(pMain->m_pMask1P);
	pMain->m_pMaskSurface->Render(0, 0,shader2D,"Mask");

	// マスク
	pMain->MaskRender(pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)]);
	shader2D->SetMaskScreen(pMain->m_pMask2P);
	pMain->m_pMaskSurface->Render(0, 0, shader2D, "Mask");


#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "SlideEnd");
#endif // _DEBUG

}

bool BattleLoadingState::FinalStep::OnMessage(BattleLoading *pMain, const Message & msg)
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
//					フェードチェンジステート
/*******************************************************/

void BattleLoadingState::FadeChangeStep::Enter(BattleLoading *pMain)
{
	// フェード
	Fade::Set(Fade::FLAG::FADE_OUT, 8);

	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::FadeChangeStep::Execute(BattleLoading *pMain)
{

	// 更新

	// キャラ
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Update();


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 24)
	{
		// 次へ
		//pMain->GetFSM()->ChangeState(BattleLoadingState::Intro::GetInstance());
		//return;
	}

}

void BattleLoadingState::FadeChangeStep::Exit(BattleLoading *pMain)
{
	// (TODO)
	//pMain->m_pImages[BattleLoading::IMAGE::F_BLACK_CIRCLE].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_BACK].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_VERSUS].End();
	//pMain->m_pImages[BattleLoading::IMAGE::F_FRAME].End();

}

void BattleLoadingState::FadeChangeStep::Render(BattleLoading *pMain)
{

	// キャラ
	//shader2D->SetMaskScreen(pMain->m_pMask2P);
	//pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Render();
	//shader2D->SetMaskScreen(pMain->m_pMask2P);

	// マスク
	pMain->MaskRender(pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)]);
	shader2D->SetMaskScreen(pMain->m_pMask1P);
	pMain->m_pMaskSurface->Render(0, 0, shader2D, "Mask");

	// マスク
	pMain->MaskRender(pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)]);
	shader2D->SetMaskScreen(pMain->m_pMask2P);
	pMain->m_pMaskSurface->Render(0, 0, shader2D, "Mask");


#ifdef _DEBUG
	tdnText::Draw(0, 0, 0xffffffff, "SlideEnd");
#endif // _DEBUG

}

bool BattleLoadingState::FadeChangeStep::OnMessage(BattleLoading *pMain, const Message & msg)
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









