﻿#include "TDNLIB.h"
#include "SelectData.h"
#include "../Sound/BattleMusic.h"

SideData::SideData() :
eCharacter(CHARACTER::BALANCE),	// 選択キャラクター
ePartner(PARTNER::MOKOI),			// 選択パートナー
iDeviceID(0),						// 操作するデバイスID
bAI(false),							// AIかどうか
eAIType(AI_TYPE::PRACTICE_LAND),	// AIのタイプ
eColorType(COLOR_TYPE::NORMAL)		// カラータイプ
{}

SelectData::SelectData():
eStage(STAGE::A),				// ステージ
bTutorial(false),							// チュートリアルかどうか
bTraining(true),							// トレーニングかどうか	
bChallenge(false),							// チャレンジかどうか	
iBattleMusicID(BattleMusicManager::RANDOM),	// 流す曲の番号
iRoundTime(99),								// 一試合の初期時間
eOrderTrainingSide(SIDE::LEFT),				// トレーニングを選んだ側
iTutorialType(0),							// 選択しているチュートリアル
iChallengeType(0)							// 選択しているチャレンジ
{
	tagSideDatas[(int)SIDE::LEFT].iDeviceID = 0;
	tagSideDatas[(int)SIDE::RIGHT].iDeviceID = 1;

	tagSideDatas[(int)SIDE::RIGHT].eCharacter = CHARACTER::TEKI;

	// 片方AI用
	tagSideDatas[(int)SIDE::RIGHT].bAI = true;
}

TrainingData::TrainingData():
eStartPosition(START_POSITION::MIDDLE),				// 初期のポジション
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
iEnemyLv(20),										// 敵のLv
eWassyoiType(WASSYOI_TYPE::NO)						// 封印されし魂のわっしょいゲージ
{

}
