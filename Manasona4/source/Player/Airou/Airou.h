#pragma once
#include "../AI\AI.h"
class AI;

class Airou : public BasePlayer
{
public:
	Airou(int deviceID,TEAM team, bool bAI);

	/****************************/
	//	�L�����N�^�[�ŗL�̗l�X�ȃ|�W�V����
	/****************************/
	virtual Vector3 GetCenterPos(){ return m_pos + Vector3(0, 5, 0); };	 // �v���C���[�̐^�񒆂̏ꏊ
	virtual Vector3 GetHoldPos() { return m_pos + Vector3((m_dir == DIR::RIGHT) ? 5.0f : -5.0f, 5.0f, 0); };   // ���݂̑���̏ꏊ


	/****************************/
	//	�L�����N�^�[�ŗL�X�L��
	/****************************/
	void SkillInit();
	void SkillExit();
	void SkillUpdate();//

	/****************************/
	//	�q�[�z�[�h���C�u
	/****************************/
	void HeavehoDriveInit();
	void HeavehoDriveExit();
	void HeavehoDriveUpdate();

	/****************************/
	//	�q�[�z�[�h���C�u_�I�[�o�[�t���[
	/****************************/
	void HeavehoDriveOverFlowInit();
	void HeavehoDriveOverFlowExit();
	void HeavehoDriveOverFlowUpdate();//
	void HeavehoDriveOverFlowSuccess();

	/****************************/
	//	AI
	/****************************/
	// ���ꂼ��̃L�����N�^�[����AI�ɂ��Z�̃{�^����ݒ�I
	void AIHighAtackButton()
	{
		//m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		//return PLAYER_INPUT::D;
	};
	// �U���n�X�L��
	void AIAtackSkillButton()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
	};

	// AI�̍U���͈�
	float AIAtackRange() { return 20; }
	float AIRunAtackRange() { return 35; }

private:
	// �������z�֐��I�[�o�[���C�h
	void InitActionDatas();
	void InitMotionDatas();

	// �A�C���[�h�����p�̕ϐ�
	Vector2 m_OrgFlyVector[2];
	int m_DrillFrame;
	int m_DrillHitCount;	// ���i�q�b�g�̃J�E���g
};

