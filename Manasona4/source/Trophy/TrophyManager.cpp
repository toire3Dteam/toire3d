#include "TrophyManager.h"
#include "Data\PlayerData.h"

//+--------------------------
//	トロフィーの管理クラス
//+--------------------------

// 実体の宣言
TrophyManager* TrophyManager::m_pInstance = nullptr;

TrophyManager::TrophyManager() :BaseGameEntity(ENTITY_ID::TROPHY_MGR)	// エンティティID登録
{
	m_eSelectTrophy = TROPHY_TYPE::FIRST;

	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		switch ((TROPHY_TYPE)i)
		{
		case TROPHY_TYPE::FIRST:
			m_pTrophy[i] = new FirstTrophy();

			break;
		case TROPHY_TYPE::FIRST_BATTLE:
			m_pTrophy[i] = new FirstBattleTrophy();

			break;
		default:
			m_pTrophy[i] = new FirstTrophy();
			//MyAssert(0, "そのタイプは存在しない。");	
			break;
		}

	}
}

TrophyManager::~TrophyManager()
{
	// トロフィー全解放
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTrophy[i]);
	}

}

void TrophyManager::Update()
{
	// 選択しているトロフィー
	m_pTrophy[(int)m_eSelectTrophy]->Update();

	// 全トロフィー更新
	//for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	//{
	//	m_pTrophy[i]->Update();
	//}

}

void TrophyManager::Render()
{
	// 選択しているトロフィー
	m_pTrophy[(int)m_eSelectTrophy]->Render();

	// 全トロフィー描画
	//for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	//{
	//	m_pTrophy[i]->Render();
	//}

#ifdef _DEBUG

	tdnText::Draw(10, 690, 0xffffffff, "1番目のトロフィー->%d", PlayerDataMgr->m_TrophyData.iFirstPlay);	
	tdnText::Draw(300, 690, 0xffffffff, "2番目のトロフィー->%d", PlayerDataMgr->m_TrophyData.iFirstBattle);

#endif // _DEBUG

}

bool TrophyManager::HandleMessage(const Message & msg)
{
	// 情報復元
	TROPHY_TYPE* l_eType = (TROPHY_TYPE*)msg.ExtraInfo;

	// IDを確認して送られてきたメッセージを仕訳
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TROPHY_GET:
		

		switch (*l_eType)
		{
		case TROPHY_TYPE::FIRST:
			if (PlayerDataMgr->m_TrophyData.iFirstPlay == 0) 
			{
				PlayerDataMgr->m_TrophyData.iFirstPlay = 1;
				m_eSelectTrophy = TROPHY_TYPE::FIRST;
				m_pTrophy[(int)m_eSelectTrophy]->Action(30);
				return true;
			}
			break;
		case TROPHY_TYPE::FIRST_BATTLE:
			if (PlayerDataMgr->m_TrophyData.iFirstBattle== 0)
			{
				PlayerDataMgr->m_TrophyData.iFirstBattle = 1;
				m_eSelectTrophy = TROPHY_TYPE::FIRST_BATTLE;
				m_pTrophy[(int)m_eSelectTrophy]->Action(30);
				return true;
			}
			break;
		default:
			break;
		}


		break;
	default:

		break;
	}

	return false;
}
