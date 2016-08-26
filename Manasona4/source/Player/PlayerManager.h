#pragma once
#include "BasePlayer.h"

// 前方宣言
namespace Stand
{
	class Base;
}


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
	void Render(tdnShader* shader, char* name);
	void RenderDeferred();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }

	void CalcTeamPoint();
	
	// アクセサ
	int GetPointA(){ return m_PointAteam; }
	int GetPointB(){ return m_PointBteam; }

	int GetOverDriveDim(){ return m_OverDriveDim; }

private:
	// 1つしかない実体
	static PlayerManager *pInstance;

	int m_NumPlayer;
	BasePlayer **m_pPlayers;
	Stage::Base *m_pStage;	// 参照するだけ(Updateの引数でもらってたのだが、BaseGameEntityの継承で引数なしのUpdateを使わないといけなくなったため、メンバ変数に)

	// チームポイント
	int m_PointAteam;
	int m_PointBteam;

	// 覚醒暗転
	int m_OverDriveDim;

	// ★プレイヤーとプレイヤーの攻撃の判定
	bool CollisionPlayerAttack(BasePlayer *my, BasePlayer *you);

	// ★スタンドの攻撃に対するプレイヤーの判定
	bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// シングルトンの作法
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// 実体化
#define PlayerMgr (PlayerManager::GetInstance())