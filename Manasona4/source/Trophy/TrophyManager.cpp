#include "TrophyManager.h"
#include "Data\PlayerData.h"
#include "Player\PlayerManager.h"
#include "UI\GameUI.h"
#include "BaseEntity\Message\MessageDispatcher.h"

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
		case TROPHY_TYPE::BIG_DAMAGE:
			m_pTrophy[i] = new BigDamageTrophy();

			break;
		case TROPHY_TYPE::MAX_HP_FINISH:
			m_pTrophy[i] = new MaxHpFinishTrophy();

			break;
		case TROPHY_TYPE::SPEED_FINISH:
			m_pTrophy[i] = new SpeedFinishTrophy();

			break;
		case TROPHY_TYPE::TRAINING_TIME:
			m_pTrophy[i] = new TrainingTimeTrophy();

			break;
		case TROPHY_TYPE::COMPLETE_TROPHY:
			m_pTrophy[i] = new CompleteTrophy();

			break;
		default:
			m_pTrophy[i] = new FirstTrophy();
			//MyAssert(0, "そのタイプは存在しない。");	
			break;
		}

	}

	// ロックアイコン
	m_pRockIcon = new tdn2DObj("Data/Trophy/Rock.png");

}

TrophyManager::~TrophyManager()
{
	// トロフィー全解放
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTrophy[i]);
	}

	SAFE_DELETE(m_pRockIcon);

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

	// ★トロフィーを全て集めたか確認処理
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iCompleteTrophy== 0)
	{
		int l_iCount = 0;
		for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
		{
			// コンプリートトロフィーは飛ばす
			if (PlayerDataMgr->m_TrophyData.iAllData[i]
				== (int)TROPHY_TYPE::COMPLETE_TROPHY)return;

			// 手に入れてたらカウント
			if (PlayerDataMgr->m_TrophyData.iAllData[i] == 1)
			{
				l_iCount++;
			}	

		}

		// 全部手に入れたら
		if (l_iCount == (int)TROPHY_TYPE::ARRAY_END - 1)
		{
			// コンプリートおめでとう
			TROPHY_TYPE eType = TROPHY_TYPE::COMPLETE_TROPHY;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
		}
	}

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

void TrophyManager::RenderRoom(int iTrophyType, int iX, int iY)
{
	// ロック
	bool l_bRock = true;

	//switch ((TROPHY_TYPE)iTrophyType)
	//{
	//case TROPHY_TYPE::FIRST:
	//	if (PlayerDataMgr->m_TrophyData.iFirstPlay == 1)
	//	{
	//		l_bRock = false;
	//	}
	//
	//	break;
	//case TROPHY_TYPE::FIRST_BATTLE:
	//	if (PlayerDataMgr->m_TrophyData.iFirstBattle == 1)
	//	{
	//		l_bRock = false;
	//	}
	//	break;
	//default:
	//	//MyAssert(0, "そのタイプないです");
	//	break;
	//}

	if (PlayerDataMgr->m_TrophyData.iAllData[iTrophyType] == 1)
	{
		// 選択しているトロフィー
		m_pTrophy[(int)iTrophyType]->RenderRoom(iX, iY);
	}
	else
	{
		// ロックイラスト
		m_pRockIcon->SetScale(0.5f);
		m_pRockIcon->Render((int)m_pTrophy[(int)iTrophyType]->GetRoomPos().x + iX,
			(int)m_pTrophy[(int)iTrophyType]->GetRoomPos().y + iY);
	}


}

void TrophyManager::RenderInfo(int iTrophyType, int iX, int iY)
{
	// ロック が解除されているなら
	if (PlayerDataMgr->m_TrophyData.iAllData[iTrophyType] == 1)
	{
		// 普通に説明描画
		m_pTrophy[(int)iTrophyType]->RenderInfo(iX, iY);

	}else
	{
		// 解除されてないかつ隠しトロフィーなら
		if (m_pTrophy[(int)iTrophyType]->isHide())
		{
			// 隠し用のメッセージ
			// タイトル
			tdnFont::RenderString("隠しトロフィー", "HGｺﾞｼｯｸE",
				22, iX, iY, 0xffffffff, RS::COPY);
			// 説明
			tdnFont::RenderString("解禁情報が隠されているトロフィーです。", "HGｺﾞｼｯｸE",
				19, iX, iY + 30, 0xffffffff, RS::COPY);
		}
		else
		{
			// 普通に説明描画
			m_pTrophy[(int)iTrophyType]->RenderInfo(iX, iY);
		}


	}


}

// 演出を切る
void TrophyManager::Stop()
{
	// 選択しているトロフィー
	m_pTrophy[(int)m_eSelectTrophy]->Stop();

}

// 全トロフィー解禁
void TrophyManager::AllReset()
{
	// 全トロフィーロック
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		PlayerDataMgr->m_TrophyData.iAllData[i] = 0;
	}

}

bool TrophyManager::HandleMessage(const Message & msg)
{
	// 情報復元
	TROPHY_TYPE* l_eType = (TROPHY_TYPE*)msg.ExtraInfo;

	// IDを確認して送られてきたメッセージを仕訳
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TROPHY_GET:
		

		//switch (*l_eType)
		//{
		//case TROPHY_TYPE::FIRST:
		//	if (PlayerDataMgr->m_TrophyData.iFirstPlay == 0) 
		//	{
		//		PlayerDataMgr->m_TrophyData.iFirstPlay = 1;
		//		m_eSelectTrophy = TROPHY_TYPE::FIRST;
		//		m_pTrophy[(int)m_eSelectTrophy]->Action(30);
		//		return true;
		//	}
		//	break;
		//case TROPHY_TYPE::FIRST_BATTLE:
		//	if (PlayerDataMgr->m_TrophyData.iFirstBattle== 0)
		//	{
		//		PlayerDataMgr->m_TrophyData.iFirstBattle = 1;
		//		m_eSelectTrophy = TROPHY_TYPE::FIRST_BATTLE;
		//		m_pTrophy[(int)m_eSelectTrophy]->Action(30);
		//		return true;
		//	}
		//	break;
		//default:
		//	break;
		//}


		if (PlayerDataMgr->m_TrophyData.iAllData[(int)*l_eType] == 0)
		{
			PlayerDataMgr->m_TrophyData.iAllData[(int)*l_eType] = 1;
			m_eSelectTrophy = *l_eType;
			m_pTrophy[(int)m_eSelectTrophy]->Action(20);
			return true;
		}


		break;
	default:

		break;
	}

	return false;
}

int TrophyManager::GetTrophyOwned()
{
	int l_iCount = 0;
	//
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		if (PlayerDataMgr->m_TrophyData.iAllData[i] == 1)
		{
			l_iCount++;
		}

	}


	return l_iCount;
}

void TrophyManager::InitSeceneMain()
{
	// トレーニング滞在時間
	m_iTrainingFrame = 0;
}

void TrophyManager::CheakBigDamage(bool bVS)
{

	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iBigDamage == 0)
	{

		if (PlayerMgr->GetPlayer(SIDE::LEFT)->isAI() == false)
		{
			// 最大ダメージ5000以上
			if (GameUIMgr->GetComboUI(SIDE::RIGHT)->GetMaxDamage() >= 5000)
			{
				TROPHY_TYPE eType = TROPHY_TYPE::BIG_DAMAGE;
				MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

			}

		}
		else if (PlayerMgr->GetPlayer(SIDE::RIGHT)->isAI() == false)
		{
			// 最大ダメージ5000以上
			if (GameUIMgr->GetComboUI(SIDE::LEFT)->GetMaxDamage() >= 5000)
			{
				TROPHY_TYPE eType = TROPHY_TYPE::BIG_DAMAGE;
				MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

			}
		}

	}

	

}

void TrophyManager::CheakMaxHpFinish(int iRemainingHP)
{
	// 対戦じゃなかったら返す
	//if (bVS == false)return;

	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iMaxHpFinish == 0)
	{
		// 100%勝利なら
		if (iRemainingHP >= 100)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::MAX_HP_FINISH;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}


}

void TrophyManager::CheakSpeedFinish(int iElapsedTime)
{
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iSpeedFinish == 0)
	{
		// ↓の数値よりも早く勝利したなら
		if (iElapsedTime <= 15)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::SPEED_FINISH;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}

}

void TrophyManager::CheakTrainingTime()
{
	m_iTrainingFrame++;

	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iTrainingTime == 0)
	{
		// ↓時間以上プレイしてたら
		if (m_iTrainingFrame >= (60 * 60) * 5)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::TRAINING_TIME;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}
}



