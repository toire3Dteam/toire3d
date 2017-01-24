#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1番目の説明
//+-----------------------------
class  AramitamaChallenge1 :public BaseChallenge
{
public:
	AramitamaChallenge1(int iTitleNo);
	~AramitamaChallenge1() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	2番目の説明
//+-----------------------------
class  AramitamaChallenge2 :public BaseChallenge
{
public:
	AramitamaChallenge2(int iTitleNo);
	~AramitamaChallenge2() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	3番目の説明
//+-----------------------------
class  AramitamaChallenge3 :public BaseChallenge
{
public:
	AramitamaChallenge3(int iTitleNo);
	~AramitamaChallenge3() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

