#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	BGMのパラメーター設定
//+----------------------------

class BGMParamSetting: public BaseParamSetting
{
public:
	BGMParamSetting(int Number, int WidthSize);
	~BGMParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



