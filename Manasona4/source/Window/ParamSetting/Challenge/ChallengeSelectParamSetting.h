#pragma once
#include "../BaseParamSetting.h"

//+------------------------------------------
//	�`�������W�Z���N�g�̃p�����[�^�[�ݒ�
//+------------------------------------------

class ChallengeSelectParamSetting : public BaseParamSetting
{
public:
	ChallengeSelectParamSetting(int Number, int WidthSize);
	~ChallengeSelectParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


