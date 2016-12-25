#pragma once
#include "../BaseParamSetting.h"

//+--------------------------------
//	2pのHP最大値のパラメーター設定
//+--------------------------------

class Hp2pParamSetting: public BaseParamSetting
{
public:
	Hp2pParamSetting(int Number, int WidthSize);
	~Hp2pParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



