#include "SEParamSetting.h"
#include "Data\PlayerData.h"
#include "../../Sound/SoundManager.h"

//+----------------------------
//	SEのパラメーター設定
//+----------------------------

SEParamSetting::SEParamSetting(int Number, int WidthSize) :BaseParamSetting(Number, WidthSize)
{

}

SEParamSetting::~SEParamSetting()
{

}

//+----------------------------
//	更新・描画
//+----------------------------
void SEParamSetting::Update()
{
	BaseParamSetting::Update();
}

// ★各自様々な描画方法で描画　ユーザーに分かりやすく作る
void SEParamSetting::Render(int x, int y, bool selectFlag)
{
	//// 共通描画（矢印とか）
	//BaseParamSetting::Render(x, y, selectFlag);
	// 矢印描画
	if (PlayerDataMgr->m_ConfigData.iSEVolume > 0)
	m_pLeftArrow->Render(x, y, 32, 32, 0, (32 * selectFlag), 32, 32);	// 左
	if (PlayerDataMgr->m_ConfigData.iSEVolume < 100)
	m_pRightArrow->Render(x + m_iWidthSize, y, 32, 32, 32, (32 * selectFlag), 32, 32);	// 右

	//  これは％表記で描画
	
	// %のサイズ
	float fPersentSize = (float)(m_iWidthSize*0.725f);

	// 最大値と現在の値を割って割合を取得
	float fPersent = (float)(PlayerDataMgr->m_ConfigData.iSEVolume) / (float)(100);
	float fAns = fPersentSize *fPersent;


	// ゲージの左淵
	m_pPersentFrame->Render(x + 32 - 2, y, 2, 32, (selectFlag * 2) , 0, 2, 32);	
	// 中身
	for (int i = 0; i < (int)fPersentSize; i++)
	{
		// 中身があれば塗りつぶす
		if (i < (int)fAns)
		{
			m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 0, 1, 32);
		}
		else
		{
			m_pPersentGage->Render(x + 32 + i, y, 1, 32, selectFlag, 32, 1, 32);
		}
	}
	// ゲージの右淵
	m_pPersentFrame->Render(x + 32+ (int)fPersentSize, y, 2, 32, (selectFlag * 2), 0, 2, 32);

	// 数値
	RenderNumber(x + m_iWidthSize - 24, y, PlayerDataMgr->m_ConfigData.iSEVolume, selectFlag);

}


//	操作
void SEParamSetting::Ctrl(int DeviceID)
{
	// 基本操作
	BaseParamSetting::Ctrl(DeviceID);

	// ↑からフラグが設定されるのでそれを利用にして
	// ここからオリジナルの設定に

	// 左押してたら
	if (m_bLeftPush)
	{
		PlayerDataMgr->m_ConfigData.iSEVolume--;
	}
	// 右押してたら
	if (m_bRightPush)
	{
		PlayerDataMgr->m_ConfigData.iSEVolume++;
	}

	if (m_bRightPush || m_bLeftPush)
	{
		// 100を超えたら0へ 0より下げたら100へ
		if (PlayerDataMgr->m_ConfigData.iSEVolume > 100)PlayerDataMgr->m_ConfigData.iSEVolume = 0;
		if (PlayerDataMgr->m_ConfigData.iSEVolume < 0)PlayerDataMgr->m_ConfigData.iSEVolume = 100;

		// 設定
		se->SetBaseVolume(PlayerDataMgr->m_ConfigData.iSEVolume * 0.01f);
	}

	// ★100または0まで来たら一度完全に止めて、もう一度押し込むまでパラメータを変更できない
	if (PlayerDataMgr->m_ConfigData.iSEVolume == 100||
		PlayerDataMgr->m_ConfigData.iSEVolume == 0)
	{
		m_iLeftPushFrame = 0;
		m_iRightPushFrame = 0;
		m_iSpeedLv = 0;
	}

	// 0~100の間に止める
	//PlayerDataMgr->m_ConfigData.iSEVolume = min(100, max(0, PlayerDataMgr->m_ConfigData.iSEVolume));
}
