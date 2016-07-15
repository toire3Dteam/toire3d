#include "BasePlayer.h"
#include "BasePlayerState.h"


/*******************************************************/
//					グローバルステート
/*******************************************************/

BasePlayerState::Global * BasePlayerState::Global::GetInstance()
{
	// ここに変数を作る
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
//					待機ステート
/*******************************************************/

BasePlayerState::Wait * BasePlayerState::Wait::GetInstance()
{
	// ここに変数を作る
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
