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
	//////////////////////////////////////////////
	//	ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// ジャンプステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	攻撃ボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		// 攻撃ステートに行く
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
	//////////////////////////////////////////////
	//	ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// ジャンプステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	攻撃ボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		// 攻撃ステートに行く
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
	else // ニュートラル
	{
		Vector3 move = pPerson->GetMove();

		// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
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
	//////////////////////////////////////////////
	//	ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// ジャンプステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	攻撃ボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		// 攻撃ステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	左
	//============================================
	// 押した瞬間
	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		// 反対方向
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetDir(DIR::LEFT);
			// ブレーキステートへ
			pPerson->GetFSM()->ChangeState(BasePlayerState::TurnOverBrake::GetInstance());
		}
	}
	// 押してる間
	else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 1)
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::LEFT);

	}

	//////////////////////////////////////////////
	//	右
	//============================================
	// 押した瞬間
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		// 反対方向
		if (pPerson->GetDir() == DIR::LEFT)
		{
			pPerson->SetDir(DIR::RIGHT);
			// ブレーキステートへ
			pPerson->GetFSM()->ChangeState(BasePlayerState::TurnOverBrake::GetInstance());
		}
	}
	// 押してる間
	else if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 1)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		pPerson->SetDir(DIR::RIGHT);
	}




	else // ニュートラル
	{
		Vector3 move = pPerson->GetMove();
		// ある程度減速したら、前ブレーキステートに移行(減速自体はBaseのアップデートでやってる)
		if (
			abs(move.x) < BasePlayer::c_FRONT_BRAKE_LINE
			)
		{
			// 前のステートがUターンブレーキだったら、「待機」ステートに戻る(ターンブレーキの後にフロントブレーキが発生してしまうから)
			if (pPerson->GetFSM()->isPrevState(*BasePlayerState::TurnOverBrake::GetInstance()))
				pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			else
			{
				pPerson->GetFSM()->ChangeState(BasePlayerState::FrontBrake::GetInstance());
			}
		}
	}

	// 移動地制御
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
//					フロントブレーキステート
/*******************************************************/

BasePlayerState::FrontBrake * BasePlayerState::FrontBrake::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::FrontBrake instance;
	return &instance;
}

void BasePlayerState::FrontBrake::Enter(BasePlayer * pPerson)
{
	// ブレーキモーションに変える
	pPerson->SetMotion(0);
}

void BasePlayerState::FrontBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// ジャンプステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

	Vector3 move = pPerson->GetMove();
	
	// A列車: MOVE値をここで直書きしている
	move *= .95f;
	pPerson->SetMove(move);

	// ほぼ止まってたら、(減速自体はBaseのアップデートでやってる)
	if (
		//move.Length() < .1f
		abs(move.x) < BasePlayer::c_END_MOVE_LINE
		)
	{
		// 方向キーを入力していたら、再度「走り」ステートへ
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

		// 左右方向キーを押してなかったら、待機ステートへ
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
//					Uターンブレーキステート
/*******************************************************/

BasePlayerState::TurnOverBrake * BasePlayerState::TurnOverBrake::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::TurnOverBrake instance;
	return &instance;
}

void BasePlayerState::TurnOverBrake::Enter(BasePlayer * pPerson)
{
	// ブレーキモーションに変える
	pPerson->SetMotion(18);
}

void BasePlayerState::TurnOverBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// ジャンプステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return;
	}

	Vector3 move = pPerson->GetMove();

	// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
	if (
		//move.Length() < .1f
		abs(move.x) < BasePlayer::c_END_MOVE_LINE
		)
	{
		// 方向キーを入力していたら、再度「走り」ステートへ
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}

		// 左右方向キーを押してなかったら、待機ステートへ
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
//					ジャンプステート
/*******************************************************/

BasePlayerState::Jump * BasePlayerState::Jump::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Jump instance;
	return &instance;
}

void BasePlayerState::Jump::Enter(BasePlayer * pPerson)
{
	// ブレーキモーションに変える
	pPerson->SetMotion(8);

	// しゃがみフラグ初期化
	pPerson->GetJump()->Clear();
}

void BasePlayerState::Jump::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプ発動前のしゃがみ中
	//============================================
	if (pPerson->GetJump()->bHold)
	{
		if(pPerson->isPushInput(PLAYER_INPUT::C))
		{
			// プレイヤーが押してる時間計測
			pPerson->GetJump()->PlayerHoldTimer++;
		}

		if (++pPerson->GetJump()->HoldTimer > 3)
		{
			// 初速度の設定
			Vector3 add(0, 0, 0);

			// しゃがんでる間、ずっと押してたので、最大ジャンプ
			if (pPerson->GetJump()->HoldTimer == pPerson->GetJump()->PlayerHoldTimer)
			{
				add.y = BasePlayer::c_MAX_JUMP;
			}
			// そうじゃないので小ジャンプ
			else
			{
				add.y = BasePlayer::c_MAX_JUMP * .65f;
			}

			// 左右入力
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))		add.x = -.25f;
			//else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT)) add.x = .25f;

			// 移動地を足す！
			pPerson->AddMove(add);

			// 地上フラグオフ！
			pPerson->SetLand(false);

			// しゃがみフラグ終了
			pPerson->GetJump()->bHold = false;
		}
	}

	//////////////////////////////////////////////
	//	ジャンプ中
	//============================================
	else
	{

		//////////////////////////////////////////////
		//	左入力
		//============================================
		if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
		{
			pPerson->AddMove(Vector3(-0.05f, 0.0f, 0.0f));
		}


		//////////////////////////////////////////////
		//	右入力
		//============================================
		else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
		{
			pPerson->AddMove(Vector3(0.05f, 0.0f, 0.0f));
		}

		// 移動地制御
		pPerson->MoveClampX(pPerson->GetMaxSpeed() * 1.1f);


		// 地上フラグtrue(着地したら)
		if (pPerson->isLand())
		{
			// 着地ステートに移行
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
//					着地ステート
/*******************************************************/

BasePlayerState::Land * BasePlayerState::Land::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Land instance;
	return &instance;
}

void BasePlayerState::Land::Enter(BasePlayer * pPerson)
{
	// ブレーキモーションに変える
	pPerson->SetMotion(12);

	// 着地時間初期化
	pPerson->GetJump()->LandTimer = 0;

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed());
}

void BasePlayerState::Land::Execute(BasePlayer * pPerson)
{
	// 一定時間着地したら
	if (++pPerson->GetJump()->LandTimer > 5)
	{
		// 方向キーを入力していたら、「走り」ステートへ
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

		// 左右方向キーを押してなかったら、「待機」ステートへ
		else
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

	}

	//////////////////////////////////////////////
	//	ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3 && pPerson->GetJump()->LandTimer > 3)
	{
		// ジャンプステートに行く
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
//					ラッシュ攻撃ステート
/*******************************************************/

BasePlayerState::RushAttack * BasePlayerState::RushAttack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::RushAttack instance;
	return &instance;
}

void BasePlayerState::RushAttack::Enter(BasePlayer * pPerson)
{
	// 初撃モーションに変える
	pPerson->SetMotion(2);

	// ラッシュ攻撃構造体初期化
	pPerson->GetRushAttack()->Clear();

	// 攻撃ステートを1段目に設定する
	pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH1);
}

void BasePlayerState::RushAttack::Execute(BasePlayer * pPerson)
{
	switch (pPerson->GetRushAttack()->step)
	{
	case 0:	// 1段目
		// ヒットしてる状態
		if (pPerson->GetRushAttack()->bHit)
		{
			// 攻撃ボタン押したら
			if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
			{
				// 次のモーションセット

				// 攻撃ステートを2段目に設定する
				pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH2);

				pPerson->GetRushAttack()->step++;
				pPerson->GetRushAttack()->bHit = false;
			}
		}
		// 攻撃終了してたら
		if (!pPerson->isAttackState())
		{
			// 待機モーションに戻る
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
		break;
	case 1:	// 2段目
		// ヒットしてる状態
		if (pPerson->GetRushAttack()->bHit)
		{
			// 攻撃ボタン押したら
			if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
			{
				// 次のモーションセット

				// 攻撃ステートを3段目に設定する
				pPerson->SetAttackState(BASE_ATTACK_STATE::RUSH3);

				pPerson->GetRushAttack()->step++;
				pPerson->GetRushAttack()->bHit = false;
			}
		}
		// 攻撃終了してたら
		if (!pPerson->isAttackState())
		{
			// 待機モーションに戻る
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
		break;
	case 2:	// 3段目
		// 攻撃終了してたら
		if (!pPerson->isAttackState())
		{
			// 待機モーションに戻る
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
		break;
	}
}

void BasePlayerState::RushAttack::Exit(BasePlayer * pPerson)
{
	// ★攻撃ステートをやめさせる
	pPerson->SetAttackState(BASE_ATTACK_STATE::END);
}

void BasePlayerState::RushAttack::Render(BasePlayer * pPerson)
{
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::RushAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 攻撃当たったよメッセージ
	case MESSAGE_TYPE::HIT_ATTACK:
	{
										HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
										pPerson->GetRushAttack()->bHit = true;
										break;
	}
		// 攻撃くらったよメッセージ
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
										HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
										break;
	}
	default:
		return true;
		break;
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}
