#pragma once
#include "BasePlayer.h"

class PlayerManager :public BaseGameEntity
{
public:
	// 実体取得
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
	// 1つしかない実体
	static PlayerManager *pInstance;

	int m_NumPlayer;
	BasePlayer **m_pPlayers;
	Stage::Base *m_pStage;	// 参照するだけ(Updateの引数でもらってたのだが、BaseGameEntityの継承で引数なしのUpdateを使わないといけなくなったため、メンバ変数に)

	// ★プレイヤーとプレイヤーの攻撃の判定
	bool CollisionPlayerAttack(BasePlayer *my, BasePlayer *you);

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// シングルトンの作法
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// 実体化
#define PlayerMgr (PlayerManager::GetInstance())