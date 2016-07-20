#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"
#include "../Sound/SoundManager.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

// ���̂̐錾
PlayerManager *PlayerManager::pInstance = nullptr;

PlayerManager::PlayerManager() :BaseGameEntity(ENTITY_ID::PLAYER_MGR),	// �G���e�B�e�BID�o�^
m_pStage(nullptr), m_NumPlayer(1), m_pPlayers(nullptr)
{

}
PlayerManager::PlayerManager(const PlayerManager&) :BaseGameEntity(ENTITY_ID::PLAYER_MGR){}

void PlayerManager::Initialize(int NumPlayer, Stage::Base *pStage)
{
	m_NumPlayer = NumPlayer;
	m_pStage = pStage;

	// �v���C���m��
	m_pPlayers = new BasePlayer*[NumPlayer];

	FOR(NumPlayer)
	{
		m_pPlayers[i] = new Airou(i);
		m_pPlayers[i]->InitAttackDatas();		// ���U�������e�L�����ɏ�����������
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
	}

	// �v���C���[VS�v���C���[�̍U������
	for (int my = 0; my < m_NumPlayer; my++)
	{
		for (int you = my + 1; you < m_NumPlayer; you++)
		{
			CollisionPlayerAttack(my, you);
			CollisionPlayerAttack(you, my);
		}
	}

	// �ʒu���m��
	FOR(m_NumPlayer)
	{
		m_pStage->Collision(m_pPlayers[i]);	// �X�e�[�W�Ƃ̔���ŁAmove�l���ǂ����������������
		m_pPlayers[i]->UpdatePos();			// ���W��move�l�𑫂��X�V
	}
}

void PlayerManager::Render()
{
	// �v���C���[�����`��
	FOR(m_NumPlayer) m_pPlayers[i]->Render();
}

void PlayerManager::CollisionPlayerAttack(int my, int you)
{
	if (m_pPlayers[my]->isAttackFrame()) // �U���t���[�����Ȃ�
	{
		if (m_pPlayers[you]->GetInvincibleLV() <= m_pPlayers[my]->GetPierceLV()) // ���肪���G�łȂ�
		{
			if (Math::Length(m_pPlayers[my]->GetPos(), m_pPlayers[you]->GetPos()) < 10)
			{
				// �q�b�g���̏���

				/* ���b�Z�[�W���M */

				// �܂��A�U�����q�b�g�������l�ɑ��M
				HIT_ATTACK_INFO hai;
				hai.HitPlayerNo = you;	// �_���[�W��^��������̔ԍ�
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + my), MESSAGE_TYPE::HIT_ATTACK, &hai);

				// �����āA�_���[�W���󂯂��l�ɑ��M
				HIT_DAMAGE_INFO hdi;
				hdi.BeInvincible = m_pPlayers[my]->GetAttackData()->bBeInvincible;
				hdi.damage = m_pPlayers[my]->GetAttackData()->damage;
				hdi.FlyVector = Vector2(0, 4);
				MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + you), MESSAGE_TYPE::HIT_DAMAGE, &hdi);

				// ���o��
				LPCSTR seID = m_pPlayers[my]->GetAttackSE();
				if (strcmp(seID, "") != 0) se->Play((LPSTR)seID);
			}
		}
	}
}

bool PlayerManager::HandleMessage(const Message &msg)
{
	// ���̂Ƃ��둗���邱�Ƃ͂Ȃ��̂���
	return false;
}