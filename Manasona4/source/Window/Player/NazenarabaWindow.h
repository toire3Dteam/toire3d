#pragma once
#include "../BaseWindow.h"
#include "../ParamSetting/DifficultyParamSetting.h"
#include "../ParamSetting/RoundNumTypeParamSetting.h"
#include "../ParamSetting/RoundTimeTypeParamSetting.h"

//+-------------------------------
//	アイル―ウィンドウ
//+-------------------------------

class NazenarabaWindow : public BaseWindow
{
public:
	NazenarabaWindow(Vector2 vPos);
	~NazenarabaWindow();

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
		INVINCIBLE,
		OVERDRIVE,
	};


private:
	static const int m_cSizeX = 640;

};
