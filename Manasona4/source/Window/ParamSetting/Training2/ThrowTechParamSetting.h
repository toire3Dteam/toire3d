#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	なげ抜けの設定
//+----------------------------

class ThrowTechParamSetting : public BaseParamSetting
{
public:
	ThrowTechParamSetting(int Number, int WidthSize);
	~ThrowTechParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};

