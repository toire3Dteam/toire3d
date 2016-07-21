#include "TDNLIB.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"


//_/_/_/_/_/_/__/_/__/_/__/_/__
// �萔
//_/_/_/_/__/_/__/_/__/_/__/_/_
const float BasePlayer::c_END_MOVE_LINE = .25f;		// �ړ����I����đҋ@�ɖ߂�ړ��l���C��
const float BasePlayer::c_FRONT_BRAKE_LINE = .55f;	// ���蒆�ɑ���𗣂��ău���[�L�Ɉڍs����܂ł̈ړ��l���C��
const float BasePlayer::c_GRAVITY = .1f;
const float BasePlayer::c_MAX_JUMP = 2.2f;

static const float DIR_ANGLE[(int)DIR::MAX] =
{
	PI * 1.5f,	// ������
	PI * .5,	// �E����
};

// �U���t���[�����ǂݍ���
void BasePlayer::LoadAttackFrameList(char *filename)
{
	std::ifstream ifs(filename);
	MyAssert(ifs, "�A�^�b�N�t���[���̃e�L�X�g�����ĂȂ�");

	FOR((int)BASE_ATTACK_STATE::END)
	{
		char skip[64];	// �ǂݔ�΂��p�ϐ�

		ifs >> skip;

		// �n���A�����A�d���t���[����ǂݍ���
		int count(0);

		for (int j = 0; j < (int)ATTACK_FRAME::END; j++)
		{
			int frame;
			ifs >> frame;
			for (int k = 0; k < frame; k++)
			{
				m_AttackFrameList[i][count++] = (ATTACK_FRAME)j;
			}
		}
		// �I�[
		m_AttackFrameList[i][count] = ATTACK_FRAME::END;
	}
}

BasePlayer::BasePlayer(int deviceID) :BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO), m_bLand(false), m_AttackState(BASE_ATTACK_STATE::END), m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentAttackFrame(0)
{
	// �f�t�H���g�ݒ�
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// ������
	memset(m_InputList, 0, sizeof(m_InputList));
	ZeroMemory(&m_jump, sizeof(Jump));
	ZeroMemory(&m_RushAttack, sizeof(RushAttack));
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
	// �U���t���[���̍X�V
	if (isAttackState())
	{
		// �t���[���Ō�܂ōĐ�������
		if (m_AttackFrameList[m_AttackState][m_CurrentAttackFrame++] == ATTACK_FRAME::END)
		{
			// ���U���X�e�[�g����
			m_AttackState = BASE_ATTACK_STATE::END;
		}
	}

	// ���͎�t
	Control();

	// ���͎�t��ɃX�e�[�g�}�V���X�V
	m_pStateMachine->Update();

	//if (m_InputList[(int)PLAYER_INPUT::LEFT] == 1) m_move.x = -2;
	//else m_move.x = 0;

	// �����̐���
	Move();

	// �A���O����ԏ���
	m_angleY = m_angleY*.65f + DIR_ANGLE[(int)m_dir] * .35f;

	// ���b�V���̍X�V
	m_pObj->Animation();
	m_pObj->SetAngle(m_angleY);	// ���E�̃A���O���̃Z�b�g
	m_pObj->SetPos(m_pos);
	m_pObj->Update();
	
}

void BasePlayer::Move() 
{
	// �ړ��ʍX�V
	m_move.y -= c_GRAVITY;
	//if (m_move.y <= -3.0f) { m_move.y = -3.0f; } // �����鑬�x��}��
	m_move.y = max(-2.75f, m_move.y);// �����鑬�x��}��

	// ��C��R(x�̒l�����X�Ɍ��炵�Ă���)
	if (m_bLand)
	{
		//if (m_move.x > 0)		m_move.x = max(m_move.x - .055f, 0);
		//else if (m_move.x < 0)	m_move.x = min(m_move.x + .055f, 0);
		m_move.x *= 0.92f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
	}
	else
	{	
		//if (m_move.x > 0)		m_move.x = max(m_move.x - .035f, 0);
		//else if (m_move.x < 0)	m_move.x = min(m_move.x + .035f, 0);
		m_move.x *= 0.98f;	// ����(A���:���̒l�̓L�����ŗL�̒l)
	}

	// ���E��Move�l
	//m_move.x = Math::Clamp(m_move.x, -m_maxSpeed, m_maxSpeed);


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

	tdnText::Draw(0, 0, 0xffffffff,"%.1f", m_move.y);
}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}