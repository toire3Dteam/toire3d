#pragma once
#include "../BaseWindow.h"
#include "../ParamSetting/DifficultyParamSetting.h"
#include "../ParamSetting/RoundNumTypeParamSetting.h"
#include "../ParamSetting/RoundTimeTypeParamSetting.h"

//+-------------------------------
//	兄貴ウィンドウ
//+-------------------------------

class AnikiWindow : public BaseWindow
{
public:
	AnikiWindow(Vector2 vPos);
	~AnikiWindow();

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
		INVINCIBLE,
		OVERDRIVE,
		OVERDRIVE2,
	};


private:
	static const int m_cSizeX = 640;

};
