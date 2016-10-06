#pragma once
#include "TDNLIB.h"
// エンティティ関連のインクルード
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"
#include "HpGage.h"
#include "SpGage.h"
#include "PersonaUI.h"
//#include "Combo.h"


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

	// 参照させる
	void ReferencesPlayer(BasePlayer* pLeftPlayer, BasePlayer* pRightPlayer);

	void Update();
	void Render();

	void Action();

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

private:

	//tdn2DObj* m_teamPic;

	tdn2DAnim* m_pHpFrame;
	tdn2DAnim* m_pSpFrame;

	// HPゲージ
	HpGage* m_pHpGage1P;
	HpGage* m_pHpGage2P;

	// SPゲージ
	SpGage* m_pSpGage1P;
	SpGage* m_pSpGage2P;

	// ペルソナ用
	PersonaUI* m_pPersona1P;
	PersonaUI* m_pPersona2P;

	//struct  HPGage
	//{
	//	tdn2DObj* obj;
	//	float frame;
	//};
	//HPGage	

	//Combo* m_combo;

};

// 実体化
#define GameUIMgr (GameUIManager::GetInstance())