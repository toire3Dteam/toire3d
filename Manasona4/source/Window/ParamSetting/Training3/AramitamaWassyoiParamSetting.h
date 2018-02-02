#pragma once
#include "../BaseParamSetting.h"

//+------------------------------------
//	ïïàÛÇ≥ÇÍÇµç∞ÇÃÇÌÇ¡ÇµÇÂÇ¢ÉQÅ[ÉWÇÃê›íË
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

