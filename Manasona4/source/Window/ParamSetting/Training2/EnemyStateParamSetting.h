#pragma once
#include "../BaseParamSetting.h"

//+----------------------------
//	“G‚Ìó‘Ô‚Ìİ’è
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

