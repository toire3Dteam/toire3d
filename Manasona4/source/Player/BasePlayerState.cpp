#include "BasePlayer.h"
#include "BasePlayerState.h"


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
}

bool BasePlayerState::Global::OnMessage(BasePlayer * pPerson, const Message & msg)
{
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
	//	�W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
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

	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	}
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
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
	// ���郂�[�V�����ɕς���
	pPerson->SetMotion(1);

	// �����x�̐ݒ�
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x -= .5f;
		break;
	case DIR::RIGHT:
		move.x += .5f;
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
	//	�W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
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
}

void BasePlayerState::Run::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
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
	// �������u��
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
		if (
			abs(move.x) < BasePlayer::c_FRONT_BRAKE_LINE
			)
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
	// �u���[�L���[�V�����ɕς���
	pPerson->SetMotion(0);
}

void BasePlayerState::FrontBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

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
	// �u���[�L���[�V�����ɕς���
	pPerson->SetMotion(18);
}

void BasePlayerState::TurnOverBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	�W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

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
	// �u���[�L���[�V�����ɕς���
	pPerson->SetMotion(8);

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
		if(pPerson->isPushInput(PLAYER_INPUT::C))
		{
			// �v���C���[�������Ă鎞�Ԍv��
			pPerson->GetJump()->PlayerHoldTimer++;
		}

		if (++pPerson->GetJump()->HoldTimer > 3)
		{
			// �����x�̐ݒ�
			Vector3 add(0, 0, 0);

			// ���Ⴊ��ł�ԁA�����Ɖ����Ă��̂ŁA�ő�W�����v
			if (pPerson->GetJump()->HoldTimer == pPerson->GetJump()->PlayerHoldTimer)
			{
				add.y = BasePlayer::c_MAX_JUMP;
			}
			// ��������Ȃ��̂ŏ��W�����v
			else
			{
				add.y = BasePlayer::c_MAX_JUMP * .65f;
			}

			// ���E����
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))		add.x = -.25f;
			//else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT)) add.x = .25f;

			// �ړ��n�𑫂��I
			pPerson->AddMove(add);

			// �n��t���O�I�t�I
			pPerson->SetLand(false);

			// ���Ⴊ�݃t���O�I��
			pPerson->GetJump()->bHold = false;
		}
	}

	//////////////////////////////////////////////
	//	�W�����v��
	//============================================
	else
	{

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


		// �n��t���Otrue(���n������)
		if (pPerson->isLand())
		{
			// ���n�X�e�[�g�Ɉڍs
			pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
		}
	}
}

void BasePlayerState::Jump::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::Jump::Render(BasePlayer * pPerson)
{
	//tdnText::Draw(20, 690, 0xffffffff, "JumpState");
}

bool BasePlayerState::Jump::OnMessage(BasePlayer * pPerson, const Message & msg)
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
	// �u���[�L���[�V�����ɕς���
	pPerson->SetMotion(12);

	// ���n���ԏ�����
	pPerson->GetJump()->LandTimer = 0;

	// �ړ��n����
	pPerson->MoveClampX(pPerson->GetMaxSpeed());
}

void BasePlayerState::Land::Execute(BasePlayer * pPerson)
{
	// ��莞�Ԓ��n������
	if (++pPerson->GetJump()->LandTimer > 5)
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

	//////////////////////////////////////////////
	//	�W�����v�{�^��
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3 && pPerson->GetJump()->LandTimer > 3)
	{
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}
}

void BasePlayerState::Land::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::Land::Render(BasePlayer * pPerson)
{
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
	pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH1);
}

void BasePlayerState::RushAttack::Execute(BasePlayer * pPerson)
{
	switch (pPerson->GetRushAttack()->step)
	{
	case 0:	// 1�i��
		// �q�b�g���Ă���
		if (pPerson->GetRushAttack()->bHit)
		{
			// �U���{�^����������
			if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
			{
				// ���̃��[�V�����Z�b�g

				// �U���X�e�[�g��2�i�ڂɐݒ肷��
				pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH2);

				pPerson->GetRushAttack()->step++;
				pPerson->GetRushAttack()->bHit = false;
			}
		}
		// �U���I�����Ă���
		if (!pPerson->isAttackState())
		{
			// �ҋ@���[�V�����ɖ߂�
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
		break;
	case 1:	// 2�i��
		// �q�b�g���Ă���
		if (pPerson->GetRushAttack()->bHit)
		{
			// �U���{�^����������
			if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
			{
				// ���̃��[�V�����Z�b�g

				// �U���X�e�[�g��3�i�ڂɐݒ肷��
				pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH3);

				pPerson->GetRushAttack()->step++;
				pPerson->GetRushAttack()->bHit = false;
			}
		}
		// �U���I�����Ă���
		if (!pPerson->isAttackState())
		{
			// �ҋ@���[�V�����ɖ߂�
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
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
	// ���U���X�e�[�g����߂�����
	pPerson->SetAttackState(BASE_ATTACK_STATE::END);
}

void BasePlayerState::RushAttack::Render(BasePlayer * pPerson)
{
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::RushAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �U�����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_ATTACK:
	{
										HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
										pPerson->GetRushAttack()->bHit = true;
										break;
	}
		// �U����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
										HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
										break;
	}
	default:
		return true;
		break;
	}

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}
