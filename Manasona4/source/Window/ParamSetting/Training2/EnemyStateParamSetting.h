#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	敵の状態の設定
//+----------------------------

class EnemyStateParamSetting : public BaseParamSetting
{
public:
	EnemyStateParamSetting(int Number, int WidthSize);
	~EnemyStateParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};

