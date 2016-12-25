#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	トレーニングの情報設定
//+----------------------------

class InfoParamSetting : public BaseParamSetting
{
public:
	InfoParamSetting(int Number, int WidthSize);
	~InfoParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


