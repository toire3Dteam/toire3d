#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	���E���h�̎��Ԃ̃p�����[�^�[�ݒ�
//+----------------------------

class RoundTimeTypeParamSetting: public BaseParamSetting
{
public:
	RoundTimeTypeParamSetting(int Number, int WidthSize);
	~RoundTimeTypeParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



