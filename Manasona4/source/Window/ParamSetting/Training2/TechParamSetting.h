#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	受け身の設定
//+----------------------------

class TechParamSetting : public BaseParamSetting
{
public:
	TechParamSetting(int Number, int WidthSize);
	~TechParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


