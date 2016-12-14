#include "TDNLIB.h"
#include "SelectData.h"
#include "../Sound/BattleMusic.h"

SideData::SideData() :
character(CHARACTER::TEKI),		// 選択キャラクター
partner(PARTNER::MAYA),			// 選択パートナー
iDeviceID(0),					// 操作するデバイスID
bAI(false)						// AI
{}

SelectData::SelectData():stage(STAGE::NANASATO_SITY),
bTutorial(false), bTraining(false),
iWinRound(2),	// ラウンド数
iBattleMusicID(BattleMusicManager::RANDOM),
iRoundTime(99)	// 一試合の初期時間
{
	tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
	tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

	// 片方AI用
	tagSideDatas[(int)SIDE::RIGHT].bAI = true;
}