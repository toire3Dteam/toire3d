#pragma once
#include "../BaseParamSetting.h"

//+------------------------------------------
//	チャレンジセレクトのパラメーター設定
//+------------------------------------------

class ChallengeSelectParamSetting : public BaseParamSetting
{
public:
	ChallengeSelectParamSetting(int Number, int WidthSize);
	~ChallengeSelectParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


