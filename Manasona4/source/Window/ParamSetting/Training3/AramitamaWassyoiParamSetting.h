#pragma once
#include "../BaseParamSetting.h"

//+------------------------------------
//	封印されし魂のわっしょいゲージの設定
//+------------------------------------

class AramitamaWassyoiParamSetting : public BaseParamSetting
{
public:
	AramitamaWassyoiParamSetting(int Number, int WidthSize);
	~AramitamaWassyoiParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};

