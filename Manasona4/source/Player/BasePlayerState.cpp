#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

// �萔
static const float HUTTOBI_LINE = 3.0f;
static const float REPEAT_ATTACK_RATE = 0.85f;		// �����U��������ȃ��[�g

/*******************************************************/
//	�t�@���N�V����
/*******************************************************/
bool JumpCansel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�{�^���ŃW�����v�L�����Z��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3 || pPerson->GetInputList(PLAYER_INPUT::UP) == 3)
	{
		// �n�ʂɂ��Ă�����
		if (pPerson->isLand() == true)
		{
			// �W�����v�X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
			return true;
		}
		else if (pPerson->isAerialJump() == true)	// �󒆂ŋ󒆃W�����v�̌����������
		{
			// �󒆃W�����v�X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			return true;
		}
		else return false;
	
	}
	else return false;
}

bool RunCansel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	���E�L�[�ő���L�����Z��
	//============================================
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		return true;
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		return true;
	}
	else return false;
}

bool SquatCansel(BasePlayer * pPerson)
{
	// ���Ⴊ�݃L�����Z��
	if (pPerson->isPushInput(PLAYER_INPUT::DOWN))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		return true;
	}
	else return false;
}

bool StandCansel(BasePlayer * pPerson)
{
	// �X�^���h�L�����Z��
	if (pPerson->isPushInput(PLAYER_INPUT::L1))
	{
		// �ǋL�@�X�^���h�X�g�b�N���Ȃ��Ƃ��̓L�����Z���ł��Ȃ�
		if (pPerson->GetStand()->GetStandStock() <= 0)return false;
		pPerson->GetFSM()->ChangeState(BasePlayerState::StandAction::GetInstance());
		return true;
	}
	else return false;
}

bool RushAttackCansel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		if (pPerson->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
			return true;
		}
		else
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
			return true;
		}
	}
	else return false;
}

bool FinishAttackCansel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (pPerson->isPushInput(PLAYER_INPUT::R2))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::FinishAttack::GetInstance());
		return true;
	}
	else return false;
}

bool SkillCansel(BasePlayer *pPerson)
{
	// �U���L�����Z��
	if (pPerson->GetInputList(PLAYER_INPUT::D) == 3)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Skill::GetInstance());
		return true;
	}
	else return false;
}

bool OverDriveCansel(BasePlayer *pPerson)
{
	if ((pPerson->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()) == true && pPerson->GetActionFrame() != FRAME_STATE::FOLLOW) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::ThrowMiss::GetInstance()) == true ||
		pPerson->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true)// �����Ă�Ƃ��͊���
		return false;


	// �o���i�o�[�X�g�j�L�����Z��
	if (pPerson->GetInputList(PLAYER_INPUT::R3)==3)
	{
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

bool EscapeCansel(BasePlayer *pPerson)
{
	//if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	//{
	//	// �G�X�P�[�v�X�e�[�g�ɍs��
	//	pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
	//	return true;
	//}
	//else return false;

	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		pPerson->SetDir(DIR::LEFT);// ������ς���
		// �G�X�P�[�v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return true;
	}
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		
		pPerson->SetDir(DIR::RIGHT);// ������ς���
		// �G�X�P�[�v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return true;
	}
	else return false;
}

bool ThrowCansel(BasePlayer * pPerson)
{
	// �����L�����Z��
	if (pPerson->GetInputList(PLAYER_INPUT::L2) == 3)
	{
		// �n�ʂɂ��Ă��Ȃ��ƂȂ��Ƃ��̓L�����Z���ł��Ȃ�
		if (pPerson->isLand() == false)return false;
		pPerson->GetFSM()->ChangeState(BasePlayerState::Throw::GetInstance());
		return true;
	}
	else return false;
}

bool GuardCansel(BasePlayer * pPerson)
{
	// �K�[�h�L�����Z��
	if (pPerson->isPushInput(PLAYER_INPUT::R1))
	{
		// �n�ʂɂ��Ă��Ȃ��ƂȂ��Ƃ��̓L�����Z���ł��Ȃ�
		if (pPerson->isLand() == false)return false;
		pPerson->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());
		return true;
	}
	else return false;
}

bool HeaveHoCansel(BasePlayer * pPerson)
{
	// �K�E�Z�L�����Z��
	if (pPerson->GetInputList(PLAYER_INPUT::A) == 3)
	{	
		pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDrive::GetInstance());
		return true;
	}
	else return false;
}

bool HeaveHoOverFlowCansel(BasePlayer * pPerson)
{
	if (pPerson->isOverDrive() == false)return false;

	// ���K�E�Z�L�����Z��
	if (pPerson->GetInputList(PLAYER_INPUT::R3) == 3)
	{
		if ((pPerson->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()) == true && pPerson->GetActionFrame() != FRAME_STATE::FOLLOW) ||
			pPerson->GetFSM()->isInState(*BasePlayerState::ThrowMiss::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true || 
			pPerson->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true || 
			pPerson->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == true
			)// �����Ă�Ƃ��͊���
			return false;

		pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDriveOverFlow::GetInstance());
		return true;
	}
	else return false;
}

/*******************************************************/
//					�O���[�o���X�e�[�g
/*******************************************************/

BasePlayerState::Global * BasePlayerState::Global::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Global instance;
	return &instance;
}

void BasePlayerState::Global::Enter(BasePlayer * pPerson)
{
}

void BasePlayerState::Global::Execute(BasePlayer * pPerson)
{
	////////////////////////////////////////////////////////////////
	//	�I�[�o�[�h���C�u���݂̂ł��钴�K�E�Z�@
	//==============================================================
	if (HeaveHoOverFlowCansel(pPerson)) return;

	////////////////////////////////////////////////////////////////
	//	�I�[�o�[�h���C�u�@�ǂ�����ł��o����ŋ��̃L�����Z�����[�g
	//==============================================================
	if (OverDriveCansel(pPerson)) return;
	


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
		BE_THROWN_INFO *ThrowInfo = (BE_THROWN_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		pPerson->SetThrowPlayerID(ThrowInfo->ThrowPlayerID);
		pPerson->SetDir((ThrowInfo->iThrowPlayerDir == (int)DIR::LEFT) ? DIR::RIGHT : DIR::LEFT);	// �v�͒͂񂾑���̕�������

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

		// �A�N�V�����X�e�[�g�I�t(�܂茻�݂܂ł��������������������)
		pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

		HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

		// �������U�����ꂽ�����֌����悤��
		if (hdi->FlyVector.x > 0.0f)	pPerson->SetDir(DIR::LEFT);
		else		pPerson->SetDir(DIR::RIGHT);

		// ������уx�N�g�������Z (A���)���Z�������ɕύX
		//pPerson->AddMove(Vector3(hdi->LandFlyVector.x, hdi->LandFlyVector.y, 0));	
		pPerson->SetMove(Vector3(hdi->FlyVector.x, hdi->FlyVector.y, 0));

		// �q�b�g�X�g�b�v
		pPerson->SetHitStopFrame(hdi->hitStopFlame);

		// �d���t���[���ݒ�
		int RecoveryFrame = hdi->recoveryFlame;
		for (auto it : *pPerson->GetRecoveryDamageCount())
		{
			if (it == (BASE_ACTION_STATE)hdi->iAttackType)
			{
				RecoveryFrame = (int)(RecoveryFrame * REPEAT_ATTACK_RATE);	// ���Z�␳
			}
		}
		pPerson->SetRecoveryFrame(RecoveryFrame);

		// ��������U�����X�g�ǉ�
		pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);

		// (�ǉ�)�q�b�g�G�t�F�N�g����! 
		// (A���)�G�t�F�N�g�����͎����̃|�W�V�����ɂ����Ă�I�I�I�@��Ńq�b�g�|�W�V�����Ƃ����邩��
		pPerson->AddEffectAction(pPerson->GetPos()+Vector3(0,4,-3),(EFFECT_TYPE)hdi->HitEffectType);

		// �󒆃W�����v�̌�������
		pPerson->SetAerialJump(true);

		// �U���ɑ���𖳓G�ɂ����鑮���������Ă���
		if (hdi->BeInvincible)
		{
			//�ނĂ�������
			// ������ς肱���Ŗ��G���Ԑݒ�
			pPerson->SetInvincible(90, 1);
		}

		// ������ы���	�Ɂ@������	�_���[�W�X�e�[�g��ς���
		const float len = hdi->FlyVector.Length();
		if (len <  HUTTOBI_LINE)
		{
			// �����ŋ󒆂ł�����Ă邩�n��ł�����Ă邩�𕪂���
			if (pPerson->isLand() == true)
			{
				// �n��m�b�N�o�b�N�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::KnockBack::GetInstance());
			}
			else
			{
				// �󒆃m�b�N�o�b�N�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialKnockBack::GetInstance());
			}
		
		}
		else
		{
			// �m�b�N�_�E���ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::KnockDown::GetInstance());

		}

		return true;
		break;
	}

	// �U�����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_ATTACK:
	{
		HIT_ATTACK_INFO *HitAttackInfo = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		pPerson->SetHitStopFrame(HitAttackInfo->hitStopFlame);// �q�b�g�X�g�b�v
		pPerson->AddCollectScore(HitAttackInfo->HitScore);	// ���̑O�̃X�R�A�����Z
		if (HitAttackInfo->bFinishAttack) pPerson->ConversionScore();	// �t�B�j�b�V���A�^�b�N�Ȃ�X�R�A���Y
		break;
	}


	// (A���)���񂾂����b�Z�[�W
	}

	return false;
}


/*******************************************************/
//					�ҋ@�X�e�[�g
/*******************************************************/

BasePlayerState::Wait * BasePlayerState::Wait::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Wait instance;
	return &instance;
}

void BasePlayerState::Wait::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::WAIT);

}

void BasePlayerState::Wait::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	//if (EscapeCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���{�^��
	//============================================
	if (RushAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCansel(pPerson)) return;
	
	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	if (GuardCansel(pPerson)) return;
	

	//////////////////////////////////////////////
	//	�ړ��{�^��
	//============================================
	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
		// �����X�e�[�g�ɂ���
		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	}
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
		// �����X�e�[�g�ɂ���
		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	}



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

BasePlayerState::Walk * BasePlayerState::Walk::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Walk instance;
	return &instance;
}

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
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	//if (EscapeCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (RushAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	if (GuardCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCansel(pPerson)) return;

	if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		pPerson->SetDir(DIR::RIGHT);
		return;
	}

	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		pPerson->SetDir(DIR::LEFT);
		return;
	}

	if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 1)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::RIGHT);
	}
	else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 1)
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::LEFT);
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
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * .7f);
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
//					����X�e�[�g
/*******************************************************/

BasePlayerState::Run * BasePlayerState::Run::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Run instance;
	return &instance;
}

void BasePlayerState::Run::Enter(BasePlayer * pPerson)
{
	// ���郂�[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::RUN);

	// ����G�t�F�N�g�����I
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::RUN);

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
}

void BasePlayerState::Run::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�X�L���L�����Z��
	//============================================
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�G�X�P�[�v�{�^��
	//============================================
	//if (EscapeCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (RushAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�����L�����Z��
	//============================================
	if (ThrowCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�K�[�h�L�����Z��
	//============================================
	if (GuardCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	��
	//============================================
	// �������u��
	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		// ���Ε���
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetDir(DIR::LEFT);
			// �u���[�L�X�e�[�g��
			pPerson->GetFSM()->ChangeState(BasePlayerState::TurnOverBrake::GetInstance());
		}
	}
	// �����Ă��
	else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 1)
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::LEFT);

	}

	//////////////////////////////////////////////
	//	�E
	//============================================
	// �������u�� ���_��@�����A���������Ƃ͂��肦�Ȃ��@�j���[�g����������
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		// ���Ε���
		if (pPerson->GetDir() == DIR::LEFT)
		{
			pPerson->SetDir(DIR::RIGHT);
			// �u���[�L�X�e�[�g��
			pPerson->GetFSM()->ChangeState(BasePlayerState::TurnOverBrake::GetInstance());
		}
	}
	// �����Ă��
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 1)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::RIGHT);
	}


	else // �j���[�g����
	{
		Vector3 move = pPerson->GetMove();
		// ������x����������A�O�u���[�L�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
		//if (abs(move.x) < BasePlayer::c_FRONT_BRAKE_LINE)
		{
			// �O�̃X�e�[�g��U�^�[���u���[�L��������A�u�ҋ@�v�X�e�[�g�ɖ߂�(�^�[���u���[�L�̌�Ƀt�����g�u���[�L���������Ă��܂�����)
			if (pPerson->GetFSM()->isPrevState(*BasePlayerState::TurnOverBrake::GetInstance()))
				pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			else
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
//					�t�����g�u���[�L�X�e�[�g
/*******************************************************/

BasePlayerState::FrontBrake * BasePlayerState::FrontBrake::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::FrontBrake instance;
	return &instance;
}

void BasePlayerState::FrontBrake::Enter(BasePlayer * pPerson)
{
	// �O�u���[�L���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BRAKE);

	// SE�Đ�
	se->Play("�u���[�L1");
}

void BasePlayerState::FrontBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	Vector3 move = pPerson->GetMove();
	
	// A���: MOVE�l�������Œ��������Ă���
	move *= .95f;
	pPerson->SetMove(move);

	// �قڎ~�܂��Ă���A(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
	if (
		//move.Length() < .1f
		abs(move.x) < BasePlayer::c_END_MOVE_LINE
		)
	{
		// �����L�[����͂��Ă�����A�ēx�u����v�X�e�[�g��
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->SetDir(DIR::LEFT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->SetDir(DIR::RIGHT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}

		// ���E�����L�[�������ĂȂ�������A�ҋ@�X�e�[�g��
		else
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

	}
}

void BasePlayerState::FrontBrake::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::FrontBrake::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "�t�����gBrakeState");
}

bool BasePlayerState::FrontBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
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
//					U�^�[���u���[�L�X�e�[�g
/*******************************************************/

BasePlayerState::TurnOverBrake * BasePlayerState::TurnOverBrake::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::TurnOverBrake instance;
	return &instance;
}

void BasePlayerState::TurnOverBrake::Enter(BasePlayer * pPerson)
{
	// U�^�[���u���[�L���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::U_TURN);

	// SE�Đ�
	se->Play("�u���[�L1");
}

void BasePlayerState::TurnOverBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	Vector3 move = pPerson->GetMove();

	// �قڎ~�܂��Ă���AWait�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
	if (
		//move.Length() < .1f
		abs(move.x) < BasePlayer::c_END_MOVE_LINE
		)
	{
		// �����L�[����͂��Ă�����A�ēx�u����v�X�e�[�g��
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}

		// ���E�����L�[�������ĂȂ�������A�ҋ@�X�e�[�g��
		else
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

	}
}

void BasePlayerState::TurnOverBrake::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::TurnOverBrake::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "TurnOverBrakeState");
}

bool BasePlayerState::TurnOverBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
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
//					�W�����v�X�e�[�g
/*******************************************************/

BasePlayerState::Jump * BasePlayerState::Jump::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Jump instance;
	return &instance;
}

void BasePlayerState::Jump::Enter(BasePlayer * pPerson)
{
	// �W�����v���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::JUMP);

	// ���Ⴊ�݃t���O������
	pPerson->GetJump()->Clear();
}

void BasePlayerState::Jump::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�����O�̂��Ⴊ�ݒ�
	//============================================
	if (pPerson->GetJump()->bHold)
	{
		if (pPerson->isPushInput(PLAYER_INPUT::C))
		{
			// �v���C���[�������Ă鎞�Ԍv��
			pPerson->GetJump()->PlayerHoldTimer++;
		}

		if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
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
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))		add.x = -.25f;
			//else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT)) add.x = .25f;

			// �ړ��n�𑫂��I
			pPerson->AddMove(add);

			// �n��t���O�I�t�I
			//pPerson->SetLand(false);

			// ���Ⴊ�݃t���O�I��
			pPerson->GetJump()->bHold = false;

			// ���Ⴊ��ł�Œ��ɍU���{�^���������Ă���
			if (pPerson->GetJump()->bAttackPush)
			{
				if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
				{
					// �󒆉��U���X�e�[�g�ɍs��
					pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
				}
				else
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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���{�^��
		//============================================
	
		if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
		{
			if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
			{
				// �󒆉��U���X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
			}
			else
			{
				// �󒆍U���X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
			}
		
			return;
		}

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (FinishAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�󒆃W�����v�{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		//{
		//	// �󒆃W�����v�̌������Ȃ�������e��
		//	if (pPerson->isAerialJump() == true)
		//	{
		//		// �󒆃W�����v�X�e�[�g�ɍs��
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		//	}
		//}
		if (JumpCansel(pPerson))return;

		//////////////////////////////////////////////
		//	������
		//============================================
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->AddMove(Vector3(-0.03f, 0.0f, 0.0f));
		}


		//////////////////////////////////////////////
		//	�E����
		//============================================
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->AddMove(Vector3(0.03f, 0.0f, 0.0f));
		}

		// �ړ��n����
		pPerson->MoveClampX(pPerson->GetMaxSpeed() * 0.8f);
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

BasePlayerState::AerialJump * BasePlayerState::AerialJump::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::AerialJump instance;
	return &instance;
}

void BasePlayerState::AerialJump::Enter(BasePlayer * pPerson)
{
	// �W�����v���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::JUMP);

	// �󒆃W�����v�̌������g�p
	pPerson->SetAerialJump(false);

	// �ړ��n�𑫂��I
	Vector3 set(pPerson->GetMove() * .5f);//c_MAX_AerialJump
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		set.x -= .075f;
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		set.x += .075f;
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
	if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���{�^��
		//============================================

		if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
		{
			if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
			{
				// �󒆉��U���X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
			}
			else
			{
				// �󒆍U���X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
			}

			return;
		}

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (FinishAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	������
		//============================================
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->AddMove(Vector3(-0.025f, 0.0f, 0.0f));
		}


		//////////////////////////////////////////////
		//	�E����
		//============================================
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->AddMove(Vector3(0.025f, 0.0f, 0.0f));
		}

		// �ړ��n����
		pPerson->MoveClampX(pPerson->GetMaxSpeed() * 0.8f);


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

BasePlayerState::Fall * BasePlayerState::Fall::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Fall instance;
	return &instance;
}

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
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCansel(pPerson)) return;


	//////////////////////////////////////////////
	//	�U���{�^��
	//============================================

	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
		{
			// �󒆉��U���X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
		}
		else
		{
			// �󒆍U���X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		return;
	}

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	������
	//============================================
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->AddMove(Vector3(-0.05f, 0.0f, 0.0f));
	}


	//////////////////////////////////////////////
	//	�E����
	//============================================
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->AddMove(Vector3(0.05f, 0.0f, 0.0f));
	}

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

BasePlayerState::Land * BasePlayerState::Land::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Land instance;
	return &instance;
}

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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCansel(pPerson))
			return;

		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���L�����Z��
		//============================================
		if (RushAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (FinishAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	���Ⴊ�݃L�����Z��
		//============================================
		if (SquatCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�͂���L�����Z��
		//============================================
		if (RunCansel(pPerson)) return;
	}

	// ��莞�Ԓ��n������
	if (++pPerson->GetJump()->LandTimer > 12)
	{
		// �����L�[����͂��Ă�����A�u����v�X�e�[�g��
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->SetDir(DIR::LEFT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->SetDir(DIR::RIGHT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
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

BasePlayerState::RushAttack * BasePlayerState::RushAttack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::RushAttack instance;
	return &instance;
}

void BasePlayerState::RushAttack::Enter(BasePlayer * pPerson)
{
	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK1);

	// ���b�V���U���\���̏�����
	pPerson->GetRushAttack()->Clear();

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
	switch (pPerson->GetRushAttack()->step)
	{
	case 0:	// 1�i��
			// �U���I�����Ă���
		if (!pPerson->isAttackState())
		{
			// �ҋ@���[�V�����ɖ߂�
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

		// �q�b�g���Ă���
		else if (pPerson->GetAttackData()->bHit)
		{
			/* �L�����Z�����[�g */
			
			//////////////////////////////////////////////
			//	�X�L���L�����Z��
			//============================================
			if (SkillCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	�X�^���h�L�����Z��
			//============================================
			if (StandCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	���Ⴊ�݃L�����Z��
			//============================================
			if (SquatCansel(pPerson)) return;

			// �t�H���[���[�V����(���̍U����)
			if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				// �U���{�^����������(�������ςȂ�����)
				if (pPerson->isPushInput(PLAYER_INPUT::B))
				{
					// ���̃��[�V�����Z�b�g
					pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK2);

					// �U���X�e�[�g��2�i�ڂɐݒ肷��
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH2);

					pPerson->GetRushAttack()->step++;
					//pPerson->GetRushAttack()->bNextOK = false;

					// �����Ă�����ɃL�[����͂���ƈړ�
					const float pow = 0.8f;	
					if(pPerson->GetDir() == DIR::RIGHT)
					{
						if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))// �����L�[����͂��Ă���
						pPerson->AddMove(Vector3(pow, 0, 0));
					}
					else
					{
						if (pPerson->isPushInput(PLAYER_INPUT::LEFT)) // �����L�[����͂��Ă���
						pPerson->AddMove(Vector3(-pow, 0, 0));
					}
						
				}
			}
		}

		break;
	case 1:	// 2�i��

		// �U���I�����Ă���
		if (!pPerson->isAttackState())
		{
			// �ҋ@���[�V�����ɖ߂�
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

		// �q�b�g���Ă���
		else if (pPerson->GetAttackData()->bHit)
		{
			/* �L�����Z�����[�g */
			//////////////////////////////////////////////
			//	�X�L���L�����Z��
			//============================================
			if (SkillCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	�X�^���h�L�����Z��
			//============================================
			if (StandCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	���Ⴊ�݃L�����Z��
			//============================================
			if (SquatCansel(pPerson)) return;

			// �t�H���[���[�V����(���̍U����)
			if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				// �U���{�^����������(�������ςȂ�����)
				if (pPerson->isPushInput(PLAYER_INPUT::B))
				{
					// ���̃��[�V�����Z�b�g
					pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK3);

					// �U���X�e�[�g��3�i�ڂɐݒ肷��
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH3);

					pPerson->GetRushAttack()->step++;
					//pPerson->GetRushAttack()->bNextOK = false;

					//const float pow = 0.2f;
					//pPerson->AddMove((pPerson->GetDir() == DIR::RIGHT) ? Vector3(pow, 0, 0) : Vector3(-pow, 0, 0));

					// �����Ă�����ɃL�[����͂���ƈړ�
					const float pow = 0.4f;
					if (pPerson->GetDir() == DIR::RIGHT)
					{
						if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))// �����L�[����͂��Ă���
							pPerson->AddMove(Vector3(pow, 0, 0));
					}
					else
					{
						if (pPerson->isPushInput(PLAYER_INPUT::LEFT)) // �����L�[����͂��Ă���
							pPerson->AddMove(Vector3(-pow, 0, 0));
					}
				}
			}
		}

		break;
	case 2:	// 3�i��
		// �U���I�����Ă���
		if (!pPerson->isAttackState())
		{
			// �ҋ@���[�V�����ɖ߂�
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
		break;
	}
}

void BasePlayerState::RushAttack::Exit(BasePlayer * pPerson)
{
	// �U���X�e�[�g����߂�����
	//pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);
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

BasePlayerState::KnockBack * BasePlayerState::KnockBack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::KnockBack instance;
	return &instance;
}


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
	//								 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//								 // �����Ă��Ԃł�������U�����X�g�ǉ�
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);
	//								 // true�͕Ԃ��Ȃ��B�����ł́A�_�E����Ԃł̂�������J�E���g����邾���Ȃ̂ŁB
	//}
	//}
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�G�A���A���m�b�N�o�b�N�X�e�[�g
/*******************************************************/

BasePlayerState::AerialKnockBack * BasePlayerState::AerialKnockBack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::AerialKnockBack instance;
	return &instance;
}

void BasePlayerState::AerialKnockBack::Enter(BasePlayer * pPerson)
{
	// �󒆃m�b�N�o�b�N���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKDOWN_AERIAL);

}

void BasePlayerState::AerialKnockBack::Execute(BasePlayer * pPerson)
{
	// �d����0�ȉ��Ȃ�d���I��
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// �d����0�ɂȂ�����@�Ȃ��������ꂩ�̃{�^���������Ă����畜�A�X�e�[�g��
		if (pPerson->isPushInput(PLAYER_INPUT::A)||
			pPerson->isPushInput(PLAYER_INPUT::B)||
			pPerson->isPushInput(PLAYER_INPUT::C)||
			pPerson->isPushInput(PLAYER_INPUT::D)||
			pPerson->isPushInput(PLAYER_INPUT::R1))
		{

			// �n��I�A��
			if (pPerson->isLand() == true)
			{
				// �n�ナ�J�o���[�X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
				// �������J�E���g���Z�b�g
				pPerson->GetRecoveryDamageCount()->clear();
			}
			else
			{
				// �󒆃��J�o���[�X�e�[�g�ցI
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialRecovery::GetInstance());
				// �������J�E���g���Z�b�g
				pPerson->GetRecoveryDamageCount()->clear();
			}

		}
	}
}

void BasePlayerState::AerialKnockBack::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::AerialKnockBack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "�I�肦��m�b�N�o�b�N");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::AerialKnockBack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
		//{
		//	// �U����������惁�b�Z�[�W
		//case MESSAGE_TYPE::HIT_DAMAGE:
		//{
		//								 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		//								 // �����Ă��Ԃł�������U�����X�g�ǉ�
		//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);
		//								 // true�͕Ԃ��Ȃ��B�����ł́A�_�E����Ԃł̂�������J�E���g����邾���Ȃ̂ŁB
		//}
		//}
		// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
		return false;
}



/*******************************************************/
//					�m�b�N�_�E���X�e�[�g
/*******************************************************/

BasePlayerState::KnockDown * BasePlayerState::KnockDown::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::KnockDown instance;
	return &instance;
}

void BasePlayerState::KnockDown::Enter(BasePlayer * pPerson)
{
	// �m�b�N�_�E�����[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKDOWN_RAND);

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
		return;
	}

	// �d����0�ȉ��Ȃ�d���I��
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		//pPerson->SetInvincible(60, 1);

		if (pPerson->isPushInput(PLAYER_INPUT::A) ||
			pPerson->isPushInput(PLAYER_INPUT::B) ||
			pPerson->isPushInput(PLAYER_INPUT::C) ||
			pPerson->isPushInput(PLAYER_INPUT::D) ||
			pPerson->isPushInput(PLAYER_INPUT::R1)  )
		{
			// �ҋ@�X�e�[�g�ɖ߂�
			pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());

			// �������J�E���g���Z�b�g
			pPerson->GetRecoveryDamageCount()->clear();
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
	//								 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
	//								 // �����Ă��Ԃł�������U�����X�g�ǉ�
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);
	//								 // true�͕Ԃ��Ȃ��B�����ł́A�_�E����Ԃł̂�������J�E���g����邾���Ȃ̂ŁB
	//}
	//}
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/*******************************************************/
//					�n�ナ�J�o���[�X�e�[�g
/*******************************************************/

BasePlayerState::LandRecovery * BasePlayerState::LandRecovery::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::LandRecovery instance;
	return &instance;
}

void BasePlayerState::LandRecovery::Enter(BasePlayer * pPerson)
{
	// ���J�o���[�ɕς���
	pPerson->SetMotion(MOTION_TYPE::WAIT);

	// �肩�o���[������Ƃ�������
	pPerson->SetRecoveryCount(0);

	// ���J�o�[���͖��G�I�I�I
	//pPerson->SetInvincibleLV(1);
	pPerson->SetInvincible(12, 1);

	// �G�t�F�N�g�����v
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3), EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::LandRecovery::Execute(BasePlayer * pPerson)
{
	pPerson->AddRecoveryCount(1);

	// ���J�o���[�t���[���𒴂�����I���I
	if (pPerson->GetRecoveryCount() >= BasePlayer::c_RECOVERY_FLAME)
	{
		// �ҋ@�X�e�[�g��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());

		// �������J�E���g���Z�b�g
		pPerson->GetRecoveryDamageCount()->clear();
	}
}

void BasePlayerState::LandRecovery::Exit(BasePlayer * pPerson)
{
	// ���G�����I�I
	//pPerson->InvincibleOff();

	// �肩�o���[������Ƃ�������
	pPerson->SetRecoveryCount(0);
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

BasePlayerState::AerialRecovery * BasePlayerState::AerialRecovery::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::AerialRecovery instance;
	return &instance;
}

void BasePlayerState::AerialRecovery::Enter(BasePlayer * pPerson)
{
	// ���J�o���[�ɕς���
	pPerson->SetMotion(MOTION_TYPE::FALL);

	// �肩�o���[������Ƃ�������
	pPerson->SetRecoveryCount(0);

	// ���J�o�[���͖��G�I�I�I
	//pPerson->SetInvincibleLV(1);
	pPerson->SetInvincible(90, 1);

	Vector3 move;
	move = pPerson->GetMove()*0.25f;
	pPerson->SetMove(move);

	// �{�l������
	Vector3 addMove = Vector3(0, 2.55f, 0);

	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		addMove += Vector3(0.7f, 0, 0);
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		addMove += Vector3(-0.7f, 0, 0);
	}
	
	pPerson->AddMove(addMove);

	// �G�t�F�N�g�����v
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3) , EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::AerialRecovery::Execute(BasePlayer * pPerson)
{
	pPerson->AddRecoveryCount(1);

	// ���J�o���[�t���[���𒴂�����I���I
	if (pPerson->GetRecoveryCount() >= BasePlayer::c_RECOVERY_FLAME)
	{
		// �󒆂Ȃ̂ŗ�����X�e�[�g��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());

		// �������J�E���g���Z�b�g
		pPerson->GetRecoveryDamageCount()->clear();
	}
}

void BasePlayerState::AerialRecovery::Exit(BasePlayer * pPerson)
{
	// ���G�����I�I
	//pPerson->InvincibleOff();

	pPerson->SetRecoveryCount(0);
}

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

BasePlayerState::Squat * BasePlayerState::Squat::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Squat instance;
	return &instance;
}

void BasePlayerState::Squat::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::SQUAT);

}

void BasePlayerState::Squat::Execute(BasePlayer * pPerson)
{
	//	���Ⴊ�ݍU���{�^��
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		// ���Ⴊ�ݍU���X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�W�����v�L�����Z��
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�{�^��
	//============================================
	if (HeaveHoCansel(pPerson)) return;

	// ���Ⴊ�݃{�^���𗣂�����ҋ@�ɖ߂�
	if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == false)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}

}

void BasePlayerState::Squat::Exit(BasePlayer * pPerson)
{


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

BasePlayerState::SquatAttack * BasePlayerState::SquatAttack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::SquatAttack instance;
	return &instance;
}

void BasePlayerState::SquatAttack::Enter(BasePlayer * pPerson)
{
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::SQUAT_ATTACK);


	// ���U���X�e�[�g��2�i�ڂɐݒ肷��@����HIT�t���O�������Ă���
	pPerson->SetActionState(BASE_ACTION_STATE::SQUAT);

}

void BasePlayerState::SquatAttack::Execute(BasePlayer * pPerson)
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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;


		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�L�����Z��
		//============================================
		if (HeaveHoCansel(pPerson)) return;

		//============================================
		//	�W�����v�{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		//{

		//	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		//	{
		//		pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));		
		//	}
		//	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		//	{
		//		pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		//	}

		//	// ������΂� 
		//	pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		//	// �W�����v�X�e�[�g�ɍs��
		//	pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		//	return;
		//}
		
		if (JumpCansel(pPerson))
		{
			if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		
			if (pPerson->isPushInput(PLAYER_INPUT::LEFT))	
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
//					�󒆍U���X�e�[�g
/*******************************************************/

BasePlayerState::AerialAttack * BasePlayerState::AerialAttack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::AerialAttack instance;
	return &instance;
}

void BasePlayerState::AerialAttack::Enter(BasePlayer * pPerson)
{
	// �󒆍U�����[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::AERIAL_ATTACK);


	// ���U���X�e�[�g��ݒ肷��@����HIT�t���O�������Ă���
	pPerson->SetActionState(BASE_ACTION_STATE::AERIAL);


	// ���͕����Ɍ���
	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
	}
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
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;

		//============================================
		//	�󒆃W�����v�L�����Z���{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		//{

		//	//if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		//	//{
		//	//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		//	//}
		//	//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
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
		if (JumpCansel(pPerson))return;
		

	}


}

void BasePlayerState::AerialAttack::Exit(BasePlayer * pPerson)
{


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

BasePlayerState::AerialDropAttack * BasePlayerState::AerialDropAttack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::AerialDropAttack instance;
	return &instance;
}

void BasePlayerState::AerialDropAttack::Enter(BasePlayer * pPerson)
{
	// �󒆉��U�����[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::AERIALDROP_ATTACK);


	// ���U���X�e�[�g��ݒ肷��@����HIT�t���O�������Ă���
	pPerson->SetActionState(BASE_ACTION_STATE::AERIALDROP);

	// ���͕����Ɍ���
	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
	}

}

void BasePlayerState::AerialDropAttack::Execute(BasePlayer * pPerson)
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
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HIT���Ă�����L�����Z���ł���
	if (pPerson->GetAttackData()->bHit == true)
	{
		//============================================
		//	�W�����v�{�^��
		//============================================
		//if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		//{

		//	//if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		//	//{
		//	//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		//	//}
		//	//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		//	//{
		//	//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		//	//}

		//	//// ������΂� 
		//	//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		//	// �󒆃W�����v�̌������Ȃ�������e��
		//	if (pPerson->isAerialJump() == true)
		//	{
		//		// �󒆃W�����v�X�e�[�g�ɍs��
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		//	}
		//	return;
		//}
		if (JumpCansel(pPerson))return;
	

	}

}

void BasePlayerState::AerialDropAttack::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::AerialDropAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "AerialDropAttackState");
}

bool BasePlayerState::AerialDropAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// ���b�Z�[�W�^�C�v
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
//					����X�e�[�g
/*******************************************************/

BasePlayerState::Escape * BasePlayerState::Escape::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Escape instance;
	return &instance;
}

void BasePlayerState::Escape::Enter(BasePlayer * pPerson)
{
	// ������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::ESCAPE);

	// ���G�X�P�[�v�̃t���[���ɐݒ肷��
	pPerson->SetActionState(BASE_ACTION_STATE::ESCAPE);
	
	// �t�̌���������
	pPerson->TurnOverDir();

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
		if (pPerson->GetDir() != DIR::RIGHT)
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
		if (pPerson->GetDir() != DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(1.75f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-1.75f, 0, 0));
		}
		break;

	case FRAME_STATE::FOLLOW:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(false);
		// �N�C�b�N�G�X�P�[�v
		// �����Ă�������ɃX�D�[���ƈړ�
		if (pPerson->GetDir() != DIR::RIGHT)
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

BasePlayerState::StandAction * BasePlayerState::StandAction::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::StandAction instance;
	return &instance;
}

void BasePlayerState::StandAction::Enter(BasePlayer * pPerson)
{
	// �������[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::PERSONA);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::STAND);

	// ���X�^���h�A�N�V��������
	pPerson->GetStand()->Action(Stand::ACTION_TYPE::LAND);

	// �d�͂̉e���󂯂�ȁI
	pPerson->SetMoveUpdate(false);

	// SE�Đ�
	se->Play("�y���\�i����");

	// �y���\�i�����G�t�F�N�g�I
	pPerson->AddEffectAction(pPerson->GetPos(),EFFECT_TYPE::PERSONA);

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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	���Ⴊ�݃L�����Z��
		//============================================
		if (SquatCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCansel(pPerson))
		{

			if (pPerson->isPushInput(PLAYER_INPUT::LEFT) == true)
			{
				pPerson->AddMove(Vector3(-0.45f, 0, 0));
			}
			else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT) == true)
			{
				pPerson->AddMove(Vector3(0.45f, 0, 0));
			}

			return;
		}
		
		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�L�����Z��
		//============================================
		if (HeaveHoCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	���E�L�[�ő���L�����Z��
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
		{
			pPerson->SetDir(DIR::LEFT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
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
//					�t�B�j�b�V���A�^�b�N�X�e�[�g
/*******************************************************/

BasePlayerState::FinishAttack * BasePlayerState::FinishAttack::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::FinishAttack instance;
	return &instance;
}

void BasePlayerState::FinishAttack::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::FINISH_ATTACK);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::FINISH);

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
}

void BasePlayerState::FinishAttack::Exit(BasePlayer * pPerson)
{

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

BasePlayerState::Skill * BasePlayerState::Skill::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Skill instance;
	return &instance;
}

void BasePlayerState::Skill::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::CHARA_ATTACK_LAND);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::SKILL);

	// �X�L���p�ϐ�������
	pPerson->SkillInit();

	// SE�Đ�
	se->Play("�y���\�i����");

}

void BasePlayerState::Skill::Execute(BasePlayer * pPerson)
{

	// ���̃L�����N�^�[�̃X�L���̍X�V
	pPerson->SkillUpdate();


	//////////////////////////////////////////////
	//	�q�[�z�[�h���C�u�L�����Z��
	//============================================
	if (HeaveHoCansel(pPerson)) return;

	// �U���X�e�[�g�I�������
	if (pPerson->GetActionFrame()==FRAME_STATE::END)
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::Skill::Exit(BasePlayer * pPerson)
{
	// �A�C���[�Ƃ��ŏd�͖��������肷��̂ŁA�ꉞ�����ŉ������Ă���
	pPerson->SetMoveUpdate(true);

	// �X�L���̏I���
	pPerson->SkillExit();
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

BasePlayerState::OverDrive_OneMore * BasePlayerState::OverDrive_OneMore::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::OverDrive_OneMore instance;
	return &instance;
}

void BasePlayerState::OverDrive_OneMore::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::OVERDRIVE_ONEMORE);

	// �I�[�o�[�h���C�u
	pPerson->ActionOverDrive(OVERDRIVE_TYPE::ONEMORE);

	// SE�Đ�
	se->Play("�y���\�i����");

	// ��ɕ���
	pPerson->SetMove(Vector3(0, 1, 0));

	// �o�[�X�g�O�G�t�F�N�g�@�����I
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::ONEMORE_BURST_START);

	// �Q�[�����~�߂�t���O
	pPerson->SetGameTimerStopFlag(true);

}

void BasePlayerState::OverDrive_OneMore::Execute(BasePlayer * pPerson)
{

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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�W�����v�L�����Z��
		//============================================
		if (JumpCansel(pPerson)) return;
		
		//////////////////////////////////////////////
		//	�q�[�z�[�h���C�u�{�^��
		//============================================
		if (HeaveHoCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�U���{�^��
		//============================================
		if (RushAttackCansel(pPerson)) return;
		
		//////////////////////////////////////////////
		//	�t�B�j�b�V���U���{�^��
		//============================================
		if (FinishAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;

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

BasePlayerState::OverDrive_Burst * BasePlayerState::OverDrive_Burst::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::OverDrive_Burst instance;
	return &instance;
}

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
{


}

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

BasePlayerState::Guard * BasePlayerState::Guard::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Guard instance;
	return &instance;
}

void BasePlayerState::Guard::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BURST);

	pPerson->GuardEffectAction();

	// ������
	pPerson->SetGuard(true);// �K�[�h��Ԃ�
	pPerson->SetGuardFollowFrame(0);

}

void BasePlayerState::Guard::Execute(BasePlayer * pPerson)
{	
	// ���d����0�ȉ��Ȃ�d���I��
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		if (pPerson->isGuard())
		{

			//////////////////////////////////////////////
			//	����L�����Z��
			//============================================
			if (EscapeCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	�X�^���h�L�����Z��
			//============================================
			if (StandCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	�W�����v�L�����Z��
			//============================================
			if (JumpCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	�q�[�z�[�h���C�u�{�^��
			//============================================
			if (HeaveHoCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	�����L�����Z��
			//============================================
			if (ThrowCansel(pPerson)) return;

			// �K�[�h�{�^���𗣂�����
			if (pPerson->isPushInput(PLAYER_INPUT::R1) == false)
			{
				pPerson->GuardEffectStop();
				pPerson->SetGuard(false);// �K�[�h��ԉ�����
			}

		}
		else
		{
			// �J�E���g
			pPerson->AddGuardFollowFrame();

			enum { FOLLOE_END = 6 };
			if (pPerson->GetGuardFollowFrame() >= FOLLOE_END)
			{
				pPerson->SetGuardFollowFrame(0);
				pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
				return;
			}	

		}

	}

	// ���������ɍD�����ق���
}

void BasePlayerState::Guard::Exit(BasePlayer * pPerson)
{
	pPerson->SetGuard(false);// �K�[�h��ԉ���

	pPerson->GuardEffectStop();

}

void BasePlayerState::Guard::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "GuardState");
}

bool BasePlayerState::Guard::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	if (pPerson->isGuard() == false)return false;

	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// ���K�[�h���͖��G�I
	case MESSAGE_TYPE::HIT_DAMAGE:
	{

									 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
									 
									 // ���t�B�j�b�V���A�[�c�͒ʂ�
									 if (hdi->bFinishAttack == true)
									 {
										 pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::GUARD_BREAK);
										 return false;
									 }

									 // �󂯂�Z�̃��x���ɂ��K�[�h�̍d����ω��@�iGGX���j
									 const float len = hdi->FlyVector.Length();
									 int RecoveryFrame = 0;
									 if (len < HUTTOBI_LINE)
									 {
										 // ���x��1�@�@�iGGX���j
										 RecoveryFrame = 14;

										 // �������U�����ꂽ�����֌����悤�Ɂ�����
										 if (hdi->FlyVector.x > 0.0f)
										 {
											 pPerson->SetDir(DIR::LEFT);
											 pPerson->SetMove(Vector3(1.0f, 0, 0));
										 }
										 else
										 {
											 pPerson->SetDir(DIR::RIGHT);
											 pPerson->SetMove(Vector3(-1.0f, 0, 0));
										 }
										

									 }
									 else
									 {
										 // ���x��2�@�@�iGGX���j
										 RecoveryFrame = 22;
										 // �������U�����ꂽ�����֌����悤�Ɂ�����
										 if (hdi->FlyVector.x > 0.0f)
										 {
											 pPerson->SetDir(DIR::LEFT);
											 pPerson->SetMove(Vector3(1.5f, 0, 0));
										 }
										 else
										 {
											 pPerson->SetDir(DIR::RIGHT);
											 pPerson->SetMove(Vector3(-1.5f, 0, 0));
										 }
									 }

									 // �Z�̍U���Ɉˑ��@(�X�}�u����)
									 int RecoveryFrame2 = (int)(hdi->recoveryFlame *0.3f)+2;
									 
									 pPerson->SetRecoveryFrame(RecoveryFrame2);

									 // �O���[�o���X�e�[�g�ɍs���Ȃ��悤�ɂ���I
									 return true;
	}

	}
	return false;
}



/*******************************************************/
//					�����X�e�[�g
/*******************************************************/

BasePlayerState::Throw * BasePlayerState::Throw::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::Throw instance;
	return &instance;
}

void BasePlayerState::Throw::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::THROW);

	// SE�Đ�
	se->Play("�͂�");

	// �����t���O������
	pPerson->SetThrowSuccess(false);
}

void BasePlayerState::Throw::Execute(BasePlayer * pPerson)
{		

	// �����ʂ��P�\�t���[��
	if (BasePlayer::c_THROW_ESCAPE_FRAME > pPerson->GetCurrentFrame())
	{
		// �������������łȂ�������A�U�����肪�o��O�Ƀ~�X�����X�e�[�g��
		// ����ThrowSuccess�̓v���C���[�}�l�[�W���[�̓�������֐��̒��ŏ��������Ă���
		if (!pPerson->isThrowSuccess())
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::ThrowMiss::GetInstance());
			return;
		}
	}
	

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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�X�^���h�L�����Z��
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	�I�[�o�[�h���C�u�L�����Z��
		//============================================
		if (OverDriveCansel(pPerson)) return;
	}


}

void BasePlayerState::Throw::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::Throw::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "ThrowState");
}

bool BasePlayerState::Throw::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::THROW_SUCCESS:	// ��������񂾂�Ƃ������b�Z�[�W(�����ɒ͂܂ꂽ����ɂ����b�Z�[�W�������Ă���)
		pPerson->SetThrowSuccess(true);	// �͂ݐ����t���OON�I
		return true;
		break;
	case MESSAGE_TYPE::CAN_THROW_RELEASE:	// �͂�ł�l����A�����������Ă��Ƒ����Ă��郁�b�Z�[�W
		// �P�\�t���[���������������
		if ( pPerson->GetCurrentFrame() < BasePlayer::c_THROW_ESCAPE_FRAME)// �������͐��l�������ɂ������ق�����������
		{
			MsgMgr->Dispatch(0, pPerson->GetID(), msg.Sender, MESSAGE_TYPE::THROW_RELEASE, nullptr);	// �����ɓ�������OK�Ƒ���Ԃ�

			// ������(A���)���͉��Ŏ������������������X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(SuccessThrowRelease::GetInstance());
		}
		return true;
		break;
	}

	return false;
}



/*******************************************************/
//					�~�X�����X�e�[�g
/*******************************************************/

BasePlayerState::ThrowMiss * BasePlayerState::ThrowMiss::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::ThrowMiss instance;
	return &instance;
}

void BasePlayerState::ThrowMiss::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// �t���[���J�E���g�p
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);
}

void BasePlayerState::ThrowMiss::Execute(BasePlayer * pPerson)
{
	// �������s�d���I���t���[��
	if (pPerson->GetCurrentFrame() >= BasePlayer::c_THROW_MISS_FRAME)
	{
		// �ҋ@���[�V�����ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::ThrowMiss::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::ThrowMiss::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "ThrowMissState");
}

bool BasePlayerState::ThrowMiss::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{

	//}

	return false;
}


/*******************************************************/
//					�͂܂ꂽ�X�e�[�g
/*******************************************************/

BasePlayerState::ThrowBind * BasePlayerState::ThrowBind::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::ThrowBind instance;
	return &instance;
}

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
}

void BasePlayerState::ThrowBind::Execute(BasePlayer * pPerson)
{
	// ����ˑ��Ȃ̂Ŏ������炩���邱�Ƃ͂ł��Ȃ��B�����_���[�W�X�e�[�g�ɍs���ƐM����

	// ���������{�^��
	if (pPerson->GetInputList(PLAYER_INPUT::L2) == 3)
	{
		// �͂�ł��ɑ΂��ē��������������ƃ��b�Z�[�W�𑗂�(�P�\�t���[�����Ȃ甲���Ă���������b�Z�[�W�𑗂�Ԃ��Ă����)
		MsgMgr->Dispatch(0, pPerson->GetID(), pPerson->GetThrowPlayerID(), MESSAGE_TYPE::CAN_THROW_RELEASE, nullptr);
	}
}

void BasePlayerState::ThrowBind::Exit(BasePlayer * pPerson)
{
	// �d�͂Ƃ��̈ړ��ʂ�L��������
	pPerson->SetMoveUpdate(true);

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

BasePlayerState::SuccessThrowRelease * BasePlayerState::SuccessThrowRelease::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::SuccessThrowRelease instance;
	return &instance;
}

void BasePlayerState::SuccessThrowRelease::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::KNOCKBACK);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// �����x�̐ݒ�
	static const float pow(1.0f);	// ���ړ��ʂ̗�
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
{


}

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
//				�͂݉����������Ă͂����X�e�[�g
/*******************************************************/

BasePlayerState::HeavehoDrive * BasePlayerState::HeavehoDrive::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::HeavehoDrive instance;
	return &instance;
}

void BasePlayerState::HeavehoDrive::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE);

	// �q�[�z�[�p�ϐ�������
	pPerson->HeavehoDriveInit();

}

void BasePlayerState::HeavehoDrive::Execute(BasePlayer * pPerson)
{
	// ���̃L�����N�^�[�̃q�[�z�[�̍X�V
	pPerson->HeavehoDriveUpdate();

	// �U���X�e�[�g�I�������
	if (!pPerson->isAttackState())
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::HeavehoDrive::Exit(BasePlayer * pPerson)
{
	// �q�[�z�[�̏I���
	pPerson->HeavehoDriveExit();

}

void BasePlayerState::HeavehoDrive::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDrive");
}

bool BasePlayerState::HeavehoDrive::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//				���K�E�Z�X�e�[�g
/*******************************************************/

BasePlayerState::HeavehoDriveOverFlow * BasePlayerState::HeavehoDriveOverFlow::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::HeavehoDriveOverFlow instance;
	return &instance;
}

void BasePlayerState::HeavehoDriveOverFlow::Enter(BasePlayer * pPerson)
{
	// ���[�V�����ɕς���
	pPerson->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE);

	// �q�[�z�[�p�ϐ�������
	pPerson->HeavehoDriveInit();

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

	// �q�b�g���Ă��琬���E���o�X�e�[�g�ɍs��
	if (pPerson->GetAttackData()->bHit)
	{

	}
}

void BasePlayerState::HeavehoDriveOverFlow::Exit(BasePlayer * pPerson)
{
	// �q�[�z�[�̏I���
	pPerson->HeavehoDriveExit();

}

void BasePlayerState::HeavehoDriveOverFlow::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDriveOverFlow");
}

bool BasePlayerState::HeavehoDriveOverFlow::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}

/*******************************************************/
//				���K�E�Z_�����X�e�[�g
/*******************************************************/

BasePlayerState::HeavehoDriveOverFlow_Success * BasePlayerState::HeavehoDriveOverFlow_Success::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::HeavehoDriveOverFlow_Success instance;
	return &instance;
}

void BasePlayerState::HeavehoDriveOverFlow_Success::Enter(BasePlayer * pPerson)
{


}

void BasePlayerState::HeavehoDriveOverFlow_Success::Execute(BasePlayer * pPerson)
{

}

void BasePlayerState::HeavehoDriveOverFlow_Success::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::HeavehoDriveOverFlow_Success::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDriveOverFlow_Success");
}

bool BasePlayerState::HeavehoDriveOverFlow_Success::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	return false;
}