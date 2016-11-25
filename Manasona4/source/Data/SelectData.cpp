#include "TDNLIB.h"
#include "SelectData.h"
#include "../Sound/BattleMusic.h"

SelectData::SelectData():stage(STAGE::SEA), bTutorial(false), iWinRound(2), iBattleMusicID(BattleMusicManager::RANDOM)
{
	tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
	tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

	// 片方AI用
	//tagSideDatas[(int)SIDE::LEFT].bAI = true;
}