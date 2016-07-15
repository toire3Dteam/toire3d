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
	Stage::Base *m_pStage;	// 参照するだけ(Updateの引数でもらってたのだが、BaseGameEntityの継承で引数なしのUpdateを使わないといけなくなったため、メンバ変数に)

							// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);
};
