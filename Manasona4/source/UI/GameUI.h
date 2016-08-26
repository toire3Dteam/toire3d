#pragma once
#include "TDNLIB.h"
// エンティティ関連のインクルード
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"



class GameUIManager :public BaseGameEntity
{
private:
	// シングルトンの作法
	GameUIManager();
	~GameUIManager();
	GameUIManager(const GameUIManager&);
	GameUIManager &operator=(const GameUIManager&){}
	
	// 1つしかない実体
	static GameUIManager *pInstance;

public:
	// 実体取得
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
	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

private:

	tdn2DObj* m_teamPic;


};

// 実体化
#define GameUIMgr (GameUIManager::GetInstance())