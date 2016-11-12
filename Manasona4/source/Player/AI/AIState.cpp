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
		pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
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
		pPerson->SetTargetPlayer(PlayerMgr->GetPlayer(targetNo));
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
	// 
	if (MyPlayer->isLand())
	{
		// ジャンプ
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
	pPerson->SetPracticeGuardFrame(120);
	pPerson->SetPracticeGuardFlag(true);
}

void AIState::PracticeLand::Execute(AI * pPerson)
{
	
	// ★何か攻撃を受けている最中ならば
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialKnockBack::GetInstance()) == true
		)
	{
		// 復帰ボタン
		pPerson->PushInputList(PLAYER_INPUT::A);
		pPerson->SetPracticeGuardFrame(0);
		pPerson->SetPracticeGuardFlag(false);
	}


	//  まだガードフラグがたっていなくリカバリー？できていたら
	if (pPerson->GetPracticeGuardFlag() == false)
	{
		// ノックバック・空中リカバリー・地上リカバリーが前のステートになっていたらガードする
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true ||
			MyPlayer->GetFSM()->isPrevState(*BasePlayerState::AerialRecovery::GetInstance()) == true ||
			MyPlayer->GetFSM()->isPrevState(*BasePlayerState::LandRecovery::GetInstance()) == true)
		{
			pPerson->PushInputList(PLAYER_INPUT::R1);// ガードする
			pPerson->SetPracticeGuardFlag(true);
		}
	}
	else
	{
		pPerson->AddPracticeGuardFrame(1);
		if (pPerson->GetPracticeGuardFrame() < 60)
		{
			pPerson->PushInputList(PLAYER_INPUT::R1);// ガードする
			//pPerson->SetPracticeGuardFrame(0);
			//pPerson->SetPracticeGuardFlag(false);
		}

	}

}

void AIState::PracticeLand::Exit(AI * pPerson)
{
}

void AIState::PracticeLand::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AI地面");
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

}

void AIState::Wait::Exit(AI * pPerson)
{
}

void AIState::Wait::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AIもじもじ");
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
	pPerson->PushInputList(PLAYER_INPUT::C);
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
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// ガード＋移動で　回避発動させる
	pPerson->PushInputList(PLAYER_INPUT::R1);
	// 相手の場所に応じて
	if (TargetPos.x < MyPos.x) pPerson->PushInputList(PLAYER_INPUT::LEFT);
	else pPerson->PushInputList(PLAYER_INPUT::RIGHT);


}

void AIState::Escape::Execute(AI * pPerson)
{
	// エスケープ後または待機になっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true||
		MyPlayer->GetFSM()->isPrevState(*BasePlayerState::Escape::GetInstance()) == true)
	{
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
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
//				ガードステート
/*******************************************************/

AIState::Guard * AIState::Guard::GetInstance()
{
	// ここに変数を作る
	static AIState::Guard instance;
	return &instance;
}

void AIState::Guard::Enter(AI * pPerson)
{
	// ガードボタン押す
	pPerson->PushInputList(PLAYER_INPUT::R1);
}

void AIState::Guard::Execute(AI * pPerson)
{
	//　ガードボタンを押し続ける
	pPerson->PushInputList(PLAYER_INPUT::R1);
	
	// ガード後または待機になっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true ||
		MyPlayer->GetFSM()->isPrevState(*BasePlayerState::Guard::GetInstance()) == true)
	{
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
	
	// 一度のガード時間　最高でも8フレームは硬直

	// 8フレーム毎に回避か防御継続かを判断
	if (pPerson->GetGuardFrame() % 8 == 0)
	{
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		float len = MyPlayer->AIAtackRange();

		/***********************/
		// 回避or防御
		/***********************/
		// 相手が攻撃ステートなら
		if (pPerson->GetTargetPlayer()->isAttackState())
		{
			if (tdnRandom::Get(0, 1) == 0)
			{
				// 一定まで相手が近くにいて
				if (Math::Length(TargetPos, MyPos) < len)
				{
					// 回避してよける
					pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
					return;
				}
			}

		}
	}


	// 40フレームまできたら今度は相手が攻撃していなかったらガードを解除
	if (pPerson->GetGuardFrame() > 40)
	{
		pPerson->SetGuardFrame(0);// リセット

		// 相手が攻撃していなかったら
		if (pPerson->GetTargetPlayer()->isAttackState()==false)
		{
			// ガード解除
			pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
			return;

		}

	}

	pPerson->AddGuardFrame(1);//++


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
//				追いかけるステート
/*******************************************************/
//くうちゅうにいたら空中おいかけにもっていこうかな
AIState::Chase * AIState::Chase::GetInstance()
{
	// ここに変数を作る
	static AIState::Chase instance;
	return &instance;
}

void AIState::Chase::Enter(AI * pPerson)
{

	// 追いかけタイマー初期化
	pPerson->SetChaseFrame(0);

	//	誰をおいかけるか考える
	if (SearchNearPlayer(pPerson) == false)
	{
		// 敵を見つけられなかった
		pPerson->GetFSM()->ChangeState(AIState::Space::GetInstance());
	}

	//Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	//Vector3 MyPos = MyPlayer->GetPos();

	//if (TargetPos.x > MyPos.x)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//}
	//else
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//}
}

void AIState::Chase::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	//// HITしていたら
	//if (MyPlayer->GetAttackData()->bHit == false) // ※このアタックデータのゲッターこわい
	//{
	//	return;
	//}

	// 自分が空中に浮いていたら　浮いてる理由ー＞リカバリーとか
	if (MyPlayer->isLand() == false)
	{
		pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
		return;
	}

	/***********************************************************/
	//pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
	//return;
	/***********************************************************/

	//ふぃにっしゅあーつAIでｔくろうかなあああ
	// 走ってるか歩いているかで ★（TODO）キャラクター毎にレンジを変える
	float len = 0;
	if (MyPlayer->GetFSM()->isInState
		(*BasePlayerState::Run::GetInstance()) == true)  len = MyPlayer->AIRunAtackRange();// 仮で数値じかうち
	else len = MyPlayer->AIAtackRange();
	
	// 一定まで近づいたら
	if (Math::Length(TargetPos, MyPos) < len)
	{
		if (tdnRandom::Get(0, 2) == 0)// なぞの式やけどええ感じやからこのままで
		{
			// 前回空中攻撃をかましていなかったら
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDropAtack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return;
			}
		}

		// 読みの回避 あるき待機||歩きで確立少なめ
		if (MyPlayer->GetFSM()->isInState
			(*BasePlayerState::Wait::GetInstance()) == true||
			MyPlayer->GetFSM()->isInState
			(*BasePlayerState::Walk::GetInstance()) == true)
		{
			// 連続で防御するの防止
			if (pPerson->GetFSM()->isPrevState(*AIState::Guard::GetInstance()) != true)
			{
				//4分の１
				if (tdnRandom::Get(0, 3) == 3)
				{
					pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
					return;
				}
			}
		}


		// 地上か空中か
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			// 左右の振り向き
			if (TargetPos.x > MyPos.x)
			{
				// 左向いてたら向きを変えるまで右ボタンを押す
				if (MyPlayer->GetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					// 3分の1の確立でスキル攻撃
					if (tdnRandom::Get(0, 2) == 0)
					{
						pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
						return;
					}
					else
					{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					}

				}

			}
			else
			{
				//　↑同様に向きを変える
				if (MyPlayer->GetDir() == DIR::RIGHT)
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}		
				else
				{
					// 3分の1の確立でスキル攻撃
					if (tdnRandom::Get(0, 2) == 0)
					{
						pPerson->GetFSM()->ChangeState(AIState::SkillAtack::GetInstance());
						return;
					}
					else
					{
						pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
						return;
					}
				}
			}
		}
		else
		{
			// ★（難易度に属する物）製作者の慈悲により絶対対空当てるマンを確立に
			
			// 2分の1の確立で対空攻撃
			if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return;
			}
			else if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
			else
			{
				pPerson->GetFSM()->ChangeState(AIState::RushAtack::GetInstance());
				return;
			}
		}
	}
	

	// ダッシュ東野
	if (Math::Length(TargetPos, MyPos) > 50)
	{
		// 歩いている場合
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Walk::GetInstance()) == true||
			MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// 押しては離す　この動きで走りにさせる
			if (TargetPos.x > MyPos.x)
			{
				if (MyPlayer->GetInputList(PLAYER_INPUT::RIGHT) == 0)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
			}
			else 
			{
				if (MyPlayer->GetInputList(PLAYER_INPUT::LEFT) == 0)
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}				
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
	else 
	{
		// 左右の移動
		if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	}

		
	/********************/
	//	粘着防止
	/********************/
	pPerson->AddChaseFrame(1);			// 追いかけタイマー更新
	if (pPerson->GetChaseFrame() >= 60)	// 60秒間粘着して追いかける
	{
		pPerson->SetChaseFrame(0); // 初期化
	
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

// (TODO) 名前がおかしい　チェイスをつけるな　ただジャンプしてるだけ エアーじゃなくてジャンプ？

AIState::ChaseAir * AIState::ChaseAir::GetInstance()
{
	// ここに変数を作る
	static AIState::ChaseAir instance;
	return &instance;
}

void AIState::ChaseAir::Enter(AI * pPerson)
{
	// ジャンプ
	pPerson->PushInputList(PLAYER_INPUT::C);

}

void AIState::ChaseAir::Execute(AI * pPerson)
{
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 左右の移動
	if (TargetPos.x > MyPos.x + 13)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	else if (TargetPos.x < MyPos.x - 13)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	// 感性での飛びすぎ防止（静止させる）
	else if (MyPlayer->GetMove().x < -0.3f)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else if (MyPlayer->GetMove().x > 0.3f)
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}

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
			// ★（仮）でフィニッシュ攻撃もいれてみた
			if (MyPlayer->GetCollectScore() >= 10000)
			{
				// フィニッシュ攻撃
				pPerson->GetFSM()->ChangeState(AIState::FinishAtack::GetInstance());
				return;
			}
			else
			{
				// 空中下攻撃で制圧
				pPerson->GetFSM()->ChangeState(AIState::AerialDropAtack::GetInstance());
				return;
			}

			
		}
	}
	
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

	// ★共有 間隔を空けたかったので今はこれで
	static int Flame = 0;
	Flame++;
	//if (Flame > 5)
	{
		Flame = 0;
		if (MyPlayer->GetDir()== DIR::RIGHT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
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
	// しゃがみ押す
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
		abs(TargetPos.y - MyPos.y) < 32
		)
	{
		// きゃらがしゃがみモードになってたら
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()) == true)
		{
			// 対空発動ステートへ！
			pPerson->GetFSM()->ChangeState(AIState::AnitiAir::GetInstance());
			return;

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
	
	// しゃがみ攻撃が終わっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()) == false)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// しゃがみ攻撃中
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance()) == true)
	{
		if (!MyPlayer->isAttackState())return;
		// HITしていたらキャンセルして飛べ！！
		if (MyPlayer->GetAttackData()->bHit == true) // ※このアタックデータのゲッターこわい
		{
			if (pPerson->GetTargetPlayer()->isLand()==true)
			{
				// 相手が浮いてなかったら
				//pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
				//return;
			}
			else
			{
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
	// ジャンプ押す
	pPerson->PushInputList(PLAYER_INPUT::C);
}

void AIState::SetAerialAtack::Execute(AI * pPerson)
{
	// 相手は地上に着地してしまったら
	if (pPerson->GetTargetPlayer()->isLand())
	{
		// 対空攻撃できないので戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 左右の移動
	if (TargetPos.x > MyPos.x+5)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);	
	else if (TargetPos.x < MyPos.x-5)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

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
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		// 左右の入力(方向転回)
		if (TargetPos.x > MyPos.x)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else						pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}

	// もうジャンプキャンセルできなかったら〆技
	if (MyPlayer->isAerialJump() == true)
	{
		// 攻撃ボタン押す
		pPerson->PushInputList(PLAYER_INPUT::B);

	}
	else
	{
		//
		// 強力な攻撃ボタン押す
		MyPlayer->AIHighAtackButton();

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
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
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
				// 追跡モードに戻る
				pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
				return;
			}

		}
	}

	if (MyPlayer->GetActionFrame()==FRAME_STATE::END)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
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
//			空中下攻撃 ステート
/*******************************************************/

AIState::AerialDropAtack * AIState::AerialDropAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::AerialDropAtack  instance;
	return &instance;
}

void AIState::AerialDropAtack::Enter(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();
	// 左右の入力(方向転回)
	if (TargetPos.x > MyPos.x )	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	else						pPerson->PushInputList(PLAYER_INPUT::LEFT);

	// 空中下攻撃
	pPerson->PushInputList(PLAYER_INPUT::DOWN);
	pPerson->PushInputList(PLAYER_INPUT::B);
	
}

void AIState::AerialDropAtack::Execute(AI * pPerson)
{

	// 地上に着地してしまったら
	if (MyPlayer->isLand() == true)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// エリアルアタックステートへ移行していなかったら待つ(空下封印したのでコメントアウトしました！)
	//if (MyPlayer->GetFSM()->isPrevState(*BasePlayerState::AerialDropAttack::GetInstance()) == false)
	//{
	//	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialDropAttack::GetInstance()) == false)
	//		return;
	//}


	// HITすれば戻る
	if (!MyPlayer->isAttackState())return;
	if (MyPlayer->GetAttackData()->bHit == true) // ※このアタックデータのゲッターこわい
	{
		// 追跡モードに戻る
		//pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		//return;
	}
	



}

void AIState::AerialDropAtack::Exit(AI * pPerson)
{

}

void AIState::AerialDropAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialDropAtack");
}

bool AIState::AerialDropAtack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}

	return false;
}


/*******************************************************/
//			フィニッシュ攻撃 ステート
/*******************************************************/

AIState::FinishAtack * AIState::FinishAtack::GetInstance()
{
	// ここに変数を作る
	static AIState::FinishAtack  instance;
	return &instance;
}

void AIState::FinishAtack::Enter(AI * pPerson)
{
	// フィニッシュボタン
	pPerson->PushInputList(PLAYER_INPUT::R2);

}

void AIState::FinishAtack::Execute(AI * pPerson)
{
	// FinishAttackステートが終わっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == false)
	{
		// 追跡モードに戻る
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}

	// FinishAttack中
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) == true)
	{
		if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
		{
			// 追跡モードに戻る
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

	}
}

void AIState::FinishAtack::Exit(AI * pPerson)
{

}

void AIState::FinishAtack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "FinishAtack");
}

bool AIState::FinishAtack::OnMessage(AI * pPerson, const Message & msg)
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
