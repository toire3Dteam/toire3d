#pragma once
#include "TDNLIB.h"
#include "BaseEntity\Message\Message.h"

//+------------------------
// ���U���g�f�[�^
//+------------------------
struct ResultData
{
	SIDE eWinnerSide;		// ���҂̃T�C�h
	int iMaxDamage;			// �ő�_���[�W
	int iRemainingHP;		// �c��HP
	int iElapsedTime;		// �o�ߎ���

	// ������
	ResultData() :eWinnerSide(SIDE::LEFT), iMaxDamage(0), iRemainingHP(0), iElapsedTime(0) {}

};