#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	SE�̃p�����[�^�[�ݒ�
//+----------------------------

class SEParamSetting: public BaseParamSetting
{
public:
	SEParamSetting(int Number, int WidthSize);
	~SEParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



