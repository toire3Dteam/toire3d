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

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<FieldPlayer>(this);
	if (isAI)
	{
		m_pStateMachine->SetGlobalState(FieldPlayer_GlobalState_AI::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
	}
	else
	{
		m_pStateMachine->SetGlobalState(FieldPlayer_GlobalState::GetInstance());// �O���[�o��
		m_pStateMachine->SetCurrentState(FieldPlayer_Wait::GetInstance());

	}

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	//m_pStateMachine = new StateMachine<FieldPlayer>(this);
	//m_pStateMachine->SetGlobalState(FieldPlayer_GlobalState_AI::GetInstance());// �O���[�o��
	//m_pStateMachine->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());

	// �O���t������
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

	// ���X�e�[�g�}�V���X�V
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

	// �x�[�X�v���C���[
	BasePerson::Update();


	if (m_invincibleFlame <= 0)
	{

		// ����ł��疳��
		if (m_pStateMachine->GetCurrentState() != FieldPlayer_Dead::GetInstance())
		{
			// ���ɓ����Ă���Ƃ�
			if (SOCCER_BALL.GetMove().x != 0.0f)
			{
				// ������������p�X
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

	// �����S�������ĂȂ��Ƃ�
	if (isHave == false)
	{
		// �i�V�ѐS�j���n����܂ōX�V�͋�����
		if (m_bRand == true)
		{
			// �^�X�N����
			// �󂶂�Ȃ�������
			if (!m_task.empty())
			{

				TASK ta = m_task.top();
				//���@�^�[�Q�b�g�ݒ�	
				if (m_targetPos != ta.pos)
				{
					// �^�[�Q�b�g�X�V
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
				// �߂Â�����^�X�N�����o�����̃^�X�N��
				if (length <= 6.5f)
				{
					m_task.pop();

					// ���̂ŋ�ɂȂ�����
					if (m_task.empty())
					{
						// �ҋ@��
						m_pStateMachine->SetCurrentState(FieldPlayer_Wait_AI::GetInstance());
					}
				}

			}
		}

	}
	else // �����S�������Ă���
	{
		SOCCER_BALL.SetPos(m_obj->WorldBonePos(8));

	}

};

void FieldPlayer::Render()
{
	m_obj->Render();

	// ���̃X�e�[�g�}�V���̕`��
	m_pStateMachine->Render();

	// �O���t
	//m_graph->Render();

	int id = (int)(GetID()-GAME_ID::id_person_first );
	// �Q�[�W
	m_gage->Render(20 + (id * 326) , 0, 256, 128, +(id * 256), 0, 256, 128);
	m_life->Render(140 + (id * 326), 0, 128, 128, +(m_LifePoint * 128), 0, 128, 128);

	BasePerson::Render();
	BasePerson::UIRender();
};

// �X�e�[�g�}�V���̃��b�Z�[�W
bool FieldPlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void FieldPlayer::taskSetting()
{
	// ��(TODO)���X�e�[�g�A�邽�߂ɖ������
	//m_targetPos = VECTOR_ZERO;

	// �܂��͑S�m�[�h�Ƃ̋������v�Z
	//navi->	 targetPos
	m_goalNode = m_graph->NearPosNode(m_goalPos);

	// �X�^�[�g�͎����̃|�W�V����
	FieldNode* startNode = m_graph->NearPosNode(m_pos);

	// �o�H�T���J�n�I�iA*�j
	m_graph->Planning(startNode, m_goalNode);


	/*
	����m_GoalFieldNode��ݒ肵���̂Ō�̓|�W�V�������l�߂Ă���
	*/
	FieldNode* N;
	N = m_graph->m_GoalFieldNode;

	if (N == nullptr)
	{
		return;
	}

	// �O�̃^�X�N�N���A
	// ��ɂȂ�܂Ń|�b�v
	for (;;)
	{
		// �󂾂�����A��
		if (m_task.empty()) { break; }// 
		m_task.pop();
	}

	// �V���������
	TASK task;
	task.pos = VECTOR_ZERO;
	task.fieldState = FIELD_STATE::RUN;// ����
	for (;;)
	{
		// �l�ߍ���
		task.pos = N->m_pos;
		task.fieldState = N->m_fieldState;

		// �|�W�V����������
		m_task.push(task);

		// �e�̓t�B�[���h�m�[�h�̂Ă��ŃL���X�g
		N = (FieldNode*)N->m_parent;


		if (N == nullptr)
		{
			break;
		}
	}

}