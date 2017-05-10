#include "SelectUIManager.h"
#include "Data\SelectData.h"
#include "SelectUI.h"

//+-----------------------------------
//	キャラクターセレクトマネージャー
//+-----------------------------------

SelectUIManager::SelectUIManager()
{

	// (TODO)一つの処理にまとめる

	//// 左にAIが存在するかチェック
	//if (SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI == true)
	//{
	//	// 二つのキャラセレを片方の一人が操作するように
	//	m_pLeftSide = new SelectUI(SIDE::LEFT, ENTITY_ID::SELECT_UI_LEFT,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI);
	//	m_pRightSide = new SelectUI(SIDE::RIGHT, ENTITY_ID::SELECT_UI_RIGHT,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI);

	//}// 右にAIが存在するかチェック
	//else if (SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI == true)
	//{
	//	// 二つのキャラセレを片方の一人が操作するように
	//	m_pLeftSide = new SelectUI(SIDE::LEFT, ENTITY_ID::SELECT_UI_LEFT,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI);

	//	m_pRightSide = new SelectUI(SIDE::RIGHT, ENTITY_ID::SELECT_UI_RIGHT,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID,
	//		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI);
	//}
	//else // どちらもAIじゃなかった
	{
		// ★デバイス番号を設定　＆AIかどうか
		m_pLeftSide = new SelectUI(SIDE::LEFT, ENTITY_ID::SELECT_UI_LEFT,
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].iDeviceID,
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::LEFT].bAI);

		m_pRightSide = new SelectUI(SIDE::RIGHT, ENTITY_ID::SELECT_UI_RIGHT,
			SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].iDeviceID,
		SelectDataMgr->Get()->tagSideDatas[(int)SIDE::RIGHT].bAI);
	}



	//for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	//{
	//	m_pPic[i] = nullptr;
	//}

	//m_pPic[PIC_TYPE::BG] = new tdn2DAnim("DATA/UI/CharacterSelect/BG.png");
	//m_pPic[PIC_TYPE::BG]->OrderNone();
	//m_pPic[PIC_TYPE::BG]->Action();

	//m_pPic[PIC_TYPE::TITLE] = new tdn2DAnim("DATA/UI/CharacterSelect/title.png");
	//m_pPic[PIC_TYPE::TITLE]->OrderMoveAppeared(16, 0, -200);

	//m_pPic[PIC_TYPE::BLACK_LINE] = new tdn2DAnim("DATA/UI/CharacterSelect/BlackLine.png");
	//m_pPic[PIC_TYPE::BLACK_LINE]->OrderMoveAppeared(24, 1280, 0);

	//m_pPic[PIC_TYPE::INFO_PLATE] = new tdn2DAnim("DATA/UI/CharacterSelect/Information.png");
	//m_pPic[PIC_TYPE::INFO_PLATE]->OrderMoveAppeared(24, 0, 592 + 200);

	//// 説明のBOX
	//m_pPic[PIC_TYPE::INFO_BOX_LEFT] = new tdn2DAnim("DATA/UI/CharacterSelect/Info/InfoBox.png");
	//m_pPic[PIC_TYPE::INFO_BOX_LEFT]->OrderMoveAppeared(12, - 384, 354);
	//
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT] = new tdn2DAnim("DATA/UI/CharacterSelect/Info/InfoBox.png");
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT]->OrderMoveAppeared(12, 899 + 384 , 354);

	// アイコン幅
	m_iWidthIcon = 88;// 88
	m_iIconX = 404;	  // 384

	// キャラアイコン
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{

		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/airou.png");
			break;
		case CHARACTER::TEKI:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/teki.png");
			break;
		case CHARACTER::NAZENARA:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/nazenaraba.png");
			break;
		case CHARACTER::ARAMITAMA:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/aramitama.png");
			break;
		case CHARACTER::ANIKI:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/aniki.png");
			break;
		case CHARACTER::BALANCE:
			m_pCharacterIcon[i] = new tdn2DAnim("Data/UI/CharacterSelect/Icon/aniki.png");
			break;
		default:
			MyAssert(0,"そんなアイコンはない");
			break;
		}	

		// 共通設定
		m_pCharacterIcon[i]->OrderMoveAppeared(12, ((i * m_iWidthIcon) % (MOVING_UP * m_iWidthIcon)) + m_iIconX, 482 + m_iIconX);
	}


}

SelectUIManager::~SelectUIManager()
{
	SAFE_DELETE(m_pLeftSide);
	SAFE_DELETE(m_pRightSide);

	//for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	//{
	//	SAFE_DELETE(m_pPic[i]);
	//}

	// キャラアイコン
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		SAFE_DELETE(m_pCharacterIcon[i])
	}

}

//	更新時にデバイス取得
void SelectUIManager::Update(bool bControl)
{

	
	// 更新
	//for (int i = 0; i < PIC_TYPE::ARRAY_END; i++)
	//{
	//	m_pPic[i]->Update();
	//}

	// キャラアイコン更新
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		m_pCharacterIcon[i]->Update();
	}

	// 左にAIが存在するかチェック
	if (m_pLeftSide->IsAI() == true)
	{
		// 中でステートマシンによる複雑な更新がある
		if (m_pRightSide->IsOK())
		{
			//
			m_pLeftSide->Update(bControl);
			m_pRightSide->Update(false);
		}
		else
		{
			m_pLeftSide->Update(false);
			m_pRightSide->Update(bControl);
		}

	}else if (m_pRightSide->IsAI() == true)
	{
		// 中でステートマシンによる複雑な更新がある
		if (m_pLeftSide->IsOK())
		{
			//
			m_pLeftSide->Update(false);
			m_pRightSide->Update(bControl);
		}
		else
		{
			m_pLeftSide->Update(bControl);
			m_pRightSide->Update(false);

		}

	}else // 二人参加
	{
		// 中でステートマシンによる複雑な更新がある
		m_pLeftSide->Update(bControl);
		m_pRightSide->Update(bControl);
	}

	
}

void SelectUIManager::Render()
{
	//m_pPic[PIC_TYPE::BG]->Render(0, 0);

	//m_pPic[PIC_TYPE::BLACK_LINE]->Render(0, 0);

	// キャラアイコン
	for (int i = 0; i < (int)CHARACTER::BALANCE; i++)
	{
		if ((int)m_pLeftSide->GetSelectCharacter() == i)
		{
			if (m_pLeftSide->GetFSM()->isInState(*SelectUIState::Intro::GetInstance()) == false)
			{
				m_pCharacterIcon[i]->SetARGB(0xffffffff);
			}
			else
			{
				m_pCharacterIcon[i]->SetARGB(0xffaaaaaa);
			}

		}else if ((int)m_pRightSide->GetSelectCharacter() == i)
		{
			if (m_pRightSide->GetFSM()->isInState(*SelectUIState::Intro::GetInstance()) == false)
			{
				m_pCharacterIcon[i]->SetARGB(0xffffffff);
			}
			else
			{
				m_pCharacterIcon[i]->SetARGB(0xffaaaaaa);
			}
		}
		else
		{
			m_pCharacterIcon[i]->SetARGB(0xffaaaaaa);
		}


		m_pCharacterIcon[i]->Render(m_iIconX + ((i * m_iWidthIcon) % (MOVING_UP * m_iWidthIcon)), 482 + ((i / MOVING_UP)*ICON_HEIGHT));
	}

	// 中でステートマシンによる複雑な描画がある
	m_pLeftSide->Render();
	m_pRightSide->Render();

	//m_pPic[PIC_TYPE::INFO_BOX_LEFT]->Render(0, 354);
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT]->Render(899, 354);

	//m_pPic[PIC_TYPE::TITLE]->Render(0, 0);
	//m_pPic[PIC_TYPE::INFO_PLATE]->Render(0, 592 + 5);

}

// 立ち絵描画
void SelectUIManager::RenderCharacter()
{
	// 両方の立ち絵
	m_pLeftSide->RenderCharacter();
	m_pRightSide->RenderCharacter();

}

void SelectUIManager::FirstAction()
{
	//m_pPic[PIC_TYPE::TITLE]->Action();
	//m_pPic[PIC_TYPE::BLACK_LINE]->Action(6);
	//m_pPic[PIC_TYPE::INFO_PLATE]->Action(6);

	// キャラアイコン更新
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		m_pCharacterIcon[i]->Action(i * 8);
	}

	//m_pPic[PIC_TYPE::INFO_BOX_LEFT]->Action();
	//m_pPic[PIC_TYPE::INFO_BOX_RIGHT]->Action();

}
