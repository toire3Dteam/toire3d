#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	ƒK[ƒh‚ÌØ‚è‘Ö‚¦İ’è
//+----------------------------

class EnemyGuardSwitchParamSetting : public BaseParamSetting
{
public:
	EnemyGuardSwitchParamSetting(int Number, int WidthSize);
	~EnemyGuardSwitchParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};

