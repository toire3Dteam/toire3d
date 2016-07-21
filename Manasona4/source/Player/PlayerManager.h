#pragma once
#include "BasePlayer.h"

class PlayerManager :public BaseGameEntity
{
public:
	// ���̎擾
	static PlayerManager *GetInstance()
	{
		if (!pInstance) pInstance = new PlayerManager;
		return pInstance;
	}

	~PlayerManager();
	void Initialize(int NumPlayer, Stage::Base *pStage);
	void Release(){ SAFE_DELETE(pInstance); }
	void Update();
	void Render();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }

private:
	// 1�����Ȃ�����
	static PlayerManager *pInstance;

	int m_NumPlayer;
	BasePlayer **m_pPlayers;
	Stage::Base *m_pStage;	// �Q�Ƃ��邾��(Update�̈����ł�����Ă��̂����ABaseGameEntity�̌p���ň����Ȃ���Update���g��Ȃ��Ƃ����Ȃ��Ȃ������߁A�����o�ϐ���)

	// ���v���C���[�ƃv���C���[�̍U���̔���
	bool CollisionPlayerAttack(BasePlayer *my, BasePlayer *you);

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �V���O���g���̍�@
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// ���̉�
#define PlayerMgr (PlayerManager::GetInstance())