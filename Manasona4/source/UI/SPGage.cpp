#include "SpGage.h"

SpGage::SpGage(BasePlayer * pPlayer)
{
	m_pPlayerReferences = pPlayer;
	m_fRate = 0.0f;
	m_iGageWidth = 310;
	m_iGageHeight = 19;
	m_iGageLevel = GAGE_LEVEL::NORMAL;

	// SPゲージ
	m_sSideFlag = pPlayer->GetSide();
	m_pGage = new tdn2DAnim("Data/UI/Game/SPGage.png");
	m_pGage->OrderShrink(12, 1, 1.1f);
	m_pGageRip = new tdn2DAnim("Data/UI/Game/SPGage.png");
	m_pGageRip->OrderRipple(12, 1, 0.03f);
	m_pSpNum = new tdn2DAnim("Data/UI/Game/SPNum.png");
	m_pSpNum->OrderShrink(12, 1, 1.1f);
	
	m_pGageFlash = new tdn2DObj("Data/UI/Game/SPGageAdd.png");
	//m_pGageFlash->OrderShrink(12, 1, 1.1f);
	m_iAlpha = 0;
	m_bAlphaReturnFlag = false;
	m_iAlphaPow = 0;

	if (m_sSideFlag == SIDE::LEFT)
	{
		m_vPos.x = 63+32;
		m_vPos.y = 672;

		m_vNumPos.x = 57;
		m_vNumPos.y = 664;
	}
	else
	{
		m_vPos.x = 908-32;
		m_vPos.y = 672;

		m_vNumPos.x = 1240;
		m_vNumPos.y = 664;
	}

}

SpGage::~SpGage()
{
	SAFE_DELETE(m_pGage);
	SAFE_DELETE(m_pGageRip);
	SAFE_DELETE(m_pSpNum);
	SAFE_DELETE(m_pGageFlash);
}

void SpGage::Update()
{

	// SPレート
	if (m_pPlayerReferences->isOverDrive())
	{	
		m_fRate = (float)m_pPlayerReferences->GetOverDriveFrame() / (float)m_pPlayerReferences->c_OVERDRIVE_MAX_TIME;
		
		// 色は常に固定
		m_iGageLevel = GAGE_LEVEL::OVER_DRIVE;
	}
	else
	{
		m_fRate = (float)m_pPlayerReferences->GetOverDriveGage() / (float)m_pPlayerReferences->c_OVERDRIVE_MAX_GAGE;
		
		// ゲージの色を変える処理
		if (m_fRate >= 1.0f)
		{
			if (m_iGageLevel != GAGE_LEVEL::MAX)
			{
				m_iGageLevel = GAGE_LEVEL::MAX;
				m_pGageRip->Action();

				m_iAlpha = 0;
				m_bAlphaReturnFlag = false;
				m_iAlphaPow = 255 / 12;
			}
			
		}
		else if (m_fRate >= 0.5f)
		{
			if (m_iGageLevel != GAGE_LEVEL::HARF)
			{
				m_iGageLevel = GAGE_LEVEL::HARF;
				m_pGageRip->Action();	

				m_iAlpha = 0;
				m_bAlphaReturnFlag = false;
				m_iAlphaPow = 255 / 40;
			}
		}
		else
		{
			if (m_iGageLevel != GAGE_LEVEL::NORMAL)
			{
				m_iGageLevel = GAGE_LEVEL::NORMAL;				
			
			}

			m_iAlpha = 0;
			m_bAlphaReturnFlag = false;
		}

	}

	// 更新
	m_pGage->Update();
	m_pGageRip->Update();

	m_pSpNum->Update();

	// 更新
	if (m_bAlphaReturnFlag == false)
	{
		m_iAlpha += m_iAlphaPow;
		if (m_iAlpha>=255)
		{
			m_bAlphaReturnFlag = true;
		}

	}
	else
	{
		m_iAlpha -= m_iAlphaPow;
		if (m_iAlpha <= 0)
		{
			m_bAlphaReturnFlag = false;
		}

	}
	
	//animekese
	m_iAlpha = (int)Math::Clamp((float)m_iAlpha, 0, 255);
	m_pGageFlash->SetAlpha(m_iAlpha);
	//m_pGageFlash->Update();
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
				m_iGageWidth - l_w, m_iGageHeight*m_iGageLevel ,
				l_w, m_iGageHeight);

			// 点滅
			m_pGageFlash->Render((int)m_vPos.x + (m_iGageWidth - l_w), (int)m_vPos.y,
				l_w, m_iGageHeight,
				m_iGageWidth - l_w, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight, RS::ADD);

		}
		else
		{

			int l_w = (int)(m_iGageWidth * m_fRate);

			m_pGage->Render((int)m_vPos.x + (m_iGageWidth - l_w), (int)m_vPos.y,
				l_w, m_iGageHeight,
				m_iGageWidth - l_w, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight);
			
			// 点滅
			m_pGageFlash->Render((int)m_vPos.x + (m_iGageWidth - l_w), (int)m_vPos.y,
				l_w, m_iGageHeight,
				m_iGageWidth - l_w, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight, RS::ADD);


			// 波紋
			m_pGageRip->Render((int)m_vPos.x + (m_iGageWidth - l_w), (int)m_vPos.y,
				l_w, m_iGageHeight,
				m_iGageWidth - l_w, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight, RS::ADD);

			int number = (int)m_pPlayerReferences->GetOverDriveGage();
			for (int count = 0; count <= 2; ++count)
			{
				int digitNum = number % 10;	// 一番小さい桁を入手
				number = (int)(number / 10);// 数値の一番小さい桁を消す

				m_pSpNum->Render((int)m_vNumPos.x - ((count)* 24), (int)m_vNumPos.y,
					32, 32, digitNum*32, 0, 32, 32);

				//if (number <= 0)break;// 数値が０以下になったらさよなら
			}


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
				0, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight);

			// 点滅
			m_pGageFlash->Render((int)m_vPos.x, (int)m_vPos.y,
				l_w, m_iGageHeight,
				0, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight,RS::ADD);
		}
		else
		{
			int l_w = (int)(m_iGageWidth * m_fRate);

			m_pGage->Render((int)m_vPos.x, (int)m_vPos.y,
				l_w, m_iGageHeight,
				0, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight);

			// 点滅
			m_pGageFlash->Render((int)m_vPos.x, (int)m_vPos.y,
				l_w, m_iGageHeight,
				0, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight, RS::ADD);

			// 波紋
			m_pGageRip->Render((int)m_vPos.x, (int)m_vPos.y,
				l_w, m_iGageHeight,
				0, m_iGageHeight*m_iGageLevel,
				l_w, m_iGageHeight, RS::ADD);

			int number = (int)m_pPlayerReferences->GetOverDriveGage();
			for (int count = 0; count <= 2; ++count)
			{
				int digitNum = number % 10;	// 一番小さい桁を入手
				number = (int)(number / 10);// 数値の一番小さい桁を消す

				m_pSpNum->Render((int)m_vNumPos.x - ((count)* 24), (int)m_vNumPos.y,
					32, 32, digitNum * 32, 0, 32, 32);

				//if (number <= 0)break;// 数値が０以下になったらさよなら
			}
		}

	}

}

void SpGage::Action(int delayTimer)
{

	m_pGage->Action(delayTimer);
	//m_pGageFlash->Action(delayTimer);
	m_pSpNum->Action(delayTimer);

	m_iAlpha = 0;
	m_bAlphaReturnFlag = false;
}