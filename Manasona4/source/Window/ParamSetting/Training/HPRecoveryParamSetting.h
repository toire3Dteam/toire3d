#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	Hp�̉񕜂̏�Ԑݒ�
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



