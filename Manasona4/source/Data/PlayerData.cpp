#include "TDNLIB.h"
#include "PlayerData.h"
#include "SelectData.h"

//------------------------------------------------------
//	初期化
//------------------------------------------------------
bool PlayerDataManager::Initialize()
{
	//ClearData();
	//SavePlayerData();
	//return true;
	
	// この関数でプレイ情報を読み込み。ついでにデータがない(新規作成させる)かのboolを返す
	return LoadPlayerData();
}

//------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------
PlayerDataManager::PlayerDataManager() :m_bSaveFlag(false),
m_StartClock(clock())	// 現在時刻保存
{
}

//------------------------------------------------------
//	デストラクタ
//------------------------------------------------------
PlayerDataManager::~PlayerDataManager()
{
	// セーブフラグあったらデータ記録
	if (m_ConfigData.iAutoSave == (int)AUTO_SAVE_TYPE::YES && m_bSaveFlag)
	{
		SavePlayerData();
	}
}

//------------------------------------------------------
//	セーブデータ読み込み
//------------------------------------------------------
bool PlayerDataManager::LoadPlayerData()
{
	// バイナリ読み込みくん
	FILE *fp;
	if (fopen_s(&fp, "DATA/Save/data.bin", "rb") != 0) return false;

	// プレイヤー基本データ読み込み
	//PlayerData::PrevInfomation a;
	fread_s((LPSTR)&m_PlayerInfo, sizeof(PlayerData::Infomation), sizeof(PlayerData::Infomation), 1, fp);
	//ZeroMemory(&m_PlayerInfo, sizeof(PlayerData::Infomation));
	//m_PlayerInfo.PlayCount = a.PlayCount;
	//m_PlayerInfo.PlayTime = a.PlayTime;
	//m_PlayerInfo.coin = a.coin;
	m_PlayerInfo.PlayCount++;	// プレイカウント++
	
	// コンフィグ
	fread_s((LPSTR)&m_ConfigData, sizeof(PlayerData::Confing), sizeof(PlayerData::Confing), 1, fp);

	// トレーニング設定データ
	fread_s((LPSTR)&m_TrainingData, sizeof(PlayerData::Training), sizeof(PlayerData::Training), 1, fp);

	// トロフィー読み込み
	//PlayerData::PrevTrophy a;
	fread_s((LPSTR)&m_TrophyData, sizeof(PlayerData::Trophy), sizeof(PlayerData::Trophy), 1, fp);
	//ZeroMemory(&m_TrophyData, sizeof(PlayerData::Trophy));

	// 隠し情報読み込み
	//PlayerData::PrevSecret a;
	fread_s((LPSTR)&m_SecretData, sizeof(PlayerData::Secret), sizeof(PlayerData::Secret), 1, fp);
	//ZeroMemory(&m_SecretData, sizeof(m_SecretData));

	//m_SecretData.Pic空き枠10 = 1;
	//m_SecretData.Movie空き枠2 = 1;

	// ファイル閉じる
	fclose(fp);

	// 全部読み込み切ったらセーブしてもいい
	m_bSaveFlag = true;

	return true;		// 読み込み成功
}

//------------------------------------------------------
//	セーブデータ書き出し
//------------------------------------------------------
void PlayerDataManager::SavePlayerData()
{
	m_bSaveFlag = false;

	// プレイ時間加算(ミリ秒→秒→分)
	const DWORD delta(((clock() - m_StartClock) / 1000) / 60);
	m_PlayerInfo.PlayTime += delta;

	// バイナリ書き出しくん
	FILE *fp;
	MyAssert(fopen_s(&fp, "DATA/Save/data.bin", "wb") == 0, "デデドン(絶望)\nセーブデータ書き出しに失敗した！");	// まず止まることはないと思うが…

	// プレイヤー基本データ書き出し
	fwrite((LPSTR)&m_PlayerInfo, 1, sizeof(PlayerData::Infomation), fp);

	// プレイヤー設定データ書き出し
	fwrite((LPSTR)&m_ConfigData, 1, sizeof(PlayerData::Confing), fp);

	// プレイヤートレーニング設定データ書き出し
	fwrite((LPSTR)&m_TrainingData, 1, sizeof(PlayerData::Training), fp);

	// トロフィー情報書き出し
	fwrite((LPSTR)&m_TrophyData, 1, sizeof(PlayerData::Trophy), fp);

	// 隠し要素書き出し
	fwrite((LPSTR)&m_SecretData, 1, sizeof(PlayerData::Secret), fp);

	// ファイル閉じる
	fclose(fp);
}

//------------------------------------------------------
//	新規データ作成
//------------------------------------------------------
void PlayerDataManager::NewCreate()
{
	// 一旦全消去
	ClearData();

	// セーブデータを書き込む
	SavePlayerData();
}

//------------------------------------------------------
//	データ全消去
//------------------------------------------------------
void PlayerDataManager::ClearData()
{
	// オールクリア
	ZeroMemory(&m_PlayerInfo, sizeof(PlayerData::Infomation));
	ZeroMemory(&m_TrophyData, sizeof(PlayerData::Trophy));
	ZeroMemory(&m_SecretData, sizeof(PlayerData::Secret));
}