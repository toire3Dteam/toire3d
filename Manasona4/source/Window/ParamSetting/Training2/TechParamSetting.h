#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	�󂯐g�̐ݒ�
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


