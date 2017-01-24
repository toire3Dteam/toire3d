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

//+-----------------------------
//	2番目の説明
//+-----------------------------
class  TekiChallenge2 :public BaseChallenge
{
public:
	TekiChallenge2(int iTitleNo);
	~TekiChallenge2() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	3番目の説明
//+-----------------------------
class  TekiChallenge3 :public BaseChallenge
{
public:
	TekiChallenge3(int iTitleNo);
	~TekiChallenge3() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

