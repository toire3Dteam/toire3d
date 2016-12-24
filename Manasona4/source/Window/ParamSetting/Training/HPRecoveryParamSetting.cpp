#include "DifficultyParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	CPU難易度のパラメーター設定
//+----------------------------

DifficultyParamSetting::DifficultyParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

DifficultyParamSetting::~DifficultyParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void DifficultyParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void DifficultyParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_ConfigData.iDifficultyAI != (int)AI_TYPE::CPU_EASY)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_ConfigData.iDifficultyAI != (int)AI_TYPE::CPU_YOKOE)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右


	// これは文字表記で描画

	// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((AI_TYPE)PlayerDataMgr->m_ConfigData.iDifficultyAI)
	{
	case AI_TYPE::CPU_EASY:
		l_pString = "EASY";
		break;
	case AI_TYPE::CPU_NORMAL:
		l_pString = "NORMAL";
		break;
	case AI_TYPE::CPU_HARD:
		l_pString = "HARD";
		break;
	case AI_TYPE::CPU_YOKOE:
		l_pString = "YOKOE";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);


	//  これは％表記で描画

	//// %のサイズ
	//float fPersentSize = (float)(m_iWidthSize*0.725f);

	//// 最大値と現在の値を割って割合を取得
	//float fPersent = (float)(PlayerDataMgr->m_ConfigData.iVoiceVolume) / (float)(100);
	//float fAns = fPersentSize *fPersent;
	//// ゲージの左淵
	//m_pPersentFrame->Render(x + 32 - 2, y, 2, 32, (selectFlag * 2) , 0, 2, 32);	
	//// 中身
	//for (int i = 0; i < (int)fPersentSize; i++)
	//{
	//	// 中身があれば塗りつぶす
	//	if (i < (int)fAns)
	//	{
	//		m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 0, 1, 32);
	//	}
	//	else
	//	{
	//		m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 32, 1, 32);
	//	}
	//}
	//// ゲージの右淵
	//m_pPersentFrame->Render(x + 32+ (int)fPersentSize, y, 2, 32, (selectFlag * 2), 0, 2, 32);

	//// 数値
	//RenderNumber(x + m_iWidthSize - 24, y, PlayerDataMgr->m_ConfigData.iDifficultyAI, selectFlag);

}


//	操作
void DifficultyParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iDifficultyAI = (PlayerDataMgr->m_ConfigData.iDifficultyAI > 0) ? PlayerDataMgr->m_ConfigData.iDifficultyAI - 1 : 3;	// ★ 3という数字は0,1,2,3という事を想定した仮コード
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iDifficultyAI = (PlayerDataMgr->m_ConfigData.iDifficultyAI < 3) ? PlayerDataMgr->m_ConfigData.iDifficultyAI + 1 : 0;	// ★ 3という数字は0,1,2,3という事を想定した仮コード
	}
}
