#include "MovieManager.h"
#include "Data\PlayerData.h"
#include "Player\PlayerManager.h"
#include "UI\GameUI.h"
#include "BaseEntity\Message\MessageDispatcher.h"

//+--------------------------
//	ムービーの管理クラス
//+--------------------------

// 実体の宣言
//MovieManager* MovieManager::m_pInstance = nullptr;

MovieManager::MovieManager() :BaseGameEntity(ENTITY_ID::MOVIE_MGR)	// エンティティID登録
{
	m_eSelectMovie = MOVIE_TYPE::OP;

	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		switch ((MOVIE_TYPE)i)
		{
		case MOVIE_TYPE::OP:
			m_pMovie[i] = new OpMovie();
			break;
		case MOVIE_TYPE::OLD_OP:
			m_pMovie[i] = new OldOpMovie();
			break;
		case MOVIE_TYPE::STAFF_ROLL:
			m_pMovie[i] = new StaffRollMovie();
			break;
		default:
			m_pMovie[i] = new OpMovie();
			//MyAssert(0, "そのタイプは存在しない。");	
			break;
		}

	}

	// ロックアイコン
	m_pRockIcon = new tdn2DObj("Data/UI/Collect/Icon/Rock.png");

}

MovieManager::~MovieManager()
{
	// トロフィー全解放
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pMovie[i]);
	}

	SAFE_DELETE(m_pRockIcon);

}

//+---------------------------------
// 起動時
//+---------------------------------

// 選択時毎回初期化
void MovieManager::InitExe(MOVIE_TYPE eSelectType)
{
	// 
	m_eSelectMovie = eSelectType;

	// 選択しているイラスト
	m_pMovie[(int)m_eSelectMovie]->InitExe();
}

//+---------------------------------
// 選択した後の画像の　処理・描画
//+---------------------------------
bool MovieManager::UpdateExe()
{
	// 選択しているイラスト
	return m_pMovie[(int)m_eSelectMovie]->UpdateExe();

}

void MovieManager::CtrlExe(int iDevice)
{
	// 選択しているイラスト
	m_pMovie[(int)m_eSelectMovie]->CtrlExe(iDevice);
}

void MovieManager::RenderExe()
{
	// 選択しているイラスト
	m_pMovie[(int)m_eSelectMovie]->RenderExe();

#ifdef _DEBUG

#endif // _DEBUG

}

void MovieManager::RenderExeInfo(int iMovieType, int iX, int iY)
{
	// 解放後のイラストの説明
	m_pMovie[(int)iMovieType]->RenderExeInfo(iX, iY);

}

// 選択時毎回解放
void MovieManager::RereaseExe()
{
	// 選択しているイラスト
	m_pMovie[(int)m_eSelectMovie]->RereaseExe();
}

//+---------------------------------
// アイコンの描画
//+---------------------------------
void MovieManager::RenderIcon(int iMovieType, int iX, int iY)
{
	// ロック
	bool l_bRock = true;

	if (PlayerDataMgr->m_SecretData.iAllMovie[iMovieType] == 1)
	{
		// 選択しているイラスト
		m_pMovie[(int)iMovieType]->RenderIcon(iX, iY);
	}
	else
	{
		// 解除されてないかつ鍵つきなら
		if (m_pMovie[(int)iMovieType]->isRock())
		{
			// ロックイラスト
			m_pRockIcon->Render((int)m_pMovie[(int)iMovieType]->GetMovieIcon().vPos.x + iX,
				(int)m_pMovie[(int)iMovieType]->GetMovieIcon().vPos.y + iY);
		
		}else
		{
			// 選択しているイラストを未購入バージョンで描画
			m_pMovie[(int)iMovieType]->RenderIconNonPurchase(iX, iY);


		}


	}


}

//+---------------------------------
// アイコンの説明
//+---------------------------------
void MovieManager::RenderIconInfo(int iMovieType, int iX, int iY)
{
	// ロック が解除されているなら
	if (PlayerDataMgr->m_SecretData.iAllMovie[iMovieType] == 1)
	{
		// 普通に説明描画
		m_pMovie[(int)iMovieType]->RenderIconInfo(iX, iY);

	}
	else
	{
		// 解除されてないかつ鍵つきなら
		if (m_pMovie[(int)iMovieType]->isRock())
		{
			// 隠し用のメッセージ
			// タイトル
			tdnFont::RenderString("未開放コンテンツ", "HGｺﾞｼｯｸE",
				22, iX, iY, 0xffffffff, RS::COPY);
			// 説明
			tdnFont::RenderString("解放されていないコンテンツです。", "HGｺﾞｼｯｸE",
				19, iX, iY + 30, 0xffffffff, RS::COPY);
		}
		else
		{
			// 普通に説明描画
			m_pMovie[(int)iMovieType]->RenderIconInfo(iX, iY);
		}

	}


}

// 全イラストロック
void MovieManager::AllReset()
{
	// 
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		PlayerDataMgr->m_SecretData.iAllMovie[i] = 0;
	}

}

bool MovieManager::HandleMessage(const Message & msg)
{
	// 情報復元
	MOVIE_TYPE* l_eType = (MOVIE_TYPE*)msg.ExtraInfo;

	// IDを確認して送られてきたメッセージを仕訳
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::MOVIE_GET:

		if (PlayerDataMgr->m_SecretData.iAllMovie[(int)*l_eType] == 0)
		{
			PlayerDataMgr->m_SecretData.iAllMovie[(int)*l_eType] = 1;
			//m_eSelectMovie = *l_eType;
			//m_pMovie[(int)m_eSelectMovie]->Action(20);
			return true;
		}

		break;
	default:

		break;
	}

	return false;
}


int MovieManager::GetMovieOwned()
{
	int l_iCount = 0;
	//
	for (int i = 0; i < (int)MOVIE_TYPE::ARRAY_END; i++)
	{
		if (PlayerDataMgr->m_SecretData.iAllMovie[i] == 1)
		{
			l_iCount++;
		}

	}

	return l_iCount;
}
