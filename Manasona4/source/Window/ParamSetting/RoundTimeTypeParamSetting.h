#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	ラウンドの時間のパラメーター設定
//+----------------------------

class RoundTimeTypeParamSetting: public BaseParamSetting
{
public:
	RoundTimeTypeParamSetting(int Number, int WidthSize);
	~RoundTimeTypeParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



