#include "ComboUI.h"

//ComboUI::ComboUI()
//{
//	m_num = new Number("DATA/UI/Combo/Number.png", 128, Number::NUM_KIND::COMBO);
//	m_damageNum = new Number("DATA/UI/Combo/damageNumber.png", 32, Number::NUM_KIND::NORMAL);
//	//m_num->SetCol(Number::RGB(,255,255,255));
//
//	// コンボ
//	m_iCount = 0;
//	m_iMaxCount = 0;
//
//	m_iDamage = 0;
//	m_iMaxDamage = 0;
//
//	// 
//	m_frontPic = new tdn2DObj("DATA/UI/Combo/front.png");
//	m_backPic = new tdn2DObj("DATA/UI/Combo/back.png");
//
//	m_gageFramePic = new tdn2DObj("DATA/UI/Combo/comboGageFrame.png");
//	m_gageBackPic = new tdn2DObj("DATA/UI/Combo/comboGageFrameBack.png");
//	m_gagePic = new tdn2DObj("DATA/UI/Combo/comboGage.png");
//
//	m_pPlayerData = nullptr;
//	m_iRecoveryFrame = 0;
//	m_iMaxRecoveryFrame = 100;
//
//	// カウンター
//	m_tagCounter.pPic = new tdn2DAnim("Data/UI/Combo/Counter.png");
//	m_tagCounter.pPic->OrderAlphaMove(120, 2, 120 - 8);
//	
//
//}

ComboUI::ComboUI(BasePlayer* PlayerData)
{
	m_num = new Number("DATA/UI/Combo/Number.png", 128, Number::NUM_KIND::COMBO);
	m_damageNum = new Number("DATA/UI/Combo/damageNumber.png", 32, Number::NUM_KIND::NORMAL);
	//m_num->SetCol(Number::RGB(,255,255,255));
	// コンボ
	m_iCount = 0;
	m_iMaxCount = 0;

	m_iDamage = 0;
	m_iNowDamage = 0;
	m_iComboDamage = 0;
	m_iMaxDamage = 0;

	// 
	m_frontPic = new tdn2DObj("DATA/UI/Combo/front.png");
	m_backPic = new tdn2DObj("DATA/UI/Combo/back.png");

	m_gageFramePic = new tdn2DObj("DATA/UI/Combo/comboGageFrame.png");
	m_gageBackPic = new tdn2DObj("DATA/UI/Combo/comboGageFrameBack.png");
	m_gagePic = new tdn2DObj("DATA/UI/Combo/comboGage.png");

	m_pPlayerData = PlayerData;
	m_iRecoveryFrame = 0;
	m_iMaxRecoveryFrame = 100;

	m_bRenderFlag = false;
	m_iRenderFlagWaitFrame = 0;

	m_bGuardFlag = false;

	// カウンター
	m_tagCounter.pPic = new tdn2DAnim("Data/UI/Combo/Counter.png");
	m_tagCounter.pPic->OrderAlphaMove(90, 4, 90 - 8);
	
	m_eSide = m_pPlayerData->GetSide();
	if (m_eSide == SIDE::LEFT)
	{
		m_vPos.x = (int)100;
		m_vPos.y = (int)200;
	}else
	{
		m_vPos.x = (int)1050;
		m_vPos.y = (int)200;
	}
	m_tagCounter.iAddX = 0;

}

ComboUI::~ComboUI()
{
	SAFE_DELETE(m_num);
	SAFE_DELETE(m_damageNum);

	SAFE_DELETE(m_frontPic);
	SAFE_DELETE(m_backPic)

	SAFE_DELETE(m_gageFramePic);
	SAFE_DELETE(m_gageBackPic);
	SAFE_DELETE(m_gagePic);

	SAFE_DELETE(m_tagCounter.pPic);

}

void ComboUI::Update()
{
	MyAssert(m_iMaxRecoveryFrame != 0, "最大値が0だと0で割ることになる");

	// 数字の更新
	m_num->Update();

	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);

	// 0以上 ガードしてたら初期化しない
	if (rate <= 0.0f || m_bGuardFlag == true)
	{
		// レートが0ならば2秒後UIを消す
		m_iRenderFlagWaitFrame++;
		if (m_iRenderFlagWaitFrame > 120)
		{
			m_bRenderFlag = false;
			m_iCount = 0;
			m_iDamage = 0;
		}

	}
	else
	{
		m_bRenderFlag = true;
		m_iRenderFlagWaitFrame = 0;

	}

	// 更新
	m_tagCounter.pPic->Update();
	// カウンターの動き
	if (m_eSide == SIDE::LEFT)
	{
		m_tagCounter.iAddX += 256 / 8;
		if (m_tagCounter.iAddX >= 0)
		{
			m_tagCounter.iAddX = 0;
		}
	}
	else
	{
		m_tagCounter.iAddX -= 256 / 8;
		
		if (m_tagCounter.iAddX <= 0)
		{
			m_tagCounter.iAddX = 0;
		}
	}

}

void ComboUI::Render()
{
	if (m_bRenderFlag == false)return;	// 描画させない	
	if (m_iCount == 0)return;			// コンボが0なら描画させない

	m_backPic->Render((int)m_vPos.x, (int)m_vPos.y, RS::COPY_NOZ);
	m_num->Render((int)m_vPos.x, (int)m_vPos.y -14, m_iCount, Number::NUM_KIND::COMBO);// 数値
	m_frontPic->Render((int)m_vPos.x, (int)m_vPos.y, RS::COPY_NOZ);

	int gageX, gageY;
	gageX = 16;
	gageY = 144;

	// ゲージ類
	m_gageBackPic->Render((int)m_vPos.x + gageX, (int)m_vPos.y + gageY,RS::COPY_NOZ);
	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);
	if (m_bGuardFlag == true)rate = 0.0f;
	m_gagePic->Render((int)m_vPos.x + gageX, (int)m_vPos.y + gageY, int(128 * rate), 64, 0, 0, int(128 * rate), 64, RS::COPY_NOZ);
	m_gageFramePic->Render((int)m_vPos.x + gageX, (int)m_vPos.y + gageY,RS::COPY_NOZ);

	m_damageNum->Render((int)m_vPos.x + gageX + 64, (int)m_vPos.y + gageY - 30, m_iDamage, Number::NUM_KIND::DAMAGE_SCORE);// 数値

	// カウンター
	m_tagCounter.pPic->Render((int)m_vPos.x + m_tagCounter.iAddX , (int)m_vPos.y);

}

//void ComboUI::GageUpdate()
//{
//	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);
//	
//	if (rate > 0.0f)
//	{
//		m_bRenderFlag = true;
//	}
//	else
//	{
//		m_bRenderFlag = false;
//	}
//}

void ComboUI::Count(int damage, int maxRecovery, bool bCounterHit)
{
	MyAssert(maxRecovery != 0, "最大値が0だと0で割ることになる");


	// ウェイトタイマーが作動してるじょうたいなら
	if (m_iRenderFlagWaitFrame >= 1)
	{
		// 初期化
		m_iCount = 0;
		m_iDamage = 0;
	}

	m_num->Action();

	m_bGuardFlag = false;

	m_iCount++;	
	// 最大コンボ数を保存
	if (m_iMaxCount <= m_iCount)
	{
		m_iMaxCount = m_iCount;
	}
	
	m_iDamage += damage;

	m_iNowDamage = damage;
	m_iComboDamage = m_iDamage;

	// 最大コンボ数を保存
	if (m_iMaxDamage <= m_iDamage)
	{
		m_iMaxDamage = m_iDamage;
	}

	m_iMaxRecoveryFrame = maxRecovery;

	// カウンターヒットならばカウンターの演出追加
	if (bCounterHit)
	{
		// カウンター
		m_tagCounter.pPic->Action();
		if (m_eSide == SIDE::LEFT)
		{
			m_tagCounter.iAddX = -256;
		}
		else
		{
			m_tagCounter.iAddX = +256;
		}
	}

}

//void ComboUI::Count(int damage, int maxRecovery,int* recovery)
//{
//	// ウェイトタイマーが作動してるじょうたいなら
//	if (m_iRenderFlagWaitFrame >= 1)
//	{
//		// 初期化
//		m_iCount = 0;
//		m_iDamage = 0;
//	}
//
//	m_num->Action();
//	
//	m_bGuardFlag = false;
//
//	m_iCount++;
//	// 最大コンボ数を保存
//	if (m_iMaxCount <= m_iCount)
//	{
//		m_iMaxCount = m_iCount;
//	}
//
//	m_iDamage += damage;
//	// 最大コンボ数を保存
//	if (m_iMaxDamage <= m_iDamage)
//	{
//		m_iMaxDamage = m_iDamage;
//	}
//
//	m_iMaxRecoveryFrame = maxRecovery;
//
//	m_pRecoveryFrame = recovery;
//}

void ComboUI::Guard()
{
	m_bGuardFlag = true;
}
