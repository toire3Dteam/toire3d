#pragma once
#include "BasePlayer.h"
#include "Data/SelectData.h"
#include "BaseEntity\Message\Message.h"

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
	void Initialize(int NumPlayer, Stage::Base *pStage, SideData aSideDatas[(int)SIDE::ARRAY_MAX], int iNumRound);
	void Release(){ SAFE_DELETE(pInstance); }
	void Update(PLAYER_UPDATE flag);
	void UpdateTraining();
	void UpdateHit();
	void UpdatePos();
	void Render();
	void Render(tdnShader* shader, char* name);
	void RenderShadow();
	void RenderDeferred();
	void RenderUI();
	void RenderTraining();

	int GetNumPlayer() { return m_NumPlayer; }
	BasePlayer *GetPlayer(int no) { return m_pPlayers[no]; }
	BasePlayer *GetPlayer(ENTITY_ID id){ FOR(m_NumPlayer) { if (m_pPlayers[i]->GetID() == id) return m_pPlayers[i]; }return nullptr; }
	BasePlayer *GetPlayerByDeviceID(int DeviceID){ FOR(m_NumPlayer) { if (m_pPlayers[i]->GetDeviceID() == DeviceID) return m_pPlayers[i]; }return nullptr; }	// デバイスIDから探す
	BasePlayer *GetPlayer(SIDE side) 
	{
		// サイドが合ってたら
		if (side == m_pPlayers[0]->GetSide())return m_pPlayers[0];	
		else return m_pPlayers[1];
	}

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

	//
	void SetIntroState(){ FOR(m_NumPlayer) m_pPlayers[i]->GetFSM()->ChangeState(BasePlayerState::Intro::GetInstance()); }

	void GetInput(){}

	// ラウンドを跨いだ時にリセットする用
	void Reset(){ FOR(m_NumPlayer) m_pPlayers[i]->Reset(); }

	// プレイヤーたちが勝っているいる数の合計(★実質のラウンド数)
	int GetRoundNumber()
	{
		int sum(0);
		FOR(m_NumPlayer)sum += m_pPlayers[i]->GetWinNum();
		return sum;
	}

	bool isGameStop(){ FOR(m_NumPlayer){ if (m_pPlayers[i]->isGameTimerStopFlag())return true; } return false; }

	BasePlayer *GetHPWinner()
	{
		const float l_fRateHP0((float)m_pPlayers[0]->GetHP() / m_pPlayers[0]->GetMaxHP()), l_fRateHP1((float)m_pPlayers[1]->GetHP() / m_pPlayers[1]->GetMaxHP());
		if (l_fRateHP0 > l_fRateHP1) return m_pPlayers[0];
		else if (l_fRateHP1 > l_fRateHP0) return m_pPlayers[1];
		else return nullptr;	// あいこ
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

	// ヒーホードライブ中の演出
	bool m_bHeaveHoDriveOverFlow;

	// 覚醒暗転
	float m_OverDriveDim;

	// トレーニング用の画像
	tdn2DObj* m_pDamageInfoPlate;

	// プレイヤーが持っている敵への向きフラグを設定
	//void UpdatePlayerTargetDir(BasePlayer *my, BasePlayer *you);

	// ★プレイヤーとプレイヤーの攻撃の判定
	//void CollisionPlayerAttack(BasePlayer *my, BasePlayer *you,  HIT_DAMAGE_INFO **OutDamageInfo);

	// ★スタンドの攻撃に対するプレイヤーの判定
	//bool CollisionStandAttack(Stand::Base *pStand, BasePlayer *pYou);

	// ★投げの攻撃に対するプレイヤーの判定
	//bool CollisionThrowAttack(BasePlayer *my, BasePlayer *you);

	//void SendHitMessage(BasePlayer *pAttackPlayer, BasePlayer *pDamagePlayer, HIT_DAMAGE_INFO *pHitDamageInfo);

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// シングルトンの作法
	PlayerManager();
	PlayerManager(const PlayerManager&);
	PlayerManager &operator=(const PlayerManager&){}
};

// 実体化
#define PlayerMgr (PlayerManager::GetInstance())