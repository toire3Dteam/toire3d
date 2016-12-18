#pragma once

#include "../BaseEntity/Message/Message.h"

//*****************************************************************************
//
//		セレクトシーンで選択したデータ用(シングルトン)
//
//*****************************************************************************

// キャラクターのタイプ
enum class CHARACTER { AIROU, TEKI, NAZENARA, END };

// パートナーのタイプ
enum class PARTNER { MOKOI, MAYA, END };

// ステージのタイプ
enum class STAGE { SAND, SYUTEN, SEA, GARDEN, NANASATO_SITY, 
	//HEAVE_HO, A,	// 無かったことにされたステージたち
	MAX };

// AIのタイプ
enum class AI_TYPE
{
	CPU_EASY,			// 弱いAI
	CPU_NORMAL,			// 普通のAI
	CPU_HARD,			// 強いAI
	CPU_YOKOE,			// 横江くん用に作られたマジキチなAI

	PRACTICE_LAND,		//	[練習用]何も動かない
	PRACTICE_JUMP,		//	[練習用]飛び続ける
	PRACTICE_ATTACK,	//	[練習用]一定間隔で攻撃する

	INTERRUPT_INVINCIBLE	// [デバッグ用]ガード中にひたすら無敵割り込みをする
};

struct SideData
{
	CHARACTER eCharacter;
	PARTNER ePartner;
	int iDeviceID;	// コントローラーデバイスのID
	bool bAI;		// AIフラグ
	AI_TYPE eAIType;// AIのタイプ
	SideData();
	//SideData &operator=(const SideData &data) { character = data.character, partner = data.partner, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// 左右の情報
	STAGE eStage;			// 選んでるステージ
	bool bTutorial;			// チュートリアルフラグ
	bool bTraining;			// トレーニングフラグ
	int iWinRound;			// 勝利ラウンド数
	int iRoundTime;			// 一試合時間
	int iBattleMusicID;		// 戦闘曲の番号

	SelectData();
	//SelectData &operator=(const SelectData &data) { tagSideDatas[(int)SIDE::LEFT] = data.tagSideDatas[(int)SIDE::LEFT], tagSideDatas[(int)SIDE::RIGHT] = data.tagSideDatas[(int)SIDE::RIGHT], stage = data.stage, bTutorial = data.bTutorial, iWinRound = data.iWinRound, iBattleMusicID = data.iBattleMusicID; return *this; }
};

class SelectDataManager
{
public:
	static SelectDataManager *GetInstance(){ static SelectDataManager i; return &i; }

	SelectData *Get() { return &m_tagData; }
private:

	SelectData m_tagData;

	SelectDataManager(){}
	SelectDataManager(const SelectDataManager&) {}
	SelectDataManager &operator=(const SelectDataManager&) {}
};

#define SelectDataMgr (SelectDataManager::GetInstance())