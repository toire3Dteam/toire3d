#pragma once
#include "TDNLib.h"
#include "../BasePlayer.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../../BaseEntity/State/StateMachine.h"
#include "../../BaseEntity/State/state.h"

class AI : public BaseGameEntity
{
public:
	AI(int DeviceID, BasePlayer* myBasePlayer);
	~AI();

	void Control();
	void Update();

	bool HandleMessage(const Message & msg);

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

	void SetChaseFrame(int rate){ m_iChaseFrame = rate; }
	void SetWaitFrame(int rate){ m_iWaitFrame = rate; }

	void AddChaseFrame(int add){ m_iChaseFrame += add; }
	void AddWaitFrame(int add){ m_iWaitFrame += add; }


private:
	StateMachine<AI>* m_pStateMachine; // ��AI�X�e�[�g�}�V��
	BasePlayer* m_pMyBasePlayer;
	BasePlayer* m_pTargetPlayer;		// �U������

	bool m_bPushFlag[(int)PLAYER_INPUT::MAX];	// �{�^���������ꂽ�t���[��

	// AI�ɕK�v�ȃp�����[�^�[
	int m_iChaseFrame;		// ��r�ɒǂ������Ă鎞��

	int m_iWaitFrame;		// �����������Ă鎞��

};
