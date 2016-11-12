#include "RoundIcon.h"

RoundIcon::RoundIcon(BasePlayer * pPlayer, int iRoundNum)
{
	m_pPlayerReferences = pPlayer;

	// ラウンドアイコンゲージ
	m_sSideFlag = pPlayer->GetSide();


	//+-----------------------------
	//	アイコンカード
	//+-----------------------------

	// 最大アイコン数を取得
	//m_iMaxIcons = m_pPlayerReferences->GetWinNum();
	m_iMaxIcons = iRoundNum;

	// その分データを作成
	for (int i = 0; i < m_iMaxIcons; i++)
	{
		WinIcon data;

		data.pIconTable = new tdn2DAnim("Data/UI/Game/WinIcon0.png");
		//data.pIconTable ->OrderNone();
		data.pWinIcon = new tdn2DAnim("Data/UI/Game/WinIcon1.png");	
		data.pWinIconRip = new tdn2DAnim("Data/UI/Game/WinIcon1.png");
		//data.pWinIcon ->OrderNone();

		// 右と左で場所を変える
		if (m_sSideFlag == SIDE::LEFT)
		{
			data.vPos.x = 493.0f + (i * -32);
		}else
		{
			data.vPos.x = 724.0f + (i * 32);
		}

		data.vPos.y = 16;
		data.bWinFlag = false; //

		data.pIconTable->OrderMoveAppeared(12, (int)data.vPos.x, (int)data.vPos.y - 36);
		//data.pWinIcon->OrderShrink(12, 1, 2);
		data.pWinIcon->OrderJump(12, 1, 0.75f);
		data.pWinIconRip->OrderRipple(12, 1, 0.2f);

		// 追加
		m_sIcons.push_back(data);
	}

	m_iAnimTimer = 0;
}

RoundIcon::~RoundIcon()
{
	for (int i = 0; i < (int)m_sIcons.size(); i++)
	{
		SAFE_DELETE(m_sIcons[i].pIconTable);
		SAFE_DELETE(m_sIcons[i].pWinIcon);
		SAFE_DELETE(m_sIcons[i].pWinIconRip);
	}
	m_sIcons.clear();
}

void RoundIcon::Update()
{
	// アニメーションタイマー
	if (++m_iAnimTimer >= 60*3)
	{
		for (int i = 0; i < (int)m_sIcons.size(); i++)
		{
			// 点灯画像
			if (m_sIcons[i].bWinFlag == true)
			{
				m_sIcons[i].pWinIcon->Action(i * 10);
			}
		}
		// 0に戻す
		m_iAnimTimer = 0;
	}


	for (int i = 0; i < (int)m_sIcons.size(); i++)
	{
		m_sIcons[i].pIconTable->Update();
		m_sIcons[i].pWinIcon->Update();
		m_sIcons[i].pWinIconRip->Update();

	}

	
}

void RoundIcon::Render()
{
	for (int i = 0; i < (int)m_sIcons.size(); i++)
	{
		m_sIcons[i].pIconTable->Render((int)m_sIcons[i].vPos.x, (int)m_sIcons[i].vPos.y);

		// 点灯画像
		if (m_sIcons[i].bWinFlag == true)
		{
			m_sIcons[i].pWinIcon->Render((int)m_sIcons[i].vPos.x, (int)m_sIcons[i].vPos.y);
			m_sIcons[i].pWinIconRip->Render((int)m_sIcons[i].vPos.x, (int)m_sIcons[i].vPos.y, RS::ADD);

		}
		

	}

}

void RoundIcon::Action(int waitFrame)
{
	for (int i = 0; i < (int)m_sIcons.size(); i++)
	{
		m_sIcons[i].pIconTable->Action(waitFrame + (i * 8));
	}

}

// アップ
void RoundIcon::AppIcon()
{
	int winnum = m_pPlayerReferences->GetWinNum();
	//if ((int)m_sIcons.size() <= no)MyAssert(0, "最大サイズをこえてるんご");

	m_sIcons[winnum].bWinFlag= true;
	m_sIcons[winnum].pWinIcon->Action();
	m_sIcons[winnum].pWinIconRip->Action();
	//m_sIcons[win].fullCardRip->Action();

}
