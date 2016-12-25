#pragma once
#include "../BaseParamSetting.h"

//+--------------------------------
//	SPゲージのパラメーター設定
//+--------------------------------

class SpGageParamSetting : public BaseParamSetting
{
public:
	SpGageParamSetting(int Number, int WidthSize);
	~SpGageParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


