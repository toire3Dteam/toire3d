#include "SpGage.h"

SpGage::SpGage(BasePlayer * pPlayer, SIDE bSide)
{
	m_pPlayerReferences = pPlayer;
	m_fRate = 0.0f;
	m_iGageWidth = 310;
	m_iGageHeight = 19;

	// SPゲージ
	m_sSideFlag = bSide;
	m_pGage = new tdn2DObj("Data/UI/Game/SPGage.png");
	if (m_sSideFlag == SIDE::LEFT)
	{
		m_vPos.x = 63;
		m_vPos.y = 672;
	}
	else
	{
		m_vPos.x = 908;
		m_vPos.y = 672;
	}

}

SpGage::~SpGage()
{
	SAFE_DELETE(m_pGage);
}

void SpGage::Update()
{
	// SPレート
	if (m_pPlayerReferences->isOverDrive())
	{	
		m_fRate = (float)m_pPlayerReferences->GetOverDriveFrame() / (float)m_pPlayerReferences->c_OVERDRIVE_MAX_TIME;
	}
	else
	{
		m_fRate = (float)m_pPlayerReferences->GetOverDriveGage() / (float)m_pPlayerReferences->c_OVERDRIVE_MAX_GAGE;
	}
}

void SpGage::Render()
{
	// ゲージの描画
	if (m_sSideFlag == SIDE::LEFT)
	{

		//-----------------
		// SPゲージ
		//-----------------

		// 覚醒した後の減少
		if (m_pPlayerReferences->isOverDrive())
		{

			int l_w = (int)(m_iGageWidth * m_fRate);

			m_pGage->Render((int)m_vPos.x + (m_iGageWidth - l_w), (int)m_vPos.y,
				l_w, m_iGageHeight,
				m_iGageWidth - l_w, 0,
				l_w, m_iGageHeight);
		}
		else
		{

			int l_w = (int)(m_iGageWidth * m_fRate);

			m_pGage->Render((int)m_vPos.x + (m_iGageWidth - l_w), (int)m_vPos.y,
				l_w, m_iGageHeight,
				m_iGageWidth - l_w, 0,
				l_w, m_iGageHeight);
		}
	}
	else
	{
		//-----------------
		//	SPゲージ
		//-----------------

		// 覚醒した後の減少
		if (m_pPlayerReferences->isOverDrive())
		{
			int l_w = (int)(m_iGageWidth * m_fRate);

			m_pGage->Render((int)m_vPos.x, (int)m_vPos.y,
				l_w, m_iGageHeight,
				0, 0,
				l_w, m_iGageHeight);
		}
		else
		{
			int l_w = (int)(m_iGageWidth * m_fRate);

			m_pGage->Render((int)m_vPos.x, (int)m_vPos.y,
				l_w, m_iGageHeight,
				0, 0,
				l_w, m_iGageHeight);

		}

	}

}
