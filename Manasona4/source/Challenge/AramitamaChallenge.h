#pragma once
#include "BaseChallenge.h"

//+-----------------------------
//	1�Ԗڂ̐���
//+-----------------------------
class  AramitamaChallenge1 :public BaseChallenge
{
public:
	AramitamaChallenge1(int iTitleNo);
	~AramitamaChallenge1() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	2�Ԗڂ̐���
//+-----------------------------
class  AramitamaChallenge2 :public BaseChallenge
{
public:
	AramitamaChallenge2(int iTitleNo);
	~AramitamaChallenge2() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	3�Ԗڂ̐���
//+-----------------------------
class  AramitamaChallenge3 :public BaseChallenge
{
public:
	AramitamaChallenge3(int iTitleNo);
	~AramitamaChallenge3() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

