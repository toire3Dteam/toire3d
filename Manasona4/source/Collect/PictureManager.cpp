#include "PictureManager.h"
#include "Data\PlayerData.h"
#include "Player\PlayerManager.h"
#include "UI\GameUI.h"
#include "BaseEntity\Message\MessageDispatcher.h"

//+--------------------------
//	ピクチャーの管理クラス
//+--------------------------

// 実体の宣言
//PictureManager* PictureManager::m_pInstance = nullptr;

PictureManager::PictureManager() :BaseGameEntity(ENTITY_ID::PICTURE_MGR)	// エンティティID登録
{
	m_eSelectPicture = PICTURE_TYPE::AIROU;

	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		switch ((PICTURE_TYPE)i)
		{
		case PICTURE_TYPE::REST:
			m_pPicture[i] = new RestPicture();
			break;
		case PICTURE_TYPE::GATHERING:
			m_pPicture[i] = new GatheringPicture();
			break;
		case PICTURE_TYPE::STYLISH:
			m_pPicture[i] = new StylishPicture();
			break;
		case PICTURE_TYPE::COOL:
			m_pPicture[i] = new CoolPicture();
			break;
		case PICTURE_TYPE::ALLEY:
			m_pPicture[i] = new AlleyPicture();
			break;
		case PICTURE_TYPE::SAD:
			m_pPicture[i] = new SadPicture();
			break;
		case PICTURE_TYPE::SEXY:
			m_pPicture[i] = new SexyPicture();
			break;
		case PICTURE_TYPE::SEA:
			m_pPicture[i] = new SeaPicture();
			break;
		case PICTURE_TYPE::AIROU:
			m_pPicture[i] = new AirouPicture();
			break;
		case PICTURE_TYPE::TEKI:
			m_pPicture[i] = new TekiPicture();
			break;
		case PICTURE_TYPE::NAZENARABA:
			m_pPicture[i] = new NazenarabaPicture();
			break;
		case PICTURE_TYPE::ARAMITAMA:
			m_pPicture[i] = new AramitamaPicture();
			break;
		case PICTURE_TYPE::ANIKI:
			m_pPicture[i] = new AnikiPicture();
			break;
		case PICTURE_TYPE::MANATU:
			m_pPicture[i] = new Manatu1Picture();
			break;
		case PICTURE_TYPE::SUIKA:
			m_pPicture[i] = new SuikaPicture();
			break;
		case PICTURE_TYPE::MAHUYU:
			m_pPicture[i] = new MahuyuPicture();
			break;
		case PICTURE_TYPE::TOIRE:
			m_pPicture[i] = new ToirePicture();
			break;
		case PICTURE_TYPE::MANATU2:
			m_pPicture[i] = new Manatu2Picture();
			break;
		default:
			m_pPicture[i] = new AirouPicture();
			//MyAssert(0, "そのタイプは存在しない。");	
			break;
		}

	}

	// ロックアイコン
	m_pRockIcon = new tdn2DObj("Data/UI/Collect/Icon/Rock.png");

}

PictureManager::~PictureManager()
{
	// トロフィー全解放
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pPicture[i]);
	}

	SAFE_DELETE(m_pRockIcon);

}

//+---------------------------------
// 起動時
//+---------------------------------

// 選択時毎回初期化
void PictureManager::InitExe(PICTURE_TYPE eSelectType)
{
	// 
	m_eSelectPicture = eSelectType;

	// 選択しているイラスト
	m_pPicture[(int)m_eSelectPicture]->InitExe();
}

//+---------------------------------
// 選択した後の画像の　処理・描画
//+---------------------------------
void PictureManager::UpdateExe()
{
	// 選択しているイラスト
	m_pPicture[(int)m_eSelectPicture]->UpdateExe();

}

void PictureManager::CtrlExe(int iDevice)
{
	// 選択しているイラスト
	m_pPicture[(int)m_eSelectPicture]->CtrlExe(iDevice);
}

void PictureManager::RenderExe()
{
	// 選択しているイラスト
	m_pPicture[(int)m_eSelectPicture]->RenderExe();

#ifdef _DEBUG

#endif // _DEBUG

}

void PictureManager::RenderExeInfo(int iPictureType, int iX, int iY)
{
	// 解放後のイラストの説明
	m_pPicture[(int)iPictureType]->RenderExeInfo(iX, iY);

}

// 選択時毎回解放
void PictureManager::RereaseExe()
{
	// 選択しているイラスト
	m_pPicture[(int)m_eSelectPicture]->RereaseExe();
}

//+---------------------------------
// アイコンの描画
//+---------------------------------
void PictureManager::RenderIcon(int iPictureType, int iX, int iY)
{
	// ロック
	bool l_bRock = true;

	if (PlayerDataMgr->m_SecretData.iAllPic[iPictureType] == 1)
	{
		// 選択しているイラスト
		m_pPicture[(int)iPictureType]->RenderIcon(iX, iY);
	}
	else
	{
		// 解除されてないかつ鍵つきなら
		if (m_pPicture[(int)iPictureType]->isRock())
		{
			// ロックイラスト
			m_pRockIcon->Render((int)m_pPicture[(int)iPictureType]->GetPictureIcon().vPos.x + iX,
				(int)m_pPicture[(int)iPictureType]->GetPictureIcon().vPos.y + iY);
		
		}else
		{
			// 選択しているイラストを未購入バージョンで描画
			m_pPicture[(int)iPictureType]->RenderIconNonPurchase(iX, iY);


		}


	}


}

//+---------------------------------
// アイコンの説明
//+---------------------------------
void PictureManager::RenderIconInfo(int iPictureType, int iX, int iY)
{
	// ロック が解除されているなら
	if (PlayerDataMgr->m_SecretData.iAllPic[iPictureType] == 1)
	{
		// 普通に説明描画
		m_pPicture[(int)iPictureType]->RenderIconInfo(iX, iY);

	}
	else
	{
		// 解除されてないかつ鍵つきなら
		if (m_pPicture[(int)iPictureType]->isRock())
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
			m_pPicture[(int)iPictureType]->RenderIconInfo(iX, iY);
		}

	}


}

// 全イラストロック
void PictureManager::AllReset()
{
	// 
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		PlayerDataMgr->m_SecretData.iAllPic[i] = 0;
	}

}

bool PictureManager::HandleMessage(const Message & msg)
{
	// 情報復元
	PICTURE_TYPE* l_eType = (PICTURE_TYPE*)msg.ExtraInfo;

	// IDを確認して送られてきたメッセージを仕訳
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::PICTURE_GET:

		if (PlayerDataMgr->m_SecretData.iAllPic[(int)*l_eType] == 0)
		{
			PlayerDataMgr->m_SecretData.iAllPic[(int)*l_eType] = 1;
			//m_eSelectPicture = *l_eType;
			//m_pPicture[(int)m_eSelectPicture]->Action(20);
			return true;
		}

		break;
	default:

		break;
	}

	return false;
}


int PictureManager::GetPictureOwned()
{
	int l_iCount = 0;
	//
	for (int i = 0; i < (int)PICTURE_TYPE::ARRAY_END; i++)
	{
		if (PlayerDataMgr->m_SecretData.iAllPic[i] == 1)
		{
			l_iCount++;
		}

	}

	return l_iCount;
}
