#pragma once
#include "../BaseParamSetting.h"

//+--------------------------------
//	COM�̃��x���̃p�����[�^�[�ݒ�
//+--------------------------------

class EnemyLvParamSetting : public BaseParamSetting
{
public:
	EnemyLvParamSetting(int Number, int WidthSize);
	~EnemyLvParamSetting();

	void Update();
	void Render(int x, int y, bool selectFlag);
	void Ctrl(int DeviceID);

private:


};


