#include "TutorialSelectParamSetting.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "MenuUI\TutorialManager.h"

//+----------------------------------------------
//	チュートリアルセレクトのパラメーター設定
//+----------------------------------------------

TutorialSelectParamSetting::TutorialSelectParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

TutorialSelectParamSetting::~TutorialSelectParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void TutorialSelectParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void TutorialSelectParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (SelectDataMgr->Get()->iTutorialType != (int)TUTORIAL_TYPE::WALK)
		m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (SelectDataMgr->Get()->iTutorialType != (int)TUTORIAL_TYPE::ARRAY_END - 1)
		m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右


	// これは文字表記で描画
	// 文字の色
	DWORD l_dCol = 0xff4e84e6;
	if (selectFlag == true)l_dCol = 0xff030a58;

	tdnFont::RenderStringCentering(TutorialMgr->GetTitleString(SelectDataMgr->Get()->iTutorialType).c_str(), "HGｺﾞｼｯｸE", 24, x + 24 + (int)(m_iWidthSize / 2), y + 4, l_dCol, RS::COPY);

}


//	操作
void TutorialSelectParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		if ((SelectDataMgr->Get()->iTutorialType > (int)TUTORIAL_TYPE::WALK))
		{
			//TutorialMgr->BackStep();
			SelectDataMgr->Get()->iTutorialType--;
		}
		else
		{
			//TutorialMgr->SetSelectType((TUTORIAL_TYPE)((int)TUTORIAL_TYPE::ARRAY_END - 1));			
			SelectDataMgr->Get()->iTutorialType = (int)TUTORIAL_TYPE::ARRAY_END - 1;
		}
			
	}
	// 右押してたら
	if (m_bRightPush)
	{
		if ((SelectDataMgr->Get()->iTutorialType < (int)TUTORIAL_TYPE::ARRAY_END - 1))
		{
			//TutorialMgr->NectStep();
			SelectDataMgr->Get()->iTutorialType++;
		}
		else
		{
			//TutorialMgr->SetSelectType((TUTORIAL_TYPE)((int)TUTORIAL_TYPE::WALK));
			SelectDataMgr->Get()->iTutorialType = 0;
		}

	}
}
