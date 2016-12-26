#include "EnemyStateParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	敵の状態の設定
//+----------------------------

EnemyStateParamSetting::EnemyStateParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

EnemyStateParamSetting::~EnemyStateParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void EnemyStateParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void EnemyStateParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_TrainingData.iEnemyState != (int)ENEMY_STATE_TYPE::STAND)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_TrainingData.iEnemyState != (int)ENEMY_STATE_TYPE::COM)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右

																							// これは文字表記で描画
																							// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((ENEMY_STATE_TYPE)PlayerDataMgr->m_TrainingData.iEnemyState)
	{
	case ENEMY_STATE_TYPE::STAND:
		l_pString = "立ち";
		break;
	case ENEMY_STATE_TYPE::SQUAT:
		l_pString = "しゃがみ";
		break;
	case ENEMY_STATE_TYPE::JUMP:
		l_pString = "ジャンプ";
		break;
	case ENEMY_STATE_TYPE::ATTACK:
		l_pString = "攻撃し続ける";
		break;
	case ENEMY_STATE_TYPE::COM:
		l_pString = "COM";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void EnemyStateParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyState = (PlayerDataMgr->m_TrainingData.iEnemyState > (int)ENEMY_STATE_TYPE::STAND)
			? PlayerDataMgr->m_TrainingData.iEnemyState - 1 : (int)ENEMY_STATE_TYPE::COM;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iEnemyState = (PlayerDataMgr->m_TrainingData.iEnemyState < (int)ENEMY_STATE_TYPE::COM)
			? PlayerDataMgr->m_TrainingData.iEnemyState + 1 : (int)ENEMY_STATE_TYPE::STAND;
	}
}
