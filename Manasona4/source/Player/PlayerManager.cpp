#include "PlayerManager.h"
#include "Airou\Airou.h"
#include "Stage\Stage.h"

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