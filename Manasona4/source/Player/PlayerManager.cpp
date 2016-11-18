#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"
#include "../Stand/Stand.h"
#include "../Number/Number.h"

// 実体の宣言
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// エンティティID登録
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr),
m_bHeaveHoDriveOverFlow(false)
{
	m_PointAteam = 0;
	m_PointBteam = 0;

	m_OverDriveDim = 255;
}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage, SideData SideDatas[(int)SIDE::ARRAY_MAX])
{
	m_NumPlayer = NumPlayer;
	m_pStage = pStage;

	// プレイヤ確保
	m_pPlayers = new BasePlayer*[NumPlayer];

	FOR(NumPlayer)
	{
		// チームを今は仮で振り分け!! 2016/10.04日　左か右をTeamで変えてる
		SIDE side;
		if (i % 2 == 0)
		{
			side = SIDE::LEFT;
		}
		else
		{
			side = SIDE::RIGHT;
		}

		// AIかどうかフラグ
		const bool bAI(
			//(i == 2)/*(i == 1 || i == 2 || i == 3)*/);//*i == 1*/0))
			SideDatas[(int)side].bAI	// 選ばれていないサイドはAI
			);


		// キャラクターで分岐する
		switch (SideDatas[(int)side].CharacterType)
		{
		case CHARACTER::AIROU:
			m_pPlayers[i] = new Airou(SideDatas[(int)side].iDeviceID, side, bAI);
			break;
		default:
			MyAssert(false,"そんなキャラクターはいない")
			break;
		}
		m_pPlayers[i]->InitActionDatas();		// ★攻撃情報を各キャラに初期化させる
		m_pPlayers[i]->InitMotionDatas();		// 各キャラごとの必殺技
	}

	// プレイヤーが2人という前提のコード
	assert(NumPlayer == 2);
	m_pPlayers[0]->SetTargetPlayer(m_pPlayers[1]);
	m_pPlayers[1]->SetTargetPlayer(m_pPlayers[0]);

	FOR(NumPlayer)
	{
		// AI初期化 なぜここで初期化するというと、初期化の中でプレイヤーを参照することがあり、全員の初期化が完了した後に呼び出したいから
		m_pPlayers[i]->InitAI();
	}
}

PlayerManager::~PlayerManager()
{
	// ダブルポインターの開放
	FOR(m_NumPlayer)delete m_pPlayers[i];
	delete[] m_pPlayers;
}

void PlayerManager::Update(bool bControl)
{	
	// 誰かが1More覚醒していたら全員の動きを止める
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->isGameTimerStopFlag() == true)
		{
			m_pPlayers[i]->Update(bControl);			// モーションとか移動値の作成とか、基本的な更新。
			
			m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
			m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新

			// ●暗転処理
			m_OverDriveDim = max(m_OverDriveDim - 0.1f , 0.25f);

			return; // ほかのキャラクターの所へは断じて通さない
		}
		
	}
	// 誰も覚醒していなかったので通常通り更新
	
	// ●暗転解放処理
	m_OverDriveDim = min(m_OverDriveDim + 0.1f, 1.0f);
	
	

	/* プレイヤーたち更新 */
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)m_pPlayers[i]->UpdateDrive();			// モーションとか移動値の作成とか、基本的な更新。
		else m_pPlayers[i]->Update(bControl);
	}

	/* プレイヤーVSプレイヤーの攻撃判定 */
	HIT_DAMAGE_INFO **pHitDamageInfo=new HIT_DAMAGE_INFO*[m_NumPlayer];
	FOR(m_NumPlayer)
	{
		pHitDamageInfo[i] = nullptr;
	}

	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			// 敵への方向フラグ更新
			UpdatePlayerTargetDir(m_pPlayers[my], m_pPlayers[you]);

			// めり込み判定
			if(!m_pPlayers[my]->isEscape() && !m_pPlayers[you]->isEscape())m_pStage->Sinking(m_pPlayers[my], m_pPlayers[you]);

			// ( my->you you->my )つまり交互に当たっていないかチェックする

			/* プレイヤーVSプレイヤーの攻撃判定 */
			/* プレイヤーVSプレイヤーの投げ掴み判定 */
			if (m_pPlayers[my]->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// 自分が投げモードに入ってたら
				CollisionThrowAttack(m_pPlayers[my], m_pPlayers[you]);	// 投げ判定関数
			else
				CollisionPlayerAttack(m_pPlayers[my], m_pPlayers[you], &pHitDamageInfo[you]);

			if (m_pPlayers[you]->GetFSM()->isInState(*BasePlayerState::Throw::GetInstance()))	// 自分が投げモードに入ってたら
				CollisionThrowAttack(m_pPlayers[you], m_pPlayers[my]);	// 投げ判定関数
			else
				CollisionPlayerAttack(m_pPlayers[you], m_pPlayers[my], &pHitDamageInfo[my]);


			/* スタンドVSプレイヤーの攻撃判定 */
			bool receive;	// 判定結果受取り用変数
			receive = CollisionStandAttack(m_pPlayers[my]->GetStand(), m_pPlayers[you]);
			if (receive) m_pPlayers[my]->GetStand()->GetAttackData()->bHit = true;
			receive = CollisionStandAttack(m_pPlayers[you]->GetStand(), m_pPlayers[my]);
			if (receive) m_pPlayers[you]->GetStand()->GetAttackData()->bHit = true;
		}
	}
	// プレイヤーVSプレイヤー攻撃結果確定
	for (int DamagedPlayerNo(0); DamagedPlayerNo < m_NumPlayer; DamagedPlayerNo++)
	{
		// 当たってたら
		if (pHitDamageInfo[DamagedPlayerNo])
		{
			// ダメージを受けた人のターゲットプレイヤーはすなわちダメージを与えた人
			BasePlayer *pAttackPlayer(m_pPlayers[DamagedPlayerNo]->GetTargetPlayer());

			// メッセージを送信
			SendHitMessage(pAttackPlayer, m_pPlayers[DamagedPlayerNo], pHitDamageInfo[DamagedPlayerNo]);

			// 2重ヒット防止用のフラグをONにする
			if (pAttackPlayer->isAttackState())pAttackPlayer->GetAttackData()->bHit = true;
		}
	}


	/* 位置を確定 */
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i], m_pPlayers[i]->GetMoveAddress());	// ステージとの判定で、move値をどうこういじった後に
		m_pPlayers[i]->UpdatePos();			// 座標にmove値を足す更新
	}

	// 解放
	FOR(m_NumPlayer)
	{
		if (pHitDamageInfo[i]) delete pHitDamageInfo[i];
	}
	delete[] pHitDamageInfo;

	// チームポイント計算
	CalcTeamPoint();

}

void PlayerManager::Render()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer)
	{
		if(m_bHeaveHoDriveOverFlow)m_pPlayers[i]->RenderDrive();
		else m_pPlayers[i]->Render();
	}
}

void PlayerManager::Render(tdnShader* shader, char* name)
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->Render(shader, name);
}

void PlayerManager::RenderShadow()
{
	// プレイヤーの影描画用
	FOR(m_NumPlayer) m_pPlayers[i]->RenderShadow();

}

void PlayerManager::RenderDeferred()
{
	// プレイヤーたち描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderDeferred();

}

void PlayerManager::RenderUI()
{
	// プレイヤーのUI描画
	FOR(m_NumPlayer) m_pPlayers[i]->RenderUI();
}

void PlayerManager::CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_DAMAGE_INFO **OutDamageInfo)
{
	// チーム同じだよ！
	if (my->GetSide() == you->GetSide()) return;

	// 攻撃系のステートじゃないよ！
	if (!my->isAttackState()) return;

	// 相手がエスケープ中だよ！
	if (you->isEscape()) return;

	if (my->isActiveFrame()) // 攻撃フレーム中なら
	{
		// 渾身の対空の処理
		/***************************************/
		// 相手も攻撃中なら
		if (you->isAttackState() == true)
		{
			// その攻撃が　もし始動・持続の間に
			// 「対空」効果を持っていた場合
			if ((you->GetActionFrame() == FRAME_STATE::ACTIVE ||
				you->GetActionFrame() == FRAME_STATE::START)
				&& you->GetAttackData()->bAntiAir== true)
			{
				// 自分が空中で攻撃していた場合
				if (my->isLand() == false)return;
			}

		}
		/******************************************/

		if (you->GetInvincibleLV() <= my->GetAttackData()->pierceLV &&	// 相手が無敵でない
			!my->GetAttackData()->bHit)									// まだ攻撃を当ててない
		{
			// 攻撃判定形状と相手の四角で判定をとる
			CollisionShape::Square AttackShape, YouShape;
			memcpy_s(&AttackShape, sizeof(CollisionShape::Square), my->GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			memcpy_s(&YouShape, sizeof(CollisionShape::Square), you->GetHitSquare(), sizeof(CollisionShape::Square));
			if (my->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			AttackShape.pos += my->GetPos();
			YouShape.pos += you->GetPos();
			if (Collision::HitCheck(&AttackShape, &YouShape))
			{
				// ヒット時の処理
				//my->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする

				/* メッセージ送信 */

				// 相手がヒットしたときの地上にいたか空中にいたか
				int iHitPlace((you->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL);

				// (A列車)(TODO) ★★★これだとフィニッシュアーツがひとつしか存在しなくなるのでどうにかしよう
				// フィニッシュアーツかどうか
				bool bOverDrive(my->GetActionState() == BASE_ACTION_STATE::HEAVEHO_DRIVE);
				
				int hitScore(my->GetAttackData()->HitScore);
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
				(*OutDamageInfo)->BeInvincible = my->GetAttackData()->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
				(*OutDamageInfo)->damage = my->GetAttackData()->damage;				// ダメージ(スコア)
				(*OutDamageInfo)->FlyVector = my->GetAttackData()->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
				(*OutDamageInfo)->hitStopFlame = my->GetAttackData()->places[iHitPlace].hitStopFlame;		// ヒットストップ
				(*OutDamageInfo)->HitRecoveryFrame = my->GetAttackData()->places[iHitPlace].HitRecoveryFrame;		// 硬直時間
				(*OutDamageInfo)->GuardRecoveryFrame = my->GetAttackData()->GuardRecoveryFrame;		// 硬直時間
				(*OutDamageInfo)->HitEffectType = (int)my->GetAttackData()->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
				(*OutDamageInfo)->iAttackType = (int)my->GetActionState();						// 何の攻撃かのタイプ(コンボ中に同じ攻撃を使わせないように)
				(*OutDamageInfo)->bOverDrive = bOverDrive;										// ふぃにしゅアーツかどうか	
				(*OutDamageInfo)->iAntiGuard = (int)my->GetAttackData()->AntiGuard;				// ガード突き破りタイプ
				(*OutDamageInfo)->HitSE = my->GetAttackData()->HitSE;
				(*OutDamageInfo)->bFinishOK = my->GetAttackData()->bFinish;

				// ★コンボUI エフェクト(カウント)発動
				// ★★プレイヤーのグローバルステートのダメージのところに移しました
				//if (you->isGuard() == false)
				//	you->GetComboUI()->Count(hitScore, (*OutDamageInfo)->recoveryFlame); // 相手がガード中でなければ
				//else you->GetComboUI()->Guard();// ガードされた

				// ★★★超絶仮
				//my->SetHP(my->GetMaxHP() - my->GetCollectScore());
				//you->SetHP(my->GetMaxHP() - my->GetCollectScore());

				if (
					//my->GetPos().x > you->GetPos().x// 位置関係によるベクトル
					my->GetDir() == DIR::LEFT			// 当てた人の向きによるベクトル
					) (*OutDamageInfo)->FlyVector.x *= -1;
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);

				// カメラに振動メッセージを送る
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &my->GetAttackData()->ShakeCameraInfo);
			}
		}
	}
}

bool PlayerManager::CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou)
{
	// チーム同じだよ！
	if (pStand->GetPlayer()->GetSide() == pYou->GetSide()) return false;

	// スタンドがアクティブじゃないよ！
	if (!pStand->isActive()) return false;
	
	// スタンドがアタックタイプじゃないよ！
	if (pStand->GetAttackData() == nullptr)return false;

	// 相手がエスケープ中だよ！
	if (pYou->isEscape()) return false;
	
	if (pStand->isAttackFrame()) // 攻撃フレーム中なら
	{
		if (pYou->GetInvincibleLV() == 0 &&								// 相手が無敵でない
			!pStand->GetAttackData()->bHit)									// まだ攻撃を当ててない
		{
			// 攻撃判定形状と相手の四角で判定をとる
			CollisionShape::Square AttackShape, YouShape;
			memcpy_s(&AttackShape, sizeof(CollisionShape::Square), pStand->GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			memcpy_s(&YouShape, sizeof(CollisionShape::Square), pYou->GetHitSquare(), sizeof(CollisionShape::Square));
			if (pStand->GetDir() == DIR::LEFT) AttackShape.pos.x *= -1;	// このposは絶対+(右)なので、左向きなら逆にする
			AttackShape.pos += pStand->GetPos();
			YouShape.pos += pYou->GetPos();
			if (Collision::HitCheck(&AttackShape, &YouShape))
			{
				// ヒット時の処理
				//my->GetAttackData()->bHit = true;	// 2重ヒット防止用のフラグをONにする
	
				/* メッセージ送信 */
	
				// 相手がヒットしたときの地上にいたか空中にいたか
				int iHitPlace = (pYou->isLand()) ? (int)AttackData::HIT_PLACE::LAND : (int)AttackData::HIT_PLACE::AERIAL;

				// まず、攻撃をヒットさせた人に送信
				//HIT_ATTACK_INFO hai;
				//hai.bOverDrive = false;
				//hai.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// 自分自身にもの自分のヒットストップ
				//hai.HitScore = pStand->GetAttackData()->HitScore;				// ダメージ(スコア)
				//MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + pStand->GetDeviceID()), MESSAGE_TYPE::HIT_ATTACK, &hai);
				//// 数字エフェクト追加
				//Vector2 screenPos = Math::WorldToScreen(pStand->GetPos());
				//NumberEffect.AddNumber((int)screenPos.x, (int)screenPos.y - 200, pStand->GetAttackData()->damage, Number_Effect::COLOR_TYPE::WHITE, Number::NUM_KIND::NORMAL);

				// そして、ダメージを受けた人に送信
				HIT_DAMAGE_INFO HitDamageInfo;
				HitDamageInfo.bOverDrive = false;// ★(仮)
				HitDamageInfo.BeInvincible = pStand->GetAttackData()->places[iHitPlace].bBeInvincible;	// 無敵になるかどうか
				HitDamageInfo.damage = pStand->GetAttackData()->damage;				// ダメージ(スコア)
				HitDamageInfo.FlyVector = pStand->GetAttackData()->places[iHitPlace].FlyVector;			// 吹っ飛びベクトル
				HitDamageInfo.hitStopFlame = pStand->GetAttackData()->places[iHitPlace].hitStopFlame;		// ヒットストップ
				HitDamageInfo.HitRecoveryFrame = pStand->GetAttackData()->places[iHitPlace].HitRecoveryFrame;		// 硬直時間
				HitDamageInfo.GuardRecoveryFrame = pStand->GetAttackData()->GuardRecoveryFrame;		// 硬直時間
				HitDamageInfo.HitEffectType = (int)pStand->GetAttackData()->HitEffectType;			// この攻撃のヒットエフェクトを相手に送る
				HitDamageInfo.iAttackType = (int)BASE_ACTION_STATE::NO_ACTION;	// NO_ACTIONのときは、コンボ継続関連のを無視する
				HitDamageInfo.iAntiGuard = (int)pStand->GetAttackData()->AntiGuard;				// ガード突き破りタイプ
				HitDamageInfo.HitSE = pStand->GetAttackData()->HitSE;
				HitDamageInfo.bFinishOK = pStand->GetAttackData()->bFinish;

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
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, pYou->GetID(), MESSAGE_TYPE::HIT_DAMAGE, &HitDamageInfo);
	

				// カメラに振動メッセージを送る
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, ENTITY_ID::CAMERA_MGR, MESSAGE_TYPE::SHAKE_CAMERA, &pStand->GetAttackData()->ShakeCameraInfo);
	
				return true;	// 当たった
			}
		}
	}
	return false;	// 当たらなかった
}

// ★投げの攻撃に対するプレイヤーの判定
bool PlayerManager::CollisionThrowAttack(BasePlayer *my, BasePlayer *you)
{
	// チーム同じだよ！
	if (my->GetSide() == you->GetSide()) return false;

	// 相手が空中だったらつかめない
	if (!you->isLand()) return false;

	// もうすでに掴んでるよ！
	if (my->isThrowSuccess()) return false;

	// 相手が無敵状態
	if (you->GetInvincibleLV() > my->GetAttackData()->pierceLV) return false;

	// 投げ猶予過ぎたよ！
	if (
		//my->GetCurrentFrame() >= BasePlayer::c_THROW_ESCAPE_FRAME
		my->GetActionFrame() != FRAME_STATE::ACTIVE
		) return false;

	// 球距離判定
	Vector3 v(you->GetPos() - my->GetPos());
	if (v.x * v.x + v.y * v.y < 10 * 10 &&
		((v.x > 0 && my->GetDir() == DIR::RIGHT) || (v.x < 0 && my->GetDir() == DIR::LEFT)))
	{
		// ★ここでつかまれた相手の座標をつかんだ相手の掴み座標に移動(普通に代入すると壁抜けするので、壁判定もろもろここでやってしまおうという事)
		if (my->GetDir() == DIR::LEFT  && my->isPushInput(PLAYER_COMMAND_BIT::RIGHT) ||
			my->GetDir() == DIR::RIGHT && my->isPushInput(PLAYER_COMMAND_BIT::LEFT))
		{
			// 向きを逆にする
			my->ReverseDir();

			// 相手の座標と自分の座標を入れ替える
			Vector3 temp(my->GetPos());
			my->SetPos(you->GetPos());
			you->SetPos(temp);
		}

		const Vector3 p(my->GetPos());
		Vector3 v(my->GetFlontPos() - p), v2(v);

		m_pStage->Collision(my, &v);	// ステージとの判定で、move値をどうこういじった後に
		if (fabsf(v.x) == 0)
		{
			my->GetPosAddress()->x -= v2.x;
			you->SetPos(my->GetPos() + v2);
		}
		else you->SetPos(p + v);

		/* メッセージ送信 */

		// 掴み成功したよとメッセージを送る
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, my->GetID(), MESSAGE_TYPE::THROW_SUCCESS, nullptr);

		// 掴まれたメッセージ
		MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, you->GetID(), MESSAGE_TYPE::BE_THROWN, nullptr);

		// 掴みSEをここで再生
		se->Play("掴み成功");

		return true;
	}

	return false;
}

void PlayerManager::SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo)
{
	/* メッセージ送信 */

	// ダメージ受けたよメッセージ
	MsgMgr->Dispatch(0, pAttackPlayer->GetID(), pDamagePlayer->GetID(), MESSAGE_TYPE::HIT_DAMAGE, pHitDamageInfo);
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::HEAVE_HO_OVER_DRIVE_HIT:
		m_bHeaveHoDriveOverFlow = true;
		break;
	}
	return false;
}

// チームポイント計算
void PlayerManager::CalcTeamPoint()
{
	m_PointAteam = 0;
	m_PointBteam = 0;
	
	// プレイヤーたち描画
	FOR(m_NumPlayer)
	{
		if (m_pPlayers[i]->GetSide()==SIDE::LEFT)
		{
			m_PointAteam += m_pPlayers[i]->GetScore();
		}
		else
		{
			m_PointBteam += m_pPlayers[i]->GetScore();
		}	
	}


}

void PlayerManager::UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you)
{
	const Vector3 v(you->GetPos() - my->GetPos());
	if (v.x > 0)
	{
		my->SetTargetDir(DIR::RIGHT);
		you->SetTargetDir(DIR::LEFT);
	}
	else if (v.x < 0)
	{
		my->SetTargetDir(DIR::LEFT);
		you->SetTargetDir(DIR::RIGHT);
	}
}