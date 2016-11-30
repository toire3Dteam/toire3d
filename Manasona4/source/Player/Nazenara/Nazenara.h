#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"


class Nazenara : public BasePlayer
{
public:
	Nazenara(SIDE side, const SideData &data);
	~Nazenara();
	void Update(PLAYER_UPDATE flag)override;
	void Render()override;
	void RenderDrive()override;

	/****************************/
	//	�L�����N�^�[�ŗL�̗l�X�ȃ|�W�V����
	/****************************/
	Vector3 GetCenterPos(){ return m_vPos + Vector3(0, 5, 0); };	 // �v���C���[�̐^�񒆂̏ꏊ
	Vector3 GetFlontPos() { return m_vPos + Vector3((m_dir == DIR::RIGHT) ? 5.0f : -5.0f, 5.0f, 0); };   // ���݂̑���̏ꏊ

	/****************************/
	// ���E���h�ׂ��̏�����
	/****************************/
	void Reset();

	/****************************/
	// �|�������̃L�������Ƃ̍X�V
	/****************************/
	void KakeaiInit();
	void KakeaiExit();
	void KakeaiUpdate();

	/****************************/
	//	�ʏ�O�i��
	/****************************/
	void ThirdRushInit();		// �X�L���������ɌĂяo��
	void ThirdRushExit();
	void ThirdRushUpdate();//

	/****************************/
	//	�L�����N�^�[�ŗL�X�L��
	/****************************/
	void SkillInit();
	void SkillExit();
	bool SkillUpdate();//

	class SkillAction
	{
	public:
		class Base
		{
		public:
			Base(Nazenara *pNazenara, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Nazenara *m_pNazenara;
			int m_OrgDamage;
			float m_fOrgComboRate;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];

			void SetOrgInfo(BASE_ACTION_STATE state);
		};

		class Land:public Base
		{
		public:
			Land(Nazenara *pNazenara);
			void Enter();
			bool Execute();
			void Exit();
		};

		class Squat :public Base
		{
		public:
			Squat(Nazenara *pNazenara);
			void Enter();
			bool Execute();
			void Exit();
		private:
		};

		class Aerial :public Base
		{
		public:
			Aerial(Nazenara *pNazenara);
			void Enter();
			bool Execute();
			void Exit();
		};

		class AerialDrop:public Base
		{
		public:
			AerialDrop(Nazenara *pNazenara);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_ActiveFrame;	// ���i�A��i�ƍU���𕪂���̂ŁA���Ԃ̌v����
		};
	};

	SkillAction::Base *m_pSkillActions[(int)SKILL_ACTION_TYPE::MAX];

	/****************************/
	//	�q�[�z�[�h���C�u
	/****************************/
	void HeavehoDriveInit();
	void HeavehoDriveExit();
	bool HeavehoDriveUpdate();
	void HeavehoDriveHitEvent();

	/****************************/
	//	�q�[�z�[�h���C�u_�I�[�o�[�t���[
	/****************************/
	void HeavehoDriveOverFlowInit();
	void HeavehoDriveOverFlowExit();
	void HeavehoDriveOverFlowUpdate();//
	void HeavehoDriveOverFlowSuccessInit();
	void HeavehoDriveOverFlowSuccessUpdate();

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

	// ���ŗL�̘r�P��
	iex3DObj *m_pArm;
	bool m_bArm;	// ���Ńt���O(true�Ȃ�X�V���ĕ`�悷��)
};
