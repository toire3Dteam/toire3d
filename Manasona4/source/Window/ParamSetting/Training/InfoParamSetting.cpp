#include "InfoParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	トレーニングの情報設定
//+----------------------------

InfoParamSetting::InfoParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

InfoParamSetting::~InfoParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void InfoParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void InfoParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_TrainingData.iInfo != (int)TRAINING_INFO_TYPE::NONE)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_TrainingData.iInfo != (int)TRAINING_INFO_TYPE::ALL)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右

	// これは文字表記で描画
	// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((TRAINING_INFO_TYPE)PlayerDataMgr->m_TrainingData.iInfo)
	{
	case TRAINING_INFO_TYPE::NONE:
		l_pString = "表示しない";
		break;
	case TRAINING_INFO_TYPE::DAMEGE:
		l_pString = "ダメージ表示";
		break;
	case TRAINING_INFO_TYPE::COMMAND:
		l_pString = "コマンド入力";
		break;
	case TRAINING_INFO_TYPE::ALL:
		l_pString = "両方";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void InfoParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iInfo = (PlayerDataMgr->m_TrainingData.iInfo > (int)TRAINING_INFO_TYPE::NONE)
			? PlayerDataMgr->m_TrainingData.iInfo - 1 : (int)TRAINING_INFO_TYPE::ALL;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iInfo = (PlayerDataMgr->m_TrainingData.iInfo < (int)TRAINING_INFO_TYPE::ALL)
			? PlayerDataMgr->m_TrainingData.iInfo + 1 : (int)TRAINING_INFO_TYPE::NONE;
	}
}
