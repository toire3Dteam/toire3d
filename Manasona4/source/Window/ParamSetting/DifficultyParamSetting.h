#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	CPU難易度のパラメーター設定
//+----------------------------

class DifficultyParamSetting: public BaseParamSetting
{
public:
	DifficultyParamSetting(int Number, int WidthSize);
	~DifficultyParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



