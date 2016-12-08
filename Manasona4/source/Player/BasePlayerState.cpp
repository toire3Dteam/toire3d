#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "PlayerManager.h"
#include "../Cutin/CutIn.h"
#include "../Camera/camera.h"

// 定数
static const float HUTTOBI_LINE = 3.0f;
static const float REPEAT_ATTACK_RATE = 0.85f;		// 同じ攻撃当たるなレート

#define GUARD_ON	// ガードあり

/*******************************************************/
//	ファンクション
/*******************************************************/
bool isNeutralPOV(BasePlayer *pPerson)
{
	return (!pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT) && !pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) && !pPerson->isPushInput(PLAYER_COMMAND_BIT::UP) && !pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN));
}

void ChangeWaitState(BasePlayer *pPerson)
{
	// 下キーを押しているなら、待機ステートではなく、しゃがみステートに行く
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());

	// それ以外なら待機ステートに戻る
	else pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
}

bool JumpCancel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプボタンでジャンプキャンセル
	//============================================
	// 地面についていたら
	if (pPerson->isLand() == true)
	{
		// 押しっぱなしでキャンセル
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::UP))
		{
			// ジャンプステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
			return true;
		}
	}
	else if (pPerson->isAerialJump() == true)	// 空中で空中ジャンプの権利があれば
	{
		// ★ジャンプ時のみトリガー
		if (
			//pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3 ||	// ボタンでジャンプを封印
			pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::UP, true))
		{
			// 先行入力リセット
			pPerson->AheadCommandReset();

			// 空中ジャンプステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			return true;
		}
	}

	return false;
}

bool isDamageCounterState(BasePlayer *pPerson)
{
	return (
		pPerson->GetFSM()->isInState(*BasePlayerState::RushAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::Escape::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::DokkoiAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::DownAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::AerialAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::Skill::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::AntiAirAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::FinishAttack::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::SquatAttack::GetInstance())
		);
}

bool isDamageCounterStatePrev(BasePlayer *pPerson)
{
	return (
		pPerson->GetFSM()->isPrevState(*BasePlayerState::RushAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::Escape::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::DokkoiAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::DownAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::AerialAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::Skill::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::AntiAirAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::FinishAttack::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::HeavehoDrive::GetInstance()) ||
		pPerson->GetFSM()->isPrevState(*BasePlayerState::SquatAttack::GetInstance())
		);
}

bool isThrownState(BasePlayer *pPerson)
{
	return (
		pPerson->isLand() &&	// 地上にいて

		// ダメージ系のステート以外だったら
		!pPerson->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) &&
		!pPerson->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) &&
		!pPerson->GetFSM()->isInState(*BasePlayerState::LandRecovery::GetInstance())
		);
}

bool RunCancel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	左右キーで走りキャンセル
	//============================================
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT, true))
	{
		//pPerson->SetDir(DIR::LEFT);
		if (pPerson->GetDir() == DIR::LEFT)
		{
			// 先行入力リセット
			pPerson->AheadCommandReset();

			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
			return true;
		}
	}
	else if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::RIGHT, true))
	{
		//pPerson->SetDir(DIR::RIGHT);
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			// 先行入力リセット
			pPerson->AheadCommandReset();

			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
			return true;
		}
	}
	return false;
}

bool SquatCancel(BasePlayer * pPerson)
{
	// しゃがみキャンセル
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		return true;
	}
	else return false;
}

bool RushAttackCancel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (isNeutralPOV(pPerson) && pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && !pPerson->GetFSM()->isPrevState(*BasePlayerState::RushAttack::GetInstance()))
	{
		// 先行入力リセット
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
		return true;
	}
	else return false;
}

bool SquatAttackCancel(BasePlayer * pPerson)
{
	// しゃがみ攻撃キャンセル
	if ((pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true)) && pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && !pPerson->GetFSM()->isPrevState(*BasePlayerState::SquatAttack::GetInstance()))
	{
		// 先行入力リセット
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
		return true;
	}
	else return false;
}

bool AntiAirCancel(BasePlayer *pPerson)
{
	// 対空攻撃キャンセル
	if ((pPerson->isPushInput((pPerson->GetDir() == DIR::LEFT) ? PLAYER_COMMAND_BIT::RIGHT : PLAYER_COMMAND_BIT::LEFT, true)) && pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true) && !pPerson->GetFSM()->isPrevState(*BasePlayerState::AntiAirAttack::GetInstance()))
	{
		// 先行入力リセット
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::AntiAirAttack::GetInstance());
		return true;
	}
	else return false;
}

bool StandCancel(BasePlayer * pPerson)
{
	// スタンドキャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::L1, true))
	{
		// 追記　スタンドストックがないときはキャンセルできない
		if (pPerson->GetStand()->GetStandStock() <= 0)return false;

		// 先行入力リセット
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::StandAction::GetInstance());
		return true;
	}
	else return false;
}

bool AttackCancel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
	{
		if (pPerson->isLand() == true)
		{
			// 4入力
			if (pPerson->isPushInput((pPerson->GetDir() == DIR::LEFT) ? PLAYER_COMMAND_BIT::RIGHT : PLAYER_COMMAND_BIT::LEFT, true))
			{
				// 対空攻撃
				pPerson->GetFSM()->ChangeState(BasePlayerState::AntiAirAttack::GetInstance());
			}
			// 2入力
			else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true))
			{
				// しゃがみ下段
				pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
			}
			else
			{
				// 通常
				pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
			}
		}
		else 
		{
			// 空中
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		// 先行入力リセット
		pPerson->AheadCommandReset();

		return true;
	}
	else return false;
}

bool InvincibleAttackCancel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::L2, true))
	{
		// 先行入力リセット
		pPerson->AheadCommandReset();

		pPerson->GetFSM()->ChangeState(BasePlayerState::FinishAttack::GetInstance());
		return true;
	}
	else return false;
}

bool SkillCancel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::D, true))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Skill::GetInstance());
		return true;
	}
	else return false;
}

bool OverDriveCancel(BasePlayer *pPerson)
{
	if ((pPerson->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()) == true && pPerson->GetActionFrame() != FRAME_STATE::FOLLOW) ||
		pPerson->GetFSM()->isInState(*BasePlayerState::ThrowSuccess::GetInstance()) == true ||
		pPerson->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true ||// 投げてるときは勘弁
		pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) == true)
		return false;

	// [1205] 掴まれたり・必殺技を受けてるときは反応しない
	if (pPerson->isNotOverDrive() == true)return false;


	// 覚醒（バースト）キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R3, true))
	{
		// 先行入力リセット
		pPerson->AheadCommandReset();

		// ゲージが溜まってたら
		if (pPerson->GetOverDriveGage() == BasePlayer::c_OVERDRIVE_MAX_GAGE)
		{
			// 攻めの1Moreか守りのバーストかを判断
			if (pPerson->GetRecoveryFrame() <= 0)
			{
				// 攻めの覚醒
				pPerson->GetFSM()->ChangeState(BasePlayerState::OverDrive_OneMore::GetInstance());
				return true;
			}
			else
			{
				// 守りのバースト
				pPerson->GetFSM()->ChangeState(BasePlayerState::OverDrive_Burst::GetInstance());
				return true;
			}

		}
		else return false;
	}
	else return false;
}

bool EscapeCancel(BasePlayer *pPerson)
{
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::B, true))
	{
		// 先行入力リセット
		pPerson->AheadCommandReset();

		// エスケープステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return true;
	}
	else return false;

	//if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	//pPerson->SetDir(DIR::LEFT);// 向きを変える
	//	// エスケープステートに行く
	//	//if (pPerson->GetDir() == DIR::LEFT)
	//	{
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
	//		return true;
	//	}
	//}
	//else if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	//pPerson->SetDir(DIR::RIGHT);// 向きを変える
	//	// エスケープステートに行く
	//	//if (pPerson->GetDir() == DIR::RIGHT)
	//	{
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
	//		return true;
	//	}
	//}
	//return false;
}

bool ThrowCancel(BasePlayer * pPerson)
{
	// 投げキャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::A))
	{
		// 地面についていないとないときはキャンセルできない
		if (pPerson->isLand() == false)return false;
		pPerson->GetFSM()->ChangeState(BasePlayerState::Throw::GetInstance());
		return true;
	}
	else return false;
}

//bool GuardCancel(BasePlayer * pPerson)
//{
//	// ガードキャンセル
//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::R1))
//	{
//		// 地面についていないとないときはキャンセルできない
//		if (pPerson->isLand() == false)return false;
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());
//		return true;
//	}
//	else return false;
//}

bool HeaveHoCancel(BasePlayer * pPerson)
{
	// 必殺技キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R2))
	{	
		// オーバードライブ中ならゲージなしで打てる
		if (pPerson->isOverDrive())
		{
			// オーバードライブを終了して発動
			pPerson->SetOverDriveFrame(0);

		}else
		{
			// まずゲージが50％あるかないか
			if (pPerson->GetOverDriveGage() < 50)return false;

			// 50%消費
			pPerson->AddOverDriveGage(-50);
		}

		// 必殺ステートへ
		pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDrive::GetInstance());
		return true;
	}
	else return false;
}

bool HeaveHoOverFlowCancel(BasePlayer * pPerson)
{
	if (pPerson->isOverDrive() == false)return false;

	// 超必殺技キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R3))
	{
		if ((pPerson->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()) == true && pPerson->GetActionFrame() != FRAME_STATE::FOLLOW) ||
			pPerson->GetFSM()->isInState(*BasePlayerState::ThrowSuccess::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::ThrowBind::GetInstance()) == true || 
			pPerson->GetFSM()->isInState(*BasePlayerState::KnockBack::GetInstance()) == true || 
			pPerson->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_OneMore::GetInstance()) == true ||
			pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDriveOverFlow::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::HeavehoDrive::GetInstance()) == true||
			pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive_Burst::GetInstance()) == true// バースト中も勘弁
			)// 投げてるときは勘弁
			return false;

		pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDriveOverFlow::GetInstance());
		return true;
	}
	else return false;
}

bool DashCancel(BasePlayer * pPerson)
{
	// ダッシュキャンセル
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::RIGHT, 12))
	{
		// 同じ方向なら走る
		if (pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		
		// 逆方向ならバクステ
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());

		return true;
	}
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::LEFT, 12))
	{
		// 同じ方向なら走る
		if (pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());

		// 逆方向ならバクステ
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());

		return true;
	}
	
	return false;
}

bool AerialDashCancel(BasePlayer *pPerson)
{
	// 空中ダッシュキャンセル
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::RIGHT, 12))
	{
		// 同じ方向なら走る
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDash::GetInstance());
		}
		// 逆方向ならバクステ
		else pPerson->GetFSM()->ChangeState(BasePlayerState::AerialBackDash::GetInstance());

		return true;
	}
	if (pPerson->isDoublePush(PLAYER_COMMAND_BIT::LEFT, 12))
	{
		// 同じ方向なら走る
		if (pPerson->GetDir() == DIR::LEFT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDash::GetInstance());
		}

		// 逆方向ならバクステ
		else pPerson->GetFSM()->ChangeState(BasePlayerState::AerialBackDash::GetInstance());

		return true;
	}

	return false;
}

bool AerialAttackCancel(BasePlayer *pPerson)
{
	// 攻撃ボタン入力で空中攻撃
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C))
	{
		// 空下封印
		//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN) == true)
		//{
		//	// 空中下攻撃ステートに行く
		//	pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
		//}
		//else
		{
			// 空中攻撃ステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		return true;
	}

	return false;
}

bool DokkoiAttackCancel(BasePlayer *pPerson)
{
	// どっこい攻撃キャンセル
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R1, true))
	{
		if (pPerson->isLand() == true)
		{
			if (/*pPerson->isSquat() == false*/
				pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true))
			{
				// しゃがみ
				pPerson->GetFSM()->ChangeState(BasePlayerState::DownAttack::GetInstance());
			}
			else
			{
				// 通常
				pPerson->GetFSM()->ChangeState(BasePlayerState::DokkoiAttack::GetInstance());
			}

		}
		else
		{
			// 空中
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		// 先行入力リセット
		pPerson->AheadCommandReset();

		return true;
	}
	else return false;

}

bool isInputGuardCommand(BasePlayer *pPerson)
{
	bool bGuardCommand(false);

	// 相手に対する向き(例えば右向きに走ってても左に相手がいるならそれは左向き)
	if (pPerson->GetTargetDir() == DIR::LEFT)
	{
		// 相手に対して反対の方を入力してたら
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT)) bGuardCommand = true;
	}
	// 反対
	else
	{
		// 相手に対して反対の方を入力してたら
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT)) bGuardCommand = true;
	}

	return bGuardCommand;
}

void GuardUpdate(BasePlayer *pPerson)
{
#ifdef GUARD_ON
	// ガードコマンドを入力してたら
	if (isInputGuardCommand(pPerson))
	{
		// ★相手が何かしらの攻撃をしていたら、ガードステートに移行
		if ((pPerson->GetTargetPlayer()->isAttackState() || (pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData())) &&															// 相手が攻撃状態
			Math::Length(pPerson->GetPos(),pPerson->GetTargetPlayer()->GetPos()) < BasePlayer::c_GUARD_DISTANCE)	// ガード発動距離
			pPerson->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());

		//// しゃがみガード
		//if (pPerson->GetFSM()->isInState(*BasePlayerState::Squat::GetInstance()))
		//{
		//	// ガードステートを設定する
		//	pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
		//
		//	// 相手が何かしらの攻撃をしていたら、ガードモーションをとる
		//	if (pPerson->GetTargetPlayer()->isAttackState())
		//	{
		//		// TODO:モーション戻す時どうする
		//		pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);
		//	}
		//}
		//
		//// 立ちガード
		//else
		//{
		//	// ガードステートを設定する
		//	pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
		//
		//	// 相手が何かしらの攻撃をしていたら、ガードモーションをとる
		//	if (pPerson->GetTargetPlayer()->isAttackState())
		//	{
		//		// TODO:モーション戻す時どうする
		//		pPerson->SetMotion(MOTION_TYPE::UP_GUARD);
		//	}
		//}
	}

	// ガードコマンドを入力していないので、ガードしてないステートを設定
	else
#endif
	{
		pPerson->SetGuardState(GUARD_STATE::NO_GUARD);
	}

}

bool isPossibleGuardState(BasePlayer *pPerson)
{
	// ガードができるステートかどうか
	StateMachine<BasePlayer> *pState(pPerson->GetFSM());
	bool bGuardState(pState->isInState(*BasePlayerState::Wait::GetInstance()) ||
		pState->isInState(*BasePlayerState::BackWalk::GetInstance()) ||
		pState->isInState(*BasePlayerState::Run::GetInstance()) ||
		//pState->isInState(*BasePlayerState::AerialJump::GetInstance()) ||
		//pState->isInState(*BasePlayerState::Fall::GetInstance()) ||
		pState->isInState(*BasePlayerState::Squat::GetInstance()));
	
	//if (!bGuardState)
	//{
	//	if (pState->isInState(*BasePlayerState::Jump::GetInstance()))
	//	{
	//		// ★ジャンプの瞬間のガードできないフレーム
	//		if (!pPerson->GetJump()->bHold) bGuardState = true;
	//	}
	//}

	return bGuardState;
}

bool WalkCancel(BasePlayer *pPerson)
{
	//////////////////////////////////////////////
	//	移動ボタン
	//============================================
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		//pPerson->SetDir(DIR::RIGHT);
		// 歩きステートにいく
		if (pPerson->GetDir() == DIR::RIGHT)pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		return true;
	}
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		//pPerson->SetDir(DIR::LEFT);
		// 歩きステートにいく
		if (pPerson->GetDir() == DIR::LEFT)pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
		else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		return true;
	}
	return false;
}

SKILL_ACTION_TYPE GetSkillActionType(BasePlayer *pPerson)
{
	if (!pPerson->isLand())
		return (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true)) ? SKILL_ACTION_TYPE::AERIALDROP : SKILL_ACTION_TYPE::AERIAL;
	else
		return (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN, true)) ? SKILL_ACTION_TYPE::SQUAT : SKILL_ACTION_TYPE::LAND;
}


/*******************************************************/
//					グローバルステート
/*******************************************************/

void BasePlayerState::Global::Enter(BasePlayer * pPerson)
{
}

void BasePlayerState::Global::Execute(BasePlayer * pPerson)
{
	////////////////////////////////////////////////////////////////
	//	ガード更新処理
	//==============================================================
	if (isPossibleGuardState(pPerson)) GuardUpdate(pPerson);

	////////////////////////////////////////////////////////////////
	//	オーバードライブ時のみできる超必殺技　
	//==============================================================
	if (HeaveHoOverFlowCancel(pPerson)) return;

	////////////////////////////////////////////////////////////////
	//	オーバードライブ　どこからでも出せる最強のキャンセルルート
	//==============================================================
	if (OverDriveCancel(pPerson)) return;
	


}

void BasePlayerState::Global::Exit(BasePlayer * pPerson)
{
}

void BasePlayerState::Global::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 610, 0xffffffff, "ぐろーばるすてーと");
}

bool BasePlayerState::Global::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 掴まれたよメッセージ
	case MESSAGE_TYPE::BE_THROWN:
	{
									pPerson->SetDir((pPerson->GetTargetPlayer()->GetDir() == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT);	// 要は掴んだ相手の方を向く
									pPerson->GetFSM()->ChangeState(ThrowBind::GetInstance());		// 投げくらいステートへ
									break;
	}
		// 攻撃くらったよメッセージ
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
									 //// もしバースト中ならはじく
									 //if (pPerson->GetFSM()->isInState(*BasePlayerState::OverDrive::GetInstance()) == true)
									 //{
									 //	 // アクションフレームが続いてたら
									 //	 if (pPerson->isFrameAction())
									 //	 {			 
									 //		 return true;
									 //	 }
									 //}

									 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る

									 // ★ダメージを与えた人に送り返す(ガードしても必殺が発動するバグを直すために、このHandleMessageを受け取ったら発動という風にする)
									 HIT_ATTACK_INFO HitAttackInfo;
									 HitAttackInfo.bOverDrive = HitDamageInfo->bOverDrive;
									 HitAttackInfo.HitScore = HitDamageInfo->damage;
									 HitAttackInfo.hitStopFlame = HitDamageInfo->hitStopFlame;
									 MsgMgr->Dispatch(0, msg.Receiver, msg.Sender, MESSAGE_TYPE::HIT_ATTACK, &HitAttackInfo);
									 //bool bGuardSuccess(false);
									 //if (pPerson->GetGuardState() == GUARD_STATE::UP_GUARD)
									 //{
										// // ★相手が立ちガードしてて、下段攻撃じゃなかったらガード成功！
										// if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::DOWN_ATTACK) bGuardSuccess = true;
									 //}
									 //else if (pPerson->GetGuardState() == GUARD_STATE::DOWN_GUARD)
									 //{
										// // ★相手がしゃがみガードしてて、上段攻撃じゃなかったらガード成功！
										// if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::UP_ATTACK) bGuardSuccess = true;
									 //}

									 //// ★ガード成功してたら！！！
									 //if (bGuardSuccess)
									 //{
										// // (A列車)ここにガード成功時のSE、エフェクト
										// se->Play("ガード");

										// // 懇親の
										// if (pPerson->GetDir() == DIR::RIGHT)
										// {
										//	 pPerson->SetMove(Vector3(-0.5f, 0, 0));
										// }
										// else
										// {
										//	 pPerson->SetMove(Vector3(0.5f, 0, 0));
										// }

										// pPerson->GetComboUI()->Guard();// ガードされた

										// // ガードステートに行く

										// // 以降のダメージ処理を省く
										// return true;
									 //}
									 
									 // ダメージSEの再生
									 LPCSTR seID = HitDamageInfo->HitSE;
									 if (seID) se->Play((LPSTR)seID);

									 // 数字エフェクト追加
									// Vector2 screenPos = Math::WorldToScreen(pPerson->GetPos());
									// NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, HitDamageInfo->damage, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

									 // 空中ダッシュ消す
									 pPerson->SetAerialDashFrame(0);

									 // ペルソナブレイク
									 pPerson->GetStand()->Break();

									 // アクションステートオフ(つまり現在までやったいた動きが消える)
									 pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

									 // 向きを攻撃された方向へ向くように
									 if (HitDamageInfo->FlyVector.x > 0.0f)	pPerson->SetDir(DIR::LEFT);
									 else		pPerson->SetDir(DIR::RIGHT);

									 // 吹っ飛びベクトルを加算 (A列車)加算から代入に変更
									 //pPerson->AddMove(Vector3(HitDamageInfo->LandFlyVector.x, HitDamageInfo->LandFlyVector.y, 0));	
									 pPerson->SetMove(Vector3(HitDamageInfo->FlyVector.x, HitDamageInfo->FlyVector.y, 0));

									 // ヒットストップ
									 pPerson->SetHitStopFrame(HitDamageInfo->hitStopFlame);

									 // 硬直フレーム設定
									 int RecoveryFrame(HitDamageInfo->HitRecoveryFrame);

									 // ダメージ
									 int damage((int)(HitDamageInfo->damage * pPerson->GetDamageRate()));							// ★自分のダメージレートにかけ合わせる

									 // 根性値が発動してるなら
									 if (pPerson->isWillPower())
									 {
										 // 防御力を上げる
										 damage = (int)((float)damage* 0.7f);
									 }
									 

									 // ★必殺技中は同技補正を無視(アイルーのやつとかひどいことになるから)
									 if (!HitDamageInfo->bOverDrive)
									 {
										 for (auto it : *pPerson->GetRecoveryDamageCount())
										 {
											 if (it == (BASE_ACTION_STATE)HitDamageInfo->iAttackType)
											 {
												 RecoveryFrame = (int)(RecoveryFrame * REPEAT_ATTACK_RATE);	// 同技補正
											 }
										 }
									 }
									 RecoveryFrame = max(RecoveryFrame, 1);		// 絶対1以上！
									 pPerson->SetRecoveryFrame(RecoveryFrame);

									 // ★コンボUI エフェクト(カウント)発動
									 COMBO_DESK comboDesk;

									 // ★初段のみ入るところ
									 if (pPerson->GetRecoveryDamageCount()->empty())
									 {
										 if (isDamageCounterState(pPerson)) comboDesk.bCounter = true;
											 damage += BasePlayer::c_FIRST_HIT_ADD_DAMAGE;	// 初段だけダメージを増やす
									 }

									 comboDesk.side = pPerson->GetSide();
									 comboDesk.damage = damage;
									 comboDesk.recoveryFrame = RecoveryFrame;
									 MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);

									 // くらった攻撃リスト追加
									 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);

									 // (追加)ヒットエフェクト発動! 
									 // (TODO)エフェクトを今は自分のポジションにおいてる！！！　後でヒットポジションとかいるかも
									 pPerson->AddEffectAction(pPerson->GetCenterPos() + Vector3(0, 4, 0), (EFFECT_TYPE)HitDamageInfo->HitEffectType);

									 // 空中ジャンプの権利復活
									 pPerson->SetAerialJump(true);

									 // 攻撃に相手を無敵にさせる属性が入ってたら
									 if (HitDamageInfo->BeInvincible)
									 {
										 //むてきつくった
										 // ★やっぱりここで無敵時間設定
										 pPerson->SetInvincible(90, 1);
									 }

									 // ダメージ処理
									 pPerson->MultDamageRate(HitDamageInfo->fComboRate);						// ダメージレートを受けた技のレートで乗算する(だんだんダメージが減っていく)

									 const int DamagedHP(max(pPerson->GetHP() - damage, 0));
									 // ダメージを受けた後のHPを設定
									 pPerson->SetHP(DamagedHP);
									 if (DamagedHP == 0)
									 {
										 // フィニッシュフラグのある攻撃なら(必殺の途中で死なない為のフラグ)
										 if (HitDamageInfo->bFinishOK)
										 {
											 // ★KOのメッセージを送る
											 KO_INFO KOInfo;
											 KOInfo.FinishType = HitDamageInfo->bOverDrive ? FINISH_TYPE::OVER_DRIVE : FINISH_TYPE::NORMAL;
											 KOInfo.iNumWinnerRound = pPerson->GetTargetPlayer()->GetWinNum();	// 倒したやつの勝った数;
											 KOInfo.WinnerSide = pPerson->GetTargetPlayer()->GetSide();
											 KOInfo.LoseSide = pPerson->GetSide();
											 // 
											 MsgMgr->Dispatch(0, pPerson->GetTargetPlayer()->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::KO, &KOInfo);

											 // 死に
											 pPerson->GetFSM()->ChangeState(BasePlayerState::Die::GetInstance());
										 }
									 }

									 else
									 {

										 // 受けたダメージでもSPゲージ少し上昇
										 if (damage <= 50)
										 {
											// なし
										 }
										 else if (damage <= 200)
										 {
											 pPerson->AddOverDriveGage(0.5f);
										 }
										 else if (damage <= 500)
										 {
											 pPerson->AddOverDriveGage(1);

										 }
										 else if (damage <= 900)
										 {
											 pPerson->AddOverDriveGage(1.5f);

										 }
										 else 
										 {
											 pPerson->AddOverDriveGage(2.5f);
										 }

										 // 吹っ飛び距離	に　応じて	ダメージステートを変える
										 //const float len = HitDamageInfo->FlyVector.Length();
										 //if (len < HUTTOBI_LINE)
										 //{
										//	 // ここで空中でくらってるか地上でくらってるかを分ける
										//	 //if (pPerson->isLand() == true)
										//	 {
										//		 // 地上ノックバックに行く
										//		 pPerson->GetFSM()->ChangeState(BasePlayerState::KnockBack::GetInstance());
										//	 }
										//	 //else
										//	 //{
										//	//	 // 空中ノックバックに行く
										//	//	 pPerson->GetFSM()->ChangeState(BasePlayerState::AerialKnockBack::GetInstance());
										//	 //}
										 //
										 //}
										 //else
										 //{
										//	 // ノックダウンに行く
										//	 pPerson->GetFSM()->ChangeState(BasePlayerState::KnockDown::GetInstance());
										 //
										 //}
										 switch (HitDamageInfo->DamageMotion)
										 {
										 case DAMAGE_MOTION::KNOCK_BACK:
											 pPerson->GetFSM()->ChangeState(KnockBack::GetInstance());
											 break;
										 case DAMAGE_MOTION::KNOCK_DOWN:
											 pPerson->GetFSM()->ChangeState(KnockDown::GetInstance());
											 break;
										 case DAMAGE_MOTION::DOWN_FALL:
											 pPerson->GetFSM()->ChangeState(DownFall::GetInstance());
											 break;
										 }
									 }
									 return true;
									 break;
	}

		// 攻撃当たったよメッセージ
	case MESSAGE_TYPE::HIT_ATTACK:
	{
									 if (pPerson->isAttackState())
									 {
										 pPerson->GetAttackData()->bHitSuccess = true;	// マジで当たったガーーーーール！！！！
#ifdef _DEBUG
										 // コンボが当たった時間
										 Vector2 screenPos = Math::WorldToScreen(pPerson->GetPos());
										 NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pPerson->GetCurrentFrame(), Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);
#endif
									 }

									 HIT_ATTACK_INFO *HitAttackInfo = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
									 //pPerson->SetHitStopFrame(HitAttackInfo->hitStopFlame);// ヒットストップ
									 pPerson->AddCollectScore(HitAttackInfo->HitScore);	// 実体前のスコアを加算
									 if (HitAttackInfo->bOverDrive) pPerson->ConversionScore();	// フィニッシュアタックならスコア生産
									 // 必殺以外
									 else
									 {
										 // 攻撃を当ててSPゲージ上昇だ！
										 if (HitAttackInfo->HitScore <= 200)
										 {
											 pPerson->AddOverDriveGage(2);
										 }
										 else if (HitAttackInfo->HitScore <= 500)
										 {
											 pPerson->AddOverDriveGage(3);

										 }
										 else if (HitAttackInfo->HitScore <= 900)
										 {
											 pPerson->AddOverDriveGage(5);

										 }
										 else if (HitAttackInfo->HitScore <= 1200)
										 {
											 pPerson->AddOverDriveGage(6);
										 }
										 else
										 {
											 pPerson->AddOverDriveGage(8);
										 }
									 }


									 //// この攻撃で相手が相手が死んでたら
									 //if (pPerson->GetTargetPlayer()->GetHP() == 0)
									 //{
									//	 // KOのメッセージを送る
									//	 FINISH_TYPE FinishType(HitAttackInfo->bOverDrive ? FINISH_TYPE::OVER_DRIVE : FINISH_TYPE::NORMAL);
									 //
									//	 // 
									//	 MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::KO, &FinishType);
									 //}
									 break;
	}

	case HEAVE_HO_OVERFLOW_START:
	{
									// メッセージが届いたらオーバーフローへ移動
									pPerson->GetFSM()->ChangeState(BasePlayerState::HeavehoDriveOverFlow_Success::GetInstance());
									return true;
	}break;

	case MESSAGE_TYPE::END_FINISHCALL:	// フィニッシュのコールが終わったら、勝ちモーションに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Win::GetInstance());
		break;

	// (A列車)死んだぜメッセージ
	}

	return false;
}


/*******************************************************/
//					待機ステート
/*******************************************************/

void BasePlayerState::Wait::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(MOTION_TYPE::WAIT);

	//	アングル補間
	pPerson->SetDirAngle();

	// ダウンフラグを解除
	pPerson->SetDown(false);

}

void BasePlayerState::Wait::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃ボタン
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	どっこい攻撃キャンセル
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	投げキャンセル
	//============================================
	if (ThrowCancel(pPerson)) return;
	
	//////////////////////////////////////////////
	//	ガードキャンセル
	//============================================
	//if (GuardCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ダッシュキャンセル
	//============================================
	if (DashCancel(pPerson))return;
	
	//////////////////////////////////////////////
	//	移動キャンセル
	//============================================
	if (WalkCancel(pPerson))return;

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
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 落下してるよメッセージ
	case MESSAGE_TYPE::FALL:
	{
		// 落下ステートに行く！
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		break;
	}
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}

/*******************************************************/
//					歩くステート
/*******************************************************/

void BasePlayerState::Walk::Enter(BasePlayer * pPerson)
{
	// 歩きモーションに変える
	pPerson->SetMotion(MOTION_TYPE::WALK);

	// 初速度の設定
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
		MyAssert(0, "ふぁっきゅー");
		break;
	}

	pPerson->SetMove(move);
}

void BasePlayerState::Walk::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	どっこい攻撃キャンセル
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	投げキャンセル
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ガードキャンセル
	//============================================
	//if (GuardCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCancel(pPerson)) return;

	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::RIGHT) == 3)
	//{
	//	// 同じ方向なら走る
	//	if (pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	//	//pPerson->SetDir(DIR::RIGHT);
	//	return;
	//}

	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::LEFT) == 3)
	//{
	//	// 同じ方向なら走る
	//	if (pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
	//	//pPerson->SetDir(DIR::LEFT);
	//	return;
	//}

	//////////////////////////////////////////////
	//	ダッシュキャンセル
	//============================================
	if (DashCancel(pPerson))return;


	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::RIGHT);
		// 途中で相手が反対向きなら後ろ歩き
		if (pPerson->GetTargetDir() == DIR::LEFT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
			return;
		}
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::LEFT);
		// 途中で相手が反対向きなら後ろ歩き
		if (pPerson->GetTargetDir() == DIR::RIGHT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
			return;
		}
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

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * .5f);
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
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 落下してるよメッセージ
	case MESSAGE_TYPE::FALL:
	{
		// 落下ステートに行く！
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		break;
	}
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					後ろ歩きステート
/*******************************************************/

void BasePlayerState::BackWalk::Enter(BasePlayer * pPerson)
{
	// 歩きモーションに変える
	pPerson->SetMotion(MOTION_TYPE::BACK_WALK);

	// 初速度の設定
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x += .75f;
		break;
	case DIR::RIGHT:
		move.x -= .75f;
		break;
	default:
		MyAssert(0, "ふぁっきゅー");
		break;
	}

	pPerson->SetMove(move);
}

void BasePlayerState::BackWalk::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	どっこい攻撃キャンセル
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	投げキャンセル
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ガードキャンセル
	//============================================
	//if (GuardCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCancel(pPerson)) return;

	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::RIGHT) == 3)
	//{
	//	// 同じ方向ならその向きに歩く
	//	if(pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());
	//
	//	//pPerson->SetDir(DIR::RIGHT);
	//	return;
	//}
	//
	//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::LEFT) == 3)
	//{
	//	// 同じ方向ならその向きに歩く
	//	if(pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	else pPerson->GetFSM()->ChangeState(BasePlayerState::BackStep::GetInstance());
	//
	//	//pPerson->SetDir(DIR::LEFT);
	//	return;
	//}

	//////////////////////////////////////////////
	//	ダッシュキャンセル
	//============================================
	if (DashCancel(pPerson))return;

	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::RIGHT);
		if (pPerson->GetTargetDir() == DIR::RIGHT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			return;
		}
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::LEFT);
		if (pPerson->GetTargetDir() == DIR::LEFT)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			return;
		}
	}
	else // ニュートラル
	{
		//Vector3 move = pPerson->GetMove();
		//
		//// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
		//if (
		//	//move.Length() < .1f
		//	abs(move.x) < BasePlayer::c_END_MOVE_LINE
		//	)
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}
	}

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * .35f);
}

void BasePlayerState::BackWalk::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::BackWalk::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "BackWalkState");
}

bool BasePlayerState::BackWalk::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 落下してるよメッセージ
	case MESSAGE_TYPE::FALL:
	{
							   // 落下ステートに行く！
							   pPerson->GetFSM()->ChangeState(Fall::GetInstance());
							   break;
	}
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					走るステート
/*******************************************************/

void BasePlayerState::Run::Enter(BasePlayer * pPerson)
{
	// 走るモーションに変える
	pPerson->SetMotion(MOTION_TYPE::RUN);

	// 走るエフェクト発動！
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::RUN);

	pPerson->SetDirAngle();

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

	// SE再生
	se->Play("ダッシュ");

	pPerson->SetDushFrame(0);
}

void BasePlayerState::Run::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (EscapeCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	どっこい攻撃キャンセル
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	投げキャンセル
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ガードキャンセル
	//============================================
	//if (GuardCancel(pPerson)) return;

	pPerson->AddDushFrame(1);

	//////////////////////////////////////////////
	//	左
	//============================================
	// 押した瞬間
	//if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	// 相手に対して反対方向
	//	if (pPerson->GetTargetDir() == DIR::RIGHT)
	//	{
	//		// 後ろ歩きステートへ
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
	//	}
	//	else
	//	{
	//		// 前歩きステートへ
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	}
	//}
	// 押してる間
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT) && pPerson->GetDir() == DIR::LEFT)
	{
		pPerson->AddMove(Vector3(-0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::LEFT);
	}

	//////////////////////////////////////////////
	//	右
	//============================================
	// 押した瞬間 理論上　★二回連続押すことはありえない　ニュートラルを挟む
	//else if (pPerson->GetInputList(PLAYER_COMMAND_BIT::RIGHT) == 3)
	//{
	//	// 相手に対して反対方向
	//	if (pPerson->GetTargetDir() == DIR::LEFT)
	//	{
	//		// 後ろ歩きステートへ
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
	//	}
	//	else
	//	{
	//		// 前歩きステートへ
	//		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	//	}
	//}
	// 押してる間
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) && pPerson->GetDir() == DIR::RIGHT)
	{
		pPerson->AddMove(Vector3(0.1f, 0.0f, 0.0f));
		//pPerson->SetDir(DIR::RIGHT);
	}


	else // ニュートラル
	{
		//Vector3 move = pPerson->GetMove();
		// ある程度減速したら、前ブレーキステートに移行(減速自体はBaseのアップデートでやってる)
		//if (abs(move.x) < BasePlayer::c_FRONT_BRAKE_LINE)
		if(pPerson->GetDushFrame() >= 12)
		{
			//// 前のステートがUターンブレーキだったら、「待機」ステートに戻る(ターンブレーキの後にフロントブレーキが発生してしまうから)
			//if (pPerson->GetFSM()->isPrevState(*BasePlayerState::TurnOverBrake::GetInstance()))
			//	pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			//else
			{
				// ★　フロントブレーキ封印して待機にした
				//pPerson->GetFSM()->ChangeState(BasePlayerState::FrontBrake::GetInstance());
				pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
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
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 落下してるよメッセージ
	case MESSAGE_TYPE::FALL:
	{
		// 落下ステートに行く！
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		break;
	}
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					バクステステート
/*******************************************************/

void BasePlayerState::BackStep::Enter(BasePlayer * pPerson)
{
	// 走るモーションに変える
	pPerson->SetMotion(MOTION_TYPE::BACK_STEP);

	// バクステアクションに変える
	pPerson->SetActionState(BASE_ACTION_STATE::BACKSTEP);

	// 走るエフェクト発動！
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::BACK_STEP);

	// 初速度の設定
	Vector3 move = pPerson->GetMove();
	switch (pPerson->GetDir())
	{
	case DIR::LEFT:
		move.x += 1.5f;
		break;
	case DIR::RIGHT:
		move.x -= 1.5f;
		break;
	default:
		MyAssert(0, "ふぁっきゅー");
		break;
	}

	pPerson->SetMove(move);

	// SE再生
	se->Play("ダッシュ");
}

void BasePlayerState::BackStep::Execute(BasePlayer * pPerson)
{
	// エスケープの時間更新&エスケープ時間終わったら
	switch (pPerson->GetActionFrame())
	{
	case FRAME_STATE::START:
		// フラグ設定
		pPerson->SetEscapeFlag(false);
		break;

	case FRAME_STATE::ACTIVE:
		// フラグ設定
		pPerson->SetEscapeFlag(true);
		break;

	case FRAME_STATE::FOLLOW:
		// フラグ設定
		pPerson->SetEscapeFlag(false);
		break;

	case FRAME_STATE::END:
		// 待機モーションに戻る
	{
		Vector3 move = pPerson->GetMove();
		move *= .5f;	// 慣性を弱くする
		pPerson->SetMove(move);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
		break;
	}
}

void BasePlayerState::BackStep::Exit(BasePlayer * pPerson)
{
	pPerson->SetEscapeFlag(false);
}

void BasePlayerState::BackStep::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "BackStepState");
}

bool BasePlayerState::BackStep::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 落下してるよメッセージ
	case MESSAGE_TYPE::FALL:
	{
							   // 落下ステートに行く！
							   pPerson->GetFSM()->ChangeState(Fall::GetInstance());
							   break;
	}
	}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					フロントブレーキステート
/*******************************************************/

//BasePlayerState::FrontBrake * BasePlayerState::FrontBrake::GetInstance()
//{
//	// ここに変数を作る
//	static BasePlayerState::FrontBrake instance;
//	return &instance;
//}
//
//void BasePlayerState::FrontBrake::Enter(BasePlayer * pPerson)
//{
//	// 前ブレーキモーションに変える
//	pPerson->SetMotion(MOTION_TYPE::BRAKE);
//
//	// SE再生
//	se->Play("ブレーキ1");
//}
//
//void BasePlayerState::FrontBrake::Execute(BasePlayer * pPerson)
//{
//	//////////////////////////////////////////////
//	//	ジャンプキャンセル
//	//============================================
//	if (JumpCancel(pPerson)) return;
//
//	Vector3 move = pPerson->GetMove();
//	
//	// A列車: MOVE値をここで直書きしている
//	move *= .95f;
//	pPerson->SetMove(move);
//
//	// ほぼ止まってたら、(減速自体はBaseのアップデートでやってる)
//	if (
//		//move.Length() < .1f
//		abs(move.x) < BasePlayer::c_END_MOVE_LINE
//		)
//	{
//		// 方向キーを入力していたら、再度「走り」ステートへ
//		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		{
//			pPerson->SetDir(DIR::LEFT);
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//		else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//		{
//			pPerson->SetDir(DIR::RIGHT);
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//
//		// 左右方向キーを押してなかったら、待機ステートへ
//		else
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		}
//
//	}
//}
//
//void BasePlayerState::FrontBrake::Exit(BasePlayer * pPerson)
//{
//
//
//}
//
//void BasePlayerState::FrontBrake::Render(BasePlayer * pPerson)
//{
//	tdnText::Draw(20, 690, 0xffffffff, "フロントBrakeState");
//}
//
//bool BasePlayerState::FrontBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	// メッセージタイプ
//	switch (msg.Msg)
//	{
//		// 落下してるよメッセージ
//	case MESSAGE_TYPE::FALL:
//	{
//		// 落下ステートに行く！
//		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
//		break;
//	}
//	}
//
//	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
//	return false;
//}



/*******************************************************/
//					Uターンブレーキステート
/*******************************************************/

//BasePlayerState::TurnOverBrake * BasePlayerState::TurnOverBrake::GetInstance()
//{
//	// ここに変数を作る
//	static BasePlayerState::TurnOverBrake instance;
//	return &instance;
//}
//
//void BasePlayerState::TurnOverBrake::Enter(BasePlayer * pPerson)
//{
//	// Uターンブレーキモーションに変える
//	pPerson->SetMotion(MOTION_TYPE::U_TURN);
//
//	// SE再生
//	se->Play("ブレーキ1");
//}
//
//void BasePlayerState::TurnOverBrake::Execute(BasePlayer * pPerson)
//{
//	//////////////////////////////////////////////
//	//	ジャンプキャンセル
//	//============================================
//	if (JumpCancel(pPerson)) return;
//
//	Vector3 move = pPerson->GetMove();
//
//	// ほぼ止まってたら、Waitステートに移行(減速自体はBaseのアップデートでやってる)
//	if (
//		//move.Length() < .1f
//		abs(move.x) < BasePlayer::c_END_MOVE_LINE
//		)
//	{
//		// 方向キーを入力していたら、再度「走り」ステートへ
//		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//		else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
//		}
//
//		// 左右方向キーを押してなかったら、待機ステートへ
//		else
//		{
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		}
//
//	}
//}
//
//void BasePlayerState::TurnOverBrake::Exit(BasePlayer * pPerson)
//{
//
//
//}
//
//void BasePlayerState::TurnOverBrake::Render(BasePlayer * pPerson)
//{
//	tdnText::Draw(20, 690, 0xffffffff, "TurnOverBrakeState");
//}
//
//bool BasePlayerState::TurnOverBrake::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	// メッセージタイプ
//	switch (msg.Msg)
//	{
//		// 落下してるよメッセージ
//	case MESSAGE_TYPE::FALL:
//	{
//		// 落下ステートに行く！
//		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
//		break;
//	}
//	}
//
//	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
//	return false;
//}



/*******************************************************/
//					ジャンプステート
/*******************************************************/

void BasePlayerState::Jump::Enter(BasePlayer * pPerson)
{
	// ジャンプモーションに変える
	pPerson->SetMotion(MOTION_TYPE::JUMP);

	// しゃがみフラグ初期化
	pPerson->GetJump()->Clear();

	pPerson->SetDirAngle();
}

void BasePlayerState::Jump::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプ発動前のしゃがみ中
	//============================================
	if (pPerson->GetJump()->bHold)
	{
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::C))
		{
			// プレイヤーが押してる時間計測
			pPerson->GetJump()->PlayerHoldTimer++;
		}

		if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::B))
			pPerson->GetJump()->bAttackPush = true;

		if (++pPerson->GetJump()->HoldTimer > 2)
		{
			// 初速度の設定
			Vector3 add(0, 0, 0);

			//// しゃがんでる間、ずっと押してたので、最大ジャンプ
			//if (pPerson->GetJump()->HoldTimer == pPerson->GetJump()->PlayerHoldTimer)
			{
				add.y = BasePlayer::c_MAX_JUMP;
			}
			//// そうじゃないので小ジャンプ
			//else
			//{
			//	add.y = BasePlayer::c_MAX_JUMP * .65f;
			//}

			// 左右入力
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))		add.x = -.5f;
			else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT)) add.x = .5f;

			// 移動地を足す！
			if (pPerson->GetFSM()->isPrevState(*Run::GetInstance()))
			{
				add.x *= .25f;
				pPerson->AddMove(add);
			}
			else
			{
				pPerson->SetMove(add);
			}

			// ★ジャンプエフェクト
			pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::JUMP);

			// 地上フラグオフ！
			//pPerson->SetLand(false);

			// しゃがみフラグ終了
			pPerson->GetJump()->bHold = false;

			// しゃがんでる最中に攻撃ボタンを押してたら
			if (pPerson->GetJump()->bAttackPush)
			{
				// 空下封印
				//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN) == true)
				//{
				//	// 空中下攻撃ステートに行く
				//	pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
				//}
				//else
				{
					// 空中攻撃ステートに行く
					pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
				}
				return;
			}

			// SE再生
			se->Play("ジャンプ");
		}
	}

	//////////////////////////////////////////////
	//	ジャンプ中
	//============================================
	else
	{
		// 地上フラグtrue(着地したら)
		if (pPerson->isLand())
		{
			// 着地ステートに移行
			pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
			return;
		}

		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		////////////////////////////////////////////////
		////	空中攻撃キャンセル
		////============================================
		//if (AerialAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	攻撃キャンセル
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	どっこい攻撃キャンセル
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ヒーホードライブボタン
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	空中ダッシュキャンセル
		//============================================
		if (AerialDashCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	空中ジャンプボタン
		//============================================
		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
		//{
		//	// 空中ジャンプの権利がなかったら弾く
		//	if (pPerson->isAerialJump() == true)
		//	{
		//		// 空中ジャンプステートに行く
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		//	}
		//}
		if (JumpCancel(pPerson))return;

		//////////////////////////////////////////////
		//	左入力
		//============================================
		//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		//{
		//	pPerson->AddMove(Vector3(-0.03f, 0.0f, 0.0f));
		//}


		//////////////////////////////////////////////
		//	右入力
		//============================================
		//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		//{
		//	pPerson->AddMove(Vector3(0.03f, 0.0f, 0.0f));
		//}
		//
		//// 移動地制御
		pPerson->MoveClampX(pPerson->GetMaxSpeed());
	}
}

void BasePlayerState::Jump::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::Jump::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "ジャンプすｙてーと");
	//tdnText::Draw(20, 690, 0xffffffff, "JumpState");
}

bool BasePlayerState::Jump::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//					空中ジャンプステート
/*******************************************************/

void BasePlayerState::AerialJump::Enter(BasePlayer * pPerson)
{
	// ジャンプモーションに変える
	pPerson->SetMotion(MOTION_TYPE::JUMP);

	// 空中ジャンプの権利を使用
	pPerson->SetAerialJump(false);

	// 移動地を足す！
	//Vector3 set(pPerson->GetMove());//c_MAX_AerialJump
	Vector3 set(0, 0, 0);
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		set.x = -.475f;
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		set.x = .475f;
	}
	set.y = 2.0f;
	pPerson->SetMove(set);// 前回のMOVE値を消す
	//pPerson->AddMove(add); 

	// 地上フラグオフ！
	//pPerson->SetLand(false);

	// SE再生
	se->Play("ジャンプ");
}

void BasePlayerState::AerialJump::Execute(BasePlayer * pPerson)
{

	//////////////////////////////////////////////
	//	空中ジャンプ中
	//============================================

	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	空中攻撃キャンセル
	//============================================
	//if (AerialAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (AttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	左入力
	//============================================
	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	pPerson->AddMove(Vector3(-0.025f, 0.0f, 0.0f));
	//}


	//////////////////////////////////////////////
	//	右入力
	//============================================
	//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	pPerson->AddMove(Vector3(0.025f, 0.0f, 0.0f));
	//}

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * 0.5f);

	// Yの移動量が-になったら落下ステートへ
	if (pPerson->GetMove().y < 0)
	{
		pPerson->GetFSM()->ChangeState(Fall::GetInstance());
		return;
	}

	// 地上フラグtrue(着地したら)
	if (pPerson->isLand())
	{
		// 着地ステートに移行
		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
	}

}

void BasePlayerState::AerialJump::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
	int i = 0;
}

void BasePlayerState::AerialJump::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "空中ジャンプすｙてーと");
	//tdnText::Draw(20, 690, 0xffffffff, "AerialJumpState");
}

bool BasePlayerState::AerialJump::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//					落下
/*******************************************************/

void BasePlayerState::Fall::Enter(BasePlayer * pPerson)
{
	// 落下モーションに変える
	pPerson->SetMotion(MOTION_TYPE::FALL);

	// 地面にいないよ！ということを設定
	pPerson->SetLand(false);
}

void BasePlayerState::Fall::Execute(BasePlayer * pPerson)
{
	// 地上フラグtrue(着地したら)
	if (pPerson->isLand())
	{
		// 着地ステートに移行
		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	落下中
	//============================================

	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;


	//////////////////////////////////////////////
	//	空中攻撃キャンセル
	//============================================
	//if (AerialAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (AttackCancel(pPerson))return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	左入力
	//============================================
	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	pPerson->AddMove(Vector3(-0.05f, 0.0f, 0.0f));
	//}
	//
	//
	////////////////////////////////////////////////
	//	右入力
	//============================================
	//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	pPerson->AddMove(Vector3(0.05f, 0.0f, 0.0f));
	//}

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * 1.1f);

}

void BasePlayerState::Fall::Exit(BasePlayer * pPerson)
{
	//pPerson->GetMove().y = 0;
}

void BasePlayerState::Fall::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "おちたな");
	//tdnText::Draw(20, 690, 0xffffffff, "FallState");
}

bool BasePlayerState::Fall::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}


/*******************************************************/
//					着地ステート
/*******************************************************/

void BasePlayerState::Land::Enter(BasePlayer * pPerson)
{
	// 着地モーションに変える
	pPerson->SetMotion(MOTION_TYPE::RAND);

	// 着地時間初期化
	pPerson->GetJump()->LandTimer = 0;

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed());

	// 着地エフェクト！
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::DROP_IMPACT);


	// アクションステートオフ(つまり現在までやったいた動きが消える)
	pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);
}

void BasePlayerState::Land::Execute(BasePlayer * pPerson)
{
	// 攻撃していて落ちたらスキがでるように
	//if (pPerson->GetFSM()->isPrevState(*BasePlayerState::AerialAttack::GetInstance()) == false &&
	//	pPerson->GetFSM()->isPrevState(*BasePlayerState::AerialDropAttack::GetInstance()) == false)
	{
		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ジャンプキャンセル
		//============================================
		if (JumpCancel(pPerson))
			return;

		//////////////////////////////////////////////
		//	ヒーホードライブボタン
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	攻撃キャンセル
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	しゃがみキャンセル
		//============================================
		if (SquatCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	はしりキャンセル
		//============================================
		if (RunCancel(pPerson)) return;
	}

	// 一定時間着地したら
	if (++pPerson->GetJump()->LandTimer > 12)
	{
		// 方向キーを入力していたら、「走り」ステートへ
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		{
			//pPerson->SetDir(DIR::LEFT);
			if (pPerson->GetDir() == DIR::LEFT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		}
		else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		{
			//pPerson->SetDir(DIR::RIGHT);
			if (pPerson->GetDir() == DIR::RIGHT) pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
			else pPerson->GetFSM()->ChangeState(BasePlayerState::BackWalk::GetInstance());
		}

		// 左右方向キーを押してなかったら、「待機」ステートへ
		else
		{
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

	}


}

void BasePlayerState::Land::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::Land::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "着地");
	//tdnText::Draw(20, 690, 0xffffffff, "LandState");
}

bool BasePlayerState::Land::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	return false;
}

/*******************************************************/
//					ラッシュ攻撃ステート
/*******************************************************/

void BasePlayerState::RushAttack::Enter(BasePlayer * pPerson)
{
	// 正面向きながら攻撃防止
	pPerson->SetDirAngle();

	// 初撃モーションに変える
	pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK1);

	// ラッシュステップ初期化
	pPerson->RushStepReset();

	// 攻撃ステートを1段目に設定する
	pPerson->SetActionState(BASE_ACTION_STATE::RUSH1);

	// 初速度の設定

	// もし走っている状態で攻撃をしたら　ダッシュ攻撃っぽく攻撃
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
	// 攻撃終了してたら
	if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	switch (pPerson->GetRushStep())
	{
	case 0:	// 1段目

		// ★若干早い段階から先行入力を受け付ける
		if (pPerson->GetActionFrame() == FRAME_STATE::START && pPerson->GetCurrentFrame() > BasePlayer::c_RUSH_AHEAD_START) pPerson->AheadCommandUpdate();

		// ヒットしてる状態
		if (pPerson->GetAttackData()->bHit)
		{
			// フォローモーション(次の攻撃へ)
			//if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				/* キャンセルルート */

				//////////////////////////////////////////////
				//	スキルキャンセル
				//============================================
				if (SkillCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	スタンドキャンセル
				//============================================
				if (StandCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	ヒーホードライブキャンセル
				//============================================
				if (HeaveHoCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	どっこい攻撃キャンセル
				//============================================
				if (DokkoiAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	対空攻撃キャンセル
				//============================================
				if (AntiAirCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	しゃがみ攻撃キャンセル
				//============================================
				if (SquatAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	ジャンプキャンセル
				//============================================
				if (JumpCancel(pPerson)) return;
				

				//////////////////////////////////////////////
				//	ヒーホードライブオーバーフローキャンセル
				//============================================
				if (HeaveHoOverFlowCancel(pPerson)) return;
				// 攻撃ボタン押したら(押しっぱなし込み)
				if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
				{
					// 先行入力リセット
					pPerson->AheadCommandReset();

					// 次のモーションセット
					pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK2);

					// 攻撃ステートを2段目に設定する
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH2);

					pPerson->AddRushStep();
					//pPerson->GetRushAttack()->bNextOK = false;

					// 向いてる方向にキーを入力すると移動
					const float pow = 0.8f;
					if (pPerson->GetDir() == DIR::RIGHT)
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))// 方向キーを入力してたら
							pPerson->AddMove(Vector3(pow, 0, 0));
					}
					else
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT)) // 方向キーを入力してたら
							pPerson->AddMove(Vector3(-pow, 0, 0));
					}

				}
			}
		}

		break;
	case 1:	// 2段目

		// ★若干早い段階から先行入力を受け付ける
		if (pPerson->GetActionFrame() == FRAME_STATE::START && pPerson->GetCurrentFrame() > BasePlayer::c_RUSH_AHEAD_START) pPerson->AheadCommandUpdate();

		// ヒットしてる状態
		if (pPerson->GetAttackData()->bHit)
		{
			// フォローモーション(次の攻撃へ)
			//if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				/* キャンセルルート */
				//////////////////////////////////////////////
				//	スキルキャンセル
				//============================================
				if (SkillCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	スタンドキャンセル
				//============================================
				if (StandCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	ヒーホードライブキャンセル
				//============================================
				if (HeaveHoCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	どっこい攻撃キャンセル
				//============================================
				if (DokkoiAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	対空攻撃キャンセル
				//============================================
				if (AntiAirCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	しゃがみ攻撃キャンセル
				//============================================
				if (SquatAttackCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	ジャンプキャンセル
				//============================================
				if (JumpCancel(pPerson)) return;

				//////////////////////////////////////////////
				//	ヒーホードライブオーバーフローキャンセル
				//============================================
				if (HeaveHoOverFlowCancel(pPerson)) return;

				// 攻撃ボタン押したら(押しっぱなし込み)
				if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::C, true))
				{
					// 先行入力リセット
					pPerson->AheadCommandReset();

					pPerson->AddRushStep();
					//pPerson->GetRushAttack()->bNextOK = false;

					// 向いてる方向にキーを入力すると移動
					const float pow = 0.8f;
					if (pPerson->GetDir() == DIR::RIGHT)
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))// 方向キーを入力してたら
							pPerson->AddMove(Vector3(pow, 0, 0));
					}
					else
					{
						if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT)) // 方向キーを入力してたら
							pPerson->AddMove(Vector3(-pow, 0, 0));
					}

					pPerson->ThirdRushInit();
				}
			}
		}

		break;
	case 2:	// 3段目
		pPerson->ThirdRushUpdate();
		break;
	}
}

void BasePlayerState::RushAttack::Exit(BasePlayer * pPerson)
{
	if (pPerson->GetRushStep()) pPerson->ThirdRushExit();
	// 先行入力リセット
	pPerson->AheadCommandReset();
}

void BasePlayerState::RushAttack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "ラッシュ");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::RushAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃当たったよメッセージ
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// ヒットストップ
	//	pPerson->GetRushAttack()->bNextOK = true;
	//	break;
	//}
	//}

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/*******************************************************/
//					ノックバックステート
/*******************************************************/

void BasePlayerState::KnockBack::Enter(BasePlayer * pPerson)
{
	// ダメージモーションに変える
	pPerson->SetMotion(MOTION_TYPE::KNOCKBACK);

}

void BasePlayerState::KnockBack::Execute(BasePlayer * pPerson)
{
	// 硬直が0以下なら硬直終了
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// 待機ステートに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());

		// 喰らったカウントリセット
		pPerson->GetRecoveryDamageCount()->clear();
		// 先行入力リセット
		pPerson->AheadCommandReset();
		// ダメージ補正リセット
		pPerson->ResetDamageRate();
	}
}

void BasePlayerState::KnockBack::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::KnockBack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "ノックバック！");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::KnockBack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//								 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//								 // 喰らってる状態でくらった攻撃リスト追加
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);
	//								 // trueは返さない。ここでは、ダウン状態でのくらったカウントを取るだけなので。
	//}
	//}
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					足払いくらいノックバックステート
/*******************************************************/

void BasePlayerState::DownFall::Enter(BasePlayer * pPerson)
{
	// 足払いくらいモーションに変える
	pPerson->SetMotion(MOTION_TYPE::KNOCKDOWN_DOWN);

	// ダウンフラグを立てる
	pPerson->SetDown(true);
}

void BasePlayerState::DownFall::Execute(BasePlayer * pPerson)
{
	// 硬直が0以下ならダメージ補正解除
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// ダメージ補正リセット
		pPerson->ResetDamageRate();
		// コンボ補正リセット
		pPerson->GetRecoveryDamageCount()->clear();
	}

	{
		// 硬直が0になったら　なおかついずれかのボタンを押していたら復帰ステートへ
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::A)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::B)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::C)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::D)||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::R1))
		{

			// ダウンフラグを解除
			pPerson->SetDown(false);
			// 喰らったカウントリセット
			pPerson->GetRecoveryDamageCount()->clear();
			// 先行入力リセット
			pPerson->AheadCommandReset();
			// ダメージ補正リセット
			pPerson->ResetDamageRate();

			// 地上オア空中
			if (pPerson->isLand() == true)
			{
				// 地上リカバリーステートへ！
				pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
			}
			//else
			//{
			//	// 空中リカバリーステートへ！
			//	pPerson->GetFSM()->ChangeState(BasePlayerState::AerialRecovery::GetInstance());
			//}


		}
	}
}

void BasePlayerState::DownFall::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::DownFall::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "選りえるノックバック");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::DownFall::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
		//{
		//	// 攻撃くらったよメッセージ
		//case MESSAGE_TYPE::HIT_DAMAGE:
		//{
		//								 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
		//								 // 喰らってる状態でくらった攻撃リスト追加
		//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);
		//								 // trueは返さない。ここでは、ダウン状態でのくらったカウントを取るだけなので。
		//}
		//}
		// Flaseで返すとグローバルステートのOnMessageの処理へ行く
		return false;
}



/*******************************************************/
//					ノックダウンステート
/*******************************************************/

void BasePlayerState::KnockDown::Enter(BasePlayer * pPerson)
{
	const float MoveLen(pPerson->GetMove().LengthSq());

	// ノックダウンモーションに変える
	pPerson->SetMotion((MoveLen > 1) ? MOTION_TYPE::KNOCKDOWN : MOTION_TYPE::KNOCKDOWN_DOWN);

	// ダウンフラグを立てる
	pPerson->SetDown(true);

	//むてきつくった
	// ★やっぱりここで無敵時間設定(GlobalのMessageで書きます)
	//pPerson->SetInvincible(90,1);

	// 仮
	//pPerson->SetRecoveryFrame(120);

}

void BasePlayerState::KnockDown::Execute(BasePlayer * pPerson)
{
	if (pPerson->isLand() == false)
	{
		// 吹っ飛びけむりパーティクル
		ParticleManager::EffectFlySmoke(pPerson->GetPos());
		//return; // [10/20日]　空中でも復帰できるようにしました
	}

	// 硬直が0以下なら硬直終了
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// 喰らったカウントリセット
		pPerson->GetRecoveryDamageCount()->clear();
		// ダメージ補正リセット
		pPerson->ResetDamageRate();

		//pPerson->SetInvincible(60, 1);

		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::A) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::B) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::C) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::D) ||
			pPerson->isPushInput(PLAYER_COMMAND_BIT::R1)  )
		{
			// ダウンフラグを解除
			pPerson->SetDown(false);

			// 先行入力リセット
			pPerson->AheadCommandReset();

			// 地上オア空中
			if (pPerson->isLand() == true)
			{
				// 地上リカバリーステートへ！
				pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
				return;
			}
			else
			{
				// 空中リカバリーステートへ！
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialRecovery::GetInstance());
				return;	
			}


		}

	}
}

void BasePlayerState::KnockDown::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::KnockDown::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "ノックダウン");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::KnockDown::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//								 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//								 // 喰らってる状態でくらった攻撃リスト追加
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)HitDamageInfo->iAttackType);
	//								 // trueは返さない。ここでは、ダウン状態でのくらったカウントを取るだけなので。
	//}
	//}
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/*******************************************************/
//					死に状態ステート
/*******************************************************/

void BasePlayerState::Die::Enter(BasePlayer * pPerson)
{
	// 死にモーションに変える
	pPerson->SetMotion(MOTION_TYPE::KNOCKDOWN);
}

void BasePlayerState::Die::Execute(BasePlayer * pPerson)
{
}

void BasePlayerState::Die::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::Die::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "死に");
}

bool BasePlayerState::Die::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					地上リカバリーステート
/*******************************************************/

void BasePlayerState::LandRecovery::Enter(BasePlayer * pPerson)
{
	// リカバリーに変える
	pPerson->SetMotion(MOTION_TYPE::REVERSAL);

	// りかバリーかうんとを初期化
	//pPerson->SetRecoveryCount(0);

	// カウントをするステートに変更
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// リカバー中は無敵！！！
	//pPerson->SetInvincibleLV(1);
	pPerson->SetInvincible(
#ifdef GUARD_ON
		114514
#else
		40
#endif
		, 1);	// 1031 無敵時間を12→40にした

	// エフェクト発動」
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3), EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::LandRecovery::Execute(BasePlayer * pPerson)
{
	//pPerson->AddRecoveryCount(1);

	const int iRecoveryCount(pPerson->GetCurrentFrame());

	// ★5フレーム前から先行入力受付(リバサ無敵用)
	if (iRecoveryCount > BasePlayer::c_RECOVERY_FLAME - 5)pPerson->AheadCommandUpdate();

	// リカバリーフレームを超えたら終わり！
	if (iRecoveryCount >= BasePlayer::c_RECOVERY_FLAME)
	{
		// リバサ無敵
		if (InvincibleAttackCancel(pPerson)) return;

		// 小パン暴れ
		if (AttackCancel(pPerson)) return;

		// ガード
		//if (isInputGuardCommand(pPerson)) pPerson->GetFSM(Guard::GetInstance());

		// ↑いれっぱでジャンプ逃げ
		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::UP))
		{
			pPerson->GetFSM()->ChangeState(Jump::GetInstance());
			return;
		}

		// 待機ステートへ
		pPerson->GetFSM()->ChangeState(Wait::GetInstance());
	}
}

void BasePlayerState::LandRecovery::Exit(BasePlayer * pPerson)
{
	pPerson->SetInvincible(0, 0);
}

void BasePlayerState::LandRecovery::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "りかばり");
}

bool BasePlayerState::LandRecovery::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}

/*******************************************************/
//					空中リカバリーステート (空中)
/*******************************************************/

void BasePlayerState::AerialRecovery::Enter(BasePlayer * pPerson)
{
	// リカバリーに変える
	pPerson->SetMotion(MOTION_TYPE::FALL);

	// りかバリーかうんとを初期化
	//pPerson->SetRecoveryCount(0);

	// カウントをするステートに変更
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// リカバー中は無敵！！！
	//pPerson->SetInvincibleLV(1);
	pPerson->SetInvincible(90, 1);

	Vector3 move;
	move = pPerson->GetMove()*0.25f;
	move += Vector3(0, 1.5f, 0);
	//pPerson->SetMove(move);

	// [10/20] moveではなくセットに
	// ＋Ｍｏｖｅ
	//Vector3 addMove = Vector3(0, 2.55f, 0);

	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	{
		//addMove += Vector3(0.7f, 0, 0);
		move += Vector3(0.7f, 0, 0);
	}
	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		//addMove += Vector3(-0.7f, 0, 0);
		move += Vector3(-0.7f, 0, 0);
	}
	
	pPerson->SetMove(move);

	//pPerson->AddMove(addMove);

	// エフェクト発動」
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3) , EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::AerialRecovery::Execute(BasePlayer * pPerson)
{
	//pPerson->AddRecoveryCount(1);
	const int iRecoveryCount(pPerson->GetCurrentFrame());

	// ★5フレーム前から先行入力受付(リバサ無敵用)
	if (iRecoveryCount > BasePlayer::c_RECOVERY_FLAME - 5)pPerson->AheadCommandUpdate();

	// リカバリーフレームを超えたら終わり！
	if (iRecoveryCount >= BasePlayer::c_RECOVERY_FLAME)
	{
		// 空中なので落ちるステートに
		pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
	}
}

void BasePlayerState::AerialRecovery::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::AerialRecovery::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "空中りかばり");
}

bool BasePlayerState::AerialRecovery::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
		return false;
}


/*******************************************************/
//					しゃがみステート
/*******************************************************/

void BasePlayerState::Squat::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(MOTION_TYPE::SQUAT);

	// しゃがみフラグON
	pPerson->SetSquat(true);

}

void BasePlayerState::Squat::Execute(BasePlayer * pPerson)
{
	//	しゃがみ攻撃ボタン
	//if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::A))
	//{
	//	// しゃがみ攻撃ステートに行く
	//	pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
	//	return;
	//}

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (AttackCancel(pPerson))return;

	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	どっこい攻撃キャンセル
	//============================================
	if (DokkoiAttackCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	ヒーホードライブボタン
	//============================================
	if (HeaveHoCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	投げキャンセル
	//============================================
	if (ThrowCancel(pPerson)) return;

	//////////////////////////////////////////////
	//	逆切れキャンセル
	//============================================
	if (InvincibleAttackCancel(pPerson)) return;

	// しゃがみボタンを離したら待機に戻る
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN) == false)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}

}

void BasePlayerState::Squat::Exit(BasePlayer * pPerson)
{
	// しゃがみフラグOFF
	pPerson->SetSquat(false);

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
//					しゃがみ攻撃ステート
/*******************************************************/

void BasePlayerState::SquatAttack::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(MOTION_TYPE::SQUAT_ATTACK);

	// ★攻撃ステートを2段目に設定する　↓でHITフラグを消している
	pPerson->SetActionState(BASE_ACTION_STATE::SQUAT_ATTACK);

	pPerson->SetDirAngle();
}

void BasePlayerState::SquatAttack::Execute(BasePlayer * pPerson)
{
	// 攻撃終了してたら
	if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		ChangeWaitState(pPerson);
		return;
	}

	// HITしていたらキャンセルできる
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	どっこい攻撃と足払いキャンセル
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	対空攻撃キャンセル
		//============================================
		if (AntiAirCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ラッシュ攻撃キャンセル
		//============================================
		if (RushAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ヒーホードライブキャンセル
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//============================================
		//	ジャンプボタン
		//============================================		
		if (JumpCancel(pPerson))
		{
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))	
				pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		
			// 高く飛ばす 
			pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

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
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃当たったよメッセージ
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// ヒットストップ
	//	break;
	//}
	//}
	return false;
}


/*******************************************************/
//					対空攻撃ステート
/*******************************************************/

void BasePlayerState::AntiAirAttack::Enter(BasePlayer * pPerson)
{
	// 対空攻撃モーションに変える
	pPerson->SetMotion(MOTION_TYPE::ANTI_AIR_ATTACK);

	// 攻撃ステートを対空に
	pPerson->SetActionState(BASE_ACTION_STATE::ANTI_AIR);

	// 慣性を消す
	pPerson->SetMove(Vector3(0,0,0));

	pPerson->SetDirAngle();
}

void BasePlayerState::AntiAirAttack::Execute(BasePlayer * pPerson)
{
	// 攻撃終了してたら
	if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HITしていたらキャンセルできる
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	どっこい攻撃と足払いキャンセル
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ヒーホードライブキャンセル
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//============================================
		//	ジャンプボタン
		//============================================
		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
		//{

		//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		//	{
		//		pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));		
		//	}
		//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		//	{
		//		pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		//	}

		//	// 高く飛ばす 
		//	pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		//	// ジャンプステートに行く
		//	pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		//	return;
		//}

		if (JumpCancel(pPerson))
		{
			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));

			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
				pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));

			// 高く飛ばす 
			pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			return;
		}

	}

}

void BasePlayerState::AntiAirAttack::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::AntiAirAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "AntiAirAttackState");
}

bool BasePlayerState::AntiAirAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}


/*******************************************************/
//					足払い攻撃ステート
/*******************************************************/

void BasePlayerState::DownAttack::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(MOTION_TYPE::DOWN_ATTACK);

	// ★攻撃ステートを2段目に設定する　↓でHITフラグを消している
	pPerson->SetActionState(BASE_ACTION_STATE::DOWN_ATTACK);

	pPerson->SetDirAngle();
}

void BasePlayerState::DownAttack::Execute(BasePlayer * pPerson)
{
	// 攻撃終了してたら
	if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		ChangeWaitState(pPerson);
		return;
	}

	// HITしていたらキャンセルできる
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ヒーホードライブキャンセル
		//============================================
		if (HeaveHoCancel(pPerson)) return;
	}

}

void BasePlayerState::DownAttack::Exit(BasePlayer * pPerson)
{
	// 先行入力リセット
	pPerson->AheadCommandReset();
}

void BasePlayerState::DownAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "DownAttackState");
}

bool BasePlayerState::DownAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃当たったよメッセージ
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// ヒットストップ
	//	break;
	//}
	//}
	return false;
}


/*******************************************************/
//					中段攻撃(どっこいアタック)ステート
/*******************************************************/

void BasePlayerState::DokkoiAttack::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(MOTION_TYPE::DOKKOI_ATTACK);

	// ★中段攻撃ステートを設定
	pPerson->SetActionState(BASE_ACTION_STATE::DOKKOI_ATTACK);

	// 仮
	pPerson->AddEffectAction(pPerson->GetPos(), EFFECT_TYPE::DOKKOI);

	pPerson->SetDirAngle();
}

void BasePlayerState::DokkoiAttack::Execute(BasePlayer * pPerson)
{
	// ダッシュキャンセル中
	if (pPerson->GetActionState() == BASE_ACTION_STATE::FRAMECOUNT)
	{
		// 一定時間走って何もしてなかったら待機ステートへ
		if (pPerson->GetCurrentFrame() > 30)
		{
			pPerson->GetFSM()->ChangeState(Wait::GetInstance());
			return;
		}

		float speed(pPerson->GetMaxSpeed());
		// 向いてる方に移動
		pPerson->SetMove(Vector3((pPerson->GetDir() == DIR::LEFT) ? -speed : speed, 0, 0));

		//////////////////////////////////////////////
		//	攻撃キャンセル
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	必殺キャンセル
		//============================================
		if (HeaveHoCancel(pPerson)) return;
	}

	// 攻撃終了してたら
	else if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HITしていたらキャンセルできる
	else if (pPerson->GetAttackData()->bHitSuccess == true)
	{
		// 同じボタンか相手の方向
		if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::R1, true) || pPerson->isPushInputTRG((pPerson->GetDir() == DIR::LEFT) ? PLAYER_COMMAND_BIT::LEFT : PLAYER_COMMAND_BIT::RIGHT, true))
		{
			// 走りモーション
			pPerson->SetMotion(MOTION_TYPE::RUN);

			// フレーム計測ステートへ
			pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);
		}
	}
}

void BasePlayerState::DokkoiAttack::Exit(BasePlayer * pPerson)
{
	// 先行入力リセット
	pPerson->AheadCommandReset();
}

void BasePlayerState::DokkoiAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "DokkoiAttackState");
}

bool BasePlayerState::DokkoiAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃当たったよメッセージ
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// ヒットストップ
	//	break;
	//}
	//}
	return false;
}


/*******************************************************/
//					空中攻撃ステート
/*******************************************************/

void BasePlayerState::AerialAttack::Enter(BasePlayer * pPerson)
{
	// 空中攻撃モーションに変える
	pPerson->SetMotion(MOTION_TYPE::AERIAL_ATTACK);


	// ★攻撃ステートを設定する　↓でHITフラグを消している
	pPerson->SetActionState(BASE_ACTION_STATE::AERIAL);

	// アングルを相手の向きへ
	pPerson->SetDirAngle();

	// 入力方向に向く
	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
	//{
	//	pPerson->SetDir(DIR::RIGHT);
	//}
	//else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	//{
	//	pPerson->SetDir(DIR::LEFT);
	//}
}

void BasePlayerState::AerialAttack::Execute(BasePlayer * pPerson)
{
	// 地上フラグtrue(着地したら)
	if (pPerson->isLand())
	{
		// 着地ステートに移行
		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
		return;
	}

	// 攻撃終了してたら
	if (!pPerson->isAttackState())
	{
		// 地上、空中で分岐
		//if (pPerson->isLand())
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		//else if(pPerson->isAerialJump()) pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		//else pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
		return;
	}

	// HITしていたらキャンセルできる
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ヒーホードライブボタン
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;


		//============================================
		//	空中ジャンプキャンセルボタン
		//============================================
		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
		//{

		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
		//	//{
		//	//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
		//	//}
		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		//	//{
		//	//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
		//	//}

		//	//// 高く飛ばす 
		//	//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		//
		//	// 空中ジャンプの権利がなかったら弾く
		//	if (pPerson->isAerialJump() == true)
		//	{
		//		// 空中ジャンプステートに行く
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		//	}
		//	
		//	return;
		//}
		if (JumpCancel(pPerson))return;
		

	}


}

void BasePlayerState::AerialAttack::Exit(BasePlayer * pPerson)
{
	// 先行入力初期化
	pPerson->AheadCommandReset();
}

void BasePlayerState::AerialAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "AerialAttackState");
}

bool BasePlayerState::AerialAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃当たったよメッセージ
	//case MESSAGE_TYPE::HIT_ATTACK:
	//{
	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// ヒットストップ
	//	break;
	//}
	//}
	return false;
}

/*******************************************************/
//					空中下攻撃ステート
/*******************************************************/

//BasePlayerState::AerialDropAttack * BasePlayerState::AerialDropAttack::GetInstance()
//{
//	// ここに変数を作る
//	static BasePlayerState::AerialDropAttack instance;
//	return &instance;
//}
//
//void BasePlayerState::AerialDropAttack::Enter(BasePlayer * pPerson)
//{
//	// 空中下攻撃モーションに変える
//	pPerson->SetMotion(MOTION_TYPE::AERIALDROP_ATTACK);
//
//
//	// ★攻撃ステートを設定する　↓でHITフラグを消している
//	pPerson->SetActionState(BASE_ACTION_STATE::AERIALDROP);
//
//	// 入力方向に向く
//	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//	{
//		pPerson->SetDir(DIR::RIGHT);
//	}
//	else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//	{
//		pPerson->SetDir(DIR::LEFT);
//	}
//
//}
//
//void BasePlayerState::AerialDropAttack::Execute(BasePlayer * pPerson)
//{
//	// 地上フラグtrue(着地したら)
//	if (pPerson->isLand())
//	{
//		// 着地ステートに移行
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Land::GetInstance());
//		return;
//	}
//
//	// 攻撃終了してたら
//	if (!pPerson->isAttackState())
//	{
//		// 待機モーションに戻る
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		return;
//	}
//
//	// HITしていたらキャンセルできる
//	if (pPerson->GetAttackData()->bHit == true)
//	{
//		//============================================
//		//	ジャンプボタン
//		//============================================
//		//if (pPerson->GetInputList(PLAYER_COMMAND_BIT::C) == 3)
//		//{
//
//		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT))
//		//	//{
//		//	//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
//		//	//}
//		//	//if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		//	//{
//		//	//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
//		//	//}
//
//		//	//// 高く飛ばす 
//		//	//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));
//
//		//	// 空中ジャンプの権利がなかったら弾く
//		//	if (pPerson->isAerialJump() == true)
//		//	{
//		//		// 空中ジャンプステートに行く
//		//		pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
//		//	}
//		//	return;
//		//}
//		if (JumpCancel(pPerson))return;
//	
//
//	}
//
//}
//
//void BasePlayerState::AerialDropAttack::Exit(BasePlayer * pPerson)
//{
//
//
//}
//
//void BasePlayerState::AerialDropAttack::Render(BasePlayer * pPerson)
//{
//
//	tdnText::Draw(20, 690, 0xffffffff, "AerialDropAttackState");
//}
//
//bool BasePlayerState::AerialDropAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	//// メッセージタイプ
//	//switch (msg.Msg)
//	//{
//	//	// 攻撃当たったよメッセージ
//	//case MESSAGE_TYPE::HIT_ATTACK:
//	//{
//	//	HIT_ATTACK_INFO *hai = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
//	//	pPerson->SetHitStopFrame(hai->hitStopFlame);// ヒットストップ
//	//	break;
//	//}
//	//}
//	return false;
//}

/*******************************************************/
//					回避ステート
/*******************************************************/

void BasePlayerState::Escape::Enter(BasePlayer * pPerson)
{
	// 回避モーションに変える
	pPerson->SetMotion(MOTION_TYPE::ESCAPE);

	// ★エスケープのフレームに設定する
	pPerson->SetActionState(BASE_ACTION_STATE::ESCAPE);
	
	// 逆の向きを向く
	//pPerson->TurnOverDir();

	pPerson->SetDirAngle();

	// 回避エフェクト発動！
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::ESCAPE);

	// SE再生
	se->Play("エスケープ");
}

void BasePlayerState::Escape::Execute(BasePlayer * pPerson)
{
	// エスケープの時間更新&エスケープ時間終わったら
	switch (pPerson->GetActionFrame())
	{
	case FRAME_STATE::START:
		// フラグ設定
		pPerson->SetEscapeFlag(false);
		// クイックエスケープ
		// 向いている方向にスゥーっと移動
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(.1f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.1f, 0, 0));
		}
		break;

	case FRAME_STATE::ACTIVE:
		// フラグ設定
		pPerson->SetEscapeFlag(true);
		// クイックエスケープ
		// 向いている方向にスゥーっと移動
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(1.45f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-1.45f, 0, 0));
		}
		break;

	case FRAME_STATE::FOLLOW:
		// フラグ設定
		pPerson->SetEscapeFlag(false);
		// クイックエスケープ
		// 向いている方向にスゥーっと移動
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(.1f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.1f, 0, 0));
		}
		break;

	case FRAME_STATE::END:
		pPerson->SetEscapeFlag(false);
		// 待機モーションに戻る
		Vector3 move = pPerson->GetMove();
		move *= .5f;	// 慣性を弱くする
		pPerson->SetMove(move);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}



}

void BasePlayerState::Escape::Exit(BasePlayer * pPerson)
{
	// フラグ設定
	pPerson->SetEscapeFlag(false);

}

void BasePlayerState::Escape::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "EscapeState");
}

bool BasePlayerState::Escape::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// グローバルステートに行かないようにする！
	//	return true;
	//}
	//}

	return false;
}


/*******************************************************/
//					ペルソナ召喚モーションステート
/*******************************************************/

void BasePlayerState::StandAction::Enter(BasePlayer * pPerson)
{
	// 召喚モーションに変える
	pPerson->SetMotion(MOTION_TYPE::PERSONA);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::STAND);

	// ★スタンドアクション発動

	pPerson->GetStand()->Action(GetSkillActionType(pPerson));

	// 重力の影響受けるな！
	pPerson->SetMoveUpdate(false);

	// SE再生
	se->Play("ペルソナ召喚");

	// ペルソナ発動エフェクト！
	pPerson->AddEffectAction(pPerson->GetPos(),EFFECT_TYPE::PERSONA);

	pPerson->SetDirAngle();

	// 前回移動量保存
	//pPerson->SetStandSaveMove(pPerson->GetMove());
}

void BasePlayerState::StandAction::Execute(BasePlayer * pPerson)
{
	// オーラのパーティクル
	ParticleManager::EffectPersonaAura(Vector3(pPerson->GetPos().x, pPerson->GetPos().y, -0.5f));	// 若干手前

	if (pPerson->GetStand()->isHit())
	{
		/* キャンセルルート */

		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	しゃがみキャンセル
		//============================================
		if (SquatCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ジャンプキャンセル
		//============================================
		if (JumpCancel(pPerson))
		{

			if (pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT) == true)
			{
				pPerson->AddMove(Vector3(-0.45f, 0, 0));
			}
			else if (pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) == true)
			{
				pPerson->AddMove(Vector3(0.45f, 0, 0));
			}

			return;
		}
		
		//////////////////////////////////////////////
		//	ヒーホードライブキャンセル
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	左右キーで走りキャンセル
		//============================================
		if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
		{
			pPerson->SetDir(DIR::LEFT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::LEFT))
		{
			pPerson->SetDir(DIR::RIGHT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
	}

	// 移動量無し
	//pPerson->SetMove(move);

	// アクションフレーム終了してたら、戻る
	if (!pPerson->isFrameAction())
	{
		// 保存してる移動量を戻す
		//pPerson->SetMove(pPerson->GetStandSaveMove());

		// 待機に戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::StandAction::Exit(BasePlayer * pPerson)
{
	// 重力の影響を元に戻す
	pPerson->SetMoveUpdate(true);
}

void BasePlayerState::StandAction::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "EscapeState");
}

bool BasePlayerState::StandAction::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// グローバルステートに行かないようにする！
	//	return true;
	//}
	//}

	return false;
}



/*******************************************************/
//					無敵攻撃ステート
/*******************************************************/

void BasePlayerState::FinishAttack::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::FINISH_ATTACK);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::INVINCIBLE_ATTACK);

	// ★無敵フラグをONにする
	pPerson->SetInvincible(114514, 1);

	// 始動エフェクト
	pPerson->AddEffectAction(pPerson->GetPos() , EFFECT_TYPE::INVINCIBLE_ATTACK);

	pPerson->SetDirAngle();

	// SE再生
	//se->Play("ペルソナ召喚");
}

void BasePlayerState::FinishAttack::Execute(BasePlayer * pPerson)
{
	// 攻撃ステート終わったら
	if (!pPerson->isAttackState())
	{
		// 待機ステートに(さすがにキャンセルはやめておこう)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
	// ★フォローモーションになったら無敵を解除
	if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
	{
		pPerson->SetInvincible(0, 0);
	}
}

void BasePlayerState::FinishAttack::Exit(BasePlayer * pPerson)
{
	// 先行入力リセット
	pPerson->AheadCommandReset();
}

void BasePlayerState::FinishAttack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "フィニッシュあたっくState");
}

bool BasePlayerState::FinishAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// グローバルステートに行かないようにする！
	//	return true;
	//}
	//}

	return false;
}



/*******************************************************/
//					スキルステート
/*******************************************************/

void BasePlayerState::Skill::Enter(BasePlayer * pPerson)
{
	pPerson->SetSkillActionType(GetSkillActionType(pPerson));

	// 先行入力リセット
	pPerson->AheadCommandReset();

	// スキル用変数初期化
	pPerson->SkillInit();

	// SE再生
	//se->Play("ペルソナ召喚");
}

void BasePlayerState::Skill::Execute(BasePlayer * pPerson)
{
	// そのキャラクターのスキルの更新
	const bool bEnd = pPerson->SkillUpdate();

	// 攻撃ステート終わったら
	//if (pPerson->GetActionFrame()==FRAME_STATE::END)
	if (bEnd)
	{
		// 待機ステートに(さすがにキャンセルはやめておこう)
		if (pPerson->isLand()) ChangeWaitState(pPerson);
		else pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
		return;
	}

	// 攻撃が当たっていたら
	//if (pPerson->GetAttackData()->bHit)
	//{
	//	//////////////////////////////////////////////
	//	//	ヒーホードライブキャンセル
	//	//============================================
	//	if (HeaveHoCancel(pPerson)) return;
	//}
}

void BasePlayerState::Skill::Exit(BasePlayer * pPerson)
{
	// アイルーとかで重力無視したりするので、一応ここで解除しておく
	pPerson->SetMoveUpdate(true);

	// スキルの終わり
	pPerson->SkillExit();

	// 先行入力リセット
	pPerson->AheadCommandReset();

	pPerson->SetDirAngle();
}

void BasePlayerState::Skill::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "スキルState");
}

bool BasePlayerState::Skill::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//{
	//	// グローバルステートに行かないようにする！
	//	return true;
	//}
	//}

	return false;
}


/*******************************************************/
//					1more覚醒ステート
/*******************************************************/

void BasePlayerState::OverDrive_OneMore::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::OVERDRIVE_ONEMORE);

	// オーバードライブ
	pPerson->ActionOverDrive(OVERDRIVE_TYPE::ONEMORE);

	// SE再生
	se->Play("オーバードライブ開始");

	// 上に浮く
	pPerson->SetMove(Vector3(0, 1, 0));

	// バースト前エフェクト　発動！
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::ONEMORE_BURST_START);

	// ゲームを止めるフラグ
	pPerson->SetGameTimerStopFlag(true);

	// ★相手の同技補正を解除する
	pPerson->GetTargetPlayer()->GetRecoveryDamageCount()->clear();
	pPerson->GetTargetPlayer()->ResetDamageRate();
}

void BasePlayerState::OverDrive_OneMore::Execute(BasePlayer * pPerson)
{

	// ★この間ゲージ減らんといてくれの処理
	pPerson->SetOverDriveFrame(BasePlayer::c_OVERDRIVE_MAX_TIME);

	if (pPerson->GetActionFrame() == FRAME_STATE::START)
	{
		// ↑
		pPerson->SetMove(Vector3(0, .21f, 0));
	}
	if (pPerson->GetActionFrame() == FRAME_STATE::ACTIVE)
	{
		// バーストエフェクト発動！
		pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, 0), EFFECT_TYPE::ONEMORE_BURST);

		// 動きを止める
		pPerson->SetMove(Vector3(0, 0, 0));
	}
	if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
	{
		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ジャンプキャンセル
		//============================================
		if (JumpCancel(pPerson)) return;
		
		//////////////////////////////////////////////
		//	ヒーホードライブボタン
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	攻撃ボタン
		//============================================
		if (AttackCancel(pPerson)) return;
		
		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		// 動きを止める
		pPerson->SetMove(Vector3(0, 0, 0));
	}

	// アクションフレーム終了してたら、戻る
	if (!pPerson->isFrameAction())
	{
		if (pPerson->isLand()==true)
		{
			// 待機ステートに
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
			return;
		}
		else
		{
			// 落下ステートに
			pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
			return;
		}


	}
	
}

void BasePlayerState::OverDrive_OneMore::Exit(BasePlayer * pPerson)
{
	// ゲームを止めるフラグ
	pPerson->SetGameTimerStopFlag(false);


}

void BasePlayerState::OverDrive_OneMore::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "OverDrive_OneMoreState");
}

bool BasePlayerState::OverDrive_OneMore::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 覚醒してるときは無敵
	case MESSAGE_TYPE::HIT_DAMAGE:
	{

									 // アクションフレームが続いてたら
									 if (pPerson->isFrameAction())
									 {
										 // グローバルステートに行かないようにする！
										 return true;
									 }


	}

	}
	return false;

}



/*******************************************************/
//					Burst覚醒ステート
/*******************************************************/

void BasePlayerState::OverDrive_Burst::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::BURST);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::OVERDRIVE_BURST);

	// オーバードライブ
	pPerson->ActionOverDrive(OVERDRIVE_TYPE::BURST);

	// SE再生
	se->Play("バースト");


}

void BasePlayerState::OverDrive_Burst::Execute(BasePlayer * pPerson)
{

	// 動きを止める
	pPerson->SetMove(Vector3(0, 0, 0));

	if (pPerson->GetActionFrame() == FRAME_STATE::ACTIVE)
	{
		// バーストエフェクト発動！
		pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, 0), EFFECT_TYPE::BURST);
	}

	// アクションフレーム終了してたら、戻る
	if (!pPerson->isFrameAction())
	{
		// 待機ステートに
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());

	}

}

void BasePlayerState::OverDrive_Burst::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::OverDrive_Burst::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "OverDrive_BurstState");
}

bool BasePlayerState::OverDrive_Burst::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 覚醒してるときは無敵
	case MESSAGE_TYPE::HIT_DAMAGE:
	{

									 // アクションフレームが続いてたら
									 if (pPerson->isFrameAction())
									 {
										 // グローバルステートに行かないようにする！
										 return true;
									 }


	}

	}

	return false;
}

/*******************************************************/
//					ガードステート
/*******************************************************/

void BasePlayerState::Guard::Enter(BasePlayer * pPerson)
{
	/* しゃがみガード */
	if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
	{
		pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);

		// ガードステートを設定する
		pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
	}

	/* 立ちガード */
	else
	{
		pPerson->SetMotion(MOTION_TYPE::UP_GUARD);

		// ガードステートを設定する
		pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
	}

}

void BasePlayerState::Guard::Execute(BasePlayer * pPerson)
{	
	// ★硬直が0以下なら硬直終了
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		//////////////////////////////////////////////
		//	攻撃キャンセル
		//============================================
		if (AttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	どっこい攻撃キャンセル
		//============================================
		if (DokkoiAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	回避キャンセル
		//============================================
		if (EscapeCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ジャンプキャンセル
		//============================================
		if (JumpCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ヒーホードライブボタン
		//============================================
		if (HeaveHoCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	投げキャンセル
		//============================================
		if (ThrowCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	逆切れキャンセル
		//============================================
		if (InvincibleAttackCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	ダッシュキャンセル
		//============================================
		if (DashCancel(pPerson))return;

		// 相手キャラが攻撃ステートなら
		if (pPerson->GetTargetPlayer()->isAttackState() || (pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData()))
		{
			if (isInputGuardCommand(pPerson))
			{
				/* しゃがみガード */
				if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
				{
					pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);

					// ガードステートを設定する
					pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
					return;
				}

				/* 立ちガード */
				else
				{
					pPerson->SetMotion(MOTION_TYPE::UP_GUARD);

					// ガードステートを設定する
					pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
					return;
				}
			}
		}
		// 待機モーションに戻る
		ChangeWaitState(pPerson);

		//// 相手キャラが攻撃ステートなら
		//if (pPerson->GetTargetPlayer()->isAttackState() || (pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData()))
		//{
		//	if (isInputGuardCommand(pPerson))
		//	{
		//		/* しゃがみガード */
		//		if (pPerson->isPushInput(PLAYER_COMMAND_BIT::DOWN))
		//		{
		//			pPerson->SetMotion(MOTION_TYPE::DOWN_GUARD);

		//			// ガードステートを設定する
		//			pPerson->SetGuardState(GUARD_STATE::DOWN_GUARD);
		//		}

		//		/* 立ちガード */
		//		else
		//		{
		//			pPerson->SetMotion(MOTION_TYPE::UP_GUARD);

		//			// ガードステートを設定する
		//			pPerson->SetGuardState(GUARD_STATE::UP_GUARD);
		//		}
		//	}
		//}
		//else
		//{
		//	// 相手が攻撃ステートじゃなかったら
		//	//if (!pPerson->GetTargetPlayer()->isAttackState() && !(pPerson->GetTargetPlayer()->GetStand()->isActive() && pPerson->GetTargetPlayer()->GetStand()->GetAttackData()))
		//	{
		//		//pPerson->GuardEffectStop();
		//		//pPerson->SetGuard(false);// ガード状態解除へ
		//		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		//	}
		//}
	}
}




void BasePlayerState::Guard::Exit(BasePlayer * pPerson)
{
	pPerson->SetGuardState(GUARD_STATE::NO_GUARD);// ガード状態解除

	pPerson->GuardEffectStop();
}

void BasePlayerState::Guard::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "GuardState");
}

bool BasePlayerState::Guard::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//if (pPerson->isGuard() == false)return false;

	// メッセージタイプ
	switch (msg.Msg)
	{
		// ★ガード中は無敵！
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
									 HIT_DAMAGE_INFO *HitDamageInfo = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る

									 bool bGuardSuccess(false);
									 if (pPerson->GetGuardState() == GUARD_STATE::UP_GUARD)
									 {
										 // ★相手が立ちガードしてて、下段攻撃じゃなかったらガード成功！
										 if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::DOWN_ATTACK) bGuardSuccess = true;
									 }
									 else if (pPerson->GetGuardState() == GUARD_STATE::DOWN_GUARD)
									 {
										 // ★相手がしゃがみガードしてて、上段攻撃じゃなかったらガード成功！
										 if (HitDamageInfo->iAntiGuard != (int)ANTIGUARD_ATTACK::UP_ATTACK) bGuardSuccess = true;
									 }

									 // ★ガード成功してたら！！！
									 if (bGuardSuccess)
									 {
										 // (A列車)ここにガード成功時のSE、エフェクト
										 char *seID;
										 switch (rand() % 3)
										 {
										 case 0:
											 seID = "ガード小";
											 // ガードでゲージ回復（小）
											 pPerson->AddOverDriveGage(0.25f);
											 // 攻撃した相手側は防御側よりも多く獲得
											 pPerson->GetTargetPlayer()->AddOverDriveGage(0.5f);
											 break;
										 case 1:
											 seID = "ガード中";
											 // ガードでゲージ回復（中）
											 pPerson->AddOverDriveGage(0.5f);
											 // 攻撃した相手側は防御側よりも多く獲得
											 pPerson->GetTargetPlayer()->AddOverDriveGage(0.5f);
											 break;
										 case 2:
											 seID = "ガード強";
											 // ガードでゲージ回復（強）
											 pPerson->AddOverDriveGage(1.0f);
											 break;

										 }
										 se->Play(seID);

										 // ガードのけぞり
										 {
											 float fGuardKnockBack(HitDamageInfo->fGuardKnockBackPower);
											 if (pPerson->GetDir() == DIR::RIGHT) fGuardKnockBack *= -1;

											 Vector3 vKnockBackMove(Collision::CheckMove(pPerson, Vector3(fGuardKnockBack, 0, 0)), 0, 0);
											 pPerson->AddMove(vKnockBackMove);

											 if (HitDamageInfo->iAttribute == (int)ATTACK_ATTRIBUTE::STRIKE)
											 {
												 // 壁だとこっちに移動がかかる
												 pPerson->GetTargetPlayer()->AddMove(Vector3(-(fGuardKnockBack - vKnockBackMove.x), 0, 0));
											 }
										 }


										 // コンボ用 ガードしたのを送る
										 COMBO_DESK comboDesk;
										 comboDesk.side = pPerson->GetSide();
										 comboDesk.damage = HitDamageInfo->damage;
										 comboDesk.recoveryFrame = 0;
										 MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_GUARD, &comboDesk);
									 }
									 // ガード失敗
									 else
									 {
										 // ウィークエフェクト発生！
										 Vector2 screenPos;
										 screenPos = Math::WorldToScreen(pPerson->GetCenterPos());
										 pPerson->GetPanelEffectManager()->AddEffect((int)screenPos.x, (int)screenPos.y - 128, PANEL_EFFECT_TYPE::WEAK);

										 return false;	// ダメージ喰らってどうぞ
									 }


									 // ★フィニッシュアーツは通す
									 //if (HitDamageInfo->bOverDrive == true)
									 //{
									 //	 pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::GUARD_BREAK);
									 //	 return false;
									 //}

									 // // 受ける技のレベルによりガードの硬直を変化　（GGX式）
									 // const float len = HitDamageInfo->FlyVector.Length();
									 // int RecoveryFrame = 0;
									 // if (len < HUTTOBI_LINE)
									 // {
									 //	 // レベル1　　（GGX式）
									 //	 RecoveryFrame = 12*2;
									 // }
									 // else
									 // {
									 //	 // レベル2　　（GGX式）
									 //	 RecoveryFrame = 18 * 2;
									 // }

									 // (追加)ガードエフェクト発動!  自分と相手の場所のベクトルを渡す
									 Vector3 vVec= pPerson->GetTargetPlayer()->GetPos() - pPerson->GetPos();
									 vVec.Normalize();
									 pPerson->AddEffectAction(pPerson->GetFlontPos(), EFFECT_TYPE::GUARD_WAVE, vVec);

									 // 技の攻撃に依存　(スマブラ式)
									 //int RecoveryFrame2 = (int)(HitDamageInfo->recoveryFlame *0.3f)+2;

									 // 技ごとに設定されている硬直フレームを設定
									 pPerson->SetRecoveryFrame(HitDamageInfo->GuardRecoveryFrame);

									 // グローバルステートに行かないようにする！
									 return true;
	}

	}
	return false;
}


/*******************************************************/
//					投げステート
/*******************************************************/

void BasePlayerState::Throw::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::THROW);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::THROW);

	// むーぶ0
	pPerson->SetMove(VECTOR_ZERO);

	// SE再生
	se->Play("掴み");
}

void BasePlayerState::Throw::Execute(BasePlayer * pPerson)
{	
	// 投げ終了してたら
	if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// 掴んでたら
	if (pPerson->GetAttackData()->bHit)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::ThrowHold::GetInstance());
	}
}

void BasePlayerState::Throw::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::Throw::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowState");
}

bool BasePlayerState::Throw::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	//	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//	if(pPerson->isActiveFrame()) return true;	// ダメージメッセージガン無視
	//	break;
	//}

	return false;
}


/*******************************************************/
//					投げホールドステート
/*******************************************************/

void BasePlayerState::ThrowHold::Enter(BasePlayer * pPerson)
{
	pPerson->GetTargetPlayer()->GetThrowMark()->Action();

	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);
}

void BasePlayerState::ThrowHold::Execute(BasePlayer * pPerson)
{
	// 投げ猶予終了
	if (pPerson->GetCurrentFrame() > BasePlayer::c_THROW_ESCAPE_FRAME)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::ThrowSuccess::GetInstance());
		return;
	}
}

void BasePlayerState::ThrowHold::Exit(BasePlayer * pPerson)
{
	pPerson->GetTargetPlayer()->GetThrowMark()->Stop();
}

void BasePlayerState::ThrowHold::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowHoldState");
}

bool BasePlayerState::ThrowHold::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		//case MESSAGE_TYPE::THROW_SUCCESS:	// 相手をつかんだよというメッセージ(同時に掴まれた相手にもメッセージが送られている)
		//	pPerson->SetThrowSuccess(true);	// 掴み成功フラグON！
		//	return true;
		//	break;
	case MESSAGE_TYPE::CAN_THROW_RELEASE:	// 掴んでる人から、投げ抜けしてぇと送られてくるメッセージ
											// 猶予フレーム中だったら解除
											// ★★★★★★★★★片方が投げ抜けできて片方が投げ抜けできない。DeviceID関連かな
											//if (pPerson->GetHitStopFrame() > 0)// ※ここは数値をじかにかいたほうがいいかと
	{
		MsgMgr->Dispatch(0, pPerson->GetID(), msg.Sender, MESSAGE_TYPE::THROW_RELEASE, nullptr);	// 送り主に投げ抜けOKと送り返す

																									// ★★★(A列車)今は仮で自分も投げ抜け成功ステートに行く
		pPerson->GetFSM()->ChangeState(SuccessThrowRelease::GetInstance());
	}
	return true;
	break;
	// 攻撃くらったよメッセージ
	//case MESSAGE_TYPE::HIT_DAMAGE:
	//	if(pPerson->GetAttackData) return true;	// ダメージメッセージガン無視
	//	break;
	}

	return false;
}


/*******************************************************/
//					投げ成功ステート
/*******************************************************/

void BasePlayerState::ThrowSuccess::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::THROW_SUCCESS);

	// アクションを成功モードに
	pPerson->SetActionState(BASE_ACTION_STATE::THROW_SUCCESS);

	BasePlayer *you(pPerson->GetTargetPlayer());

	// ★ここでつかまれた相手の座標をつかんだ相手の掴み座標に移動(普通に代入すると壁抜けするので、壁判定もろもろここでやってしまおうという事)
	if (pPerson->GetDir() == DIR::LEFT  && pPerson->isPushInput(PLAYER_COMMAND_BIT::RIGHT) ||
		pPerson->GetDir() == DIR::RIGHT && pPerson->isPushInput(PLAYER_COMMAND_BIT::LEFT))
	{
		// 向きを逆にする
		pPerson->ReverseDir();

		// 相手の座標と自分の座標を入れ替える
		Vector3 temp(pPerson->GetPos());
		pPerson->SetPos(you->GetPos());
		you->SetPos(temp);
	}

	//const Vector3 p(pPerson->GetPos());
	//Vector3 v(pPerson->GetFlontPos() - p);
	//v.y = 0;
	//Vector3 v2(v);
	//
	//// ステージとの判定で、move値をどうこういじった後に
	//if (Collision::CheckMove(pPerson, v) - v.x == 0)
	//{
	//	pPerson->GetPosAddress()->x -= v2.x;
	//	you->SetPos(pPerson->GetPos() + v2);
	//}
	//else you->SetPos(p + v);
}

void BasePlayerState::ThrowSuccess::Execute(BasePlayer * pPerson)
{
	// 投げ終了してたら
	if (!pPerson->isAttackState())
	{
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}
	// ヒットしてる状態
	else if (pPerson->GetAttackData()->bHit)
	{
		/* キャンセルルート */

		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCancel(pPerson)) return;

		//////////////////////////////////////////////
		//	オーバードライブキャンセル
		//============================================
		if (OverDriveCancel(pPerson)) return;
	}
}

void BasePlayerState::ThrowSuccess::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::ThrowSuccess::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowSuccessState");
}

bool BasePlayerState::ThrowSuccess::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 攻撃くらったよメッセージ
	case MESSAGE_TYPE::HIT_DAMAGE:
		return true;	// ダメージメッセージガン無視
		break;
	}

	return false;
}


/*******************************************************/
//					掴まれたステート
/*******************************************************/

void BasePlayerState::ThrowBind::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::KNOCKBACK);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

	// move値0
	pPerson->SetMove(VECTOR_ZERO);

	// 重力とか無視する
	pPerson->SetMoveUpdate(false);

	// ペルソナブレイク
	pPerson->GetStand()->Break();

	// 掴みSEをここで再生
	se->Play("掴み成功");

	// バースト使用不可に
	pPerson->GetTargetPlayer()->ActionNotOverDrive();

	// 投げられた時のエフェクト
	//pPerson->GetThrowMark()->Action();
	pPerson->AddEffectAction(pPerson->GetPos() , EFFECT_TYPE::THROW);

	COMBO_DESK comboDesk;
	comboDesk.side = pPerson->GetSide();
	comboDesk.damage = 0;
	comboDesk.recoveryFrame = BasePlayer::c_THROW_ESCAPE_FRAME;
	comboDesk.bCounter = isDamageCounterStatePrev(pPerson);		// 掴まれる
}

void BasePlayerState::ThrowBind::Execute(BasePlayer * pPerson)
{
	// 相手依存なので自分からかえることはできない。多分ダメージステートに行くと信じて

	// 投げ抜けボタン
	if (pPerson->isPushInputTRG(PLAYER_COMMAND_BIT::A))
	{
		// 掴んでるやつに対して投げ抜けしたいとメッセージを送る(猶予フレーム内なら抜けていいよをメッセージを送り返してくれる)
		if(!isDamageCounterStatePrev(pPerson)) MsgMgr->Dispatch(0, pPerson->GetID(), pPerson->GetTargetPlayer()->GetID(), MESSAGE_TYPE::CAN_THROW_RELEASE, nullptr);
	}
}

void BasePlayerState::ThrowBind::Exit(BasePlayer * pPerson)
{
	// 重力とかの移動量を有効化する
	pPerson->SetMoveUpdate(true);

	// 投げ抜け猶予エフェクト（ビックリマーク）を止める
	//pPerson->GetThrowMark()->Stop();
}

void BasePlayerState::ThrowBind::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "ThrowBindState");
}

bool BasePlayerState::ThrowBind::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 抜けていいよと掴んだ日とか来るラオ繰られて
	case MESSAGE_TYPE::THROW_RELEASE:
		// SE再生
		se->Play("投げ抜け");
		pPerson->GetFSM()->ChangeState(BasePlayerState::SuccessThrowRelease::GetInstance());
		return true;
		break;
	}

	return false;
}


/*******************************************************/
//				掴み解除成功してはじくステート
/*******************************************************/

void BasePlayerState::SuccessThrowRelease::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::THROW_DRAW);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// 初速度の設定
	static const float pow(2.0f);	// 横移動量の力
	pPerson->SetMove(Vector3((pPerson->GetDir() == DIR::LEFT) ? pow : -pow, 0, 0));	// 向いてる方と反対方向に移動する
}

void BasePlayerState::SuccessThrowRelease::Execute(BasePlayer * pPerson)
{
	// 投げ抜けパシン終了フレーム
	if (pPerson->GetCurrentFrame() >= BasePlayer::c_THROW_RELEASE_FRAME)
	{
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::SuccessThrowRelease::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::SuccessThrowRelease::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "SuccessThrowReleaseState");
}

bool BasePlayerState::SuccessThrowRelease::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}

/*******************************************************/
//				ヒーホードライブ
/*******************************************************/

void BasePlayerState::HeavehoDrive::Enter(BasePlayer * pPerson)
{
	// ヒーホー用変数初期化
	pPerson->HeavehoDriveInit();

	// 背景変更
	bool bDriveStage(true);
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::OVER_DRIVE_STAGE, &bDriveStage);

	// SE
	se->Play("必殺発動");

	// 必殺始動エフェクト
	pPerson->AddEffectAction(pPerson->GetCenterPos(),EFFECT_TYPE::OVER_DRIVE_ACTION);

	// ★必殺はフォローまで無敵
	pPerson->SetInvincible(114514, 1);
}

void BasePlayerState::HeavehoDrive::Execute(BasePlayer * pPerson)
{
	// そのキャラクターのヒーホーの更新
	const bool bEnd(pPerson->HeavehoDriveUpdate());

	// 終わったら
	if (bEnd)
	{
		// 待機ステートに(さすがにキャンセルはやめておこう)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// フォローになったら無敵解除
	if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW) pPerson->InvincibleOff();
}

void BasePlayerState::HeavehoDrive::Exit(BasePlayer * pPerson)
{
	// 一応無敵を解除する
    pPerson->InvincibleOff();

	// ヒーホーの終わり
	pPerson->HeavehoDriveExit();

	// 相手のバースト使用可能に
	pPerson->GetTargetPlayer()->StopNotOverDrive();

	// 背景変更
	bool bDriveStage(false);
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::OVER_DRIVE_STAGE, &bDriveStage);
}

void BasePlayerState::HeavehoDrive::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDrive");
}

bool BasePlayerState::HeavehoDrive::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::HIT_ATTACK:
		pPerson->HeavehoDriveHitEvent();
		break;
	}

	return false;
}

/*******************************************************/
//				超必殺技ステート
/*******************************************************/

void BasePlayerState::HeavehoDriveOverFlow::Enter(BasePlayer * pPerson)
{
	// モーションに変える
	pPerson->SetMotion(MOTION_TYPE::HEAVEHO_DRIVE_OVERFLOW);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::HEAVEHO_DRIVE_OVERFLOW);

	// ヒーホードライブオーバーフロー用変数初期化
	pPerson->HeavehoDriveOverFlowInit();

	// SE
	se->Play("超必殺発動");
}

void BasePlayerState::HeavehoDriveOverFlow::Execute(BasePlayer * pPerson)
{
	// そのキャラクターのヒーホーの更新
	pPerson->HeavehoDriveOverFlowUpdate();

	// 攻撃ステート終わったら
	if (!pPerson->isAttackState())
	{
		// 待機ステートに(さすがにキャンセルはやめておこう)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HandleMessageに移植
	//static bool flag = false;
	//// ヒットしてたら成功・演出ステートに行く
	//if (pPerson->GetAttackData()->bHit)
	//{
	//	if (flag == false)
	//	{
	//		flag = true;
	//		// カットイン発動メッセージを送る
	//		CUTIN_TYPE_NAME data = CUTIN_TYPE_NAME::AIROU;
	//		MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CUTIN_MGR, MESSAGE_TYPE::CUTIN_ACTION, &data);
	//
	//		// シーンメインステートに対して曲変更メッセージ
	//		MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START, nullptr);
	//		return;
	//	}
	//}
}

void BasePlayerState::HeavehoDriveOverFlow::Exit(BasePlayer * pPerson)
{
	// ヒーホーの終わり
	pPerson->HeavehoDriveExit();
	pPerson->SetOverDriveFrame(0);
}

void BasePlayerState::HeavehoDriveOverFlow::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDriveOverFlow");
}

bool BasePlayerState::HeavehoDriveOverFlow::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
		// 攻撃当たってたら
	case MESSAGE_TYPE::HIT_ATTACK:
	{
									 pPerson->HeavehoDriveOverFlowSuccessInit();	// 必殺のターゲットとなる相手の実体を初期化のときに突っ込む
									 // カットイン発動メッセージを送る
									 CUTIN_TYPE_NAME data = CUTIN_TYPE_NAME::AIROU;
									 MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CUTIN_MGR, MESSAGE_TYPE::CUTIN_ACTION, &data);

									 // シーンメインステートに対して曲変更メッセージ
									 MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::HEAVE_HO_OVERFLOW_START, nullptr);

									 // return trueはしない。SPゲージ関連の処理をGlobalStateのメッセージを行っているため
									 break;
	}
	}
	return false;
}

/*******************************************************/
//				超必殺技_成功ステート
/*******************************************************/

void BasePlayerState::HeavehoDriveOverFlow_Success::Enter(BasePlayer * pPerson)
{
	// シーンメインに成功したことを伝える
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT, nullptr);

	// プレイヤーマネージャーにも伝える
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::PLAYER_MGR, MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT, nullptr);

	// カメラにエフェクトカメラの発動をお願いする
	EFFECT_CAMERA_INFO EffectCameraInfo;
	EffectCameraInfo.scriptID = (int)pPerson->GetOverFlowCameraID();
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::EFFECT_CAMERA, &EffectCameraInfo);

	// オブジェクト変更
	pPerson->ChangeHeaveHoDriveOverFlowObj();

	// アングル固定
	pPerson->SetAngleY(PI);

	// 原点に置く
	pPerson->SetPos(VECTOR_ZERO);
}

void BasePlayerState::HeavehoDriveOverFlow_Success::Execute(BasePlayer * pPerson)
{
	// 原点に置く
	pPerson->SetPos(VECTOR_ZERO);

	pPerson->HeavehoDriveOverFlowSuccessUpdate();
}

void BasePlayerState::HeavehoDriveOverFlow_Success::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::HeavehoDriveOverFlow_Success::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "HeavehoDriveOverFlow_Success");
}

bool BasePlayerState::HeavehoDriveOverFlow_Success::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}

/*******************************************************/
//				WINステート
/*******************************************************/

void BasePlayerState::Win::Enter(BasePlayer * pPerson)
{
	// UIにメッセージを送る
	SIDE side(pPerson->GetSide());
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::UI_MGR, MESSAGE_TYPE::APP_WIN_ICON, &side);

	// カメラにメッセージを送る
	MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::WINNER_CAMERA, pPerson->GetPosAddress());

	// 何もしないアクション
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// 勝ちモーション
	pPerson->SetMotion(MOTION_TYPE::WIN);

	// 勝ってる数カウント
	pPerson->AddWinNum(1);

	// 正面を向く
	pPerson->SetAngleY(PI);
}

void BasePlayerState::Win::Execute(BasePlayer * pPerson)
{
	if (pPerson->GetCurrentFrame() == BasePlayer::c_WINNER_TIME || pPerson->isAnyPushKey())
	{
		// シーンメインに終わったよメッセージ
		MsgMgr->Dispatch(0, pPerson->GetID(), ENTITY_ID::SCENE_MAIN, MESSAGE_TYPE::END_WINNER, nullptr);
	}
}

void BasePlayerState::Win::Exit(BasePlayer * pPerson)
{}

void BasePlayerState::Win::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "Win");
}

bool BasePlayerState::Win::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}

/*******************************************************/
//				空中ダッシュステート
/*******************************************************/

void BasePlayerState::AerialDash::Enter(BasePlayer * pPerson)
{
	// とりあえずフレーム数えてみる
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// ダッシュモーション
	pPerson->SetMotion(MOTION_TYPE::AERIAL_DASH);

	// SE
	se->Play("空中ダッシュ");

	// 空中ジャンプの権利喪失
	pPerson->SetAerialJump(false);

	// 空中ダッシュフラグ
	pPerson->SetAerialDash(true);
	// タイマー
	pPerson->SetAerialDashFrame(16);

	const float fMoveY(pPerson->GetMove().y);
	const float fDashSpeed(pPerson->GetAerialDashSpeed());

	// 移動
	if (pPerson->GetDir() == DIR::RIGHT)
	{
		// わっしょい
		pPerson->SetMove(Vector3(fDashSpeed, 0, 0));
	}
	else
	{
		// わっしょい
		pPerson->SetMove(Vector3(-fDashSpeed, 0, 0));
	}

	if (fMoveY > 0)pPerson->AddMove(Vector3(0, fMoveY, 0));

	// 空中ダッシュエフェクト
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::AERIAL_JUMP);

}

void BasePlayerState::AerialDash::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	空中攻撃キャンセル
	//============================================
	if (AerialAttackCancel(pPerson)) return;

	if (pPerson->GetAerialDashFrame() <= 0)
	{
		// 地上、空中で分岐
		if (pPerson->isLand())pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		else pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
	}

	//// 移動地
	//// 移動
	//if (pPerson->GetDir() == DIR::RIGHT)
	//{
	//	// わっしょい
	//	pPerson->SetMove(Vector3(1.5f, 0, 0));
	//}
	//else
	//{
	//	// わっしょい
	//	pPerson->SetMove(Vector3(-1.5f, 0, 0));
	//}

}

void BasePlayerState::AerialDash::Exit(BasePlayer * pPerson)
{
	// 空中ダッシュフラグオフ
	//pPerson->SetAerialDash(false);

}

void BasePlayerState::AerialDash::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "AerialDash");
}

bool BasePlayerState::AerialDash::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}

/*******************************************************/
//				空中バックダッシュステート
/*******************************************************/

void BasePlayerState::AerialBackDash::Enter(BasePlayer * pPerson)
{
	// ダッシュモーション
	pPerson->SetMotion(MOTION_TYPE::BACK_STEP);

	// 空中ジャンプの権利喪失
	pPerson->SetAerialJump(false);

	// SE
	se->Play("空中ダッシュ");

	// とりあえずフレーム数えてみる
	pPerson->SetActionState(BASE_ACTION_STATE::FRAMECOUNT);

	// 空中ダッシュフラグ
	pPerson->SetAerialDash(true);
	// タイマー
	pPerson->SetAerialDashFrame(14);

	const float MoveY(pPerson->GetMove().y);

	// 移動
	if (pPerson->GetDir() == DIR::RIGHT)
	{
		// わっしょい
		pPerson->SetMove(Vector3(-.75f, 0, 0));
	}
	else
	{
		// わっしょい
		pPerson->SetMove(Vector3(.75f, 0, 0));
	}

	if (MoveY > 0)pPerson->AddMove(Vector3(0, MoveY, 0));

	// 空中ダッシュエフェクト
	pPerson->AddEffectAction(pPerson->GetCenterPos(), EFFECT_TYPE::AERIAL_JUMP);
}

void BasePlayerState::AerialBackDash::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	空中攻撃キャンセル
	//============================================
	//if (AerialAttackCancel(pPerson)) return;

	if (pPerson->GetAerialDashFrame() <= 0)
	{
		// 地上、空中で分岐
		if(pPerson->isLand())pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		else 
			pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());
	}

	//// 移動地
	//// 移動
	//if (pPerson->GetDir() == DIR::RIGHT)
	//{
	//	// わっしょい
	//	pPerson->SetMove(Vector3(1.5f, 0, 0));
	//}
	//else
	//{
	//	// わっしょい
	//	pPerson->SetMove(Vector3(-1.5f, 0, 0));
	//}

}

void BasePlayerState::AerialBackDash::Exit(BasePlayer * pPerson)
{
	// 空中ダッシュフラグオフ
	//pPerson->SetAerialDash(false);

}

void BasePlayerState::AerialBackDash::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "AerialBackDash");
}

bool BasePlayerState::AerialBackDash::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	return false;
}


/*******************************************************/
//				イントロ演出用ステート
/*******************************************************/

void BasePlayerState::Intro::Enter(BasePlayer * pPerson)
{
	// イントロ用モーション
	pPerson->SetMotion(MOTION_TYPE::APPEAR);

	// キャラごとの掛け合いの初期化
	pPerson->KakeaiInit();
}

void BasePlayerState::Intro::Execute(BasePlayer * pPerson)
{
	// キャラごとの掛け合いの更新
	pPerson->KakeaiUpdate();
}

void BasePlayerState::Intro::Exit(BasePlayer * pPerson)
{
	// キャラごとの掛け合いの終了
	pPerson->KakeaiExit();
}

void BasePlayerState::Intro::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 690, 0xffffffff, "Intro");
}

bool BasePlayerState::Intro::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::KAKEAI_END:
		pPerson->GetFSM()->ChangeState(Wait::GetInstance());
		return true;
		break;
	}
	return false;
}

/*******************************************************/
//				どっこい攻撃 (三段目)
/*******************************************************/

//void BasePlayerState::StrongAttack::Enter(BasePlayer * pPerson)
//{
//	//　モーションセット
//	pPerson->SetMotion(MOTION_TYPE::RUSH_ATTACK3);
//
//	// 攻撃ステートを3段目に設定する
//	pPerson->SetActionState(BASE_ACTION_STATE::RUSH3);
//
//
//
//
//}
//
//void BasePlayerState::StrongAttack::Execute(BasePlayer * pPerson)
//{
//
//	// 攻撃終了してたら
//	if (!pPerson->isAttackState())
//	{
//		// 待機モーションに戻る
//		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		return;
//	}
//
//	// ヒットしてる状態
//	if (pPerson->GetAttackData()->bHit)
//	{
//		/* キャンセルルート */
//		//////////////////////////////////////////////
//		//	スキルキャンセル
//		//============================================
//		if (SkillCancel(pPerson)) return;
//
//		//////////////////////////////////////////////
//		//	スタンドキャンセル
//		//============================================
//		if (StandCancel(pPerson)) return;
//
//		//////////////////////////////////////////////
//		//	ヒーホードライブキャンセル
//		//============================================
//		if (HeaveHoCancel(pPerson)) return;
//
//		//////////////////////////////////////////////
//		//	ヒーホードライブオーバーフローキャンセル
//		//============================================
//		if (HeaveHoOverFlowCancel(pPerson)) return;
//
//		// 攻撃終了してたら
//		if (!pPerson->isAttackState())
//		{
//			// 待機モーションに戻る
//			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
//		}
//	}
//
//}
//
//void BasePlayerState::StrongAttack::Exit(BasePlayer * pPerson)
//{
//	// 空中ダッシュフラグオフ
//	//pPerson->SetAerialDash(false);
//
//}
//
//void BasePlayerState::StrongAttack::Render(BasePlayer * pPerson)
//{
//	tdnText::Draw(20, 690, 0xffffffff, "StrongAttack");
//}
//
//bool BasePlayerState::StrongAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
//{
//	// メッセージタイプ
//	return false;
//}
