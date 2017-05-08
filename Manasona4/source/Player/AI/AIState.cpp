#include "AI.h"
#include "AIState.h"
#include "../../Player/BasePlayer.h"
#include "../../Player/PlayerManager.h"
#include "../BasePlayerState.h"
#include "Stand\Stand.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "Shot\BaseShot.h"
#include "Shot\ShotManager.h"


#define MyPlayer (pPerson->GetBasePlayer())
#define TargetPlayer (pPerson->GetTargetPlayer())

//void PushButton(AI* pPerson, PLAYER_INPUT input)
//{
//	pPerson->PushInputList(input);
//}

// (TODO)スタンドに対しての処理や遠距離の技に対しての動きを作る

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

// 相手と自分のXの距離
float GetRange(AI * pPerson)
{
	// 距離
	//if (fabsf(MyPlayer->GetPos().x - (TargetPlayer->GetPos().x +
	//	TargetPlayer->GetAttackData()->pCollisionShape->pos.x * TargetPlayer->GetDirVecX()))
	//		> BasePlayer::c_GUARD_DISTANCE)
	//{
	//	return false;				// ガード発動距離
	//}
	//else
	//{
	//	return true;
	//}
	
	return abs(TargetPlayer->GetPos().x - MyPlayer->GetPos().x);
}

// 相手の攻撃と自分のXの距離
float GetRangeVsAttack(AI * pPerson)
{
	// 相手が攻撃してきたら。
	if (TargetPlayer->isAttackState() == true)
	{
		return  (fabsf(MyPlayer->GetPos().x - (TargetPlayer->GetPos().x +
			TargetPlayer->GetAttackData()->pCollisionShape->pos.x * TargetPlayer->GetDirVecX())));
	}
	else
	{
		return 0.0f;
	}
}

// 相手スタンドと自分のXの距離
float GetRangeVsStand(AI * pPerson)
{

	float l_fRange = 0.0f;

	// スタンド
	//if (TargetPlayer->GetStand()->isActive() &&
	//	TargetPlayer->GetStand()->GetAttackData() &&
	//	TargetPlayer->GetStand()->isAttackFrame())
	//{
	//	// 距離
	//	return l_fRange = fabsf(TargetPlayer->GetStand()->GetPos().x - MyPlayer->GetPos().x);
	//}
	//else
	//{
	//	l_fRange = 0.0f;
	//}	

	if (TargetPlayer->GetStand()->isActive() == true)
	{
		l_fRange = TargetPlayer->GetStand()->GetPos().x
			+ TargetPlayer->GetStand()->GetAttackData()->pCollisionShape->pos.x * ((TargetPlayer->GetStand()->GetDir() == DIR::LEFT) ? -1.0f : 1.0f);

	}else
	{
		l_fRange = 0.0f;
	}

	return abs(l_fRange - MyPlayer->GetPos().x);

}

// 相手の遠距離と自分のXの距離
//float GetRangeVsShot(AI * pPerson)
//{
//	float l_fRange = 0.0f;
//
//	if (TargetPlayer->GetStand()->isActive() == true)
//	{
//		l_fRange = TargetPlayer->GetStand()->GetPos().x;
//
//	}
//	else
//	{
//		l_fRange = 0.0f;
//	}
//
//	return abs(l_fRange - MyPlayer->GetPos().x);
//
//}

// 難易度が低い追いかけるステート　ものすごくしょぼい
bool EasyTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		ガード
	//+----------------------------
	// 相手が攻撃してきたら。
	if (TargetPlayer->isAttackState() == true)
	{
		if (tdnRandom::Get(0, 100) <= 20)
		{
			// ガード
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;

		}else
		{
			// 何もしない　やられよう
			pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			return true;
		}

	}


	// 相手が無敵じゃなかったら
	//if (TargetPlayer->GetInvincibleTime() <= 0)
	{
		// もじもじタイマーがある一定時間を過ぎてから行動ができる
		if (pPerson->GetWaitFrame() >= 60)
		{

			// ジャンプしてみる
			if (tdnRandom::Get(0, 100) <= 15)// ←の確率で
			{
				pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
				return true;

			}

			// 後ろに下がってみる　（こっちに来たのにもどるのか）
			if (tdnRandom::Get(0, 100) <= 30)// ←の確率で
			{
				pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
				return true;
			}

			//+---------------------------------------
			// 相手が地上・空中にいるかで動きを変える
			//+---------------------------------------
			if (pPerson->GetTargetPlayer()->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
				return true;
			}

		}
	}

	return false;
}


// 難易度が普通の追いかけるステート　まあまあ
bool NormalTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		ガード
	//+----------------------------
	// 相手が攻撃してきたら。
	if (TargetPlayer->isAttackState() == true)
	{
		// 5%の確率で無敵
		if (tdnRandom::Get(0, 100) <= 5)
		{
			// 無敵技(リバーサル昇竜)
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
			return true;

		}
		else if (tdnRandom::Get(0, 100) <= 30)
		{
			// ガード
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;

		}
		else
		{
			// 何もしない　やられよう
			pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			return true;
		}

	}


	// 相手が無敵じゃなかったら
	if (TargetPlayer->GetInvincibleTime() <= 0)
	{

		// もじもじタイマーがある一定時間を過ぎてから行動ができる
		if (pPerson->GetWaitFrame() >= 40)
		{
			// 弱いステート　ノーマルでも少しの確率で残しておく
			{
				// ジャンプしてみる
				if (tdnRandom::Get(0, 100) <= 10)// ←の確率で
				{
					pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
					return true;
				}

				// 後ろに下がってみる　（こっちに来たのにもどるのか）
				if (tdnRandom::Get(0, 100) <= 25)// ←の確率で
				{
					pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
					return true;
				}
			}

			// 投げステート
			if (tdnRandom::Get(0, 100) <= 20)// ←の確率で飛ぶ
			{
				// ★相手がダウンしていないときかつ、飛んでいないとき
				if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
				{
					pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
					return true;
				}
			}

			// 空中攻撃
			if (tdnRandom::Get(0, 100) <= 15)// ←の確率で飛ぶ
			{
				// 前回空中崩し攻撃をかましていなかったら
				if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAttack::GetInstance()) != true)
				{
					pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
					return true;
				}
			}

			//+---------------------------------------
			// 相手が地上・空中にいるかで動きを変える
			//+---------------------------------------
			if (pPerson->GetTargetPlayer()->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
				return true;
			}

		}
	}
	return false;
}

// 難易度が高い追いかけるステート　様々な選択肢がある
bool HardTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		ガード・切り替えし
	//+----------------------------
	// 相手が攻撃してきたら。
	if (TargetPlayer->isAttackState() == true)
	{
		int ram = tdnRandom::Get(0, 100);

		// 15%の確率で無敵
		if (ram <= 15)
		{
			// 無敵技(リバーサル昇竜)
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
			return true;

		}
		else if (ram <= 40)
		{
			// 回避ステート
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return true;
		}
		else if (ram <= 70)
		{
			// ガード
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;
		}
		else
		{
			// 何もしない　やられよう
			pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			return true;
		}
	}


	// 相手が無敵じゃなかったら
	if (TargetPlayer->GetInvincibleTime() <= 0)
	{

		// 投げステート
		if (tdnRandom::Get(0, 100) <= 20)// ←の確率で飛ぶ
		{
			// ★相手がダウンしていないときかつ、飛んでいないとき
			if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)// ←の確率で飛ぶ
		{
			// 前回空中崩し攻撃をかましていなかったら
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAttack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)//	←の確率で飛ぶ
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return  true;
		}

		if (tdnRandom::Get(0, 100) <= 20)//	←の確率で飛ぶ
		{
			// 無敵歩行（逆切れ対策）
			pPerson->GetFSM()->ChangeState(AIState::MightyWark::GetInstance());
			return true;
		}

		//+---------------------------------------
		// 相手が地上・空中にいるかで動きを変える
		//+---------------------------------------
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
			return  true;
		}
		else
		{
			// 相手が空中にいる場合は走っていようが歩いてでも当たる場所に行く
			//float  nearLen = MyPlayer->AIAttackRange();	// 自分のキャラクターのレンジ

			//TargetPlayer->GetRecoveryFrame

			// 2分の1の確立で対空攻撃
			if (tdnRandom::Get(0, 2) == 0)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return true;
			}
			else
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return true;
			}

		}
	}

	return false;
}



// 難易度が最強の追いかけるステート　最強
bool YokoeTypeChaseBrain(AI * pPerson)
{

	//+----------------------------
	//		ガード・切り替えし
	//+----------------------------
	// 相手が攻撃してきたら。
	if (TargetPlayer->isAttackState() == true)
	{
		int ram = tdnRandom::Get(0, 100);

		// 40%の確率で無敵
		if (ram <= 40)
		{
			// 無敵技(リバーサル昇竜)
			pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
			return true;

		}
		else if (ram <= 65)
		{
			// 回避ステート
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return true;
		}
		else
		{
			// ガード
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;
		}
	}

	// 相手のスタンドが発動してたら	
	if (TargetPlayer->GetStand()->isActive() &&
		BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetStand()->GetAttackData()->pCollisionShape->width >= GetRangeVsStand(pPerson))//
	{
		int l_iRam = tdnRandom::Get(0, 100);
		if (l_iRam <= 35)
		{
			// 回避ステート
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
			return true;
		}
		else
		{
			// ガード
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return true;
		}

	}

	// 相手が無敵じゃなかったら
	if (TargetPlayer->GetInvincibleTime() <= 0)
	{

		// 投げステート
		if (tdnRandom::Get(0, 100) <= 20)// ←の確率で飛ぶ
		{
			// ★相手がダウンしていないときかつ、飛んでいないとき
			if (TargetPlayer->isDown() == false && TargetPlayer->isLand() == true)
			{
				pPerson->GetFSM()->ChangeState(AIState::SetThrow::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 5)// ←の確率で飛ぶ
		{
			// 前回空中崩し攻撃をかましていなかったら
			if (pPerson->GetFSM()->isPrevState(*AIState::AerialDestructAttack::GetInstance()) != true)
			{
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return true;
			}
		}

		if (tdnRandom::Get(0, 100) <= 15)//	←の確率で飛ぶ
		{
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return  true;
		}

		if (tdnRandom::Get(0, 100) <= 25)//	←の確率で飛ぶ
		{
			// 無敵歩行（逆切れ対策）
			pPerson->GetFSM()->ChangeState(AIState::MightyWark::GetInstance());
			return true;
		}

		//+---------------------------------------
		// 相手が地上・空中にいるかで動きを変える
		//+---------------------------------------
		if (pPerson->GetTargetPlayer()->isLand() == true)
		{
			pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
			return  true;
		}
		else
		{
			// 相手が空中にいる場合は走っていようが歩いてでも当たる場所に行く
			//float  nearLen = MyPlayer->AIAttackRange();	// 自分のキャラクターのレンジ


			// ★（難易度に属する物）製作者の慈悲により絶対対空当てるマンを確立に

			// 2分の1の確立で対空攻撃
			//if (tdnRandom::Get(0, 2) == 0)
			//{
				pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
				return true;
			//}
			//else
			//{
			//	pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
			//	return true;
			//}

		}
	}
	return false;
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
		// なげ抜けステートにまだ移行してなかったら
		if (pPerson->GetFSM()->isInState(*AIState::ThrowTech::GetInstance()) == false)
		{
			// 投げ抜け判断ステートへ
			pPerson->GetFSM()->ChangeState(AIState::ThrowTech::GetInstance());
			return;
		}
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
	// 投げに捕まったら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true)
	{
		// 設定で投げ抜けするようにしていたら
		if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyThrowTech == ENEMY_THROW_TECH_TYPE::OK)
		{
			// 投げ抜け
			pPerson->PushInputList(PLAYER_INPUT::A);
		}
	}


	// ガードの設定
	switch ((ENEMY_GUARD_TYPE)SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuard)
	{
	case ENEMY_GUARD_TYPE::NO:
		// ガードしない
		pPerson->m_bPracticeGuardFlag = false;
		break;
	case ENEMY_GUARD_TYPE::WAY:
		// 途中からガード

		break;
	case ENEMY_GUARD_TYPE::ALL:
		// 全部ガードだった場合常にガードし続ける
		pPerson->m_bPracticeGuardFlag = true;
		break;
	default:
		MyAssert(0, "そのタイプはない");
		break;
	}


	// ★何か攻撃を受けて倒れる最中ならば
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// 受け身
		switch ((ENEMY_TECH_TYPE)SelectDataMgr->Get()->tagTrainingDatas.eEnemyTech)
		{
		case ENEMY_TECH_TYPE::NO:
			// 何もボタンを押さない

			break;
		case ENEMY_TECH_TYPE::LAND:
			if (MyPlayer->isLand() == true)
			{
				// 復帰ボタン
				pPerson->PushInputList(PLAYER_INPUT::B);
			}
			break;
		case ENEMY_TECH_TYPE::AIR:
			if (MyPlayer->isLand() == false)
			{
				// 復帰ボタン
				pPerson->PushInputList(PLAYER_INPUT::B);
			}
			break;
		case ENEMY_TECH_TYPE::ALL:
			// 復帰ボタン
			pPerson->PushInputList(PLAYER_INPUT::B);
			break;
		default:
			MyAssert(0, "そのタイプはない");
			break;
		}


	}

	// 倒れないノックバック技を受けたなら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true)
	{
		pPerson->m_bPracticeGuardFlag = true;
		pPerson->m_iPracticeGuardFrame = 0;
	}

	// 練習用ガードフラグが立ってたら+地上にいたら　
	if (pPerson->m_bPracticeGuardFlag == true &&
		MyPlayer->isLand() == true && 
		MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == false)
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
		



		// 相手のスタンドが発動してたら	
		if (TargetPlayer->GetStand()->isActive() &&
			TargetPlayer->GetStand()->isCanGuardStand() &&
			BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetStand()->GetAttackData()->pCollisionShape->width  >= GetRangeVsStand(pPerson))//
		{
			// ★設定によりガードを切り替えるか変えないか設定する
			if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch == ENEMY_GUARD_SWITCH_TYPE::OK)
			{
				// 相手と逆方向レバーを押す
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}

				// 下段なら
				if (TargetPlayer->GetStand()->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
				{
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
				}

			}
			else
			{

				// 相手と逆方向レバーを押す
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}


			}// 中段か下段か

		}// 相手スタンドに対するガード


		// 遠距離の判定
		for (auto it : *ShotMgr->GetList(pPerson->GetTargetPlayer()->GetSide()))
		{
			float fRange = fabsf((it)->GetPos().x - pPerson->GetBasePlayer()->GetPos().x);

			// 遠距離技との距離が縮まれば
			if (BasePlayer::c_GUARD_ADD_RANGE + (it)->GetAttackData()->pCollisionShape->width  > fRange)
			{
				// ★設定によりガードを切り替えるか変えないか設定する
				if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch == ENEMY_GUARD_SWITCH_TYPE::OK)
				{
					// 相手と逆方向レバーを押す
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}

					// 下段なら
					if ((it)->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
					{
						pPerson->PushInputList(PLAYER_INPUT::DOWN);
					}

				}
				else
				{

					// 相手と逆方向レバーを押す
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}


				}// 中段か下段か

			}


		}// 遠距離の判定

		 // 相手が攻撃振ったらかつ
		 // 	相手の技が近くにきたら
		if (TargetPlayer->isAttackState() == true &&
			BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetAttackData()->pCollisionShape->width >= GetRangeVsAttack(pPerson))// (TODO)
		{
			// 投げだったらハジク(投げに対して移動してしまうため)
			if (TargetPlayer->GetActionState() != BASE_ACTION_STATE::THROW)
			{

				// ★設定によりガードを切り替えるか変えないか設定する
				if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch == ENEMY_GUARD_SWITCH_TYPE::OK)
				{
					// 相手と逆方向レバーを押す
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
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
				else
				{

					// 相手と逆方向レバーを押す
					if (MyPlayer->GetTargetDir() == DIR::LEFT)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}


				}// 中段か下段か

			}// 投げだったらハジク

		}// プレイヤーに対するガード

	}// 練習用ガードフラグ

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

	// ガード状態じゃなかったら
	if (pPerson->m_bPracticeGuardFlag == false)
	{
		// 地上に付いたらジャンプする
		if (MyPlayer->isLand())
		{
			// ジャンプ
			pPerson->PushInputList(PLAYER_INPUT::UP);
		}
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
//				練習用地面しゃがみステート
/*******************************************************/

AIState::PracticeSquat * AIState::PracticeSquat::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeSquat instance;
	return &instance;
}

void AIState::PracticeSquat::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);

}

void AIState::PracticeSquat::Execute(AI * pPerson)
{	
	// 練習用ガードフラグ
	if (pPerson->m_bPracticeGuardFlag == true/*&& MyPlayer->GetRecoveryFrame() <= 0　ガード硬直も含むのでなし*/)
	{
		// 相手が攻撃振っているか
		if (TargetPlayer->isAttackState() == true &&
			BasePlayer::c_GUARD_ADD_RANGE + TargetPlayer->GetAttackData()->pCollisionShape->width  >= GetRangeVsAttack(pPerson))// (TODO)
		{
			// ★設定によりガードを切り替えるか変えないか
			if (SelectDataMgr->Get()->tagTrainingDatas.eEnemyGuardSwitch 
				== ENEMY_GUARD_SWITCH_TYPE::OK)
			{
				// しゃがまない

			}
			else
			{
				// 常にしゃがみ状態
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
			}

		}
		else
		{
			// 常にしゃがみ状態
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
		}

	}
	else
	{
		// 常にしゃがみ状態
		pPerson->PushInputList(PLAYER_INPUT::DOWN);
	}



}

void AIState::PracticeSquat::Exit(AI * pPerson)
{
}

void AIState::PracticeSquat::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI地面しゃがみ");

#endif // _DEBUG

}

bool AIState::PracticeSquat::OnMessage(AI * pPerson, const Message & msg)
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
//				練習用ガードステート
/*******************************************************/

AIState::PracticeGuard * AIState::PracticeGuard::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeGuard instance;
	return &instance;
}

void AIState::PracticeGuard::Enter(AI * pPerson)
{
	//pPerson->SetPracticeGuardFrame(120);
	//pPerson->SetPracticeGuardFlag(true);

}

void AIState::PracticeGuard::Execute(AI * pPerson)
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
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
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

void AIState::PracticeGuard::Exit(AI * pPerson)
{
}

void AIState::PracticeGuard::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI練習ガード");

#endif // _DEBUG

}

bool AIState::PracticeGuard::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}

/*******************************************************/
//				無敵割り込みチェック用ステート
/*******************************************************/

AIState::InterruptInvincible * AIState::InterruptInvincible::GetInstance()
{
	// ここに変数を作る
	static AIState::InterruptInvincible instance;
	return &instance;
}

void AIState::InterruptInvincible::Enter(AI * pPerson){}

void AIState::InterruptInvincible::Execute(AI * pPerson)
{
	// ★何か攻撃を受けて倒れる最中ならば
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true
		)
	{
		// 復帰ボタン
		pPerson->PushInputList(PLAYER_INPUT::B);

		pPerson->m_bPracticeGuardFlag = false;
		pPerson->m_iGuardFrame = 0;
	}

	// ガード中
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Guard::GetInstance()))
	{
		// まだ相手の攻撃をガードしていない状態(モーションだけガードしてる状態)
		if (!pPerson->m_bPracticeGuardFlag)
		{
			// ガードリカバリーフレームが0以上、すなわち相手の攻撃を受けたらフラグをONにする
			pPerson->m_bPracticeGuardFlag = (MyPlayer->GetRecoveryFrame() > 0);
		}

		// ガードを1度でも受けた場合のif文
		else
		{
			// 2フレーム単位で無敵攻撃ボタンを連打する
			if (++pPerson->m_iGuardFrame % 2) pPerson->PushInputList(PLAYER_INPUT::L2);
		}
	}

	// 無敵攻撃を振ってたら、じょうほうをりせっと　
	else if (MyPlayer->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()))
	{
		pPerson->m_bPracticeGuardFlag = false;
		pPerson->m_iGuardFrame = 0;
	}

	// 相手が攻撃振ったら
	if (TargetPlayer->isAttackState() == true)
	{
		// 相手と逆方向レバーを押す
		if (MyPlayer->GetTargetDir() == DIR::LEFT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}

		// 基本下段ガード
		if (TargetPlayer->GetAttackData()->AntiGuard != ANTIGUARD_ATTACK::UP_ATTACK)
		{
			pPerson->PushInputList(PLAYER_INPUT::DOWN);
		}
	}
}

void AIState::InterruptInvincible::Exit(AI * pPerson)
{
}

void AIState::InterruptInvincible::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AI無敵割り込みチェック用");

#endif // _DEBUG

}

bool AIState::InterruptInvincible::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//				練習用中段攻撃ステート
/*******************************************************/

AIState::PracticeDokkoiAttack* AIState::PracticeDokkoiAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeDokkoiAttack instance;
	return &instance;
}

void AIState::PracticeDokkoiAttack::Enter(AI * pPerson)
{
	pPerson->m_iPracticeAttackIntervalFrame = 60;

}

void AIState::PracticeDokkoiAttack::Execute(AI * pPerson)
{

	// 一定時間がたったら攻撃
	pPerson->m_iPracticeAttackIntervalFrame++;
	if (pPerson->m_iPracticeAttackIntervalFrame >= 90)
	{


		// 相手方向に歩く
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		if (MyPlayer->GetRecoveryFrame() <= 0 &&	// 自分が攻撃終わったら
			MyPlayer->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) == false)
		{
			if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
			else
			{
				// 相手の懐に入ったら
				pPerson->m_iPracticeAttackIntervalFrame = 0;
				// 中段攻撃
				pPerson->PushInputList(PLAYER_INPUT::R1);
			}
		}




	}


}

void AIState::PracticeDokkoiAttack::Exit(AI * pPerson)
{
}

void AIState::PracticeDokkoiAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AIPracticeDokkoiAttack練習");

#endif // _DEBUG

}

bool AIState::PracticeDokkoiAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//				練習用下段攻撃ステート
/*******************************************************/

AIState::PracticeDownAttack* AIState::PracticeDownAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::PracticeDownAttack instance;
	return &instance;
}

void AIState::PracticeDownAttack::Enter(AI * pPerson)
{
	pPerson->m_iPracticeAttackIntervalFrame = 60;

}

void AIState::PracticeDownAttack::Execute(AI * pPerson)
{

	// 一定時間がたったら攻撃
	pPerson->m_iPracticeAttackIntervalFrame++;
	if (pPerson->m_iPracticeAttackIntervalFrame >= 90)
	{

		// 相手方向に歩く
		Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
		Vector3 MyPos = MyPlayer->GetPos();
		if (MyPlayer->GetRecoveryFrame() <= 0)// 自分が攻撃終わったら
		{
			if (TargetPos.x > MyPos.x + 14)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			else if (TargetPos.x < MyPos.x - 14)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
			else
			{
				// 相手の懐に入ったら
				pPerson->m_iPracticeAttackIntervalFrame = 0;
				// 下段攻撃
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
				pPerson->PushInputList(PLAYER_INPUT::C);
			}
		}

	}


}

void AIState::PracticeDownAttack::Exit(AI * pPerson)
{
}

void AIState::PracticeDownAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AIPracticeDownAttack練習");

#endif // _DEBUG

}

bool AIState::PracticeDownAttack::OnMessage(AI * pPerson, const Message & msg)
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
		// 攻撃へ
		//pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
		//return;

		// 確率  (値が低いほど性能の良い技がでる)
		int l_iStrong= tdnRandom::Get(0, 100);
		// 難易度による補正値
		switch (pPerson->m_eAIType)
		{
		case AI_TYPE::CPU_EASY:
			l_iStrong += 50;
			break;
		case AI_TYPE::CPU_NORMAL:
			l_iStrong += 25;
			break;
		case AI_TYPE::CPU_HARD:
			l_iStrong += 0;
			break;
		case AI_TYPE::CPU_YOKOE:
			l_iStrong += -25;
			break;
		default:
			break;
		}

		// 相手が起き上がり攻撃被せてきたら
		if (TargetPlayer->isAttackState() == true &&
			TargetPlayer->AIAttackRange() > GetRange(pPerson))
		{
			// ↓%の確率で無敵
			if (l_iStrong <= 30)
			{
				// 無敵技(リバーサル昇竜)
				pPerson->GetFSM()->ChangeState(AIState::InvincibleAttack::GetInstance());
				return;
			} 		
			else if (l_iStrong <= 60)	// ←%の確率でガード
			{
				// ガード
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}

		}

		// 最高難易度限定
		if (pPerson->m_eAIType == AI_TYPE::CPU_YOKOE)
		{
			// 相手のスタンドが発動してたら	
			if (TargetPlayer->GetStand()->isActive() &&
				30 >= GetRangeVsStand(pPerson))//
			{
				if (l_iStrong <= 25)
				{
					// 回避ステート
					pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
					return;
				}
				else
				{
					// ガード
					pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
					return;
				}
			}
		}

		// ★パートナーゲージが溜まっていて
		// 相手にダメージが通る場合
		// 低確率でパートナー発動
			if (TargetPlayer->GetInvincibleTime() <= 0 &&
				TargetPlayer->isAttackState() == false && 
				MyPlayer->GetStand()->GetStandStock() >= 1)
		{
			// 確率 
			int l_iPartnerPow = 0;
			// 難易度による補正値
			switch (pPerson->m_eAIType)
			{
			case AI_TYPE::CPU_EASY:
				l_iPartnerPow = 5;
				break;
			case AI_TYPE::CPU_NORMAL:
				l_iPartnerPow = 15;
				break;
			case AI_TYPE::CPU_HARD:
				l_iPartnerPow = 25;
				break;
			case AI_TYPE::CPU_YOKOE:
				l_iPartnerPow = 30;
				break;
			default:
				break;
			}

			if (tdnRandom::Get(0, 100) <= l_iPartnerPow)
			{
				// パートナー発動モードに
				pPerson->GetFSM()->ChangeState(AIState::PartnerAttack::GetInstance());
				return;
			}
		}

		// 相手にダメージが通る　かつ　低確率でスキル技も出してみる
		if (TargetPlayer->GetInvincibleTime() <= 0 && 
			TargetPlayer->isAttackState() == false)
		{
			if (tdnRandom::Get(0, 100) <= 7)
			{
				// スキルモードに
				pPerson->GetFSM()->ChangeState(AIState::SkillAttack::GetInstance());
				return;
			}
		}

		//if (pPerson->m_eAIType == AI_TYPE::CPU_EASY) 
		//{
		//	// 3分の1で
		//	if (tdnRandom::Get(0, 3) <= 0)
		//	{
		//		// 後ろへ歩く
		//		pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
		//		return;
		//	}
		//}
		if (pPerson->m_eAIType == AI_TYPE::CPU_EASY||
			pPerson->m_eAIType == AI_TYPE::CPU_NORMAL)
		{
			// 5分の1で
			if (tdnRandom::Get(0, 5) <= 0)
			{
				// 前回後ろ歩きしていなかったら
				if (pPerson->GetFSM()->isPrevState(*AIState::BackWark::GetInstance()) != true)
				{
					// 後ろへ歩く
					pPerson->GetFSM()->ChangeState(AIState::BackWark::GetInstance());
					return;
				}
			}
		}

		// 空中か地上どちらで追いかけるか
		if (tdnRandom::Get(0, 100) <= 20)
		{
			// イージーモードなら弱体化
			if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)
			{
				// 前回飛んでいなかったら
				if (pPerson->GetFSM()->isPrevState(*AIState::SimpleJump::GetInstance()) != true)
				{
					// ジャンプモードに
					pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
					return;
				}
				else
				{
					// 追跡モードに
					pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
					return;
				}

			}
			else
			{
				// 空中追跡モードに
				pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
				return;
			}

		}
		else
		{
			// もしイージーモードなら
			if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)
			{	
				// 前回飛んでいなかったら
				if (pPerson->GetFSM()->isPrevState(*AIState::SimpleJump::GetInstance()) != true)
				{
					// 4分の1で
					if (tdnRandom::Get(0, 4) <= 0)
					{
						// 後ろへ歩く
						pPerson->GetFSM()->ChangeState(AIState::SimpleJump::GetInstance());
						return;
					}
				}
			}

			// 追跡モードに
			pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
			return;
		}

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
	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	//+---------------------------------------------------
	// 近距離攻撃が当たらないほど相手が離れてたら
	//+---------------------------------------------------
	// 踏み込み時限定
	if (MyPlayer->GetJump()->bHold == true)
	{
		if (abs(TargetPlayer->GetPos().x - MyPlayer->GetPos().x)
			>= MyPlayer->AIAttackRange())
		{
			// 相手方向にキー入力
			if (TargetPlayer->GetPos().x > MyPlayer->GetPos().x)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}
		}
	}

	//+-----------------------------------------
	//	終了処理
	//+-----------------------------------------
	// ジャンプの踏み込みはカウントしない
	if (MyPlayer->GetJump()->bHold == false)
	{
		// 地上に着地したら かつ　待機ステートにいれば
		if (MyPlayer->isLand() && MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
		{
			// AI待機に戻る
			pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
			return;
		}
	}

}

void AIState::SimpleJump::Exit(AI * pPerson)
{
}

void AIState::SimpleJump::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "AIシンプルジャンプ");
#endif // _DEBUG

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
	// かつスタンドも出していなかったら解除(2/4)
	if (pPerson->GetTargetPlayer()->isAttackState() == false &&
		TargetPlayer->GetStand()->isActive() == false)
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

	// 確率  (値が低いほど性能の良い技がでる)
	int l_iRamdom = tdnRandom::Get(0, 100);
	// 難易度による補正値
	switch (pPerson->m_eAIType)
	{
	case AI_TYPE::CPU_EASY:
		l_iRamdom += 45;
		break;
	case AI_TYPE::CPU_NORMAL:
		l_iRamdom += 25;
		break;
	case AI_TYPE::CPU_HARD:
		l_iRamdom += 0;
		break;
	case AI_TYPE::CPU_YOKOE:
		l_iRamdom += -70;
		break;
	default:
		break;
	}
	//if(いーじーだったら)
	//{
	//}else
	//{

	// 相手が攻撃振ったら
	if (TargetPlayer->isAttackState() == true)
	{
		if (l_iRamdom <= 20) // ←の確率で完全ガード
		{
			// 相手と逆方向レバーを押す
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
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
		else  if (l_iRamdom <= 50) 
		{
			// しゃがみガード
			// 相手と逆方向レバーを押す
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}

			if (TargetPlayer->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
			{
				pPerson->PushInputList(PLAYER_INPUT::DOWN);
			}
		}
		else 
		{
			// 立ちガード
			// 相手と逆方向レバーを押す
			if (MyPlayer->GetTargetDir() == DIR::LEFT)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}
		}
	}

	// スタンドパートナーに対するガード
	if (pPerson->m_eAIType == AI_TYPE::CPU_YOKOE)
	{

		// 相手のスタンドが発動してたら	
		if (TargetPlayer->GetStand()->isActive() &&
			30 >= GetRangeVsStand(pPerson))//
		{
			if (l_iRamdom <= 20) // ←の確率で完全ガード
			{
				// 相手と逆方向レバーを押す
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}

				// 下段なら
				if (TargetPlayer->GetStand()->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
				{
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
				}
			}
			else  if (l_iRamdom <= 50)
			{
				// しゃがみガード
				// 相手と逆方向レバーを押す
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}

				if (TargetPlayer->GetStand()->GetAttackData()->AntiGuard == ANTIGUARD_ATTACK::DOWN_ATTACK)
				{
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
				}
			}
			else
			{
				// 立ちガード
				// 相手と逆方向レバーを押す
				if (MyPlayer->GetTargetDir() == DIR::LEFT)
				{
					pPerson->PushInputList(PLAYER_INPUT::RIGHT);
				}
				else
				{
					pPerson->PushInputList(PLAYER_INPUT::LEFT);
				}
			}

		}// 相手スタンドに対するガード
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

	// もじもじタイマー初期化
	pPerson->m_iWaitFrame = 0;
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
		len = MyPlayer->AIRunAttackRange();	// 自分のキャラクターのレンジ
	else len = MyPlayer->AIAttackRange();	// 自分のキャラクターのレンジ
	
	//+-------------------------------------------------
	//	↑の距離で一定まで近づいたら　(ランダム)で攻撃 (1210) xの距離に変更 
	//+-------------------------------------------------

	if (Math::Length(TargetPos.x, MyPos.x) < len)
	{
		bool bReturn = false;

		// AIのタイプによりここを変更
		switch (pPerson->m_eAIType)
		{
		case AI_TYPE::CPU_EASY:
			bReturn = EasyTypeChaseBrain(pPerson);
			break;
		case AI_TYPE::CPU_NORMAL:
			bReturn = NormalTypeChaseBrain(pPerson);
			break;
		case AI_TYPE::CPU_HARD:
			bReturn = HardTypeChaseBrain(pPerson);
			break;
		case AI_TYPE::CPU_YOKOE:
			bReturn = YokoeTypeChaseBrain(pPerson);
			break;
		default:
			break;
		}
	
		// リターンフラグが立ってたら
		if (bReturn == true)return;
		
	}
	

	//+-----------------------------------------
	// ダッシュ東野　(12/10) 移動完璧
	//+-----------------------------------------
	int iDushRange = 25;
	if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)iDushRange = 60;
	if (pPerson->GetAIType() == AI_TYPE::CPU_NORMAL)iDushRange = 40;

	if (Math::Length(TargetPos, MyPos) >= iDushRange)		// ←のパラメーターが走りに切り替える距離
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
		if (TargetPos.x > MyPos.x + 8)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		else if (TargetPos.x < MyPos.x - 8)	pPerson->PushInputList(PLAYER_INPUT::LEFT);
		else// ↓相手のにぎりぎりまで迫ったときの処理
		{
			// 待機に戻ろう　ここまで近づいて何もしないのは
			// かなりやさしい難易度 
			// しょぼい攻撃へ
			// ここはまた後でなんとかします
			//pPerson->GetFSM()->ChangeState(AIState::EasyAttack::GetInstance());
			//return;
			
			//pPerson->m_iWaitFrame += 100;

		}

		//if (TargetPos.x > MyPos.x )	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		//else 	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	}

		
	/*******************************************/
	//	相手を追いかけすぎないようの新しい行動
	/*******************************************/
	pPerson->m_iChaseFrame++;			// 追いかけタイマー更新
	if (pPerson->m_iChaseFrame >= 300)	// 5秒間粘着して追いかける
	{
		pPerson->m_iChaseFrame = 0; // 初期化
	
		// 待機へ
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// もじもじタイマー更新
	pPerson->m_iWaitFrame++;
}

void AIState::Chase::Exit(AI * pPerson)
{
}

void AIState::Chase::Render(AI * pPerson)
{

#ifdef _DEBUG
	tdnText::Draw(420, 610, 0xffffffff, "AIチェイス");
	tdnText::Draw(420, 660, 0xffffffff, "AI追いかけてるプレイヤーNo%d", pPerson->GetTargetPlayer()->GetDeviceID());
	tdnText::Draw(420, 700, 0xffffffff, "距離%.2f", Math::Length(TargetPlayer->GetPos(), MyPlayer->GetPos()));


#endif // _DEBUG


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

	//+---------------------------------------------------
	// 近距離攻撃が当たらないほど相手が離れてたら
	//+---------------------------------------------------
	// 踏み込み時限定
	if (MyPlayer->GetJump()->bHold == true)
	{
		if (abs(TargetPlayer->GetPos().x - MyPlayer->GetPos().x)
			>= MyPlayer->AIAttackRange())
		{
			// 相手方向にキー入力
			if (TargetPlayer->GetPos().x > MyPlayer->GetPos().x)
			{
				pPerson->PushInputList(PLAYER_INPUT::RIGHT);
			}
			else
			{
				pPerson->PushInputList(PLAYER_INPUT::LEFT);
			}
		}
	}

	//+---------------------------------------------------
	// 相手との距離が一定以上離れていて空中ジャンプの権利があれば
	// さらに地面からそれなりに離れてたら
	//+---------------------------------------------------
	if (abs(TargetPos.x - MyPos.x) >= 35 &&
		MyPlayer->isAerialJump() == true &&
		MyPos.y >= 7.0f)
	{
		// 空中ダッシュ
		if (TargetPos.x > MyPos.x)
		{	
			// ★二回押し 空中ダッシュのために
			pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
		}
		else
		{
			// ★二回押し 空中ダッシュのために
			pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
		}

	}

	//+---------------------------------------------------
	// 攻撃の処理
	//+---------------------------------------------------
	// 浮いてたら
	if (MyPlayer->isLand()==false)
	{
		// 一定まで近づいたら
		if (Math::Length(TargetPos, MyPos) < 32)
		{
			// 落下し始めた瞬間から
			if (MyPlayer->GetMove().y < 0.5f)
			{
				if (pPerson->GetTargetPlayer()->isLand() == false)
				{
					// ※相手も空中にいるなら空中コンボに持っていく
					pPerson->GetFSM()->ChangeState(AIState::AerialAttack::GetInstance());
					return;
				}
			}

			// 少し落下し始めたら
			if (MyPlayer->GetMove().y < -0.3f)
			{
				// 空中下攻撃で制圧
				pPerson->GetFSM()->ChangeState(AIState::AerialDestructAttack::GetInstance());
				return;
			}

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

AIState::RushAttack * AIState::RushAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::RushAttack instance;
	return &instance;
}

void AIState::RushAttack::Enter(AI * pPerson)
{
	// 相手が浮いていたら
	if (pPerson->GetTargetPlayer()->isLand() == false)
	{		
		// AIのタイプによりここを変更
		switch (pPerson->GetAIType())
		{
		case AI_TYPE::CPU_HARD:

			break;
		case AI_TYPE::CPU_YOKOE:
			// 空中攻撃へ
			//pPerson->GetFSM()->ChangeState(AIState::ChaseAir::GetInstance());
			//return;
			// 対空
			pPerson->GetFSM()->ChangeState(AIState::SetAnitiAir::GetInstance());
			return;
			break;
		default:
			break;
		}
		
	}
	

	// 攻撃開始、最初の一段目を振るで！！！
	pPerson->PushInputList(PLAYER_INPUT::C);

	// 二分の一で下段
	if (tdnRandom::Get(0, 1) == 0)
	{
		pPerson->PushInputList(PLAYER_INPUT::DOWN);
	}

}

void AIState::RushAttack::Execute(AI * pPerson)
{

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// プレイヤーが待機にもどってたら＋
	// [1210] GetAtactDataでアサートが起こるので追加　後でバースト見てみるか
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)/*!MyPlayer->isAttackState()*/
	{
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


	// 難易度最高だった場合
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
	{

		// ラッシュ攻撃する途中で相手が先に殴ってきたら＋
		// 自分がまだ攻撃していなかったら
		if (MyPlayer->isAttackState() == false && 
			TargetPlayer->isAttackState() == true)
		{
			//+----------------------------
			//		ガード・切り替えし
			//+----------------------------
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
			return;
		}

	}

	// 相手方向にレバーを倒す うまくつながらん
	//if (TargetPos.x > MyPos.x)
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	//}
	//else
	//{
	//	pPerson->PushInputList(PLAYER_INPUT::LEFT);
	//}


	// 攻撃中！！
	if (MyPlayer->isAttackState() == true)
	{

		// 下段がヒットしたら状態
		if (MyPlayer->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()) == true &&
			MyPlayer->GetAttackData()->bHit == true)
		{
			// 一段目攻撃！
			pPerson->PushInputList(PLAYER_INPUT::C);
			return;
		}


		// 一段目がヒットしたら状態
		if (MyPlayer->GetRushStep() == 0 &&
			MyPlayer->GetAttackData()->bHit == true)
		{
			// 二段目攻撃！
			pPerson->PushInputList(PLAYER_INPUT::C);
			return;
		}

		// 二段目がヒットしたら状態
		if (MyPlayer->GetRushStep() == 1 &&
			MyPlayer->GetAttackData()->bHit == true)
		{

			// (難易度別)もし相手にガードされてたら崩す
			if (pPerson->GetAIType() == AI_TYPE::CPU_HARD ||
				pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
			{

				if (TargetPlayer->GetGuardState() == GUARD_STATE::DOWN_GUARD)
				{
					// 中距離技があたらないなら
					if (abs(TargetPos.x - MyPos.x) <= MyPlayer->AIAttackRange())
					{
						// 確率で中段ステートへ
						if (tdnRandom::Get(0, 1) == 0)
						{
							pPerson->GetFSM()->ChangeState(AIState::BackStep::GetInstance());
							return;

						}
						else
						{
							//pPerson->PushInputList(PLAYER_INPUT::R1);
							pPerson->GetFSM()->ChangeState(AIState::DokkoiAttack::GetInstance());
							return;
						}

					}
					else
					{
						// バックステップ
						pPerson->GetFSM()->ChangeState(AIState::BackStep::GetInstance());
						return;

					}
					//pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
					return;

				}
				else if (TargetPlayer->GetGuardState() == GUARD_STATE::UP_GUARD)
				{
					// 足払い
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
					// 相手の方向レバー
					if (MyPlayer->GetPos().x < TargetPlayer->GetPos().x)
					{
						pPerson->PushInputList(PLAYER_INPUT::RIGHT);
					}
					else
					{
						pPerson->PushInputList(PLAYER_INPUT::LEFT);
					}
					pPerson->PushInputList(PLAYER_INPUT::R1);
					return;
				}

			}// 難易度毎の設定
			

			{
				// 最後三段目攻撃！
				pPerson->PushInputList(PLAYER_INPUT::C);
				return;
			}

		}



		// 三段目がヒットして||ゲージ50%以上溜まってたら
		if (MyPlayer->GetRushStep() == 2 &&
			MyPlayer->GetAttackData()->bHit == true &&
			MyPlayer->GetOverDriveGage() >= 50.0f)
		{
			// ガードされていなかったら
			if (TargetPlayer->GetGuardState() == GUARD_STATE::NO_GUARD)
			{
				// 必殺技発動！！
				pPerson->PushInputList(PLAYER_INPUT::C);
				return;
			}
			else // ガードされていたら待機へ
			{
				pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
				return;
			}
		}

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

void AIState::RushAttack::Exit(AI * pPerson)
{
}

void AIState::RushAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "ラッシュ攻撃");
}

bool AIState::RushAttack::OnMessage(AI * pPerson, const Message & msg)
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
	int l_iRate = tdnRandom::Get(0, 100);	
	
	switch (pPerson->GetAIType())
	{
	case AI_TYPE::CPU_EASY:
		l_iRate += 40;
		break;
	case AI_TYPE::CPU_NORMAL:
		l_iRate += 30;
		break;
	case AI_TYPE::CPU_HARD:
		l_iRate += -30;
		break;
	case AI_TYPE::CPU_YOKOE:
		l_iRate += -50;
		break;
	default:
		break;
	}

	// ↓の数値よりも下なら投げ抜けして
	if (l_iRate <= 40)
	{
		// 投げ抜け
		pPerson->PushInputList(PLAYER_INPUT::A);
	}
	//else
	//{
	//	// 投げ抜け失敗。待機に戻る
	//	pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
	//	return;
	//}


}

void AIState::ThrowTech::Execute(AI * pPerson)
{
	// ↑でなげ抜けをしてなかったらここで足止め

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

	// 投げ範囲内 (自分の攻撃範囲内)
	//enum { ABJUST = 2 };
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIThrowRange() /*/ ABJUST*/))
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


	// 難易度最高だった場合
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
	{
		// 相手の近くにいて＆＆相手が攻撃振ったら
		if (MyPlayer->AIAttackRange() >= GetRange(pPerson) &&
			TargetPlayer->isAttackState() == true)
		{
			if (tdnRandom::Get(0, 1) == 0)
			{
				// ガードへ
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
			else
			{
				// 回避へ
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
		}


		// 相手のスタンドが発動してたら	
		if (TargetPlayer->GetStand()->isActive() &&
			30 >= GetRangeVsStand(pPerson))//
		{
			if (tdnRandom::Get(0, 100) <= 20)
			{
				// 回避ステート
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
			else
			{
				// ガード
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
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
	if (abs(TargetPos.x - MyPos.x) < (MyPlayer->AIAttackRange() / 1.5f))
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

	// 難易度最高だった場合
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)
	{
		// 相手の近くにいて＆＆相手が攻撃振ったら+地面に相手がいてたら
		if (MyPlayer->AIAttackRange() >= GetRange(pPerson) &&
			TargetPlayer->isAttackState() == true&&
			TargetPlayer->isLand() == true)
		{
			if (tdnRandom::Get(0, 1) == 0)
			{
				// ガードへ
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
			else
			{
				// 回避へ
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
		}


	
			// 相手のスタンドが発動してたら	
		if (TargetPlayer->GetStand()->isActive() &&
			30 >= GetRangeVsStand(pPerson))//
		{
			if (tdnRandom::Get(0, 100) <= 25)
			{
				// 回避ステート
				pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
				return;
			}
			else
			{
				// ガード
				pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
				return;
			}
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
	pPerson->PushInputList(PLAYER_INPUT::R1);
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
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAttack::GetInstance());
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

AIState::SetAerialAttack * AIState::SetAerialAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::SetAerialAttack  instance;
	return &instance;
}

void AIState::SetAerialAttack::Enter(AI * pPerson)
{
	// 相手との距離が離れていたら方向キーを入力
	float l_fXLength = abs(MyPlayer->GetPos().x - TargetPlayer->GetPos().x);
	if (l_fXLength >= 3) 
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

void AIState::SetAerialAttack::Execute(AI * pPerson)
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
		// 攻撃できないので戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	Vector3 TargetPos = pPerson->GetTargetPlayer()->GetPos();
	Vector3 MyPos = MyPlayer->GetPos();

	// 左右の移動
	//if (TargetPos.x > MyPos.x+5)	pPerson->PushInputList(PLAYER_INPUT::RIGHT);	
	//else if (TargetPos.x < MyPos.x-5)	pPerson->PushInputList(PLAYER_INPUT::LEFT);

	// 攻撃範囲内
	if ( Math::Length(TargetPos, MyPos) < 18 )
	{
		// (TODO) 見栄えとかで少し見づらい式になってるので後で直す

		// 自分が落ちてるとき
		if (MyPlayer->GetMove().y < -0.0f ||
			MyPlayer->isAerialJump() == false&& MyPlayer->GetMove().y < 1.0f)// 二段ジャンプ目なら落下前にキャンセルできます
		{
			// ジャンプ・エリアルジャンプモードになってたら
			if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Jump::GetInstance()) == true ||
				MyPlayer->GetFSM()->isInState(*BasePlayerState::AerialJump::GetInstance()) == true)
			{

				// 空中攻撃発動ステートへ！
				pPerson->GetFSM()->ChangeState(AIState::AerialAttack::GetInstance());
				return;

			}
		}
	}

}

void AIState::SetAerialAttack::Exit(AI * pPerson)
{

}

void AIState::SetAerialAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SetAerialAttack");
}

bool AIState::SetAerialAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::AerialAttack * AIState::AerialAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::AerialAttack  instance;
	return &instance;
}

void AIState::AerialAttack::Enter(AI * pPerson)
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
		// 〆の強力なスキル発動ステートへ移動！
		pPerson->GetFSM()->ChangeState(AIState::SkillAttack::GetInstance());
		return;
	}
}

void AIState::AerialAttack::Execute(AI * pPerson)
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
				pPerson->GetFSM()->ChangeState(AIState::SetAerialAttack::GetInstance());
				return;
			}
			else
			{
				//pPerson->GetFSM()->ChangeState(AIState::SetAerialAttack::GetInstance());

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

void AIState::AerialAttack::Exit(AI * pPerson)
{

}

void AIState::AerialAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialAttack");
}

bool AIState::AerialAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::AerialDestructAttack * AIState::AerialDestructAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::AerialDestructAttack  instance;
	return &instance;
}

void AIState::AerialDestructAttack::Enter(AI * pPerson)
{

	// 空中攻撃
	pPerson->PushInputList(PLAYER_INPUT::C);
	
}

void AIState::AerialDestructAttack::Execute(AI * pPerson)
{


	// 地上に着地したら
	if (MyPlayer->isLand() == true)
	{
		// 崩しているのだから追跡モードに行くしかない！
		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
		return;
	}
}

void AIState::AerialDestructAttack::Exit(AI * pPerson)
{

}

void AIState::AerialDestructAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "AerialDestructAttack");
}

bool AIState::AerialDestructAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::InvincibleAttack * AIState::InvincibleAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::InvincibleAttack  instance;
	return &instance;
}

void AIState::InvincibleAttack::Enter(AI * pPerson)
{
	// 無敵ボタン
	pPerson->PushInputList(PLAYER_INPUT::L2);

}

void AIState::InvincibleAttack::Execute(AI * pPerson)
{
	// 無敵ステートが終わっていたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) == false)
	{
		//待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	//// InvincibleAttack中
	//if (MyPlayer->GetFSM()->isInState(*BasePlayerState::InvincibleAttack::GetInstance()) == true)
	//{
	//	if (MyPlayer->GetActionFrame() == FRAME_STATE::END)
	//	{
	//		// 追跡モードに戻る
	//		pPerson->GetFSM()->ChangeState(AIState::Chase::GetInstance());
	//		return;
	//	}

	//}
}

void AIState::InvincibleAttack::Exit(AI * pPerson)
{

}

void AIState::InvincibleAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "InvincibleAttack");
}

bool AIState::InvincibleAttack::OnMessage(AI * pPerson, const Message & msg)
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

AIState::SkillAttack * AIState::SkillAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::SkillAttack  instance;
	return &instance;
}

void AIState::SkillAttack::Enter(AI * pPerson)
{
	// ここで現状の相手と自分のポジションで出す技を決める
	// 現在存在する種類は　[近距離]　[中距離]　[遠距離]　[近距離空中]

	// キャラクターのX軸での距離を取得
	float l_fRange= Math::Length(TargetPlayer->GetPos().x, MyPlayer->GetPos().x);
	enum
	{
		NEAR_RANGE = 25,
		MIDDLE_RANGE = 40, 
		//FAR_RANGE = 80,
	};

	// 場所に伴ったコマンドを入力
	bool l_bActionFlag = false;
	if (l_fRange <= NEAR_RANGE)
	{	
		if (MyPlayer->isLand() == true)
		{	
			// 近距離
			l_bActionFlag = MyPlayer->AINearSkill();
		}
		else
		{
			// 空中近距離
			l_bActionFlag = MyPlayer->AIAerialNearSkill();
		}
	}
	else if (l_fRange <= MIDDLE_RANGE)
	{
		// 中距離
		l_bActionFlag = MyPlayer->AIMiddleSkill();
	}
	else
	{
		// 遠距離
		l_bActionFlag = MyPlayer->AIFarSkill();
	}
	

	// 発動できる技がなかった
	if (l_bActionFlag == false)
	{
		// もし当てはまった環境の技が設定されていなかった場合待機に戻る
		//待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


}

void AIState::SkillAttack::Execute(AI * pPerson)
{
	// 待機に戻ってたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// 待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::SkillAttack::Exit(AI * pPerson)
{

}

void AIState::SkillAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "SkillAttack");
}

bool AIState::SkillAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			中段技 ステート
/*******************************************************/

AIState::DokkoiAttack * AIState::DokkoiAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::DokkoiAttack  instance;
	return &instance;
}

void AIState::DokkoiAttack::Enter(AI * pPerson)
{
	// 中段技
	pPerson->PushInputList(PLAYER_INPUT::R1);

	// 初期化
	pPerson->SetWaitFrame(0);
	pPerson->SetDokkoiDush(false);

}

void AIState::DokkoiAttack::Execute(AI * pPerson)
{
	// 待機に戻ってたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// 待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// 中段発動中
	if (MyPlayer->isAttackState() == true && 
		MyPlayer->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) == true)
	{
		// 当たった時
		if (MyPlayer->GetAttackData()->bHit == true) 
		{	
			// ついげきダッシュへ
			pPerson->PushInputList(PLAYER_INPUT::R1);
			// ダッシュフラグ立てる
			pPerson->SetWaitFrame(0);
			pPerson->SetDokkoiDush(true);
		}
	}

	// ラッシュへ
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) == true)
	{
		// ダッシュフラグが立っていたら
		if (pPerson->GetDokkoiDush() == true)
		{
			// ↓のフレームを超えたら
			pPerson->AddWaitFrame(1);
			if (pPerson->GetWaitFrame() >= 20)
			{
				// キャンセル後はラッシュに
				pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
				return;
			}

		}
	}

}

void AIState::DokkoiAttack::Exit(AI * pPerson)
{

}

void AIState::DokkoiAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "DokkoiAttack");
}

bool AIState::DokkoiAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			イージーアタック ステート
/*******************************************************/

AIState::EasyAttack * AIState::EasyAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::EasyAttack  instance;
	return &instance;
}

void AIState::EasyAttack::Enter(AI * pPerson)
{
	pPerson->SetWaitFrame(0);
}

void AIState::EasyAttack::Execute(AI * pPerson)
{
	pPerson->AddWaitFrame(1);

	float fWaitRate = 70;
	
	// 難易度によってレートえお下げる
	if (pPerson->GetAIType() == AI_TYPE::CPU_NORMAL)fWaitRate = 40;
	if (pPerson->GetAIType() == AI_TYPE::CPU_HARD)fWaitRate = 10;
	if (pPerson->GetAIType() == AI_TYPE::CPU_YOKOE)fWaitRate = 5;

	// 自分が負けてたら
	if (MyPlayer->GetHP()<= TargetPlayer->GetHP())
	{
		// HPが少ないほど強く！
		// ユーザーに敵を骨のある感触にさせるために
		fWaitRate *= (MyPlayer->GetHPPercentage()*0.01f);
	}
	
	if (pPerson->GetWaitFrame() >= (int)fWaitRate)
	{
		// まだこれだけ待って相手が近くにいたら攻撃
		if (GetRange(pPerson) <=  MyPlayer->AIAttackRange())
		{
			if (pPerson->GetAIType() == AI_TYPE::CPU_EASY)
			{
				// 3分の一でラッシュに
				if (tdnRandom::Get(0, 100) <= 30)
				{
					// ラッシュに
					pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
					return;
				}
				else
				{
					// 一発殴っておわり
					pPerson->PushInputList(PLAYER_INPUT::C);
					pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
					return;
				}
			}
			else
			{
				// 45%でラッシュに
				if (tdnRandom::Get(0, 100) <= 45)
				{
					// ラッシュに
					pPerson->GetFSM()->ChangeState(AIState::RushAttack::GetInstance());
					return;
				}
				else
				{
					// 一発殴っておわり
					pPerson->PushInputList(PLAYER_INPUT::DOWN);
					pPerson->PushInputList(PLAYER_INPUT::C);	
					pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
					return;
				}
			}


		}
		else 
		{
			// 離れていたらスキルで試みてみる
			pPerson->GetFSM()->ChangeState(AIState::SkillAttack::GetInstance());
			return;
		}

	}	
}

void AIState::EasyAttack::Exit(AI * pPerson)
{

}

void AIState::EasyAttack::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "EasyAttack");
#endif // _DEBUG

}

bool AIState::EasyAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			後ろ歩きステート
/*******************************************************/

AIState::BackWark * AIState::BackWark::GetInstance()
{
	// ここに変数を作る
	static AIState::BackWark  instance;
	return &instance;
}

void AIState::BackWark::Enter(AI * pPerson)
{
	pPerson->SetWaitFrame(0);
}

void AIState::BackWark::Execute(AI * pPerson)
{
	// 相手が攻撃してないとき
	if (TargetPlayer->isAttackState() == false)
	{
		// ひたすら後ろに歩くだけ
		if (MyPlayer->GetDir() == DIR::LEFT)
		{
			pPerson->PushInputList(PLAYER_INPUT::RIGHT);
		}
		else
		{
			pPerson->PushInputList(PLAYER_INPUT::LEFT);
		}
	}

	pPerson->AddWaitFrame(1);
	if (pPerson->GetWaitFrame() >= 60)
	{
		// 待機へ
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::BackWark::Exit(AI * pPerson)
{

}

void AIState::BackWark::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "BackWark");

#endif // _DEBUG
}

bool AIState::BackWark::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}



/*******************************************************/
//			最強の歩行 (攻撃してきたらガードへ)ステート
/*******************************************************/

AIState::MightyWark * AIState::MightyWark::GetInstance()
{
	// ここに変数を作る
	static AIState::MightyWark  instance;
	return &instance;
}

void AIState::MightyWark::Enter(AI * pPerson)
{
	// 相手の方向に進む
	if (MyPlayer->GetDir() == DIR::RIGHT)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}

	pPerson->SetWaitFrame(0);
}

void AIState::MightyWark::Execute(AI * pPerson)
{
	// 相手の方向に進む
	if (MyPlayer->GetDir() == DIR::RIGHT)
	{
		pPerson->PushInputList(PLAYER_INPUT::RIGHT);
	}
	else
	{
		pPerson->PushInputList(PLAYER_INPUT::LEFT);
	}


	// 相手の近くにいて＆＆相手が攻撃振ったら
	if (MyPlayer->AIAttackRange() >= GetRange(pPerson) && 
		TargetPlayer->isAttackState() == true)
	{
		if (tdnRandom::Get(0, 1) == 0)
		{
			// ガードへ
			pPerson->GetFSM()->ChangeState(AIState::Guard::GetInstance());
		}else
		{
			// 回避へ
			pPerson->GetFSM()->ChangeState(AIState::Escape::GetInstance());
		}
		return;
	}

	// 歩い一定まで
	pPerson->AddWaitFrame(1);
	if (pPerson->GetWaitFrame() >= 20)
	{
		// 待機へ
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::MightyWark::Exit(AI * pPerson)
{

}

void AIState::MightyWark::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "MightyWark");

#endif // _DEBUG
}

bool AIState::MightyWark::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			バックステップステート
/*******************************************************/

AIState::BackStep * AIState::BackStep::GetInstance()
{
	// ここに変数を作る
	static AIState::BackStep  instance;
	return &instance;
}

void AIState::BackStep::Enter(AI * pPerson)
{
	pPerson->SetWaitFrame(0);
	
	// バックステップ
	if (TargetPlayer->GetPos().x < MyPlayer->GetPos().x)
	{
		// ★二回押し
		pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
	}
	else
	{
		// ★二回押し
		pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
	}

}

void AIState::BackStep::Execute(AI * pPerson)
{
	// バックステップ
	if (TargetPlayer->GetPos().x < MyPlayer->GetPos().x)
	{
		// ★二回押し
		pPerson->ActionDoublePush(PLAYER_INPUT::RIGHT);
	}
	else
	{
		// ★二回押し
		pPerson->ActionDoublePush(PLAYER_INPUT::LEFT);
	}

	pPerson->AddWaitFrame(1);
	if (pPerson->GetWaitFrame() >= 20)
	{
		// 待機へ
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// バックステップに成功したら終り
	if (MyPlayer->GetFSM()->isPrevState(*BasePlayerState::BackStep::GetInstance()) == true)
	{
		// 待機へ
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::BackStep::Exit(AI * pPerson)
{

}

void AIState::BackStep::Render(AI * pPerson)
{
#ifdef _DEBUG

	tdnText::Draw(420, 610, 0xffffffff, "BackStep");

#endif // _DEBUG
}

bool AIState::BackStep::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}


/*******************************************************/
//			パートナー発動攻撃 ステート
/*******************************************************/

AIState::PartnerAttack * AIState::PartnerAttack::GetInstance()
{
	// ここに変数を作る
	static AIState::PartnerAttack  instance;
	return &instance;
}

void AIState::PartnerAttack::Enter(AI * pPerson)
{
	// ★ゲージがまだ溜まっていなかったら
	if (MyPlayer->GetStand()->GetStandStock() == 0)
	{
		// 待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

	// ここで現状の相手と自分のポジションで出す技を決める
	// 現在存在する種類は　[近距離]　[中距離]　[遠距離]　[近距離空中]

	// キャラクターのX軸での距離を取得
	float l_fRange = Math::Length(TargetPlayer->GetPos().x, MyPlayer->GetPos().x);
	enum
	{
		NEAR_RANGE = 25,
		MIDDLE_RANGE = 40,
		//FAR_RANGE = 80,
	};

	// 場所に伴ったコマンドを入力
	bool l_bActionFlag = false;
	if (l_fRange <= NEAR_RANGE)
	{
		// 近距離
		switch (MyPlayer->GetStand()->GetPartnerType())
		{
		case PARTNER::MOKOI:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		case PARTNER::MAYA:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		default:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		}

	}
	else if (l_fRange <= MIDDLE_RANGE)
	{
		// 中距離
		switch (MyPlayer->GetStand()->GetPartnerType())
		{
		case PARTNER::MOKOI:
			break;
		case PARTNER::MAYA:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		case PARTNER::END:
			break;
		default:
			break;
		}

	}
	else
	{
		// 遠距離
		switch (MyPlayer->GetStand()->GetPartnerType())
		{
		case PARTNER::MOKOI:
			break;
		case PARTNER::MAYA:
			pPerson->PushInputList(PLAYER_INPUT::L1);
			l_bActionFlag = true;
			break;
		case PARTNER::END:
			break;
		default:
			break;
		}
	}


	// 発動できる技がなかった
	if (l_bActionFlag == false)
	{
		// もし当てはまった環境の技が設定されていなかった場合待機に戻る
		//待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}


}

void AIState::PartnerAttack::Execute(AI * pPerson)
{
	// 待機に戻ってたら
	if (MyPlayer->GetFSM()->isInState(*BasePlayerState::Wait::GetInstance()) == true)
	{
		// 待機に戻る
		pPerson->GetFSM()->ChangeState(AIState::Wait::GetInstance());
		return;
	}

}

void AIState::PartnerAttack::Exit(AI * pPerson)
{

}

void AIState::PartnerAttack::Render(AI * pPerson)
{
	tdnText::Draw(420, 610, 0xffffffff, "PartnerAttack");
}

bool AIState::PartnerAttack::OnMessage(AI * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//}
	return false;
}
