#pragma once

#include "../Player/BasePlayer.h"


namespace Stand
{
	class Base
	{
	public:
		Base(BasePlayer *pPlayer);
		virtual ~Base();
		virtual void Update(bool bControl);
		virtual void Render(tdnShader* shader,char* name);

		// �v���C���[���y���\�i�{�^����������Ăяo��
		virtual void Action(SKILL_ACTION_TYPE type);

		// �y���\�i�u���C�N
		void Break();

		// ���������U�����Ă��邩�ǂ����ƁA�K�[�h���Ȃ��悤�ɂ������X�^���h�ւ̃I�[�o�[���C�h�����˂Ă���
		virtual bool isAttackFrame()
		{
			// �U������o�Ă�
			return (m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
		}
		//virtual bool isActiveBeforeFrom()
		//{
		//	FRAME_STATE eState = m_ActionFrameList[(int)m_ActionType][m_CurrentActionFrame];
		//	return (eState == FRAME_STATE::ACTIVE_BEFORE || eState == FRAME_STATE::ACTIVE);
		//}

		// ���A�b�^�J�C�����K�[�h���Ȃ��悤�ɂ��邽�߂ɁA�X�^���h���ƂɃK�[�h����X�^���h��
		virtual bool isCanGuardStand() = 0;

		bool isActive(){ return m_bActive; }
		bool isHit()
		{
			// �A�^�b�N�f�[�^�����������Ȃ�
			if (!m_pAttackData[(int)m_ActionType]) return false;
			return m_pAttackData[(int)m_ActionType]->bHit;
		}
		AttackData *GetAttackData(){ return m_pAttackData[(int)m_ActionType]; }
		DIR GetDir(){ return m_dir; }
		Vector3 &GetPos(){ return m_pos; }
		int GetDeviceID(){ return m_pPlayer->GetDeviceID(); }
		BasePlayer *GetPlayer(){ return m_pPlayer; }

		int GetStandGageMAX(){ return m_standGageMAX; };
		int GetStandGage(){ return m_standGage; };
		void SetStandGage(int gage){  m_standGage = gage; }
		int GetStandStockMAX(){ return m_standStockMAX; };
		int GetStandStock(){ return m_standStock; };
		void SetStandStock(int stock){ m_standStock= stock; }

		// �A�C�R���̃A�N�Z�T
		tdn2DAnim* GetIcon() { return m_pIcon; }
		tdn2DAnim* GetIconRip() { return m_pIconRip; }

		//�p�[�g�i�[�̎��
		PARTNER GetPartnerType() { return m_ePartnerType; }

		// ���S�̍��W
		virtual Vector3 GetCenterPos() { return m_pos + Vector3(0, 7.5f, 0); };



	protected:
		BasePlayer *m_pPlayer;	// �����̎��̂������Ă���v���C���[�ւ̃|�C���^
		iex3DObj *m_pObj;		// 3D����
		Vector3 m_pos;			// ���W
		Vector3 m_move;			// �ړ��l
		DIR m_dir;				// ����
		bool m_bActive;			// �o�Ă邩�ǂ���
		float m_fScale;			// �g��k��

		// �G�t�F�N�g
		SummonEffect* m_pSummonEffect;
		SummonWaveEffect* m_pSummonWaveEffect;
		ExitEffect*	  m_pExitEffect;

		// �A�C�R��
		tdn2DAnim* m_pIcon;
		tdn2DAnim* m_pIconRip;

		// �o����y���\�i
		int m_standStockMAX;	// �X�^���h�X�g�b�N�ő吔
		int m_standStock;		// �X�^���h�X�g�b�N��
		int m_standGageMAX;		// �X�^���h�Q�[�W�ő�l
		int m_standGage;		// �X�^���h�Q�[�W


		FRAME_STATE m_ActionFrameList[(int)SKILL_ACTION_TYPE::MAX][FRAME_MAX * 2];// ���X�^���h�����t���[������
		SKILL_ACTION_TYPE m_ActionType;	// �A�N�V�����̎��(���̍��̓Y�����̒��g)
		int m_CurrentActionFrame;	// �A�N�V�����t���[�����X�g�̒����Đ����Ă���t���[��(�����̉E�̓Y�����̒��g)

		void LoadActionFrameList(char *filename);

		AttackData *m_pAttackData[(int)SKILL_ACTION_TYPE::MAX];	// �U���f�[�^

		PARTNER m_ePartnerType;

	private:
		void StandGageUpdate();


	};

	/**************************/
	//		���R�C
	class Mokoi : public Base
	{
	public:
		Mokoi(BasePlayer *pPlayer);
		//~Mokoi();
		void Update(bool bControl);
		// �������z�I�[�o�[���C�h
		void Action(SKILL_ACTION_TYPE type);
		bool isCanGuardStand() { return true; }	// �K�[�h�ł���X�^���h
	};

	/**************************/
	//		�}�[��
	class Maya : public Base
	{
	public:
		Maya(BasePlayer *pPlayer);
		~Maya();
		void Update(bool bControl);
		// �������z�I�[�o�[���C�h
		void Action(SKILL_ACTION_TYPE type);
		bool isAttackFrame(){ return false; }	// �}�[���͔���Ȃ�
		bool isCanGuardStand() { return false; }	// �K�[�h�ł��Ȃ��X�^���h
	private:
		BaseUVEffect *m_pBullet;	// ��ѓ���̃��b�V���̎���(�ʂɎQ�Ƃ�����)
		BaseUVEffect *m_pBullet2;	// ��ѓ���̃��b�V���̎���(�ʂɎQ�Ƃ�����)

	};

	/**************************/
	//		�w�e
	class Hete : public Base
	{
	public:
		Hete(BasePlayer *pPlayer);
		//~Mokoi();
		void Update(bool bControl);

		// �������z�I�[�o�[���C�h
		void Action(SKILL_ACTION_TYPE type);
		bool isCanGuardStand() { return true; }	// �K�[�h�ł���X�^���h
	};


	/**************************/
	//		�A�b�^�J�C
	class Attakai : public Base
	{
	public:
		Attakai(BasePlayer *pPlayer);
		~Attakai();
		void Update(bool bControl);
		void Render(tdnShader* shader, char* name);

		// �������z�I�[�o�[���C�h
		void Action(SKILL_ACTION_TYPE type);
		bool isCanGuardStand() { return false; }	// �K�[�h�ł��Ȃ��X�^���h
	private:
		BasePanelEffect* m_pBomEf;
		BasePanelEffect* m_pBomAddEf;
		float m_fDangerRate;
		int m_iActiveStartFrame;
		int m_iSoundFrame;						// �`�J�`�JSE�p�̃t���[��
		static const int c_SE_INTERVAL = 90;	// �`�J�`�JSE�̊Ԋu

	};
}