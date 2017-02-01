#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"

// �O���錾
namespace Ketsudram{class Base;}
class Aniki : public BasePlayer
{
public:
	Aniki(SIDE side, const SideData &data);
	~Aniki();
	void Update(PLAYER_UPDATE flag)override;
	void Render()override;
	void RenderDrive()override;

	/****************************/
	//	�L�����N�^�[�ŗL�̗l�X�ȃ|�W�V����
	/****************************/
	Vector3 GetCenterPos() { return m_vPos + Vector3(0, 5, 0); };	 // �v���C���[�̐^�񒆂̏ꏊ
	Vector3 GetFlontPos() { return m_vPos + Vector3((m_dir == DIR::RIGHT) ? 6.0f : -6.0f, 5.0f, 0); };   // ���݂̑���̏ꏊ

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
	//	������
	/****************************/
	void DownAttackInit()
	{
		BasePlayer::DownAttackInit();

		// �ړ�
		SetMove(Vector3(GetDirVecX() * 1, 1, 0));
	}

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
			Base(Aniki *pAniki, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Aniki *m_pAniki;
			int m_OrgDamage;
			float m_fOrgComboRate;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];
			LPCSTR m_lpcOrgHitSE;
			int m_iOrgRecoveryFrame;
		};

		class Land :public Base
		{
		public:
			Land(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitCount;
		};

		class Squat :public Base
		{
		public:
			Squat(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
		};

		// �n��͂񂾌�
		class Land2 :public Base
		{
		public:
			Land2(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitFrame;
			bool m_bKetudoramed;
		};

		// �΋�͂񂾌�
		class Land3 :public Base
		{
		public:
			Land3(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitFrame;
			bool m_bKetudoramed;
		};

		class Aerial :public Base
		{
		public:
			Aerial(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_ActiveFrame;	// ���i�A��i�ƍU���𕪂���̂ŁA���Ԃ̌v����
		};

		// ���n�ɂ�����
		class AerialDrop :public Base
		{
		public:
			AerialDrop(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		};
	};

	SkillAction::Base *m_pSkillActions[(int)SKILL_ACTION_TYPE::MAX];

	class HeaveHoAction
	{
	public:
		class Base
		{
		public:
			Base(Aniki *pAniki) :m_pAniki(pAniki){}
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Aniki *m_pAniki;
		};

		class Strike:public Base
		{
		public:
			Strike(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		};

		class Throw:public Base
		{
		public:
			Throw(Aniki *pAniki);
			void Enter();
			bool Execute();
			void Exit();
		};
	};

	HeaveHoAction::Base *m_pHeaveHoActions[2];

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
	void AIHighAttackButton()
	{
		//m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		//return PLAYER_INPUT::D;
	};
	// �U���n�X�L��
	void AIAttackSkillButton()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
	};

	// �ߋ����X�L��
	bool AINearSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// �������X�L��
	bool AIMiddleSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// �������X�L��
	bool AIFarSkill()
	{
		return false;	// ���̃L�����N�^�[�ɉ������Z�͂Ȃ�
	};

	// �󒆋ߋ����X�L��
	bool AIAerialNearSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// AI�̍U���͈�
	float AIAttackRange() { return 15; }
	float AIRunAttackRange() { return 27; }
	float AIThrowRange() { return 6; }

	/****************************/
	//	 �J���[
	/****************************/
	void ChangeColor();

private:
	// �������z�֐��I�[�o�[���C�h
	void InitActionDatas();
	void InitMotionDatas();

	// �P�c�h�����p
	Ketsudram::Base *m_pKetsudram;
	bool m_bHeavehoHit;	// �Z�M�̕K�E�����������Ƃ��p

	void Ketsudram()
	{
		m_pDefaultObj->SetFrame(462);
	}
};

