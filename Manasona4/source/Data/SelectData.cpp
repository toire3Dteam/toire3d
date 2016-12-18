#include "TDNLIB.h"
#include "SelectData.h"
#include "../Sound/BattleMusic.h"

SideData::SideData() :
eCharacter(CHARACTER::NAZENARA),	// 選択キャラクター
ePartner(PARTNER::MAYA),			// 選択パートナー
iDeviceID(0),						// 操作するデバイスID
bAI(false),							// AIかどうか
eAIType(AI_TYPE::PRACTICE_LAND)		// AIのタイプ
{}

SelectData::SelectData():
eStage(STAGE::SAND),						// ステージ
bTutorial(false),							// チュートリアルかどうか
bTraining(false),							// トレーニングかどうか
iWinRound(2),								// ラウンド数			
iBattleMusicID(BattleMusicManager::RANDOM),	// 流す曲の番号
iRoundTime(99)								// 一試合の初期時間
{
	tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
	tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

	// 片方AI用
	//tagSideDatas[(int)SIDE::RIGHT].bAI = true;
}