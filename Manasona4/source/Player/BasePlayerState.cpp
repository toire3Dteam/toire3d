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
	if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		pPerson->SetDir(DIR::RIGHT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	}
	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
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
			abs(move.x) < .35f
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
	// �E�������u��
	if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		// ���Ε���
		if (pPerson->GetDir() == DIR::LEFT)
		{
			pPerson->SetDir(DIR::RIGHT);
			// �u���[�L�X�e�[�g��
			pPerson->GetFSM()->ChangeState(BasePlayerState::TrunOverBrake::GetInstance());
		}
	}
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 1)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::RIGHT);
	}

	// ���������u��
	else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		// ���Ε���
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetDir(DIR::LEFT);
			// �u���[�L�X�e�[�g��
			pPerson->GetFSM()->ChangeState(BasePlayerState::TrunOverBrake::GetInstance());
		}
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
			abs(move.x) < .5f
			)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
	}

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
//					U�^�[���u���[�L�X�e�[�g
/*******************************************************/

BasePlayerState::TrunOverBrake * BasePlayerState::TrunOverBrake::GetInstance()
{
	// �����ɕϐ������
	static BasePlayerState::TrunOverBrake instance;
	return &instance;
}

void BasePlayerState::TrunOverBrake::Enter(BasePlayer * pPerson)
{
	// �u���[�L���[�V�����ɕς���
	pPerson->SetMotion(18);
}

void BasePlayerState::TrunOverBrake::Execute(BasePlayer * pPerson)
{
	Vector3 move = pPerson->GetMove();

	// �قڎ~�܂��Ă���AWait�X�e�[�g�Ɉڍs(�������̂�Base�̃A�b�v�f�[�g�ł���Ă�)
	if (
		//move.Length() < .1f
		abs(move.x) < .1f
		)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	}
}

void BasePlayerState::TrunOverBrake::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::TrunOverBrake::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "TrunOverBrakeState");
}

bool BasePlayerState::TrunOverBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}
