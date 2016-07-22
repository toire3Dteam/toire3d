#pragma once
#include "../BasePlayer.h"

class Airou : public BasePlayer
{
public:
	Airou(int deviceID);

private:
	void InitActionDatas();	// 純粋仮想関数オーバーライド
};

