#pragma once
#include "../BasePlayer.h"

class Airou : public BasePlayer
{
public:
	Airou(int deviceID);

private:
	void InitAttackDatas();	// 純粋仮想関数オーバーライド
};

