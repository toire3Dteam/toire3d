#pragma once
#include "../AI\AI.h"
#include "../BasePlayer.h"

class Aramitama : public BasePlayer
{
public:
	Aramitama(SIDE side, const SideData &data);
	~Aramitama();
	void Update(PLAYER_UPDATE flag)override;
	void Render()override;
	void RenderDrive()override;

	/****************************/
	//	�L�����N�^�[�ŗL�̗l�X�ȃ|�W�V����
	/****************************/
	Vector3 GetCenterPos(){ return m_vPos + Vector3(0, 7, 0); };	 // �v���C���[�̐^�񒆂̏ꏊ
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
			Base(Aramitama *pAramitama, BASE_ACTION_STATE state);
			virtual void Enter() = 0;
			virtual bool Execute() = 0;
			virtual void Exit() = 0;
		protected:
			Aramitama *m_pAramitama;
			int m_iOrgHitStopFrame[2];
			int m_iOrgHitRecoveryFrame[2];
			int m_OrgDamage;
			float m_fOrgComboRate;
			CollisionShape::Square m_OrgHitSquare;
			Vector2 m_OrgFlyVector[2];

			void SetOrgInfo(BASE_ACTION_STATE state);
		};

		class Land:public Base
		{
		public:
			Land(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
			int m_iHitCount;
		};

		class Squat :public Base
		{
		public:
			Squat(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
		};

		class Aerial :public Base
		{
		public:
			Aerial(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
			bool m_bMoved;
			bool m_bHitCheck;
		};

		class AerialDrop :public Base
		{
		public:
			AerialDrop(Aramitama *pAramitama);
			void Enter();
			bool Execute();
			void Exit();
		private:
			bool m_bMoved;
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
	//	�L�����N�^�[�ʂ̃f�[�^
	/****************************/
	void SetCharacterData(void* desk)override;
	void CharacterDataReset()override;			//	�ʏ��������
	void CharacterDataUIRender()override;	// ��UI�`��
	void CharacterDataUIAction(int iDelayTimer)override;	// ��UI�N��

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
		m_pAI->PushInputList(PLAYER_INPUT::L2);
		return true;
	};

	// �������X�L��
	bool AIMiddleSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// �������X�L��
	bool AIFarSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::DOWN);
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// �󒆋ߋ����X�L��
	bool AIAerialNearSkill()
	{
		m_pAI->PushInputList(PLAYER_INPUT::D);
		return true;
	};

	// AI�̍U���͈�
	float AIAttackRange() { return 14; }
	float AIRunAttackRange() { return 27; }
	float AIThrowRange() { return 6; }

	/****************************/
	//	 �J���[
	/****************************/
	void ChangeColor();

	static const int c_WASSYOIGAUGE_MAX;			// �Q�[�W�ő�l=���󔭓�����

	
	//AramitamaShotEffect* GetShotEffect() { return m_pAShot; }

private:
	// �������z�֐��I�[�o�[���C�h
	void InitActionDatas();
	void InitMotionDatas();

	// �G�t�F�N�g
	AramitamaChargeWaveEffect*	m_pAChargeWave;		// �`���[�W�̃E�F�[�u
	AramitamaChargeAuraEffect*	m_pAChargeAura;		// �`���[�W�̃I�[��					
	AramitamaCircleEffect*		m_pACircle;			// �T�[�N��
	AramitamaCanonEffect*		m_pACanon;			// �K�E�L���m��
	AramitamaNozzleFlashEffect* m_pANozzleFlash;	// �K�E�}�Y���t���b�V��
	AramitamaSoulAuraEffect*	m_pASoulAura;		// ���I�[��
	AramitamaShotEffect*		m_pAShot;			// �V���b�g
	AramitamaRippleEffect*		m_pARipple;			// �g��
	float m_fCircle;								// �T�[�N����

	// ����
	bool m_bWassyoi;
	int m_iWassyoiGauge;
	// �Q�[�W�̊G
	tdn2DAnim* m_pWassyoiGage;
	tdn2DAnim* m_pWassyoiFrame;
	tdn2DAnim* m_pWassyoiGageRip;

	void AddWassyoiGauge(int iAdd);

	void AddWassyoiGauge(bool bHitSuccess, int iAdd = c_WASSYOIGAUGE_MAX / 8)
	{
		// �Q�[�W������(12.5%)
		AddWassyoiGauge(bHitSuccess ? iAdd * 2 : iAdd);
	}

	enum class MUSHI_TYPE
	{
		LAND, SQUAT, AERIAL, MAX
	};
	class MushiData
	{
	public:
		AttackData *tagpAttackData;

		MushiData(LPSTR filepath, int iCoolTime) :iCoolTime(0), c_COOL_TIME(iCoolTime), pBullet(nullptr), tagpAttackData(nullptr)
		{
			pBullet = new iex3DObj(filepath);
			pBullet->SetMotion(0);
			tagpAttackData = new AttackData;
		}
		~MushiData()
		{
			SAFE_DELETE(pBullet);
			SAFE_DELETE(tagpAttackData);
		}
		void Update()
		{
			// �N�[���^�C���X�V
			if (iCoolTime > 0) iCoolTime--;
		}

		void Action(Aramitama *pAramitama, MUSHI_TYPE type);
	private:
		int iCoolTime;
		const int c_COOL_TIME;
		iex3DObj *pBullet;
	};
	MushiData *m_pMushi[(int)MUSHI_TYPE::MAX];

	void InitMushiDatas();
};

