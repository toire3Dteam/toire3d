#pragma once
#include "../BaseParamSetting.h"

//+--------------------------------
//	1p��HP�ő�l�̃p�����[�^�[�ݒ�
//+--------------------------------

class Hp1pParamSetting : public BaseParamSetting
{
public:
	Hp1pParamSetting(int Number, int WidthSize);
	~Hp1pParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


