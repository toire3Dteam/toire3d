#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	ラウンド数のパラメーター設定
//+----------------------------

class RoundNumTypeParamSetting: public BaseParamSetting
{
public:
	RoundNumTypeParamSetting(int Number, int WidthSize);
	~RoundNumTypeParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



