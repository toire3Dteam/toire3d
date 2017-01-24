#pragma once

#include "../BaseEntity/Message/Message.h"

//*****************************************************************************
//
//		セレクトシーンで選択したデータ用(シングルトン)
//
//*****************************************************************************

// キャラクターのタイプ
enum class CHARACTER { AIROU, TEKI, NAZENARA, ARAMITAMA, ANIKI, END };

// パートナーのタイプ
enum class PARTNER { MOKOI, MAYA, END };

// ステージのタイプ
enum class STAGE { SAND, SYUTEN, SEA, GARDEN, NANASATO_SITY, MAX };

// AIのタイプ
enum class AI_TYPE
{
	CPU_EASY,			// 弱いAI
	CPU_NORMAL,			// 普通のAI
	CPU_HARD,			// 強いAI
	CPU_YOKOE,			// 横江くん用に作られたマジキチなAI

	PRACTICE_LAND,		//	[練習用]何も動かない
	PRACTICE_SQUAT,		//	[練習用]しゃがんで何も動かない
	PRACTICE_JUMP,		//	[練習用]飛び続ける
	PRACTICE_ATTACK,	//	[練習用]一定間隔で攻撃する

	T_DOKKOI_ATTACK,	// [説明用]中段攻撃
	T_DOWN_ATTACK,		// [説明用]下段攻撃

	INTERRUPT_INVINCIBLE	// [デバッグ用]ガード中にひたすら無敵割り込みをする
};

// ラウンド数タイプ
enum class ROUND_NUM_TYPE
{
	ROUND_1,
	ROUND_2,
	ROUND_3,
	ROUND_4,
};

// ラウンド時間タイプ
enum class ROUND_TIME_TYPE
{
	SEC_30,
	SEC_45,
	SEC_60,
	SEC_75,
	SEC_99,
	SEC_INFINITY,
};

// ランク
enum class RANK_TYPE
{
	SS, S, A, B, C
};

//+----------------------
//	システム用
//+----------------------

// オートセーブするかしないか
enum class AUTO_SAVE_TYPE
{
	YES,		// オートセーブする
	NO,			// オートセーブしない
};


//+----------------------
//	トレーニング用
//+----------------------

// 初期のポジション
enum class START_POSITION
{
	MIDDLE,		// 真ん中から
	LEFT,		// 左から
	RIGHT,		// 右から
};

// 自動回復するかしないか
enum class HP_RECOVERY_TYPE
{
	DEFAULT,		// 通常
	AUTO_RECOVERY,	// 自動回復
};

// パートナーゲージ
enum class PARTNER_RECOVERY_TYPE
{
	DEFAULT,		// 通常
	MAX,			// MAX固定
};

// 最大ダメージや入力覆歴を表示するかどうか
enum class TRAINING_INFO_TYPE
{
	NONE,		// なし
	DAMEGE,		// ダメージのみ
	COMMAND,	// コマンドのみ
	ALL			// 全て
};

// ダミー
// 敵の状態
enum class ENEMY_STATE_TYPE
{
	STAND,				// 立ち状態
	SQUAT,				// しゃがみ状態
	JUMP,				// ジャンプし続ける
	ATTACK,				// 一定間隔で攻撃
	COM,				// コンピュータ
	T_DOKKOI_ATTACK,	// [説明用]中段攻撃
	T_DOWN_ATTACK,		// [説明用]足払い攻撃
};

// 敵のガードの設定
enum class ENEMY_GUARD_TYPE
{
	NO,		// ガードしない
	WAY,	// 途中からガードする
	ALL		// 全部ガードする
};

// 敵のガード切り替えの設定
enum class ENEMY_GUARD_SWITCH_TYPE
{
	NO,		// 切り替えしない
	OK,		// 切り替えする
};

// 敵の受け身の設定
enum class ENEMY_TECH_TYPE
{
	NO,		// 受け身しない
	LAND,	// 地上のみ受け身をする
	AIR,	// 空中のみ受け身をする
	ALL,	// 全て
};

// 敵のなげ抜けの設定
enum class ENEMY_THROW_TECH_TYPE
{
	NO,		// なげ抜けしない
	OK		// なげ抜けする
};


//+-------------------------
//	キャラクター固有データ
//+-------------------------

// アラミタマ用デスク
struct AramitamaDesk
{
	int iWassyoiGauge;
};

// アラミタマのわっしょいゲージ設定
enum class WASSYOI_TYPE
{
	NO,		// 普通
	MAX		// 常に最大
};


// トレーニング用データ
struct TrainingData
{
	START_POSITION			eStartPosition;		// 初期ポジション
	HP_RECOVERY_TYPE		eHpRecovery;		// 自動回復するかしないか
	int						iHp1P;				// 1PのHP
	int						iHp2P;				// 1PのHP
	int						iSpGage;			// SPゲージ
	PARTNER_RECOVERY_TYPE	ePartnerRecovery;	// パートナーゲージ
	TRAINING_INFO_TYPE		eTrainingInfo;		// 最大ダメージや入力覆歴を表示するかどうか
	ENEMY_STATE_TYPE		eEnemyState;		// 敵の状態
	ENEMY_GUARD_TYPE		eEnemyGuard;		// 敵のガードの設定
	ENEMY_GUARD_SWITCH_TYPE eEnemyGuardSwitch;	// 敵のガード切り替えの設定
	ENEMY_TECH_TYPE			eEnemyTech;			// 敵の受け身の設定
	ENEMY_THROW_TECH_TYPE	eEnemyThrowTech;	// 敵のなげ抜けの設定
	int						iEnemyLv;			// 敵のLV
	WASSYOI_TYPE			eWassyoiType;		// アラミタマのわっしょいゲージ
	TrainingData();
};


//+---------------------------
//	チュートリアルタイプ
//+---------------------------
enum class TUTORIAL_TYPE
{
	WALK,
	SQUAT,
	JUMP,
	ATTACK,
	SKILL,
	OVER_DRIVE,
	RUSH,
	STAND_GUARD,
	SQUAT_GUARD,
	DUSH,
	BACK_STEP,
	PARTNER,
	INVINCIBLE,
	THROW,
	ESCAPE,
	DOKKOI,
	ANTI_AIR,
	DOWN_ATTACK,
	OD_ONEMORE,
	OD_BURST,
	ARRAY_END
};


//+-----------------------------------------
//	チャレンジモードの種類　（全キャラ共通）
//+-----------------------------------------
enum class CHALLENGE_TYPE
{
	MISSION_1,
	MISSION_2,
	MISSION_3,
	ARRAY_END
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
	STAGE eStage;					// 選んでるステージ
	bool bTutorial;					// チュートリアルフラグ
	bool bTraining;					// トレーニングフラグ
	bool bChallenge;				// チャレンジフラグ
	int iRoundTime;					// 一試合時間
	int iBattleMusicID;				// 戦闘曲の番号
	SIDE eOrderTrainingSide;		// トレーニングを選んだ側
	TrainingData tagTrainingDatas;	// トレーニングのデータ
	int iTutorialType;				// 選択しているチュートリアル 
	int iChallengeType;				// 選択しているチャレンジ
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