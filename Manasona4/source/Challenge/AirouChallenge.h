#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1�Ԗڂ̐���
//+-----------------------------
class  AirouChallenge1 :public BaseChallenge
{
public:
	AirouChallenge1(int iTitleNo);
	~AirouChallenge1() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
class  AirouChallenge2 :public BaseChallenge
{
public:
	AirouChallenge2(int iTitleNo);
	~AirouChallenge2() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
class  AirouChallenge3 :public BaseChallenge
{
public:
	AirouChallenge3(int iTitleNo);
	~AirouChallenge3() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};
