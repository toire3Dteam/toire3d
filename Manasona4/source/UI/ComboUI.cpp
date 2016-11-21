#include "ComboUI.h"

ComboUI::ComboUI()
{
	m_num = new Number("DATA/UI/Combo/Number.png", 128, Number::NUM_KIND::COMBO);
	m_damageNum = new Number("DATA/UI/Combo/damageNumber.png", 32, Number::NUM_KIND::NORMAL);
	//m_num->SetCol(Number::RGB(,255,255,255));

	// コンボ
	m_iCount = 0;
	m_iMaxCount = 0;

	m_iDamage = 0;
	m_iMaxDamage = 0;

	// 
	m_frontPic = new tdn2DObj("DATA/UI/Combo/front.png");
	m_backPic = new tdn2DObj("DATA/UI/Combo/back.png");

	m_gageFramePic = new tdn2DObj("DATA/UI/Combo/comboGageFrame.png");
	m_gageBackPic = new tdn2DObj("DATA/UI/Combo/comboGageFrameBack.png");
	m_gagePic = new tdn2DObj("DATA/UI/Combo/comboGage.png");

	m_pPlayerData = nullptr;
	m_iRecoveryFrame = 0;
	m_iMaxRecoveryFrame = 100;

}

ComboUI::ComboUI(BasePlayer* PlayerData)
{
	m_num = new Number("DATA/UI/Combo/Number.png", 128, Number::NUM_KIND::COMBO);
	m_damageNum = new Number("DATA/UI/Combo/damageNumber.png", 32, Number::NUM_KIND::NORMAL);
	//m_num->SetCol(Number::RGB(,255,255,255));
	// コンボ
	m_iCount = 0;
	m_iMaxCount = 0;

	m_iDamage = 0;
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
}

void ComboUI::Update()
{
	MyAssert(m_iMaxRecoveryFrame != 0, "最大値が0だと0で割ることになる");

	// 数字の更新
	m_num->Update();

	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);

	if (rate > 0.0f)
	{
		m_bRenderFlag = true;
		m_iRenderFlagWaitFrame = 0;
	}
	else
	{
	
		if (++m_iRenderFlagWaitFrame > 120)
		{
			m_bRenderFlag = false;
			m_iCount = 0;
			m_iDamage = 0;
		}	

	}

}

void ComboUI::Render(int x, int y)
{
	if (m_bRenderFlag == false)return;// 描画させない
	
	m_backPic->Render(x, y, RS::COPY_NOZ);
	m_num->Render(x , y-14, m_iCount, Number::NUM_KIND::COMBO);// 数値
	m_frontPic->Render(x, y, RS::COPY_NOZ);

	int gageX, gageY;
	gageX = 16;
	gageY = 144;

	// ゲージ類
	m_gageBackPic->Render(x + gageX, y + gageY,RS::COPY_NOZ);
	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);
	if (m_bGuardFlag == true)rate = 0.0f;
	m_gagePic->Render(x + gageX, y + gageY, int(128 * rate), 64, 0, 0, int(128 * rate), 64, RS::COPY_NOZ);
	m_gageFramePic->Render(x + gageX, y + gageY,RS::COPY_NOZ);

	m_damageNum->Render(x + gageX + 64, y + gageY - 30, m_iDamage, Number::NUM_KIND::DAMAGE_SCORE);// 数値

}//つぎはげーじとかだめーじほぞんできるようにする

void ComboUI::GageUpdate()
{
	float rate = float(m_pPlayerData->GetRecoveryFrame()) / float(m_iMaxRecoveryFrame);
	
	if (rate > 0.0f)
	{
		m_bRenderFlag = true;
	}
	else
	{
		m_bRenderFlag = false;
	}
}

void ComboUI::Count(int damage, int maxRecovery)
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
	// 最大コンボ数を保存
	if (m_iMaxDamage <= m_iDamage)
	{
		m_iMaxDamage = m_iDamage;
	}

	m_iMaxRecoveryFrame = maxRecovery;
		
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
