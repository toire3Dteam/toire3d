#pragma once
#include "../BaseWindow.h"
#include "../ParamSetting/DifficultyParamSetting.h"
#include "../ParamSetting/RoundNumTypeParamSetting.h"
#include "../ParamSetting/RoundTimeTypeParamSetting.h"

//+-------------------------------
//	封印されし魂ウィンドウ
//+-------------------------------

class AramitamaWindow : public BaseWindow
{
public:
	AramitamaWindow(Vector2 vPos);
	~AramitamaWindow();

	bool Update();	// 戻り値で起動しているか確認　他の制御を止める為に使います
	void Redner();	// 描画

	bool Ctrl(int DeviceID);    // 操作

	void Action();	// Windowを出す
	void Stop();	// 止める

	// オプション用選択状態
	enum OPTION_STATE
	{
		SKILL_1,
		SKILL_2,
		SKILL_3,
		SKILL_4,
		SKILL_5,
		SKILL_6,
		SKILL_7,
		INVINCIBLE,
		OVERDRIVE,
	};


private:
	static const int m_cSizeX = 640;

};
