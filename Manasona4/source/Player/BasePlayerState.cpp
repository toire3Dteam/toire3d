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
	// 待機モーションに変える
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
//					歩くステート
/*******************************************************/

BasePlayerState::Walk * BasePlayerState::Walk::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Walk instance;
	return &instance;
}

void BasePlayerState::Walk::Enter(BasePlayer * pPerson)
{
	// 走るモーションに変える
	pPerson->SetMotion(1);

	// 初速度の設定
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
		MyAssert(0, "ふぁっきゅー");
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
	else // ニュートラル
	{
		Vector3 move = pPerson->GetMove();

		// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
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
//					走るステート
/*******************************************************/

BasePlayerState::Run * BasePlayerState::Run::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Run instance;
	return &instance;
}

void BasePlayerState::Run::Enter(BasePlayer * pPerson)
{
	// 走るモーションに変える
	pPerson->SetMotion(1);

	// 初速度の設定
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
		MyAssert(0, "ふぁっきゅー");
		break;
	}

	pPerson->SetMove(move);
}

void BasePlayerState::Run::Execute(BasePlayer * pPerson)
{
	// 右押した瞬間
	if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		// 反対方向
		if (pPerson->GetDir() == DIR::LEFT)
		{
			pPerson->SetDir(DIR::RIGHT);
			// ブレーキステートへ
			pPerson->GetFSM()->ChangeState(BasePlayerState::TrunOverBrake::GetInstance());
		}
	}
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 1)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::RIGHT);
	}

	// 左押した瞬間
	else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		// 反対方向
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetDir(DIR::LEFT);
			// ブレーキステートへ
			pPerson->GetFSM()->ChangeState(BasePlayerState::TrunOverBrake::GetInstance());
		}
	}
	else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 1)
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::LEFT);

	}
	else // ニュートラル
	{
		Vector3 move = pPerson->GetMove();

		// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
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
//					Uターンブレーキステート
/*******************************************************/

BasePlayerState::TrunOverBrake * BasePlayerState::TrunOverBrake::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::TrunOverBrake instance;
	return &instance;
}

void BasePlayerState::TrunOverBrake::Enter(BasePlayer * pPerson)
{
	// ブレーキモーションに変える
	pPerson->SetMotion(18);
}

void BasePlayerState::TrunOverBrake::Execute(BasePlayer * pPerson)
{
	Vector3 move = pPerson->GetMove();

	// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
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
