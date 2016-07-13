#include "FieldPlayer.h"
#include "FieldPlayerState.h"
#include "AI\FieldGraph\FieldGraph.h"
#include "SoccerBall\SoccerBall.h"

FieldPlayer::FieldPlayer(ENTITY_ID id, bool isAi) :BasePerson(id)
{

	// AI
	isAI = isAi;

	if (isAI)
	{

		m_obj = new iex3DObj("DATA/Player/nazenaraba_toire.IEM");
	}
	else
	{
		if (0 == (int)(GetID() - GAME_ID::id_person_first))
		{
			m_obj = new iex3DObj("DATA/Player2/nazenaraba_toire.IEM");
		}
		else if (1 == (int)(GetID() - GAME_ID::id_person_first))
		{
			m_obj = new iex3DObj("DATA/Player3/nazenaraba_toire.IEM");

		}
		else
		{
			m_obj = new iex3DObj("DATA/Player4/nazenaraba_toire.IEM");
		}
	}

	// ステートマシン　引数は自分自身のポインタ
	m_pStateMachine = new StateMachine<FieldPlayer>(this);
	if (isAI)
	{
		m_pStateMachine->SetGlobalState(FieldPlayer_GlobalState_AI::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
	}
	else
	{
		m_pStateMachine->SetGlobalState(FieldPlayer_GlobalState::GetInstance());// グローバル
		m_pStateMachine->SetCurrentState(FieldPlayer_Wait::GetInstance());

	}

	// ステートマシン　引数は自分自身のポインタ
	//m_pStateMachine = new StateMachine<FieldPlayer>(this);
	//m_pStateMachine->SetGlobalState(FieldPlayer_GlobalState_AI::GetInstance());// グローバル
	//m_pStateMachine->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());

	// グラフ初期化
	m_graph = new FieldGraph();
	m_graph->LoadNode("DATA/AI/suikaNode3.txt");

	m_targetPos = VECTOR_ZERO;
	m_goalPos= VECTOR_ZERO;
	isHave = false;

	m_goalNode = nullptr;

	isAI = false;
	m_nearPlayer = nullptr;

	m_gage = new tdn2DObj("Data/UI/GAGE.png");
	m_life = new tdn2DObj("Data/UI/life.png");
	m_LifePoint = 0;

	m_invincibleFlame = 0;

	m_PadNum =  (int)(GetID() - GAME_ID::id_person_first);
	if (m_PadNum >= 3)
	{
		m_PadNum = 3;
	}


}

FieldPlayer::~FieldPlayer()
{
	SAFE_DELETE(m_graph);
	SAFE_DELETE(m_pStateMachine);
	SAFE_DELETE(m_gage);
	SAFE_DELETE(m_life);

}

void FieldPlayer::Update()
{

	// ★ステートマシン更新
	m_pStateMachine->Update();

	if (KeyBoard('B')==1)
	{
		isHave = false;
		taskSetting();

		//m_pStateMachine->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
	}

	//if (isAI)
	//{
	//	AI();
	//}
	//else
	//{
	//	Ctrl();
	//}

	// 

	// ベースプレイヤー
	BasePerson::Update();


	if (m_invincibleFlame <= 0)
	{

		// 死んでたら無効
		if (m_pStateMachine->GetCurrentState() != FieldPlayer_Dead::GetInstance())
		{
			// 横に動いているとき
			if (SOCCER_BALL.GetMove().x != 0.0f)
			{
				// 同じだったらパス
				if (SOCCER_BALL.m_owner == (ENTITY_ID)GetID())return;

				if (Math::Length(SOCCER_BALL.GetPos(), m_pos) < 10)
				{
					m_LifePoint++;
					m_pStateMachine->SetCurrentState(FieldPlayer_Dead::GetInstance());
				}
			}

		}
	}
	else
	{
		m_invincibleFlame--;

	}

}
void FieldPlayer::Ctrl()
{

}
// AI
void FieldPlayer::AI()
{

	// リンゴを持ってないとき
	if (isHave == false)
	{
		// （遊び心）着地するまで更新は許さん
		if (m_bRand == true)
		{
			// タスク処理
			// 空じゃなかったら
			if (!m_task.empty())
			{

				TASK ta = m_task.top();
				//★　ターゲット設定	
				if (m_targetPos != ta.pos)
				{
					// ターゲット更新
					m_targetPos = ta.pos;

					switch (ta.fieldState)
					{
					case FIELD_STATE::RUN:
						m_pStateMachine->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
						break;
					case FIELD_STATE::JUMP:
						m_pStateMachine->SetCurrentState(FieldPlayer_Jump_AI::GetInstance());
						break;
						//case FIELD_STATE::DOWN:
						//m_pStateMachine->SetCurrentState(FieldPlayer_Run_AI::GetInstance());
						//break;
					default:
						break;
					}
				}


				float length = Math::Length(m_pos, m_targetPos);
				// 近づいたらタスクを放り出し次のタスクへ
				if (length <= 6.5f)
				{
					m_task.pop();

					// ↑ので空になったら
					if (m_task.empty())
					{
						// 待機へ
						m_pStateMachine->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
					}
				}

			}
		}

	}
	else // リンゴを持っている
	{
		SOCCER_BALL.SetPos(m_obj->WorldBonePos(8));

	}

};

void FieldPlayer::Render()
{
	m_obj->Render();

	// このステートマシンの描画
	m_pStateMachine->Render();

	// グラフ
	//m_graph->Render();

	int id = (int)(GetID()-GAME_ID::id_person_first );
	// ゲージ
	m_gage->Render(20 + (id * 326) , 0, 256, 128, +(id * 256), 0, 256, 128);
	m_life->Render(140 + (id * 326), 0, 128, 128, +(m_LifePoint * 128), 0, 128, 128);

	BasePerson::Render();
	BasePerson::UIRender();
};

// ステートマシンのメッセージ
bool FieldPlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void FieldPlayer::taskSetting()
{
	// ★(TODO)↓ステート帰るために無理やり
	//m_targetPos = VECTOR_ZERO;

	// まずは全ノードとの距離を計算
	//navi->	 targetPos
	m_goalNode = m_graph->NearPosNode(m_goalPos);

	// スタートは自分のポジション
	FieldNode* startNode = m_graph->NearPosNode(m_pos);

	// 経路探索開始！（A*）
	m_graph->Planning(startNode, m_goalNode);


	/*
	↑でm_GoalFieldNodeを設定したので後はポジションを詰めていく
	*/
	FieldNode* N;
	N = m_graph->m_GoalFieldNode;

	if (N == nullptr)
	{
		return;
	}

	// 前のタスククリア
	// 空になるまでポップ
	for (;;)
	{
		// 空だったら帰れ
		if (m_task.empty()) { break; }// 
		m_task.pop();
	}

	// 新しく入れる
	TASK task;
	task.pos = VECTOR_ZERO;
	task.fieldState = FIELD_STATE::RUN;// 初期
	for (;;)
	{
		// 詰め込む
		task.pos = N->m_pos;
		task.fieldState = N->m_fieldState;

		// ポジションを入れる
		m_task.push(task);

		// 親はフィールドノードのていでキャスト
		N = (FieldNode*)N->m_parent;


		if (N == nullptr)
		{
			break;
		}
	}

}