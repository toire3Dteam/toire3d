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
	void Update(bool bControl);
	void Render();
	void Render(tdnShader* shader, char* name);
	void RenderDeferred();
	void RenderUI();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }

	void CalcTeamPoint();
	
	// アクセサ
	int GetPointA(){ return m_PointAteam; }
	int GetPointB(){ return m_PointBteam; }

	float GetOverDriveDim(){ return m_OverDriveDim; }

	// 引数のチームのやつを探し出す
	BasePlayer* GetPlayer_TeamInSearch(SIDE side)
	{	
	
		FOR(m_NumPlayer)
		{
			if (m_pPlayers[i]->GetSide() == side)
			{
				return m_pPlayers[i];
			}
		}

		MyAssert(0, "PLAYERMGR：見つからなかった！");
		return m_pPlayers[0];
	}

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
	float m_OverDriveDim;

	// ★プレイヤーとプレイヤーの攻撃の判定
	void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you, HIT_ATTACK_INFO **OutAttackInfo, HIT_DAMAGE_INFO **OutDamageInfo);

	// ★スタンドの攻撃に対するプレイヤーの判定
	bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);

	// ★投げの攻撃に対するプレイヤーの判定
	bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you);

	void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_ATTACK_INFO *pHitAttackInfo, HIT_DAMAGE_INFO *pHitDamageInfo);

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// シングルトンの作法
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// 実体化
#define PlayerMgr (PlayerManager::GetInstance())