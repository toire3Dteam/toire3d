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
}

void BasePlayerState::Wait::Execute(BasePlayer * pPerson)
{
	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 1)
	{
		pPerson->SetMove(Vector3(-1, 1, 0));
	}
}

void BasePlayerState::Wait::Exit(BasePlayer * pPerson)
{
	

}

void BasePlayerState::Wait::Render(BasePlayer * pPerson)
{
}

bool BasePlayerState::Wait::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}
