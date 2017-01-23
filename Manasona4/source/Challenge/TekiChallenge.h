#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1番目の説明
//+-----------------------------
class  TekiChallenge1 :public BaseChallenge
{
public:
	TekiChallenge1(int iTitleNo);
	~TekiChallenge1() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};
