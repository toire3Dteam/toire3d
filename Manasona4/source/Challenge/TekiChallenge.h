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
