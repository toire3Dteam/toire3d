#pragma once
#include "TDNLIB.h"
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"



class GameUIManager :public BaseGameEntity
{
private:
	// �V���O���g���̍�@
	GameUIManager();
	~GameUIManager();
	GameUIManager(const GameUIManager&);
	GameUIManager &operator=(const GameUIManager&){}
	
	// 1�����Ȃ�����
	static GameUIManager *pInstance;

public:
	// ���̎擾
	static GameUIManager *GetInstance()
	{
		if (!pInstance) pInstance = new GameUIManager;
		return pInstance;
	}
	static void Rerease(){
		SAFE_DELETE(pInstance);
	}


	void Update();
	void Render();
	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

private:

	tdn2DObj* m_teamPic;


};

// ���̉�
#define GameUIMgr (GameUIManager::GetInstance())