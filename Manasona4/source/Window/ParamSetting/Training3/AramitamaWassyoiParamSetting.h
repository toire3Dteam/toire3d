#pragma once
#include "../BaseParamSetting.h"

//+------------------------------------
//	���󂳂ꂵ���̂�����傢�Q�[�W�̐ݒ�
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

