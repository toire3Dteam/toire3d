#include "BattleLoading.h"
#include "BattleLoadingState.h"
#include "../Fade/Fade.h"
#include "BaseEntity\Message\MessageDispatcher.h"
#include "system\System.h"


//+---------------------------
//	バトル前ロード画面
//+---------------------------

// 前方宣言
BattleLoading* BattleLoading::m_pInstance = nullptr;

BattleLoading::BattleLoading() :BaseGameEntity(ENTITY_ID::BATTLE_LOADING)
{
	// 画像初期化
	m_pImages[IMAGE::BLACK_LINE].pPic = new tdn2DAnim("DATA/UI/BattleLoading/BlackLine.png");
	m_pImages[IMAGE::S_UP_FRAME].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/upframe.png");
	m_pImages[IMAGE::S_DOWN_FRAME].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/downframe.png");
	m_pImages[IMAGE::S_VS].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/vs.png");
	m_pImages[IMAGE::S_1PLAYER].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/1PLAYER.png");
	m_pImages[IMAGE::S_2PLAYER].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/2PLAYER.png");
	m_pImages[IMAGE::S_1PBACK].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/1PBACK.png");
	m_pImages[IMAGE::S_2PBACK].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Slide/2PBACK.png");

	m_pImages[IMAGE::F_BACK].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Final/Back.png");
	m_pImages[IMAGE::F_VERSUS].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Final/Versus.png");
	m_pImages[IMAGE::F_FRAME].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Final/versus_frame_all.png");
	m_pImages[IMAGE::F_FLASH].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Final/Flash.png");
	m_pImages[IMAGE::F_FLASH_BACK].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Final/BackFlash.png");

	m_pImages[IMAGE::F_BLACK_CIRCLE].pPic = new tdn2DAnim("DATA/UI/BattleLoading/Final/BlackCircle.png");

	for (int i = 0; i < IMAGE::ARRAY_END; i++)
	{
		m_pImages[i].pPic->OrderMoveAppeared(4, 0, 0);
	}

	m_pImages[IMAGE::BLACK_LINE].pPic->Action();
	
	// 各画像の設定
	m_pImages[IMAGE::S_UP_FRAME].pPic->OrderMoveAppeared(12, 0, 256);
	m_pImages[IMAGE::S_UP_FRAME].vMove.y = -0.05f;
	m_pImages[IMAGE::S_DOWN_FRAME].pPic->OrderMoveAppeared(12, 0, -256);
	m_pImages[IMAGE::S_DOWN_FRAME].vMove.y = +0.05f;

	m_pImages[IMAGE::S_VS].pPic->OrderMoveAppeared(8, 0, -128);
	m_pImages[IMAGE::S_VS].vMove.y = +0.01f;

	m_pImages[IMAGE::S_1PLAYER].pPic->OrderMoveAppeared(8, 0, 128);
	m_pImages[IMAGE::S_2PLAYER].pPic->OrderMoveAppeared(8, 0, 128);

	// 
	m_pImages[IMAGE::F_FRAME].pPic->OrderMoveAppeared(8, 0, 0);
	m_pImages[IMAGE::F_VERSUS].pPic->OrderShrink(8, 1, 3);
	m_pImages[IMAGE::F_FLASH].pPic->OrderRipple(16, 1.5f, -0.05f);
	m_pImages[IMAGE::F_FLASH_BACK].pPic->OrderRipple(42, 1.0f, 0.001f);

	//m_pImages[IMAGE::F_BLACK_CIRCLE].pPic->OrderShrink(8, 1, 3);
	//m_pImages[IMAGE::F_BLACK_CIRCLE].vPos.x=

	m_pMask1P = new tdn2DObj("Data/UI/BattleLoading/Final/LeftMask.png");
	m_pMask2P = new tdn2DObj("Data/UI/BattleLoading/Final/RightMask.png");
	m_pMaskSurface = new tdn2DObj(1280, 720, TDN2D::USEALPHA);

	m_vCirclePos.x = 300;
	m_vCirclePos.y = 413;
	m_pImages[IMAGE::F_BLACK_CIRCLE].vStartPos = m_vCirclePos;
	m_pImages[IMAGE::F_BLACK_CIRCLE].pPic->OrderMoveAppeared(8,
		(int)m_pImages[IMAGE::F_BLACK_CIRCLE].vStartPos.x, (int)m_pImages[IMAGE::F_BLACK_CIRCLE].vStartPos.y);


	for (int i = 0; i < (int)CHARACTER::END; i++)
	{

		// 初期設定
		m_tagSlideChara1P[i].vStartPos.x = 200;
		m_tagSlideChara2P[i].vStartPos.x = -200;
		m_tagSlideChara1P[i].vStartPos.y = 0;
		m_tagSlideChara2P[i].vStartPos.y = 0;

		// 動き
		m_tagSlideChara1P[i].vMove.x = 1.0f;
		m_tagSlideChara2P[i].vMove.x = -1.0f;

		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_tagSlideChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Airou.png");
			m_tagSlideChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Airou.png");
			m_tagSlideChara1P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/AirouName.png");
			m_tagSlideChara2P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/AirouName.png");

			m_tagSlideChara1P[i].vStartPos.x = 0;
			m_tagSlideChara2P[i].vStartPos.x = -400;

			break;
		case CHARACTER::TEKI:
			m_tagSlideChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Teki.png");
			m_tagSlideChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Teki.png");
			m_tagSlideChara1P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/TekiName.png");
			m_tagSlideChara2P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/TekiName.png");

			break;
		case CHARACTER::NAZENARA:
			m_tagSlideChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Nazenaraba.png");
			m_tagSlideChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Nazenaraba.png");
			m_tagSlideChara1P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/NazenarabaName.png");
			m_tagSlideChara2P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/NazenarabaName.png");

			break;
		case CHARACTER::ARAMITAMA:
			m_tagSlideChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aramitama.png");
			m_tagSlideChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aramitama.png");
			m_tagSlideChara1P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/AramitamaName.png");
			m_tagSlideChara2P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/AramitamaName.png");

			break;
		case CHARACTER::ANIKI:
			m_tagSlideChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aniki.png");
			m_tagSlideChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aniki.png");
			m_tagSlideChara1P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/AnikiName.png");
			m_tagSlideChara2P[i].pNamePic = new tdn2DAnim("Data/UI/BattleLoading/CHR/AnikiName.png");

			break;
		default:
			MyAssert(0, "そのタイプは存在しない");
			break;
		}

		// 左から右へ
		m_tagSlideChara1P[i].pPic->OrderMoveAppeared(12, (int)m_tagSlideChara1P[i].vPos.x -(1280), (int)m_tagSlideChara1P[i].vPos.y);
		// 右から左へ
		m_tagSlideChara2P[i].pPic->OrderMoveAppeared(12, (int)m_tagSlideChara2P[i].vPos.x +(1280), (int)m_tagSlideChara2P[i].vPos.y);

		// 名前
		m_tagSlideChara1P[i].pNamePic->OrderMoveAppeared(8, 0, 128);
		m_tagSlideChara2P[i].pNamePic->OrderMoveAppeared(8, 0, 128);

	}

	//+--------------------------------------------
	// 最後
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		// 初期設定
		//m_tagFinalChara1P[i].vStartPos.x = -300;
		//m_tagFinalChara2P[i].vStartPos.x = 300;
		m_tagFinalChara1P[i].vStartPos.x = -350;
		m_tagFinalChara2P[i].vStartPos.x = 350;
		
		m_tagFinalChara1P[i].vStartPos.y = 0;
		m_tagFinalChara2P[i].vStartPos.y = 0;

		// 動き
		//m_tagFinalChara1P[i].vMove.x = 1.0f;
		//m_tagFinalChara2P[i].vMove.x = -1.0f;

		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_tagFinalChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Airou.png");
			m_tagFinalChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Airou.png");

			m_tagFinalChara1P[i].vStartPos.x = -500;
			m_tagFinalChara2P[i].vStartPos.x = 100;
			break;
		case CHARACTER::TEKI:
			m_tagFinalChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Teki.png");
			m_tagFinalChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Teki.png");
			break;
		case CHARACTER::NAZENARA:
			m_tagFinalChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Nazenaraba.png");
			m_tagFinalChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Nazenaraba.png");
	
			break;
		case CHARACTER::ARAMITAMA:
			m_tagFinalChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aramitama.png");
			m_tagFinalChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aramitama.png");

			break;
		case CHARACTER::ANIKI:
			m_tagFinalChara1P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aniki.png");
			m_tagFinalChara2P[i].pPic = new tdn2DAnim("Data/UI/BattleLoading/CHR/Aniki.png");

			break;
		default:
			MyAssert(0, "そのタイプは存在しない");
			break;
		}

		// 左から右へ
		m_tagFinalChara1P[i].pPic->OrderMoveAppeared(18, (int)m_tagFinalChara1P[i].vPos.x + (128), (int)m_tagFinalChara1P[i].vPos.y);
		m_tagFinalChara2P[i].pPic->OrderMoveAppeared(18, (int)m_tagFinalChara2P[i].vPos.x - (128), (int)m_tagFinalChara2P[i].vPos.y);

	}

	// シーン用のフレーム
	m_iSceneFrame = 0;

	// ステートマシン初期化 
	m_pStateMachine = new StateMachine<BattleLoading>(this);
	m_pStateMachine->SetCurrentState(BattleLoadingState::Intro::GetInstance());


}

BattleLoading::~BattleLoading()
{
	FOR(IMAGE::ARRAY_END)
	{
		SAFE_DELETE(m_pImages[i].pPic);
	}
	
	FOR((int)CHARACTER::END)
	{
		SAFE_DELETE(m_tagSlideChara1P[i].pPic);
		SAFE_DELETE(m_tagSlideChara2P[i].pPic);
		SAFE_DELETE(m_tagSlideChara1P[i].pNamePic);
		SAFE_DELETE(m_tagSlideChara2P[i].pNamePic);

		SAFE_DELETE(m_tagFinalChara1P[i].pPic);
		SAFE_DELETE(m_tagFinalChara2P[i].pPic);
	}

	SAFE_DELETE(m_pMask1P);
	SAFE_DELETE(m_pMask2P);
	SAFE_DELETE(m_pMaskSurface);


	SAFE_DELETE(m_pStateMachine);

}

void BattleLoading::Update()
{
	FOR(IMAGE::ARRAY_END)
	{
		m_pImages[i].Update();
	}

	//FOR((int)CHARACTER::END)
	//{
	//	m_tagSlideChara1P[i].Update();
	//	m_tagSlideChara2P[i].Update();
	//}

	// ★ステートマシン更新
	m_pStateMachine->Update();

#ifdef _DEBUG
	if (KeyBoardTRG(KB_Z))
	{
		m_tagSlideChara1P[0].Action();
	}
	
	if (KeyBoardTRG(KB_X))
	{
		m_tagSlideChara2P[0].Action();
	}

#endif // _DEBUG

}

void BattleLoading::Render()
{
	//FOR((int)CHARACTER::END)
	//{
	//	m_tagSlideChara1P[i].Render();
	//	m_tagSlideChara2P[i].Render();
	//}

	// 背景
	m_pImages[IMAGE::S_1PBACK].Render();
	m_pImages[IMAGE::S_2PBACK].Render();

	m_pImages[IMAGE::F_BACK].Render();
	m_pImages[IMAGE::F_FLASH_BACK].Render(RS::ADD);


	// ★ここにステートマシン描画
	m_pStateMachine->Render();

	// 上下のライン
	m_pImages[IMAGE::BLACK_LINE].Render();
	m_pImages[IMAGE::S_UP_FRAME].Render();
	m_pImages[IMAGE::S_DOWN_FRAME].Render();
	m_pImages[IMAGE::S_VS].Render();
	m_pImages[IMAGE::S_1PLAYER].Render();
	m_pImages[IMAGE::S_2PLAYER].Render();

	m_pImages[IMAGE::F_BLACK_CIRCLE].Render();
	m_pImages[IMAGE::F_FRAME].Render();
	m_pImages[IMAGE::F_VERSUS].Render();

	m_pImages[IMAGE::F_FLASH].Render(RS::ADD);

	// キャラのネーム
	m_tagSlideChara1P[GetCharaType(SIDE::LEFT)].RenderName();
	m_tagSlideChara2P[GetCharaType(SIDE::RIGHT)].RenderName();



	//FOR(IMAGE::ARRAY_END)
	//{
	//	m_pImages[i].Render();
	//}

}

void BattleLoading::MaskRender()
{
	// 現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);
	m_pMaskSurface->RenderTarget();

	// サーフェイス画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	// キャラクター画像
	m_tagFinalChara1P[GetCharaType(SIDE::LEFT)].Render();
	m_tagFinalChara2P[GetCharaType(SIDE::RIGHT)].Render();

	// レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);

}

void BattleLoading::MaskRender(FinalCharaDesc pFinalChara)
{
	// 現在のレンダーターゲットを一時的に確保
	Surface* now = nullptr;
	tdnSystem::GetDevice()->GetRenderTarget(0, &now);
	m_pMaskSurface->RenderTarget();

	// サーフェイス画面クリア
	tdnSystem::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	// キャラクター画像
	pFinalChara.Render();

	// レンダーターゲットの復元
	tdnSystem::GetDevice()->SetRenderTarget(0, now);

}

//+---------------------------------
//	スライドデータ
//+---------------------------------

void SlideDesc::Update()
{
	// 動き更新
	vPos += vMove;
	
	// フラグが立っていれば徐々透明に(TODO)
	if (bEnd == true)
	{
		fAlpha += -0.2f;

		// アルファ処理
		Math::Clamp(fAlpha, 0.0f, 1.0f);//   0~1.0fへ
		pPic->SetAlpha((int)(fAlpha * 255));
	}
	else
	{
		// MoveAppendの効果を適用
		pPic->Update();
	}


}

void SlideDesc::Render(RS eRS)
{
	pPic->Render((int)vPos.x, (int)vPos.y, eRS);
}

void SlideDesc::RenderSP()
{
	pPic->RenderSpecial((int)vPos.x, (int)vPos.y);
}

void SlideDesc::Action(int iDelayTimer)
{
	// 初期値へ
	vPos = vStartPos;
	fAlpha = 1.0f;

	// 演出開始
	pPic->Action(iDelayTimer);

	bEnd = false;

}

//+---------------------------------
//	スライドキャラクターデータ
//+---------------------------------

void SlideCharaDesc::Update()
{
	// 動き更新
	vPos += vMove;

	// フラグが立っていれば徐々透明に(TODO)
	if (bEnd == true)
	{
		fAlpha += -0.2f;

		// アルファ処理
		Math::Clamp(fAlpha, 0.0f, 1.0f);//   0~1.0fへ
		pPic->SetAlpha((int)(fAlpha * 255));
		pNamePic->SetAlpha((int)(fAlpha * 255));
	}
	else
	{
		// MoveAppendの効果を適用
		pPic->Update();
		pNamePic->Update();
	}


}

void SlideCharaDesc::Render()
{
	pPic->Render((int)vPos.x, (int)vPos.y);
}

void SlideCharaDesc::RenderName()
{
	pNamePic->Render(0, 0);
}

void SlideCharaDesc::Action(int iDelayTimer)
{
	// 初期値へ
	vPos = vStartPos;
	fAlpha = 1.0f;

	// 演出開始
	pPic->Action(iDelayTimer);
	pNamePic->Action(iDelayTimer);

	bEnd = false;

}

//+---------------------------------
//	最後のキャラクターデータ
//+---------------------------------

void FinalCharaDesc::Update()
{
	// 動き更新
	vPos += vMove;

	// フラグが立っていれば徐々透明に(TODO)
	if (bEnd == true)
	{
		// アルファ処理
		//Math::Clamp(fAlpha, 0.0f, 1.0f);//   0~1.0fへ
		//pPic->SetAlpha((int)(fAlpha * 255));
	}else
	{
		// MoveAppendの効果を適用
		pPic->Update();
	}

}

void FinalCharaDesc::Render()
{
	// マスク適用
	//pPic->Render((int)vPos.x, (int)vPos.y, shader2D, "Mask");// 
	//pPic->SetScale(0.9f);
	pPic->Render((int)vPos.x, (int)vPos.y);

}

void FinalCharaDesc::Action(int iDelayTimer)
{
	// 初期値へ
	vPos = vStartPos;

	// 演出開始
	pPic->Action(iDelayTimer);

	bEnd = false;

}
