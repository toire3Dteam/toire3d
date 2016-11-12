#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"

// �O���錾
namespace OshiokiManager
{
	class Base;
}

class Airou : public BasePlayer
{
public:
	Airou(int deviceID, SIDE side, bool bAI);
	~Airou();
	void Update(bool bControl)override;
	void Render()override;
	void RenderDrive()override;

	/****************************/
	//	�L�����N�^�[�ŗL�̗l�X�ȃ|�W�V����
	/****************************/
	Vector3 GetCenterPos(){ return m_pos + Vector3(0, 5, 0); };	 // �v���C���[�̐^�񒆂̏ꏊ
	Vector3 GetFlontPos() { return m_pos + Vector3((m_dir == DIR::RIGHT) ? 5.0f : -5.0f, 5.0f, 0); };   // ���݂̑���̏ꏊ

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
			Base(Airou *pAirou, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Airou *m_pAirou;
			int m_OrgDamage;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];

			void SetOrgInfo(BASE_ACTION_STATE state);
		};

		class Land:public Base
		{
		public:
			Land(Airou *pAirou);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_DrillFrame;
			int m_DrillHitCount;	// ���i�q�b�g�̃J�E���g
			bool m_bHit;			// 1��ł�����������t���O
		};

		class Squat :public Base
		{
		public:
			Squat(Airou *pAirou);
			void Enter();
			bool Execute();
			void Exit(){}
		private:
		};

		class AerialDrop:public Base
		{
		public:
			AerialDrop(Airou *pAirou);
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

	// ���������p
	OshiokiManager::Base *m_pOshiokiMgr;
	bool m_bHeavehoHit;	// �A�C���[�̕K�E�����������Ƃ��p

	BaseUVEffect *m_pAirouEntryEffect;
	BaseUVEffect *m_pAirouEntryCircleEffect;
};

