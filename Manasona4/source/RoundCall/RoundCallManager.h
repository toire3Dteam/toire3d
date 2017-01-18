#pragma once
#include "TDNLIB.h"
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "../BaseEntity/Entity/EntityEnum.h"
#include "RoundCall.h"

//+-----------------------------
//	���E���h�R�[���}�l�[�W���[
//+-----------------------------
enum class ROUND_TYPE
{
	FIRST,
	REBEL,
	ACTION,
	FINISH,
	OVERDRIVE_FINISH,
	TIME_UP,
	DRAW,
	ARRAY_END
};

class RoundCallManager : public BaseGameEntity
{
public:


public:
	RoundCallManager();
	~RoundCallManager();

	void Update();
	void Render();

	// �R�[���Ăяo���֐�
	void CallFirstGameRound();
	void CallRound(int roundNum);
	void CallFinish(ENTITY_ID WinnerID);
	void CallOverDriveFinish(ENTITY_ID WinnerID);
	void CallTimeUp(ENTITY_ID WinnerID);
	void CallDraw();

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool HandleMessage(const Message& msg);


private:
	void Action(ROUND_TYPE type, int delayTimer = 0);
	void RebelAction(int roundNum, int delayTimer = 0);


	// �l�X�ȃ��E���h�R�[��
	BaseRoundCall* m_pRoundCall[(int)ROUND_TYPE::ARRAY_END];
	
	// �f�B���C�^�C�}�[
	int m_iDelayTimer[(int)ROUND_TYPE::ARRAY_END];


};
