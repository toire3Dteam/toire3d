#pragma once
#include "../BaseParamSetting.h"

//+--------------------------------
//	2p��HP�ő�l�̃p�����[�^�[�ݒ�
//+--------------------------------

class Hp2pParamSetting: public BaseParamSetting
{
public:
	Hp2pParamSetting(int Number, int WidthSize);
	~Hp2pParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



