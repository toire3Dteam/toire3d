#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	���E���h���̃p�����[�^�[�ݒ�
//+----------------------------

class RoundNumTypeParamSetting: public BaseParamSetting
{
public:
	RoundNumTypeParamSetting(int Number, int WidthSize);
	~RoundNumTypeParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



