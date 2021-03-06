#include "HpGage.h"

//+-----------------------------
//		HPゲージ
//+-----------------------------

// [1206](最適化) 100->20

HpGage::HpGage(BasePlayer* pPlayer)
{
	m_pGage = new tdn2DObj("Data/UI/Game/HPGage.png");
	m_iGageWidth = 422;
	m_iGageHeight = 25;

	m_pGageUsually = new tdn2DObj("Data/UI/Game/HPGageUsually.png");
	m_pGagePinch = new tdn2DObj("Data/UI/Game/HPGagePinch.png");

	m_pGagePinchRip = new tdn2DAnim("Data/UI/Game/HPGagePinchRip.png");
	m_pGagePinchRip->OrderRipple(16, 1.0f, 0.025f);
	//m_pGagePinchRip->OrderAlphaMove(15, 7, 8);
	m_iGagePinchRipFrame = 0;


	m_pWave = new tdn2DObj("Data/UI/Game/HPGage_Wave.png");
	m_fWaveUV = 0.0f;

	m_pPlayerReferences = pPlayer;

	m_iMaxHP = m_pPlayerReferences->GetMaxHP();
	m_iHP = m_pPlayerReferences->GetHP();
	m_iRemainingHP = m_pPlayerReferences->GetMaxHP();// 一試合の最低HP

	m_pDamageGage = new tdn2DObj("Data/UI/Game/HPGage_Damage.png");
	m_iDamagePoint = m_pPlayerReferences->GetHP();
		
	m_fDamageRate = 0.0f;
	m_fRate = 0.0f;

	// 右か左
	m_sSideFlag = pPlayer->GetSide();
	if (m_sSideFlag == SIDE::LEFT)
	{
		m_vPos.x = 125;
		m_vPos.y = 67;
	}
	else
	{
		m_vPos.x = 733;
		m_vPos.y = 67;
		m_pWave->SetTurnOver(true);
		//m_fWaveUV = -1;
	}

	// マスク
	m_pMaskScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom / 3, TDN2D::RENDERTARGET);
	m_pUseMaskScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom / 3 , TDN2D::USEALPHA);

	m_pMaskPic = new tdn2DObj("Data/UI/Game/HPGage_Mask.png");
	m_fMaskPicScare = 1.0f;

	m_bFirstActionFlag = false;
	m_iWaitFrame = 0;

	m_bUseMask = true;

	//// SPゲージ
	//m_pSpGage = new tdn2DObj("Data/UI/Game/SPGage.png");
	//if (m_sSideFlag == UI_SIDE::LEFT)
	//{
	//	m_vSpPos.x = 63;
	//	m_vSpPos.y = 672;
	//}
	//else
	//{
	//	m_vSpPos.x = 908;
	//	m_vSpPos.y = 672;
	//}

	//// SPゲージ
	//m_fSpRate = 0.0f;
	//m_iSPGageWidth = 310;
	//m_iSPGageHeight = 19;


}

HpGage::~HpGage()
{
	SAFE_DELETE(m_pGage);
	SAFE_DELETE(m_pGageUsually);
	SAFE_DELETE(m_pGagePinch);
	SAFE_DELETE(m_pGagePinchRip);
	SAFE_DELETE(m_pWave);
	SAFE_DELETE(m_pDamageGage);

	// マスク
	SAFE_DELETE(m_pMaskScreen);
	SAFE_DELETE(m_pUseMaskScreen);
	SAFE_DELETE(m_pMaskPic);

	// SP
	//SAFE_DELETE(m_pSpGage);

}

void HpGage::Update()
{
	// ウェイト処理	
	if (m_iWaitFrame > 0) 
	{
		m_iWaitFrame--;
		if (m_iWaitFrame > 0)
		{
			return;
		}	
	}

	// ウェーブのUV
	m_fWaveUV -= 1.0f;

	// HPのレートを計算
	MyAssert((m_pPlayerReferences->GetMaxHP() != 0), "最大HPが0だと0で割ることに");
	m_fRate= (float)m_pPlayerReferences->GetHP() / (float)m_pPlayerReferences->GetMaxHP();


	// ダメージ赤バーのレートを計算 コンボが続いてたら更新しない
	if (m_pPlayerReferences->GetRecoveryFrame() <= 0)
	{
		m_iDamagePoint -= 50;
		
		// HP分引く
		if ( m_iDamagePoint <= m_pPlayerReferences->GetHP())
		{
			m_iDamagePoint = m_pPlayerReferences->GetHP();
		}

	}

	m_fDamageRate = (float)m_iDamagePoint / (float)m_pPlayerReferences->GetMaxHP();

	// 覚醒HP波紋
	m_iGagePinchRipFrame++;
	if (m_iGagePinchRipFrame >= 40)
	{
		m_iGagePinchRipFrame = 0;
		if (m_pPlayerReferences->isWillPower())
		{
			m_pGagePinchRip->Action();
		}
	}
	
	m_pGagePinchRip->Update();


	// SPレート
	//if (m_pPlayerReferences->isOverDrive())
	//{	
	//	m_fSpRate = (float)m_pPlayerReferences->GetOverDriveFrame() / (float)m_pPlayerReferences->c_OVERDRIVE_MAX_TIME;
	//}
	//else
	//{
	//	m_fSpRate = (float)m_pPlayerReferences->GetOverDriveGage() / (float)m_pPlayerReferences->c_OVERDRIVE_MAX_GAGE;
	//}


	// マスク更新
	if (m_bFirstActionFlag == true &&
		m_bUseMask == true)
	{
		MaskUpdate();
	}

}

void HpGage::MaskUpdate()
{

	// スケールを大きく
	m_fMaskPicScare += 0.2f;
	if (m_fMaskPicScare >= 18.0f)
	{
		m_fMaskPicScare = 18.0f;
		m_bUseMask = false;		// マスク描画解除
	}

	// マスク
	m_pMaskPic->SetScale(m_fMaskPicScare);
}

void HpGage::Render()
{
	if (m_iWaitFrame > 0) return;
	if (m_bFirstActionFlag == false) return;

	// マスクを使用してる時で描画を分ける
	if (m_bUseMask == true)
	{
		// マスクスクリーン
		MaskScreen();

		// マスク適用スクリーン
		UseMaskScreen();

		m_pUseMaskScreen->Render(0, 0, shader2D, "Mask");
	}
	else
	{
		// レンダーターゲットを使わない描画
		//NotMaskRender();
		UseMaskScreen();
		m_pUseMaskScreen->Render(0, 0);

	}

	//m_pUseMaskScreen->Render(0, 0);

	//if (m_sSideFlag == UI_SIDE::LEFT)
	//{
	//	m_pGage->Render((int)m_vPos.x, (int)m_vPos.y-25);
	//}
	//else
	//{
	//	m_pGage->Render((int)m_vPos.x, (int)m_vPos.y+ 25);
	//}
}

// マスク適用
void HpGage::UseMaskScreen()
{

	Surface* saveSurface;
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	// マスクに切り替え
	m_pUseMaskScreen->RenderTarget(0);
	{
		// 画面クリア もちろんHPが無くなったとこの描画が大変なことになるので
		tdnView::Clear();


		// ゲージの描画
		if (m_sSideFlag == SIDE::LEFT)
		{
			// 赤ダメージゲージ
			m_pDamageGage->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fDamageRate)), (int)m_vPos.y,
				(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight,
				m_iGageWidth - (int)(m_iGageWidth  * m_fDamageRate), 0,
				(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight);

			// HPによって色を変える
			// 覚醒してるか
			if (m_pPlayerReferences->isWillPower())
			{
				// 覚醒時は黄色
				m_pGagePinch->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight);
				// 波紋
				m_pGagePinchRip->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight, RS::ADD);
			}
			else
			{
				if (m_fRate >= 1.0f)
				{
					m_pGage->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight,
						m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight);
				}
				else /*if (m_fRate >= 0.35f)*/// 普通位
				{
					m_pGageUsually->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight,
						m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight);

				}
			}


			//m_pWave->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
			//	(int)(m_iGageWidth), m_iGageHeight,
			//	m_iGageWidth - (int)(m_iGageWidth  * m_fRate)+ (int)m_fWaveUV, 0,
			//	(int)(m_iGageWidth), m_iGageHeight, RS::ADD);

			//-----------------
			// SPゲージ
			//-----------------
			//if (m_pPlayerReferences->isOverDrive())
			//{

			//	int l_w = (int)(m_iSPGageWidth * m_fSpRate);

			//	m_pSpGage->Render((int)m_vSpPos.x + (m_iSPGageWidth - l_w), (int)m_vSpPos.y,
			//		l_w, m_iSPGageHeight,
			//		m_iSPGageWidth - l_w, 0,
			//		l_w, m_iSPGageHeight);
			//}
			//else
			//{

			//	int l_w = (int)(m_iSPGageWidth * m_fSpRate);

			//	m_pSpGage->Render((int)m_vSpPos.x + (m_iSPGageWidth - l_w), (int)m_vSpPos.y,
			//		l_w, m_iSPGageHeight,
			//		m_iSPGageWidth - l_w, 0,
			//		l_w, m_iSPGageHeight);
			//}

		
		}
		else
		{
			// 赤ダメージゲージ
			m_pDamageGage->Render((int)m_vPos.x , (int)m_vPos.y,
				(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight,
				0, 0,
				(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight);
			
			
			// HPによって色を変える
			// 覚醒してるか
			if (m_pPlayerReferences->isWillPower())
			{
				// 覚醒時は黄色
				m_pGagePinch->Render((int)m_vPos.x, (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					0, 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight);

				// 波紋
				m_pGagePinchRip->Render((int)m_vPos.x, (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					0, 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight, RS::ADD);
			}
			else
			{

				if (m_fRate >= 1.0f)
				{
					m_pGage->Render((int)m_vPos.x, (int)m_vPos.y,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight,
						0, 0,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight);

				}
				else /*if (m_fRate >= 0.35f)*/// 普通位
				{
					m_pGageUsually->Render((int)m_vPos.x, (int)m_vPos.y,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight,
						0, 0,
						(int)(m_iGageWidth*m_fRate), m_iGageHeight);

				}
			}


			//// TODO
			//m_pWave->Render((int)m_vPos.x, (int)m_vPos.y,
			//	m_iGageWidth, m_iGageHeight,
			//	(int)m_fWaveUV, 0,
			//	m_iGageWidth, m_iGageHeight, RS::ADD);


			//-----------------
			//	SPゲージ
			//-----------------
			//if (m_pPlayerReferences->isOverDrive())
			//{
			//	int l_w = (int)(m_iSPGageWidth * m_fSpRate);

			//	m_pSpGage->Render((int)m_vSpPos.x, (int)m_vSpPos.y,
			//		l_w, m_iSPGageHeight,
			//		0, 0,
			//		l_w, m_iSPGageHeight);
			//}
			//else
			//{
			//	int l_w = (int)(m_iSPGageWidth * m_fSpRate);

			//	m_pSpGage->Render((int)m_vSpPos.x, (int)m_vSpPos.y,
			//		l_w, m_iSPGageHeight,
			//		0, 0,
			//		l_w, m_iSPGageHeight);


			//}

		} // m_sSideFlag


	}// RenderTarget

	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//レンダーターゲットの復元


}


// マスクスクリーン
void HpGage::MaskScreen()
{
	Surface* saveSurface;
	// まずは今のサーフェイスを保存
	tdnSystem::GetDevice()->GetRenderTarget(0, &saveSurface);
	// マスクに切り替え
	m_pMaskScreen->RenderTarget(0);
	{
		// 画面クリア
		tdnView::Clear();

		// 初期の真ん中から広がり描画される演出のためのマスク
		m_pMaskPic->Render((tdnSystem::GetScreenSize().right / 2) - 128 , 0);

	}
	tdnSystem::GetDevice()->SetRenderTarget(0, saveSurface);//レンダーターゲットの復元

	// ↑で得られた情報をマスクとして送る
	shader2D->SetMaskScreen(m_pMaskScreen);

}

// マスクを使用しない描画
void HpGage::NotMaskRender()
{
	// ゲージの描画
	if (m_sSideFlag == SIDE::LEFT)
	{
		// 赤ダメージゲージ
		m_pDamageGage->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fDamageRate)), (int)m_vPos.y,
			(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight,
			m_iGageWidth - (int)(m_iGageWidth  * m_fDamageRate), 0,
			(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight);

		// HPによって色を変える
		// 覚醒してるか
		if (m_pPlayerReferences->isWillPower())
		{
			// 覚醒時は黄色
			m_pGagePinch->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight,
				m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight);
			// 波紋
			m_pGagePinchRip->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight,
				m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight, RS::ADD);
		}
		else
		{
			if (m_fRate >= 1.0f)
			{
				m_pGage->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight);
			}
			else /*if (m_fRate >= 0.35f)*/// 普通位
			{
				m_pGageUsually->Render((int)m_vPos.x + (m_iGageWidth - (int)(m_iGageWidth * m_fRate)), (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					m_iGageWidth - (int)(m_iGageWidth  * m_fRate), 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight);

			}
		}


		//m_pWave->Render((int)m_vPos.x /*+ (m_iGageWidth - (int)(m_iGageWidth * m_fRate))*/, (int)m_vPos.y,
		//	(int)(m_iGageWidth), m_iGageHeight,
		//	/*m_iGageWidth - (int)(m_iGageWidth  * m_fRate) +*/ (int)m_fWaveUV, 0,
		//	(int)(m_iGageWidth), m_iGageHeight, RS::ADD);




	}
	else
	{
		// 赤ダメージゲージ
		m_pDamageGage->Render((int)m_vPos.x, (int)m_vPos.y,
			(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight,
			0, 0,
			(int)(m_iGageWidth*m_fDamageRate), m_iGageHeight);


		// HPによって色を変える
		// 覚醒してるか
		if (m_pPlayerReferences->isWillPower())
		{
			// 覚醒時は黄色
			m_pGagePinch->Render((int)m_vPos.x, (int)m_vPos.y,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight,
				0, 0,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight);

			// 波紋
			m_pGagePinchRip->Render((int)m_vPos.x, (int)m_vPos.y,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight,
				0, 0,
				(int)(m_iGageWidth*m_fRate), m_iGageHeight, RS::ADD);
		}
		else
		{

			if (m_fRate >= 1.0f)
			{
				m_pGage->Render((int)m_vPos.x, (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					0, 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight);

			}
			else /*if (m_fRate >= 0.35f)*/// 普通位
			{
				m_pGageUsually->Render((int)m_vPos.x, (int)m_vPos.y,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight,
					0, 0,
					(int)(m_iGageWidth*m_fRate), m_iGageHeight);

			}
		}


		// TODO
		//m_pWave->Render((int)m_vPos.x, (int)m_vPos.y,
		//	m_iGageWidth, m_iGageHeight,
		//	(int)m_fWaveUV, 0,
		//	m_iGageWidth, m_iGageHeight, RS::ADD);

	}

}


void HpGage::FirstAction(int waitFrame)
{
	m_iWaitFrame = waitFrame;
	m_bFirstActionFlag = true;	

	// 初期設定
	m_fMaskPicScare = 1.0f;

	m_bUseMask = true;// マスクフラグ立てる
}

void HpGage::JudeRemainingHP()
{
	// 最低HPを保存
	if (m_iRemainingHP > m_pPlayerReferences->GetHP())
	{
		m_iRemainingHP = m_pPlayerReferences->GetHP();
	}

}
