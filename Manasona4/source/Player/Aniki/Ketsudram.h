#pragma once

#include "Aniki.h"
#include "../BasePlayer.h"
#include "../../Sound/SoundManager.h"

//*************************************************************
//	�P�c�h����
namespace Ketsudram
{
	class Base
	{
	public:
		Base(Aniki *pAniki, BasePlayer *pTarget) :m_pAniki(pAniki), m_pTarget(pTarget){}
		virtual ~Base(){}
		virtual void Update() = 0;
		void Action(){ m_bAction = true; }
		bool isAction(){ return m_bAction; }
	protected:
		Aniki *m_pAniki;				// ���̃P�c�h�������g���v���C���[�̃|�C���^
		BasePlayer *m_pTarget;		// �U���Ώۂ̃|�C���^
		bool m_bAction;
	};

	class OverDrive : public Base
	{
	private:
		static const int c_iENDTIME = 200;
		static const int c_iMOTIONFRAME = 462;
		iex3DObj *m_pRefObj;
		int m_iCurrentFrame;
	public:
		OverDrive(Aniki *pAniki, BasePlayer *pTarget) :Base(pAniki, pTarget), m_pRefObj(m_pAniki->GetDefaultObj()), m_iCurrentFrame(0)
		{
			pTarget->SetAngleY(DIR_ANGLE[(int)pAniki->GetDir()]);
		}
		void Update()
		{
			if (!m_bAction) return;

			const int l_ciMotionFrame(m_pRefObj->GetFrame());

			// �P�c�h�����I��
			if (m_iCurrentFrame > c_iENDTIME)
			{
				if (l_ciMotionFrame == c_iMOTIONFRAME + 3)
				{
					// �v���C���[�Ƀ��b�Z�[�W���M
					HIT_DAMAGE_INFO DamageInfo;
					DamageInfo.fComboRate = 1;
					DamageInfo.bOverDrive = true;

					DamageInfo.BeInvincible = true;		// ���G�ɂȂ邩�ǂ���
					DamageInfo.damage = 514;				// �_���[�W(�X�R�A)
					DamageInfo.FlyVector.Set(0, 0);			// ������уx�N�g��
					DamageInfo.iHitStopFrame = 1;			// �q�b�g�X�g�b�v
					DamageInfo.HitRecoveryFrame = 40;		// �d������
					DamageInfo.HitEffectType = (int)EFFECT_TYPE::MULTIPLE_HIT_BLOW;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
					DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;			// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
					DamageInfo.HitSE = "�P�c�h����1";
					DamageInfo.bFinishOK = true;
					DamageInfo.DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
					DamageInfo.fRepeatAttackRate = 1;

					MsgMgr->Dispatch(0, m_pAniki->GetID(), m_pTarget->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &DamageInfo);
				}
				else if (l_ciMotionFrame > c_iMOTIONFRAME + 19)
				{
					m_bAction = false;
				}
				return;
			}


			if (m_iCurrentFrame++ == c_iENDTIME)
			{
				m_pRefObj->SetFrame(c_iMOTIONFRAME);
				return;
			}

			// ����t���[��
			if (l_ciMotionFrame == c_iMOTIONFRAME + 3)
			{
				// �v���C���[�Ƀ��b�Z�[�W���M
				HIT_DAMAGE_INFO DamageInfo;
				DamageInfo.fComboRate = 1;
				DamageInfo.bOverDrive = true;

				DamageInfo.BeInvincible = false;		// ���G�ɂȂ邩�ǂ���
				DamageInfo.damage = 69;					// �_���[�W(�X�R�A)
				DamageInfo.FlyVector.Set(0, 0);			// ������уx�N�g��
				DamageInfo.iHitStopFrame = 1;			// �q�b�g�X�g�b�v
				DamageInfo.HitRecoveryFrame = 114514;	// �d������
				DamageInfo.HitEffectType = (int)EFFECT_TYPE::MULTIPLE_HIT_BLOW;			// ���̍U���̃q�b�g�G�t�F�N�g�𑊎�ɑ���
				DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;			// ���̍U�����̃^�C�v(�R���{���ɓ����U�����g�킹�Ȃ��悤��)
				DamageInfo.HitSE = "�P�c�h����1";
				DamageInfo.bFinishOK = false;
				DamageInfo.DamageMotion = DAMAGE_MOTION::KNOCK_DOWN;
				DamageInfo.fRepeatAttackRate = 1;

				MsgMgr->Dispatch(0, m_pAniki->GetID(), m_pTarget->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &DamageInfo);
			}
			if (l_ciMotionFrame > c_iMOTIONFRAME + 8 || l_ciMotionFrame < c_iMOTIONFRAME)
			{
				//if (m_pAniki->isPushInputTRG(PLAYER_COMMAND_BIT::A, false) || m_pAniki->isPushInputTRG(PLAYER_COMMAND_BIT::B, false) || m_pAniki->isPushInputTRG(PLAYER_COMMAND_BIT::C, false) || m_pAniki->isPushInputTRG(PLAYER_COMMAND_BIT::D, false))
				{
					m_pRefObj->SetFrame(c_iMOTIONFRAME);
				}
			}
		}
	};

	class OverFlow :public Base
	{
	public:
		OverFlow(Aniki *pAniki, BasePlayer *pTarget, bool bTsukisasaru, int ID = 0) :Base(pAniki, pTarget)
		{

		}
		~OverFlow()
		{

		}
		void Update()
		{

		}
	};
}