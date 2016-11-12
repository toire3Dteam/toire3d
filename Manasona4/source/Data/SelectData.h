#pragma once

#include "BaseEntity\Message\Message.h"

//*****************************************************************************
//
//		セレクトシーンで選択したデータ用(シングルトン)
//
//*****************************************************************************

// キャラクターのタイプ
enum class CHARACTER { AIROU, END };

// ステージのタイプ
enum class STAGE { SENJO, SYUTEN, A, HEAVE_HO, MAX };

struct SideData
{
	CHARACTER CharacterType;
	int iDeviceID;	// コントローラーデバイスのID
	bool bAI;		// AIフラグ
	SideData() :CharacterType(CHARACTER::AIROU), iDeviceID(0), bAI(false) {}
	SideData &operator=(const SideData &data) { CharacterType = data.CharacterType, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// 左右の情報
	STAGE stage;			// 選んでるステージ
	bool bTutorial;			// チュートリアルフラグ
	int iWinRound;			// 勝利ラウンド数
	int iBattleMusicID;		// 戦闘曲の番号

	SelectData();
	SelectData &operator=(const SelectData &data) { tagSideDatas[(int)SIDE::LEFT] = data.tagSideDatas[(int)SIDE::LEFT], tagSideDatas[(int)SIDE::RIGHT] = data.tagSideDatas[(int)SIDE::RIGHT], stage = data.stage, bTutorial = data.bTutorial; return *this; }
};

class SelectDataManager
{
public:
	static SelectDataManager *GetInstance(){ static SelectDataManager i; return &i; }

	SelectData *Get() { return &m_data; }
private:

	SelectData m_data;

	SelectDataManager(){}
	SelectDataManager(const SelectDataManager&) {}
	SelectDataManager &operator=(const SelectDataManager&) {}
};

#define SelectDataMgr (SelectDataManager::GetInstance())