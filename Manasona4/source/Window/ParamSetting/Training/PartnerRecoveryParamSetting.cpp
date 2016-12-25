#include "PartnerRecoveryParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	パートナーゲージの設定
//+----------------------------

PartnerRecoveryParamSetting::PartnerRecoveryParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

PartnerRecoveryParamSetting::~PartnerRecoveryParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void PartnerRecoveryParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void PartnerRecoveryParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_TrainingData.iPartnerRecovery != (int)PARTNER_RECOVERY_TYPE::DEFAULT)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_TrainingData.iPartnerRecovery != (int)PARTNER_RECOVERY_TYPE::MAX)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右

																							// これは文字表記で描画
																							// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((PARTNER_RECOVERY_TYPE)PlayerDataMgr->m_TrainingData.iPartnerRecovery)
	{
	case PARTNER_RECOVERY_TYPE::DEFAULT:
		l_pString = "通常";
		break;
	case PARTNER_RECOVERY_TYPE::MAX:
		l_pString = "高速回復";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void PartnerRecoveryParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iPartnerRecovery = (PlayerDataMgr->m_TrainingData.iPartnerRecovery > (int)PARTNER_RECOVERY_TYPE::DEFAULT)
			? PlayerDataMgr->m_TrainingData.iPartnerRecovery - 1 : (int)PARTNER_RECOVERY_TYPE::MAX;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iPartnerRecovery = (PlayerDataMgr->m_TrainingData.iPartnerRecovery < (int)PARTNER_RECOVERY_TYPE::MAX)
			? PlayerDataMgr->m_TrainingData.iPartnerRecovery + 1 : (int)PARTNER_RECOVERY_TYPE::DEFAULT;
	}
}
