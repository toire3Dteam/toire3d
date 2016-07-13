#include "TDNLIB.h"
#include "Player.h"
#include "../Collision/Collision.h"
#include "../Stage/Stage.h"

Fighter::Base::Base(int deviceID) :m_Gravity(.14f), m_dir(DIR::LEFT),m_deviceID(deviceID), m_pHitSquare(new CollisionShape::Square), m_pObj(nullptr), m_move(VECTOR_ZERO)
{
	// �f�t�H���g�ݒ�
	m_pHitSquare->height = 4;
	m_pHitSquare->width = 1;
	m_pHitSquare->pos.Set(.0f, 4.0f, .0f);

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
}

Fighter::Base::~Base()
{
	SAFE_DELETE(m_pObj);
	delete m_pHitSquare;
}

void Fighter::Base::Update()
{
	// ���͎�t
	Control();

	// ���b�V���̍X�V
	m_pObj->Animation();
	m_pObj->SetAngle((m_dir == DIR::LEFT) ? PI * .5f : PI * 1.5f);	// ���E�̃A���O���̃Z�b�g
	m_pObj->SetPos(m_pos);
	m_pObj->Update();

	// �ړ��ʍX�V
	m_move.y -= m_Gravity;
}

void Fighter::Base::Control()
{
	const float speed = 2.0f;
	if (KeyBoard(KB_LEFT)) m_move.x = -speed;
	else if (KeyBoard(KB_RIGHT)) m_move.x = speed;
	else m_move.x = 0;
}

void Fighter::Base::UpdatePos()
{
	// ���W�X�V
	m_pos += m_move;
}

void Fighter::Base::Render()
{
	m_pObj->Render();
}

Fighter::Airou::Airou(int deviceID) :Base(deviceID)
{
	// �A�C���[���b�V��
	m_pObj = new iex3DObj("DATA/CHR/Airou/airou.IEM");
}

PlayerManager::PlayerManager(int NumPlayer, Stage::Base *pStage) :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// �G���e�B�e�BID�o�^
m_pStage(pStage), m_NumPlayer(NumPlayer)
{
	// �v���C���m��
	m_pPlayers = new Fighter::Base*[NumPlayer];

	FOR(NumPlayer)
	{
		m_pPlayers[i] = new Fighter::Airou(i);
	}
}

PlayerManager::~PlayerManager()
{
	// �_�u���|�C���^�[�̊J��
	FOR(m_NumPlayer)delete m_pPlayers[i];
	delete[] m_pPlayers;
}

void PlayerManager::Update()
{
	// �v���C���[�����X�V
	FOR(m_NumPlayer)
	{
		m_pPlayers[i]->Update();			// ���[�V�����Ƃ��ړ��l�̍쐬�Ƃ��A��{�I�ȍX�V�B
		m_pStage->Collision(m_pPlayers[i]);	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}
}

void PlayerManager::Render()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	// ���̂Ƃ��둗���邱�Ƃ͂Ȃ��̂���
	return false;
}