#include "EnemyGuardSwitchParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	ガードの切り替え設定
//+----------------------------

EnemyGuardSwitchParamSetting::EnemyGuardSwitchParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

EnemyGuardSwitchParamSetting::~EnemyGuardSwitchParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void EnemyGuardSwitchParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void EnemyGuardSwitchParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch != (int)ENEMY_GUARD_SWITCH_TYPE::NO)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch != (int)ENEMY_GUARD_SWITCH_TYPE::OK)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右


																							// これは文字表記で描画

																							// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((ENEMY_GUARD_SWITCH_TYPE)PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch)
	{
	case ENEMY_GUARD_SWITCH_TYPE::NO:
		l_pString = "行わない";
		break;
	case ENEMY_GUARD_SWITCH_TYPE::OK:
		l_pString = "行う";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void EnemyGuardSwitchParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch = (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch > (int)ENEMY_GUARD_SWITCH_TYPE::NO)
			? PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch - 1 : (int)ENEMY_GUARD_SWITCH_TYPE::OK;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch = (PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch < (int)ENEMY_GUARD_SWITCH_TYPE::OK)
			? PlayerDataMgr->m_TrainingData.iEnemyGuardSwitch + 1 : (int)ENEMY_GUARD_SWITCH_TYPE::NO;
	}
}
