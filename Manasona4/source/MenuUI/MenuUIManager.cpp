#include "MenuUIManager.h"

//+--------------------------
//	メニューUIマネージャー
//+--------------------------

MenuUIManager::MenuUIManager(int iSelectNo)
{
	// セレクトナンバー
	m_iSelectNo = iSelectNo;

	//　アイコンの幅
	m_fIconWidth = 256;
	m_fIconHeight = -48;


	int xSize = 0;

	// UIアイコン
	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		switch (MENU_ICON_TYPE(i))
		{
		case MENU_ICON_TYPE::TUTORIAL:
			m_pIcon[i] = new TutorialIcon();
			break;
		case MENU_ICON_TYPE::BATTLE:
			m_pIcon[i] = new BattleIcon();
			break;
		case MENU_ICON_TYPE::TRAINING:
			m_pIcon[i] = new TrainingIcon();
			break;
		case MENU_ICON_TYPE::COLLECT:
			m_pIcon[i] = new CollectIcon();
			break;
		case MENU_ICON_TYPE::OPTION:
			m_pIcon[i] = new OptionIcon();
			break;
		case MENU_ICON_TYPE::ARRAY_END:
			
			break;
		default:
			break;
		}

		int rate = 0;
		int heght = 0;
		if (i == (m_iSelectNo))// 選択してる絵
		{
			rate = 64;
			heght = 32;
		}
		if (i == (m_iSelectNo + 1))rate = 64;// 選択してる絵のそのあとのアイコン			
		xSize += rate;// ＋幅


		// ポジションの初期位置設定
		float ScroolX = (m_iSelectNo*-m_fIconWidth);
		m_pIcon[i]->SetPos(Vector2(48 + (i * m_fIconWidth) + ScroolX + xSize,
			m_fIconHeight + heght));

		// ポジション
		//m_pIcon[i]->SetPos(Vector2((m_fIconWidth*i)+128, m_fIconHeight));
		
		// スケール
		m_pIcon[i]->SetScale(0.5f);
	}


	
	// 青い線
	m_pblueLine = new tdn2DObj("Data/UI/Menu/BlueLine.png");
	m_iBlueLineY = 0;
	m_iNextBlueLineY = 0;

	m_pBlackLine = new tdn2DAnim("Data/UI/Menu/BlackLine.png");
	m_pBlackLine->OrderMoveAppeared(32, -1280, 0);

	//
	m_pInfo = new tdn2DAnim("Data/UI/Menu/Information.png");
	m_iInfoY = 592;
	m_pInfo->OrderMoveAppeared(16, 0, m_iInfoY + 128);
	
	// お金
	m_pCoinUI = new CoinUI(Vector2(15.0f, 540.0f));

	// 仮
	pExp = new tdn2DObj("Data/UI/Menu/exp.png");

	// 動画
	m_pMoveMgr = new tdnMovie("DATA/UI/Menu/Menu2.wmv");
	m_pMove = new tdn2DObj(m_pMoveMgr->GetTexture());
	m_pMoveMgr->Play();

}

MenuUIManager::~MenuUIManager()
{
	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pIcon[i]);
	}

	SAFE_DELETE(m_pCoinUI);

	SAFE_DELETE(m_pblueLine);
	SAFE_DELETE(m_pBlackLine);
	SAFE_DELETE(m_pInfo);

	SAFE_DELETE(pExp);

	SAFE_DELETE(m_pMoveMgr);
	SAFE_DELETE(m_pMove);
}

// 更新
void MenuUIManager::Update()
{
	// 動画のループ更新
	m_pMoveMgr->LoopUpdate();

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		m_pIcon[i]->Update();
	}

	m_pInfo->Update();

	m_pBlackLine->Update();

	m_pCoinUI->Update();

	// 動く処理
	Move();
}

// 動く処理
void MenuUIManager::Move()
{
	// ラインの動き
	m_iBlueLineY = (int)(m_iBlueLineY*0.7f + m_iNextBlueLineY*0.3f);


}

// 描画
void MenuUIManager::Render()
{
	// 仮
	pExp->Render(0, 0);
	m_pMove->Render(0, 0);

	m_pBlackLine->Render(0, 0);

	// 青い線
	m_pblueLine->Render(0, m_iBlueLineY);

	// 説明の淵
	m_pInfo->Render(0, m_iInfoY);

	// お金
	m_pCoinUI->Render();

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		//if (m_iSelectNo == i)
		//{
			m_pIcon[i]->Render(m_iBlueLineY);
		//}
		//else
		//{
		//	m_pIcon[i]->RenderGray();
		//}
	
	}

	tdnText::Draw(100, 100, 0xffffffff, "%dセレクト番号", m_iSelectNo);
}

// [初回]アクション
void MenuUIManager::Action()
{
	int iWaitFrame = 0;

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		if ( i >= m_iSelectNo)
		{
			iWaitFrame += 6;
		}		

		m_pIcon[i]->Action(iWaitFrame);


		// 選択してるアイコンのみ
		if (i == m_iSelectNo)
		{
			m_pIcon[i]->GetInfo()->Action(14);
		}
	}

	m_pInfo->Action();
	m_pBlackLine->Action();
	m_pCoinUI->Action();
}

// 下るアクション[選択肢2へ]
void MenuUIManager::ActionFall()
{
	// ライン下げます
	m_iNextBlueLineY = -500;


}

void MenuUIManager::ActionUp()
{
	// ライン上げます
	m_iNextBlueLineY = 0;

}

// 説明文を全て消す
void MenuUIManager::InfoStop()
{
	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		m_pIcon[i]->GetInfo()->Stop();
	}
}

void MenuUIManager::InfoAction()
{
	m_pIcon[m_iSelectNo]->GetInfo()->Action();
}

void MenuUIManager::ChangeSelectNo(int setSelectNo)
{
	m_iSelectNo = setSelectNo;

	int xSize = 0;

	for (int i = 0; i < (int)MENU_ICON_TYPE::ARRAY_END; i++)
	{
		int rate = 0;
		int heght = 0;
		if (i == (m_iSelectNo))// 選択してる絵
		{
			rate = 64;
			heght = 32;
		}
		if (i == (m_iSelectNo+1))rate = 64;// 選択してる絵のそのあとのアイコン			
		xSize += rate;// ＋幅
		
		// 次のポジションを設定
		float ScroolX = (setSelectNo*-m_fIconWidth);
		m_pIcon[i]->SetNextPos(Vector2(48 + (i * m_fIconWidth)+ ScroolX + xSize,
			m_fIconHeight+ heght));

		// 選択してるアイコンの処理
		if (setSelectNo == i)
		{
			m_pIcon[i]->SetSelectFlag(true);
			m_pIcon[i]->SetNextScale(1.0f);
			m_pIcon[i]->GetInfo()->Action();
		}
		else
		{
			m_pIcon[i]->SetSelectFlag(false);
			m_pIcon[i]->SetNextScale(0.5f);
			m_pIcon[i]->GetInfo()->Stop();
		}

	}	

}
