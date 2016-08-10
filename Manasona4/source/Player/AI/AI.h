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

private:
	StateMachine<AI>* m_pStateMachine; // ��AI�X�e�[�g�}�V��
	BasePlayer* m_pMyBasePlayer;
	BasePlayer* m_pTargetPlayer;		// �U������

	bool m_bPushFlag[(int)PLAYER_INPUT::MAX];	// �{�^���������ꂽ�t���[��

};
