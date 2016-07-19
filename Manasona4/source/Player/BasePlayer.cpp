#include "TDNLIB.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"

static const float DIR_ANGLE[(int)DIR::MAX] =
{
	PI * 1.5f,	// ������
	PI * .5,	// �E����
};

BasePlayer::BasePlayer(int deviceID) :BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_Gravity(.14f), m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO)
{
	// �f�t�H���g�ݒ�
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// ������
	memset(m_InputList, 0, sizeof(m_InputList));

	// �X�e�[�g�}�V���@�����͎������g�̃|�C���^
	m_pStateMachine = new StateMachine<BasePlayer>(this);
	m_pStateMachine->SetGlobalState(BasePlayerState::Global::GetInstance());// �O���[�o��
	m_pStateMachine->SetCurrentState(BasePlayerState::Wait::GetInstance());

	// ID�ō��W�̕���
	switch (deviceID)
	{
	case 0:
		m_pos.Set(-50, 20, 0);
		break;

	case 1:
		m_pos.Set(-25, 20, 0);
		break;

	case 2:
		m_pos.Set(25, 20, 0);
		break;

	case 3:
		m_pos.Set(50, 20, 0);
		break;
	}

	m_angleY = DIR_ANGLE[(int)m_dir];
}

BasePlayer::~BasePlayer()
{
	SAFE_DELETE(m_pObj);
	SAFE_DELETE(m_pStateMachine);
	delete m_pHitSquare;
}

void BasePlayer::Update()
{
	// ���͎�t
	Control();

	// ���͎�t��ɃX�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
	//else m_move.x = 0;

	// �����̐���
	Move();

	// �A���O����ԏ���
	m_angleY = m_angleY*.75f + DIR_ANGLE[(int)m_dir] * .25f;

	// ���b�V���̍X�V
	m_pObj->Animation();
	m_pObj->SetAngle(m_angleY);	// ���E�̃A���O���̃Z�b�g
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
	
}

void BasePlayer::Move() 
{
	// �ړ��ʍX�V
	m_move.y -= m_Gravity;
	//if (m_move.y <= -3.0f) { m_move.y = -3.0f; } // �����鑬�x��}��
	m_move.y = max(-3.0f, m_move.y);// �����鑬�x��}��


	// ���E��Move�l
	m_move.x = Math::Clamp(m_move.x, -m_maxSpeed, m_maxSpeed);

	m_move *= 0.9f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
}

void BasePlayer::Control()
{
	// �������L�[����
	for (int i = 0; i < (int)PLAYER_INPUT::MAX; i++)
	{
		m_InputList[i] = tdnInput::KeyGet(KeyCodeList[i], m_deviceID);
	}

}

void BasePlayer::UpdatePos()
{
	// ���W�X�V
	m_pos += m_move;
}

void BasePlayer::Render()
{
	m_pObj->Render();
	
	m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��
}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}