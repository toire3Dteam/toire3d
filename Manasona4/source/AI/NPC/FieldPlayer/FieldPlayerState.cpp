#include "FieldPlayerState.h"
#include "AI\NPC\FieldPlayer\FieldPlayer.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

#include "SoccerBall\SoccerBall.h" // サッカーボール


/***************************************/
//	グローバルステート_AI
/***************************************/
FieldPlayer_GlobalState_AI* FieldPlayer_GlobalState_AI::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_GlobalState_AI instance;
	return &instance;
}

// 入り口
void FieldPlayer_GlobalState_AI::Enter(FieldPlayer *pPerson)
{

}

// 実行中
void FieldPlayer_GlobalState_AI::Execute(FieldPlayer *pPerson)
{
	//if (tdnInput::KeyGet(KEY_A, 0) == 3)
	//{

	//}

	// 死んでたら無効!!!
	if (pPerson->GetFSM()->GetCurrentState() != FieldPlayer_Dead::GetInstance())
	{

		// 行動分岐
		if (SOCCER_BALL.isHold == true && pPerson->isHave == false)
		{
			// 持っていなかったらリンゴをターゲットへ
			FieldNode*  targetNode = pPerson->GetGraph()->FarPosNode(SOCCER_BALL.GetPos());
			FieldNode*  nearNode = pPerson->GetGraph()->NearPosNode(pPerson->GetPos());
			// リンゴの位置が変わっていたら
			//if (targetNode != pPerson->m_goalNode)
			{
				// さらにキャラクターのタスクが全て終了していたら！
				if (pPerson->m_task.empty())
				{
					// 一番遠い場所にいたらランダム
					if (nearNode == nearNode)
					{
						pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
						pPerson->taskSetting();
					}
					else
					{
						// 遠い場所へ
						pPerson->m_goalPos = targetNode->m_pos;
						pPerson->taskSetting();

					}

				}
			}


		}
		// リンゴを持っているか
		else if (pPerson->isHave == false)
		{
			// 持っていなかったらリンゴをターゲットへ
			FieldNode*  targetNode = pPerson->GetGraph()->NearPosNode(SOCCER_BALL.GetPos());

			// リンゴの位置が変わっていたら
			//if (targetNode != pPerson->m_goalNode)
			{

				DEBUG_FLAME++;
				// さらにキャラクターのタスクが全て終了していたら！
				if (pPerson->m_task.empty())
				{
					// 完璧!
					// リンゴをターゲットに
					//pPerson->SetTargetPos(SOCCER_BALL.GetPos());
					//pPerson->m_goalPos = SOCCER_BALL.GetPos();
					//pPerson->taskSetting();

					// ★ここでプレイヤー全検索する
					PLAYER_INFO ex;
					ex.pos = pPerson->GetPos();
					ex.id = pPerson->GetID();

					// マネージャーへ送信 
					MsgMgr->Dispatch(
						MSG_NO_DELAY,
						pPerson->GetID(),
						ENTITY_ID::PERSON_MNG,
						SOCCER_BALL_NEAR_PLAYER,
						(void*)&ex	// [追記情報]自分のタイプを送る
						);
				}
				else if (DEBUG_FLAME >= 60 * 3)
				{
					DEBUG_FLAME = 0;

					// ★ここでプレイヤー全検索する
					PLAYER_INFO ex;
					ex.pos = pPerson->GetPos();
					ex.id = pPerson->GetID();

					pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
					pPerson->taskSetting();

					// マネージャーへ送信 
					//MsgMgr->Dispatch(
					//	MSG_NO_DELAY,
					//	pPerson->GetID(),
					//	ENTITY_ID::PERSON_MNG,
					//	SOCCER_BALL_NEAR_PLAYER,
					//	(void*)&ex	// [追記情報]自分のタイプを送る
					//	);
				}

				// ターゲットのポジションとそのリンゴのポジションが被ってたら
				// タスク放棄
				if (!pPerson->m_task.empty())// タスクがあれば
				{
					TASK ta = pPerson->m_task.top();
					if (targetNode->m_pos == ta.pos)
					{
						// 前のタスククリア
						// 空になるまでポップ
						//for (;;)
						//{
						//	// 空だったら帰れ
						//	if (pPerson->m_task.empty()) { break; }// 
						//	pPerson->m_task.pop();
						//}

						// 空にしたことにより↑の文へ行かないための応急処置
						pPerson->m_goalNode = targetNode;
					}

				}
			}

			// タスクが空の時
			//if (!pPerson->m_task.empty())

			if (pPerson->IsRand())// 着地しているとき
			{
				// リンゴとの距離が近かったら
				float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

				// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
				if (length <= 15.5f)
				{
					// リンゴをターゲットに
					pPerson->SetTargetPos(SOCCER_BALL.GetPos());
					// ↑のため歩くステートへ
					pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());


					for (;;)
					{
						// 空だったら帰れ
						if (pPerson->m_task.empty()) { break; }// 
						pPerson->m_task.pop();
					}

					// ★持つ範囲
					if (length <= 5.5f)
					{
						// リンゴが地面についていたら
						if (SOCCER_BALL.isRand)
						{
							// 持つステートへ
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have_AI::GetInstance());
						}
					}
				}
			}


		}
		else
		{


			/*************************************/
			//		タスクの設定
			/********************************/
				// キャラクターのタスクが全て終了していたら！
			if (pPerson->m_task.empty())
			{
				// ★ここでプレイヤー全検索する
				PLAYER_INFO ex;
				ex.pos = pPerson->GetPos();
				ex.id = pPerson->GetID();

				// マネージャー送信 
				MsgMgr->Dispatch(
					MSG_NO_DELAY,
					pPerson->GetID(),
					ENTITY_ID::PERSON_MNG,
					TARGET_PLAYERS,
					(void*)&ex	// [追記情報]自分のタイプを送る
					);

				// 完璧!
				// タスクセット
			}


			// 投げるステートのときは来ないでください
			if (pPerson->GetFSM()->GetCurrentState() != FieldPlayer_throw_AI::GetInstance())
			{

				/**********************************/
				// ターゲットとの距離が近かったら
				/**********************************/
				if (pPerson->m_nearPlayer == NULL) //NULL やったわ
				{
					MyDebugString("NULLだった");
				}
				else
				{
					// ポインタ―に変更
					float length = Math::Length(pPerson->GetPos(), pPerson->m_nearPlayer->GetPos());

					// 距離を大きくしないとノードとの関係で近くにいても投げないときが出てくる
					// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
					if (length <= 30.5f)
					{
						//リングアベル流上下絞り
						float height = abs(pPerson->m_goalPos.y - pPerson->GetPos().y);
						if (height <= 5)
						{



							// アングル調整
							if (pPerson->m_goalPos.x >= pPerson->GetPos().x)
							{
								pPerson->SetAngle(1.57f);
							}
							else
							{
								pPerson->SetAngle(-1.57f);
							}

							// 投げるステートへ
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw_AI::GetInstance());
							for (;;)
							{
								// 空だったら帰れ
								if (pPerson->m_task.empty()) { break; }//	 
								pPerson->m_task.pop();
							}

						}

					}
				}
	

			}

			// ターゲットのポジションとそのリンゴのポジションが被ってたら
			// タスク放棄
			//if (!pPerson->m_task.empty())// タスクがあれば
			//{
			//	TASK ta = pPerson->m_task.top();
			//	if (targetNode->m_pos == ta.pos)
			//	{
			//		// 前のタスククリア
			//		// 空になるまでポップ
			//		for (;;)
			//		{
			//			// 空だったら帰れ
			//			if (pPerson->m_task.empty()) { break; }// 
			//			pPerson->m_task.pop();
			//		}

			//		// 空にしたことにより↑の文へ行かないための応急処置
			//		pPerson->m_goalNode = targetNode;
			//	}

			//}


		// タスクが空の時
		//if (!pPerson->m_task.empty())
		//if (pPerson->IsRand())// 着地しているとき
		//{
		//	// リンゴとの距離が近かったら
		//	float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		//	// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
		//	if (length <= 15.5f)
		//	{
		//		// リンゴをターゲットに
		//		pPerson->SetTargetPos(SOCCER_BALL.GetPos());
		//		// ↑のため歩くステートへ
		//		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());



		}





		/*******************/
		// タスク
		/*******************/

		// リンゴを持ってないとき
		if (pPerson->isHave == false)
		{
			// （遊び心）着地するまで更新は許さん
			if (pPerson->IsRand() == true)
			{
				// タスク処理
				// 空じゃなかったら
				if (!pPerson->m_task.empty())
				{

					TASK ta = pPerson->m_task.top();
					//★　ターゲット設定	
					if (pPerson->GetTargetPos() != ta.pos)
					{
						// ターゲット更新
						pPerson->SetTargetPos(ta.pos);

						switch (ta.fieldState)
						{
						case FIELD_STATE::RUN:
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
							break;
						case FIELD_STATE::JUMP:
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump_AI::GetInstance());
							break;
							//case FIELD_STATE::DOWN:
							//m_pStateMachine->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
							//break;
						default:
							break;
						}
					}


					float length = Math::Length(pPerson->GetPos(), pPerson->GetTargetPos());
					// 近づいたらタスクを放り出し次のタスクへ
					if (length <= 6.5f)
					{
						pPerson->m_task.pop();

						// ↑ので空になったら
						if (pPerson->m_task.empty())
						{
							// ★↑でジャンプした後にここにきてもうてるからバグる
							// 待機へ
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
						}
					}

				}
			}

		}
		else // リンゴを持っている
		{
			SOCCER_BALL.SetPos(pPerson->GetObj()->WorldBonePos(8));

			// 投げるステートのときは来ないでください
			if (pPerson->GetFSM()->GetCurrentState() != FieldPlayer_throw_AI::GetInstance())
			{

				// （遊び心）着地するまで更新は許さん
				if (pPerson->IsRand() == true)
				{
					// タスク処理
					// 空じゃなかったら
					if (!pPerson->m_task.empty())
					{

						TASK ta = pPerson->m_task.top();
						//★　ターゲット設定	
						if (pPerson->GetTargetPos() != ta.pos)
						{
							// ターゲット更新
							pPerson->SetTargetPos(ta.pos);

							switch (ta.fieldState)
							{
							case FIELD_STATE::RUN:
								pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
								break;
							case FIELD_STATE::JUMP:
								pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump_AI::GetInstance());
								break;
								//case FIELD_STATE::DOWN:
								//m_pStateMachine->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
								//break;
							default:
								break;
							}
						}


						float length = Math::Length(pPerson->GetPos(), pPerson->GetTargetPos());
						// 近づいたらタスクを放り出し次のタスクへ
						if (length <= 6.5f)
						{
							pPerson->m_task.pop();

							// ↑ので空になったら
							if (pPerson->m_task.empty())
							{
								// ★↑でジャンプした後にここにきてもうてるからバグる
								// 待機へ
								pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
							}
						}

					}
				}
			}// 投げるステート

		}


	}
}

// 出口
void FieldPlayer_GlobalState_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_GlobalState_AI::Render(FieldPlayer * pPerson)
{

}

//
bool FieldPlayer_GlobalState_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SEND_PLAYER_POS:
	{
		MyDebugString("[%d]から近い人の距離届いたで\n", msg.Sender);

		// 近いプレイヤーの場所取得！
		//Vector3* nearPos = (Vector3*)msg.ExtraInfo;
		pPerson->m_nearPlayer =(BasePerson*)msg.ExtraInfo;
		//int* nearPlayer = (int*)msg.ExtraInfo;

		if (pPerson->m_nearPlayer->GetID() == pPerson->GetID())
		{
			MyDebugString("誰もいなくなった・・・\n");
		}
		else
		{

			//pPerson->SetTargetPos(*(Vector3*)msg.ExtraInfo);
			//pPerson->m_goalPos = *(Vector3*)msg.ExtraInfo;   // これもスタックがゼロの時VECTOR_ZEROで来るから怪しい・・
			pPerson->m_goalPos = pPerson->m_nearPlayer->GetPos();
			//pPerson->SetTargetPos(*nearPos);
			pPerson->taskSetting();

		}
		return true;


	}
		break;
	case MESSAGE_TYPE::SEND_NEAR_PLAYER:
	{
		MyDebugString("[%d]から近い人の距離届いた２\n", msg.Sender);

		// 近いプレイヤーの場所取得！
		ENTITY_ID* id = (ENTITY_ID*)msg.ExtraInfo;

		// 持っていなかったらリンゴから一番とおい場所
		FieldNode*  targetNode = pPerson->GetGraph()->FarPosNode(SOCCER_BALL.GetPos());
		FieldNode*  nearNode = pPerson->GetGraph()->NearPosNode(pPerson->GetPos());
		if (pPerson->GetID() == *id)
		{
			// リンゴをターゲットに
			pPerson->m_goalPos = SOCCER_BALL.GetPos();
			pPerson->taskSetting();
		}
		else if (targetNode == nearNode)
		{
			pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
			pPerson->taskSetting();
		}
		else
		{
			// 持っていなかったらリンゴから一番とおい場所
			FieldNode*  targetNode = pPerson->GetGraph()->FarPosNode(SOCCER_BALL.GetPos());

			// リンゴをターゲットに
			pPerson->m_goalPos = targetNode->m_pos;
			pPerson->taskSetting();
			
		}



		return true;
	}
	break;
	default:

		break;
	}//end switch

	 // Flaseで返すとグローバルステートなので正真正銘の終り
	return false;
}



/***************************************/
//	待機 AI
/***************************************/
FieldPlayer_Wait_AI* FieldPlayer_Wait_AI::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Wait_AI instance;
	return &instance;
}

// 入り口
void FieldPlayer_Wait_AI::Enter(FieldPlayer *pPerson)
{
	m_MoveFlame = 0;
	// 待機モーション
	pPerson->GetObj()->SetMotion(0);
}

// 実行中
void FieldPlayer_Wait_AI::Execute(FieldPlayer *pPerson)
{
	//pPerson->SetAngle(pPerson->GetAngle() + 0.1f);

	//// ここで波紋がIsEndになったら
	//pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance()->GetInstance());

	//ずっと立っていたら気合で動かす
	m_MoveFlame++;
	if (m_MoveFlame > 60)
	{
		pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
		pPerson->taskSetting();
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
	}

	// m_isShed = true になったら流した場所へ行く
	//if (pPerson->IsShed() == true)
	//{
	//	pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance());
	//}
}

// 出口
void FieldPlayer_Wait_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Wait_AI::Render(FieldPlayer * pPerson)
{
	//pPerson->GetObj()->Render(shader, "copy");
}

bool FieldPlayer_Wait_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::Riple_Red:

	//	// 自分の噂の波紋を出す。
	//	pPerson->ActionGossipRipple();

	//	return true;

	//	break;
	//default:

	//	break;
	//}//end switch
	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	走る AI
/***************************************/
FieldPlayer_Run_AI* FieldPlayer_Run_AI::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Run_AI instance;
	return &instance;
}

// 入り口
void FieldPlayer_Run_AI::Enter(FieldPlayer *pPerson)
{


	// 走りモーション。 改良：走っていたらそのまま継続
	if (pPerson->GetObj()->GetMotion() != 1)
	{
		pPerson->GetObj()->SetMotion(1);
	}
	
}

// 実行中
void FieldPlayer_Run_AI::Execute(FieldPlayer *pPerson)
{
	// 移動処理


	// 空じゃなかったら
	//if (!pPerson->m_task.empty())
	//{

		//idou
		//Vector3 move = pPerson->GetTargetPos() - pPerson->GetPos();
		//move.Normalize();

		// 移動
		//pPerson->SetMove(move);
		if (pPerson->GetTargetPos().x > pPerson->GetPos().x)
		{
			// アングル
			pPerson->SetAngle(1.57f);
			pPerson->SetMove(Vector3(0.5, pPerson->GetMove().y, pPerson->GetMove().z));

		}
		else if (pPerson->GetTargetPos().x < pPerson->GetPos().x - 1.5f)
		{
			// アングル
			pPerson->SetAngle(-1.57f);
			pPerson->SetMove(Vector3(-0.5, pPerson->GetMove().y, pPerson->GetMove().z));
			//pPerson->SetMove(pPerson->GetMove() + Vector3(-1, 0, 0));
		}
		else //　多分真上or真下
		{
			// 待機へ
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
		}

		//float length = Math::Length(pPerson->GetPos(), task.pos);

		//if (length <= 15)
		//{
		//	pPerson->m_task.pop();
		//}

	//}

		// 壁にぶつかった。ぶつかりながら走っている
		if (pPerson->IsCollWall()==true)
		{
			// ジャンプ
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump_AI::GetInstance());
		}

}

// 出口
void FieldPlayer_Run_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Run_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Run_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	ジャンプ AI
/***************************************/
FieldPlayer_Jump_AI* FieldPlayer_Jump_AI::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Jump_AI instance;
	return &instance;
}

// 入り口
void FieldPlayer_Jump_AI::Enter(FieldPlayer *pPerson)
{

	// ジャンプモーション
	pPerson->GetObj()->SetMotion(2);
	pPerson->SetMove(Vector3(pPerson->GetMove().x, 1.4f , pPerson->GetMove().z));

}

// 実行中
void FieldPlayer_Jump_AI::Execute(FieldPlayer *pPerson)
{
	// 移動処理
	if (pPerson->GetTargetPos().x > pPerson->GetPos().x)
	{
		// アングル
		pPerson->SetAngle(1.57f);
		pPerson->SetMove(Vector3(0.6f, pPerson->GetMove().y, pPerson->GetMove().z));

		//pPerson->SetMove(pPerson->GetMove() + Vector3(0.1f, 0, 0));
		//if (pPerson->GetMove().x >= 1.5f)
		//{
		//	pPerson->SetMove(Vector3(1.5f, pPerson->GetMove().y, pPerson->GetMove().z));
		//}
	}
	else if (pPerson->GetTargetPos().x < pPerson->GetPos().x - 1.5f)
	{

		// アングル
		pPerson->SetAngle(-1.57f);
		pPerson->SetMove(Vector3(-0.6f, pPerson->GetMove().y, pPerson->GetMove().z));
		

		//pPerson->SetMove(pPerson->GetMove() + Vector3(-0.5f, 0, 0));
		//if (pPerson->GetMove().x <= -1.5f)
		//{
		//	pPerson->SetMove(Vector3(-1.5f, pPerson->GetMove().y, pPerson->GetMove().z));
		//}
	}
	else //　多分真上or真下
	{	
		// 待機へ
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
	}

	// 着地したら終わり
	if (pPerson->IsRand()==true)
	{
		// 空じゃなかったら　歩きへ
		if (!pPerson->m_task.empty())
		{
			// 走るぜ
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
		}
		else
		{
			// 待機へ
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
		}
	}

}

// 出口
void FieldPlayer_Jump_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Jump_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Jump_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	持つ AI
/***************************************/
FieldPlayer_Have_AI* FieldPlayer_Have_AI::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Have_AI instance;
	return &instance;
}

// 入り口
void FieldPlayer_Have_AI::Enter(FieldPlayer *pPerson)
{

	// 持つモーション
	pPerson->GetObj()->SetMotion(6);

	//持ちましたフラグOn
	pPerson->isHave = true;
	SOCCER_BALL.isHold = true;
	// owner設定
	SOCCER_BALL.m_owner = pPerson->GetID();

}

// 実行中
void FieldPlayer_Have_AI::Execute(FieldPlayer *pPerson)
{

	// 掴みフレーム終了
	if (pPerson->GetObj()->GetFrame() == 229)
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait_AI::GetInstance());
	}

}

// 出口
void FieldPlayer_Have_AI::Exit(FieldPlayer *pPerson)
{
	//持ちましたフラグOn
	//pPerson->isHave = true;
}

void FieldPlayer_Have_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Have_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	投げる AI
/***************************************/
FieldPlayer_throw_AI* FieldPlayer_throw_AI::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_throw_AI instance;
	return &instance;
}

// 入り口
void FieldPlayer_throw_AI::Enter(FieldPlayer *pPerson)
{
	// 投げるモーション
	pPerson->GetObj()->SetMotion(8);

}

// 実行中
void FieldPlayer_throw_AI::Execute(FieldPlayer *pPerson)
{
	// アングル調整
	if (pPerson->m_goalPos.x >= pPerson->GetPos().x)
	{
		pPerson->SetAngle(1.57f);
	}
	else
	{
		pPerson->SetAngle(-1.57f);
	}

	// フレーム終了
	if (pPerson->GetObj()->GetFrame() == 255)
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait_AI::GetInstance());

	}

}

// 出口
void FieldPlayer_throw_AI::Exit(FieldPlayer *pPerson)
{
	// リンゴ外す
	pPerson->isHave = false;
	SOCCER_BALL.isHold = false;

	if (pPerson->GetAngle() >= 1.57f)
	{
		SOCCER_BALL.SetMove(Vector3(1.8f, .5f, 0));
	}
	else
	{
		SOCCER_BALL.SetMove(Vector3(-1.8f, .5f, 0));
	}

}

void FieldPlayer_throw_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_throw_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}






/***************************************/
//	グローバルステート
/***************************************/
FieldPlayer_GlobalState* FieldPlayer_GlobalState::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_GlobalState instance;
	return &instance;
}

// 入り口
void FieldPlayer_GlobalState::Enter(FieldPlayer *pPerson)
{

}

// 実行中
void FieldPlayer_GlobalState::Execute(FieldPlayer *pPerson)
{
	if (pPerson->isHave)
	{
		SOCCER_BALL.SetPos(pPerson->GetObj()->WorldBonePos(8));
	}
	
}

// 出口
void FieldPlayer_GlobalState::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_GlobalState::Render(FieldPlayer * pPerson)
{

}

//
bool FieldPlayer_GlobalState::OnMessage(FieldPlayer *pPerson, const Message &msg)
{
	// メッセージタイプ
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TARGET_PLAYERS:

		//printf("赤い波紋に当たったんご\n");
		//MyDebugString("赤い波紋に当たったんご\n");

		// 赤い波紋に当たったよ

		return true;

		break;
	default:

		break;
	}//end switch

	 // Flaseで返すとグローバルステートなので正真正銘の終り
	return false;
}


/***************************************/
//	待機 
/***************************************/
FieldPlayer_Wait* FieldPlayer_Wait::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Wait instance;
	return &instance;
}

// 入り口
void FieldPlayer_Wait::Enter(FieldPlayer *pPerson)
{

	// 待機モーション
	pPerson->GetObj()->SetMotion(0);
}

// 実行中
void FieldPlayer_Wait::Execute(FieldPlayer *pPerson)
{
	// リンゴを持っているか
	if (pPerson->isHave)
	{
		// Zキーで投げる
		if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
		{
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw::GetInstance());
		}
	}
	else
	{
		// リンゴとの距離が近かったら
		float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
		if (length <= 5.5f)
		{
			// Zキーで掴む
			if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
			{
				pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have::GetInstance());
			}
		}
	}


	// 左右でRunへ
	if (pPerson->isAI == true)return;

	if (tdnInput::KeyGet(KEY_LEFT,pPerson->m_PadNum))
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run::GetInstance());
	}
	if (tdnInput::KeyGet(KEY_RIGHT,pPerson->m_PadNum))
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run::GetInstance());
	}

	// ジャンプ
	if (tdnInput::KeyGet(KEY_C,pPerson->m_PadNum) == 3)
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump::GetInstance());
	}

}

// 出口
void FieldPlayer_Wait::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Wait::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Wait::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	return false;
}



/***************************************/
//	走る 
/***************************************/
FieldPlayer_Run* FieldPlayer_Run::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Run instance;
	return &instance;
}

// 入り口
void FieldPlayer_Run::Enter(FieldPlayer *pPerson)
{

	// 走りモーション。 改良：走っていたらそのまま継続
	if (pPerson->GetObj()->GetMotion() != 1)
	{
		pPerson->GetObj()->SetMotion(1);
	}

}

// 実行中
void FieldPlayer_Run::Execute(FieldPlayer *pPerson)
{
	// リンゴを持っているか
	if (pPerson->isHave)
	{
		// Zキーで投げる
		if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
		{
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw::GetInstance());
		}
	}
	else
	{
		// リンゴとの距離が近かったら
		float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
		if (length <= 5.5f)
		{
			// Zキーで掴む
			if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
			{
				pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have::GetInstance());
			}
		}
	}

	// ジャンプステートへ
	if (tdnInput::KeyGet(KEY_C,pPerson->m_PadNum) == 3)
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump::GetInstance());
	}

	// 左右でRunへ
	if (tdnInput::KeyGet(KEY_LEFT,pPerson->m_PadNum))
	{
			// アングル
		pPerson->SetAngle(-1.57f);
		pPerson->SetMove(Vector3(-0.5, pPerson->GetMove().y, pPerson->GetMove().z));

	}
	else if (tdnInput::KeyGet(KEY_RIGHT,pPerson->m_PadNum))
	{
		// アングル
		pPerson->SetAngle(1.57f);
		pPerson->SetMove(Vector3(0.5, pPerson->GetMove().y, pPerson->GetMove().z));
	}
	else //何も押していなかったら待機へ
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait::GetInstance());
	}

}

// 出口
void FieldPlayer_Run::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Run::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Run::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	ジャンプ
/***************************************/
FieldPlayer_Jump* FieldPlayer_Jump::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Jump instance;
	return &instance;
}

// 入り口
void FieldPlayer_Jump::Enter(FieldPlayer *pPerson)
{

	// ジャンプモーション
	pPerson->GetObj()->SetMotion(2);
	pPerson->SetMove(Vector3(pPerson->GetMove().x, 1.4f, pPerson->GetMove().z));

}

// 実行中
void FieldPlayer_Jump::Execute(FieldPlayer *pPerson)
{

	// リンゴを持っているか
	if (pPerson->isHave)
	{
		// Zキーで投げる
		if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
		{
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw::GetInstance());
		}
	}
	else
	{
		// リンゴとの距離が近かったら
		float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
		if (length <= 5.5f)
		{
			// Zキーで掴む
			if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
			{
				pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have::GetInstance());
			}
		}
	}

	if (tdnInput::KeyGet(KEY_LEFT,pPerson->m_PadNum))
	{
		// アングル
		pPerson->SetAngle(-1.57f);
		pPerson->SetMove(Vector3(-0.5, pPerson->GetMove().y, pPerson->GetMove().z));

	}
	else if (tdnInput::KeyGet(KEY_RIGHT,pPerson->m_PadNum))
	{
		// アングル
		pPerson->SetAngle(1.57f);
		pPerson->SetMove(Vector3(0.5, pPerson->GetMove().y, pPerson->GetMove().z));
	}

	// 着地したら終わり
	if (pPerson->IsRand() == true)
	{
			// 待機へ
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait::GetInstance());
	}

}

// 出口
void FieldPlayer_Jump::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Jump::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Jump::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}



/***************************************/
//	持つ
/***************************************/
FieldPlayer_Have* FieldPlayer_Have::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Have instance;
	return &instance;
}

// 入り口
void FieldPlayer_Have::Enter(FieldPlayer *pPerson)
{

	// 持つモーション
	pPerson->GetObj()->SetMotion(6);


	//持ちましたフラグOn
	pPerson->isHave = true;
	SOCCER_BALL.isHold = true;
	// owner設定
	SOCCER_BALL.m_owner = pPerson->GetID();
}

// 実行中
void FieldPlayer_Have::Execute(FieldPlayer *pPerson)
{
	int nom = pPerson->GetObj()->GetMotion();
	
	// 掴みフレーム終了
	if (pPerson->GetObj()->GetFrame() == 229 )
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait::GetInstance());

	}

}

// 出口
void FieldPlayer_Have::Exit(FieldPlayer *pPerson)
{


}

void FieldPlayer_Have::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Have::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	投げる
/***************************************/
FieldPlayer_throw* FieldPlayer_throw::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_throw instance;
	return &instance;
}

// 入り口
void FieldPlayer_throw::Enter(FieldPlayer *pPerson)
{
	// 投げるモーション
	pPerson->GetObj()->SetMotion(8);

}

// 実行中
void FieldPlayer_throw::Execute(FieldPlayer *pPerson)
{

	// フレーム終了
	if (pPerson->GetObj()->GetFrame() == 255)
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait::GetInstance());

	}

}

// 出口
void FieldPlayer_throw::Exit(FieldPlayer *pPerson)
{
	// リンゴ外す
	pPerson->isHave = false;
	SOCCER_BALL.isHold = false;

	if (pPerson->GetAngle() >= 1.57f)
	{
		SOCCER_BALL.SetMove(Vector3(1.8f,.5f,0));
	}
	else
	{
		SOCCER_BALL.SetMove(Vector3(-1.8f, .5f, 0));
	}
	
}

void FieldPlayer_throw::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_throw::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


/***************************************/
//	死ぬ
/***************************************/
FieldPlayer_Dead* FieldPlayer_Dead::GetInstance()
{
	// ここに変数を作る
	static FieldPlayer_Dead instance;
	return &instance;
}

// 入り口
void FieldPlayer_Dead::Enter(FieldPlayer *pPerson)
{
	// 投げるモーション
	pPerson->GetObj()->SetMotion(4);

	DeadFlame = 0;
}

// 実行中
void FieldPlayer_Dead::Execute(FieldPlayer *pPerson)
{
	DeadFlame++;
	

	// HPが０になったらもう起き上がらない
	if (pPerson->m_LifePoint <= 2) 
	{

		if (pPerson->isAI == true)
		{
			// フレーム終了
			if (DeadFlame >= 180)
			{
				// リンゴとの距離が近かったら
				float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

				// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
				if (length <= 6.5f)
				{
					//持ちましたフラグOn
					pPerson->isHave = true;
					SOCCER_BALL.isHold = true;
					// owner設定
					SOCCER_BALL.m_owner = pPerson->GetID();

					pPerson->GetFSM()->ChangeState(FieldPlayer_Have_AI::GetInstance());
				}
				else
				{
					pPerson->GetFSM()->ChangeState(FieldPlayer_Wait_AI::GetInstance());
					pPerson->m_invincibleFlame = 60;
				}

			}
		}
		else
		{
			// フレーム終了
			if (DeadFlame >= 180)
			{
				// リンゴとの距離が近かったら
				float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());
				// ある程度リンゴに近づいたらノードではなくリンゴをターゲットに！
				if (length <= 6.5f)
				{
					//持ちましたフラグOn
					pPerson->isHave = true;
					SOCCER_BALL.isHold = true;
					// owner設定
					SOCCER_BALL.m_owner = pPerson->GetID();

					pPerson->GetFSM()->ChangeState(FieldPlayer_Have::GetInstance());
				}
				else
				{
					pPerson->GetFSM()->ChangeState(FieldPlayer_Wait::GetInstance());
					pPerson->m_invincibleFlame = 60;
				}

				

			}

		}
	}
	else
	{					//持ちましたフラグOFF
		pPerson->isHave = false;
		SOCCER_BALL.isHold = false;

	}
}

// 出口
void FieldPlayer_Dead::Exit(FieldPlayer *pPerson)
{
	// ★


}

void FieldPlayer_Dead::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Dead::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// メッセージタイプ

	// Flaseで返すとグローバルステートのOnMessageの処理へ行く
	return false;
}


