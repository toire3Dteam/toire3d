#pragma once

#include "Aniki.h"
#include "../BasePlayer.h"
#include "../../Sound/SoundManager.h"

//*************************************************************
//	ケツドラム
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
		Aniki *m_pAniki;				// このケツドラムを使うプレイヤーのポインタ
		BasePlayer *m_pTarget;		// 攻撃対象のポインタ
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

			// ケツドラム終了
			if (m_iCurrentFrame > c_iENDTIME)
			{
				if (l_ciMotionFrame == c_iMOTIONFRAME + 3)
				{
					// プレイヤーにメッセージ送信
					HIT_DAMAGE_INFO DamageInfo;
					DamageInfo.fComboRate = 1;
					DamageInfo.bOverDrive = true;

					DamageInfo.BeInvincible = true;		// 無敵になるかどうか
					DamageInfo.damage = 514;				// ダメージ(スコア)
					DamageInfo.FlyVector.Set(0, 0);			// 吹っ飛びベクトル
					DamageInfo.iHitStopFrame = 1;			// ヒットストップ
					DamageInfo.HitRecoveryFrame = 40;		// 硬直時間
					DamageInfo.HitEffectType = (int)EFFECT_TYPE::MULTIPLE_HIT_BLOW;			// この攻撃のヒットエフェクトを相手に送る
					DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;			// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
					DamageInfo.HitSE = "ケツドラム1";
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

			// 判定フレーム
			if (l_ciMotionFrame == c_iMOTIONFRAME + 3)
			{
				// プレイヤーにメッセージ送信
				HIT_DAMAGE_INFO DamageInfo;
				DamageInfo.fComboRate = 1;
				DamageInfo.bOverDrive = true;

				DamageInfo.BeInvincible = false;		// 無敵になるかどうか
				DamageInfo.damage = 69;					// ダメージ(スコア)
				DamageInfo.FlyVector.Set(0, 0);			// 吹っ飛びベクトル
				DamageInfo.iHitStopFrame = 1;			// ヒットストップ
				DamageInfo.HitRecoveryFrame = 114514;	// 硬直時間
				DamageInfo.HitEffectType = (int)EFFECT_TYPE::MULTIPLE_HIT_BLOW;			// この攻撃のヒットエフェクトを相手に送る
				DamageInfo.iAttackType = (int)BASE_ACTION_STATE::HEAVEHO_DRIVE;			// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
				DamageInfo.HitSE = "ケツドラム1";
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