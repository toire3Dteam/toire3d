#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	�p�[�g�i�[�Q�[�W�̐ݒ�
//+----------------------------

class PartnerRecoveryParamSetting : public BaseParamSetting
{
public:
	PartnerRecoveryParamSetting(int Number, int WidthSize);
	~PartnerRecoveryParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};

