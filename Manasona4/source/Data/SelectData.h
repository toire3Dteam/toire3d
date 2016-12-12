#pragma once

#include "../BaseEntity/Message/Message.h"
//#include "Player\AI\AI.h"		(TODO)AIの種類はどこで設定しようか

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
enum class STAGE { SAND, SEA, GARDEN, SYUTEN, A, HEAVE_HO, MAX };

struct SideData
{
	CHARACTER character;
	PARTNER partner;
	int iDeviceID;	// コントローラーデバイスのID
	bool bAI;		// AIフラグ
	SideData();
	SideData &operator=(const SideData &data) { character = data.character, partner = data.partner, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// 左右の情報
	STAGE stage;			// 選んでるステージ
	bool bTutorial;			// チュートリアルフラグ
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