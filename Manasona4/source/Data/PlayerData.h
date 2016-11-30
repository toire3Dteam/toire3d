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