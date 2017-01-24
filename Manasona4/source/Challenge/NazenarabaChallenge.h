#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1番目の説明
//+-----------------------------
class  NazenarabaChallenge1 :public BaseChallenge
{
public:
	NazenarabaChallenge1(int iTitleNo);
	~NazenarabaChallenge1() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	2番目の説明
//+-----------------------------
class  NazenarabaChallenge2 :public BaseChallenge
{
public:
	NazenarabaChallenge2(int iTitleNo);
	~NazenarabaChallenge2() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	3番目の説明
//+-----------------------------
class  NazenarabaChallenge3 :public BaseChallenge
{
public:
	NazenarabaChallenge3(int iTitleNo);
	~NazenarabaChallenge3() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

