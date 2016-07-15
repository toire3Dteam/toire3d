#pragma once
#include "BasePlayer.h"

class PlayerManager :public BaseGameEntity
{
public:
	PlayerManager(int NumPlayer, Stage::Base *pStage);
	~PlayerManager();
	//void Update(Stage::Base *pStage);
	void Update();
	void Render();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }

private:
	const int m_NumPlayer;
	BasePlayer **m_pPlayers;
	Stage::Base *m_pStage;	// �Q�Ƃ��邾��(Update�̈����ł�����Ă��̂����ABaseGameEntity�̌p���ň����Ȃ���Update���g��Ȃ��Ƃ����Ȃ��Ȃ������߁A�����o�ϐ���)

							// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);
};
