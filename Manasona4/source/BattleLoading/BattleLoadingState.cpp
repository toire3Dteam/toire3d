#include "TDNLIB.h"
#include "BattleLoading.h"
#include "BattleLoadingState.h"
#include "../Fade/Fade.h"
#include "../BaseEntity/Message/Message.h"
#include "Data\SelectData.h"
#include "system\System.h"
#include "system\FrameworkEx.h"


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
	// Fade::Set(Fade::FLAG::FADE_IN, 64);

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

	// 最初は黒
	pMain->m_fFadeRate = 1.0f;

	// 初期演出
	//pMain->m_pImages[BattleLoading::IMAGE::BLUE_RING].Action();
	pMain->m_pImages[BattleLoading::IMAGE::BLACK_LINE].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].Action();

	
	// タイマー初期化
	pMain->m_iSceneFrame = 0;

}

void BattleLoadingState::Intro::Execute(BattleLoading *pMain)
{


	// 
	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 18)
	{
		pMain->m_fFadeRate -= 0.1f;
	}

	if (pMain->m_iSceneFrame == 18)
	{
		pMain->m_pImages[BattleLoading::IMAGE::I_INTRO_LIGHT].Action();
	}

	//if (pMain->m_fFadeRate <= 0.0f)
	if (pMain->m_iSceneFrame >= 46)
	{
		// 1Pのスライドへ
		pMain->GetFSM()->ChangeState(BattleLoadingState::Slide1P::GetInstance());
		return;
	}
}

void BattleLoadingState::Intro::Exit(BattleLoading *pMain) 
{
	pMain->m_pImages[BattleLoading::IMAGE::I_INTRO_LIGHT].End();

}

void BattleLoadingState::Intro::Render(BattleLoading *pMain)
{

	// 黒い板
	pMain->m_pIntroBlackBack->Render(0, 0);


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
	//pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].vPos = VECTOR2_ZERO;

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

	// スピードライン
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].vPos.x += 20.0f;

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
	pMain->m_pImages[BattleLoading::IMAGE::S_1PBACK_SL].End();


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
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].Action();
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].vPos = VECTOR2_ZERO;


	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::Slide2P::Execute(BattleLoading *pMain)
{
	// 更新
	pMain->m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].Update();

	// スピードライン
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].vPos.x -= 20.0f;

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
	pMain->m_pImages[BattleLoading::IMAGE::S_2PBACK_SL].End();


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

	// 光る円
	pMain->m_pImages[BattleLoading::IMAGE::BLUE_RING].Action();

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
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].fAngle = 0.0f;
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].fAngle = 0.0f;

	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].Action();
	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].vPos = VECTOR2_ZERO;

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
	// アニメーション
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].fAngle += 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].fAngle -= 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].vPos.y += 2.0f;

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

	// ↓のフレームまでキャラを少し動かす
	if (pMain->m_iSceneFrame <= 60)
	{
		pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].vPos.x -= 0.25f;
		pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].vPos.x += 0.25f;
	}

	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 180)
	{
		// 次へ
		pMain->GetFSM()->ChangeState(BattleLoadingState::FadeChangeStep::GetInstance());
		return;
	}

	// スキップ可能なら
	if (pMain->m_bSkip == true)
	{
		// パッド分更新
		int NumDevice(tdnInputManager::GetNumDevice());
		if (NumDevice == 0)NumDevice = 1;
		for (int i = 0; i < NumDevice; i++)
		{
			// メニュー切り替え
			if (tdnInput::KeyGet(KEYCODE::KEY_ENTER, i) == 3||
				tdnInput::KeyGet(KEYCODE::KEY_A, i) == 3 || 
				tdnInput::KeyGet(KEYCODE::KEY_B, i) == 3 )
			{
				// 次へ
				pMain->GetFSM()->ChangeState(BattleLoadingState::FadeChangeStep::GetInstance());
				return;

			}
		}
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
	// Fade::Set(Fade::FLAG::FADE_OUT, 8);

	pMain->m_pImages[BattleLoading::IMAGE::LOADING_CIRCLE].Action();

	// タイマー初期化
	pMain->m_iSceneFrame = 0;
}

void BattleLoadingState::FadeChangeStep::Execute(BattleLoading *pMain)
{

	// 更新
	pMain->m_fFadeRate += 0.05f;

	// アニメーション
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_1].fAngle += 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_CIRCLE_2].fAngle -= 0.005f;
	pMain->m_pImages[BattleLoading::IMAGE::F_LIGHT_PARTICLE].vPos.y += 2.0f;

	// ローディング用
	pMain->m_pImages[BattleLoading::IMAGE::LOADING_CIRCLE].fAngle -= 0.1f;


	// キャラ
	pMain->m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Update();
	pMain->m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Update();


	pMain->m_iSceneFrame++;
	if (pMain->m_iSceneFrame >= 28)
	{
		// ★演出終了時にFPSを戻す
		if (MainFrameEX->GetFPSMode() != FPS_MODE::NORMAL)
		{
			MainFrameEX->SetFPSMode(FPS_MODE::NORMAL);
		}

		// 演出終了
		pMain->SetEndFlag(true);

		//pMain->GetFSM()->ChangeState(BattleLoadingState::Intro::GetInstance());
		//return;	
	}

}

void BattleLoadingState::FadeChangeStep::Exit(BattleLoading *pMain)
{
	pMain->m_pImages[BattleLoading::IMAGE::LOADING_CIRCLE].End();
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









