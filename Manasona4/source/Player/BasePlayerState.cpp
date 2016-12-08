#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "PlayerManager.h"
#include "../Cutin/CutIn.h"
#include "../Camera/camera.h"

// �萔
static const float HUTTOBI_LINE = 3.0f;
static const float REPEAT_ATTACK_RATE = 0.85f;		// �����U��������ȃ��[�g

#define GUARD_ON	// �K�[�h����

/*******************************************************/
//	�t�@���N�V����
/*******************************************************/
bool isNeutralPOV(BasePlayer *pPerson)
{
	return (!pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT) && !pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) && !pPerson->isPushInput(PLAYER_COMMAND_BIT::UP) && !pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN));
}

void ChangeWaitState(BasePlayer *pPerson)
{
	// ���L�[�������Ă���Ȃ�A�ҋ@�X�e�[�g�ł͂Ȃ��A���Ⴊ�݃X�e�[�g�ɍs��
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());

	// ����ȊO�Ȃ�ҋ@�X�e�[�g�ɖ߂�
	else pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
}

bool JumpCancel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�{�^���ŃW�����v�L�����Z��
	//============================================
	// �n�ʂɂ��Ă�����
	if (pPerson->isLand() == true)
	{
		// �������ςȂ��ŃL�����Z��
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::UP))
		{
			// �W�����v�X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
			return true;
		}
	}
	else if (pPerson->isAerialJump() == true)	// �󒆂ŋ󒆃W�����v�̌����������
	{
		// ���W�����v���̂݃g���K�[
		if (
			//pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3 ||	// �{�^���ŃW�����v�𕕈�
			pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::UP, true))
		{
			// ��s���̓��Z�b�g
			pPerson->AheadCommandReset();

			// �󒆃W�����v�X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			return true;
		}
	}

	return false;
}

bool isDamageCounterState(BasePlayer *pPerson)
{
	return (
		pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::Escape::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance())
		);
}

bool isDamageCounterStatePrev(BasePlayer *pPerson)
{
	return (
		pPerson->GetFSM()->isPrevState(*BasePlayerState::RushAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::Escape::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::DokkoiAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::DownAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::AerialAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::Skill::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::AntiAirAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::FinishAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::HeavehoDrive::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::SquatAttack::GetInstance())
		);
}

bool isThrownState(BasePlayer *pPerson)
{
	return (
		pPerson->isLand() &&	// �n��ɂ���

		// �_���[�W�n�̃X�e�[�g�ȊO��������
		!pPerson->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) &&
		!pPerson->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) &&
		!pPerson->GetFSM()->isInState(*BasePlayerState::LandRecovery::GetInstance())
		);
}

bool RunCancel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	���E�L�[�ő���L�����Z��
	//============================================
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT, true))
	{
		//pPerson->SetDir(DIR::LEFT);
		if (pPerson->GetDir() == DIR::LEFT)
		{
			// ��s���̓��Z�b�g
			pPerson->AheadCommandReset();

			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
			return true;
		}
	}
	else if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::RIGHT, true))
	{
		//pPerson->SetDir(DIR::RIGHT);
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			// ��s���̓��Z�b�g
			pPerson->AheadCommandReset();

			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
			return true;
		}
	}
	return false;
}

bool SquatCancel(BasePlayer * pPerson)
{
	// ���Ⴊ�݃L�����Z��
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		return true;
	}
	else return false;
}

bool RushAttackCancel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (isNeutralPOV(pPerson) && pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && !pPerson->GetFSM()->isPrevState(*BasePlayerState::RushAttack::GetInstance()))
	{
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
		return true;
	}
	else return false;
}

bool SquatAttackCancel(BasePlayer * pPerson)
{
	// ���Ⴊ�ݍU���L�����Z��
	if ((pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true)) && pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && !pPerson->GetFSM()->isPrevState(*BasePlayerState::SquatAttack::GetInstance()))
	{
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
		return true;
	}
	else return false;
}

bool AntiAirCancel(BasePlayer *pPerson)
{
	// �΋�U���L�����Z��
	if ((pPerson->isPushInput((pPerson->GetDir() == DIR::LEFT) ? PLAYER_COMMAND_BIT::RIGHT : PLAYER_COMMAND_BIT::LEFT, true)) && pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && !pPerson->GetFSM()->isPrevState(*BasePlayerState::AntiAirAttack::GetInstance()))
	{
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::AntiAirAttack::GetInstance());
		return true;
	}
	else return false;
}

bool StandCancel(BasePlayer * pPerson)
{
	// �X�^���h�L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::L1, true))
	{
		// �ǋL�@�X�^���h�X�g�b�N���Ȃ��Ƃ��̓L�����Z���ł��Ȃ�
		if (pPerson->GetStand()->GetStandStock() <= 0)return false;

		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::StandAction::GetInstance());
		return true;
	}
	else return false;
}

bool AttackCancel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
	{
		if (pPerson->isLand() == true)
		{
			// 4����
			if (pPerson->isPushInput((pPerson->GetDir() == DIR::LEFT) ? PLAYER_COMMAND_BIT::RIGHT : PLAYER_COMMAND_BIT::LEFT, true))
			{
				// �΋�U��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AntiAirAttack::GetInstance());
			}
			// 2����
			else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true))
			{
				// ���Ⴊ�݉��i
				pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
			}
			else
			{
				// �ʏ�
				pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
			}
		}
		else 
		{
			// ��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		return true;
	}
	else return false;
}

bool InvincibleAttackCancel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::L2, true))
	{
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::FinishAttack::GetInstance());
		return true;
	}
	else return false;
}

bool SkillCancel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::D, true))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Skill::GetInstance());
		return true;
	}
	else return false;
}

bool OverDriveCancel(BasePlayer *pPerson)
{
	if ((pPerson->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()) == true && pPerson->GetActionFrame() != FRAME_STATE::FOLLOW) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::ThrowSuccess::GetInstance()) == true ||
		pPerson->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true ||// �����Ă�Ƃ��͊���
		pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) == true)
		return false;

	// [1205] �͂܂ꂽ��E�K�E�Z���󂯂Ă�Ƃ��͔������Ȃ�
	if (pPerson->isNotOverDrive() == true)return false;


	// �o���i�o�[�X�g�j�L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R3, true))
	{
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		// �Q�[�W�����܂��Ă���
		if (pPerson->GetOverDriveGage() == BasePlayer::c_OVERDRIVE_MAX_GAGE)
		{
			// �U�߂�1More�����̃o�[�X�g���𔻒f
			if (pPerson->GetRecoveryFrame() <= 0)
			{
				// �U�߂̊o��
				pPerson->GetFSM()->ChangeState(BasePlayerState::OverDrive_OneMore::GetInstance());
				return true;
			}
			else
			{
				// ���̃o�[�X�g
				pPerson->GetFSM()->ChangeState(BasePlayerState::OverDrive_Burst::GetInstance());
				return true;
			}

		}
		else return false;
	}
	else return false;
}

bool EscapeCancel(BasePlayer *pPerson)
{
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::B, true))
	{
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		// �G�X�P�[�v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return true;
	}
	else return false;

	//if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	//pPerson->SetDir(DIR::LEFT);// ������ς���
	//	// �G�X�P�[�v�X�e�[�g�ɍs��
	//	//if (pPerson->GetDir() == DIR::LEFT)
	//	{
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
	//		return true;
	//	}
	//}
	//else if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	//pPerson->SetDir(DIR::RIGHT);// ������ς���
	//	// �G�X�P�[�v�X�e�[�g�ɍs��
	//	//if (pPerson->GetDir() == DIR::RIGHT)
	//	{
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
	//		return true;
	//	}
	//}
	//return false;
}

bool ThrowCancel(BasePlayer * pPerson)
{
	// �����L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::A))
	{
		// �n�ʂɂ��Ă��Ȃ��ƂȂ��Ƃ��̓L�����Z���ł��Ȃ�
		if (pPerson->isLand() == false)return false;
		pPerson->GetFSM()->ChangeState(BasePlayerState::Throw::GetInstance());
		return true;
	}
	else return false;
}

//bool GuardCancel(BasePlayer * pPerson)
//{
//	// �K�[�h�L�����Z��
//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::R1))
//	{
//		// �n�ʂɂ��Ă��Ȃ��ƂȂ��Ƃ��̓L�����Z���ł��Ȃ�
//		if (pPerson->isLand() == false)return false;
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());
//		return true;
//	}
//	else return false;
//}

bool HeaveHoCancel(BasePlayer * pPerson)
{
	// �K�E�Z�L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R2))
	{	
		// �I�[�o�[�h���C�u���Ȃ�Q�[�W�Ȃ��őłĂ�
		if (pPerson->isOverDrive())
		{
			// �I�[�o�[�h���C�u���I�����Ĕ���
			pPerson->SetOverDriveFrame(0);

		}else
		{
			// �܂��Q�[�W��50�����邩�Ȃ���
			if (pPerson->GetOverDriveGage() < 50)return false;

			// 50%����
			pPerson->AddOverDriveGage(-50);
		}

		// �K�E�X�e�[�g��
		pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDrive::GetInstance());
		return true;
	}
	else return false;
}

bool HeaveHoOverFlowCancel(BasePlayer * pPerson)
{
	if (pPerson->isOverDrive() == false)return false;

	// ���K�E�Z�L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R3))
	{
		if ((pPerson->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()) == true && pPerson->GetActionFrame() != FRAME_STATE::FOLLOW) ||
			pPerson->GetFSM()->isInState(*BasePlayerState::ThrowSuccess::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true || 
			pPerson->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true || 
			pPerson->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDriveOverFlow::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_Burst::GetInstance()) == true// �o�[�X�g��������
			)// �����Ă�Ƃ��͊���
			return false;

		pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDriveOverFlow::GetInstance());
		return true;
	}
	else return false;
}

bool DashCancel(BasePlayer * pPerson)
{
	// �_�b�V���L�����Z��
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::RIGHT, 12))
	{
		// ���������Ȃ瑖��
		if (pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		
		// �t�����Ȃ�o�N�X�e
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());

		return true;
	}
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::LEFT, 12))
	{
		// ���������Ȃ瑖��
		if (pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());

		// �t�����Ȃ�o�N�X�e
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());

		return true;
	}
	
	return false;
}

bool AerialDashCancel(BasePlayer *pPerson)
{
	// �󒆃_�b�V���L�����Z��
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::RIGHT, 12))
	{
		// ���������Ȃ瑖��
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDash::GetInstance());
		}
		// �t�����Ȃ�o�N�X�e
		else pPerson->GetFSM()->ChangeState(BasePlayerState::AerialBackDash::GetInstance());

		return true;
	}
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::LEFT, 12))
	{
		// ���������Ȃ瑖��
		if (pPerson->GetDir() == DIR::LEFT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDash::GetInstance());
		}

		// �t�����Ȃ�o�N�X�e
		else pPerson->GetFSM()->ChangeState(BasePlayerState::AerialBackDash::GetInstance());

		return true;
	}

	return false;
}

bool AerialAttackCancel(BasePlayer *pPerson)
{
	// �U���{�^�����͂ŋ󒆍U��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C))
	{
		// �󉺕���
		//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN) == true)
		//{
		//	// �󒆉��U���X�e�[�g�ɍs��
		//	pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
		//}
		//else
		{
			// �󒆍U���X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		return true;
	}

	return false;
}

bool DokkoiAttackCancel(BasePlayer *pPerson)
{
	// �ǂ������U���L�����Z��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R1, true))
	{
		if (pPerson->isLand() == true)
		{
			if (/*pPerson->isSquat() == false*/
				pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true))
			{
				// ���Ⴊ��
				pPerson->GetFSM()->ChangeState(BasePlayerState::DownAttack::GetInstance());
			}
			else
			{
				// �ʏ�
				pPerson->GetFSM()->ChangeState(BasePlayerState::DokkoiAttack::GetInstance());
			}

		}
		else
		{
			// ��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();

		return true;
	}
	else return false;

}

bool isInputGuardCommand(BasePlayer *pPerson)
{
	bool bGuardCommand(false);

	// ����ɑ΂������(�Ⴆ�ΉE�����ɑ����ĂĂ����ɑ��肪����Ȃ炻��͍�����)
	if (pPerson->GetTargetDir() == DIR::LEFT)
	{
		// ����ɑ΂��Ĕ��΂̕�����͂��Ă���
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT)) bGuardCommand = true;
	}
	// ����
	else
	{
		// ����ɑ΂��Ĕ��΂̕�����͂��Ă���
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT)) bGuardCommand = true;
	}

	return bGuardCommand;
}

void GuardUpdate(BasePlayer *pPerson)
{
#ifdef GUARD_ON
	// �K�[�h�R�}���h����͂��Ă���
	if (isInputGuardCommand(pPerson))
	{
		// �����肪��������̍U�������Ă�����A�K�[�h�X�e�[�g�Ɉڍs
		if ((pPerson->GetTargetPlayer()->isAttackState() || (pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData())) &&															// ���肪�U�����
			Math::Length(pPerson->GetPos(),pPerson->GetTargetPlayer()->GetPos()) < BasePlayer::c_GUARD_DISTANCE)	// �K�[�h��������
			pPerson->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());

		//// ���Ⴊ�݃K�[�h
		//if (pPerson->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()))
		//{
		//	// �K�[�h�X�e�[�g��ݒ肷��
		//	pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
		//
		//	// ���肪��������̍U�������Ă�����A�K�[�h���[�V�������Ƃ�
		//	if (pPerson->GetTargetPlayer()->isAttackState())
		//	{
		//		// TODO:���[�V�����߂����ǂ�����
		//		pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);
		//	}
		//}
		//
		//// �����K�[�h
		//else
		//{
		//	// �K�[�h�X�e�[�g��ݒ肷��
		//	pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
		//
		//	// ���肪��������̍U�������Ă�����A�K�[�h���[�V�������Ƃ�
		//	if (pPerson->GetTargetPlayer()->isAttackState())
		//	{
		//		// TODO:���[�V�����߂����ǂ�����
		//		pPerson->SetMotion(MOTION_TYPE::UP_GUARD);
		//	}
		//}
	}

	// �K�[�h�R�}���h����͂��Ă��Ȃ��̂ŁA�K�[�h���ĂȂ��X�e�[�g��ݒ�
	else
#endif
	{
		pPerson->SetGuardState(GUARD_STATE::NO_GUARD);
	}

}

bool isPossibleGuardState(BasePlayer *pPerson)
{
	// �K�[�h���ł���X�e�[�g���ǂ���
	StateMachine<BasePlayer> *pState(pPerson->GetFSM());
	bool bGuardState(pState->isInState(*BasePlayerState::Wait::GetInstance()) ||
		pState->isInState(*BasePlayerState::BackWalk::GetInstance()) ||
		pState->isInState(*BasePlayerState::Run::GetInstance()) ||
		//pState->isInState(*BasePlayerState::AerialJump::GetInstance()) ||
		//pState->isInState(*BasePlayerState::Fall::GetInstance()) ||
		pState->isInState(*BasePlayerState::Squat::GetInstance()));
	
	//if (!bGuardState)
	//{
	//	if (pState->isInState(*BasePlayerState::Jump::GetInstance()))
	//	{
	//		// ���W�����v�̏u�Ԃ̃K�[�h�ł��Ȃ��t���[��
	//		if (!pPerson->GetJump()->bHold) bGuardState = true;
	//	}
	//}

	return bGuardState;
}

bool WalkCancel(BasePlayer *pPerson)
{
	//////////////////////////////////////////////
	//	�ړ��{�^��
	//============================================
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		//pPerson->SetDir(DIR::RIGHT);
		// �����X�e�[�g�ɂ���
		if (pPerson->GetDir() == DIR::RIGHT)pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		return true;
	}
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		//pPerson->SetDir(DIR::LEFT);
		// �����X�e�[�g�ɂ���
		if (pPerson->GetDir() == DIR::LEFT)pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		return true;
	}
	return false;
}

SKILL_ACTION_TYPE GetSkillActionType(BasePlayer *pPerson)
{
	if (!pPerson->isLand())
		return (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true)) ? SKILL_ACTION_TYPE::AERIALDROP : SKILL_ACTION_TYPE::AERIAL;
	else
		return (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true)) ? SKILL_ACTION_TYPE::SQUAT : SKILL_ACTION_TYPE::LAND;
}


/*******************************************************/
//					�O���[�o���X�e�[�g
/*******************************************************/

void BasePlayerState::Global::Enter(BasePlayer * pPerson)
{
}

void BasePlayerState::Global::Execute(BasePlayer * pPerson)
{
	////////////////////////////////////////////////////////////////
	//	�K�[�h�X�V����
	//==============================================================
	if (isPossibleGuardState(pPerson)) GuardUpdate(pPerson);

	////////////////////////////////////////////////////////////////
	//	�I�[�o�[�h���C�u���݂̂ł��钴�K�E�Z�@
	//==============================================================
	if (HeaveHoOverFlowCancel(pPerson)) return;

	////////////////////////////////////////////////////////////////
	//	�I�[�o�[�h���C�u�@�ǂ�����ł��o����ŋ��̃L�����Z�����[�g
	//==============================================================
	if (OverDriveCancel(pPerson)) return;
	


}

void BasePlayerState::Global::Exit(BasePlayer * pPerson)
{
}

void BasePlayerState::Global::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 610, 0xffffffff, "����[�΂邷�ā[��");
}

bool BasePlayerState::Global::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �͂܂ꂽ�惁�b�Z�[�W
	case MESSAGE_TYPE::BE_THROWN:
	{
									pPerson->SetDir((pPerson->GetTargetPlayer()->GetDir() == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT);	// �v�͒͂񂾑���̕�������
									pPerson->GetFSM()->ChangeState(ThrowBind::GetInstance());		// �������炢�X�e�[�g��
									break;
	}
		// �U����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
									 //// �����o�[�X�g���Ȃ�͂���
									 //if (pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive::GetInstance()) == true)
									 //{
									 //	 // �A�N�V�����t���[���������Ă���
									 //	 if (pPerson->isFrameAction())
									 //	 {			 
									 //		 return true;
									 //	 }
									 //}

									 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

									 // ���_���[�W��^�����l�ɑ���Ԃ�(�K�[�h���Ă��K�E����������o�O�𒼂����߂ɁA����HandleMessage���󂯎�����甭���Ƃ������ɂ���)
									 HIT_ATTACK_INFO HitAttackInfo;
									 HitAttackInfo.bOverDrive = HitDamageInfo->bOverDrive;
									 HitAttackInfo.HitScore = HitDamageInfo->damage;
									 HitAttackInfo.hitStopFlame = HitDamageInfo->hitStopFlame;
									 MsgMgr->Dispatch(0, msg.Receiver, msg.Sender, MESSAGE_TYPE::HIT_ATTACK, &HitAttackInfo);
									 //bool bGuardSuccess(false);
									 //if (pPerson->GetGuardState() == GUARD_STATE::UP_GUARD)
									 //{
										// // �����肪�����K�[�h���ĂāA���i�U������Ȃ�������K�[�h�����I
										// if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::DOWN_ATTACK) bGuardSuccess = true;
									 //}
									 //else if (pPerson->GetGuardState() == GUARD_STATE::DOWN_GUARD)
									 //{
										// // �����肪���Ⴊ�݃K�[�h���ĂāA��i�U������Ȃ�������K�[�h�����I
										// if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::UP_ATTACK) bGuardSuccess = true;
									 //}

									 //// ���K�[�h�������Ă���I�I�I
									 //if (bGuardSuccess)
									 //{
										// // (A���)�����ɃK�[�h��������SE�A�G�t�F�N�g
										// se->Play("�K�[�h");

										// // ���e��
										// if (pPerson->GetDir() == DIR::RIGHT)
										// {
										//	 pPerson->SetMove(Vector3(-0.5f, 0, 0));
										// }
										// else
										// {
										//	 pPerson->SetMove(Vector3(0.5f, 0, 0));
										// }

										// pPerson->GetComboUI()->Guard();// �K�[�h���ꂽ

										// // �K�[�h�X�e�[�g�ɍs��

										// // �ȍ~�̃_���[�W�������Ȃ�
										// return true;
									 //}
									 
									 // �_���[�WSE�̍Đ�
									 LPCSTR seID = HitDamageInfo->HitSE;
									 if (seID) se->Play((LPSTR)seID);

									 // �����G�t�F�N�g�ǉ�
									// Vector2 screenPos = Math::WorldToScreen(pPerson->GetPos());
									// NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, HitDamageInfo->damage, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

									 // �󒆃_�b�V������
									 pPerson->SetAerialDashFrame(0);

									 // �y���\�i�u���C�N
									 pPerson->GetStand()->Break();

									 // �A�N�V�����X�e�[�g�I�t(�܂茻�݂܂ł��������������������)
									 pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

									 // �������U�����ꂽ�����֌����悤��
									 if (HitDamageInfo->FlyVector.x > 0.0f)	pPerson->SetDir(DIR::LEFT);
									 else		pPerson->SetDir(DIR::RIGHT);

									 // ������уx�N�g�������Z (A���)���Z�������ɕύX
									 //pPerson->AddMove(Vector3(HitDamageInfo->LandFlyVector.x, HitDamageInfo->LandFlyVector.y, 0));	
									 pPerson->SetMove(Vector3(HitDamageInfo->FlyVector.x, HitDamageInfo->FlyVector.y, 0));

									 // �q�b�g�X�g�b�v
									 pPerson->SetHitStopFrame(HitDamageInfo->hitStopFlame);

									 // �d���t���[���ݒ�
									 int RecoveryFrame(HitDamageInfo->HitRecoveryFrame);

									 // �_���[�W
									 int damage((int)(HitDamageInfo->damage * pPerson->GetDamageRate()));							// �������̃_���[�W���[�g�ɂ������킹��

									 // �����l���������Ă�Ȃ�
									 if (pPerson->isWillPower())
									 {
										 // �h��͂��グ��
										 damage = (int)((float)damage* 0.7f);
									 }
									 

									 // ���K�E�Z���͓��Z�␳�𖳎�(�A�C���[�̂�Ƃ��Ђǂ����ƂɂȂ邩��)
									 if (!HitDamageInfo->bOverDrive)
									 {
										 for (auto it : *pPerson->GetRecoveryDamageCount())
										 {
											 if (it == (BASE_ACTION_STATE)HitDamageInfo->iAttackType)
											 {
												 RecoveryFrame = (int)(RecoveryFrame * REPEAT_ATTACK_RATE);	// ���Z�␳
											 }
										 }
									 }
									 RecoveryFrame = max(RecoveryFrame, 1);		// ���1�ȏ�I
									 pPerson->SetRecoveryFrame(RecoveryFrame);

									 // ���R���{UI �G�t�F�N�g(�J�E���g)����
									 COMBO_DESK comboDesk;

									 // �����i�̂ݓ���Ƃ���
									 if (pPerson->GetRecoveryDamageCount()->empty())
									 {
										 if (isDamageCounterState(pPerson)) comboDesk.bCounter = true;
											 damage += BasePlayer::c_FIRST_HIT_ADD_DAMAGE;	// ���i�����_���[�W�𑝂₷
									 }

									 comboDesk.side = pPerson->GetSide();
									 comboDesk.damage = damage;
									 comboDesk.recoveryFrame = RecoveryFrame;
									 MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

									 // ��������U�����X�g�ǉ�
									 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);

									 // (�ǉ�)�q�b�g�G�t�F�N�g����! 
									 // (TODO)�G�t�F�N�g�����͎����̃|�W�V�����ɂ����Ă�I�I�I�@��Ńq�b�g�|�W�V�����Ƃ����邩��
									 pPerson->AddEffectAction(pPerson->GetCenterPos() + Vector3(0, 4, 0), (EFFECT_TYPE)HitDamageInfo->HitEffectType);

									 // �󒆃W�����v�̌�������
									 pPerson->SetAerialJump(true);

									 // �U���ɑ���𖳓G�ɂ����鑮���������Ă���
									 if (HitDamageInfo->BeInvincible)
									 {
										 //�ނĂ�������
										 // ������ς肱���Ŗ��G���Ԑݒ�
										 pPerson->SetInvincible(90, 1);
									 }

									 // �_���[�W����
									 pPerson->MultDamageRate(HitDamageInfo->fComboRate);						// �_���[�W���[�g���󂯂��Z�̃��[�g�ŏ�Z����(���񂾂�_���[�W�������Ă���)

									 const int DamagedHP(max(pPerson->GetHP() - damage, 0));
									 // �_���[�W���󂯂����HP��ݒ�
									 pPerson->SetHP(DamagedHP);
									 if (DamagedHP == 0)
									 {
										 // �t�B�j�b�V���t���O�̂���U���Ȃ�(�K�E�̓r���Ŏ��ȂȂ��ׂ̃t���O)
										 if (HitDamageInfo->bFinishOK)
										 {
											 // ��KO�̃��b�Z�[�W�𑗂�
											 KO_INFO KOInfo;
											 KOInfo.FinishType = HitDamageInfo->bOverDrive ? FINISH_TYPE::OVER_DRIVE : FINISH_TYPE::NORMAL;
											 KOInfo.iNumWinnerRound = pPerson->GetTargetPlayer()->GetWinNum();	// �|������̏�������;
											 KOInfo.WinnerSide = pPerson->GetTargetPlayer()->GetSide();
											 KOInfo.LoseSide = pPerson->GetSide();
											 // 
											 MsgMgr->Dispatch(0, pPerson->GetTargetPlayer()->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::KO, &KOInfo);

											 // ����
											 pPerson->GetFSM()->ChangeState(BasePlayerState::Die::GetInstance());
										 }
									 }

									 else
									 {

										 // �󂯂��_���[�W�ł�SP�Q�[�W�����㏸
										 if (damage <= 50)
										 {
											// �Ȃ�
										 }
										 else if (damage <= 200)
										 {
											 pPerson->AddOverDriveGage(0.5f);
										 }
										 else if (damage <= 500)
										 {
											 pPerson->AddOverDriveGage(1);

										 }
										 else if (damage <= 900)
										 {
											 pPerson->AddOverDriveGage(1.5f);

										 }
										 else 
										 {
											 pPerson->AddOverDriveGage(2.5f);
										 }

										 // ������ы���	�Ɂ@������	�_���[�W�X�e�[�g��ς���
										 //const float len = HitDamageInfo->FlyVector.Length();
										 //if (len < HUTTOBI_LINE)
										 //{
										//	 // �����ŋ󒆂ł�����Ă邩�n��ł�����Ă邩�𕪂���
										//	 //if (pPerson->isLand() == true)
										//	 {
										//		 // �n��m�b�N�o�b�N�ɍs��
										//		 pPerson->GetFSM()->ChangeState(BasePlayerState::KnockBack::GetInstance());
										//	 }
										//	 //else
										//	 //{
										//	//	 // �󒆃m�b�N�o�b�N�ɍs��
										//	//	 pPerson->GetFSM()->ChangeState(BasePlayerState::AerialKnockBack::GetInstance());
										//	 //}
										 //
										 //}
										 //else
										 //{
										//	 // �m�b�N�_�E���ɍs��
										//	 pPerson->GetFSM()->ChangeState(BasePlayerState::KnockDown::GetInstance());
										 //
										 //}
										 switch (HitDamageInfo->DamageMotion)
										 {
										 case DAMAGE_MOTION::KNOCK_BACK:
											 pPerson->GetFSM()->ChangeState(KnockBack::GetInstance());
											 break;
										 case DAMAGE_MOTION::KNOCK_DOWN:
											 pPerson->GetFSM()->ChangeState(KnockDown::GetInstance());
											 break;
										 case DAMAGE_MOTION::DOWN_FALL:
											 pPerson->GetFSM()->ChangeState(DownFall::GetInstance());
											 break;
										 }
									 }
									 return true;
									 break;
	}

		// �U�����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_ATTACK:
	{
									 if (pPerson->isAttackState())
									 {
										 pPerson->GetAttackData()->bHitSuccess = true;	// �}�W�œ��������K�[�[�[�[�[���I�I�I�I
#ifdef _DEBUG
										 // �R���{��������������
										 Vector2 screenPos = Math::WorldToScreen(pPerson->GetPos());
										 NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pPerson->GetCurrentFrame(), Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);
#endif
									 }

									 HIT_ATTACK_INFO *HitAttackInfo = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
									 //pPerson->SetHitStopFrame(HitAttackInfo->hitStopFlame);// �q�b�g�X�g�b�v
									 pPerson->AddCollectScore(HitAttackInfo->HitScore);	// ���̑O�̃X�R�A�����Z
									 if (HitAttackInfo->bOverDrive) pPerson->ConversionScore();	// �t�B�j�b�V���A�^�b�N�Ȃ�X�R�A���Y
									 // �K�E�ȊO
									 else
									 {
										 // �U���𓖂Ă�SP�Q�[�W�㏸���I
										 if (HitAttackInfo->HitScore <= 200)
										 {
											 pPerson->AddOverDriveGage(2);
										 }
										 else if (HitAttackInfo->HitScore <= 500)
										 {
											 pPerson->AddOverDriveGage(3);

										 }
										 else if (HitAttackInfo->HitScore <= 900)
										 {
											 pPerson->AddOverDriveGage(5);

										 }
										 else if (HitAttackInfo->HitScore <= 1200)
										 {
											 pPerson->AddOverDriveGage(6);
										 }
										 else
										 {
											 pPerson->AddOverDriveGage(8);
										 }
									 }


									 //// ���̍U���ő��肪���肪����ł���
									 //if (pPerson->GetTargetPlayer()->GetHP() == 0)
									 //{
									//	 // KO�̃��b�Z�[�W�𑗂�
									//	 FINISH_TYPE FinishType(HitAttackInfo->bOverDrive ? FINISH_TYPE::OVER_DRIVE : FINISH_TYPE::NORMAL);
									 //
									//	 // 
									//	 MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::KO, &FinishType);
									 //}
									 break;
	}

	case HEAVE_HO_OVERFLOW_START:
	{
									// ���b�Z�[�W���͂�����I�[�o�[�t���[�ֈړ�
									pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDriveOverFlow_Success::GetInstance());
									return true;
	}break;

	case MESSAGE_TYPE::END_FINISHCALL:	// �t�B�j�b�V���̃R�[�����I�������A�������[�V�����ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Win::GetInstance());
		break;

	// (A���)���񂾂����b�Z�[�W
	}

	return false;
}


/*******************************************************/
//					�ҋ@�X�e�[�g
/*******************************************************/

void BasePlayerState::Wait::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::WAIT);

	//	�A���O�����
	pPerson->SetDirAngle();

	// �_�E���t���O������
	pPerson->SetDown(false);

}

void BasePlayerState::Wait::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���{�^��
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�ǂ������U���L�����Z��
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCancel(pPerson)) return;
	
	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	//if (GuardCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�_�b�V���L�����Z��
	//============================================
	if (DashCancel(pPerson))return;
	
	//////////////////////////////////////////////
	//	�ړ��L�����Z��
	//============================================
	if (WalkCancel(pPerson))return;

}

void BasePlayerState::Wait::Exit(BasePlayer * pPerson)
{
	

}

void BasePlayerState::Wait::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "WaitState");
}

bool BasePlayerState::Wait::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �������Ă�惁�b�Z�[�W
	case MESSAGE_TYPE::FALL:
	{
		// �����X�e�[�g�ɍs���I
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		break;
	}
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}

/*******************************************************/
//					�����X�e�[�g
/*******************************************************/

void BasePlayerState::Walk::Enter(BasePlayer * pPerson)
{
	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::WALK);

	// �����x�̐ݒ�
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x -= .75f;
		break;
	case DIR::RIGHT:
		move.x += .75f;
		break;
	default:
		MyAssert(0, "�ӂ�������[");
		break;
	}

	pPerson->SetMove(move);
}

void BasePlayerState::Walk::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�ǂ������U���L�����Z��
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	//if (GuardCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCancel(pPerson)) return;

	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::RIGHT) == 3)
	//{
	//	// ���������Ȃ瑖��
	//	if (pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	//	//pPerson->SetDir(DIR::RIGHT);
	//	return;
	//}

	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::LEFT) == 3)
	//{
	//	// ���������Ȃ瑖��
	//	if (pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	//	//pPerson->SetDir(DIR::LEFT);
	//	return;
	//}

	//////////////////////////////////////////////
	//	�_�b�V���L�����Z��
	//============================================
	if (DashCancel(pPerson))return;


	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::RIGHT);
		// �r���ő��肪���Ό����Ȃ������
		if (pPerson->GetTargetDir() == DIR::LEFT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
			return;
		}
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::LEFT);
		// �r���ő��肪���Ό����Ȃ������
		if (pPerson->GetTargetDir() == DIR::RIGHT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
			return;
		}
	}
	else // �j���[�g����
	{
		Vector3 move = pPerson->GetMove();

		// �قڎ~�܂��Ă���AWait�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
		if (
			//move.Length() < .1f
			abs(move.x) < BasePlayer::c_END_MOVE_LINE
			)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
	}

	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * .5f);
}

void BasePlayerState::Walk::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::Walk::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "WalkState");
}

bool BasePlayerState::Walk::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �������Ă�惁�b�Z�[�W
	case MESSAGE_TYPE::FALL:
	{
		// �����X�e�[�g�ɍs���I
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		break;
	}
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�������X�e�[�g
/*******************************************************/

void BasePlayerState::BackWalk::Enter(BasePlayer * pPerson)
{
	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BACK_WALK);

	// �����x�̐ݒ�
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x += .75f;
		break;
	case DIR::RIGHT:
		move.x -= .75f;
		break;
	default:
		MyAssert(0, "�ӂ�������[");
		break;
	}

	pPerson->SetMove(move);
}

void BasePlayerState::BackWalk::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�ǂ������U���L�����Z��
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	//if (GuardCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCancel(pPerson)) return;

	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::RIGHT) == 3)
	//{
	//	// ���������Ȃ炻�̌����ɕ���
	//	if(pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());
	//
	//	//pPerson->SetDir(DIR::RIGHT);
	//	return;
	//}
	//
	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::LEFT) == 3)
	//{
	//	// ���������Ȃ炻�̌����ɕ���
	//	if(pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());
	//
	//	//pPerson->SetDir(DIR::LEFT);
	//	return;
	//}

	//////////////////////////////////////////////
	//	�_�b�V���L�����Z��
	//============================================
	if (DashCancel(pPerson))return;

	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::RIGHT);
		if (pPerson->GetTargetDir() == DIR::RIGHT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			return;
		}
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::LEFT);
		if (pPerson->GetTargetDir() == DIR::LEFT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			return;
		}
	}
	else // �j���[�g����
	{
		//Vector3 move = pPerson->GetMove();
		//
		//// �قڎ~�܂��Ă���AWait�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
		//if (
		//	//move.Length() < .1f
		//	abs(move.x) < BasePlayer::c_END_MOVE_LINE
		//	)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
	}

	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * .35f);
}

void BasePlayerState::BackWalk::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::BackWalk::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "BackWalkState");
}

bool BasePlayerState::BackWalk::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �������Ă�惁�b�Z�[�W
	case MESSAGE_TYPE::FALL:
	{
							   // �����X�e�[�g�ɍs���I
							   pPerson->GetFSM()->ChangeState(Fall::GetInstance());
							   break;
	}
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					����X�e�[�g
/*******************************************************/

void BasePlayerState::Run::Enter(BasePlayer * pPerson)
{
	// ���郂�[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::RUN);

	// ����G�t�F�N�g�����I
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::RUN);

	pPerson->SetDirAngle();

	// �����x�̐ݒ�
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x -= 1.0f;
		break;
	case DIR::RIGHT:
		move.x += 1.0f;
		break;
	default:
		MyAssert(0, "�ӂ�������[");
		break;
	}

	pPerson->SetMove(move);

	// SE�Đ�
	se->Play("�_�b�V��");

	pPerson->SetDushFrame(0);
}

void BasePlayerState::Run::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�ǂ������U���L�����Z��
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	//if (GuardCancel(pPerson)) return;

	pPerson->AddDushFrame(1);

	//////////////////////////////////////////////
	//	��
	//============================================
	// �������u��
	//if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	// ����ɑ΂��Ĕ��Ε���
	//	if (pPerson->GetTargetDir() == DIR::RIGHT)
	//	{
	//		// �������X�e�[�g��
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
	//	}
	//	else
	//	{
	//		// �O�����X�e�[�g��
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	}
	//}
	// �����Ă��
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT) && pPerson->GetDir() == DIR::LEFT)
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::LEFT);
	}

	//////////////////////////////////////////////
	//	�E
	//============================================
	// �������u�� ���_��@�����A���������Ƃ͂��肦�Ȃ��@�j���[�g����������
	//else if (pPerson->GetInputList(PLAYER_COMMAND_BIT::RIGHT) == 3)
	//{
	//	// ����ɑ΂��Ĕ��Ε���
	//	if (pPerson->GetTargetDir() == DIR::LEFT)
	//	{
	//		// �������X�e�[�g��
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
	//	}
	//	else
	//	{
	//		// �O�����X�e�[�g��
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	}
	//}
	// �����Ă��
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) && pPerson->GetDir() == DIR::RIGHT)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::RIGHT);
	}


	else // �j���[�g����
	{
		//Vector3 move = pPerson->GetMove();
		// ������x����������A�O�u���[�L�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
		//if (abs(move.x) < BasePlayer::c_FRONT_BRAKE_LINE)
		if(pPerson->GetDushFrame() >= 12)
		{
			//// �O�̃X�e�[�g��U�^�[���u���[�L��������A�u�ҋ@�v�X�e�[�g�ɖ߂�(�^�[���u���[�L�̌�Ƀt�����g�u���[�L���������Ă��܂�����)
			//if (pPerson->GetFSM()->isPrevState(*BasePlayerState::TurnOverBrake::GetInstance()))
			//	pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			//else
			{
				// ���@�t�����g�u���[�L���󂵂đҋ@�ɂ���
				//pPerson->GetFSM()->ChangeState(BasePlayerState::FrontBrake::GetInstance());
				pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			}
		}
	}



	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed());
}

void BasePlayerState::Run::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::Run::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "RunState");
}

bool BasePlayerState::Run::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �������Ă�惁�b�Z�[�W
	case MESSAGE_TYPE::FALL:
	{
		// �����X�e�[�g�ɍs���I
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		break;
	}
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�o�N�X�e�X�e�[�g
/*******************************************************/

void BasePlayerState::BackStep::Enter(BasePlayer * pPerson)
{
	// ���郂�[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BACK_STEP);

	// �o�N�X�e�A�N�V�����ɕς���
	pPerson->SetActionState(BASE_ACTION_STATE::BACKSTEP);

	// ����G�t�F�N�g�����I
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::BACK_STEP);

	// �����x�̐ݒ�
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x += 1.5f;
		break;
	case DIR::RIGHT:
		move.x -= 1.5f;
		break;
	default:
		MyAssert(0, "�ӂ�������[");
		break;
	}

	pPerson->SetMove(move);

	// SE�Đ�
	se->Play("�_�b�V��");
}

void BasePlayerState::BackStep::Execute(BasePlayer * pPerson)
{
	// �G�X�P�[�v�̎��ԍX�V&�G�X�P�[�v���ԏI�������
	switch (pPerson->GetActionFrame())
	{
	case FRAME_STATE::START:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(false);
		break;

	case FRAME_STATE::ACTIVE:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(true);
		break;

	case FRAME_STATE::FOLLOW:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(false);
		break;

	case FRAME_STATE::END:
		// �ҋ@���[�V�����ɖ߂�
	{
		Vector3 move = pPerson->GetMove();
		move *= .5f;	// �������キ����
		pPerson->SetMove(move);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
		break;
	}
}

void BasePlayerState::BackStep::Exit(BasePlayer * pPerson)
{
	pPerson->SetEscapeFlag(false);
}

void BasePlayerState::BackStep::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "BackStepState");
}

bool BasePlayerState::BackStep::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �������Ă�惁�b�Z�[�W
	case MESSAGE_TYPE::FALL:
	{
							   // �����X�e�[�g�ɍs���I
							   pPerson->GetFSM()->ChangeState(Fall::GetInstance());
							   break;
	}
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�t�����g�u���[�L�X�e�[�g
/*******************************************************/

//BasePlayerState::FrontBrake * BasePlayerState::FrontBrake::GetInstance()
//{
//	// �����ɕϐ������
//	static BasePlayerState::FrontBrake instance;
//	return &instance;
//}
//
//void BasePlayerState::FrontBrake::Enter(BasePlayer * pPerson)
//{
//	// �O�u���[�L���[�V�����ɕς���
//	pPerson->SetMotion(MOTION_TYPE::BRAKE);
//
//	// SE�Đ�
//	se->Play("�u���[�L1");
//}
//
//void BasePlayerState::FrontBrake::Execute(BasePlayer * pPerson)
//{
//	//////////////////////////////////////////////
//	//	�W�����v�L�����Z��
//	//============================================
//	if (JumpCancel(pPerson)) return;
//
//	Vector3 move = pPerson->GetMove();
//	
//	// A���: MOVE�l�������Œ��������Ă���
//	move *= .95f;
//	pPerson->SetMove(move);
//
//	// �قڎ~�܂��Ă���A(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
//	if (
//		//move.Length() < .1f
//		abs(move.x) < BasePlayer::c_END_MOVE_LINE
//		)
//	{
//		// �����L�[����͂��Ă�����A�ēx�u����v�X�e�[�g��
//		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		{
//			pPerson->SetDir(DIR::LEFT);
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//		else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//		{
//			pPerson->SetDir(DIR::RIGHT);
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//
//		// ���E�����L�[�������ĂȂ�������A�ҋ@�X�e�[�g��
//		else
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		}
//
//	}
//}
//
//void BasePlayerState::FrontBrake::Exit(BasePlayer * pPerson)
//{
//
//
//}
//
//void BasePlayerState::FrontBrake::Render(BasePlayer * pPerson)
//{
//	tdnText::Draw(20, 690, 0xffffffff, "�t�����gBrakeState");
//}
//
//bool BasePlayerState::FrontBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	// ���b�Z�[�W�^�C�v
//	switch (msg.Msg)
//	{
//		// �������Ă�惁�b�Z�[�W
//	case MESSAGE_TYPE::FALL:
//	{
//		// �����X�e�[�g�ɍs���I
//		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
//		break;
//	}
//	}
//
//	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
//	return false;
//}



/*******************************************************/
//					U�^�[���u���[�L�X�e�[�g
/*******************************************************/

//BasePlayerState::TurnOverBrake * BasePlayerState::TurnOverBrake::GetInstance()
//{
//	// �����ɕϐ������
//	static BasePlayerState::TurnOverBrake instance;
//	return &instance;
//}
//
//void BasePlayerState::TurnOverBrake::Enter(BasePlayer * pPerson)
//{
//	// U�^�[���u���[�L���[�V�����ɕς���
//	pPerson->SetMotion(MOTION_TYPE::U_TURN);
//
//	// SE�Đ�
//	se->Play("�u���[�L1");
//}
//
//void BasePlayerState::TurnOverBrake::Execute(BasePlayer * pPerson)
//{
//	//////////////////////////////////////////////
//	//	�W�����v�L�����Z��
//	//============================================
//	if (JumpCancel(pPerson)) return;
//
//	Vector3 move = pPerson->GetMove();
//
//	// �قڎ~�܂��Ă���AWait�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
//	if (
//		//move.Length() < .1f
//		abs(move.x) < BasePlayer::c_END_MOVE_LINE
//		)
//	{
//		// �����L�[����͂��Ă�����A�ēx�u����v�X�e�[�g��
//		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//		else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//
//		// ���E�����L�[�������ĂȂ�������A�ҋ@�X�e�[�g��
//		else
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		}
//
//	}
//}
//
//void BasePlayerState::TurnOverBrake::Exit(BasePlayer * pPerson)
//{
//
//
//}
//
//void BasePlayerState::TurnOverBrake::Render(BasePlayer * pPerson)
//{
//	tdnText::Draw(20, 690, 0xffffffff, "TurnOverBrakeState");
//}
//
//bool BasePlayerState::TurnOverBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	// ���b�Z�[�W�^�C�v
//	switch (msg.Msg)
//	{
//		// �������Ă�惁�b�Z�[�W
//	case MESSAGE_TYPE::FALL:
//	{
//		// �����X�e�[�g�ɍs���I
//		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
//		break;
//	}
//	}
//
//	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
//	return false;
//}



/*******************************************************/
//					�W�����v�X�e�[�g
/*******************************************************/

void BasePlayerState::Jump::Enter(BasePlayer * pPerson)
{
	// �W�����v���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::JUMP);

	// ���Ⴊ�݃t���O������
	pPerson->GetJump()->Clear();

	pPerson->SetDirAngle();
}

void BasePlayerState::Jump::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�����O�̂��Ⴊ�ݒ�
	//============================================
	if (pPerson->GetJump()->bHold)
	{
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::C))
		{
			// �v���C���[�������Ă鎞�Ԍv��
			pPerson->GetJump()->PlayerHoldTimer++;
		}

		if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::B))
			pPerson->GetJump()->bAttackPush = true;

		if (++pPerson->GetJump()->HoldTimer > 2)
		{
			// �����x�̐ݒ�
			Vector3 add(0, 0, 0);

			//// ���Ⴊ��ł�ԁA�����Ɖ����Ă��̂ŁA�ő�W�����v
			//if (pPerson->GetJump()->HoldTimer == pPerson->GetJump()->PlayerHoldTimer)
			{
				add.y = BasePlayer::c_MAX_JUMP;
			}
			//// ��������Ȃ��̂ŏ��W�����v
			//else
			//{
			//	add.y = BasePlayer::c_MAX_JUMP * .65f;
			//}

			// ���E����
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))		add.x = -.5f;
			else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT)) add.x = .5f;

			// �ړ��n�𑫂��I
			if (pPerson->GetFSM()->isPrevState(*Run::GetInstance()))
			{
				add.x *= .25f;
				pPerson->AddMove(add);
			}
			else
			{
				pPerson->SetMove(add);
			}

			// ���W�����v�G�t�F�N�g
			pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::JUMP);

			// �n��t���O�I�t�I
			//pPerson->SetLand(false);

			// ���Ⴊ�݃t���O�I��
			pPerson->GetJump()->bHold = false;

			// ���Ⴊ��ł�Œ��ɍU���{�^���������Ă���
			if (pPerson->GetJump()->bAttackPush)
			{
				// �󉺕���
				//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN) == true)
				//{
				//	// �󒆉��U���X�e�[�g�ɍs��
				//	pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
				//}
				//else
				{
					// �󒆍U���X�e�[�g�ɍs��
					pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
				}
				return;
			}

			// SE�Đ�
			se->Play("�W�����v");
		}
	}

	//////////////////////////////////////////////
	//	�W�����v��
	//============================================
	else
	{
		// �n��t���Otrue(���n������)
		if (pPerson->isLand())
		{
			// ���n�X�e�[�g�Ɉڍs
			pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
			return;
		}

		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		////////////////////////////////////////////////
		////	�󒆍U���L�����Z��
		////============================================
		//if (AerialAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���L�����Z��
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�ǂ������U���L�����Z��
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�󒆃_�b�V���L�����Z��
		//============================================
		if (AerialDashCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�󒆃W�����v�{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
		//{
		//	// �󒆃W�����v�̌������Ȃ�������e��
		//	if (pPerson->isAerialJump() == true)
		//	{
		//		// �󒆃W�����v�X�e�[�g�ɍs��
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		//	}
		//}
		if (JumpCancel(pPerson))return;

		//////////////////////////////////////////////
		//	������
		//============================================
		//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		//{
		//	pPerson->AddMove(Vector3(-0.03f, 0.0f, 0.0f));
		//}


		//////////////////////////////////////////////
		//	�E����
		//============================================
		//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		//{
		//	pPerson->AddMove(Vector3(0.03f, 0.0f, 0.0f));
		//}
		//
		//// �ړ��n����
		pPerson->MoveClampX(pPerson->GetMaxSpeed());
	}
}

void BasePlayerState::Jump::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::Jump::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�W�����v�����ā[��");
	//tdnText::Draw(20, 690, 0xffffffff, "JumpState");
}

bool BasePlayerState::Jump::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//					�󒆃W�����v�X�e�[�g
/*******************************************************/

void BasePlayerState::AerialJump::Enter(BasePlayer * pPerson)
{
	// �W�����v���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::JUMP);

	// �󒆃W�����v�̌������g�p
	pPerson->SetAerialJump(false);

	// �ړ��n�𑫂��I
	//Vector3 set(pPerson->GetMove());//c_MAX_AerialJump
	Vector3 set(0, 0, 0);
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		set.x = -.475f;
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		set.x = .475f;
	}
	set.y = 2.0f;
	pPerson->SetMove(set);// �O���MOVE�l������
	//pPerson->AddMove(add); 

	// �n��t���O�I�t�I
	//pPerson->SetLand(false);

	// SE�Đ�
	se->Play("�W�����v");
}

void BasePlayerState::AerialJump::Execute(BasePlayer * pPerson)
{

	//////////////////////////////////////////////
	//	�󒆃W�����v��
	//============================================

	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�󒆍U���L�����Z��
	//============================================
	//if (AerialAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	������
	//============================================
	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	pPerson->AddMove(Vector3(-0.025f, 0.0f, 0.0f));
	//}


	//////////////////////////////////////////////
	//	�E����
	//============================================
	//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	pPerson->AddMove(Vector3(0.025f, 0.0f, 0.0f));
	//}

	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * 0.5f);

	// Y�̈ړ��ʂ�-�ɂȂ����痎���X�e�[�g��
	if (pPerson->GetMove().y < 0)
	{
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		return;
	}

	// �n��t���Otrue(���n������)
	if (pPerson->isLand())
	{
		// ���n�X�e�[�g�Ɉڍs
		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
	}

}

void BasePlayerState::AerialJump::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
	int i = 0;
}

void BasePlayerState::AerialJump::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�󒆃W�����v�����ā[��");
	//tdnText::Draw(20, 690, 0xffffffff, "AerialJumpState");
}

bool BasePlayerState::AerialJump::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//					����
/*******************************************************/

void BasePlayerState::Fall::Enter(BasePlayer * pPerson)
{
	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::FALL);

	// �n�ʂɂ��Ȃ���I�Ƃ������Ƃ�ݒ�
	pPerson->SetLand(false);
}

void BasePlayerState::Fall::Execute(BasePlayer * pPerson)
{
	// �n��t���Otrue(���n������)
	if (pPerson->isLand())
	{
		// ���n�X�e�[�g�Ɉڍs
		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	������
	//============================================

	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;


	//////////////////////////////////////////////
	//	�󒆍U���L�����Z��
	//============================================
	//if (AerialAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (AttackCancel(pPerson))return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	������
	//============================================
	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	pPerson->AddMove(Vector3(-0.05f, 0.0f, 0.0f));
	//}
	//
	//
	////////////////////////////////////////////////
	//	�E����
	//============================================
	//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	pPerson->AddMove(Vector3(0.05f, 0.0f, 0.0f));
	//}

	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * 1.1f);

}

void BasePlayerState::Fall::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::Fall::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "��������");
	//tdnText::Draw(20, 690, 0xffffffff, "FallState");
}

bool BasePlayerState::Fall::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//					���n�X�e�[�g
/*******************************************************/

void BasePlayerState::Land::Enter(BasePlayer * pPerson)
{
	// ���n���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::RAND);

	// ���n���ԏ�����
	pPerson->GetJump()->LandTimer = 0;

	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed());

	// ���n�G�t�F�N�g�I
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::DROP_IMPACT);


	// �A�N�V�����X�e�[�g�I�t(�܂茻�݂܂ł��������������������)
	pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);
}

void BasePlayerState::Land::Execute(BasePlayer * pPerson)
{
	// �U�����Ă��ė�������X�L���ł�悤��
	//if (pPerson->GetFSM()->isPrevState(*BasePlayerState::AerialAttack::GetInstance()) == false &&
	//	pPerson->GetFSM()->isPrevState(*BasePlayerState::AerialDropAttack::GetInstance()) == false)
	{
		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCancel(pPerson))
			return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���L�����Z��
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	���Ⴊ�݃L�����Z��
		//============================================
		if (SquatCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�͂���L�����Z��
		//============================================
		if (RunCancel(pPerson)) return;
	}

	// ��莞�Ԓ��n������
	if (++pPerson->GetJump()->LandTimer > 12)
	{
		// �����L�[����͂��Ă�����A�u����v�X�e�[�g��
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		{
			//pPerson->SetDir(DIR::LEFT);
			if (pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		}
		else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		{
			//pPerson->SetDir(DIR::RIGHT);
			if (pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		}

		// ���E�����L�[�������ĂȂ�������A�u�ҋ@�v�X�e�[�g��
		else
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

	}


}

void BasePlayerState::Land::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::Land::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "���n");
	//tdnText::Draw(20, 690, 0xffffffff, "LandState");
}

bool BasePlayerState::Land::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//					���b�V���U���X�e�[�g
/*******************************************************/

void BasePlayerState::RushAttack::Enter(BasePlayer * pPerson)
{
	// ���ʌ����Ȃ���U���h�~
	pPerson->SetDirAngle();

	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK1);

	// ���b�V���X�e�b�v������
	pPerson->RushStepReset();

	// �U���X�e�[�g��1�i�ڂɐݒ肷��
	pPerson->SetActionState(BASE_ACTION_STATE::RUSH1);

	// �����x�̐ݒ�

	// ���������Ă����ԂōU����������@�_�b�V���U�����ۂ��U��
	if (pPerson->GetFSM()->isPrevState(*Run::GetInstance()) == true)
	{
		float moveX = pPerson->GetMove().x * .75f;
		//static const float speedPow = 1;
		//if (pPerson->GetDir() == DIR::LEFT) 
		//{
		//	moveX = -speedPow;
		//}
		//else
		//{
		//	moveX = speedPow;
		//}
		pPerson->AddMove(Vector3(moveX, 0, 0));
	}

}

void BasePlayerState::RushAttack::Execute(BasePlayer * pPerson)
{
	// �U���I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	switch (pPerson->GetRushStep())
	{
	case 0:	// 1�i��

		// ���኱�����i�K�����s���͂��󂯕t����
		if (pPerson->GetActionFrame() == FRAME_STATE::START && pPerson->GetCurrentFrame() > BasePlayer::c_RUSH_AHEAD_START) pPerson->AheadCommandUpdate();

		// �q�b�g���Ă���
		if (pPerson->GetAttackData()->bHit)
		{
			// �t�H���[���[�V����(���̍U����)
			//if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				/* �L�����Z�����[�g */

				//////////////////////////////////////////////
				//	�X�L���L�����Z��
				//============================================
				if (SkillCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�X�^���h�L�����Z��
				//============================================
				if (StandCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�q�[�z�[�h���C�u�L�����Z��
				//============================================
				if (HeaveHoCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�ǂ������U���L�����Z��
				//============================================
				if (DokkoiAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�΋�U���L�����Z��
				//============================================
				if (AntiAirCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	���Ⴊ�ݍU���L�����Z��
				//============================================
				if (SquatAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�W�����v�L�����Z��
				//============================================
				if (JumpCancel(pPerson)) return;
				

				//////////////////////////////////////////////
				//	�q�[�z�[�h���C�u�I�[�o�[�t���[�L�����Z��
				//============================================
				if (HeaveHoOverFlowCancel(pPerson)) return;
				// �U���{�^����������(�������ςȂ�����)
				if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
				{
					// ��s���̓��Z�b�g
					pPerson->AheadCommandReset();

					// ���̃��[�V�����Z�b�g
					pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK2);

					// �U���X�e�[�g��2�i�ڂɐݒ肷��
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH2);

					pPerson->AddRushStep();
					//pPerson->GetRushAttack()->bNextOK = false;

					// �����Ă�����ɃL�[����͂���ƈړ�
					const float pow = 0.8f;
					if (pPerson->GetDir() == DIR::RIGHT)
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))// �����L�[����͂��Ă���
							pPerson->AddMove(Vector3(pow, 0, 0));
					}
					else
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT)) // �����L�[����͂��Ă���
							pPerson->AddMove(Vector3(-pow, 0, 0));
					}

				}
			}
		}

		break;
	case 1:	// 2�i��

		// ���኱�����i�K�����s���͂��󂯕t����
		if (pPerson->GetActionFrame() == FRAME_STATE::START && pPerson->GetCurrentFrame() > BasePlayer::c_RUSH_AHEAD_START) pPerson->AheadCommandUpdate();

		// �q�b�g���Ă���
		if (pPerson->GetAttackData()->bHit)
		{
			// �t�H���[���[�V����(���̍U����)
			//if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				/* �L�����Z�����[�g */
				//////////////////////////////////////////////
				//	�X�L���L�����Z��
				//============================================
				if (SkillCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�X�^���h�L�����Z��
				//============================================
				if (StandCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�q�[�z�[�h���C�u�L�����Z��
				//============================================
				if (HeaveHoCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�ǂ������U���L�����Z��
				//============================================
				if (DokkoiAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�΋�U���L�����Z��
				//============================================
				if (AntiAirCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	���Ⴊ�ݍU���L�����Z��
				//============================================
				if (SquatAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�W�����v�L�����Z��
				//============================================
				if (JumpCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	�q�[�z�[�h���C�u�I�[�o�[�t���[�L�����Z��
				//============================================
				if (HeaveHoOverFlowCancel(pPerson)) return;

				// �U���{�^����������(�������ςȂ�����)
				if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
				{
					// ��s���̓��Z�b�g
					pPerson->AheadCommandReset();

					pPerson->AddRushStep();
					//pPerson->GetRushAttack()->bNextOK = false;

					// �����Ă�����ɃL�[����͂���ƈړ�
					const float pow = 0.8f;
					if (pPerson->GetDir() == DIR::RIGHT)
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))// �����L�[����͂��Ă���
							pPerson->AddMove(Vector3(pow, 0, 0));
					}
					else
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT)) // �����L�[����͂��Ă���
							pPerson->AddMove(Vector3(-pow, 0, 0));
					}

					pPerson->ThirdRushInit();
				}
			}
		}

		break;
	case 2:	// 3�i��
		pPerson->ThirdRushUpdate();
		break;
	}
}

void BasePlayerState::RushAttack::Exit(BasePlayer * pPerson)
{
	if (pPerson->GetRushStep()) pPerson->ThirdRushExit();
	// ��s���̓��Z�b�g
	pPerson->AheadCommandReset();
}

void BasePlayerState::RushAttack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "���b�V��");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::RushAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U�����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
	//	pPerson->GetRushAttack()->bNextOK = true;
	//	break;
	//}
	//}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					�m�b�N�o�b�N�X�e�[�g
/*******************************************************/

void BasePlayerState::KnockBack::Enter(BasePlayer * pPerson)
{
	// �_���[�W���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKBACK);

}

void BasePlayerState::KnockBack::Execute(BasePlayer * pPerson)
{
	// �d����0�ȉ��Ȃ�d���I��
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// �ҋ@�X�e�[�g�ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());

		// �������J�E���g���Z�b�g
		pPerson->GetRecoveryDamageCount()->clear();
		// ��s���̓��Z�b�g
		pPerson->AheadCommandReset();
		// �_���[�W�␳���Z�b�g
		pPerson->ResetDamageRate();
	}
}

void BasePlayerState::KnockBack::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::KnockBack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�m�b�N�o�b�N�I");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::KnockBack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//								 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//								 // �����Ă��Ԃł�������U�����X�g�ǉ�
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);
	//								 // true�͕Ԃ��Ȃ��B�����ł́A�_�E����Ԃł̂�������J�E���g����邾���Ȃ̂ŁB
	//}
	//}
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					���������炢�m�b�N�o�b�N�X�e�[�g
/*******************************************************/

void BasePlayerState::DownFall::Enter(BasePlayer * pPerson)
{
	// ���������炢���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKDOWN_DOWN);

	// �_�E���t���O�𗧂Ă�
	pPerson->SetDown(true);
}

void BasePlayerState::DownFall::Execute(BasePlayer * pPerson)
{
	// �d����0�ȉ��Ȃ�_���[�W�␳����
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// �_���[�W�␳���Z�b�g
		pPerson->ResetDamageRate();
		// �R���{�␳���Z�b�g
		pPerson->GetRecoveryDamageCount()->clear();
	}

	{
		// �d����0�ɂȂ�����@�Ȃ��������ꂩ�̃{�^���������Ă����畜�A�X�e�[�g��
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::A)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::B)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::C)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::D)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::R1))
		{

			// �_�E���t���O������
			pPerson->SetDown(false);
			// �������J�E���g���Z�b�g
			pPerson->GetRecoveryDamageCount()->clear();
			// ��s���̓��Z�b�g
			pPerson->AheadCommandReset();
			// �_���[�W�␳���Z�b�g
			pPerson->ResetDamageRate();

			// �n��I�A��
			if (pPerson->isLand() == true)
			{
				// �n�ナ�J�o���[�X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
			}
			//else
			//{
			//	// �󒆃��J�o���[�X�e�[�g�ցI
			//	pPerson->GetFSM()->ChangeState(BasePlayerState::AerialRecovery::GetInstance());
			//}


		}
	}
}

void BasePlayerState::DownFall::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::DownFall::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�I�肦��m�b�N�o�b�N");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::DownFall::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
		//{
		//	// �U����������惁�b�Z�[�W
		//case MESSAGE_TYPE::HIT_DAMAGE:
		//{
		//								 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		//								 // �����Ă��Ԃł�������U�����X�g�ǉ�
		//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);
		//								 // true�͕Ԃ��Ȃ��B�����ł́A�_�E����Ԃł̂�������J�E���g����邾���Ȃ̂ŁB
		//}
		//}
		// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
		return false;
}



/*******************************************************/
//					�m�b�N�_�E���X�e�[�g
/*******************************************************/

void BasePlayerState::KnockDown::Enter(BasePlayer * pPerson)
{
	const float MoveLen(pPerson->GetMove().LengthSq());

	// �m�b�N�_�E�����[�V�����ɕς���
	pPerson->SetMotion((MoveLen > 1) ? MOTION_TYPE::KNOCKDOWN : MOTION_TYPE::KNOCKDOWN_DOWN);

	// �_�E���t���O�𗧂Ă�
	pPerson->SetDown(true);

	//�ނĂ�������
	// ������ς肱���Ŗ��G���Ԑݒ�(Global��Message�ŏ����܂�)
	//pPerson->SetInvincible(90,1);

	// ��
	//pPerson->SetRecoveryFrame(120);

}

void BasePlayerState::KnockDown::Execute(BasePlayer * pPerson)
{
	if (pPerson->isLand() == false)
	{
		// ������т��ނ�p�[�e�B�N��
		ParticleManager::EffectFlySmoke(pPerson->GetPos());
		//return; // [10/20��]�@�󒆂ł����A�ł���悤�ɂ��܂���
	}

	// �d����0�ȉ��Ȃ�d���I��
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// �������J�E���g���Z�b�g
		pPerson->GetRecoveryDamageCount()->clear();
		// �_���[�W�␳���Z�b�g
		pPerson->ResetDamageRate();

		//pPerson->SetInvincible(60, 1);

		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::A) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::B) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::C) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::D) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::R1)  )
		{
			// �_�E���t���O������
			pPerson->SetDown(false);

			// ��s���̓��Z�b�g
			pPerson->AheadCommandReset();

			// �n��I�A��
			if (pPerson->isLand() == true)
			{
				// �n�ナ�J�o���[�X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
				return;
			}
			else
			{
				// �󒆃��J�o���[�X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialRecovery::GetInstance());
				return;	
			}


		}

	}
}

void BasePlayerState::KnockDown::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::KnockDown::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�m�b�N�_�E��");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::KnockDown::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//								 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//								 // �����Ă��Ԃł�������U�����X�g�ǉ�
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);
	//								 // true�͕Ԃ��Ȃ��B�����ł́A�_�E����Ԃł̂�������J�E���g����邾���Ȃ̂ŁB
	//}
	//}
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/*******************************************************/
//					���ɏ�ԃX�e�[�g
/*******************************************************/

void BasePlayerState::Die::Enter(BasePlayer * pPerson)
{
	// ���Ƀ��[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKDOWN);
}

void BasePlayerState::Die::Execute(BasePlayer * pPerson)
{
}

void BasePlayerState::Die::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::Die::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "����");
}

bool BasePlayerState::Die::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�n�ナ�J�o���[�X�e�[�g
/*******************************************************/

void BasePlayerState::LandRecovery::Enter(BasePlayer * pPerson)
{
	// ���J�o���[�ɕς���
	pPerson->SetMotion(MOTION_TYPE::REVERSAL);

	// �肩�o���[������Ƃ�������
	//pPerson->SetRecoveryCount(0);

	// �J�E���g������X�e�[�g�ɕύX
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// ���J�o�[���͖��G�I�I�I
	//pPerson->SetInvincibleLV(1);
	pPerson->SetInvincible(
#ifdef GUARD_ON
		114514
#else
		40
#endif
		, 1);	// 1031 ���G���Ԃ�12��40�ɂ���

	// �G�t�F�N�g�����v
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3), EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::LandRecovery::Execute(BasePlayer * pPerson)
{
	//pPerson->AddRecoveryCount(1);

	const int iRecoveryCount(pPerson->GetCurrentFrame());

	// ��5�t���[���O�����s���͎�t(���o�T���G�p)
	if (iRecoveryCount > BasePlayer::c_RECOVERY_FLAME - 5)pPerson->AheadCommandUpdate();

	// ���J�o���[�t���[���𒴂�����I���I
	if (iRecoveryCount >= BasePlayer::c_RECOVERY_FLAME)
	{
		// ���o�T���G
		if (InvincibleAttackCancel(pPerson)) return;

		// ���p���\��
		if (AttackCancel(pPerson)) return;

		// �K�[�h
		//if (isInputGuardCommand(pPerson)) pPerson->GetFSM(Guard::GetInstance());

		// ��������ςŃW�����v����
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::UP))
		{
			pPerson->GetFSM()->ChangeState(Jump::GetInstance());
			return;
		}

		// �ҋ@�X�e�[�g��
		pPerson->GetFSM()->ChangeState(Wait::GetInstance());
	}
}

void BasePlayerState::LandRecovery::Exit(BasePlayer * pPerson)
{
	pPerson->SetInvincible(0, 0);
}

void BasePlayerState::LandRecovery::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�肩�΂�");
}

bool BasePlayerState::LandRecovery::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//					�󒆃��J�o���[�X�e�[�g (��)
/*******************************************************/

void BasePlayerState::AerialRecovery::Enter(BasePlayer * pPerson)
{
	// ���J�o���[�ɕς���
	pPerson->SetMotion(MOTION_TYPE::FALL);

	// �肩�o���[������Ƃ�������
	//pPerson->SetRecoveryCount(0);

	// �J�E���g������X�e�[�g�ɕύX
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// ���J�o�[���͖��G�I�I�I
	//pPerson->SetInvincibleLV(1);
	pPerson->SetInvincible(90, 1);

	Vector3 move;
	move = pPerson->GetMove()*0.25f;
	move += Vector3(0, 1.5f, 0);
	//pPerson->SetMove(move);

	// [10/20] move�ł͂Ȃ��Z�b�g��
	// �{�l������
	//Vector3 addMove = Vector3(0, 2.55f, 0);

	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		//addMove += Vector3(0.7f, 0, 0);
		move += Vector3(0.7f, 0, 0);
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		//addMove += Vector3(-0.7f, 0, 0);
		move += Vector3(-0.7f, 0, 0);
	}
	
	pPerson->SetMove(move);

	//pPerson->AddMove(addMove);

	// �G�t�F�N�g�����v
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3) , EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::AerialRecovery::Execute(BasePlayer * pPerson)
{
	//pPerson->AddRecoveryCount(1);
	const int iRecoveryCount(pPerson->GetCurrentFrame());

	// ��5�t���[���O�����s���͎�t(���o�T���G�p)
	if (iRecoveryCount > BasePlayer::c_RECOVERY_FLAME - 5)pPerson->AheadCommandUpdate();

	// ���J�o���[�t���[���𒴂�����I���I
	if (iRecoveryCount >= BasePlayer::c_RECOVERY_FLAME)
	{
		// �󒆂Ȃ̂ŗ�����X�e�[�g��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
	}
}

void BasePlayerState::AerialRecovery::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::AerialRecovery::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�󒆂肩�΂�");
}

bool BasePlayerState::AerialRecovery::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
		return false;
}


/*******************************************************/
//					���Ⴊ�݃X�e�[�g
/*******************************************************/

void BasePlayerState::Squat::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::SQUAT);

	// ���Ⴊ�݃t���OON
	pPerson->SetSquat(true);

}

void BasePlayerState::Squat::Execute(BasePlayer * pPerson)
{
	//	���Ⴊ�ݍU���{�^��
	//if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::A))
	//{
	//	// ���Ⴊ�ݍU���X�e�[�g�ɍs��
	//	pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
	//	return;
	//}

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (AttackCancel(pPerson))return;

	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�ǂ������U���L�����Z��
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�؂�L�����Z��
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	// ���Ⴊ�݃{�^���𗣂�����ҋ@�ɖ߂�
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN) == false)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}

}

void BasePlayerState::Squat::Exit(BasePlayer * pPerson)
{
	// ���Ⴊ�݃t���OOFF
	pPerson->SetSquat(false);

}

void BasePlayerState::Squat::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "SquatState");
}

bool BasePlayerState::Squat::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//					���Ⴊ�ݍU���X�e�[�g
/*******************************************************/

void BasePlayerState::SquatAttack::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::SQUAT_ATTACK);

	// ���U���X�e�[�g��2�i�ڂɐݒ肷��@����HIT�t���O�������Ă���
	pPerson->SetActionState(BASE_ACTION_STATE::SQUAT_ATTACK);

	pPerson->SetDirAngle();
}

void BasePlayerState::SquatAttack::Execute(BasePlayer * pPerson)
{
	// �U���I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		ChangeWaitState(pPerson);
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�ǂ������U���Ƒ������L�����Z��
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�΋�U���L�����Z��
		//============================================
		if (AntiAirCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	���b�V���U���L�����Z��
		//============================================
		if (RushAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�L�����Z��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//============================================
		//	�W�����v�{�^��
		//============================================		
		if (JumpCancel(pPerson))
		{
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))	
				pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		
			// ������΂� 
			pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			return;
		}

	}

}

void BasePlayerState::SquatAttack::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::SquatAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "SquatAttackState");
}

bool BasePlayerState::SquatAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U�����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
	//	break;
	//}
	//}
	return false;
}


/*******************************************************/
//					�΋�U���X�e�[�g
/*******************************************************/

void BasePlayerState::AntiAirAttack::Enter(BasePlayer * pPerson)
{
	// �΋�U�����[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::ANTI_AIR_ATTACK);

	// �U���X�e�[�g��΋��
	pPerson->SetActionState(BASE_ACTION_STATE::ANTI_AIR);

	// ����������
	pPerson->SetMove(Vector3(0,0,0));

	pPerson->SetDirAngle();
}

void BasePlayerState::AntiAirAttack::Execute(BasePlayer * pPerson)
{
	// �U���I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�ǂ������U���Ƒ������L�����Z��
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�L�����Z��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//============================================
		//	�W�����v�{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
		//{

		//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		//	{
		//		pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));		
		//	}
		//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		//	{
		//		pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		//	}

		//	// ������΂� 
		//	pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		//	// �W�����v�X�e�[�g�ɍs��
		//	pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		//	return;
		//}

		if (JumpCancel(pPerson))
		{
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));

			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
				pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));

			// ������΂� 
			pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			return;
		}

	}

}

void BasePlayerState::AntiAirAttack::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::AntiAirAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "AntiAirAttackState");
}

bool BasePlayerState::AntiAirAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}


/*******************************************************/
//					�������U���X�e�[�g
/*******************************************************/

void BasePlayerState::DownAttack::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::DOWN_ATTACK);

	// ���U���X�e�[�g��2�i�ڂɐݒ肷��@����HIT�t���O�������Ă���
	pPerson->SetActionState(BASE_ACTION_STATE::DOWN_ATTACK);

	pPerson->SetDirAngle();
}

void BasePlayerState::DownAttack::Execute(BasePlayer * pPerson)
{
	// �U���I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		ChangeWaitState(pPerson);
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�L�����Z��
		//============================================
		if (HeaveHoCancel(pPerson)) return;
	}

}

void BasePlayerState::DownAttack::Exit(BasePlayer * pPerson)
{
	// ��s���̓��Z�b�g
	pPerson->AheadCommandReset();
}

void BasePlayerState::DownAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "DownAttackState");
}

bool BasePlayerState::DownAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U�����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
	//	break;
	//}
	//}
	return false;
}


/*******************************************************/
//					���i�U��(�ǂ������A�^�b�N)�X�e�[�g
/*******************************************************/

void BasePlayerState::DokkoiAttack::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::DOKKOI_ATTACK);

	// �����i�U���X�e�[�g��ݒ�
	pPerson->SetActionState(BASE_ACTION_STATE::DOKKOI_ATTACK);

	// ��
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::DOKKOI);

	pPerson->SetDirAngle();
}

void BasePlayerState::DokkoiAttack::Execute(BasePlayer * pPerson)
{
	// �_�b�V���L�����Z����
	if (pPerson->GetActionState() == BASE_ACTION_STATE::FRAMECOUNT)
	{
		// ��莞�ԑ����ĉ������ĂȂ�������ҋ@�X�e�[�g��
		if (pPerson->GetCurrentFrame() > 30)
		{
			pPerson->GetFSM()->ChangeState(Wait::GetInstance());
			return;
		}

		float speed(pPerson->GetMaxSpeed());
		// �����Ă���Ɉړ�
		pPerson->SetMove(Vector3((pPerson->GetDir() == DIR::LEFT) ? -speed : speed, 0, 0));

		//////////////////////////////////////////////
		//	�U���L�����Z��
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�K�E�L�����Z��
		//============================================
		if (HeaveHoCancel(pPerson)) return;
	}

	// �U���I�����Ă���
	else if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	else if (pPerson->GetAttackData()->bHitSuccess == true)
	{
		// �����{�^��������̕���
		if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R1, true) || pPerson->isPushInputTRG((pPerson->GetDir() == DIR::LEFT) ? PLAYER_COMMAND_BIT::LEFT : PLAYER_COMMAND_BIT::RIGHT, true))
		{
			// ���胂�[�V����
			pPerson->SetMotion(MOTION_TYPE::RUN);

			// �t���[���v���X�e�[�g��
			pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);
		}
	}
}

void BasePlayerState::DokkoiAttack::Exit(BasePlayer * pPerson)
{
	// ��s���̓��Z�b�g
	pPerson->AheadCommandReset();
}

void BasePlayerState::DokkoiAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "DokkoiAttackState");
}

bool BasePlayerState::DokkoiAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U�����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
	//	break;
	//}
	//}
	return false;
}


/*******************************************************/
//					�󒆍U���X�e�[�g
/*******************************************************/

void BasePlayerState::AerialAttack::Enter(BasePlayer * pPerson)
{
	// �󒆍U�����[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::AERIAL_ATTACK);


	// ���U���X�e�[�g��ݒ肷��@����HIT�t���O�������Ă���
	pPerson->SetActionState(BASE_ACTION_STATE::AERIAL);

	// �A���O���𑊎�̌�����
	pPerson->SetDirAngle();

	// ���͕����Ɍ���
	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	pPerson->SetDir(DIR::RIGHT);
	//}
	//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	pPerson->SetDir(DIR::LEFT);
	//}
}

void BasePlayerState::AerialAttack::Execute(BasePlayer * pPerson)
{
	// �n��t���Otrue(���n������)
	if (pPerson->isLand())
	{
		// ���n�X�e�[�g�Ɉڍs
		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
		return;
	}

	// �U���I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �n��A�󒆂ŕ���
		//if (pPerson->isLand())
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		//else if(pPerson->isAerialJump()) pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		//else pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;


		//============================================
		//	�󒆃W�����v�L�����Z���{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
		//{

		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		//	//{
		//	//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		//	//}
		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		//	//{
		//	//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		//	//}

		//	//// ������΂� 
		//	//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		//
		//	// �󒆃W�����v�̌������Ȃ�������e��
		//	if (pPerson->isAerialJump() == true)
		//	{
		//		// �󒆃W�����v�X�e�[�g�ɍs��
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		//	}
		//	
		//	return;
		//}
		if (JumpCancel(pPerson))return;
		

	}


}

void BasePlayerState::AerialAttack::Exit(BasePlayer * pPerson)
{
	// ��s���͏�����
	pPerson->AheadCommandReset();
}

void BasePlayerState::AerialAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "AerialAttackState");
}

bool BasePlayerState::AerialAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U�����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
	//	break;
	//}
	//}
	return false;
}

/*******************************************************/
//					�󒆉��U���X�e�[�g
/*******************************************************/

//BasePlayerState::AerialDropAttack * BasePlayerState::AerialDropAttack::GetInstance()
//{
//	// �����ɕϐ������
//	static BasePlayerState::AerialDropAttack instance;
//	return &instance;
//}
//
//void BasePlayerState::AerialDropAttack::Enter(BasePlayer * pPerson)
//{
//	// �󒆉��U�����[�V�����ɕς���
//	pPerson->SetMotion(MOTION_TYPE::AERIALDROP_ATTACK);
//
//
//	// ���U���X�e�[�g��ݒ肷��@����HIT�t���O�������Ă���
//	pPerson->SetActionState(BASE_ACTION_STATE::AERIALDROP);
//
//	// ���͕����Ɍ���
//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//	{
//		pPerson->SetDir(DIR::RIGHT);
//	}
//	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//	{
//		pPerson->SetDir(DIR::LEFT);
//	}
//
//}
//
//void BasePlayerState::AerialDropAttack::Execute(BasePlayer * pPerson)
//{
//	// �n��t���Otrue(���n������)
//	if (pPerson->isLand())
//	{
//		// ���n�X�e�[�g�Ɉڍs
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
//		return;
//	}
//
//	// �U���I�����Ă���
//	if (!pPerson->isAttackState())
//	{
//		// �ҋ@���[�V�����ɖ߂�
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		return;
//	}
//
//	// HIT���Ă�����L�����Z���ł���
//	if (pPerson->GetAttackData()->bHit == true)
//	{
//		//============================================
//		//	�W�����v�{�^��
//		//============================================
//		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
//		//{
//
//		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//		//	//{
//		//	//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
//		//	//}
//		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		//	//{
//		//	//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
//		//	//}
//
//		//	//// ������΂� 
//		//	//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));
//
//		//	// �󒆃W�����v�̌������Ȃ�������e��
//		//	if (pPerson->isAerialJump() == true)
//		//	{
//		//		// �󒆃W�����v�X�e�[�g�ɍs��
//		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
//		//	}
//		//	return;
//		//}
//		if (JumpCancel(pPerson))return;
//	
//
//	}
//
//}
//
//void BasePlayerState::AerialDropAttack::Exit(BasePlayer * pPerson)
//{
//
//
//}
//
//void BasePlayerState::AerialDropAttack::Render(BasePlayer * pPerson)
//{
//
//	tdnText::Draw(20, 690, 0xffffffff, "AerialDropAttackState");
//}
//
//bool BasePlayerState::AerialDropAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	//// ���b�Z�[�W�^�C�v
//	//switch (msg.Msg)
//	//{
//	//	// �U�����������惁�b�Z�[�W
//	//case MESSAGE_TYPE::HIT_ATTACK:
//	//{
//	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
//	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
//	//	break;
//	//}
//	//}
//	return false;
//}

/*******************************************************/
//					����X�e�[�g
/*******************************************************/

void BasePlayerState::Escape::Enter(BasePlayer * pPerson)
{
	// ������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::ESCAPE);

	// ���G�X�P�[�v�̃t���[���ɐݒ肷��
	pPerson->SetActionState(BASE_ACTION_STATE::ESCAPE);
	
	// �t�̌���������
	//pPerson->TurnOverDir();

	pPerson->SetDirAngle();

	// ����G�t�F�N�g�����I
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::ESCAPE);

	// SE�Đ�
	se->Play("�G�X�P�[�v");
}

void BasePlayerState::Escape::Execute(BasePlayer * pPerson)
{
	// �G�X�P�[�v�̎��ԍX�V&�G�X�P�[�v���ԏI�������
	switch (pPerson->GetActionFrame())
	{
	case FRAME_STATE::START:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(false);
		// �N�C�b�N�G�X�P�[�v
		// �����Ă�������ɃX�D�[���ƈړ�
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(.1f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.1f, 0, 0));
		}
		break;

	case FRAME_STATE::ACTIVE:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(true);
		// �N�C�b�N�G�X�P�[�v
		// �����Ă�������ɃX�D�[���ƈړ�
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(1.45f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-1.45f, 0, 0));
		}
		break;

	case FRAME_STATE::FOLLOW:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(false);
		// �N�C�b�N�G�X�P�[�v
		// �����Ă�������ɃX�D�[���ƈړ�
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(.1f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.1f, 0, 0));
		}
		break;

	case FRAME_STATE::END:
		pPerson->SetEscapeFlag(false);
		// �ҋ@���[�V�����ɖ߂�
		Vector3 move = pPerson->GetMove();
		move *= .5f;	// �������キ����
		pPerson->SetMove(move);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}



}

void BasePlayerState::Escape::Exit(BasePlayer * pPerson)
{
	// �t���O�ݒ�
	pPerson->SetEscapeFlag(false);

}

void BasePlayerState::Escape::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "EscapeState");
}

bool BasePlayerState::Escape::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
	//	return true;
	//}
	//}

	return false;
}


/*******************************************************/
//					�y���\�i�������[�V�����X�e�[�g
/*******************************************************/

void BasePlayerState::StandAction::Enter(BasePlayer * pPerson)
{
	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::PERSONA);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::STAND);

	// ���X�^���h�A�N�V��������

	pPerson->GetStand()->Action(GetSkillActionType(pPerson));

	// �d�͂̉e���󂯂�ȁI
	pPerson->SetMoveUpdate(false);

	// SE�Đ�
	se->Play("�y���\�i����");

	// �y���\�i�����G�t�F�N�g�I
	pPerson->AddEffectAction(pPerson->GetPos(),EFFECT_TYPE::PERSONA);

	pPerson->SetDirAngle();

	// �O��ړ��ʕۑ�
	//pPerson->SetStandSaveMove(pPerson->GetMove());
}

void BasePlayerState::StandAction::Execute(BasePlayer * pPerson)
{
	// �I�[���̃p�[�e�B�N��
	ParticleManager::EffectPersonaAura(Vector3(pPerson->GetPos().x, pPerson->GetPos().y, -0.5f));	// �኱��O

	if (pPerson->GetStand()->isHit())
	{
		/* �L�����Z�����[�g */

		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	���Ⴊ�݃L�����Z��
		//============================================
		if (SquatCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCancel(pPerson))
		{

			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT) == true)
			{
				pPerson->AddMove(Vector3(-0.45f, 0, 0));
			}
			else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) == true)
			{
				pPerson->AddMove(Vector3(0.45f, 0, 0));
			}

			return;
		}
		
		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�L�����Z��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	���E�L�[�ő���L�����Z��
		//============================================
		if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
		{
			pPerson->SetDir(DIR::LEFT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
		{
			pPerson->SetDir(DIR::RIGHT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
	}

	// �ړ��ʖ���
	//pPerson->SetMove(move);

	// �A�N�V�����t���[���I�����Ă���A�߂�
	if (!pPerson->isFrameAction())
	{
		// �ۑ����Ă�ړ��ʂ�߂�
		//pPerson->SetMove(pPerson->GetStandSaveMove());

		// �ҋ@�ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::StandAction::Exit(BasePlayer * pPerson)
{
	// �d�͂̉e�������ɖ߂�
	pPerson->SetMoveUpdate(true);
}

void BasePlayerState::StandAction::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "EscapeState");
}

bool BasePlayerState::StandAction::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
	//	return true;
	//}
	//}

	return false;
}



/*******************************************************/
//					���G�U���X�e�[�g
/*******************************************************/

void BasePlayerState::FinishAttack::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::FINISH_ATTACK);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::INVINCIBLE_ATTACK);

	// �����G�t���O��ON�ɂ���
	pPerson->SetInvincible(114514, 1);

	// �n���G�t�F�N�g
	pPerson->AddEffectAction(pPerson->GetPos() , EFFECT_TYPE::INVINCIBLE_ATTACK);

	pPerson->SetDirAngle();

	// SE�Đ�
	//se->Play("�y���\�i����");
}

void BasePlayerState::FinishAttack::Execute(BasePlayer * pPerson)
{
	// �U���X�e�[�g�I�������
	if (!pPerson->isAttackState())
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
	// ���t�H���[���[�V�����ɂȂ����疳�G������
	if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
	{
		pPerson->SetInvincible(0, 0);
	}
}

void BasePlayerState::FinishAttack::Exit(BasePlayer * pPerson)
{
	// ��s���̓��Z�b�g
	pPerson->AheadCommandReset();
}

void BasePlayerState::FinishAttack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "�t�B�j�b�V����������State");
}

bool BasePlayerState::FinishAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
	//	return true;
	//}
	//}

	return false;
}



/*******************************************************/
//					�X�L���X�e�[�g
/*******************************************************/

void BasePlayerState::Skill::Enter(BasePlayer * pPerson)
{
	pPerson->SetSkillActionType(GetSkillActionType(pPerson));

	// ��s���̓��Z�b�g
	pPerson->AheadCommandReset();

	// �X�L���p�ϐ�������
	pPerson->SkillInit();

	// SE�Đ�
	//se->Play("�y���\�i����");
}

void BasePlayerState::Skill::Execute(BasePlayer * pPerson)
{
	// ���̃L�����N�^�[�̃X�L���̍X�V
	const bool bEnd = pPerson->SkillUpdate();

	// �U���X�e�[�g�I�������
	//if (pPerson->GetActionFrame()==FRAME_STATE::END)
	if (bEnd)
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		if (pPerson->isLand()) ChangeWaitState(pPerson);
		else pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
		return;
	}

	// �U�����������Ă�����
	//if (pPerson->GetAttackData()->bHit)
	//{
	//	//////////////////////////////////////////////
	//	//	�q�[�z�[�h���C�u�L�����Z��
	//	//============================================
	//	if (HeaveHoCancel(pPerson)) return;
	//}
}

void BasePlayerState::Skill::Exit(BasePlayer * pPerson)
{
	// �A�C���[�Ƃ��ŏd�͖��������肷��̂ŁA�ꉞ�����ŉ������Ă���
	pPerson->SetMoveUpdate(true);

	// �X�L���̏I���
	pPerson->SkillExit();

	// ��s���̓��Z�b�g
	pPerson->AheadCommandReset();

	pPerson->SetDirAngle();
}

void BasePlayerState::Skill::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "�X�L��State");
}

bool BasePlayerState::Skill::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
	//	return true;
	//}
	//}

	return false;
}


/*******************************************************/
//					1more�o���X�e�[�g
/*******************************************************/

void BasePlayerState::OverDrive_OneMore::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::OVERDRIVE_ONEMORE);

	// �I�[�o�[�h���C�u
	pPerson->ActionOverDrive(OVERDRIVE_TYPE::ONEMORE);

	// SE�Đ�
	se->Play("�I�[�o�[�h���C�u�J�n");

	// ��ɕ���
	pPerson->SetMove(Vector3(0, 1, 0));

	// �o�[�X�g�O�G�t�F�N�g�@�����I
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::ONEMORE_BURST_START);

	// �Q�[�����~�߂�t���O
	pPerson->SetGameTimerStopFlag(true);

	// ������̓��Z�␳����������
	pPerson->GetTargetPlayer()->GetRecoveryDamageCount()->clear();
	pPerson->GetTargetPlayer()->ResetDamageRate();
}

void BasePlayerState::OverDrive_OneMore::Execute(BasePlayer * pPerson)
{

	// �����̊ԃQ�[�W�����Ƃ��Ă���̏���
	pPerson->SetOverDriveFrame(BasePlayer::c_OVERDRIVE_MAX_TIME);

	if (pPerson->GetActionFrame() == FRAME_STATE::START)
	{
		// ��
		pPerson->SetMove(Vector3(0, .21f, 0));
	}
	if (pPerson->GetActionFrame() == FRAME_STATE::ACTIVE)
	{
		// �o�[�X�g�G�t�F�N�g�����I
		pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, 0), EFFECT_TYPE::ONEMORE_BURST);

		// �������~�߂�
		pPerson->SetMove(Vector3(0, 0, 0));
	}
	if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
	{
		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCancel(pPerson)) return;
		
		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���{�^��
		//============================================
		if (AttackCancel(pPerson)) return;
		
		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		// �������~�߂�
		pPerson->SetMove(Vector3(0, 0, 0));
	}

	// �A�N�V�����t���[���I�����Ă���A�߂�
	if (!pPerson->isFrameAction())
	{
		if (pPerson->isLand()==true)
		{
			// �ҋ@�X�e�[�g��
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			return;
		}
		else
		{
			// �����X�e�[�g��
			pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
			return;
		}


	}
	
}

void BasePlayerState::OverDrive_OneMore::Exit(BasePlayer * pPerson)
{
	// �Q�[�����~�߂�t���O
	pPerson->SetGameTimerStopFlag(false);


}

void BasePlayerState::OverDrive_OneMore::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "OverDrive_OneMoreState");
}

bool BasePlayerState::OverDrive_OneMore::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �o�����Ă�Ƃ��͖��G
	case MESSAGE_TYPE::HIT_DAMAGE:
	{

									 // �A�N�V�����t���[���������Ă���
									 if (pPerson->isFrameAction())
									 {
										 // �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
										 return true;
									 }


	}

	}
	return false;

}



/*******************************************************/
//					Burst�o���X�e�[�g
/*******************************************************/

void BasePlayerState::OverDrive_Burst::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::OVERDRIVE_BURST);

	// �I�[�o�[�h���C�u
	pPerson->ActionOverDrive(OVERDRIVE_TYPE::BURST);

	// SE�Đ�
	se->Play("�o�[�X�g");


}

void BasePlayerState::OverDrive_Burst::Execute(BasePlayer * pPerson)
{

	// �������~�߂�
	pPerson->SetMove(Vector3(0, 0, 0));

	if (pPerson->GetActionFrame() == FRAME_STATE::ACTIVE)
	{
		// �o�[�X�g�G�t�F�N�g�����I
		pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, 0), EFFECT_TYPE::BURST);
	}

	// �A�N�V�����t���[���I�����Ă���A�߂�
	if (!pPerson->isFrameAction())
	{
		// �ҋ@�X�e�[�g��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());

	}

}

void BasePlayerState::OverDrive_Burst::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::OverDrive_Burst::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "OverDrive_BurstState");
}

bool BasePlayerState::OverDrive_Burst::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �o�����Ă�Ƃ��͖��G
	case MESSAGE_TYPE::HIT_DAMAGE:
	{

									 // �A�N�V�����t���[���������Ă���
									 if (pPerson->isFrameAction())
									 {
										 // �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
										 return true;
									 }


	}

	}

	return false;
}

/*******************************************************/
//					�K�[�h�X�e�[�g
/*******************************************************/

void BasePlayerState::Guard::Enter(BasePlayer * pPerson)
{
	/* ���Ⴊ�݃K�[�h */
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
	{
		pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);

		// �K�[�h�X�e�[�g��ݒ肷��
		pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
	}

	/* �����K�[�h */
	else
	{
		pPerson->SetMotion(MOTION_TYPE::UP_GUARD);

		// �K�[�h�X�e�[�g��ݒ肷��
		pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
	}

}

void BasePlayerState::Guard::Execute(BasePlayer * pPerson)
{	
	// ���d����0�ȉ��Ȃ�d���I��
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		//////////////////////////////////////////////
		//	�U���L�����Z��
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�ǂ������U���L�����Z��
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	����L�����Z��
		//============================================
		if (EscapeCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�����L�����Z��
		//============================================
		if (ThrowCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�؂�L�����Z��
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�_�b�V���L�����Z��
		//============================================
		if (DashCancel(pPerson))return;

		// ����L�������U���X�e�[�g�Ȃ�
		if (pPerson->GetTargetPlayer()->isAttackState() || (pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData()))
		{
			if (isInputGuardCommand(pPerson))
			{
				/* ���Ⴊ�݃K�[�h */
				if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
				{
					pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);

					// �K�[�h�X�e�[�g��ݒ肷��
					pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
					return;
				}

				/* �����K�[�h */
				else
				{
					pPerson->SetMotion(MOTION_TYPE::UP_GUARD);

					// �K�[�h�X�e�[�g��ݒ肷��
					pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
					return;
				}
			}
		}
		// �ҋ@���[�V�����ɖ߂�
		ChangeWaitState(pPerson);

		//// ����L�������U���X�e�[�g�Ȃ�
		//if (pPerson->GetTargetPlayer()->isAttackState() || (pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData()))
		//{
		//	if (isInputGuardCommand(pPerson))
		//	{
		//		/* ���Ⴊ�݃K�[�h */
		//		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
		//		{
		//			pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);

		//			// �K�[�h�X�e�[�g��ݒ肷��
		//			pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
		//		}

		//		/* �����K�[�h */
		//		else
		//		{
		//			pPerson->SetMotion(MOTION_TYPE::UP_GUARD);

		//			// �K�[�h�X�e�[�g��ݒ肷��
		//			pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
		//		}
		//	}
		//}
		//else
		//{
		//	// ���肪�U���X�e�[�g����Ȃ�������
		//	//if (!pPerson->GetTargetPlayer()->isAttackState() && !(pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData()))
		//	{
		//		//pPerson->GuardEffectStop();
		//		//pPerson->SetGuard(false);// �K�[�h��ԉ�����
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		//	}
		//}
	}
}




void BasePlayerState::Guard::Exit(BasePlayer * pPerson)
{
	pPerson->SetGuardState(GUARD_STATE::NO_GUARD);// �K�[�h��ԉ���

	pPerson->GuardEffectStop();
}

void BasePlayerState::Guard::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "GuardState");
}

bool BasePlayerState::Guard::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//if (pPerson->isGuard() == false)return false;

	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// ���K�[�h���͖��G�I
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
									 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

									 bool bGuardSuccess(false);
									 if (pPerson->GetGuardState() == GUARD_STATE::UP_GUARD)
									 {
										 // �����肪�����K�[�h���ĂāA���i�U������Ȃ�������K�[�h�����I
										 if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::DOWN_ATTACK) bGuardSuccess = true;
									 }
									 else if (pPerson->GetGuardState() == GUARD_STATE::DOWN_GUARD)
									 {
										 // �����肪���Ⴊ�݃K�[�h���ĂāA��i�U������Ȃ�������K�[�h�����I
										 if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::UP_ATTACK) bGuardSuccess = true;
									 }

									 // ���K�[�h�������Ă���I�I�I
									 if (bGuardSuccess)
									 {
										 // (A���)�����ɃK�[�h��������SE�A�G�t�F�N�g
										 char *seID;
										 switch (rand() % 3)
										 {
										 case 0:
											 seID = "�K�[�h��";
											 // �K�[�h�ŃQ�[�W�񕜁i���j
											 pPerson->AddOverDriveGage(0.25f);
											 // �U���������葤�͖h�䑤���������l��
											 pPerson->GetTargetPlayer()->AddOverDriveGage(0.5f);
											 break;
										 case 1:
											 seID = "�K�[�h��";
											 // �K�[�h�ŃQ�[�W�񕜁i���j
											 pPerson->AddOverDriveGage(0.5f);
											 // �U���������葤�͖h�䑤���������l��
											 pPerson->GetTargetPlayer()->AddOverDriveGage(0.5f);
											 break;
										 case 2:
											 seID = "�K�[�h��";
											 // �K�[�h�ŃQ�[�W�񕜁i���j
											 pPerson->AddOverDriveGage(1.0f);
											 break;

										 }
										 se->Play(seID);

										 // �K�[�h�̂�����
										 {
											 float fGuardKnockBack(HitDamageInfo->fGuardKnockBackPower);
											 if (pPerson->GetDir() == DIR::RIGHT) fGuardKnockBack *= -1;

											 Vector3 vKnockBackMove(Collision::CheckMove(pPerson, Vector3(fGuardKnockBack, 0, 0)), 0, 0);
											 pPerson->AddMove(vKnockBackMove);

											 if (HitDamageInfo->iAttribute == (int)ATTACK_ATTRIBUTE::STRIKE)
											 {
												 // �ǂ��Ƃ������Ɉړ���������
												 pPerson->GetTargetPlayer()->AddMove(Vector3(-(fGuardKnockBack - vKnockBackMove.x), 0, 0));
											 }
										 }


										 // �R���{�p �K�[�h�����̂𑗂�
										 COMBO_DESK comboDesk;
										 comboDesk.side = pPerson->GetSide();
										 comboDesk.damage = HitDamageInfo->damage;
										 comboDesk.recoveryFrame = 0;
										 MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_GUARD, &comboDesk);
									 }
									 // �K�[�h���s
									 else
									 {
										 // �E�B�[�N�G�t�F�N�g�����I
										 Vector2 screenPos;
										 screenPos = Math::WorldToScreen(pPerson->GetCenterPos());
										 pPerson->GetPanelEffectManager()->AddEffect((int)screenPos.x, (int)screenPos.y - 128, PANEL_EFFECT_TYPE::WEAK);

										 return false;	// �_���[�W�����Ăǂ���
									 }


									 // ���t�B�j�b�V���A�[�c�͒ʂ�
									 //if (HitDamageInfo->bOverDrive == true)
									 //{
									 //	 pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::GUARD_BREAK);
									 //	 return false;
									 //}

									 // // �󂯂�Z�̃��x���ɂ��K�[�h�̍d����ω��@�iGGX���j
									 // const float len = HitDamageInfo->FlyVector.Length();
									 // int RecoveryFrame = 0;
									 // if (len < HUTTOBI_LINE)
									 // {
									 //	 // ���x��1�@�@�iGGX���j
									 //	 RecoveryFrame = 12*2;
									 // }
									 // else
									 // {
									 //	 // ���x��2�@�@�iGGX���j
									 //	 RecoveryFrame = 18 * 2;
									 // }

									 // (�ǉ�)�K�[�h�G�t�F�N�g����!  �����Ƒ���̏ꏊ�̃x�N�g����n��
									 Vector3 vVec= pPerson->GetTargetPlayer()->GetPos() - pPerson->GetPos();
									 vVec.Normalize();
									 pPerson->AddEffectAction(pPerson->GetFlontPos(), EFFECT_TYPE::GUARD_WAVE, vVec);

									 // �Z�̍U���Ɉˑ��@(�X�}�u����)
									 //int RecoveryFrame2 = (int)(HitDamageInfo->recoveryFlame *0.3f)+2;

									 // �Z���Ƃɐݒ肳��Ă���d���t���[����ݒ�
									 pPerson->SetRecoveryFrame(HitDamageInfo->GuardRecoveryFrame);

									 // �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
									 return true;
	}

	}
	return false;
}


/*******************************************************/
//					�����X�e�[�g
/*******************************************************/

void BasePlayerState::Throw::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::THROW);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::THROW);

	// �ށ[��0
	pPerson->SetMove(VECTOR_ZERO);

	// SE�Đ�
	se->Play("�͂�");
}

void BasePlayerState::Throw::Execute(BasePlayer * pPerson)
{	
	// �����I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// �͂�ł���
	if (pPerson->GetAttackData()->bHit)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::ThrowHold::GetInstance());
	}
}

void BasePlayerState::Throw::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::Throw::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowState");
}

bool BasePlayerState::Throw::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//	if(pPerson->isActiveFrame()) return true;	// �_���[�W���b�Z�[�W�K������
	//	break;
	//}

	return false;
}


/*******************************************************/
//					�����z�[���h�X�e�[�g
/*******************************************************/

void BasePlayerState::ThrowHold::Enter(BasePlayer * pPerson)
{
	pPerson->GetTargetPlayer()->GetThrowMark()->Action();

	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);
}

void BasePlayerState::ThrowHold::Execute(BasePlayer * pPerson)
{
	// �����P�\�I��
	if (pPerson->GetCurrentFrame() > BasePlayer::c_THROW_ESCAPE_FRAME)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::ThrowSuccess::GetInstance());
		return;
	}
}

void BasePlayerState::ThrowHold::Exit(BasePlayer * pPerson)
{
	pPerson->GetTargetPlayer()->GetThrowMark()->Stop();
}

void BasePlayerState::ThrowHold::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowHoldState");
}

bool BasePlayerState::ThrowHold::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		//case MESSAGE_TYPE::THROW_SUCCESS:	// ��������񂾂�Ƃ������b�Z�[�W(�����ɒ͂܂ꂽ����ɂ����b�Z�[�W�������Ă���)
		//	pPerson->SetThrowSuccess(true);	// �͂ݐ����t���OON�I
		//	return true;
		//	break;
	case MESSAGE_TYPE::CAN_THROW_RELEASE:	// �͂�ł�l����A�����������Ă��Ƒ����Ă��郁�b�Z�[�W
											// �P�\�t���[���������������
											// �������������������Е������������ł��ĕЕ������������ł��Ȃ��BDeviceID�֘A����
											//if (pPerson->GetHitStopFrame() > 0)// �������͐��l�������ɂ������ق�����������
	{
		MsgMgr->Dispatch(0, pPerson->GetID(), msg.Sender, MESSAGE_TYPE::THROW_RELEASE, nullptr);	// �����ɓ�������OK�Ƒ���Ԃ�

																									// ������(A���)���͉��Ŏ������������������X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(SuccessThrowRelease::GetInstance());
	}
	return true;
	break;
	// �U����������惁�b�Z�[�W
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//	if(pPerson->GetAttackData) return true;	// �_���[�W���b�Z�[�W�K������
	//	break;
	}

	return false;
}


/*******************************************************/
//					���������X�e�[�g
/*******************************************************/

void BasePlayerState::ThrowSuccess::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::THROW_SUCCESS);

	// �A�N�V�����𐬌����[�h��
	pPerson->SetActionState(BASE_ACTION_STATE::THROW_SUCCESS);

	BasePlayer *you(pPerson->GetTargetPlayer());

	// �������ł��܂ꂽ����̍��W�����񂾑���̒͂ݍ��W�Ɉړ�(���ʂɑ������ƕǔ�������̂ŁA�ǔ��������낱���ł���Ă��܂����Ƃ�����)
	if (pPerson->GetDir() == DIR::LEFT  && pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) ||
		pPerson->GetDir() == DIR::RIGHT && pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		// �������t�ɂ���
		pPerson->ReverseDir();

		// ����̍��W�Ǝ����̍��W�����ւ���
		Vector3 temp(pPerson->GetPos());
		pPerson->SetPos(you->GetPos());
		you->SetPos(temp);
	}

	//const Vector3 p(pPerson->GetPos());
	//Vector3 v(pPerson->GetFlontPos() - p);
	//v.y = 0;
	//Vector3 v2(v);
	//
	//// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
	//if (Collision::CheckMove(pPerson, v) - v.x == 0)
	//{
	//	pPerson->GetPosAddress()->x -= v2.x;
	//	you->SetPos(pPerson->GetPos() + v2);
	//}
	//else you->SetPos(p + v);
}

void BasePlayerState::ThrowSuccess::Execute(BasePlayer * pPerson)
{
	// �����I�����Ă���
	if (!pPerson->isAttackState())
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}
	// �q�b�g���Ă���
	else if (pPerson->GetAttackData()->bHit)
	{
		/* �L�����Z�����[�g */

		//////////////////////////////////////////////
		//	�X�L���L�����Z��
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	�I�[�o�[�h���C�u�L�����Z��
		//============================================
		if (OverDriveCancel(pPerson)) return;
	}
}

void BasePlayerState::ThrowSuccess::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::ThrowSuccess::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowSuccessState");
}

bool BasePlayerState::ThrowSuccess::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �U����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_DAMAGE:
		return true;	// �_���[�W���b�Z�[�W�K������
		break;
	}

	return false;
}


/*******************************************************/
//					�͂܂ꂽ�X�e�[�g
/*******************************************************/

void BasePlayerState::ThrowBind::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKBACK);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

	// move�l0
	pPerson->SetMove(VECTOR_ZERO);

	// �d�͂Ƃ���������
	pPerson->SetMoveUpdate(false);

	// �y���\�i�u���C�N
	pPerson->GetStand()->Break();

	// �͂�SE�������ōĐ�
	se->Play("�͂ݐ���");

	// �o�[�X�g�g�p�s��
	pPerson->GetTargetPlayer()->ActionNotOverDrive();

	// ������ꂽ���̃G�t�F�N�g
	//pPerson->GetThrowMark()->Action();
	pPerson->AddEffectAction(pPerson->GetPos() , EFFECT_TYPE::THROW);

	COMBO_DESK comboDesk;
	comboDesk.side = pPerson->GetSide();
	comboDesk.damage = 0;
	comboDesk.recoveryFrame = BasePlayer::c_THROW_ESCAPE_FRAME;
	comboDesk.bCounter = isDamageCounterStatePrev(pPerson);		// �͂܂��
}

void BasePlayerState::ThrowBind::Execute(BasePlayer * pPerson)
{
	// ����ˑ��Ȃ̂Ŏ������炩���邱�Ƃ͂ł��Ȃ��B�����_���[�W�X�e�[�g�ɍs���ƐM����

	// ���������{�^��
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::A))
	{
		// �͂�ł��ɑ΂��ē��������������ƃ��b�Z�[�W�𑗂�(�P�\�t���[�����Ȃ甲���Ă���������b�Z�[�W�𑗂�Ԃ��Ă����)
		if(!isDamageCounterStatePrev(pPerson)) MsgMgr->Dispatch(0, pPerson->GetID(), pPerson->GetTargetPlayer()->GetID(), MESSAGE_TYPE::CAN_THROW_RELEASE, nullptr);
	}
}

void BasePlayerState::ThrowBind::Exit(BasePlayer * pPerson)
{
	// �d�͂Ƃ��̈ړ��ʂ�L��������
	pPerson->SetMoveUpdate(true);

	// ���������P�\�G�t�F�N�g�i�r�b�N���}�[�N�j���~�߂�
	//pPerson->GetThrowMark()->Stop();
}

void BasePlayerState::ThrowBind::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowBindState");
}

bool BasePlayerState::ThrowBind::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �����Ă�����ƒ͂񂾓��Ƃ����郉�I�J����
	case MESSAGE_TYPE::THROW_RELEASE:
		// SE�Đ�
		se->Play("��������");
		pPerson->GetFSM()->ChangeState(BasePlayerState::SuccessThrowRelease::GetInstance());
		return true;
		break;
	}

	return false;
}


/*******************************************************/
//				�͂݉����������Ă͂����X�e�[�g
/*******************************************************/

void BasePlayerState::SuccessThrowRelease::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::THROW_DRAW);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// �����x�̐ݒ�
	static const float pow(2.0f);	// ���ړ��ʂ̗�
	pPerson->SetMove(Vector3((pPerson->GetDir() == DIR::LEFT) ? pow : -pow, 0, 0));	// �����Ă���Ɣ��Ε����Ɉړ�����
}

void BasePlayerState::SuccessThrowRelease::Execute(BasePlayer * pPerson)
{
	// ���������p�V���I���t���[��
	if (pPerson->GetCurrentFrame() >= BasePlayer::c_THROW_RELEASE_FRAME)
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::SuccessThrowRelease::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::SuccessThrowRelease::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "SuccessThrowReleaseState");
}

bool BasePlayerState::SuccessThrowRelease::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//				�q�[�z�[�h���C�u
/*******************************************************/

void BasePlayerState::HeavehoDrive::Enter(BasePlayer * pPerson)
{
	// �q�[�z�[�p�ϐ�������
	pPerson->HeavehoDriveInit();

	// �w�i�ύX
	bool bDriveStage(true);
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::OVER_DRIVE_STAGE, &bDriveStage);

	// SE
	se->Play("�K�E����");

	// �K�E�n���G�t�F�N�g
	pPerson->AddEffectAction(pPerson->GetCenterPos(),EFFECT_TYPE::OVER_DRIVE_ACTION);

	// ���K�E�̓t�H���[�܂Ŗ��G
	pPerson->SetInvincible(114514, 1);
}

void BasePlayerState::HeavehoDrive::Execute(BasePlayer * pPerson)
{
	// ���̃L�����N�^�[�̃q�[�z�[�̍X�V
	const bool bEnd(pPerson->HeavehoDriveUpdate());

	// �I�������
	if (bEnd)
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// �t�H���[�ɂȂ����疳�G����
	if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW) pPerson->InvincibleOff();
}

void BasePlayerState::HeavehoDrive::Exit(BasePlayer * pPerson)
{
	// �ꉞ���G����������
    pPerson->InvincibleOff();

	// �q�[�z�[�̏I���
	pPerson->HeavehoDriveExit();

	// ����̃o�[�X�g�g�p�\��
	pPerson->GetTargetPlayer()->StopNotOverDrive();

	// �w�i�ύX
	bool bDriveStage(false);
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::OVER_DRIVE_STAGE, &bDriveStage);
}

void BasePlayerState::HeavehoDrive::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDrive");
}

bool BasePlayerState::HeavehoDrive::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::HIT_ATTACK:
		pPerson->HeavehoDriveHitEvent();
		break;
	}

	return false;
}

/*******************************************************/
//				���K�E�Z�X�e�[�g
/*******************************************************/

void BasePlayerState::HeavehoDriveOverFlow::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE_OVERFLOW);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW);

	// �q�[�z�[�h���C�u�I�[�o�[�t���[�p�ϐ�������
	pPerson->HeavehoDriveOverFlowInit();

	// SE
	se->Play("���K�E����");
}

void BasePlayerState::HeavehoDriveOverFlow::Execute(BasePlayer * pPerson)
{
	// ���̃L�����N�^�[�̃q�[�z�[�̍X�V
	pPerson->HeavehoDriveOverFlowUpdate();

	// �U���X�e�[�g�I�������
	if (!pPerson->isAttackState())
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HandleMessage�ɈڐA
	//static bool flag = false;
	//// �q�b�g���Ă��琬���E���o�X�e�[�g�ɍs��
	//if (pPerson->GetAttackData()->bHit)
	//{
	//	if (flag == false)
	//	{
	//		flag = true;
	//		// �J�b�g�C���������b�Z�[�W�𑗂�
	//		CUTIN_TYPE_NAME data = CUTIN_TYPE_NAME::AIROU;
	//		MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CUTIN_MGR, MESSAGE_TYPE::CUTIN_ACTION, &data);
	//
	//		// �V�[�����C���X�e�[�g�ɑ΂��ċȕύX���b�Z�[�W
	//		MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START, nullptr);
	//		return;
	//	}
	//}
}

void BasePlayerState::HeavehoDriveOverFlow::Exit(BasePlayer * pPerson)
{
	// �q�[�z�[�̏I���
	pPerson->HeavehoDriveExit();
	pPerson->SetOverDriveFrame(0);
}

void BasePlayerState::HeavehoDriveOverFlow::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDriveOverFlow");
}

bool BasePlayerState::HeavehoDriveOverFlow::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �U���������Ă���
	case MESSAGE_TYPE::HIT_ATTACK:
	{
									 pPerson->HeavehoDriveOverFlowSuccessInit();	// �K�E�̃^�[�Q�b�g�ƂȂ鑊��̎��̂��������̂Ƃ��ɓ˂�����
									 // �J�b�g�C���������b�Z�[�W�𑗂�
									 CUTIN_TYPE_NAME data = CUTIN_TYPE_NAME::AIROU;
									 MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CUTIN_MGR, MESSAGE_TYPE::CUTIN_ACTION, &data);

									 // �V�[�����C���X�e�[�g�ɑ΂��ċȕύX���b�Z�[�W
									 MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START, nullptr);

									 // return true�͂��Ȃ��BSP�Q�[�W�֘A�̏�����GlobalState�̃��b�Z�[�W���s���Ă��邽��
									 break;
	}
	}
	return false;
}

/*******************************************************/
//				���K�E�Z_�����X�e�[�g
/*******************************************************/

void BasePlayerState::HeavehoDriveOverFlow_Success::Enter(BasePlayer * pPerson)
{
	// �V�[�����C���ɐ����������Ƃ�`����
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT, nullptr);

	// �v���C���[�}�l�[�W���[�ɂ��`����
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::PLAYER_MGR, MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT, nullptr);

	// �J�����ɃG�t�F�N�g�J�����̔��������肢����
	EFFECT_CAMERA_INFO EffectCameraInfo;
	EffectCameraInfo.scriptID = (int)pPerson->GetOverFlowCameraID();
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &EffectCameraInfo);

	// �I�u�W�F�N�g�ύX
	pPerson->ChangeHeaveHoDriveOverFlowObj();

	// �A���O���Œ�
	pPerson->SetAngleY(PI);

	// ���_�ɒu��
	pPerson->SetPos(VECTOR_ZERO);
}

void BasePlayerState::HeavehoDriveOverFlow_Success::Execute(BasePlayer * pPerson)
{
	// ���_�ɒu��
	pPerson->SetPos(VECTOR_ZERO);

	pPerson->HeavehoDriveOverFlowSuccessUpdate();
}

void BasePlayerState::HeavehoDriveOverFlow_Success::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::HeavehoDriveOverFlow_Success::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDriveOverFlow_Success");
}

bool BasePlayerState::HeavehoDriveOverFlow_Success::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//				WIN�X�e�[�g
/*******************************************************/

void BasePlayerState::Win::Enter(BasePlayer * pPerson)
{
	// UI�Ƀ��b�Z�[�W�𑗂�
	SIDE side(pPerson->GetSide());
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::UI_MGR, MESSAGE_TYPE::APP_WIN_ICON, &side);

	// �J�����Ƀ��b�Z�[�W�𑗂�
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::WINNER_CAMERA, pPerson->GetPosAddress());

	// �������Ȃ��A�N�V����
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// �������[�V����
	pPerson->SetMotion(MOTION_TYPE::WIN);

	// �����Ă鐔�J�E���g
	pPerson->AddWinNum(1);

	// ���ʂ�����
	pPerson->SetAngleY(PI);
}

void BasePlayerState::Win::Execute(BasePlayer * pPerson)
{
	if (pPerson->GetCurrentFrame() == BasePlayer::c_WINNER_TIME || pPerson->isAnyPushKey())
	{
		// �V�[�����C���ɏI������惁�b�Z�[�W
		MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_WINNER, nullptr);
	}
}

void BasePlayerState::Win::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::Win::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "Win");
}

bool BasePlayerState::Win::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//				�󒆃_�b�V���X�e�[�g
/*******************************************************/

void BasePlayerState::AerialDash::Enter(BasePlayer * pPerson)
{
	// �Ƃ肠�����t���[�������Ă݂�
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// �_�b�V�����[�V����
	pPerson->SetMotion(MOTION_TYPE::AERIAL_DASH);

	// SE
	se->Play("�󒆃_�b�V��");

	// �󒆃W�����v�̌����r��
	pPerson->SetAerialJump(false);

	// �󒆃_�b�V���t���O
	pPerson->SetAerialDash(true);
	// �^�C�}�[
	pPerson->SetAerialDashFrame(16);

	const float fMoveY(pPerson->GetMove().y);
	const float fDashSpeed(pPerson->GetAerialDashSpeed());

	// �ړ�
	if (pPerson->GetDir() == DIR::RIGHT)
	{
		// ������傢
		pPerson->SetMove(Vector3(fDashSpeed, 0, 0));
	}
	else
	{
		// ������傢
		pPerson->SetMove(Vector3(-fDashSpeed, 0, 0));
	}

	if (fMoveY > 0)pPerson->AddMove(Vector3(0, fMoveY, 0));

	// �󒆃_�b�V���G�t�F�N�g
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::AERIAL_JUMP);

}

void BasePlayerState::AerialDash::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�󒆍U���L�����Z��
	//============================================
	if (AerialAttackCancel(pPerson)) return;

	if (pPerson->GetAerialDashFrame() <= 0)
	{
		// �n��A�󒆂ŕ���
		if (pPerson->isLand())pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		else pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
	}

	//// �ړ��n
	//// �ړ�
	//if (pPerson->GetDir() == DIR::RIGHT)
	//{
	//	// ������傢
	//	pPerson->SetMove(Vector3(1.5f, 0, 0));
	//}
	//else
	//{
	//	// ������傢
	//	pPerson->SetMove(Vector3(-1.5f, 0, 0));
	//}

}

void BasePlayerState::AerialDash::Exit(BasePlayer * pPerson)
{
	// �󒆃_�b�V���t���O�I�t
	//pPerson->SetAerialDash(false);

}

void BasePlayerState::AerialDash::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "AerialDash");
}

bool BasePlayerState::AerialDash::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//				�󒆃o�b�N�_�b�V���X�e�[�g
/*******************************************************/

void BasePlayerState::AerialBackDash::Enter(BasePlayer * pPerson)
{
	// �_�b�V�����[�V����
	pPerson->SetMotion(MOTION_TYPE::BACK_STEP);

	// �󒆃W�����v�̌����r��
	pPerson->SetAerialJump(false);

	// SE
	se->Play("�󒆃_�b�V��");

	// �Ƃ肠�����t���[�������Ă݂�
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// �󒆃_�b�V���t���O
	pPerson->SetAerialDash(true);
	// �^�C�}�[
	pPerson->SetAerialDashFrame(14);

	const float MoveY(pPerson->GetMove().y);

	// �ړ�
	if (pPerson->GetDir() == DIR::RIGHT)
	{
		// ������傢
		pPerson->SetMove(Vector3(-.75f, 0, 0));
	}
	else
	{
		// ������傢
		pPerson->SetMove(Vector3(.75f, 0, 0));
	}

	if (MoveY > 0)pPerson->AddMove(Vector3(0, MoveY, 0));

	// �󒆃_�b�V���G�t�F�N�g
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::AERIAL_JUMP);
}

void BasePlayerState::AerialBackDash::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�󒆍U���L�����Z��
	//============================================
	//if (AerialAttackCancel(pPerson)) return;

	if (pPerson->GetAerialDashFrame() <= 0)
	{
		// �n��A�󒆂ŕ���
		if(pPerson->isLand())pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		else 
			pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
	}

	//// �ړ��n
	//// �ړ�
	//if (pPerson->GetDir() == DIR::RIGHT)
	//{
	//	// ������傢
	//	pPerson->SetMove(Vector3(1.5f, 0, 0));
	//}
	//else
	//{
	//	// ������傢
	//	pPerson->SetMove(Vector3(-1.5f, 0, 0));
	//}

}

void BasePlayerState::AerialBackDash::Exit(BasePlayer * pPerson)
{
	// �󒆃_�b�V���t���O�I�t
	//pPerson->SetAerialDash(false);

}

void BasePlayerState::AerialBackDash::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "AerialBackDash");
}

bool BasePlayerState::AerialBackDash::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}


/*******************************************************/
//				�C���g�����o�p�X�e�[�g
/*******************************************************/

void BasePlayerState::Intro::Enter(BasePlayer * pPerson)
{
	// �C���g���p���[�V����
	pPerson->SetMotion(MOTION_TYPE::APPEAR);

	// �L�������Ƃ̊|�������̏�����
	pPerson->KakeaiInit();
}

void BasePlayerState::Intro::Execute(BasePlayer * pPerson)
{
	// �L�������Ƃ̊|�������̍X�V
	pPerson->KakeaiUpdate();
}

void BasePlayerState::Intro::Exit(BasePlayer * pPerson)
{
	// �L�������Ƃ̊|�������̏I��
	pPerson->KakeaiExit();
}

void BasePlayerState::Intro::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "Intro");
}

bool BasePlayerState::Intro::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::KAKEAI_END:
		pPerson->GetFSM()->ChangeState(Wait::GetInstance());
		return true;
		break;
	}
	return false;
}

/*******************************************************/
//				�ǂ������U�� (�O�i��)
/*******************************************************/

//void BasePlayerState::StrongAttack::Enter(BasePlayer * pPerson)
//{
//	//�@���[�V�����Z�b�g
//	pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK3);
//
//	// �U���X�e�[�g��3�i�ڂɐݒ肷��
//	pPerson->SetActionState(BASE_ACTION_STATE::RUSH3);
//
//
//
//
//}
//
//void BasePlayerState::StrongAttack::Execute(BasePlayer * pPerson)
//{
//
//	// �U���I�����Ă���
//	if (!pPerson->isAttackState())
//	{
//		// �ҋ@���[�V�����ɖ߂�
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		return;
//	}
//
//	// �q�b�g���Ă���
//	if (pPerson->GetAttackData()->bHit)
//	{
//		/* �L�����Z�����[�g */
//		//////////////////////////////////////////////
//		//	�X�L���L�����Z��
//		//============================================
//		if (SkillCancel(pPerson)) return;
//
//		//////////////////////////////////////////////
//		//	�X�^���h�L�����Z��
//		//============================================
//		if (StandCancel(pPerson)) return;
//
//		//////////////////////////////////////////////
//		//	�q�[�z�[�h���C�u�L�����Z��
//		//============================================
//		if (HeaveHoCancel(pPerson)) return;
//
//		//////////////////////////////////////////////
//		//	�q�[�z�[�h���C�u�I�[�o�[�t���[�L�����Z��
//		//============================================
//		if (HeaveHoOverFlowCancel(pPerson)) return;
//
//		// �U���I�����Ă���
//		if (!pPerson->isAttackState())
//		{
//			// �ҋ@���[�V�����ɖ߂�
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		}
//	}
//
//}
//
//void BasePlayerState::StrongAttack::Exit(BasePlayer * pPerson)
//{
//	// �󒆃_�b�V���t���O�I�t
//	//pPerson->SetAerialDash(false);
//
//}
//
//void BasePlayerState::StrongAttack::Render(BasePlayer * pPerson)
//{
//	tdnText::Draw(20, 690, 0xffffffff, "StrongAttack");
//}
//
//bool BasePlayerState::StrongAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	// ���b�Z�[�W�^�C�v
//	return false;
//}
