#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1�Ԗڂ̐���
//+-----------------------------
class  NazenarabaChallenge1 :public BaseChallenge
{
public:
	NazenarabaChallenge1(int iTitleNo);
	~NazenarabaChallenge1() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
class  NazenarabaChallenge2 :public BaseChallenge
{
public:
	NazenarabaChallenge2(int iTitleNo);
	~NazenarabaChallenge2() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
class  NazenarabaChallenge3 :public BaseChallenge
{
public:
	NazenarabaChallenge3(int iTitleNo);
	~NazenarabaChallenge3() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

