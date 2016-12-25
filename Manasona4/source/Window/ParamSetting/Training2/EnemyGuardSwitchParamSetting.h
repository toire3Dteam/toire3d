#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	ガードの切り替え設定
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

