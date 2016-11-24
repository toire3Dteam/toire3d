#include "CoinUI.h"
#include "Data\SelectData.h"
#include "Data\PlayerData.h"

CoinUI::CoinUI(Vector2 vPos)
{
	m_vPos = vPos;
	m_vNumPos = m_vPos;
	m_vNumPos.x += 58.0f + (18 * 6)/*数値分*/;
	m_vNumPos.y += 6.0f;

	m_pPlate = new tdn2DAnim("Data/UI/Menu/Coin/CoinPlate.png");
	m_pPlate->OrderMoveAppeared(16, (int)m_vPos.x - 256 , (int)m_vPos.y);
	
	m_pNumber = new tdn2DAnim("Data/UI/Menu/Coin/Number.png");
	m_pNumber->OrderMoveAppeared(16, (int)m_vNumPos.x - 256, (int)m_vNumPos.y);

	// 微調整
	//float abjScale = 1.25f;
	//m_pPlate->SetScale(abjScale);
	//m_pNumber->SetScale(abjScale);

	m_bRender = false;
}

CoinUI::~CoinUI()
{
	SAFE_DELETE(m_pPlate);
	SAFE_DELETE(m_pNumber);
}

void CoinUI::Update()
{
	if (m_bRender == false)return;

	m_pPlate->Update();
	m_pNumber->Update();
}

void CoinUI::Render()
{
	if (m_bRender == false)return;

	// プレート
	m_pPlate->Render((int)m_vPos.x, (int)m_vPos.y);

	// お金
	int number = PlayerDataMgr->m_PlayerInfo.coin;
	for (int count = 0; count < 6; ++count)
	{
		int digitNum = number % 10;	// 一番小さい桁を入手
		number = (int)(number / 10);// 数値の一番小さい桁を消す

		m_pNumber->Render((int)m_vNumPos.x - ((count)* 22), (int)m_vNumPos.y,
			32, 32, digitNum * 32, 0, 32, 32);

		//if (number <= 0)break;// 数値が０以下になったらさよなら
	}

}

void CoinUI::Action()
{
	m_bRender = true;
	m_pPlate->Action();
	m_pNumber->Action();
}

void CoinUI::Stop()
{
	m_bRender = false;
	m_pPlate->Stop();
	m_pNumber->Stop();
}
