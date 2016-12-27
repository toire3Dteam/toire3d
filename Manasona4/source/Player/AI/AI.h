#pragma once
#include "TDNLib.h"
#include "../BasePlayer.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../../BaseEntity/State/StateMachine.h"
#include "../../BaseEntity/State/state.h"
// �X�e�[�g
#include "AIState.h"

#include "Data\SelectData.h"

//+---------------------------------
//	AI
//+---------------------------------
class AI : public BaseGameEntity
{
public:
	AI(SIDE side, BasePlayer* myBasePlayer, AI_TYPE AIType = AI_TYPE::PRACTICE_LAND);
	~AI();

	void Control();
	void Update();
	void DoublePushUpdate();
	void UpdateTraining();

	// AI�̎�ނ�ς���
	void ChangeAIType(AI_TYPE type);

	// ��񉟂�����
	void ActionDoublePush(PLAYER_INPUT inputNo);

	bool HandleMessage(const Message & msg);


	//+--------------------------------
	//	�A�N�Z�T		
	//+--------------------------------
	StateMachine<AI>* GetFSM()const { return m_pStateMachine; }// ���X�e�[�g�}�V���̃A�N�Z�T�����
	BasePlayer* GetBasePlayer(){ return m_pMyBasePlayer; }

	void PushInputList(PLAYER_INPUT inputNo)
	{
		int flag = m_pMyBasePlayer->GetInputList(inputNo);	// ���݂̃C���v�b�g�̏�Ԃ��擾
		flag = (flag & 0x1) ? 1 : 3;						// �������u�Ԃ��������ςȂ����̕��� 
		m_pMyBasePlayer->SetInputList(inputNo, flag);		// �ݒ肵���C���v�b�g���v���C���[�ɐݒ�
		m_bPushFlag[(int)inputNo] = true;	// ���̃t���[�����͉����Ă�̂ŁA0�ɏ��������Ȃ��ł�������
	}

	BasePlayer* GetTargetPlayer(){ return m_pTargetPlayer; };		// �U������
	void SetTargetPlayer(BasePlayer* targetplayer){ m_pTargetPlayer = targetplayer; }

	// �p�����[�^�֘A �A�N�Z�T�[
	int GetChaseFrame(){ return m_iChaseFrame; }
	int GetWaitFrame(){ return m_iWaitFrame; }
	int GetGuardFrame(){ return m_iGuardFrame; }	
	int GetPracticeGuardFrame(){ return m_iPracticeGuardFrame; }
	bool GetPracticeGuardFlag(){ return m_bPracticeGuardFlag; }

	void SetChaseFrame(int rate){ m_iChaseFrame = rate; }
	void SetWaitFrame(int rate){ m_iWaitFrame = rate; }
	void SetGuardFrame(int rate){ m_iGuardFrame = rate; }
	void SetPracticeGuardFrame(int rate){ m_iPracticeGuardFrame = rate; }
	void SetPracticeGuardFlag(bool flag){ m_bPracticeGuardFlag = flag; }

	void AddChaseFrame(int add){ m_iChaseFrame += add; }
	void AddWaitFrame(int add){ m_iWaitFrame += add; }
	void AddGuardFrame(int add){ m_iGuardFrame += add; }
	void AddPracticeGuardFrame(int add){ m_iPracticeGuardFrame += add; }

	AI_TYPE GetAIType() { return m_eAIType; }

private:
	StateMachine<AI>* m_pStateMachine; // ��AI�X�e�[�g�}�V��
	BasePlayer* m_pMyBasePlayer;
	BasePlayer* m_pTargetPlayer;		// �U������

	bool m_bPushFlag[(int)PLAYER_INPUT::MAX];	// �{�^���������ꂽ�t���[��

	// AI�ɕK�v�ȃp�����[�^�[
	int m_iChaseFrame;		// ��r�ɒǂ������Ă鎞��
	int m_iWaitFrame;		// �����������Ă鎞��
	int m_iGuardFrame;		// ��x�̃K�[�h���Ă��鎞��


	AI_TYPE m_eAIType;		// AI�̃^�C�v

	// ���K���[�h�ŃK�[�h���Ă鎞��
	int m_iPracticeGuardFrame;
	bool m_bPracticeGuardFlag;
	int m_iPracticeAttackIntervalFrame;	//	�U���Ԋu�t���[��

	// ��񉟂��p�̕ϐ�
	bool m_bDoublePush;					// ��񉟂����߂͑��̓�񉟂����I���܂Ŏ�t�ł��Ȃ��B
	int m_iDoublePushFrame;				// 
	PLAYER_INPUT m_eDoublePushInput;	// 

	//+------------------------------------
	//	�g���[�j���O�p
	//+------------------------------------
	


	// �X�e�[�g���t�����h�N���X��
	friend class AIState::PracticeGlobal;
	friend class AIState::PracticeLand;
	friend class AIState::PracticeSquat;
	friend class AIState::PracticeJump;
	friend class AIState::PracticeAttack;
	friend class AIState::PracticeGuard;
	friend class AIState::Wait;
	friend class AIState::Chase;
	friend class AIState::Guard;
	friend class AIState::InterruptInvincible;

};
