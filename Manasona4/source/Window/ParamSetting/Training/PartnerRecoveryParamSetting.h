#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	CPU��Փx�̃p�����[�^�[�ݒ�
//+----------------------------

class DifficultyParamSetting: public BaseParamSetting
{
public:
	DifficultyParamSetting(int Number, int WidthSize);
	~DifficultyParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



