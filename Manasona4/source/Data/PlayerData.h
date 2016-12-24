#pragma once

//*****************************************************************************
//
//		プレイヤーデータ用(シングルトン)
//
//*****************************************************************************

// プレイヤーに関する情報の構造体をここにまとめる
namespace PlayerData
{
	// プレイ情報
	struct Infomation
	{
		unsigned int PlayCount	: 16;	// 遊んだ回数(65535まで)
		unsigned int PlayTime	: 32;	// プレイ時間(4294967295)
		unsigned int coin		: 20;	// 真夏パチンココイン(1048575まで)
	};

	// 設定の情報
	struct Confing
	{
		unsigned int iDifficultyAI	: 4;	// AIの難易度(15まで)
		signed int iBGMVolume		: 8;
		signed int iSEVolume		: 8;
		signed int iVoiceVolume		: 8;
		unsigned int iRoundNumType	: 4;
		unsigned int iRoundTimeType : 4;
		unsigned int 空き枠2		: 32;
		unsigned int 空き枠3		: 32;
		unsigned int 空き枠4		: 32;
		unsigned int 空き枠5		: 32;
		unsigned int 空き枠6		: 32;
	};

	struct Training
	{
		int iHpRecovery;
		int iHp1P;
		int iHp2P;
		int iSpGage;
		int iPartnerRecovery;
		int iInfo;
		int 空き枠7;
		int 空き枠8;
		int 空き枠9;
		int 空き枠10;
		int 空き枠11;
		int 空き枠12;
		int 空き枠13;
		int 空き枠14;
		int 空き枠15;
		int 空き枠16;
		int 空き枠17;
		int 空き枠18;
		int 空き枠19;
		int 空き枠20;
		int 空き枠21;
		int 空き枠22;
		int 空き枠23;
		int 空き枠24;
		int 空き枠25;
		int 空き枠26;
		int 空き枠27;
		int 空き枠28;
	};

	// トロフィー
	struct Trophy
	{
		unsigned int ゲーム初プレイ : 1;
		unsigned int 空き枠			: 32;
	};

	// 隠し要素
	struct Secret
	{
		unsigned int エウレシア解禁 : 1;
		unsigned int 兄貴解禁		: 1;
		unsigned int 空き枠1		: 32;
		unsigned int 空き枠2		: 32;
	};

	//// プレイヤーのオプション
	//struct Option
	//{
	//	unsigned int bSudden		:1;		// サドゥン
	//	unsigned int SuddenPos		:10;	// サドゥン座標(Y)10ビット(1023)
	//	unsigned int HiSpeed		:5;		// ハイスピ 5ビット(31) * 0.25
	//	unsigned int iGaugeOption	:3;		// ゲージ(enumだと処理系依存なので危険) 3ビット(7)
	//	//unsigned int BasisOctave	:7;		// 開始オクターブ 7ビット(127) ※結構頻繁に変わりそうなので、テキストで管理する
	//	unsigned int bMoviePlay		:1;		// 演奏時に動画を再生するかどうか
	//	unsigned int MovieAlpha		:8;		// 演奏背景で流す動画の透明度(255)
	//	unsigned int bAutoPlay		:1;		// DJ_AUTOさん
	//	unsigned int bGuide			:1;		// ガイドありかなしか
	//};
}



class PlayerDataManager
{
public:
	static PlayerDataManager *GetInstance(){ static PlayerDataManager i; return &i; }

	// セーブフラグON
	void OnSaveFlag(){ m_bSaveFlag = true; }

	// 初期化(セーブデータのロード)
	bool Initialize();

	PlayerData::Infomation m_PlayerInfo;	// プレイ情報
	PlayerData::Confing m_ConfigData;		// プレイヤーの設定情報
	PlayerData::Training m_TrainingData;	// トレーニング
	PlayerData::Trophy m_TrophyData;			// トロフィー
	PlayerData::Secret m_SecretData;			// 隠し要素


	// デストラクタ(ダブルポインタ関連をこの中で完結させてるので、使う人は気にしなくてOK)
	~PlayerDataManager();

	// 新規データ作成
	void NewCreate();

	// セーブ
	void SavePlayerData();

private:
	//static PlayerDataManager *m_pInstance;
	bool LoadPlayerData();
	void ClearData();		// やったらあかんやつ(データ全消去)

	bool m_bSaveFlag;	// trueにしたら消したときにセーブする

	clock_t m_StartClock;	// プレイ時間加算用

	PlayerDataManager();
	PlayerDataManager(const PlayerDataManager&){}
	PlayerDataManager &operator=(const PlayerDataManager&){}
};

#define PlayerDataMgr (PlayerDataManager::GetInstance())