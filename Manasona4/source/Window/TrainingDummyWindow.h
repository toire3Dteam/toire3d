#pragma once
#include "BaseWindow.h"
#include "ParamSetting\Training2\EnemyGuardParamSetting.h"
#include "ParamSetting\Training2\EnemyGuardSwitchParamSetting.h"
#include "ParamSetting\Training2\EnemyStateParamSetting.h"
#include "ParamSetting\Training2\TechParamSetting.h"
#include "ParamSetting\Training2\ThrowTechParamSetting.h"
#include "ParamSetting\Training2\EnemyLvParamSetting.h"

//+----------------------------------
//	トレーニングダミーウィンドウ
//+----------------------------------

class TrainingDummyWindow : public BaseWindow
{
public:
	TrainingDummyWindow(Vector2 vPos);
	~TrainingDummyWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

					// パラメータ設定用に
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);


	// 選択状態
	enum TRAINING_DUMMY_STATE
	{
		ENEMY_STATE,
		ENEMY_LV,
		ENEMY_GUARD,
		ENEMY_GUARD_SWITCH,
		ENEMY_TECH,
		ENEMY_THROW_TECH,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 832;

	EnemyStateParamSetting*				m_pEStateParam;
	EnemyLvParamSetting*				m_pELvParam;
	EnemyGuardParamSetting*				m_pEGuardParam;
	EnemyGuardSwitchParamSetting*		m_pEGuardSwitchParam;
	TechParamSetting*					m_pETechParam;
	ThrowTechParamSetting*				m_pEThrowTechParam;

};
