#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	BGM�̃p�����[�^�[�ݒ�
//+----------------------------

class BGMParamSetting: public BaseParamSetting
{
public:
	BGMParamSetting(int Number, int WidthSize);
	~BGMParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



