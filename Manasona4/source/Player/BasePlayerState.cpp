#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"

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
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return true;
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
		pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
		return true;
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
	if (pPerson->isPushInput(PLAYER_INPUT::D))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Skill::GetInstance());
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
	
		// �U����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
		// �A�N�V�����X�e�[�g�I�t(�܂茻�݂܂ł��������������������)
		pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

		HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

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
	pPerson->SetMotion(0);

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
	if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	{
		// �G�X�P�[�v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return;
	}


	//////////////////////////////////////////////
	//	�U���{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		// �U���X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
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
	//	���Ⴊ�݃L�����Z��
	//============================================
	if (SquatCansel(pPerson)) return;


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
	pPerson->SetMotion(1);

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
	if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	{
		// �G�X�P�[�v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	�t�B�j�b�V���U���{�^��
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (RushAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	�X�^���h�L�����Z��
	//============================================
	if (StandCansel(pPerson)) return;

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
	pPerson->SetMotion(1);

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
	if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	{
		// �G�X�P�[�v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	�U���L�����Z��
	//============================================
	if (RushAttackCansel(pPerson)) return;

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
				pPerson->GetFSM()->ChangeState(BasePlayerState::FrontBrake::GetInstance());
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
	pPerson->SetMotion(18);

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
	tdnText::Draw(20, 690, 0xffffffff, "TurnOverBrakeState");
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
	pPerson->SetMotion(18);

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
	pPerson->SetMotion(10);

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
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{
			// �󒆃W�����v�̌������Ȃ�������e��
			if (pPerson->isAerialJump() == true)
			{
				// �󒆃W�����v�X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			}
		}

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
	pPerson->SetMotion(13);

	// �󒆃W�����v�̌������g�p
	pPerson->SetAerialJump(false);

	// �ړ��n�𑫂��I
	Vector3 set(pPerson->GetMove() * .5f);//c_MAX_AerialJump
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		set.x -= .05f;
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		set.x += .05f;
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
	pPerson->SetMotion(13);

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
	//	�󒆃W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �󒆃W�����v�̌������Ȃ�������e��
		if (pPerson->isAerialJump() == true)
		{
			// �󒆃W�����v�X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		}
	}

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
	pPerson->SetMotion(13);

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
	pPerson->SetMotion(2);

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
					pPerson->SetMotion(4);

					// �U���X�e�[�g��2�i�ڂɐݒ肷��
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH2);

					pPerson->GetRushAttack()->step++;
					//pPerson->GetRushAttack()->bNextOK = false;

					const float pow = 0.5f;
					pPerson->AddMove((pPerson->GetDir() == DIR::RIGHT) ? Vector3(pow, 0, 0) : Vector3(-pow, 0, 0));
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
					pPerson->SetMotion(6);

					// �U���X�e�[�g��3�i�ڂɐݒ肷��
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH3);

					pPerson->GetRushAttack()->step++;
					//pPerson->GetRushAttack()->bNextOK = false;
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
	pPerson->SetMotion(16);
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
		
		// [�ǋL]�@���G�͂����ł��Ȃ��I
		// �����G���Ԑݒ�
		//pPerson->SetInvincible(8);
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
	pPerson->SetMotion(17);

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
			pPerson->isPushInput(PLAYER_INPUT::D))
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
	pPerson->SetMotion(18);

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
		if (pPerson->isPushInput(PLAYER_INPUT::A) ||
			pPerson->isPushInput(PLAYER_INPUT::B) ||
			pPerson->isPushInput(PLAYER_INPUT::C) ||
			pPerson->isPushInput(PLAYER_INPUT::D))
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
	pPerson->SetMotion(13);

	// �肩�o���[������Ƃ�������
	pPerson->SetRecoveryCount(0);

	// ���J�o�[���͖��G�I�I�I
	pPerson->SetInvincible(1);

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
	pPerson->InvincibleOff();

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
	pPerson->SetMotion(13);

	// �肩�o���[������Ƃ�������
	pPerson->SetRecoveryCount(0);

	// ���J�o�[���͖��G�I�I�I
	pPerson->SetInvincible(1);

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
	pPerson->InvincibleOff();

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
	pPerson->SetMotion(8);

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
	pPerson->SetMotion(9);


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

		//============================================
		//	�W�����v�{�^��
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{

			if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
			{
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));		
			}
			if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
			{
				pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
			}

			// ������΂� 
			pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			// �W�����v�X�e�[�g�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
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
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(11);


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
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{

			//if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
			//{
			//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
			//}
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
			//{
			//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
			//}

			//// ������΂� 
			//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		
			// �󒆃W�����v�̌������Ȃ�������e��
			if (pPerson->isAerialJump() == true)
			{
				// �󒆃W�����v�X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			}
			
			return;
		}

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
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(12);


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
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{

			//if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
			//{
			//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
			//}
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
			//{
			//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
			//}

			//// ������΂� 
			//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			// �󒆃W�����v�̌������Ȃ�������e��
			if (pPerson->isAerialJump() == true)
			{
				// �󒆃W�����v�X�e�[�g�ɍs��
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			}
			return;
		}

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
	// �ҋ@���[�V�����ɕς���
	pPerson->SetMotion(13);

	// ���G�X�P�[�v�̃t���[���ɐݒ肷��
	pPerson->SetActionState(BASE_ACTION_STATE::ESCAPE);

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
			pPerson->SetMove(Vector3(.25, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.25, 0, 0));
		}
		break;

	case FRAME_STATE::ACTIVE:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(true);
		// �N�C�b�N�G�X�P�[�v
		// �����Ă�������ɃX�D�[���ƈړ�
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(1.5f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-1.5f, 0, 0));
		}
		break;

	case FRAME_STATE::FOLLOW:
		// �t���O�ݒ�
		pPerson->SetEscapeFlag(false);
		// �N�C�b�N�G�X�P�[�v
		// �����Ă�������ɃX�D�[���ƈړ�
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(.25, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.25, 0, 0));
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
	pPerson->SetMotion(15);

	// �A�N�V�����X�e�[�g�ύX
	pPerson->SetActionState(BASE_ACTION_STATE::STAND);

	// ���X�^���h�A�N�V��������
	pPerson->GetStand()->Action(Stand::ACTION_TYPE::LAND);

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
	ParticleManager::EffectPersonaAura(pPerson->GetPos());

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
				pPerson->AddMove(Vector3(-0.35f, 0, 0));
			}
			else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT) == true)
			{
				pPerson->AddMove(Vector3(0.35f, 0, 0));
			}

			return;
		}

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
	// �������[�V�����ɕς���
	pPerson->SetMotion(7);

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
	// �������[�V�����ɕς���
	pPerson->SetMotion(6);

	// �A�N�V�����X�e�[�g���L�����N�^�[�ŗL�ɕύX
	pPerson->SetActionState(BASE_ACTION_STATE::SKILL);

	// SE�Đ�
	se->Play("�y���\�i����");

}

void BasePlayerState::Skill::Execute(BasePlayer * pPerson)
{

	// ���̃L�����N�^�[�̃X�L���̍X�V
	pPerson->SkillUpdate();


	// �U���X�e�[�g�I�������
	if (pPerson->GetActionFrame()==FRAME_STATE::END)
	{
		// �ҋ@�X�e�[�g��(�������ɃL�����Z���͂�߂Ă�����)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::Skill::Exit(BasePlayer * pPerson)
{


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
