#include "FacePic.h"

//+------------------------
//		ŠçƒOƒ‰
//+------------------------	

FacePic::FacePic(BasePlayer* pPlayer)
{
	m_pPlayerReferences = pPlayer;

	m_sSideFlag = pPlayer->GetSide();

	// êŠÝ’è
	if (m_sSideFlag == SIDE::LEFT)
	{
		m_vPos.x = -8;
		m_vPos.y = 0;
	}
	else
	{
		m_pPlayerReferences->GetFacePic()->SetTurnOver(true);
		m_vPos.x = 1280 - 128 + 8;
		m_vPos.y = 0;
	}

	m_pPanelEF = new ClearEffect();

}

FacePic::~FacePic()
{
	SAFE_DELETE(m_pPanelEF);
}


void FacePic::Update()
{
	m_pPlayerReferences->GetFacePic()->Update();
	m_pPanelEF->Update();

}

void FacePic::Render()
{
	m_pPlayerReferences->GetFacePic()->Render((int)m_vPos.x, (int)m_vPos.y);
	m_pPanelEF->Render();
}

void FacePic::Action(int waitFrame)
{
	m_pPlayerReferences->GetFacePic()->Action(waitFrame);
	m_pPanelEF->Action((int)m_vPos.x+64, (int)m_vPos.y + 64, waitFrame);
}
