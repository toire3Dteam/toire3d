#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1�Ԗڂ̐���
//+-----------------------------
class  TekiChallenge1 :public BaseChallenge
{
public:
	TekiChallenge1(int iTitleNo);
	~TekiChallenge1() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
class  TekiChallenge2 :public BaseChallenge
{
public:
	TekiChallenge2(int iTitleNo);
	~TekiChallenge2() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
class  TekiChallenge3 :public BaseChallenge
{
public:
	TekiChallenge3(int iTitleNo);
	~TekiChallenge3() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

