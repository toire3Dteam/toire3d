#include "FieldPlayerState.h"
#include "AI\NPC\FieldPlayer\FieldPlayer.h"
#include "AI\Message\Message.h"
#include "AI\Message\MessageDispatcher.h"
#include "../../../Sound/SoundManager.h"

#include "SoccerBall\SoccerBall.h" // �T�b�J�[�{�[��


/***************************************/
//	�O���[�o���X�e�[�g_AI
/***************************************/
FieldPlayer_GlobalState_AI* FieldPlayer_GlobalState_AI::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_GlobalState_AI instance;
	return &instance;
}

// �����
void FieldPlayer_GlobalState_AI::Enter(FieldPlayer *pPerson)
{

}

// ���s��
void FieldPlayer_GlobalState_AI::Execute(FieldPlayer *pPerson)
{
	//if (tdnInput::KeyGet(KEY_A, 0) == 3)
	//{

	//}

	// ����ł��疳��!!!
	if (pPerson->GetFSM()->GetCurrentState() != FieldPlayer_Dead::GetInstance())
	{

		// �s������
		if (SOCCER_BALL.isHold == true && pPerson->isHave == false)
		{
			// �����Ă��Ȃ������烊���S���^�[�Q�b�g��
			FieldNode*  targetNode = pPerson->GetGraph()->FarPosNode(SOCCER_BALL.GetPos());
			FieldNode*  nearNode = pPerson->GetGraph()->NearPosNode(pPerson->GetPos());
			// �����S�̈ʒu���ς���Ă�����
			//if (targetNode != pPerson->m_goalNode)
			{
				// ����ɃL�����N�^�[�̃^�X�N���S�ďI�����Ă�����I
				if (pPerson->m_task.empty())
				{
					// ��ԉ����ꏊ�ɂ����烉���_��
					if (nearNode == nearNode)
					{
						pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
						pPerson->taskSetting();
					}
					else
					{
						// �����ꏊ��
						pPerson->m_goalPos = targetNode->m_pos;
						pPerson->taskSetting();

					}

				}
			}


		}
		// �����S�������Ă��邩
		else if (pPerson->isHave == false)
		{
			// �����Ă��Ȃ������烊���S���^�[�Q�b�g��
			FieldNode*  targetNode = pPerson->GetGraph()->NearPosNode(SOCCER_BALL.GetPos());

			// �����S�̈ʒu���ς���Ă�����
			//if (targetNode != pPerson->m_goalNode)
			{

				DEBUG_FLAME++;
				// ����ɃL�����N�^�[�̃^�X�N���S�ďI�����Ă�����I
				if (pPerson->m_task.empty())
				{
					// ����!
					// �����S���^�[�Q�b�g��
					//pPerson->SetTargetPos(SOCCER_BALL.GetPos());
					//pPerson->m_goalPos = SOCCER_BALL.GetPos();
					//pPerson->taskSetting();

					// �������Ńv���C���[�S��������
					PLAYER_INFO ex;
					ex.pos = pPerson->GetPos();
					ex.id = pPerson->GetID();

					// �}�l�[�W���[�֑��M 
					MsgMgr->Dispatch(
						MSG_NO_DELAY,
						pPerson->GetID(),
						ENTITY_ID::PERSON_MNG,
						SOCCER_BALL_NEAR_PLAYER,
						(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
						);
				}
				else if (DEBUG_FLAME >= 60 * 3)
				{
					DEBUG_FLAME = 0;

					// �������Ńv���C���[�S��������
					PLAYER_INFO ex;
					ex.pos = pPerson->GetPos();
					ex.id = pPerson->GetID();

					pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
					pPerson->taskSetting();

					// �}�l�[�W���[�֑��M 
					//MsgMgr->Dispatch(
					//	MSG_NO_DELAY,
					//	pPerson->GetID(),
					//	ENTITY_ID::PERSON_MNG,
					//	SOCCER_BALL_NEAR_PLAYER,
					//	(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
					//	);
				}

				// �^�[�Q�b�g�̃|�W�V�����Ƃ��̃����S�̃|�W�V����������Ă���
				// �^�X�N����
				if (!pPerson->m_task.empty())// �^�X�N�������
				{
					TASK ta = pPerson->m_task.top();
					if (targetNode->m_pos == ta.pos)
					{
						// �O�̃^�X�N�N���A
						// ��ɂȂ�܂Ń|�b�v
						//for (;;)
						//{
						//	// �󂾂�����A��
						//	if (pPerson->m_task.empty()) { break; }// 
						//	pPerson->m_task.pop();
						//}

						// ��ɂ������Ƃɂ�聪�̕��֍s���Ȃ����߂̉��}���u
						pPerson->m_goalNode = targetNode;
					}

				}
			}

			// �^�X�N����̎�
			//if (!pPerson->m_task.empty())

			if (pPerson->IsRand())// ���n���Ă���Ƃ�
			{
				// �����S�Ƃ̋������߂�������
				float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

				// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
				if (length <= 15.5f)
				{
					// �����S���^�[�Q�b�g��
					pPerson->SetTargetPos(SOCCER_BALL.GetPos());
					// ���̂��ߕ����X�e�[�g��
					pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());


					for (;;)
					{
						// �󂾂�����A��
						if (pPerson->m_task.empty()) { break; }// 
						pPerson->m_task.pop();
					}

					// �����͈�
					if (length <= 5.5f)
					{
						// �����S���n�ʂɂ��Ă�����
						if (SOCCER_BALL.isRand)
						{
							// ���X�e�[�g��
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have_AI::GetInstance());
						}
					}
				}
			}


		}
		else
		{


			/*************************************/
			//		�^�X�N�̐ݒ�
			/********************************/
				// �L�����N�^�[�̃^�X�N���S�ďI�����Ă�����I
			if (pPerson->m_task.empty())
			{
				// �������Ńv���C���[�S��������
				PLAYER_INFO ex;
				ex.pos = pPerson->GetPos();
				ex.id = pPerson->GetID();

				// �}�l�[�W���[���M 
				MsgMgr->Dispatch(
					MSG_NO_DELAY,
					pPerson->GetID(),
					ENTITY_ID::PERSON_MNG,
					TARGET_PLAYERS,
					(void*)&ex	// [�ǋL���]�����̃^�C�v�𑗂�
					);

				// ����!
				// �^�X�N�Z�b�g
			}


			// ������X�e�[�g�̂Ƃ��͗��Ȃ��ł�������
			if (pPerson->GetFSM()->GetCurrentState() != FieldPlayer_throw_AI::GetInstance())
			{

				/**********************************/
				// �^�[�Q�b�g�Ƃ̋������߂�������
				/**********************************/
				if (pPerson->m_nearPlayer == NULL) //NULL �������
				{
					MyDebugString("NULL������");
				}
				else
				{
					// �|�C���^�\�ɕύX
					float length = Math::Length(pPerson->GetPos(), pPerson->m_nearPlayer->GetPos());

					// ������傫�����Ȃ��ƃm�[�h�Ƃ̊֌W�ŋ߂��ɂ��Ă������Ȃ��Ƃ����o�Ă���
					// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
					if (length <= 30.5f)
					{
						//�����O�A�x�����㉺�i��
						float height = abs(pPerson->m_goalPos.y - pPerson->GetPos().y);
						if (height <= 5)
						{



							// �A���O������
							if (pPerson->m_goalPos.x >= pPerson->GetPos().x)
							{
								pPerson->SetAngle(1.57f);
							}
							else
							{
								pPerson->SetAngle(-1.57f);
							}

							// ������X�e�[�g��
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw_AI::GetInstance());
							for (;;)
							{
								// �󂾂�����A��
								if (pPerson->m_task.empty()) { break; }//	 
								pPerson->m_task.pop();
							}

						}

					}
				}
	

			}

			// �^�[�Q�b�g�̃|�W�V�����Ƃ��̃����S�̃|�W�V����������Ă���
			// �^�X�N����
			//if (!pPerson->m_task.empty())// �^�X�N�������
			//{
			//	TASK ta = pPerson->m_task.top();
			//	if (targetNode->m_pos == ta.pos)
			//	{
			//		// �O�̃^�X�N�N���A
			//		// ��ɂȂ�܂Ń|�b�v
			//		for (;;)
			//		{
			//			// �󂾂�����A��
			//			if (pPerson->m_task.empty()) { break; }// 
			//			pPerson->m_task.pop();
			//		}

			//		// ��ɂ������Ƃɂ�聪�̕��֍s���Ȃ����߂̉��}���u
			//		pPerson->m_goalNode = targetNode;
			//	}

			//}


		// �^�X�N����̎�
		//if (!pPerson->m_task.empty())
		//if (pPerson->IsRand())// ���n���Ă���Ƃ�
		//{
		//	// �����S�Ƃ̋������߂�������
		//	float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		//	// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
		//	if (length <= 15.5f)
		//	{
		//		// �����S���^�[�Q�b�g��
		//		pPerson->SetTargetPos(SOCCER_BALL.GetPos());
		//		// ���̂��ߕ����X�e�[�g��
		//		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());



		}





		/*******************/
		// �^�X�N
		/*******************/

		// �����S�������ĂȂ��Ƃ�
		if (pPerson->isHave == false)
		{
			// �i�V�ѐS�j���n����܂ōX�V�͋�����
			if (pPerson->IsRand() == true)
			{
				// �^�X�N����
				// �󂶂�Ȃ�������
				if (!pPerson->m_task.empty())
				{

					TASK ta = pPerson->m_task.top();
					//���@�^�[�Q�b�g�ݒ�	
					if (pPerson->GetTargetPos() != ta.pos)
					{
						// �^�[�Q�b�g�X�V
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
					// �߂Â�����^�X�N�����o�����̃^�X�N��
					if (length <= 6.5f)
					{
						pPerson->m_task.pop();

						// ���̂ŋ�ɂȂ�����
						if (pPerson->m_task.empty())
						{
							// �����ŃW�����v������ɂ����ɂ��Ă����Ă邩��o�O��
							// �ҋ@��
							pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
						}
					}

				}
			}

		}
		else // �����S�������Ă���
		{
			SOCCER_BALL.SetPos(pPerson->GetObj()->WorldBonePos(8));

			// ������X�e�[�g�̂Ƃ��͗��Ȃ��ł�������
			if (pPerson->GetFSM()->GetCurrentState() != FieldPlayer_throw_AI::GetInstance())
			{

				// �i�V�ѐS�j���n����܂ōX�V�͋�����
				if (pPerson->IsRand() == true)
				{
					// �^�X�N����
					// �󂶂�Ȃ�������
					if (!pPerson->m_task.empty())
					{

						TASK ta = pPerson->m_task.top();
						//���@�^�[�Q�b�g�ݒ�	
						if (pPerson->GetTargetPos() != ta.pos)
						{
							// �^�[�Q�b�g�X�V
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
						// �߂Â�����^�X�N�����o�����̃^�X�N��
						if (length <= 6.5f)
						{
							pPerson->m_task.pop();

							// ���̂ŋ�ɂȂ�����
							if (pPerson->m_task.empty())
							{
								// �����ŃW�����v������ɂ����ɂ��Ă����Ă邩��o�O��
								// �ҋ@��
								pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
							}
						}

					}
				}
			}// ������X�e�[�g

		}


	}
}

// �o��
void FieldPlayer_GlobalState_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_GlobalState_AI::Render(FieldPlayer * pPerson)
{

}

//
bool FieldPlayer_GlobalState_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SEND_PLAYER_POS:
	{
		MyDebugString("[%d]����߂��l�̋����͂�����\n", msg.Sender);

		// �߂��v���C���[�̏ꏊ�擾�I
		//Vector3* nearPos = (Vector3*)msg.ExtraInfo;
		pPerson->m_nearPlayer =(BasePerson*)msg.ExtraInfo;
		//int* nearPlayer = (int*)msg.ExtraInfo;

		if (pPerson->m_nearPlayer->GetID() == pPerson->GetID())
		{
			MyDebugString("�N�����Ȃ��Ȃ����E�E�E\n");
		}
		else
		{

			//pPerson->SetTargetPos(*(Vector3*)msg.ExtraInfo);
			//pPerson->m_goalPos = *(Vector3*)msg.ExtraInfo;   // ������X�^�b�N���[���̎�VECTOR_ZERO�ŗ��邩��������E�E
			pPerson->m_goalPos = pPerson->m_nearPlayer->GetPos();
			//pPerson->SetTargetPos(*nearPos);
			pPerson->taskSetting();

		}
		return true;


	}
		break;
	case MESSAGE_TYPE::SEND_NEAR_PLAYER:
	{
		MyDebugString("[%d]����߂��l�̋����͂����Q\n", msg.Sender);

		// �߂��v���C���[�̏ꏊ�擾�I
		ENTITY_ID* id = (ENTITY_ID*)msg.ExtraInfo;

		// �����Ă��Ȃ������烊���S�����ԂƂ����ꏊ
		FieldNode*  targetNode = pPerson->GetGraph()->FarPosNode(SOCCER_BALL.GetPos());
		FieldNode*  nearNode = pPerson->GetGraph()->NearPosNode(pPerson->GetPos());
		if (pPerson->GetID() == *id)
		{
			// �����S���^�[�Q�b�g��
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
			// �����Ă��Ȃ������烊���S�����ԂƂ����ꏊ
			FieldNode*  targetNode = pPerson->GetGraph()->FarPosNode(SOCCER_BALL.GetPos());

			// �����S���^�[�Q�b�g��
			pPerson->m_goalPos = targetNode->m_pos;
			pPerson->taskSetting();
			
		}



		return true;
	}
	break;
	default:

		break;
	}//end switch

	 // Flase�ŕԂ��ƃO���[�o���X�e�[�g�Ȃ̂Ő��^�����̏I��
	return false;
}



/***************************************/
//	�ҋ@ AI
/***************************************/
FieldPlayer_Wait_AI* FieldPlayer_Wait_AI::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Wait_AI instance;
	return &instance;
}

// �����
void FieldPlayer_Wait_AI::Enter(FieldPlayer *pPerson)
{
	m_MoveFlame = 0;
	// �ҋ@���[�V����
	pPerson->GetObj()->SetMotion(0);
}

// ���s��
void FieldPlayer_Wait_AI::Execute(FieldPlayer *pPerson)
{
	//pPerson->SetAngle(pPerson->GetAngle() + 0.1f);

	//// �����Ŕg�䂪IsEnd�ɂȂ�����
	//pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance()->GetInstance());

	//�����Ɨ����Ă�����C���œ�����
	m_MoveFlame++;
	if (m_MoveFlame > 60)
	{
		pPerson->m_goalPos = Vector3((rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50);
		pPerson->taskSetting();
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
	}

	// m_isShed = true �ɂȂ����痬�����ꏊ�֍s��
	//if (pPerson->IsShed() == true)
	//{
	//	pPerson->GetFSM()->ChangeState(PersonShedWait::GetInstance());
	//}
}

// �o��
void FieldPlayer_Wait_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Wait_AI::Render(FieldPlayer * pPerson)
{
	//pPerson->GetObj()->Render(shader, "copy");
}

bool FieldPlayer_Wait_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v
	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::Riple_Red:

	//	// �����̉\�̔g����o���B
	//	pPerson->ActionGossipRipple();

	//	return true;

	//	break;
	//default:

	//	break;
	//}//end switch
	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	���� AI
/***************************************/
FieldPlayer_Run_AI* FieldPlayer_Run_AI::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Run_AI instance;
	return &instance;
}

// �����
void FieldPlayer_Run_AI::Enter(FieldPlayer *pPerson)
{


	// ���胂�[�V�����B ���ǁF�����Ă����炻�̂܂܌p��
	if (pPerson->GetObj()->GetMotion() != 1)
	{
		pPerson->GetObj()->SetMotion(1);
	}
	
}

// ���s��
void FieldPlayer_Run_AI::Execute(FieldPlayer *pPerson)
{
	// �ړ�����


	// �󂶂�Ȃ�������
	//if (!pPerson->m_task.empty())
	//{

		//idou
		//Vector3 move = pPerson->GetTargetPos() - pPerson->GetPos();
		//move.Normalize();

		// �ړ�
		//pPerson->SetMove(move);
		if (pPerson->GetTargetPos().x > pPerson->GetPos().x)
		{
			// �A���O��
			pPerson->SetAngle(1.57f);
			pPerson->SetMove(Vector3(0.5, pPerson->GetMove().y, pPerson->GetMove().z));

		}
		else if (pPerson->GetTargetPos().x < pPerson->GetPos().x - 1.5f)
		{
			// �A���O��
			pPerson->SetAngle(-1.57f);
			pPerson->SetMove(Vector3(-0.5, pPerson->GetMove().y, pPerson->GetMove().z));
			//pPerson->SetMove(pPerson->GetMove() + Vector3(-1, 0, 0));
		}
		else //�@�����^��or�^��
		{
			// �ҋ@��
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
		}

		//float length = Math::Length(pPerson->GetPos(), task.pos);

		//if (length <= 15)
		//{
		//	pPerson->m_task.pop();
		//}

	//}

		// �ǂɂԂ������B�Ԃ���Ȃ��瑖���Ă���
		if (pPerson->IsCollWall()==true)
		{
			// �W�����v
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump_AI::GetInstance());
		}

}

// �o��
void FieldPlayer_Run_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Run_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Run_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	�W�����v AI
/***************************************/
FieldPlayer_Jump_AI* FieldPlayer_Jump_AI::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Jump_AI instance;
	return &instance;
}

// �����
void FieldPlayer_Jump_AI::Enter(FieldPlayer *pPerson)
{

	// �W�����v���[�V����
	pPerson->GetObj()->SetMotion(2);
	pPerson->SetMove(Vector3(pPerson->GetMove().x, 1.4f , pPerson->GetMove().z));

}

// ���s��
void FieldPlayer_Jump_AI::Execute(FieldPlayer *pPerson)
{
	// �ړ�����
	if (pPerson->GetTargetPos().x > pPerson->GetPos().x)
	{
		// �A���O��
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

		// �A���O��
		pPerson->SetAngle(-1.57f);
		pPerson->SetMove(Vector3(-0.6f, pPerson->GetMove().y, pPerson->GetMove().z));
		

		//pPerson->SetMove(pPerson->GetMove() + Vector3(-0.5f, 0, 0));
		//if (pPerson->GetMove().x <= -1.5f)
		//{
		//	pPerson->SetMove(Vector3(-1.5f, pPerson->GetMove().y, pPerson->GetMove().z));
		//}
	}
	else //�@�����^��or�^��
	{	
		// �ҋ@��
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
	}

	// ���n������I���
	if (pPerson->IsRand()==true)
	{
		// �󂶂�Ȃ�������@������
		if (!pPerson->m_task.empty())
		{
			// ���邺
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
		}
		else
		{
			// �ҋ@��
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
		}
	}

}

// �o��
void FieldPlayer_Jump_AI::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Jump_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Jump_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	���� AI
/***************************************/
FieldPlayer_Have_AI* FieldPlayer_Have_AI::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Have_AI instance;
	return &instance;
}

// �����
void FieldPlayer_Have_AI::Enter(FieldPlayer *pPerson)
{

	// �����[�V����
	pPerson->GetObj()->SetMotion(6);

	//�����܂����t���OOn
	pPerson->isHave = true;
	SOCCER_BALL.isHold = true;
	// owner�ݒ�
	SOCCER_BALL.m_owner = pPerson->GetID();

}

// ���s��
void FieldPlayer_Have_AI::Execute(FieldPlayer *pPerson)
{

	// �͂݃t���[���I��
	if (pPerson->GetObj()->GetFrame() == 229)
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait_AI::GetInstance());
	}

}

// �o��
void FieldPlayer_Have_AI::Exit(FieldPlayer *pPerson)
{
	//�����܂����t���OOn
	//pPerson->isHave = true;
}

void FieldPlayer_Have_AI::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Have_AI::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	������ AI
/***************************************/
FieldPlayer_throw_AI* FieldPlayer_throw_AI::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_throw_AI instance;
	return &instance;
}

// �����
void FieldPlayer_throw_AI::Enter(FieldPlayer *pPerson)
{
	// �����郂�[�V����
	pPerson->GetObj()->SetMotion(8);

}

// ���s��
void FieldPlayer_throw_AI::Execute(FieldPlayer *pPerson)
{
	// �A���O������
	if (pPerson->m_goalPos.x >= pPerson->GetPos().x)
	{
		pPerson->SetAngle(1.57f);
	}
	else
	{
		pPerson->SetAngle(-1.57f);
	}

	// �t���[���I��
	if (pPerson->GetObj()->GetFrame() == 255)
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait_AI::GetInstance());

	}

}

// �o��
void FieldPlayer_throw_AI::Exit(FieldPlayer *pPerson)
{
	// �����S�O��
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

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}






/***************************************/
//	�O���[�o���X�e�[�g
/***************************************/
FieldPlayer_GlobalState* FieldPlayer_GlobalState::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_GlobalState instance;
	return &instance;
}

// �����
void FieldPlayer_GlobalState::Enter(FieldPlayer *pPerson)
{

}

// ���s��
void FieldPlayer_GlobalState::Execute(FieldPlayer *pPerson)
{
	if (pPerson->isHave)
	{
		SOCCER_BALL.SetPos(pPerson->GetObj()->WorldBonePos(8));
	}
	
}

// �o��
void FieldPlayer_GlobalState::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_GlobalState::Render(FieldPlayer * pPerson)
{

}

//
bool FieldPlayer_GlobalState::OnMessage(FieldPlayer *pPerson, const Message &msg)
{
	// ���b�Z�[�W�^�C�v
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TARGET_PLAYERS:

		//printf("�Ԃ��g��ɓ���������\n");
		//MyDebugString("�Ԃ��g��ɓ���������\n");

		// �Ԃ��g��ɓ���������

		return true;

		break;
	default:

		break;
	}//end switch

	 // Flase�ŕԂ��ƃO���[�o���X�e�[�g�Ȃ̂Ő��^�����̏I��
	return false;
}


/***************************************/
//	�ҋ@ 
/***************************************/
FieldPlayer_Wait* FieldPlayer_Wait::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Wait instance;
	return &instance;
}

// �����
void FieldPlayer_Wait::Enter(FieldPlayer *pPerson)
{

	// �ҋ@���[�V����
	pPerson->GetObj()->SetMotion(0);
}

// ���s��
void FieldPlayer_Wait::Execute(FieldPlayer *pPerson)
{
	// �����S�������Ă��邩
	if (pPerson->isHave)
	{
		// Z�L�[�œ�����
		if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
		{
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw::GetInstance());
		}
	}
	else
	{
		// �����S�Ƃ̋������߂�������
		float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
		if (length <= 5.5f)
		{
			// Z�L�[�Œ͂�
			if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
			{
				pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have::GetInstance());
			}
		}
	}


	// ���E��Run��
	if (pPerson->isAI == true)return;

	if (tdnInput::KeyGet(KEY_LEFT,pPerson->m_PadNum))
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run::GetInstance());
	}
	if (tdnInput::KeyGet(KEY_RIGHT,pPerson->m_PadNum))
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Run::GetInstance());
	}

	// �W�����v
	if (tdnInput::KeyGet(KEY_C,pPerson->m_PadNum) == 3)
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump::GetInstance());
	}

}

// �o��
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
//	���� 
/***************************************/
FieldPlayer_Run* FieldPlayer_Run::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Run instance;
	return &instance;
}

// �����
void FieldPlayer_Run::Enter(FieldPlayer *pPerson)
{

	// ���胂�[�V�����B ���ǁF�����Ă����炻�̂܂܌p��
	if (pPerson->GetObj()->GetMotion() != 1)
	{
		pPerson->GetObj()->SetMotion(1);
	}

}

// ���s��
void FieldPlayer_Run::Execute(FieldPlayer *pPerson)
{
	// �����S�������Ă��邩
	if (pPerson->isHave)
	{
		// Z�L�[�œ�����
		if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
		{
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw::GetInstance());
		}
	}
	else
	{
		// �����S�Ƃ̋������߂�������
		float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
		if (length <= 5.5f)
		{
			// Z�L�[�Œ͂�
			if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
			{
				pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have::GetInstance());
			}
		}
	}

	// �W�����v�X�e�[�g��
	if (tdnInput::KeyGet(KEY_C,pPerson->m_PadNum) == 3)
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Jump::GetInstance());
	}

	// ���E��Run��
	if (tdnInput::KeyGet(KEY_LEFT,pPerson->m_PadNum))
	{
			// �A���O��
		pPerson->SetAngle(-1.57f);
		pPerson->SetMove(Vector3(-0.5, pPerson->GetMove().y, pPerson->GetMove().z));

	}
	else if (tdnInput::KeyGet(KEY_RIGHT,pPerson->m_PadNum))
	{
		// �A���O��
		pPerson->SetAngle(1.57f);
		pPerson->SetMove(Vector3(0.5, pPerson->GetMove().y, pPerson->GetMove().z));
	}
	else //���������Ă��Ȃ�������ҋ@��
	{
		pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait::GetInstance());
	}

}

// �o��
void FieldPlayer_Run::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Run::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Run::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	�W�����v
/***************************************/
FieldPlayer_Jump* FieldPlayer_Jump::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Jump instance;
	return &instance;
}

// �����
void FieldPlayer_Jump::Enter(FieldPlayer *pPerson)
{

	// �W�����v���[�V����
	pPerson->GetObj()->SetMotion(2);
	pPerson->SetMove(Vector3(pPerson->GetMove().x, 1.4f, pPerson->GetMove().z));

}

// ���s��
void FieldPlayer_Jump::Execute(FieldPlayer *pPerson)
{

	// �����S�������Ă��邩
	if (pPerson->isHave)
	{
		// Z�L�[�œ�����
		if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
		{
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_throw::GetInstance());
		}
	}
	else
	{
		// �����S�Ƃ̋������߂�������
		float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

		// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
		if (length <= 5.5f)
		{
			// Z�L�[�Œ͂�
			if (tdnInput::KeyGet(KEY_A,pPerson->m_PadNum) == 3)
			{
				pPerson->GetFSM()->SetCurrentState(FieldPlayer_Have::GetInstance());
			}
		}
	}

	if (tdnInput::KeyGet(KEY_LEFT,pPerson->m_PadNum))
	{
		// �A���O��
		pPerson->SetAngle(-1.57f);
		pPerson->SetMove(Vector3(-0.5, pPerson->GetMove().y, pPerson->GetMove().z));

	}
	else if (tdnInput::KeyGet(KEY_RIGHT,pPerson->m_PadNum))
	{
		// �A���O��
		pPerson->SetAngle(1.57f);
		pPerson->SetMove(Vector3(0.5, pPerson->GetMove().y, pPerson->GetMove().z));
	}

	// ���n������I���
	if (pPerson->IsRand() == true)
	{
			// �ҋ@��
			pPerson->GetFSM()->SetCurrentState(FieldPlayer_Wait::GetInstance());
	}

}

// �o��
void FieldPlayer_Jump::Exit(FieldPlayer *pPerson)
{

}

void FieldPlayer_Jump::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Jump::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}



/***************************************/
//	����
/***************************************/
FieldPlayer_Have* FieldPlayer_Have::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Have instance;
	return &instance;
}

// �����
void FieldPlayer_Have::Enter(FieldPlayer *pPerson)
{

	// �����[�V����
	pPerson->GetObj()->SetMotion(6);


	//�����܂����t���OOn
	pPerson->isHave = true;
	SOCCER_BALL.isHold = true;
	// owner�ݒ�
	SOCCER_BALL.m_owner = pPerson->GetID();
}

// ���s��
void FieldPlayer_Have::Execute(FieldPlayer *pPerson)
{
	int nom = pPerson->GetObj()->GetMotion();
	
	// �͂݃t���[���I��
	if (pPerson->GetObj()->GetFrame() == 229 )
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait::GetInstance());

	}

}

// �o��
void FieldPlayer_Have::Exit(FieldPlayer *pPerson)
{


}

void FieldPlayer_Have::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Have::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	������
/***************************************/
FieldPlayer_throw* FieldPlayer_throw::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_throw instance;
	return &instance;
}

// �����
void FieldPlayer_throw::Enter(FieldPlayer *pPerson)
{
	// �����郂�[�V����
	pPerson->GetObj()->SetMotion(8);

}

// ���s��
void FieldPlayer_throw::Execute(FieldPlayer *pPerson)
{

	// �t���[���I��
	if (pPerson->GetObj()->GetFrame() == 255)
	{
		pPerson->GetFSM()->ChangeState(FieldPlayer_Wait::GetInstance());

	}

}

// �o��
void FieldPlayer_throw::Exit(FieldPlayer *pPerson)
{
	// �����S�O��
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

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


/***************************************/
//	����
/***************************************/
FieldPlayer_Dead* FieldPlayer_Dead::GetInstance()
{
	// �����ɕϐ������
	static FieldPlayer_Dead instance;
	return &instance;
}

// �����
void FieldPlayer_Dead::Enter(FieldPlayer *pPerson)
{
	// �����郂�[�V����
	pPerson->GetObj()->SetMotion(4);

	DeadFlame = 0;
}

// ���s��
void FieldPlayer_Dead::Execute(FieldPlayer *pPerson)
{
	DeadFlame++;
	

	// HP���O�ɂȂ���������N���オ��Ȃ�
	if (pPerson->m_LifePoint <= 2) 
	{

		if (pPerson->isAI == true)
		{
			// �t���[���I��
			if (DeadFlame >= 180)
			{
				// �����S�Ƃ̋������߂�������
				float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());

				// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
				if (length <= 6.5f)
				{
					//�����܂����t���OOn
					pPerson->isHave = true;
					SOCCER_BALL.isHold = true;
					// owner�ݒ�
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
			// �t���[���I��
			if (DeadFlame >= 180)
			{
				// �����S�Ƃ̋������߂�������
				float length = Math::Length(pPerson->GetPos(), SOCCER_BALL.GetPos());
				// ������x�����S�ɋ߂Â�����m�[�h�ł͂Ȃ������S���^�[�Q�b�g�ɁI
				if (length <= 6.5f)
				{
					//�����܂����t���OOn
					pPerson->isHave = true;
					SOCCER_BALL.isHold = true;
					// owner�ݒ�
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
	{					//�����܂����t���OOFF
		pPerson->isHave = false;
		SOCCER_BALL.isHold = false;

	}
}

// �o��
void FieldPlayer_Dead::Exit(FieldPlayer *pPerson)
{
	// ��


}

void FieldPlayer_Dead::Render(FieldPlayer * pPerson)
{

}

bool FieldPlayer_Dead::OnMessage(FieldPlayer *pPerson, const Message &msg)
{

	//// ���b�Z�[�W�^�C�v

	// Flase�ŕԂ��ƃO���[�o���X�e�[�g��OnMessage�̏����֍s��
	return false;
}


