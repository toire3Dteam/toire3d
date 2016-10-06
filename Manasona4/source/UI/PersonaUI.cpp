#include "PersonaUI.h"

//+-----------------------------
//		ペルソナ
//+-----------------------------

PersonaUI::PersonaUI(BasePlayer* pPlayer, bool bLeft)
{
	m_pPlayerReferences = pPlayer;
	m_bLeftSide = bLeft;

	m_pIcon = new tdn2DAnim("Data/UI/Game/PersonaIcon/PersonaIcon_0.png");
	//m_pIcon->OrderJump(8, 1, 0.1);

	if (m_bLeftSide == true)
	{
		m_vPos.x = 68;
		m_vPos.y = 83;
	}
	else
	{
		m_pIcon->SetTurnOver(true);

		m_vPos.x = 1084;
		m_vPos.y = 83;
	}

	m_pIcon->OrderMoveAppeared(8, (int)m_vPos.x, (int)m_vPos.y - 40);

	//******************************
	//	ペルソナカード

	// 最大ストックを取得
	m_iMaxCards = m_pPlayerReferences->GetStand()->GetStandStockMAX();

	// その分データを作成
	for (int i = 0; i < m_iMaxCards; i++)
	{
		PersonaCard data;

		data.obj = new tdn2DAnim("Data/UI/Game/PersonaCard0.png");
		data.fullCard = new tdn2DObj("Data/UI/Game/PersonaCard1.png");
		data.fullCardRip = new tdn2DAnim("Data/UI/Game/PersonaCard1.png");
		data.fullCardRip->OrderRipple(8, 1.0f, 0.1f);

		// 右と左で場所を変える
		if (bLeft)
		{
			data.pos.x = 390.0f + (i * -32);
		}
		else
		{
			data.pos.x = 826.0f + (i * 32);
		}		
		data.pos.y = 108;
		data.fullflag= false;

		// 動きを調整
		data.obj->OrderMoveAppeared(12, (int)data.pos.x, (int)data.pos.y - 36);

		// 追加
		m_sCards.push_back(data);
	}
	
	m_pFullCard = new tdn2DObj("Data/UI/Game/PersonaCard1.png");
	m_pFullCardRip = new tdn2DAnim("Data/UI/Game/PersonaCard1.png");
	m_pFullCardRip->OrderRipple(12, 1.0f, 0.2f);


}

PersonaUI::~PersonaUI()
{
	SAFE_DELETE(m_pIcon);

	for (int i = 0; i < (int)m_sCards.size(); i++)
	{
		SAFE_DELETE(m_sCards[i].obj);
		SAFE_DELETE(m_sCards[i].fullCard);
		SAFE_DELETE(m_sCards[i].fullCardRip);
	}
	m_sCards.clear();

	SAFE_DELETE(m_pFullCard);
	SAFE_DELETE(m_pFullCardRip);

}

void PersonaUI::Update()
{
	m_pIcon->Update();

	for (int i = 0; i < (int)m_sCards.size(); i++)
	{
		m_sCards[i].obj->Update();
		m_sCards[i].fullCardRip->Update();

	}
}

void PersonaUI::Render()
{
	m_pIcon->Render((int)m_vPos.x, (int)m_vPos.y);

	for (int i = 0; i < (int)m_sCards.size(); i++)
	{
		m_sCards[i].obj->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y);

		// 発動可能
		if (m_sCards[i].fullflag == true)
		{
			m_sCards[i].fullCard->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y);
		}
		m_sCards[i].fullCardRip->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y, RS::ADD);

		//if (m_pPlayerReferences->GetStand()->GetStandStock() > i)
		//{
		//	m_pFullCard->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y);
		//	m_pFullCardRip->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y, RS::ADD);
		//}
	}
}

void PersonaUI::Action(int waitFrame)
{
	m_pIcon->Action(waitFrame);

	for (int i = 0; i < (int)m_sCards.size(); i++)
	{
		m_sCards[i].obj->Action(waitFrame + (i * 8));
	}

}

// アップ
void PersonaUI::UpEffectAction()
{
	int no = m_pPlayerReferences->GetStand()->GetStandStock() - 1;
	if ((int)m_sCards.size() <= no)MyAssert(0, "最大サイズをこえてるんご");
	
	m_sCards[no].fullflag = true;
	m_sCards[no].fullCardRip->Action();

}

void PersonaUI::DownEffectAction()
{
	int no = m_pPlayerReferences->GetStand()->GetStandStock() - 1;
	if ((int)m_sCards.size() <= no)MyAssert(0, "最大サイズをこえてるんご");

	m_sCards[no].fullflag = false;
	m_sCards[no].fullCardRip->Action();

}