#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	ガードの状態設定
//+----------------------------

class EnemyGuardParamSetting : public BaseParamSetting
{
public:
	EnemyGuardParamSetting(int Number, int WidthSize);
	~EnemyGuardParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};



