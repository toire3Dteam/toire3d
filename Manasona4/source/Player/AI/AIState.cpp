#include "AI.h"
#include "AIState.h"
#include "../../Player/BasePlayer.h"
#include "../../Player/PlayerManager.h"
#include "../BasePlayerState.h"

#define MyPlayer (pPerson->GetBasePlayer())

//void PushButton(AI* pPerson, PLAYER_INPUT input)
//{
//	pPerson->PushInputList(input);
//}

/*******************************************************/
//					グローバルステート
/*******************************************************/

AIState::Global * AIState::Global::GetInstance()
{
	// ここに変数を作る
	static AIState::Global instance;
	return &instance;
}

void AIState::Global::Enter(AI * pPerson)
{
}

void AIState::Global::Execute(AI * pPerson)
{
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == true
		)
	{
		// 復帰モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Recovery::GetInstance());
	}
}

void AIState::Global::Exit(AI * pPerson)
{
}

void AIState::Global::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIぐろーばるすてーと");
}

bool AIState::Global::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//				練習用ジャンプステート
/*******************************************************/

AIState::PracticeJump * AIState::PracticeJump::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeJump instance;
	return &instance;
}

void AIState::PracticeJump::Enter(AI * pPerson)
{
}

void AIState::PracticeJump::Execute(AI * pPerson)
{
	if (MyPlayer->isLand())
	{
		// ジャンプボタン押してる
		pPerson->PushInputList(PLAYER_INPUT::C);

	}

}

void AIState::PracticeJump::Exit(AI * pPerson)
{
}

void AIState::PracticeJump::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIジャンプ");
}

bool AIState::PracticeJump::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//				追いかけるステート
/*******************************************************/

AIState::Chase * AIState::Chase::GetInstance()
{
	// ここに変数を作る
	static AIState::Chase instance;
	return &instance;
}

void AIState::Chase::Enter(AI * pPerson)
{
	//	誰をおいかかえるか考える

	//近いやつをたーげーとぬい「
	float length2 = 114514810.0f;
	int targetno = 0;
	for (int i = 0; i < PlayerMgr->GetNumPlayer(); i++)
	{
		if (MyPlayer->GetDeviceID() == PlayerMgr->GetPlayer(i)->GetDeviceID())
		{
			continue;
		}

		if (PlayerMgr->GetPlayer(i)->isDownState())
		{
			continue;
		}

		float length= Math::Length(PlayerMgr->GetPlayer(i)->GetPos(), MyPlayer->GetPos());
		if (length<length2)
		{
			length2 = length;
			targetno = i;
		}
	}
	
	pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetno));



	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	if (TargetPos.x > MyPos.x)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);

	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);


	}
}

void AIState::Chase::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	if (TargetPos.x > MyPos.x)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);

	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);


	}

	// 一定まで近づいたら
	if (Math::Length(TargetPos, MyPos) < 22)
	{
		// 地上か空中か
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
			return;
		}
		else
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return;
		}
	}
	

}

void AIState::Chase::Exit(AI * pPerson)
{
}

void AIState::Chase::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIジャンプ");
}

bool AIState::Chase::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}





/*******************************************************/
//			ラッシュアタックステート
/*******************************************************/

AIState::RushAtack * AIState::RushAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::RushAtack instance;
	return &instance;
}

void AIState::RushAtack::Enter(AI * pPerson)
{
	pPerson->PushInputList(PLAYER_INPUT::B);
}

void AIState::RushAtack::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 一定まで離れたか
	if (
		//Math::Length(TargetPos, MyPos) > 6

		// プレイヤーのラッシュアタックステート終了してたら
		!MyPlayer->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance())
		)
	{
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// 攻撃！！
	//if ()
	{
		pPerson->PushInputList(PLAYER_INPUT::B);

	}

}

void AIState::RushAtack::Exit(AI * pPerson)
{
}

void AIState::RushAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "ラッシュ攻撃");
}

bool AIState::RushAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}






/*******************************************************/
//			復帰ステート
/*******************************************************/

AIState::Recovery * AIState::Recovery::GetInstance()
{
	// ここに変数を作る
	static AIState::Recovery instance;
	return &instance;
}

void AIState::Recovery::Enter(AI * pPerson)
{
	
}

void AIState::Recovery::Execute(AI * pPerson)
{
	// 復帰　何かのボタン押しっぱなし
	pPerson->PushInputList(PLAYER_INPUT::C);

	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == false &&
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == false &&
		MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == false)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	}

}

void AIState::Recovery::Exit(AI * pPerson)
{
}

void AIState::Recovery::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "リカバリー");
}

bool AIState::Recovery::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			対空準備ステート
/*******************************************************/

AIState::SetAnitiAir * AIState::SetAnitiAir::GetInstance()
{
	// ここに変数を作る
	static AIState::SetAnitiAir instance;
	return &instance;
}

void AIState::SetAnitiAir::Enter(AI * pPerson)
{
	// しゃがみ押しっぱなし
	pPerson->PushInputList(PLAYER_INPUT::DOWN);
}

void AIState::SetAnitiAir::Execute(AI * pPerson)
{
	// しゃがみ押しっぱなし
	pPerson->PushInputList(PLAYER_INPUT::DOWN);

	// しゃがみをやめる判定
	if (pPerson->GetTargetPlayer()->isLand())
	{
		// 相手は地上にいるので対空をやめる
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 対空範囲内
	if (
		//Math::Length(TargetPos, MyPos) < 24
		abs(TargetPos.y - MyPos.y) < 24
		)
	{
		// きゃらがしゃがみモードになってたら
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()) == true)
		{
			// 対空発動！
			pPerson->GetFSM()->ChangeState(AIState::AnitiAir::GetInstance());

		}
	}



}

void AIState::SetAnitiAir::Exit(AI * pPerson)
{
}

void AIState::SetAnitiAir::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SetAnitiAir");
}

bool AIState::SetAnitiAir::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}



/*******************************************************/
//			対空ステート
/*******************************************************/

AIState::AnitiAir * AIState::AnitiAir ::GetInstance()
{
	// ここに変数を作る
	static AIState::AnitiAir  instance;
	return &instance;
}

void AIState::AnitiAir ::Enter(AI * pPerson)
{
	// 対空攻撃
	pPerson->PushInputList(PLAYER_INPUT::B);
}

void AIState::AnitiAir ::Execute(AI * pPerson)
{
	
	// 追跡モードに戻る
	pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());

}

void AIState::AnitiAir ::Exit(AI * pPerson)
{
}

void AIState::AnitiAir ::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AnitiAir");
}

bool AIState::AnitiAir ::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}