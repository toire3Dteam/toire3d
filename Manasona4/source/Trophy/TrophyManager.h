#pragma once
#include "BaseEntity\Entity\BaseGameEntity.h"
#include "BaseTrophy.h"
#include "Data\SelectData.h"

//+--------------------------
//	トロフィーの管理クラス
//+--------------------------

// トロフィーの種類
enum class TROPHY_TYPE
{
	FIRST,				//	始めの一回
	FIRST_BATTLE,		//	始めてのバトル
	BIG_DAMAGE,			//	物凄い火力
	MAX_HP_FINISH,		//	最大HP勝利
	SPEED_FINISH,		//	最速勝利
	TRAINING_TIME,		//  トレーニングでひたすら練習
	POWERFUL_ENEMY,		//  最高難易度でCOMを倒した
	SS_RANK_WIN,		//  SSランクで倒した
	SS_POWERFUL_ENEMY,	//  最高難易度でCOM+SSランクで倒した
	MENY_CONTENT,		//  コンテンツをたくさん解放した
	CHALLENGE_CLEAR,	//  チャレンジモードで何かタスクをこなした
	ATTAKAI_BOM,		//  アッタカイのボム
	AIROU_CHALLENGE,	//	ぶんぶんチャレンジ
	TEKI_CHALLENGE,		//	テキチャレンジ
	BIG_BIG_DAMAGE,		//	超物凄い火力
	COMPLETE_TROPHY,	//  全てのトロフィーを集めた


	//_6,
	//_7, _8, _9, _10, _11, _12,
	//_13, _14, _15, _16, _17, _18,
	ARRAY_END
};

// 一つで十分なのとどこでも扱うのでシングルトンに
class TrophyManager : public BaseGameEntity
{
private:
	TrophyManager();
	~TrophyManager();
	TrophyManager(const TrophyManager&);
	TrophyManager &operator=(const TrophyManager&) {}

	// 1つしかない実体
	static TrophyManager *m_pInstance;

public:
	// 実体取得
	static TrophyManager *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new TrophyManager();
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	void Update();
	void Render();
	void RenderRoom(int iTrophyType, int iX, int iY);	
	void RenderInfo(int iTrophyType, int iX, int iY);

	void Stop();		// 演出を切る

	// 全データさよなら
	void AllReset();

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// アクセサ
	// トロフィーの中身
	TROPHY_DATA GetTrophyData(int iTrophyType) { return m_pTrophy[iTrophyType]->GetTrophy(); }
	//tdn2DObj* GetIcon(int iTrophyType) { m_pTrophy[iTrophyType]->GetTrophy().pIcon; }
	BaseTrophy* GetTrophy(int iTrophyType) { return m_pTrophy[iTrophyType]; };

	// トロフィー最大数
	int GetMaxTrophyNum() { return (int)TROPHY_TYPE::ARRAY_END; };

	// トロフィー所持数
	int GetTrophyOwned();

	//void SetTrainingFrame(int iFrame) { m_iTrainingFrame = iFrame; };
private:

	// トロフィー達
	BaseTrophy* m_pTrophy[(int)TROPHY_TYPE::ARRAY_END];

	// 選択しているトロフィー
	TROPHY_TYPE m_eSelectTrophy;

	// ロックアイコン
	tdn2DObj* m_pRockIcon;

	// トロフィーを解放していく順番
	std::vector<TROPHY_TYPE> m_aActionStack;
	int m_iIntervalFrame;	//	連続で入手した場合間隔を遅らす用

public:
	//+---------------------------------------------
	//	★色々なトロフィーの条件を作っていく
	//+---------------------------------------------
	void InitSeceneMain();

	void CheakPlayCount(int iPlayCount);
	void CheakBattleCount(int iBattleCount);
	void CheakBigDamage(bool bVS = false);// 対戦フラグ
	void CheakMaxHpFinish(int iRemainingHP);
	void CheakSpeedFinish(int iElapsedTime);
	void CheakTrainingTime();
	void CheakPowerfulEnemyWin(SIDE eWinnerSide);
	void CheakRank(SIDE eWinnerSide, RANK_TYPE eRank);
	void CheakBuyManyContent(int iOwnedContent);
	void CheakChallengeClear();


private:
	int m_iTrainingFrame;

};

// 実体化
#define TrophyMgr (TrophyManager::GetInstance())