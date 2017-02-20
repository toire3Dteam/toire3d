#include "TrophyManager.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
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
		case TROPHY_TYPE::POWERFUL_ENEMY:
			m_pTrophy[i] = new PowerfulEnemyWinTrophy();

			break;
		case TROPHY_TYPE::SS_RANK_WIN:
			m_pTrophy[i] = new SSRankWinTrophy();

			break;
		case TROPHY_TYPE::SS_POWERFUL_ENEMY:
			m_pTrophy[i] = new PowerfulEnemySSRankWinTrophy();

			break;
		case TROPHY_TYPE::MENY_CONTENT:
			m_pTrophy[i] = new BuyManyContentTrophy();

			break;
		case TROPHY_TYPE::CHALLENGE_CLEAR:
			m_pTrophy[i] = new ChallengeTrophy();

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


	// 連続取得用
	m_aActionStack.clear();
	m_aActionStack.reserve(8);
	m_iIntervalFrame = 0;

	m_iTrainingFrame = 0;
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
	// 連続取得用
	m_iIntervalFrame--;
	if (m_iIntervalFrame <= 0)
	{
		m_iIntervalFrame = 0;	// 0で止める

		for (int i = 0; i < (int)m_aActionStack.size(); i++)
		{
			// セレクトトロフィーを設定 最初の入ってある数値を入れる
			m_eSelectTrophy = m_aActionStack[i];

			m_pTrophy[(int)m_eSelectTrophy]->Action(20);

			// 解放したら取り出す
			//m_aActionStack.pop_back();
			m_aActionStack.erase(m_aActionStack.begin());

			// 間隔フレーム設定(トロフィーのアイコンが出る感覚はここで調整)
			m_iIntervalFrame = 240;
			break;
		}

	}


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
			//m_eSelectTrophy = *l_eType;
			//m_pTrophy[(int)m_eSelectTrophy]->Action(20);
			
			// ↑でアクションせず一時的に解放スタックに番号を入れる
			m_aActionStack.push_back(*l_eType);
			
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

// プレイカウント
void TrophyManager::CheakPlayCount(int iPlayCount)
{
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iFirstPlay == 0)
	{
		// ↓以上プレイしていたら
		if (iPlayCount >= 1)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::FIRST;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
		}

	}

}

// 対戦回数
void TrophyManager::CheakBattleCount(int iBattleCount)
{
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iFirstBattle == 0)
	{
		// ↓以上対戦していたら
		if (iBattleCount >= 1)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::FIRST_BATTLE;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
		}

	}

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
		if (iElapsedTime <= 20)
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


void TrophyManager::CheakPowerfulEnemyWin(SIDE eWinnerSide)
{
	// 負けた側のサイドを知る
	SIDE l_eLoseSide = SIDE::LEFT;
	if (eWinnerSide == SIDE::LEFT)
	{
		l_eLoseSide = SIDE::RIGHT;
	}
	
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iPowerfulEnemyWin == 0)
	{
		// ★負けた側がAIで尚且つ最高難易度だったら
		if (SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].bAI == true && 
			SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].eAIType == (AI_TYPE::CPU_YOKOE))
		{

			TROPHY_TYPE eType = TROPHY_TYPE::POWERFUL_ENEMY;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}
}

void TrophyManager::CheakRank(SIDE eWinnerSide, RANK_TYPE eRank)
{
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iSSRankWin == 0)
	{
		// ★勝った側はプレイヤーで尚且つSSランクなら
		if (SelectDataMgr->Get()->tagSideDatas[(int)eWinnerSide].bAI == false &&
			eRank == RANK_TYPE::SS)
		{

			TROPHY_TYPE eType = TROPHY_TYPE::SS_RANK_WIN;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}


	// 負けた側のサイドを知る
	SIDE l_eLoseSide = SIDE::LEFT;
	if (eWinnerSide == SIDE::LEFT)
	{
		l_eLoseSide = SIDE::RIGHT;
	}

	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iSSRankPowerfulEnemyWin == 0)
	{
		// ★負けた側がAIで尚且つ最高難易度だったら
		if (SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].bAI == true &&
			SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].eAIType == (AI_TYPE::CPU_YOKOE))
		{

			// ★勝った側はプレイヤーで尚且つSSランクなら
			if (SelectDataMgr->Get()->tagSideDatas[(int)eWinnerSide].bAI == false &&
				eRank == RANK_TYPE::SS)
			{

				TROPHY_TYPE eType = TROPHY_TYPE::SS_POWERFUL_ENEMY;
				MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

			}
		}

	}
}

void TrophyManager::CheakBuyManyContent(int iOwnedContent)
{
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iBuyManyContent == 0)
	{
		// ↓以上所持していたら
		if (iOwnedContent >= 10)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::MENY_CONTENT;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}


}


void TrophyManager::CheakChallengeClear()
{
	// まだ↓のトロフィーを手に入れていなかったら
	if (PlayerDataMgr->m_TrophyData.iChallengeClear == 0)
	{
		TROPHY_TYPE eType = TROPHY_TYPE::CHALLENGE_CLEAR;
		MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

	}

}



