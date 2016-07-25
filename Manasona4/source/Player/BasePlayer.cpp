#include "TDNLIB.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"
#include "../Sound/SoundManager.h"

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

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	FOR((int)BASE_ACTION_STATE::END)
	{
		ifs >> skip;

		// �n���A�����A�d���t���[����ǂݍ���
		int count(0);

		for (int j = 0; j < (int)FRAME_STATE::END; j++)
		{
			int frame;
			ifs >> frame;
			for (int k = 0; k < frame; k++)
			{
				m_ActionFrameList[i][count++] = (FRAME_STATE)j;
			}
		}
		// �I�[
		m_ActionFrameList[i][count] = FRAME_STATE::END;
	}
}

BasePlayer::BasePlayer(int deviceID) :BaseGameEntity((ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + deviceID)),
m_maxSpeed(1.0f), m_dir(DIR::LEFT), m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square),
m_pObj(nullptr), m_move(VECTOR_ZERO), m_bLand(false), m_bAerialJump(true), m_ActionState(BASE_ACTION_STATE::NO_ACTION),
m_InvincibleLV(0), m_InvincibleTime(0), m_CurrentActionFrame(0), m_RecoveryFlame(0), m_bEscape(false)
{
	// �f�t�H���g�ݒ�
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

	// �G�t�F�N�g�}�l�[�W���[
	m_PanelEffectMGR = new PanelEffectManager();
	m_UVEffectMGR	 = new UVEffectManager();

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

	SAFE_DELETE(m_PanelEffectMGR);
	SAFE_DELETE(m_UVEffectMGR);
	
	delete m_pHitSquare;
}

void BasePlayer::Update()
{
	// �G�t�F�N�g�}�l�[�W���[�X�V (�q�b�g�X�g�b�v����)
	m_PanelEffectMGR->Update();
	m_UVEffectMGR->Update();

	// ���d�����Ԃ̃f�N�������g
	if (m_RecoveryFlame > 0)m_RecoveryFlame--;

	// ���q�b�g�X�g�b�v��
	if (m_HitStopFrame > 0)
	{
		m_HitStopFrame--;
	}
	// �q�b�g�X�g�b�v�������܂œ����Ȃ�
	else
	{
		// �A�N�V�����t���[���̍X�V
		if (isFrameAction())
		{
			// �U���t���[���Ȃ�
			if (m_ActionDatas[(int)m_ActionState].isAttackData())
			{
				// ���̌o�ߎ��Ԃƃf�B���C�t���[���ɂȂ�����
				if (m_ActionDatas[(int)m_ActionState].pAttackData->WhiffDelayFrame == m_CurrentActionFrame)
				{
					// (A���)�����ōU�����肪���������u�Ԃ�����Ă��Ă�


					LPCSTR SE_ID = m_ActionDatas[(int)m_ActionState].pAttackData->WhiffSE;
					// ��U��SE�����Ă���
					if (SE_ID)
					{
						// �f�B���C�t���[���o�߂�����Đ�
						se->Play((LPSTR)SE_ID);
					}
				}
			}

			// �t���[���Ō�܂ōĐ�������
			if (m_ActionFrameList[m_ActionState][m_CurrentActionFrame++] == FRAME_STATE::END)
			{
				// ���A�N�V�����X�e�[�g����
				m_ActionState = BASE_ACTION_STATE::NO_ACTION;
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

		// ���b�V���̍X�V
		m_pObj->Animation();
		m_pObj->SetAngle(m_angleY);	// ���E�̃A���O���̃Z�b�g
		m_pObj->SetPos(m_pos);
		m_pObj->Update();
	}
	// �A���O����ԏ���
	m_angleY = m_angleY*.65f + DIR_ANGLE[(int)m_dir] * .35f;
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
	// (TODO)�q�b�g�X�g�b�v���ĂȂ�������
	if (m_HitStopFrame <= 0)
	{
		// ���W�X�V
		m_pos += m_move;
	}
}

void BasePlayer::Render()
{
	m_pObj->Render();
	
	m_pStateMachine->Render();// �X�e�[�g�}�V���ł̕`��

	// �G�t�F�N�g�}�l�[�W���[�`��
	m_PanelEffectMGR->Render3D();
	m_UVEffectMGR->Render();

#ifdef _DEBUG


	// ����̕`��
	CollisionShape::Square square;

	memcpy_s(&square, sizeof(CollisionShape::Square), m_pHitSquare, sizeof(CollisionShape::Square));
	square.pos += m_pos;

	Vector3 wv[3];	// ���[���h�o�[�e�b�N�X
	wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
	wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
	wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

	Vector2 sv[3];	// �X�N���[���o�[�e�b�N�X
	FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

	tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ffffff);

	/* �U������̕`�� */
	if (isAttackState())
	{
		if (isActiveFrame())
		{
			memcpy_s(&square, sizeof(CollisionShape::Square), GetAttackData()->pCollisionShape, sizeof(CollisionShape::Square));
			if (m_dir == DIR::LEFT) square.pos.x *= -1;	// ����pos�͐��+(�E)�Ȃ̂ŁA�������Ȃ�t�ɂ���
			square.pos += m_pos;

			wv[0].Set(square.pos.x - square.width, square.pos.y + square.height, 0);
			wv[1].Set(square.pos.x + square.width, square.pos.y + square.height, 0);
			wv[2].Set(square.pos.x + square.width, square.pos.y - square.height, 0);

			FOR(3)sv[i] = Math::WorldToScreen(wv[i]);

			tdnPolygon::Rect((int)sv[0].x, (int)sv[0].y, (int)(sv[1].x - sv[0].x), (int)(sv[2].y - sv[0].y), RS::COPY, 0x80ff0000);
		}
	}
#endif
}

// �X�e�[�g�}�V���ւ̑����痈�郁�b�Z�[�W
bool BasePlayer::HandleMessage(const Message & msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

// ���G�t�F�N�g�������@�i�S�ẴG�t�F�N�g�̔����ꏊ�j
void BasePlayer::AddEffectAction(Vector3 pos, EFFECT_TYPE effectType)
{
	// ���̃G�t�F�N�g���d�˂邩�͂����ŉ������߂�B

	switch (effectType)
	{
	case EFFECT_TYPE::DAMAGE:
		//m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::BURN);
		m_PanelEffectMGR->AddEffect(pos, PANEL_EFFECT_TYPE::DAMAGE);
		m_UVEffectMGR->AddEffect(pos, UV_EFFECT_TYPE::WAVE);
		break;
	default:
		assert(0);	// ����ȃG�t�F�N�g�͑��݂��Ȃ� AddEffectAction()
		break;
	}

}
