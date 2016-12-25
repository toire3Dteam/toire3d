#pragma once
#include "BaseWindow.h"
#include "ParamSetting\Training\Hp1PParamSetting.h"
#include "ParamSetting\Training\Hp2PParamSetting.h"
#include "ParamSetting\Training\HPRecoveryParamSetting.h"
#include "ParamSetting\Training\InfoParamSetting.h"
#include "ParamSetting\Training\PartnerRecoveryParamSetting.h"
#include "ParamSetting\Training\SpGageParamSetting.h"


//+----------------------------------
//	トレーニングオプションウィンドウ
//+----------------------------------

class TrainingOptionWindow : public BaseWindow
{
public:
	TrainingOptionWindow(Vector2 vPos);
	~TrainingOptionWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

					// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);


	// 選択状態
	enum TRAINING_OPTION_STATE
	{
		HP_RECOVERY,
		MAXHP_1P,
		MAXHP_2P,
		SP_GAGE,
		PARTNER_GAGE,
		INFO,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 832;

	Hp1pParamSetting*				m_pHp1PParam;
	Hp2pParamSetting*				m_pHp2PParam;
	HpRecoveryParamSetting*			m_pHpRecoveryParam;
	InfoParamSetting*				m_pInfoParam;
	PartnerRecoveryParamSetting*	m_pPartnerRecoveryParam;
	SpGageParamSetting*				m_pSpGageParam;


};
