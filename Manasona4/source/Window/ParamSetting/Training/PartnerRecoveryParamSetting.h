#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	パートナーゲージの設定
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

