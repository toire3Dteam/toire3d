#include "TDNLIB.h"
#include "Collision.h"
#include "../Player/BasePlayer.h"
#include "../Player/PlayerManager.h"
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stage/Stage.h"
#include "../Shot/ShotManager.h"
#include "../Shot/BaseShot.h"
#include "../Stand/Stand.h"

Stage::Base *Collision::m_pStage = nullptr;

void Collision::PlayerCollision(PlayerManager *pPlayerMgr, ShotManager *pShotMgr)
{
	BasePlayer *pPlayer1(pPlayerMgr->GetPlayer(0)), *pPlayer2(pPlayerMgr->GetPlayer(1));

	/* プレイヤーVSプレイヤーの攻撃判定 */
	HIT_DAMAGE_INFO *pHitDamageInfo[(int)SIDE::ARRAY_MAX] = { nullptr };

	// 敵への方向フラグ更新
	{
		const Vector3 v(pPlayer1->GetPos() - pPlayer2->GetPos());
		if (v.x > 0)
		{
			pPlayer1->SetTargetDir(DIR::LEFT);
			pPlayer2->SetTargetDir(DIR::RIGHT);
		}
		else if (v.x < 0)
		{
			pPlayer1->SetTargetDir(DIR::RIGHT);
			pPlayer2->SetTargetDir(DIR::LEFT);
		}
	}

	// めり込み判定
	if (!pPlayer1->isEscape() && !pPlayer2->isEscape()) Collision::Sinking(pPlayer1, pPlayer2);

	/* プレイヤーVSプレイヤーの攻撃判定 */
	//if (pPlayer1->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// 自分が投げモードに入ってたら
	//	CollisionThrowAttack(pPlayer1, pPlayer2, pStage);	// 投げ判定関数
	//else
		CollisionPlayerAttack(pPlayer1, pPlayer2, &pHitDamageInfo[1]);

	//if (pPlayer2->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// 自分が投げモードに入ってたら
	//	CollisionThrowAttack(pPlayer2, pPlayer1, pStage);	// 投げ判定関数
	//else
		CollisionPlayerAttack(pPlayer2, pPlayer1, &pHitDamageInfo[0]);


	/* スタンドVSプレイヤーの攻撃判定 */
	bool receive;	// 判定結果受取り用変数
	receive = CollisionStandAttack(pPlayer1->GetStand(), pPlayer2);
	if (receive) pPlayer1->GetStand()->GetAttackData()->bHit = true;
	receive = CollisionStandAttack(pPlayer2->GetStand(), pPlayer1);
	if (receive) pPlayer2->GetStand()->GetAttackData()->bHit = true;


	// プレイヤーVSプレイヤー攻撃結果確定

	// 当たってたら
	if (pHitDamageInfo[0])
	{
		// メッセージを送信
		SendHitMessage(pPlayer2, pPlayer1, pHitDamageInfo[0]);

		// 2重ヒット防止用のフラグをONにする
		if (pPlayer2->isAttackState())pPlayer2->GetAttackData()->bHit = true;
	}
	if (pHitDamageInfo[1])
	{
		// メッセージを送信
		SendHitMessage(pPlayer1, pPlayer2, pHitDamageInfo[1]);

		// 2重ヒット防止用のフラグをONにする
		if (pPlayer1->isAttackState())pPlayer1->GetAttackData()->bHit = true;
	}

	/* 玉VS判定 */
	{
		SIDE Player1Side(pPlayer1->GetSide());
		for (auto it : *pShotMgr->GetList(Player1Side))
		{
			// 玉VSステージ
			if(RaypicShot(m_pStage, it)) it->Erase();

			// 玉VSプレイヤー
			else if (CollisionShot(it,pPlayer2)) it->Erase();
		}
	}

	{
		SIDE Player2Side(pPlayer2->GetSide());
		for (auto it : *pShotMgr->GetList(Player2Side))
		{
			// 玉VSステージ
			if (RaypicShot(m_pStage, it)) it->Erase();

			// 玉VSプレイヤー
			else if (CollisionShot(it, pPlayer1)) it->Erase();
		}
	}


	// 解放
	FOR((int)SIDE::ARRAY_MAX)
	{
		if (pHitDamageInfo[i]) delete pHitDamageInfo[i];
	}

}

void Collision::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
{
	/* メッセージ送信 */

	// ダメージ受けたよメッセージ
	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);

	// ダメージを与えるやつに対してヒットストップをかける
	pAttackPlayer->SetHitStopFrame(pHitDamageInfo->hitStopFlame);
	//pDamagePlayer->SetHitStopFrame(pHitDamageInfo->hitStopFlame);
}

void Collision::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_DAMAGE_INFO **OutDamageInfo)
{
	// チーム同じだよ！
	if (my->GetSide() == you->GetSide()) return;

	// 攻撃系のステートじゃないよ！
	if (!my->isAttackState()) return;

	// 相手がエスケープ中だよ！
	if (you->isEscape()) return;

	// すでに当たってたら
	AttackData *pAttackData(my->GetAttackData());
	if (pAttackData->bHit) return;

	if (!my->isActiveFrame()) return; // 攻撃フレーム中じゃない
	// 渾身の対空の処理
	/***************************************/
	// 相手も攻撃中なら
	if (you->isAttackState() == true)
	{
		// その攻撃が　もし始動・持続の間に
		// 「対空」効果を持っていた場合
		if ((you->GetActionFrame() == FRAME_STATE::ACTIVE ||
			you->GetActionFrame() == FRAME_STATE::START)
			&& you->GetAttackData()->bAntiAir == true)
		{
			// 自分が空中で攻撃していた場合
			if (my->isLand() == false)return;
		}

	}
	/******************************************/


	if (you->GetInvincibleLV() <= pAttackData->pierceLV)	// 相手が無敵でない
	{
		// 攻撃判定形状と相手の四角で判定をとる
		CollisionShape::Square AttackShape, YouShape;
		memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pAttackData->pCollisionShape, sizeof(CollisionShape::Square));
		memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
		if (my->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
		AttackShape.pos += my->GetPos();
		YouShape.pos += you->GetPos();
		if (Collision::HitCheck(&AttackShape, &YouShape))
		{
			// ★投げ属性なら
			if (pAttackData->attribute == ATTACK_ATTRIBUTE::THROW)
			{
				// 相手が投げられる系のステートじゃなかったらつかめない
				if (!isThrownState(you)) return;

				// bHit更新
				pAttackData->bHit = true;

				// ★ごり押しでヒットストップをかける
				my->SetHitStopFrame(pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame);
				you->SetHitStopFrame(pAttackData->places[(int)AttackData::HIT_PLACE::LAND].hitStopFlame);

				// 掴まれたメッセージ
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, nullptr);
			}

			else
			{
				// ヒット時の処理
				//pAttackData->bHit = true;	// 2重ヒット防止用のフラグをONにする

				/* メッセージ送信 */

				// 相手がヒットしたときの地上にいたか空中にいたか
				int iHitPlace((!you->isLand() || you->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) || you->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance())) ? (int)AttackData::HIT_PLACE::AERIAL : (int)AttackData::HIT_PLACE::LAND);

				// (A列車)(TODO) ★★★これだとフィニッシュアーツがひとつしか存在しなくなるのでどうにかしよう
				// フィニッシュアーツかどうか
				bool bOverDrive(my->GetActionState() == BASE_ACTION_STATE::HEAVEHO_DRIVE);

				int hitScore(pAttackData->HitScore);
				//if (you->isGuard()) // 相手がガード中なら
				//{
				//	hitScore = (int)(hitScore * 0.25f);// スコア半減
				//
				//	// 懇親の
				//	if (my->GetDir() == DIR::RIGHT)
				//	{
				//		my->SetMove(Vector3(-0.5f,0,0));
				//	}
				//	else
				//	{
				//		my->SetMove(Vector3(0.5f, 0, 0));
				//	}
				//}

				// そして、ダメージを受けた人に送信
				*OutDamageInfo = new HIT_DAMAGE_INFO;
				(*OutDamageInfo)->BeInvincible = pAttackData->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
				(*OutDamageInfo)->damage = pAttackData->damage;				// ダメージ(スコア)
				(*OutDamageInfo)->FlyVector = pAttackData->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
				(*OutDamageInfo)->hitStopFlame = pAttackData->places[iHitPlace].hitStopFlame;		// ヒットストップ
				(*OutDamageInfo)->HitRecoveryFrame = pAttackData->places[iHitPlace].HitRecoveryFrame;		// 硬直時間
				(*OutDamageInfo)->GuardRecoveryFrame = pAttackData->GuardRecoveryFrame;		// 硬直時間
				(*OutDamageInfo)->HitEffectType = (int)pAttackData->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
				(*OutDamageInfo)->iAttackType = (int)my->GetActionState();						// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
				(*OutDamageInfo)->bOverDrive = bOverDrive;										// ふぃにしゅアーツかどうか	
				(*OutDamageInfo)->iAntiGuard = (int)pAttackData->AntiGuard;				// ガード突き破りタイプ
				(*OutDamageInfo)->HitSE = pAttackData->HitSE;
				(*OutDamageInfo)->bFinishOK = pAttackData->bFinish;
				(*OutDamageInfo)->fGuardKnockBackPower = pAttackData->fGuardKnockBackPower;
				(*OutDamageInfo)->DamageMotion = pAttackData->places[iHitPlace].DamageMotion;
				(*OutDamageInfo)->fComboRate = pAttackData->fComboRate;
				(*OutDamageInfo)->iAttribute = (int)pAttackData->attribute;

				// ★コンボUI エフェクト(カウント)発動
				// ★★プレイヤーのグローバルステートのダメージのところに移しました
				//if (you->isGuard() == false)
				//	you->GetComboUI()->Count(hitScore, (*OutDamageInfo)->recoveryFlame); // 相手がガード中でなければ
				//else you->GetComboUI()->Guard();// ガードされた

				if (
					//my->GetPos().x > you->GetPos().x// 位置関係によるベクトル
					my->GetDir() == DIR::LEFT			// 当てた人の向きによるベクトル
					) (*OutDamageInfo)->FlyVector.x *= -1;
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// カメラに振動メッセージを送る
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pAttackData->ShakeCameraInfo);
			}
		}
	}
}

bool Collision::CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou)
{
	// チーム同じだよ！
	if (pStand->GetPlayer()->GetSide() == pYou->GetSide()) return false;

	// スタンドがアクティブじゃないよ！
	if (!pStand->isActive()) return false;

	// 相手がエスケープ中だよ！
	if (pYou->isEscape()) return false;

	// 攻撃フレーム中じゃない
	if (!pStand->isAttackFrame()) return false;

	// スタンドがアタックタイプじゃないよ！
	AttackData *pStandAttackData(pStand->GetAttackData());
	if (pStandAttackData == nullptr)return false;

	// すでに当たってたら
	if (pStandAttackData->bHit) return false;

	if (pYou->GetInvincibleLV() == 0)								// 相手が無敵でない
	{
		// 攻撃判定形状と相手の四角で判定をとる
		CollisionShape::Square AttackShape, YouShape;
		memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pStandAttackData->pCollisionShape, sizeof(CollisionShape::Square));
		memcpy_s(&YouShape, sizeof(CollisionShape::Square), pYou->GetHitSquare(), sizeof(CollisionShape::Square));
		if (pStand->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
		AttackShape.pos += pStand->GetPos();
		YouShape.pos += pYou->GetPos();
		if (Collision::HitCheck(&AttackShape, &YouShape))
		{
			// ヒット時の処理
			//pAttackData->bHit = true;	// 2重ヒット防止用のフラグをONにする

			/* メッセージ送信 */

			// 相手がヒットしたときの地上にいたか空中にいたか
			int iHitPlace((!pYou->isLand() || pYou->GetFSM()->isInState(*BasePlayerState::KnockDown::GetInstance()) || pYou->GetFSM()->isInState(*BasePlayerState::DownFall::GetInstance())) ? (int)AttackData::HIT_PLACE::AERIAL : (int)AttackData::HIT_PLACE::LAND);
			// まず、攻撃をヒットさせた人に送信
			//HIT_ATTACK_INFO hai;
			//hai.bOverDrive = false;
			//hai.hitStopFlame = pStandAttackData->places[iHitPlace].hitStopFlame;		// 自分自身にもの自分のヒットストップ
			//hai.HitScore = pStandAttackData->HitScore;				// ダメージ(スコア)
			//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
			//// 数字エフェクト追加
			//Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
			//NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pStandAttackData->damage, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

			// そして、ダメージを受けた人に送信
			HIT_DAMAGE_INFO HitDamageInfo;
			HitDamageInfo.bOverDrive = false;// ★(仮)
			HitDamageInfo.BeInvincible = pStandAttackData->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
			HitDamageInfo.damage = pStandAttackData->damage;				// ダメージ(スコア)
			HitDamageInfo.FlyVector = pStandAttackData->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
			HitDamageInfo.hitStopFlame = pStandAttackData->places[iHitPlace].hitStopFlame;		// ヒットストップ
			HitDamageInfo.HitRecoveryFrame = pStandAttackData->places[iHitPlace].HitRecoveryFrame;		// 硬直時間
			HitDamageInfo.GuardRecoveryFrame = pStandAttackData->GuardRecoveryFrame;		// 硬直時間
			HitDamageInfo.HitEffectType = (int)pStandAttackData->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
			HitDamageInfo.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTIONのときは、コンボ継続関連のを無視する
			HitDamageInfo.iAntiGuard = (int)pStandAttackData->AntiGuard;				// ガード突き破りタイプ
			HitDamageInfo.HitSE = pStandAttackData->HitSE;
			HitDamageInfo.bFinishOK = pStandAttackData->bFinish;
			HitDamageInfo.fGuardKnockBackPower = pStandAttackData->fGuardKnockBackPower;
			HitDamageInfo.DamageMotion = pStandAttackData->places[iHitPlace].DamageMotion;
			HitDamageInfo.fComboRate = pStandAttackData->fComboRate;
			HitDamageInfo.iAttribute = (int)pStandAttackData->attribute;

			// コンボ用
			//COMBO_DESK comboDesk;
			//comboDesk.side = pYou->GetSide();
			//comboDesk.damage = HitDamageInfo.damage;
			//comboDesk.recoveryFrame = HitDamageInfo.recoveryFlame;
			//
			//// ★コンボUI エフェクト(カウント)発動
			//if (pYou->GetGuardState() == GUARD_STATE::NO_GUARD)
			//	MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_COUNT, &comboDesk);
			//else
			//	MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::UI_MGR, MESSAGE_TYPE::COMBO_GUARD, &comboDesk);
			if (
				//my->GetPos().x > you->GetPos().x// 位置関係によるベクトル
				pStand->GetDir() == DIR::LEFT			// 当てた人の向きによるベクトル
				) HitDamageInfo.FlyVector.x *= -1;
			MsgMgr->Dispatch(0, pStand->GetPlayer()->GetID(), pYou->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);


			// カメラに振動メッセージを送る
			MsgMgr->Dispatch(0, pStand->GetPlayer()->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pStandAttackData->ShakeCameraInfo);

			return true;	// 当たった
		}
	}
	return false;	// 当たらなかった
}

bool Collision::CollisionShot(Shot::Base *shot, BasePlayer *you)
{
	// 相手がエスケープ中だよ！
	if (you->isEscape()) return false;

	if (you->GetInvincibleLV() != 0) return false;									// 相手が無敵じゃない

	AttackData *pShotAttackData(shot->GetAttackData());

	if (isInputGuardCommand(you))
	{
		if (Math::Length(you->GetPos(), shot->GetPos()) < BasePlayer::c_GUARD_DISTANCE)// ガード発動距離
		{
			you->GetFSM()->ChangeState(BasePlayerState::Guard::GetInstance());
			//return false;
		}
	}

	// 攻撃判定形状と相手の四角で判定をとる
	CollisionShape::Square AttackShape, YouShape;
	memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pShotAttackData->pCollisionShape, sizeof(CollisionShape::Square));
	memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
	AttackShape.pos += shot->GetPos();
	YouShape.pos += you->GetPos();
	if (Collision::HitCheck(&AttackShape, &YouShape))
	{
		// ヒット時の処理
		//pAttackData->bHit = true;	// 2重ヒット防止用のフラグをONにする

		/* メッセージ送信 */

		// 相手がヒットしたときの地上にいたか空中にいたか
		int iHitPlace = (you->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

		// そして、ダメージを受けた人に送信
		HIT_DAMAGE_INFO HitDamageInfo;
		HitDamageInfo.bOverDrive = false;// ★(仮)
		HitDamageInfo.BeInvincible = pShotAttackData->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
		HitDamageInfo.damage = pShotAttackData->damage;				// ダメージ(スコア)
		HitDamageInfo.FlyVector = pShotAttackData->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
		HitDamageInfo.hitStopFlame = pShotAttackData->places[iHitPlace].hitStopFlame;		// ヒットストップ
		HitDamageInfo.HitRecoveryFrame = pShotAttackData->places[iHitPlace].HitRecoveryFrame;		// 硬直時間
		HitDamageInfo.GuardRecoveryFrame = pShotAttackData->GuardRecoveryFrame;		// 硬直時間
		HitDamageInfo.HitEffectType = (int)pShotAttackData->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
		HitDamageInfo.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTIONのときは、コンボ継続関連のを無視する
		HitDamageInfo.iAntiGuard = (int)pShotAttackData->AntiGuard;				// ガード突き破りタイプ
		HitDamageInfo.HitSE = pShotAttackData->HitSE;
		HitDamageInfo.bFinishOK = pShotAttackData->bFinish;
		HitDamageInfo.fGuardKnockBackPower = pShotAttackData->fGuardKnockBackPower;
		HitDamageInfo.DamageMotion = pShotAttackData->places[iHitPlace].DamageMotion;
		HitDamageInfo.iAttribute = (int)pShotAttackData->attribute;

		if (shot->GetVec().x < 0) HitDamageInfo.FlyVector.x *= -1;
		MsgMgr->Dispatch(0, shot->GetPlayer()->GetID(), you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

		// カメラに振動メッセージを送る
		MsgMgr->Dispatch(0, shot->GetPlayer()->GetID(), ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pShotAttackData->ShakeCameraInfo);

		return true;	// 当たった
	}
	else return false;
}


//// ★投げの攻撃に対するプレイヤーの判定
//bool Collision::CollisionThrowAttack(BasePlayer *my, BasePlayer *you, Stage::Base *pStage)
//{
//	// チーム同じだよ！
//	if (my->GetSide() == you->GetSide()) return false;
//
//	// 相手が投げられる系のステートじゃなかったらつかめない
//	if (!isThrownState(you)) return false;
//
//	// もうすでに掴んでるよ！
//	if (my->isThrowSuccess()) return false;
//
//	// 相手が無敵状態
//	if (you->GetInvincibleLV() > my->GetAttackData()->pierceLV) return false;
//
//	// 投げ判定中じゃない
//	if (
//		//my->GetCurrentFrame() >= BasePlayer::c_THROW_ESCAPE_FRAME
//		my->GetActionFrame() != FRAME_STATE::ACTIVE
//		) return false;
//
//	// 球距離判定
//	Vector3 v(you->GetPos() - my->GetPos());
//	if (v.x * v.x + v.y * v.y < 10 * 10 &&
//		((v.x > 0 && my->GetDir() == DIR::RIGHT) || (v.x < 0 && my->GetDir() == DIR::LEFT)))
//	{
//		// ★ここでつかまれた相手の座標をつかんだ相手の掴み座標に移動(普通に代入すると壁抜けするので、壁判定もろもろここでやってしまおうという事)
//		if (my->GetDir() == DIR::LEFT  && my->isPushInput(PLAYER_COMMAND_BIT::RIGHT) ||
//			my->GetDir() == DIR::RIGHT && my->isPushInput(PLAYER_COMMAND_BIT::LEFT))
//		{
//			// 向きを逆にする
//			my->ReverseDir();
//
//			// 相手の座標と自分の座標を入れ替える
//			Vector3 temp(my->GetPos());
//			my->SetPos(you->GetPos());
//			you->SetPos(temp);
//		}
//
//		const Vector3 p(my->GetPos());
//		Vector3 v(my->GetFlontPos() - p), v2(v);
//
//		pStage->Collision(my, &v);	// ステージとの判定で、move値をどうこういじった後に
//		if (fabsf(v.x) == 0)
//		{
//			my->GetPosAddress()->x -= v2.x;
//			you->SetPos(my->GetPos() + v2);
//		}
//		else you->SetPos(p + v);
//
//		/* メッセージ送信 */
//
//		// 掴み成功したよとメッセージを送る
//		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, my->GetID(), MESSAGE_TYPE::THROW_SUCCESS, nullptr);
//
//		// 掴まれたメッセージ
//		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, nullptr);
//
//		return true;
//	}
//
//	return false;
//}

bool Collision::RaypicShot(Stage::Base *obj, Shot::Base *shot)
{
	const Vector3 move(shot->GetMove());
	const Vector3 pos(shot->GetPos());

	if (move.y > 0) // 上
	{
		if (pos.y + move.y > 100) return true;
	}
	else if (move.y < 0) // 下
	{
		if (pos.y + move.y < obj->GetBottom()) return true;
	}

	if (move.x > 0) // 右
	{
		if (pos.x + move.x > obj->GetWidth() / 2) return true;
	}
	else if (move.x < 0) // 左
	{
		if (pos.x + move.x < -obj->GetWidth() / 2) return true;
	}

	return false;
}

void Collision::Raypic(BasePlayer *player, Vector3 *move) // ステージとプレイヤー
{
	if (move->y > 0) // 上
	{
		//RaypicUp(obj, player, move);

		// ★★★　move値が0以上だった場合、isLand判定をしているレイピックが呼ばれないため、通常3段目殻の派生が滞空にならない。
		// move値Yが0以上なら、間違いなく地上ではない場所にいるだろうと仮定、強制的に地上フラグをオフにする
		player->SetLand(false);//プレイヤーのポジションをセットした後に変更
	}
	else if (move->y <= 0) // 下
	{
		RaypicDown(m_pStage, player, move);
	}

	if (move->x != 0)
	{
		// プレイヤーとプレイヤーの距離
		const float fTargetVecX(player->GetTargetPlayer()->GetPos().x - player->GetPos().x);

		if (move->x > 0) // 右
		{
			RaypicRight(m_pStage, player, move, fTargetVecX);
		}
		else if (move->x < 0) // 左
		{
			RaypicLeft(m_pStage, player, move, fTargetVecX);
		}
	}
}

float Collision::CheckMove(BasePlayer *pPlayer, const Vector3 &vMove)
{
	Vector3 pos(pPlayer->GetPos()), move(vMove);
	const CollisionShape::Square *square(pPlayer->GetHitSquare());
	float width(m_pStage->GetWidth() / 2);

	if (move.x < 0) // 左
	{
		width *= -1;
		if (pos.x - square->width + move.x <= width)
		{
			pos.x = width + square->width - square->pos.x;

			// 自分の座標から壁へのX成分を返す
			return pos.x - pPlayer->GetPos().x;
		}
	}

	else if(move.x > 0) // 右
	{
		if (pos.x + square->width + move.x >= width)
		{
			pos.x = width - square->width + square->pos.x;

			// 自分の座標から壁へのX成分を返す
			return pos.x - pPlayer->GetPos().x;
		}
	}

	// 変わらない移動量
	return move.x;
}

void Collision::RaypicDown(Stage::Base *obj, BasePlayer *player, Vector3 *move)
{
	if (move->y <= 0) // 下
	{
		bool hit(false);

		Vector3 pos(player->GetPos());
		const CollisionShape::Square *square(player->GetHitSquare());
		Vector3 ray_pos;

		ray_pos = pos;
		ray_pos += square->pos;
		//ray_pos.x += square->width; // 右から

		const float bottom(obj->GetBottom());

		//if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			if (pos.y + move->y <= bottom)
			{
				pos.y = bottom + square->height - square->pos.y;
				move->y = 0;

				// プレイヤーにセットするのではなく、move値のアドレスを引数でもらって直接書き換える形にした。(めり込み判定で、めり込み判定用のmove値を使うことになったので)
				//player->SetMove(move);
				player->SetPos(pos);

				// ★★★　地面に付いたら
				player->SetLand(true);//プレイヤーのポジションをセットした後に変更
				player->SetAerialJump(true);// 空中ジャンプの権利も戻る
				hit = true;
			}
			// 空中にいるよ！
			else
			{
				//player->SetLand(false);//プレイヤーのポジションをセットした後に変更
				// 落下メッセージ送信
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + (int)player->GetSide()), MESSAGE_TYPE::FALL, nullptr);
			}
		}


		//if (!hit)
		//{
		//	ray_pos = pos;
		//	ray_pos += square->pos;
		//	ray_pos.x -= square->width; // 左から
		//	ray_vec.Set(0, -1, 0);
		//	dist = square->height - move->y + 1;
		//
		//	if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		//	{
		//		dist = abs(hit_pos.y - ray_pos.y);
		//		if (dist <= square->height - move->y)
		//		{
		//			pos.y = hit_pos.y + square->height - square->pos.y;
		//			move->y = 0;
		//
		//			// プレイヤーにセットするのではなく、move値のアドレスを引数でもらって直接書き換える形にした。(めり込み判定で、めり込み判定用のmove値を使うことになったので)
		//			//player->SetMove(move);
		//			player->SetPos(pos);
		//
		//			//player->Land();//プレイヤーのポジションをセットした後に変更
		//		}
		//	}
		//}
	}
}
void Collision::RaypicLeft(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetVecX)
{
	Vector3 pos(player->GetPos());
	const CollisionShape::Square *square(player->GetHitSquare());

	const float width(
		//(fabsf(fTargetVecX) > 50) ? pos.x : 
		-obj->GetWidth()/2);

	if (move->x < 0) // 左
	{
		bool hit = false;

		//if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			if (pos.x - square->width + move->x <= width)
			{
				pos.x = width + square->width - square->pos.x;
				move->x = 0;

				//player->SetMove(move);
				player->SetPos(pos);
				hit = true;
			}
		}
	}
}
void Collision::RaypicRight(Stage::Base *obj, BasePlayer *player, Vector3 *move, float fTargetVecX)
{
	Vector3 pos(player->GetPos());
	const CollisionShape::Square *square(player->GetHitSquare());

	if (move->x > 0) // 右
	{
		bool hit = false;

		const float width(
			//(fabsf(fTargetVecX) > 50) ? pos.x : 
			obj->GetWidth() / 2);

		//if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			if (pos.x + square->width + move->x >= width)
			{
				pos.x = width - square->width + square->pos.x;
				move->x = 0;

				// プレイヤーにセットするのではなく、move値のアドレスを引数でもらって直接書き換える形にした。(めり込み判定で、めり込み判定用のmove値を使うことになったので)
				//player->SetMove(move);
				player->SetPos(pos);
				hit = true;
			}
		}

		//if (!hit) // 当たらなかったから上下からray出す 
		//{
		//	float h(0);
		//	if (move->y > 0) // 上
		//	{
		//		h = square->height * 0.85f;
		//	}
		//	else // 下
		//	{
		//		h = -square->height * 0.85f;
		//	}
		//	ray_pos = pos;
		//	ray_pos += square->pos;
		//	ray_pos.y += h;
		//	ray_vec.Set(1, 0, 0);
		//	dist = square->width + move->x + 1;
		//
		//	if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		//	{
		//		dist = abs(hit_pos.x - ray_pos.x);
		//		if (dist <= square->width + move->x)
		//		{
		//			pos.x = hit_pos.x - square->width - square->pos.x;
		//			move->x = 0;
		//
		//			//player->SetMove(move);
		//			player->SetPos(pos);
		//		}
		//	}
		//}

	}
}


void Collision::Sinking(BasePlayer *pPlayer1, BasePlayer *pPlayer2)
{
	// プレイヤーの判定■
	CollisionShape::Square s1, s2;
	memcpy_s(&s1, sizeof(CollisionShape::Square), pPlayer1->GetHitSquare(), sizeof(CollisionShape::Square));
	memcpy_s(&s2, sizeof(CollisionShape::Square), pPlayer2->GetHitSquare(), sizeof(CollisionShape::Square));
	s1.pos += pPlayer1->GetPos(), s2.pos += pPlayer2->GetPos();

	// とりあえず、横だけ判定してみる
	Vector3 v = s2.pos - s1.pos;
	if (HitCheck(&s1, &s2))
	{
		const float len = v.Length();
		//v.Normalize();	// 正規化
		Vector3 sinking1((s1.width + s2.width) - sqrtf(v.x*v.x), (s1.height + s2.height) - sqrtf(v.y*v.y), 0);	// めり込み量
		Vector3 sinking2(-sinking1);
		if (v.x > 0)
		{
			sinking1.x *= -1;
			sinking2.x *= -1;
		}

		sinking1.y = 0;
		sinking2.y = 0;
		static const float rate = .5f;
		sinking1 *= rate;	// 完全に戻すと不自然なので
		sinking2 *= rate;

		// むーぶちに足すと挙動がおかしいので、座標に足す。でもそのまま足すと壁抜けするのでレイピック
		Raypic(pPlayer1, &sinking1);
		Raypic(pPlayer2, &sinking2);

		pPlayer1->SetPos(pPlayer1->GetPos() + sinking1);
		pPlayer2->SetPos(pPlayer2->GetPos() + sinking2);
	}
}


bool Collision::HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2)
{
	float dist(c1->radius + c2->radius);
	float lenx(c1->pos.x - c2->pos.x),
		leny(c1->pos.y - c2->pos.y);

	return ((dist * dist) > (lenx*lenx + leny*leny));
}

bool Collision::HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2)
{
	if (s1->height + s2->height < abs(s1->pos.y - s2->pos.y))
		return false;

	if ((s1->width + s2->width) < abs(s1->pos.x - s2->pos.x))
		return false;

	return true;
}

bool Collision::HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s)
{
	// 円に一番近い頂点の位置
	float vx, vy;
	// x
	if (c->pos.x > s->pos.x)
		vx = s->pos.x + s->width;
	else
		vx = s->pos.x - s->width;
	// y
	if (c->pos.y > s->pos.y)
		vy = s->pos.y + s->height;
	else
		vy = s->pos.y - s->height;


	// 円から頂点までの距離
	float lx, ly;
	lx = s->pos.x - c->pos.x;
	ly = s->pos.y - c->pos.y;
	// 円の中に頂点がある
	if (c->radius*c->radius < lx*lx + ly*ly)
		return true;


	bool inx(s->pos.x - s->width <= c->pos.x && c->pos.x <= s->pos.x + s->width)
		, iny(s->pos.y - s->height <= c->pos.y && c->pos.y <= s->pos.y + s->height);
	// 長方形の中に円の中心がある
	if (inx && iny)
		return true;

	// 円が長方形と触れている
	if (inx)
	{
		if (c->radius + s->height > abs(c->pos.y - s->pos.y))
			return true;
	}
	else if (iny)
	{
		if (c->radius + s->width > abs(c->pos.x - s->pos.x))
			return true;
	}

	return false;
}
