#include "BasePlayer.h"
#include "BasePlayerState.h"

// �萔
static const float HUTTOBI_LINE = 8.0f;

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
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	
		// �U����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
		// �U���X�e�[�g�I�t
		pPerson->SetAttackState(BASE_ATTACK_STATE::NO_ATTACK);

		HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��

		// ������уx�N�g�������Z (A���)���Z�������ɕύX
		//pPerson->AddMove(Vector3(hdi->FlyVector.x, hdi->FlyVector.y, 0));
		pPerson->SetMove(Vector3(hdi->FlyVector.x, hdi->FlyVector.y, 0));

		// �q�b�g�X�g�b�v
		pPerson->SetHitStopFrame(hdi->hitStopFlame);

		// �d���t���[���ݒ�
		pPerson->SetRecoveryFrame(hdi->recoveryFlame);

		// �󒆃W�����v�̌�������
		pPerson->SetAerialJump(true);

		// ������ы���	�Ɂ@������	�_���[�W�X�e�[�g��ς���
		if (hdi->FlyVector.Length() <  HUTTOBI_LINE)
		{
			// �m�b�N�o�b�N�ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::KnockBack::GetInstance());

		}
		else
		{
			// �m�b�N�_�E���ɍs��
			pPerson->GetFSM()->ChangeState(BasePlayerState::KnockDown::GetInstance());

		}

		return true;
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
	//	�ړ��{�^��
	//============================================
	// ���Ⴊ��
	if (pPerson->isPushInput(PLAYER_INPUT::DOWN))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		return;
	}
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
	pPerson->SetMotion(14);

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

	// ���Ⴊ��
	if (pPerson->isPushInput(PLAYER_INPUT::DOWN))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
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
	pPerson->SetMotion(0);

	// �󒆃W�����v�̌������g�p
	pPerson->SetAerialJump(false);

	// �ړ��n�𑫂��I
	Vector3 add(VECTOR_ZERO);//c_MAX_AerialJump
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		add.x = -.25f;
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		add.x = .25f;
	}
	add.y = 2.25f;
	pPerson->SetMove(add);// �O���MOVE�l������
	//pPerson->AddMove(add); 

}

void BasePlayerState::AerialJump::Execute(BasePlayer * pPerson)
{

	//////////////////////////////////////////////
	//	�󒆃W�����v��
	//============================================

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

void BasePlayerState::AerialJump::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::AerialJump::Render(BasePlayer * pPerson)
{
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

void BasePlayerState::Fall::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::Fall::Render(BasePlayer * pPerson)
{
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

		// ���Ⴊ�݃L�����Z��
		else if (pPerson->isPushInput(PLAYER_INPUT::DOWN) && pPerson->GetRushAttack()->bNextOK)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		}

		// �q�b�g���Ă��Ԃ��t�H���[���[�V����
		else if (pPerson->GetRushAttack()->bNextOK && pPerson->GetAttackFrame() == ATTACK_FRAME::FOLLOW)
		{
			// �U���{�^����������(�������ςȂ�����)
			if (pPerson->isPushInput(PLAYER_INPUT::B))
			{
				// ���̃��[�V�����Z�b�g
				pPerson->SetMotion(4);

				// �U���X�e�[�g��2�i�ڂɐݒ肷��
				pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH2);

				pPerson->GetRushAttack()->step++;
				pPerson->GetRushAttack()->bNextOK = false;
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

		// ���Ⴊ�݃L�����Z��
		else if (pPerson->isPushInput(PLAYER_INPUT::DOWN) && pPerson->GetRushAttack()->bNextOK)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		}

		// �q�b�g���Ă��Ԃ��t�H���[���[�V����
		else if (pPerson->GetRushAttack()->bNextOK && pPerson->GetAttackFrame() == ATTACK_FRAME::FOLLOW)
		{
			// �U���{�^����������(�������ςȂ�����)
			if (pPerson->isPushInput(PLAYER_INPUT::B))
			{
				// ���̃��[�V�����Z�b�g
				pPerson->SetMotion(6);

				// �U���X�e�[�g��3�i�ڂɐݒ肷��
				pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH3);

				pPerson->GetRushAttack()->step++;
				pPerson->GetRushAttack()->bNextOK = false;
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
	//pPerson->SetAttackState(BASE_ATTACK_STATE::NO_ATTACK);
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
		pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
		pPerson->GetRushAttack()->bNextOK = true;
		break;
	}
	}

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
	}
}

void BasePlayerState::KnockBack::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::KnockBack::Render(BasePlayer * pPerson)
{
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::KnockBack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	return true;
	//	break;
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
	pPerson->SetMotion(17);

}

void BasePlayerState::KnockDown::Execute(BasePlayer * pPerson)
{
	if (true)
	{
		// �ҋ@�X�e�[�g�ɖ߂�
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::KnockDown::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::KnockDown::Render(BasePlayer * pPerson)
{
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::KnockDown::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//default:
	//	return true;
	//	break;
	//}
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
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
		// �W�����v�X�e�[�g�ɍs��
		pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
		return;
	}

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
	pPerson->SetAttackState(BASE_ATTACK_STATE::SQUAT);

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
	switch (msg.Msg)
	{
		// �U�����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_ATTACK:
	{
		HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
		break;
	}
	}
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
	pPerson->SetAttackState(BASE_ATTACK_STATE::AERIAL);

}

void BasePlayerState::AerialAttack::Execute(BasePlayer * pPerson)
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
	switch (msg.Msg)
	{
		// �U�����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_ATTACK:
	{
		HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
		break;
	}
	}
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
	pPerson->SetAttackState(BASE_ATTACK_STATE::AERIALDROP);

}

void BasePlayerState::AerialDropAttack::Execute(BasePlayer * pPerson)
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
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
		// �U�����������惁�b�Z�[�W
	case MESSAGE_TYPE::HIT_ATTACK:
	{
		HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		pPerson->SetHitStopFrame(hai->hitStopFlame);// �q�b�g�X�g�b�v
		break;
	}
	}
	return false;
}