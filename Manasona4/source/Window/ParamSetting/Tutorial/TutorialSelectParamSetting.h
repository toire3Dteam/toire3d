#pragma once
#include "../BaseParamSetting.h"

//+------------------------------------------
//	�`���[�g���A���Z���N�g�̃p�����[�^�[�ݒ�
//+------------------------------------------

class TutorialSelectParamSetting : public BaseParamSetting
{
public:
	TutorialSelectParamSetting(int Number, int WidthSize);
	~TutorialSelectParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


