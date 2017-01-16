#include "AramitamaWassyoiParamSetting.h"
#include "Data/PlayerData.h"
#include "Data/SelectData.h"

//+----------------------------
//	敵の状態の設定
//+----------------------------

AramitamaWassyoiParamSetting::AramitamaWassyoiParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

AramitamaWassyoiParamSetting::~AramitamaWassyoiParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void AramitamaWassyoiParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void AramitamaWassyoiParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_TrainingData.iWassyoiGage != (int)WASSYOI_TYPE::NO)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_TrainingData.iWassyoiGage != (int)WASSYOI_TYPE::MAX)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右

																							// これは文字表記で描画
																							// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	LPCSTR l_pString = "なし";

	switch ((WASSYOI_TYPE)PlayerDataMgr->m_TrainingData.iWassyoiGage)
	{
	case WASSYOI_TYPE::NO:
		l_pString = "デフォルト";
		break;
	case WASSYOI_TYPE::MAX:
		l_pString = "MAX固定";
		break;
	default:
		break;
	}
	tdnFont::RenderStringCentering(l_pString, "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void AramitamaWassyoiParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_TrainingData.iWassyoiGage = (PlayerDataMgr->m_TrainingData.iWassyoiGage > (int)WASSYOI_TYPE::NO)
			? PlayerDataMgr->m_TrainingData.iWassyoiGage - 1 : (int)WASSYOI_TYPE::MAX;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_TrainingData.iWassyoiGage = (PlayerDataMgr->m_TrainingData.iWassyoiGage < (int)WASSYOI_TYPE::MAX)
			? PlayerDataMgr->m_TrainingData.iWassyoiGage + 1 : (int)WASSYOI_TYPE::NO;
	}
}
