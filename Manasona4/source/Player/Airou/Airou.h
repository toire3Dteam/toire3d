#pragma once
#include "../AI\AI.h"
class AI;

class Airou : public BasePlayer
{
public:
	Airou(int deviceID,TEAM team, bool bAI);

	/****************************/
	//	�L�����N�^�[�ŗL�X�L��
	/****************************/
	void SkillUpdate();//

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
	void InitActionDatas();	// �������z�֐��I�[�o�[���C�h



};

