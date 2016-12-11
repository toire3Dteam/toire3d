#include "AI.h"
#include "AIState.h"
#include "../../Player/BasePlayer.h"
#include "../../Player/PlayerManager.h"
#include "../BasePlayerState.h"

#define MyPlayer (pPerson->GetBasePlayer())
#define TargetPlayer (pPerson->GetTargetPlayer())

//void PushButton(AI* pPerson, PLAYER_INPUT input)
//{
//	pPerson->PushInputList(input);
//}

/**************************/
// ファンクション
/**************************/

// 一番近い倒せるプレイヤー
bool SearchNearPlayer(AI * pPerson)
{
	enum{ NOT_TARGET = -1 };

	//近いプレイヤーをターゲットに
	float length2 = FLT_MAX;
	int targetNo = NOT_TARGET;
	for (int i = 0; i < PlayerMgr->GetNumPlayer(); i++)
	{
		// 自分自身ははじく
		if (MyPlayer->GetDeviceID() == PlayerMgr->GetPlayer(i)->GetDeviceID())
		{
			continue;
		}
		// 同じチームをはじく
		if (MyPlayer->GetSide() == PlayerMgr->GetPlayer(i)->GetSide())
		{
			continue;
		}

		if (PlayerMgr->GetPlayer(i)->isInvincible())
		{
			continue;
		}

		//if (PlayerMgr->GetPlayer(i)->isDownState())
		//{
		//	continue;
		//}

		float length = Math::Length(PlayerMgr->GetPlayer(i)->GetPos(), MyPlayer->GetPos());
		if (length < length2)
		{
			length2 = length;
			targetNo = i;
		}
	}

	if (targetNo ==  NOT_TARGET)
	{
		// 駄目みたい
		// ターゲットになれるプレイヤーはいなかったようですね..
		return false;
	}
	else
	{	
		// ターゲットととしてそのプレイヤーをセット
		//pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
		return true;
	}

}

// 一番近い無敵中のプレイヤー
bool SearchNearInvinciblePlayer(AI * pPerson)
{
	enum{ NOT_TARGET = -1 };

	//近いプレイヤーをターゲットに
	float length2 = FLT_MAX;
	int targetNo = NOT_TARGET;
	for (int i = 0; i < PlayerMgr->GetNumPlayer(); i++)
	{
		// 自分自身ははじく
		if (MyPlayer->GetDeviceID() == PlayerMgr->GetPlayer(i)->GetDeviceID())
		{
			continue;
		}

		// 同じチームをはじく
		if (MyPlayer->GetSide() == PlayerMgr->GetPlayer(i)->GetSide())
		{
			continue;
		}


		if (PlayerMgr->GetPlayer(i)->isInvincible() == false)
		{
			continue;
		}

		//if (PlayerMgr->GetPlayer(i)->isDownState()==false)
		//{
		//	continue;
		//}

			float length = Math::Length(PlayerMgr->GetPlayer(i)->GetPos(), MyPlayer->GetPos());
			if (length < length2)
			{
				length2 = length;
				targetNo = i;
			}
		

	}

	if (targetNo == NOT_TARGET)
	{
		// 駄目みたい
		// ターゲットになれるプレイヤーはいなかったようですね..
		return false;
	}
	else
	{
		// ターゲットととしてそのプレイヤーをセット
		//pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
		return true;
	}

}


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
	// ダメージを食らったら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// 復帰モードへ
		pPerson->GetFSM()->ChangeState(AIState::Recovery::GetInstance());
	}

	// 投げに捕まったら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true)
	{
		// 投げ抜け判断ステートへ
		pPerson->GetFSM()->ChangeState(AIState::ThrowTech::GetInstance());
	}

}

void AIState::Global::Exit(AI * pPerson)
{
}

void AIState::Global::Render(AI * pPerson)
{
	//tdnText::Draw(420, 610, 0xffffffff, "AIぐろーばるすてーと");
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
//					プラクティスグローバルステート
/*******************************************************/

AIState::PracticeGlobal* AIState::PracticeGlobal::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeGlobal instance;
	return &instance;
}

void AIState::PracticeGlobal::Enter(AI * pPerson)
{

}

void AIState::PracticeGlobal::Execute(AI * pPerson)
{

}

void AIState::PracticeGlobal::Exit(AI * pPerson)
{
}

void AIState::PracticeGlobal::Render(AI * pPerson)
{
	//tdnText::Draw(420, 610, 0xffffffff, "AIぐろーばるすてーと");
}

bool AIState::PracticeGlobal::OnMessage(AI * pPerson, const Message & msg)
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
	// ★何か攻撃を受けて倒れる最中ならば
	if (
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// 復帰ボタン
		pPerson->PushInputList(PLAYER_INPUT::B);
	}

	// 地上に付いたらジャンプする
	if (MyPlayer->isLand())
	{
		// ジャンプ
		pPerson->PushInputList(PLAYER_INPUT::UP);
	}

}

void AIState::PracticeJump::Exit(AI * pPerson)
{
}

void AIState::PracticeJump::Render(AI * pPerson)
{

#ifdef _DEBUG

	tdnText::Draw(1000, 580, 0xffffffff, "AIジャンプ");

#endif // _DEBUG

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
//				練習用地面ステート
/*******************************************************/

AIState::PracticeLand * AIState::PracticeLand::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeLand instance;
	return &instance;
}

void AIState::PracticeLand::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);

}

void AIState::PracticeLand::Execute(AI * pPerson)
{
	
	// ★何か攻撃を受けて倒れる最中ならば
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// 復帰ボタン
		pPerson->PushInputList(PLAYER_INPUT::B);
	}

	// 倒れないノックバック技を受けたなら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true)
	{
		pPerson->m_bPracticeGuardFlag = true;
		pPerson->m_iPracticeGuardFrame = 0;
	}
	
	// 練習用ガードフラグが立ってたら　
	if (pPerson->m_bPracticeGuardFlag == true /*&& MyPlayer->GetRecoveryFrame() <= 0　ガード硬直も含むのでなし*/)
	{
		pPerson->m_iPracticeGuardFrame++;	
		
		// ガードしたらガードし続けるフレームを下げる（相手が落ち着くまでガード）
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == true)
		{
			// 相手の攻撃持続フレーム中のときリセット
			// if (TargetPlayer->isActiveFrame() == true)
			pPerson->m_iPracticeGuardFrame = 10;
		}

		// 一定フレーム守り続ける
		if (pPerson->m_iPracticeGuardFrame >= 20) // ←フレーム落ち着かなずっとガードする
		{
			// 練習ガードフラグと練習ガードフレームを初期化
			pPerson->m_bPracticeGuardFlag = false;
			pPerson->m_iPracticeGuardFrame = 0;
		}

		// 相手が攻撃振ったら
		if (TargetPlayer->isAttackState() == true)
		{

			// 相手と逆方向レバーを押す
			if (MyPlayer->GetDir() == DIR::LEFT)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}

			// 下段なら
			if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
			{
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
			}

		}

	}


}

void AIState::PracticeLand::Exit(AI * pPerson)
{
}

void AIState::PracticeLand::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI地面");

#endif // _DEBUG

}

bool AIState::PracticeLand::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//				練習用攻撃ステート
/*******************************************************/

AIState::PracticeAttack* AIState::PracticeAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeAttack instance;
	return &instance;
}

void AIState::PracticeAttack::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);
	pPerson->m_iPracticeAttackIntervalFrame = 0;

}

void AIState::PracticeAttack::Execute(AI * pPerson)
{
	// 一定時間がたったら攻撃
	pPerson->m_iPracticeAttackIntervalFrame++;
	if (pPerson->m_iPracticeAttackIntervalFrame >= 60)
	{
		pPerson->m_iPracticeAttackIntervalFrame = 0;
		// 攻撃
		pPerson->PushInputList(PLAYER_INPUT::C);

	}
	

	// ★何か攻撃を受けて倒れる最中ならば
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// 復帰ボタン
		pPerson->PushInputList(PLAYER_INPUT::B);
	}

	//// 倒れないノックバック技を受けたなら
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true)
	//{
	//	pPerson->m_bPracticeGuardFlag = true;
	//	pPerson->m_iPracticeGuardFrame = 0;
	//}

	//// 練習用ガードフラグが立ってたら　
	//if (pPerson->m_bPracticeGuardFlag == true /*&& MyPlayer->GetRecoveryFrame() <= 0　ガード硬直も含むのでなし*/)
	//{
	//	pPerson->m_iPracticeGuardFrame++;

	//	// ガードしたらガードし続けるフレームを下げる（相手が落ち着くまでガード）
	//	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == true)
	//	{
	//		// 相手の攻撃持続フレーム中のときリセット
	//		// if (TargetPlayer->isActiveFrame() == true)
	//		pPerson->m_iPracticeGuardFrame = 10;
	//	}

	//	// 一定フレーム守り続ける
	//	if (pPerson->m_iPracticeGuardFrame >= 20) // 40フレーム落ち着かなずっとガードする
	//	{
	//		// 練習ガードフラグと練習ガードフレームを初期化
	//		pPerson->m_bPracticeGuardFlag = false;
	//		pPerson->m_iPracticeGuardFrame = 0;
	//	}

	//	// 相手が攻撃振ったら
	//	if (TargetPlayer->isAttackState() == true)
	//	{

	//		// 相手と逆方向レバーを押す
	//		if (MyPlayer->GetDir() == DIR::LEFT)
	//		{
	//			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//		}
	//		else
	//		{
	//			pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//		}

	//		// 下段なら
	//		if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
	//		{
	//			pPerson->PushInputList(PLAYER_INPUT::DOWN);
	//		}

	//	}

	//}

}

void AIState::PracticeAttack::Exit(AI * pPerson)
{
}

void AIState::PracticeAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI攻撃練習");

#endif // _DEBUG

}

bool AIState::PracticeAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//				待機ステート
/*******************************************************/

AIState::Wait * AIState::Wait::GetInstance()
{
	// ここに変数を作る
	static AIState::Wait instance;
	return &instance;
}

void AIState::Wait::Enter(AI * pPerson)
{

}

void AIState::Wait::Execute(AI * pPerson)
{
	
	// 待機モーションになったら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// 追跡モードに
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
}

void AIState::Wait::Exit(AI * pPerson)
{
}

void AIState::Wait::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AIもじもじ");

#endif // _DEBUG

}

bool AIState::Wait::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//				間隔を開けるステート
/*******************************************************/

AIState::Space * AIState::Space::GetInstance()
{
	// ここに変数を作る
	static AIState::Space instance;
	return &instance;
}

void AIState::Space::Enter(AI * pPerson)
{
	//	誰から逃げるか考える
	if (SearchNearInvinciblePlayer(pPerson) == false)
	{
		// やっぱり全員ヤレル
		//pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	}

	// ウェイトタイマー初期化
	pPerson->SetWaitFrame(0);

}

void AIState::Space::Execute(AI * pPerson)
{
	BasePlayer* b = pPerson->GetTargetPlayer();
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();


	enum 
	{
		SPACE_LENGTH = 50,// 逃げる距離
		SPACE_LENGTH_FAR = 75// 逃げすぎてたら近づける距離
	};

	if (Math::Length(TargetPos, MyPos) > SPACE_LENGTH_FAR)
	{
		// 移動 (近づく)
		if (TargetPos.x > MyPos.x)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
	}
	// 一定まで離れたら
	else if (Math::Length(TargetPos, MyPos) > SPACE_LENGTH)
	{
		// アクションできる状態(WaitState)まで通さない
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true ||
			MyPlayer->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()) == true)
		{
			int random = tdnRandom::Get(0, 2);
			if (random == 1 || random == 2)
			{
				// 飛んでみる
				pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
				return;
			}
			else
			{

				// 回避してみる 
				//pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());	
				// 改めガードして見る
				//pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				// 適当に何かさせる

				return;

			}

		}
	
	}
	else
	{
		// 移動 (逃げる)
		if (TargetPos.x < MyPos.x)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
	}


	/***************************/
	//	頃合を見て次のステートへ
	/***************************/
	pPerson->AddWaitFrame(1);			// 待ちタイマー更新
	if (pPerson->GetWaitFrame() >= 20)	// 30秒間粘着して追いかける
	{
		pPerson->SetWaitFrame(0); // 初期化

		//	誰をおいかけるか考える
		if (SearchNearPlayer(pPerson) == true)// ↓チェイスの最初の処理も含めたら二回ターゲットしてるけど　boolでの使い方としても便利なので採用
		{
			// ヤレル
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}
}

void AIState::Space::Exit(AI * pPerson)
{

}

void AIState::Space::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIスペース空けて間合いを取る");
}

bool AIState::Space::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//				ただ飛ぶステート
/*******************************************************/

AIState::SimpleJump * AIState::SimpleJump::GetInstance()
{
	// ここに変数を作る
	static AIState::SimpleJump instance;
	return &instance;
}

void AIState::SimpleJump::Enter(AI * pPerson)
{
	// ジャンプボタン押す
	pPerson->PushInputList(PLAYER_INPUT::UP);
}

void AIState::SimpleJump::Execute(AI * pPerson)
{

	// ジャンプの踏み込みはカウントしない
	if (MyPlayer->GetJump()->bHold == false)
	{
		// 地上に着地したら かつ　待機ステートにいれば
		if (MyPlayer->isLand() && MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// 追いかけに戻る
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}
	}

}

void AIState::SimpleJump::Exit(AI * pPerson)
{
}

void AIState::SimpleJump::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIシンプルジャンプ");
}

bool AIState::SimpleJump::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//				回避ステート
/*******************************************************/

AIState::Escape * AIState::Escape::GetInstance()
{
	// ここに変数を作る
	static AIState::Escape instance;
	return &instance;
}

void AIState::Escape::Enter(AI * pPerson)
{
	
	// 回避発動させる
	pPerson->PushInputList(PLAYER_INPUT::B);

}

void AIState::Escape::Execute(AI * pPerson)
{

	// エスケープ後または待機になっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true||
		MyPlayer->GetFSM()->isPrevState(*BasePlayerState::Escape::GetInstance()) == true)
	{
		// 待機へ戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}
	
}

void AIState::Escape::Exit(AI * pPerson)
{
}

void AIState::Escape::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI回避");
}

bool AIState::Escape::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//				ガードステート	(1211) 完成
/*******************************************************/

AIState::Guard * AIState::Guard::GetInstance()
{
	// ここに変数を作る
	static AIState::Guard instance;
	return &instance;
}

void AIState::Guard::Enter(AI * pPerson)
{

	pPerson->m_iGuardFrame = 0;

}

void AIState::Guard::Execute(AI * pPerson)
{

	// ガードフレーム更新
	pPerson->m_iGuardFrame++;

	// ガードしたらガードし続けるフレームを下げる（相手が落ち着くまでガード）
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()) == true)
	{
		// 相手の攻撃持続フレーム中のときリセット
		pPerson->m_iGuardFrame = 10;
	}

	// 一定フレーム守り続ける
	if (pPerson->m_iGuardFrame >= 20) // ←フレーム落ち着かなずっとガードする
	{
		pPerson->m_iGuardFrame = 0;

		// AI待機ステートへ
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());

		//（TODO）切り替えし技


	}
	
	//	（TODO）フォローの値が欲しい
	// 相手が攻撃していなかったら
	if (pPerson->GetTargetPlayer()->isAttackState() == false)
	{
		// ↑より少ない一定フレームで終了
		if (pPerson->m_iGuardFrame >= 10)
		{
			pPerson->m_iGuardFrame = 0;
			// AI待機ステートへ
			pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());

			//（TODO）切り替えし技


		}

	}


	//+-----------------------------------------
	//		ガードの処理
	//+-----------------------------------------

	//if(いーじーだったら)
	//{
	//}else
	//{

	// 相手が攻撃振ったら
	if (TargetPlayer->isAttackState() == true)
	{

		// 相手と逆方向レバーを押す
		if (MyPlayer->GetDir() == DIR::LEFT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}

		// 下段なら
		if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
		{
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
		}

	}


}

void AIState::Guard::Exit(AI * pPerson)
{
}

void AIState::Guard::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIGuard");
}

bool AIState::Guard::OnMessage(AI * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//				追いかけるステート -> 近距離派生
/*******************************************************/

AIState::Chase * AIState::Chase::GetInstance()
{
	// ここに変数を作る
	static AIState::Chase instance;
	return &instance;
}

void AIState::Chase::Enter(AI * pPerson)
{

	// 追いかけタイマー初期化
	pPerson->m_iChaseFrame = 0;

}

void AIState::Chase::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 自分が空中に浮いていたら　空中追いかけステートへ
	if (MyPlayer->isLand() == false)
	{
		pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
		return;
	}

	/***********************************************************/
	//pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
	//return;
	/***********************************************************/

	// 走ってるか歩いているかで ★（TODO）キャラクター毎にレンジを変える
	float len = 0;
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Run::GetInstance()) == true)
		len = MyPlayer->AIRunAtackRange();	// 自分のキャラクターのレンジ
	else len = MyPlayer->AIAtackRange();	// 自分のキャラクターのレンジ
	
	//+-------------------------------------------------
	//	↑の距離で一定まで近づいたら　(ランダム)で攻撃 (1210) xの距離に変更 ジャンプして歩いたらダッシュになるのは頼む
	//+-------------------------------------------------
	if (Math::Length(TargetPos.x, MyPos.x) < len &&
		TargetPlayer->GetInvincibleTime() <= 0)
	{
		// 0%~100%の間の種を作ります
		//int iRandom = tdnRandom::Get(0, 100);
		
		// 投げステート
		if (tdnRandom::Get(0, 100) <= 105)// ←の確率で飛ぶ
		{
			// ★相手がダウンしていないときかつ、飛んでいないとき
			if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
				return;
			}
			return;
		}

		// 無敵技ステート
		if (tdnRandom::Get(0, 100) <= 15)// ←の確率で飛ぶ
		{
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAtack::GetInstance());
			return;
		}

		// 回避ステート
		if (tdnRandom::Get(0, 100) <= 15)// ←の確率で飛ぶ
		{
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return;
		}


		// ガードステート
		if (tdnRandom::Get(0, 100) <= 15)// ←の確率で飛ぶ
		{
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return;
		}	

		if (tdnRandom::Get(0, 100) <=	15)// ←の確率で飛ぶ
		{
			// 前回空中崩し攻撃をかましていなかったら
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAtack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)//	←の確率で飛ぶ
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return;
		}

		// きょうはここまで
		//	あしたはがーどとかそこらへんとはしりっぱなしなどの弱体化

		//// 読みの回避 あるき待機 歩きで確立少なめ
		//if (MyPlayer->GetFSM()->isInState
		//	(*BasePlayerState::Wait::GetInstance()) == true||
		//	MyPlayer->GetFSM()->isInState
		//	(*BasePlayerState::Walk::GetInstance()) == true)
		//{
		//	// 連続で防御するの防止
		//	if (pPerson->GetFSM()->isPrevState(*AIState::Guard::GetInstance()) != true)
		//	{
		//		//4分の１
		//		if (tdnRandom::Get(0, 3) == 3)
		//		{
		//			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
		//			return;
		//		}
		//	}
		//}


		// 相手が地上・空中にいるかで動きを変える
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			// 左右の振り向き
			if (TargetPos.x > MyPos.x)
			{
				//// 左向いてたら向きを変えるまで右ボタンを押す
				//if (MyPlayer->GetDir() == DIR::LEFT)
				//{
				//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				//}
				//else
				//{
					// 3分の1の確立でスキル攻撃
					//if (tdnRandom::Get(0, 2) == 0)
					//{
					//	pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
					//	return;
					//}
					//else
					//{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					//}

				//}

			}
			else
			{
				////　↑同様に向きを変える
				//if (MyPlayer->GetDir() == DIR::RIGHT)
				//{
				//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
				//}		
				//else
				//{
					// 3分の1の確立でスキル攻撃
					//if (tdnRandom::Get(0, 2) == 0)
					//{
					//	pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
					//	return;
					//}
					//else
					//{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					//}
				//}
			}
		}
		else
		{
			// 相手が空中にいる場合は走っていようが歩いてでも当たる場所に行く
			//float  nearLen = MyPlayer->AIAtackRange();	// 自分のキャラクターのレンジ


			// ★（難易度に属する物）製作者の慈悲により絶対対空当てるマンを確立に
			
			// 2分の1の確立で対空攻撃
			//if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return;
			}
			//else
			//{
			//	pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
			//	return;
			//}

		}
	}
	

	//+-----------------------------------------
	// ダッシュ東野　(12/10) 移動完璧
	//+-----------------------------------------	
	if (Math::Length(TargetPos, MyPos) >= 25)		// ←のパラメーターが走りに切り替える距離
	{
		// 歩いている場合
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()) == true||
			MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// 押しては離す　この動きで走りにさせる
			if (TargetPos.x > MyPos.x)
			{
				// ★二回押し ダッシュのために
				pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
			}else 
			{
				// ★二回押し ダッシュのために
				pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
			}
		}
		else if(MyPlayer->GetFSM()->isInState(*BasePlayerState::Run::GetInstance()) == true)
		{
			// 走るステートになっているので後はいつもどおりに
			// 左右の移動
			if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

		}
		//MyPlayer->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());	
	}
	else // 近いから歩くポン
	{
		// 左右の移動
		if (TargetPos.x > MyPos.x + 7)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else if (TargetPos.x < MyPos.x - 7)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
		else// ↓相手のにぎりぎりまで迫ったときの処理
		{

		}

	}

		
	/****************************/
	//	×粘着防止　◎新しい行動
	/****************************/
	pPerson->m_iChaseFrame++;			// 追いかけタイマー更新
	if (pPerson->m_iChaseFrame >= 60)	// 60秒間粘着して追いかける
	{
		pPerson->m_iChaseFrame = 0; // 初期化
	
		// もっかい近いやつ探す
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}



}

void AIState::Chase::Exit(AI * pPerson)
{
}

void AIState::Chase::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIチェイス");
	tdnText::Draw(420, 660, 0xffffffff, "AI追いかけてるプレイヤーNo%d",pPerson->GetTargetPlayer()->GetDeviceID());
	tdnText::Draw(420, 700, 0xffffffff, "距離%.2f", Math::Length(TargetPlayer->GetPos(), MyPlayer->GetPos()));


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
//				ジャンプして空中で追いかけるステート
/*******************************************************/


AIState::ChaseAir * AIState::ChaseAir::GetInstance()
{
	// ここに変数を作る
	static AIState::ChaseAir instance;
	return &instance;
}

void AIState::ChaseAir::Enter(AI * pPerson)
{
	// ジャンプ
	pPerson->PushInputList(PLAYER_INPUT::UP);

}

void AIState::ChaseAir::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 左右の移動
	//if (TargetPos.x > MyPos.x + 13)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//else if (TargetPos.x < MyPos.x - 13)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//// 感性での飛びすぎ防止（静止させる）
	//else if (MyPlayer->GetMove().x < -0.3f)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//}
	//else if (MyPlayer->GetMove().x > 0.3f)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//}

	// 一定まで近づいたら
	if (Math::Length(TargetPos, MyPos) < 32)
	{
		// 落下し始めた瞬間から
		if (MyPlayer->GetMove().y < -0.0f)
		{
			if (pPerson->GetTargetPlayer()->isLand() == false)
			{
				// ※相手も空中にいるなら空中コンボに持っていく
				pPerson->GetFSM()->ChangeState(AIState::AerialAtack::GetInstance());
				return;
			}
		}

		// 少し落下し始めたら
		if (MyPlayer->GetMove().y < -0.3f)
		{
			// 空中下攻撃で制圧
			pPerson->GetFSM()->ChangeState(AIState::AerialDestructAtack::GetInstance());
			return;
		}

	}
	
	//+-----------------------------------------------------
	//	相手が遠くにいて攻撃できず着地してしまった時の処理
	// ジャンプステートだったら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == true)
	{
		// ジャンプの踏み込みはカウントしない
		if (MyPlayer->GetJump()->bHold == false)
		{
			// 着地したら
			if (MyPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
				return;
			}
		}
	}
	else// 落下ステートとか？
	{			// 着地したら
		if (MyPlayer->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}

	

}

void AIState::ChaseAir::Exit(AI * pPerson)
{
}

void AIState::ChaseAir::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI空中チェイス");
}

bool AIState::ChaseAir::OnMessage(AI * pPerson, const Message & msg)
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
	// 攻撃開始、最初の一段目を振るで！！！
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::RushAtack::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// プレイヤーのラッシュアタックステート終了してたら
	// [1210] GetAtactDataでアサートが起こるので追加　後でバースト見てみるか
	if (!MyPlayer->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) ||
		!MyPlayer->isAttackState())/*!MyPlayer->isAttackState()*/
	{
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// 攻撃！！

	// 一段目がヒットしたら状態
	if (MyPlayer->GetRushStep() == 0 && 
		MyPlayer->GetAttackData()->bHit == true	)
	{
		// 二段目攻撃！
		pPerson->PushInputList(PLAYER_INPUT::C);
	}

	// 二段目がヒットしたら状態
	if (MyPlayer->GetRushStep() == 1 &&
		MyPlayer->GetAttackData()->bHit == true)
	{	
		// (難易度別)もし相手にガードされてたら崩す
		if (TargetPlayer->GetGuardState() == GUARD_STATE::DOWN_GUARD)
		{
			// 中段
			pPerson->PushInputList(PLAYER_INPUT::R1);

		}
		else if (TargetPlayer->GetGuardState() == GUARD_STATE::UP_GUARD)
		{
			// 足払い
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
			pPerson->PushInputList(PLAYER_INPUT::R1);
		}
		else
		{
			// 最後三段目攻撃！
			pPerson->PushInputList(PLAYER_INPUT::C);
		}

	}



	// 三段目がヒットして||ゲージ50%以上溜まってたら
	if (MyPlayer->GetRushStep() == 2 &&
		MyPlayer->GetAttackData()->bHit == true &&
		MyPlayer->GetOverDriveGage() >= 50.0f)
	{
		// 必殺技発動！！
		pPerson->PushInputList(PLAYER_INPUT::C);
	}

	// ★共有 間隔を空けたかったので今はこれで
	//static int Flame = 0;
	//Flame++;
	////if (Flame > 5)
	//{
	//	Flame = 0;
	//	if (MyPlayer->GetDir()== DIR::RIGHT)
	//	{
	//		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//	}
	//	else
	//	{
	//		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//	}
	//	pPerson->PushInputList(PLAYER_INPUT::B);

	//}

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
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == false)
	{
			// ウェイトに戻る
			pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());		
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
//			掴まれた（投げ抜け）ステート	◎
/*******************************************************/

AIState::ThrowTech * AIState::ThrowTech::GetInstance()
{
	// ここに変数を作る
	static AIState::ThrowTech instance;
	return &instance;
}

void AIState::ThrowTech::Enter(AI * pPerson)
{
	// 投げ抜け
	pPerson->PushInputList(PLAYER_INPUT::A);
}

void AIState::ThrowTech::Execute(AI * pPerson)
{
	// 待機ステートに戻ったらAIのステートも待機に
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// ウェイトに戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	}
}

void AIState::ThrowTech::Exit(AI * pPerson)
{
}

void AIState::ThrowTech::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "投げぬけタイム");
}

bool AIState::ThrowTech::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}

/*******************************************************/
//			投げ準備ステート	◎
/*******************************************************/

AIState::SetThrow * AIState::SetThrow::GetInstance()
{
	// ここに変数を作る
	static AIState::SetThrow instance;
	return &instance;
}

void AIState::SetThrow::Enter(AI * pPerson)
{
	// 相手の方向へ移動する
	if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
}

void AIState::SetThrow::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 対空範囲内 (自分の攻撃範囲内)
	enum { ABJUST = 3 };
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIAtackRange() / ABJUST))
	{
		// 投げ発動ステートへ！
		pPerson->GetFSM()->ChangeState(AIState::Throw::GetInstance());
		return;
	}else
	{
		// 相手の方向へ移動する
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}

	}
}

void AIState::SetThrow::Exit(AI * pPerson)
{
}

void AIState::SetThrow::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "投げ準備");
}

bool AIState::SetThrow::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			投げステート	◎
/*******************************************************/

AIState::Throw * AIState::Throw::GetInstance()
{
	// ここに変数を作る
	static AIState::Throw instance;
	return &instance;
}

void AIState::Throw::Enter(AI * pPerson)
{
	// 投げ
	pPerson->PushInputList(PLAYER_INPUT::A);
}

void AIState::Throw::Execute(AI * pPerson)
{
	// 待機ステートに戻ったらAIのステートも待機に
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// ウェイトに戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	}
}

void AIState::Throw::Exit(AI * pPerson)
{
}

void AIState::Throw::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "投げ");
}

bool AIState::Throw::OnMessage(AI * pPerson, const Message & msg)
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
	//// しゃがみ押す
	//pPerson->PushInputList(PLAYER_INPUT::DOWN);

}

void AIState::SetAnitiAir::Execute(AI * pPerson)
{
	
	// 相手が地上に落ちてたらしゃがみをやめる判定
	//if (pPerson->GetTargetPlayer()->isLand())
	//{
	//	// 相手は地上にいるので対空をやめる
	//	pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	//	return;
	//}


	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 対空範囲内 (自分の攻撃範囲内)
	enum { ABJUST = 2 };
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIAtackRange() - ABJUST))
	{
		// しゃがみ押しっぱなし +
		// 相手の逆方向レバー
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		pPerson->PushInputList(PLAYER_INPUT::DOWN);

		// AI対空発動ステートへ！
		pPerson->GetFSM()->ChangeState(AIState::AnitiAir::GetInstance());
		return;

	}
	else
	{
		// 相手の方向へ移動する
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
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
	// 相手の逆方向レバー
	if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::AnitiAir ::Execute(AI * pPerson)
{
	
	// (はずれていたら?)
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// 初期状態へ戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


	// 対空攻撃中
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()) == true)
	{
		if (!MyPlayer->isAttackState())return;
		// HITしていたらキャンセルして飛べ！！
		if (MyPlayer->GetAttackData()->bHit == true) // ※このアタックデータのゲッターこわい
		{
			if (pPerson->GetTargetPlayer()->isLand()==true)
			{
				// 相手が浮いてなかったら
				pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
				return;
			}
			else
			{
				// AIエリアルコンボ攻撃準備へ
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());
				return;
			}

		}
	}


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


/*******************************************************/
//			空中攻撃準備　ステート
/*******************************************************/

AIState::SetAerialAtack * AIState::SetAerialAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::SetAerialAtack  instance;
	return &instance;
}

void AIState::SetAerialAtack::Enter(AI * pPerson)
{
	// 相手との距離が離れていたら方向キーを入力
	float l_fXLength = abs(MyPlayer->GetPos().x - TargetPlayer->GetPos().x);
	if (l_fXLength >= 7) 
	{
		// 相手の方向へ飛ぶ
		if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
	}

	// (1210) ジャンプは押しっぱなしで発動するから一回押しても意味がなかったので
	// ここで書いていない
	// ジャンプ押す
	//pPerson->PushInputList(PLAYER_INPUT::UP);
}

void AIState::SetAerialAtack::Execute(AI * pPerson)
{
	// ジャンプに行くまで押しっぱなし
	
	// アタックステートなら
	if (MyPlayer->isAttackState())
	{
		// HITしていたらキャンセルして飛べ！！
		if (MyPlayer->GetAttackData()->bHit == true)
		{
			if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()) == true ||
				MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) == true)
			{
				// 相手の方向へ飛ぶ
				if (MyPlayer->GetPos().x > TargetPlayer->GetPos().x)
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				else 
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);

				pPerson->PushInputList(PLAYER_INPUT::UP);
			}
		}
	}


	

	// 相手は地上に着地してしまったら
	if (pPerson->GetTargetPlayer()->isLand())
	{
		// 対空攻撃できないので戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 左右の移動
	//if (TargetPos.x > MyPos.x+5)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);	
	//else if (TargetPos.x < MyPos.x-5)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	// 対空範囲内
	if ( Math::Length(TargetPos, MyPos) < 18 )
	{
		// 自分が落ちてるとき
		if (MyPlayer->GetMove().y < -0.0f)
		{
			// ジャンプ・エリアルジャンプモードになってたら
			if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == true ||
				MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()) == true)
			{

				// 空中攻撃発動ステートへ！
				pPerson->GetFSM()->ChangeState(AIState::AerialAtack::GetInstance());
				return;

			}
		}
	}

}

void AIState::SetAerialAtack::Exit(AI * pPerson)
{

}

void AIState::SetAerialAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SetAerialAtack");
}

bool AIState::SetAerialAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}

/*******************************************************/
//			空中攻撃 ステート
/*******************************************************/

AIState::AerialAtack * AIState::AerialAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::AerialAtack  instance;
	return &instance;
}

void AIState::AerialAtack::Enter(AI * pPerson)
{
	{
		//Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		//Vector3 MyPos = MyPlayer->GetPos();
		//// 左右の入力(方向転回)
		//if (TargetPos.x > MyPos.x)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		//else						pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}
	// 空中攻撃発動！
	// もうジャンプキャンセルできなかったら〆技
	if (MyPlayer->isAerialJump() == true)
	{
		// 攻撃ボタン押す
		pPerson->PushInputList(PLAYER_INPUT::C);
	}
	else
	{
		// 強力な攻撃ボタン押す
		//MyPlayer->AIHighAtackButton();
		pPerson->PushInputList(PLAYER_INPUT::C);
	}
}

void AIState::AerialAtack::Execute(AI * pPerson)
{
	// した攻撃防止
	//if (MyPlayer->isPushInput(PLAYER_INPUT::DOWN) == true)return;

	// 地上に着地してしまったら
	if (MyPlayer->isLand())
	{
		// 戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// エリアルアタックステートへ移行していなかったら待つ
	if (MyPlayer->GetFSM()->isPrevState(*BasePlayerState::AerialAttack::GetInstance()) == false)
	{
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) == false)
			return;
	}

	// 空中攻撃が終わっていたら
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) == false)
	//{
	//	// 追跡モードに戻る
	//	pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	//	return;
	//}


//	if (pPerson->GetTargetPlayer()->GetMove().y < 2.0f)
	{
		// HITすれば戻る
		if (!MyPlayer->isAttackState())return;
		if (MyPlayer->GetAttackData()->bHit == true) // ※このアタックデータのゲッターこわい
		{
			// 二段ジャンプができるならもう一度エリアルへ
			if (MyPlayer->isAerialJump() == true)
			{

				// ジャンプモードに戻る
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());
				return;
			}
			else
			{
				//pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());

				// 待機に戻る
				pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
				return;
			}

		}
	}

	if (MyPlayer->GetActionFrame()==FRAME_STATE::END)
	{
		// 待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


}

void AIState::AerialAtack::Exit(AI * pPerson)
{

}

void AIState::AerialAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialAtack");
}

bool AIState::AerialAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}

/*******************************************************/
//		◎ 空中崩し攻撃 ステート　HIT ->チェイス -> 地上攻撃へ
/*******************************************************/

AIState::AerialDestructAtack * AIState::AerialDestructAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::AerialDestructAtack  instance;
	return &instance;
}

void AIState::AerialDestructAtack::Enter(AI * pPerson)
{

	// 空中攻撃
	pPerson->PushInputList(PLAYER_INPUT::C);
	
}

void AIState::AerialDestructAtack::Execute(AI * pPerson)
{


	// 地上に着地したら
	if (MyPlayer->isLand() == true)
	{
		// 崩しているのだから追跡モードに行くしかない！
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
}

void AIState::AerialDestructAtack::Exit(AI * pPerson)
{

}

void AIState::AerialDestructAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialDestructAtack");
}

bool AIState::AerialDestructAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			無敵攻撃 ステート	(1211) ◎
/*******************************************************/

AIState::InvincibleAtack * AIState::InvincibleAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::InvincibleAtack  instance;
	return &instance;
}

void AIState::InvincibleAtack::Enter(AI * pPerson)
{
	// 無敵ボタン
	pPerson->PushInputList(PLAYER_INPUT::L2);

}

void AIState::InvincibleAtack::Execute(AI * pPerson)
{
	// FinishAttackステートが終わっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == false)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	//// FinishAttack中
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == true)
	//{
	//	if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
	//	{
	//		// 追跡モードに戻る
	//		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	//		return;
	//	}

	//}
}

void AIState::InvincibleAtack::Exit(AI * pPerson)
{

}

void AIState::InvincibleAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "InvincibleAtack");
}

bool AIState::InvincibleAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}

/*******************************************************/
//			スキル攻撃 ステート
/*******************************************************/

AIState::SkillAtack * AIState::SkillAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::SkillAtack  instance;
	return &instance;
}

void AIState::SkillAtack::Enter(AI * pPerson)
{
	// キャラクター固有の攻撃系スキル発動ステート
	MyPlayer->AIHighAtackButton();
}

void AIState::SkillAtack::Execute(AI * pPerson)
{
	// スキルステートが終わっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) == false)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// スキル中
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) == true)
	{

		// HITすれば戻る
		if (!MyPlayer->isAttackState())return;// 攻撃系のスキルなら
		if (MyPlayer->GetAttackData()->bHit == true) // ※このアタックデータのゲッターこわい
		{
			//// 二段ジャンプができるならもう一度エリアルへ
			//if (MyPlayer->isAerialJump() == true)
			//{
			//	// ジャンプモードに戻る
			//	pPerson->GetFSM()->ChangeState(AIState::SetAerialAtack::GetInstance());
			//	return;
			//}
		}


		if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
		{
			// 追跡モードに戻る
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}

}

void AIState::SkillAtack::Exit(AI * pPerson)
{

}

void AIState::SkillAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SkillAtack");
}

bool AIState::SkillAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}
