#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	オートセーブのパラメーター設定
//+----------------------------

class AutoSaveParamSetting: public BaseParamSetting
{
public:
	AutoSaveParamSetting(int Number, int WidthSize);
	~AutoSaveParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



