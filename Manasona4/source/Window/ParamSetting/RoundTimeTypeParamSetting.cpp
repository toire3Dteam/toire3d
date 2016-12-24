#include "RoundTimeTypeParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	ラウンド時間のパラメーター設定
//+----------------------------

RoundTimeTypeParamSetting::RoundTimeTypeParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

RoundTimeTypeParamSetting::~RoundTimeTypeParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void RoundTimeTypeParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void RoundTimeTypeParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_ConfigData.iRoundTimeType != (int)ROUND_TIME_TYPE::SEC_30)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_ConfigData.iRoundTimeType != (int)ROUND_TIME_TYPE::SEC_INFINITY)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右


	// これは文字表記で描画

	// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((ROUND_TIME_TYPE)PlayerDataMgr->m_ConfigData.iRoundTimeType)
	{
	case ROUND_TIME_TYPE::SEC_30:
		l_pString = "30秒";
		break;
	case ROUND_TIME_TYPE::SEC_45:
		l_pString = "45秒";
		break;
	case ROUND_TIME_TYPE::SEC_60:
		l_pString = "60秒";
		break;
	case ROUND_TIME_TYPE::SEC_75:
		l_pString = "75秒";
		break;
	case ROUND_TIME_TYPE::SEC_99:
		l_pString = "99秒";
		break;
	case ROUND_TIME_TYPE::SEC_INFINITY:
		l_pString = "無制限";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void RoundTimeTypeParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundTimeType = (PlayerDataMgr->m_ConfigData.iRoundTimeType > (int)ROUND_TIME_TYPE::SEC_30)
			? PlayerDataMgr->m_ConfigData.iRoundTimeType - 1 : 3;	
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundTimeType = (PlayerDataMgr->m_ConfigData.iRoundTimeType < (int)ROUND_TIME_TYPE::SEC_INFINITY)
			? PlayerDataMgr->m_ConfigData.iRoundTimeType + 1 : 0;	
	}
}
