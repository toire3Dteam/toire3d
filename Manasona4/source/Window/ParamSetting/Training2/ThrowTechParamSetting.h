#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	�Ȃ������̐ݒ�
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

