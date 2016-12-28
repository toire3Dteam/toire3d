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
iRoundTime(99),								// 一試合の初期時間
eOrderTrainingSide(SIDE::LEFT)				// トレーニングを選んだ側
{
	tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
	tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

	// 片方AI用
	//tagSideDatas[(int)SIDE::RIGHT].bAI = true;
}

TrainingData::TrainingData():
eHpRecovery(HP_RECOVERY_TYPE::AUTO_RECOVERY),		// 自動回復するかしないか
iHp1P(100),											// 1PのHP
iHp2P(100),											// 1PのHP
iSpGage(0),											// SPゲージ
ePartnerRecovery(PARTNER_RECOVERY_TYPE::DEFAULT),	// パートナーゲージ
eTrainingInfo(TRAINING_INFO_TYPE::NONE),			// 最大ダメージや入力覆歴を表示するかどうか
eEnemyState(ENEMY_STATE_TYPE::STAND),				// 敵の状態
eEnemyGuard(ENEMY_GUARD_TYPE::NO),					// 敵のガードの設定
eEnemyGuardSwitch(ENEMY_GUARD_SWITCH_TYPE::NO),		// 敵のガード切り替えの設定
eEnemyTech(ENEMY_TECH_TYPE::ALL),					// 敵の受け身の設定
eEnemyThrowTech(ENEMY_THROW_TECH_TYPE::NO),			// 敵のなげ抜けの設定
iEnemyLv(20)										// 敵のLv
{

}
