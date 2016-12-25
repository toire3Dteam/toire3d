#include "ThrowTechParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	なげ抜けの設定
//+----------------------------

ThrowTechParamSetting::ThrowTechParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

ThrowTechParamSetting::~ThrowTechParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void ThrowTechParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void ThrowTechParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_TrainingData.iEnemyThrowTech != (int)ENEMY_THROW_TECH_TYPE::NO)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_TrainingData.iEnemyThrowTech != (int)ENEMY_THROW_TECH_TYPE::OK)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右

																							// これは文字表記で描画
																							// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((ENEMY_THROW_TECH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyThrowTech)
	{
	case ENEMY_THROW_TECH_TYPE::NO:
		l_pString = "行わない";
		break;
	case ENEMY_THROW_TECH_TYPE::OK:
		l_pString = "行う";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void ThrowTechParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyThrowTech = (PlayerDataMgr->m_TrainingData.iEnemyThrowTech > (int)ENEMY_THROW_TECH_TYPE::NO)
			? PlayerDataMgr->m_TrainingData.iEnemyThrowTech - 1 : (int)ENEMY_THROW_TECH_TYPE::OK;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyThrowTech = (PlayerDataMgr->m_TrainingData.iEnemyThrowTech < (int)ENEMY_THROW_TECH_TYPE::OK)
			? PlayerDataMgr->m_TrainingData.iEnemyThrowTech + 1 : (int)ENEMY_THROW_TECH_TYPE::NO;
	}
}
