#pragma once
#include "BaseParamSetting.h"

//+----------------------------
//	Voice�̃p�����[�^�[�ݒ�
//+----------------------------

class VoiceParamSetting: public BaseParamSetting
{
public:
	VoiceParamSetting(int Number, int WidthSize);
	~VoiceParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



