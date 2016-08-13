#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Stand/Stand.h"
#include "../Sound/SoundManager.h"
#include "../Effect/Particle.h"

// 定数
static const float HUTTOBI_LINE = 3.0f;
static const float REPEAT_ATTACK_RATE = 0.85f;		// 同じ攻撃当たるなレート

/*******************************************************/
//	ファンクション
/*******************************************************/
bool JumpCansel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプボタンでジャンプキャンセル
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// ジャンプステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
		return true;
	}
	else return false;
}

bool RunCansel(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	左右キーで走りキャンセル
	//============================================
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		return true;
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		return true;
	}
	else return false;
}

bool SquatCansel(BasePlayer * pPerson)
{
	// しゃがみキャンセル
	if (pPerson->isPushInput(PLAYER_INPUT::DOWN))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Squat::GetInstance());
		return true;
	}
	else return false;
}

bool StandCansel(BasePlayer * pPerson)
{
	// スタンドキャンセル
	if (pPerson->isPushInput(PLAYER_INPUT::L1))
	{
		// 追記　スタンドストックがないときはキャンセルできない
		if (pPerson->GetStand()->GetStandStock() <= 0)return false;
		pPerson->GetFSM()->ChangeState(BasePlayerState::StandAction::GetInstance());
		return true;
	}
	else return false;
}

bool RushAttackCansel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::RushAttack::GetInstance());
		return true;
	}
	else return false;
}

bool FinishAttackCansel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (pPerson->isPushInput(PLAYER_INPUT::R2))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::FinishAttack::GetInstance());
		return true;
	}
	else return false;
}

bool SkillCansel(BasePlayer *pPerson)
{
	// 攻撃キャンセル
	if (pPerson->isPushInput(PLAYER_INPUT::D))
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Skill::GetInstance());
		return true;
	}
	else return false;
}

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
	tdnText::Draw(20, 610, 0xffffffff, "ぐろーばるすてーと");
}

bool BasePlayerState::Global::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	
		// 攻撃くらったよメッセージ
	case MESSAGE_TYPE::HIT_DAMAGE:
	{
		// アクションステートオフ(つまり現在までやったいた動きが消える)
		pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);

		HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る

		// 吹っ飛びベクトルを加算 (A列車)加算から代入に変更
		//pPerson->AddMove(Vector3(hdi->LandFlyVector.x, hdi->LandFlyVector.y, 0));
		pPerson->SetMove(Vector3(hdi->FlyVector.x, hdi->FlyVector.y, 0));

		// ヒットストップ
		pPerson->SetHitStopFrame(hdi->hitStopFlame);

		// 硬直フレーム設定
		int RecoveryFrame = hdi->recoveryFlame;
		for (auto it : *pPerson->GetRecoveryDamageCount())
		{
			if (it == (BASE_ACTION_STATE)hdi->iAttackType)
			{
				RecoveryFrame = (int)(RecoveryFrame * REPEAT_ATTACK_RATE);	// 同技補正
			}
		}
		pPerson->SetRecoveryFrame(RecoveryFrame);

		// くらった攻撃リスト追加
		pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);

		// (追加)ヒットエフェクト発動! 
		// (A列車)エフェクトを今は自分のポジションにおいてる！！！　後でヒットポジションとかいるかも
		pPerson->AddEffectAction(pPerson->GetPos()+Vector3(0,4,-3),(EFFECT_TYPE)hdi->HitEffectType);

		// 空中ジャンプの権利復活
		pPerson->SetAerialJump(true);


		// 吹っ飛び距離	に　応じて	ダメージステートを変える
		const float len = hdi->FlyVector.Length();
		if (len <  HUTTOBI_LINE)
		{
			// ここで空中でくらってるか地上でくらってるかを分ける
			if (pPerson->isLand() == true)
			{
				// 地上ノックバックに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::KnockBack::GetInstance());
			}
			else
			{
				// 空中ノックバックに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialKnockBack::GetInstance());
			}
		
		}
		else
		{
			// ノックダウンに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::KnockDown::GetInstance());

		}

		return true;
		break;
	}

	// 攻撃当たったよメッセージ
	case MESSAGE_TYPE::HIT_ATTACK:
	{
		HIT_ATTACK_INFO *HitAttackInfo = (HIT_ATTACK_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
		pPerson->SetHitStopFrame(HitAttackInfo->hitStopFlame);// ヒットストップ
		pPerson->AddCollectScore(HitAttackInfo->HitScore);	// 実体前のスコアを加算
		if (HitAttackInfo->bFinishAttack) pPerson->ConversionScore();	// フィニッシュアタックならスコア生産
		break;
	}


	// (A列車)死んだぜメッセージ
	}

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
	//	スキルキャンセル
	//============================================
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	{
		// エスケープステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
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
	//	フィニッシュ攻撃ボタン
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCansel(pPerson)) return;


	//////////////////////////////////////////////
	//	移動ボタン
	//============================================
	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
		// 歩きステートにいく
		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
	}
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
		// 歩きステートにいく
		pPerson->GetFSM()->ChangeState(BasePlayerState::Walk::GetInstance());
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

BasePlayerState::Walk * BasePlayerState::Walk::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Walk instance;
	return &instance;
}

void BasePlayerState::Walk::Enter(BasePlayer * pPerson)
{
	// 歩きモーションに変える
	pPerson->SetMotion(1);

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
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	{
		// エスケープステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (RushAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCansel(pPerson)) return;

	if (pPerson->GetInputList(PLAYER_INPUT::RIGHT) == 3)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		pPerson->SetDir(DIR::RIGHT);
		return;
	}

	if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		pPerson->SetDir(DIR::LEFT);
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

	// 移動地制御
	pPerson->MoveClampX(pPerson->GetMaxSpeed() * .7f);
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

	// SE再生
	se->Play("ダッシュ");
}

void BasePlayerState::Run::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	スキルキャンセル
	//============================================
	if (SkillCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	エスケープボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::R1) == 3)
	{
		// エスケープステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::Escape::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	攻撃キャンセル
	//============================================
	if (RushAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	しゃがみキャンセル
	//============================================
	if (SquatCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCansel(pPerson)) return;

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
	// 押した瞬間 理論上　★二回連続押すことはありえない　ニュートラルを挟む
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
		//if (abs(move.x) < BasePlayer::c_FRONT_BRAKE_LINE)
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

BasePlayerState::FrontBrake * BasePlayerState::FrontBrake::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::FrontBrake instance;
	return &instance;
}

void BasePlayerState::FrontBrake::Enter(BasePlayer * pPerson)
{
	// 前ブレーキモーションに変える
	pPerson->SetMotion(18);

	// SE再生
	se->Play("ブレーキ1");
}

void BasePlayerState::FrontBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCansel(pPerson)) return;

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
	// Uターンブレーキモーションに変える
	pPerson->SetMotion(18);

	// SE再生
	se->Play("ブレーキ1");
}

void BasePlayerState::TurnOverBrake::Execute(BasePlayer * pPerson)
{
	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCansel(pPerson)) return;

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
	// ジャンプモーションに変える
	pPerson->SetMotion(10);

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
		if (pPerson->isPushInput(PLAYER_INPUT::C))
		{
			// プレイヤーが押してる時間計測
			pPerson->GetJump()->PlayerHoldTimer++;
		}

		if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
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
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))		add.x = -.25f;
			//else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT)) add.x = .25f;

			// 移動地を足す！
			pPerson->AddMove(add);

			// 地上フラグオフ！
			//pPerson->SetLand(false);

			// しゃがみフラグ終了
			pPerson->GetJump()->bHold = false;

			// しゃがんでる最中に攻撃ボタンを押してたら
			if (pPerson->GetJump()->bAttackPush)
			{
				if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
				{
					// 空中下攻撃ステートに行く
					pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
				}
				else
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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	攻撃ボタン
		//============================================
	
		if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
		{
			if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
			{
				// 空中下攻撃ステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
			}
			else
			{
				// 空中攻撃ステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
			}
		
			return;
		}

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (FinishAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	空中ジャンプボタン
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{
			// 空中ジャンプの権利がなかったら弾く
			if (pPerson->isAerialJump() == true)
			{
				// 空中ジャンプステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			}
		}

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
		pPerson->MoveClampX(pPerson->GetMaxSpeed() * 0.8f);
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

BasePlayerState::AerialJump * BasePlayerState::AerialJump::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::AerialJump instance;
	return &instance;
}

void BasePlayerState::AerialJump::Enter(BasePlayer * pPerson)
{
	// ジャンプモーションに変える
	pPerson->SetMotion(13);

	// 空中ジャンプの権利を使用
	pPerson->SetAerialJump(false);

	// 移動地を足す！
	Vector3 set(pPerson->GetMove() * .5f);//c_MAX_AerialJump
	if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		set.x -= .05f;
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		set.x += .05f;
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
	if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	攻撃ボタン
		//============================================

		if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
		{
			if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
			{
				// 空中下攻撃ステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
			}
			else
			{
				// 空中攻撃ステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
			}

			return;
		}

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (FinishAttackCansel(pPerson)) return;

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
		pPerson->MoveClampX(pPerson->GetMaxSpeed() * 0.8f);


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

BasePlayerState::Fall * BasePlayerState::Fall::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Fall instance;
	return &instance;
}

void BasePlayerState::Fall::Enter(BasePlayer * pPerson)
{
	// 落下モーションに変える
	pPerson->SetMotion(13);

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
	//	空中ジャンプボタン
	//============================================
	if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
	{
		// 空中ジャンプの権利がなかったら弾く
		if (pPerson->isAerialJump() == true)
		{
			// 空中ジャンプステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
		}
	}

	//////////////////////////////////////////////
	//	攻撃ボタン
	//============================================

	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == true)
		{
			// 空中下攻撃ステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialDropAttack::GetInstance());
		}
		else
		{
			// 空中攻撃ステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::AerialAttack::GetInstance());
		}

		return;
	}

	//////////////////////////////////////////////
	//	フィニッシュ攻撃ボタン
	//============================================
	if (FinishAttackCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCansel(pPerson)) return;

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

BasePlayerState::Land * BasePlayerState::Land::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Land instance;
	return &instance;
}

void BasePlayerState::Land::Enter(BasePlayer * pPerson)
{
	// 着地モーションに変える
	pPerson->SetMotion(13);

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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	ジャンプキャンセル
		//============================================
		if (JumpCansel(pPerson))
			return;

		//////////////////////////////////////////////
		//	攻撃キャンセル
		//============================================
		if (RushAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	フィニッシュ攻撃ボタン
		//============================================
		if (FinishAttackCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	しゃがみキャンセル
		//============================================
		if (SquatCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	はしりキャンセル
		//============================================
		if (RunCansel(pPerson)) return;
	}

	// 一定時間着地したら
	if (++pPerson->GetJump()->LandTimer > 12)
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
	switch (pPerson->GetRushAttack()->step)
	{
	case 0:	// 1段目
			// 攻撃終了してたら
		if (!pPerson->isAttackState())
		{
			// 待機モーションに戻る
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

		// ヒットしてる状態
		else if (pPerson->GetAttackData()->bHit)
		{
			/* キャンセルルート */
			
			//////////////////////////////////////////////
			//	スキルキャンセル
			//============================================
			if (SkillCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	スタンドキャンセル
			//============================================
			if (StandCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	しゃがみキャンセル
			//============================================
			if (SquatCansel(pPerson)) return;

			// フォローモーション(次の攻撃へ)
			if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				// 攻撃ボタン押したら(押しっぱなし込み)
				if (pPerson->isPushInput(PLAYER_INPUT::B))
				{
					// 次のモーションセット
					pPerson->SetMotion(4);

					// 攻撃ステートを2段目に設定する
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH2);

					pPerson->GetRushAttack()->step++;
					//pPerson->GetRushAttack()->bNextOK = false;

					const float pow = 0.5f;
					pPerson->AddMove((pPerson->GetDir() == DIR::RIGHT) ? Vector3(pow, 0, 0) : Vector3(-pow, 0, 0));
				}
			}
		}

		break;
	case 1:	// 2段目
		// 攻撃終了してたら
		if (!pPerson->isAttackState())
		{
			// 待機モーションに戻る
			pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		}

		// ヒットしてる状態
		else if (pPerson->GetAttackData()->bHit)
		{
			/* キャンセルルート */

			//////////////////////////////////////////////
			//	スタンドキャンセル
			//============================================
			if (StandCansel(pPerson)) return;

			//////////////////////////////////////////////
			//	しゃがみキャンセル
			//============================================
			if (SquatCansel(pPerson)) return;

			// フォローモーション(次の攻撃へ)
			if (pPerson->GetActionFrame() == FRAME_STATE::FOLLOW)
			{
				// 攻撃ボタン押したら(押しっぱなし込み)
				if (pPerson->isPushInput(PLAYER_INPUT::B))
				{
					// 次のモーションセット
					pPerson->SetMotion(6);

					// 攻撃ステートを3段目に設定する
					pPerson->SetActionState(BASE_ACTION_STATE::RUSH3);

					pPerson->GetRushAttack()->step++;
					//pPerson->GetRushAttack()->bNextOK = false;
				}
			}
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
	// 攻撃ステートをやめさせる
	//pPerson->SetActionState(BASE_ACTION_STATE::NO_ACTION);
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

BasePlayerState::KnockBack * BasePlayerState::KnockBack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::KnockBack instance;
	return &instance;
}


void BasePlayerState::KnockBack::Enter(BasePlayer * pPerson)
{
	// ダメージモーションに変える
	pPerson->SetMotion(16);
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
		
		// [追記]　無敵はここでつけない！
		// ★無敵時間設定
		//pPerson->SetInvincible(8);
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
	//								 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//								 // 喰らってる状態でくらった攻撃リスト追加
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);
	//								 // trueは返さない。ここでは、ダウン状態でのくらったカウントを取るだけなので。
	//}
	//}
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					エアリアルノックバックステート
/*******************************************************/

BasePlayerState::AerialKnockBack * BasePlayerState::AerialKnockBack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::AerialKnockBack instance;
	return &instance;
}

void BasePlayerState::AerialKnockBack::Enter(BasePlayer * pPerson)
{
	// 空中ノックバックモーションに変える
	pPerson->SetMotion(17);

}

void BasePlayerState::AerialKnockBack::Execute(BasePlayer * pPerson)
{
	// 硬直が0以下なら硬直終了
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		// 硬直が0になったら　なおかついずれかのボタンを押していたら復帰ステートへ
		if (pPerson->isPushInput(PLAYER_INPUT::A)||
			pPerson->isPushInput(PLAYER_INPUT::B)||
			pPerson->isPushInput(PLAYER_INPUT::C)||
			pPerson->isPushInput(PLAYER_INPUT::D))
		{

			// 地上オア空中
			if (pPerson->isLand() == true)
			{
				// 地上リカバリーステートへ！
				pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());
				// 喰らったカウントリセット
				pPerson->GetRecoveryDamageCount()->clear();
			}
			else
			{
				// 空中リカバリーステートへ！
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialRecovery::GetInstance());
				// 喰らったカウントリセット
				pPerson->GetRecoveryDamageCount()->clear();
			}

		}
	}
}

void BasePlayerState::AerialKnockBack::Exit(BasePlayer * pPerson)
{

}

void BasePlayerState::AerialKnockBack::Render(BasePlayer * pPerson)
{
	tdnText::Draw(20, 650, 0xffffffff, "選りえるノックバック");
	//tdnText::Draw(20, 690, 0xffffffff, "RushState");
}

bool BasePlayerState::AerialKnockBack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
		//{
		//	// 攻撃くらったよメッセージ
		//case MESSAGE_TYPE::HIT_DAMAGE:
		//{
		//								 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
		//								 // 喰らってる状態でくらった攻撃リスト追加
		//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);
		//								 // trueは返さない。ここでは、ダウン状態でのくらったカウントを取るだけなので。
		//}
		//}
		// Flaseで返すとグローバルステートのOnMessageの処理へ行く
		return false;
}



/*******************************************************/
//					ノックダウンステート
/*******************************************************/

BasePlayerState::KnockDown * BasePlayerState::KnockDown::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::KnockDown instance;
	return &instance;
}

void BasePlayerState::KnockDown::Enter(BasePlayer * pPerson)
{
	// ノックダウンモーションに変える
	pPerson->SetMotion(18);

}

void BasePlayerState::KnockDown::Execute(BasePlayer * pPerson)
{
	if (pPerson->isLand() == false)
	{
		// 吹っ飛びけむりパーティクル
		ParticleManager::EffectFlySmoke(pPerson->GetPos());
		return;
	}

	// 硬直が0以下なら硬直終了
	if (pPerson->GetRecoveryFrame() <= 0)
	{
		if (pPerson->isPushInput(PLAYER_INPUT::A) ||
			pPerson->isPushInput(PLAYER_INPUT::B) ||
			pPerson->isPushInput(PLAYER_INPUT::C) ||
			pPerson->isPushInput(PLAYER_INPUT::D))
		{
			// 待機ステートに戻る
			pPerson->GetFSM()->ChangeState(BasePlayerState::LandRecovery::GetInstance());

			// 喰らったカウントリセット
			pPerson->GetRecoveryDamageCount()->clear();
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
	//								 HIT_DAMAGE_INFO *hdi = (HIT_DAMAGE_INFO*)msg.ExtraInfo;		// オリジナル情報構造体受け取る
	//								 // 喰らってる状態でくらった攻撃リスト追加
	//								 pPerson->GetRecoveryDamageCount()->push_back((BASE_ACTION_STATE)hdi->iAttackType);
	//								 // trueは返さない。ここでは、ダウン状態でのくらったカウントを取るだけなので。
	//}
	//}
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/*******************************************************/
//					地上リカバリーステート
/*******************************************************/

BasePlayerState::LandRecovery * BasePlayerState::LandRecovery::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::LandRecovery instance;
	return &instance;
}

void BasePlayerState::LandRecovery::Enter(BasePlayer * pPerson)
{
	// リカバリーに変える
	pPerson->SetMotion(13);

	// りかバリーかうんとを初期化
	pPerson->SetRecoveryCount(0);

	// リカバー中は無敵！！！
	pPerson->SetInvincible(1);

	// エフェクト発動」
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3), EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::LandRecovery::Execute(BasePlayer * pPerson)
{
	pPerson->AddRecoveryCount(1);

	// リカバリーフレームを超えたら終わり！
	if (pPerson->GetRecoveryCount() >= BasePlayer::c_RECOVERY_FLAME)
	{
		// 待機ステートへ
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());

		// 喰らったカウントリセット
		pPerson->GetRecoveryDamageCount()->clear();
	}
}

void BasePlayerState::LandRecovery::Exit(BasePlayer * pPerson)
{
	// 無敵解除！！
	pPerson->InvincibleOff();

	// りかバリーかうんとを初期化
	pPerson->SetRecoveryCount(0);
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

BasePlayerState::AerialRecovery * BasePlayerState::AerialRecovery::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::AerialRecovery instance;
	return &instance;
}

void BasePlayerState::AerialRecovery::Enter(BasePlayer * pPerson)
{
	// リカバリーに変える
	pPerson->SetMotion(13);

	// りかバリーかうんとを初期化
	pPerson->SetRecoveryCount(0);

	// リカバー中は無敵！！！
	pPerson->SetInvincible(1);

	Vector3 move;
	move = pPerson->GetMove()*0.25f;
	pPerson->SetMove(move);

	// ＋Ｍｏｖｅ
	Vector3 addMove = Vector3(0, 2.55f, 0);

	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		addMove += Vector3(0.7f, 0, 0);
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		addMove += Vector3(-0.7f, 0, 0);
	}
	
	pPerson->AddMove(addMove);

	// エフェクト発動」
	pPerson->AddEffectAction(pPerson->GetPos() + Vector3(0, 5, -3) , EFFECT_TYPE::RECOVERY);
}

void BasePlayerState::AerialRecovery::Execute(BasePlayer * pPerson)
{
	pPerson->AddRecoveryCount(1);

	// リカバリーフレームを超えたら終わり！
	if (pPerson->GetRecoveryCount() >= BasePlayer::c_RECOVERY_FLAME)
	{
		// 空中なので落ちるステートに
		pPerson->GetFSM()->ChangeState(BasePlayerState::Fall::GetInstance());

		// 喰らったカウントリセット
		pPerson->GetRecoveryDamageCount()->clear();
	}
}

void BasePlayerState::AerialRecovery::Exit(BasePlayer * pPerson)
{
	// 無敵解除！！
	pPerson->InvincibleOff();

	pPerson->SetRecoveryCount(0);
}

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

BasePlayerState::Squat * BasePlayerState::Squat::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Squat instance;
	return &instance;
}

void BasePlayerState::Squat::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(8);

}

void BasePlayerState::Squat::Execute(BasePlayer * pPerson)
{
	//	しゃがみ攻撃ボタン
	if (pPerson->GetInputList(PLAYER_INPUT::B) == 3)
	{
		// しゃがみ攻撃ステートに行く
		pPerson->GetFSM()->ChangeState(BasePlayerState::SquatAttack::GetInstance());
		return;
	}

	//////////////////////////////////////////////
	//	スタンドキャンセル
	//============================================
	if (StandCansel(pPerson)) return;

	//////////////////////////////////////////////
	//	ジャンプキャンセル
	//============================================
	if (JumpCansel(pPerson)) return;

	// しゃがみボタンを離したら待機に戻る
	if (pPerson->isPushInput(PLAYER_INPUT::DOWN) == false)
	{
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}

}

void BasePlayerState::Squat::Exit(BasePlayer * pPerson)
{


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

BasePlayerState::SquatAttack * BasePlayerState::SquatAttack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::SquatAttack instance;
	return &instance;
}

void BasePlayerState::SquatAttack::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(9);


	// ★攻撃ステートを2段目に設定する　↓でHITフラグを消している
	pPerson->SetActionState(BASE_ACTION_STATE::SQUAT);

}

void BasePlayerState::SquatAttack::Execute(BasePlayer * pPerson)
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
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCansel(pPerson)) return;

		//============================================
		//	ジャンプボタン
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{

			if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
			{
				pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));		
			}
			if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
			{
				pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
			}

			// 高く飛ばす 
			pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			// ジャンプステートに行く
			pPerson->GetFSM()->ChangeState(BasePlayerState::Jump::GetInstance());
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
//					空中攻撃ステート
/*******************************************************/

BasePlayerState::AerialAttack * BasePlayerState::AerialAttack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::AerialAttack instance;
	return &instance;
}

void BasePlayerState::AerialAttack::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(11);


	// ★攻撃ステートを設定する　↓でHITフラグを消している
	pPerson->SetActionState(BASE_ACTION_STATE::AERIAL);


	// 入力方向に向く
	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
	}
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
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HITしていたらキャンセルできる
	if (pPerson->GetAttackData()->bHit == true)
	{
		//////////////////////////////////////////////
		//	スタンドキャンセル
		//============================================
		if (StandCansel(pPerson)) return;

		//============================================
		//	空中ジャンプキャンセルボタン
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{

			//if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
			//{
			//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
			//}
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
			//{
			//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
			//}

			//// 高く飛ばす 
			//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

		
			// 空中ジャンプの権利がなかったら弾く
			if (pPerson->isAerialJump() == true)
			{
				// 空中ジャンプステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			}
			
			return;
		}

	}


}

void BasePlayerState::AerialAttack::Exit(BasePlayer * pPerson)
{


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

BasePlayerState::AerialDropAttack * BasePlayerState::AerialDropAttack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::AerialDropAttack instance;
	return &instance;
}

void BasePlayerState::AerialDropAttack::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(12);


	// ★攻撃ステートを設定する　↓でHITフラグを消している
	pPerson->SetActionState(BASE_ACTION_STATE::AERIALDROP);

	// 入力方向に向く
	if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
	{
		pPerson->SetDir(DIR::RIGHT);
	}
	else if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
	{
		pPerson->SetDir(DIR::LEFT);
	}

}

void BasePlayerState::AerialDropAttack::Execute(BasePlayer * pPerson)
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
		// 待機モーションに戻る
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
		return;
	}

	// HITしていたらキャンセルできる
	if (pPerson->GetAttackData()->bHit == true)
	{
		//============================================
		//	ジャンプボタン
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::C) == 3)
		{

			//if (pPerson->isPushInput(PLAYER_INPUT::RIGHT))
			//{
			//	pPerson->AddMove(Vector3(0.8f, 0.0f, 0.0f));
			//}
			//if (pPerson->isPushInput(PLAYER_INPUT::LEFT))
			//{
			//	pPerson->AddMove(Vector3(-0.8f, 0.0f, 0.0f));
			//}

			//// 高く飛ばす 
			//pPerson->AddMove(Vector3(0.0f, 0.5f, 0.0f));

			// 空中ジャンプの権利がなかったら弾く
			if (pPerson->isAerialJump() == true)
			{
				// 空中ジャンプステートに行く
				pPerson->GetFSM()->ChangeState(BasePlayerState::AerialJump::GetInstance());
			}
			return;
		}

	}

}

void BasePlayerState::AerialDropAttack::Exit(BasePlayer * pPerson)
{


}

void BasePlayerState::AerialDropAttack::Render(BasePlayer * pPerson)
{

	tdnText::Draw(20, 690, 0xffffffff, "AerialDropAttackState");
}

bool BasePlayerState::AerialDropAttack::OnMessage(BasePlayer * pPerson, const Message & msg)
{
	//// メッセージタイプ
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
//					回避ステート
/*******************************************************/

BasePlayerState::Escape * BasePlayerState::Escape::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Escape instance;
	return &instance;
}

void BasePlayerState::Escape::Enter(BasePlayer * pPerson)
{
	// 待機モーションに変える
	pPerson->SetMotion(13);

	// ★エスケープのフレームに設定する
	pPerson->SetActionState(BASE_ACTION_STATE::ESCAPE);

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
			pPerson->SetMove(Vector3(.25, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.25, 0, 0));
		}
		break;

	case FRAME_STATE::ACTIVE:
		// フラグ設定
		pPerson->SetEscapeFlag(true);
		// クイックエスケープ
		// 向いている方向にスゥーっと移動
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(1.5f, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-1.5f, 0, 0));
		}
		break;

	case FRAME_STATE::FOLLOW:
		// フラグ設定
		pPerson->SetEscapeFlag(false);
		// クイックエスケープ
		// 向いている方向にスゥーっと移動
		if (pPerson->GetDir() == DIR::RIGHT)
		{
			pPerson->SetMove(Vector3(.25, 0, 0));
		}
		else
		{
			pPerson->SetMove(Vector3(-.25, 0, 0));
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

BasePlayerState::StandAction * BasePlayerState::StandAction::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::StandAction instance;
	return &instance;
}

void BasePlayerState::StandAction::Enter(BasePlayer * pPerson)
{
	// 召喚モーションに変える
	pPerson->SetMotion(15);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::STAND);

	// ★スタンドアクション発動
	pPerson->GetStand()->Action(Stand::ACTION_TYPE::LAND);

	// SE再生
	se->Play("ペルソナ召喚");

	// ペルソナ発動エフェクト！
	pPerson->AddEffectAction(pPerson->GetPos(),EFFECT_TYPE::PERSONA);

	// 前回移動量保存
	//pPerson->SetStandSaveMove(pPerson->GetMove());
}

void BasePlayerState::StandAction::Execute(BasePlayer * pPerson)
{
	// オーラのパーティクル
	ParticleManager::EffectPersonaAura(pPerson->GetPos());

	if (pPerson->GetStand()->isHit())
	{
		/* キャンセルルート */

		//////////////////////////////////////////////
		//	スキルキャンセル
		//============================================
		if (SkillCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	しゃがみキャンセル
		//============================================
		if (SquatCansel(pPerson)) return;

		//////////////////////////////////////////////
		//	ジャンプキャンセル
		//============================================
		if (JumpCansel(pPerson))
		{

			if (pPerson->isPushInput(PLAYER_INPUT::LEFT) == true)
			{
				pPerson->AddMove(Vector3(-0.35f, 0, 0));
			}
			else if (pPerson->isPushInput(PLAYER_INPUT::RIGHT) == true)
			{
				pPerson->AddMove(Vector3(0.35f, 0, 0));
			}

			return;
		}

		//////////////////////////////////////////////
		//	左右キーで走りキャンセル
		//============================================
		if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
		{
			pPerson->SetDir(DIR::LEFT);
			pPerson->GetFSM()->ChangeState(BasePlayerState::Run::GetInstance());
		}
		else if (pPerson->GetInputList(PLAYER_INPUT::LEFT) == 3)
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
//					フィニッシュアタックステート
/*******************************************************/

BasePlayerState::FinishAttack * BasePlayerState::FinishAttack::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::FinishAttack instance;
	return &instance;
}

void BasePlayerState::FinishAttack::Enter(BasePlayer * pPerson)
{
	// 召喚モーションに変える
	pPerson->SetMotion(7);

	// アクションステート変更
	pPerson->SetActionState(BASE_ACTION_STATE::FINISH);

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
}

void BasePlayerState::FinishAttack::Exit(BasePlayer * pPerson)
{


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

BasePlayerState::Skill * BasePlayerState::Skill::GetInstance()
{
	// ここに変数を作る
	static BasePlayerState::Skill instance;
	return &instance;
}

void BasePlayerState::Skill::Enter(BasePlayer * pPerson)
{
	// 召喚モーションに変える
	pPerson->SetMotion(6);

	// アクションステートをキャラクター固有に変更
	pPerson->SetActionState(BASE_ACTION_STATE::SKILL);

	// SE再生
	se->Play("ペルソナ召喚");

}

void BasePlayerState::Skill::Execute(BasePlayer * pPerson)
{

	// そのキャラクターのスキルの更新
	pPerson->SkillUpdate();


	// 攻撃ステート終わったら
	if (pPerson->GetActionFrame()==FRAME_STATE::END)
	{
		// 待機ステートに(さすがにキャンセルはやめておこう)
		pPerson->GetFSM()->ChangeState(BasePlayerState::Wait::GetInstance());
	}
}

void BasePlayerState::Skill::Exit(BasePlayer * pPerson)
{


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
