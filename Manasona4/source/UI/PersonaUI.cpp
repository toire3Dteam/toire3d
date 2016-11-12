#include "PersonaUI.h"

//+-----------------------------
//		ペルソナ
//+-----------------------------

#define MyPersona m_pPlayerReferences->GetStand()

PersonaUI::PersonaUI(BasePlayer* pPlayer)
{
	m_pPlayerReferences = pPlayer;

	if (pPlayer->GetSide()==SIDE::LEFT)
	{
		m_bLeftSide = true;
	}
	else
	{
		m_bLeftSide = false;
	}

	//
	//MyPersona->GetIcon();
	//m_pIcon->OrderJump(8, 1, 0.1);

	if (m_bLeftSide == true)
	{
		m_vPos.x = 30;
		m_vPos.y = 110;

		m_vTriggerPos.x = m_vPos.x + 86;
		m_vTriggerPos.y = m_vPos.y + 42;

	}
	else
	{
		MyPersona->GetIcon()->SetTurnOver(true);

		m_vPos.x = 1280 - 30 - 128;
		m_vPos.y = 110;

		m_vTriggerPos.x = m_vPos.x - 24;
		m_vTriggerPos.y = m_vPos.y + 42;
	}

	MyPersona->GetIcon()->OrderMoveAppeared(8, (int)m_vPos.x, (int)m_vPos.y - 40);
	MyPersona->GetIconRip()->OrderRipple(12, 1.0f, 0.025f);

	m_bTriggerFlag = false;

	//
	m_pTrigger = new tdn2DAnim("Data/UI/Game/PersonaIcon/Trigger.png");
	//m_pTriggerGage= new tdn2DAnim("Data/UI/Game/PersonaIcon/TriggerGage.png");
	m_pTriggerGage= new Pie_graph("Data/UI/Game/PersonaIcon/TriggerGage.png");
	m_pTrigger->OrderMoveAppeared(8, (int)m_vTriggerPos.x, (int)m_vTriggerPos.y - 40);

	m_pTriggerGageRip = new tdn2DAnim("Data/UI/Game/PersonaIcon/TriggerGage.png");
	m_pTriggerGageRip->OrderRipple(12, 1.0f, 0.05f);

	// フラッシュ用
	m_iFlashRate = 0;
	m_bFlashFlag = false;

	//m_pTriggerGage->OrderMoveAppeared(8, (int)m_vTriggerPos.x, (int)m_vTriggerPos.y - 40);

	//******************************
	//	ペルソナカード

	//// 最大ストックを取得
	//m_iMaxCards = m_pPlayerReferences->GetStand()->GetStandStockMAX();

	//// その分データを作成
	//for (int i = 0; i < m_iMaxCards; i++)
	//{
	//	PersonaCard data;

	//	data.obj = new tdn2DAnim("Data/UI/Game/PersonaCard0.png");
	//	data.fullCard = new tdn2DObj("Data/UI/Game/PersonaCard1.png");
	//	data.fullCardRip = new tdn2DAnim("Data/UI/Game/PersonaCard1.png");
	//	data.fullCardRip->OrderRipple(8, 1.0f, 0.1f);

	//	// 右と左で場所を変える
	//	if (m_bLeftSide)
	//	{
	//		data.pos.x = 390.0f + (i * -32);
	//	}
	//	else
	//	{
	//		data.pos.x = 826.0f + (i * 32);
	//	}		
	//	data.pos.y = 108;
	//	data.fullflag= false;

	//	// 動きを調整
	//	data.obj->OrderMoveAppeared(12, (int)data.pos.x, (int)data.pos.y - 36);

	//	// 追加
	//	m_sCards.push_back(data);
	//}
	
	//m_pFullCard = new tdn2DObj("Data/UI/Game/PersonaCard1.png");
	//m_pFullCardRip = new tdn2DAnim("Data/UI/Game/PersonaCard1.png");
	//m_pFullCardRip->OrderRipple(12, 1.0f, 0.2f);


}

PersonaUI::~PersonaUI()
{
	//SAFE_DELETE(m_pIcon);

	SAFE_DELETE(m_pTrigger);
	SAFE_DELETE(m_pTriggerGage);
	SAFE_DELETE(m_pTriggerGageRip);

	//for (int i = 0; i < (int)m_sCards.size(); i++)
	//{
	//	SAFE_DELETE(m_sCards[i].obj);
	//	SAFE_DELETE(m_sCards[i].fullCard);
	//	SAFE_DELETE(m_sCards[i].fullCardRip);
	//}
	//m_sCards.clear();

	//SAFE_DELETE(m_pFullCard);
	//SAFE_DELETE(m_pFullCardRip);

}

void PersonaUI::Update()
{
	MyPersona->GetIcon()->Update();
	MyPersona->GetIconRip()->Update();
	
	m_pTrigger->Update();
	m_pTriggerGageRip->Update();

	// 回転式ゲージの更新
	if (m_bTriggerFlag == true )
	{
		m_pTriggerGage->Set_percent(1.0f);

		// フラッシュ処理
		if (m_bFlashFlag == true)
		{
			
			if (m_iFlashRate >= 64)
			{
				m_bFlashFlag = false;
			}

			m_iFlashRate += 2;
		}
		else
		{
			if (m_iFlashRate <= 0)
			{
				m_bFlashFlag = true;
			}

			m_iFlashRate -= 2;
		}

		// クランプ
		m_iFlashRate = (int)Math::Clamp((float)m_iFlashRate, 0, 255.0f);


	}else
	{
		m_pTriggerGage->Set_percent((float)MyPersona->GetStandGage() / (float)MyPersona->GetStandGageMAX());
	}

	//m_pTriggerGage->Update();

	//for (int i = 0; i < (int)m_sCards.size(); i++)
	//{
	//	m_sCards[i].obj->Update();
	//	m_sCards[i].fullCardRip->Update();

	//}
}

void PersonaUI::Render()
{
	MyPersona->GetIcon()->Render((int)m_vPos.x, (int)m_vPos.y, 128, 84, m_bTriggerFlag * 128 , 0, 128, 84);

	// 白点滅
	if (m_bTriggerFlag)
	{
		MyPersona->GetIcon()->SetAlpha(m_iFlashRate);
		MyPersona->GetIcon()->Render((int)m_vPos.x, (int)m_vPos.y, 128, 84, m_bTriggerFlag * 128, 0, 128, 84, RS::ADD);
		MyPersona->GetIcon()->SetAlpha(255);
	}

	MyPersona->GetIconRip()->Render((int)m_vPos.x, (int)m_vPos.y, 128, 84, m_bTriggerFlag * 128 , 0, 128, 84, RS::ADD);
	
	m_pTrigger->Render((int)m_vTriggerPos.x, (int)m_vTriggerPos.y);

	// 一回転式のゲージ
	m_pTriggerGage->Render((int)m_vTriggerPos.x, (int)m_vTriggerPos.y, 64, 64, 0, 0, 64, 64);
	m_pTriggerGageRip->Render((int)m_vTriggerPos.x, (int)m_vTriggerPos.y, RS::ADD);

	//for (int i = 0; i < (int)m_sCards.size(); i++)
	//{
	//	m_sCards[i].obj->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y);

	//	// 発動可能
	//	if (m_sCards[i].fullflag == true)
	//	{
	//		m_sCards[i].fullCard->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y);
	//	}
	//	m_sCards[i].fullCardRip->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y, RS::ADD);

	//	//if (m_pPlayerReferences->GetStand()->GetStandStock() > i)
	//	//{
	//	//	m_pFullCard->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y);
	//	//	m_pFullCardRip->Render((int)m_sCards[i].pos.x, (int)m_sCards[i].pos.y, RS::ADD);
	//	//}
	//}
}

void PersonaUI::Action(int waitFrame)
{
	MyPersona->GetIcon()->Action(waitFrame);
	
	m_pTrigger->Action(waitFrame);
	//m_pTriggerGage->Action(waitFrame);
	//for (int i = 0; i < (int)m_sCards.size(); i++)
	//{
	//	m_sCards[i].obj->Action(waitFrame + (i * 8));
	//}

}

// アップ
void PersonaUI::UpEffectAction()
{
	//int no = m_pPlayerReferences->GetStand()->GetStandStock() - 1;
	//if ((int)m_sCards.size() <= no)MyAssert(0, "最大サイズをこえてるんご");
	//
	//m_sCards[no].fullflag = true;
	//m_sCards[no].fullCardRip->Action();

	m_bTriggerFlag = true;
	MyPersona->GetIconRip()->Action();

	m_pTriggerGageRip->Action();

}

void PersonaUI::DownEffectAction()
{
	//int no = m_pPlayerReferences->GetStand()->GetStandStock() - 1;
	//if ((int)m_sCards.size() <= no)MyAssert(0, "最大サイズをこえてるんご");

	//m_sCards[no].fullflag = false;
	//m_sCards[no].fullCardRip->Action();

	m_bTriggerFlag = false;
	MyPersona->GetIconRip()->Action();

	m_pTriggerGageRip->Action();
}