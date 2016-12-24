#include "InformationPlate.h"

InformationPlate::InformationPlate()
{
	m_vPos = VECTOR2_ZERO;
	m_vPos.x = 0.0f;
	m_vPos.y = 592;

	m_fMoveY = 0.0f;

	m_pPic = new  tdn2DAnim("Data/UI/Menu/Information.png");
	m_pPic->OrderMoveAppeared(3, 0, (int)m_vPos.y + 128);	

	m_bRender = false;

}

InformationPlate::~InformationPlate()
{
	SAFE_DELETE(m_pPic);
}

void InformationPlate::Update()
{
	if (m_bRender == false)return;
	
	// 動き更新
	m_vPos.y += m_fMoveY;
	m_pPic->Update();

	// 画面↓に行けば終り
	if (m_vPos.y >= 720)
	{
		m_bRender = false;
	}

}

void InformationPlate::Render()
{
	if (m_bRender == false)return;

	// インフォメーション
	m_pPic->Render((int)m_vPos.x, (int)m_vPos.y);

}

void InformationPlate::Action()
{
	m_bRender = true;
	m_vPos.y = 596.0f;
	m_fMoveY = 0.0f;
	m_pPic->Action();
}

void InformationPlate::Stop()
{
	m_fMoveY = 64.0f;
}

