#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1番目の説明
//+-----------------------------
class  AirouChallenge1 :public BaseChallenge
{
public:
	AirouChallenge1(int iTitleNo);
	~AirouChallenge1() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	2番目の説明
//+-----------------------------
class  AirouChallenge2 :public BaseChallenge
{
public:
	AirouChallenge2(int iTitleNo);
	~AirouChallenge2() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	3番目の説明
//+-----------------------------
class  AirouChallenge3 :public BaseChallenge
{
public:
	AirouChallenge3(int iTitleNo);
	~AirouChallenge3() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};
