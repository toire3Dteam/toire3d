#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	Hp‚Ì‰ñ•œ‚Ìó‘Ôİ’è
//+----------------------------

class HpRecoveryParamSetting: public BaseParamSetting
{
public:
	HpRecoveryParamSetting(int Number, int WidthSize);
	~HpRecoveryParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



