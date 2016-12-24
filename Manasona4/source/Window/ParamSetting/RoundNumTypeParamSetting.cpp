#include "RoundNumTypeParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	ラウンド数のパラメーター設定
//+----------------------------

RoundNumTypeParamSetting::RoundNumTypeParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

RoundNumTypeParamSetting::~RoundNumTypeParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void RoundNumTypeParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void RoundNumTypeParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_ConfigData.iRoundNumType != (int)ROUND_NUM_TYPE::ROUND_1)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_ConfigData.iRoundNumType != (int)ROUND_NUM_TYPE::ROUND_4)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右


	// これは文字表記で描画

	// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((ROUND_NUM_TYPE)PlayerDataMgr->m_ConfigData.iRoundNumType)
	{
	case ROUND_NUM_TYPE::ROUND_1:
		l_pString = "1ラウンド先取";
		break;
	case ROUND_NUM_TYPE::ROUND_2:
		l_pString = "2ラウンド先取";
		break;
	case ROUND_NUM_TYPE::ROUND_3:
		l_pString = "3ラウンド先取";
		break;
	case ROUND_NUM_TYPE::ROUND_4:
		l_pString = "4ラウンド先取";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void RoundNumTypeParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundNumType = (PlayerDataMgr->m_ConfigData.iRoundNumType > (int)ROUND_NUM_TYPE::ROUND_1) ? PlayerDataMgr->m_ConfigData.iRoundNumType - 1 : 3;	// ★ 3という数字は0,1,2,3という事を想定した仮コード
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iRoundNumType = (PlayerDataMgr->m_ConfigData.iRoundNumType < (int)ROUND_NUM_TYPE::ROUND_4) ? PlayerDataMgr->m_ConfigData.iRoundNumType + 1 : 0;	// ★ 3という数字は0,1,2,3という事を想定した仮コード
	}
}
