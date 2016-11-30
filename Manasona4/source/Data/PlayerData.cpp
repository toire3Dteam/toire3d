#include "TDNLIB.h"
#include "PlayerData.h"

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
PlayerDataManager::PlayerDataManager() :m_bSaveFlag(true),
m_StartClock(clock())	// 現在時刻保存
{
}

//------------------------------------------------------
//	デストラクタ
//------------------------------------------------------
PlayerDataManager::~PlayerDataManager()
{
	// セーブフラグあったらデータ記録
	if (m_bSaveFlag)
		SavePlayerData();
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
	fread_s((LPSTR)&m_PlayerInfo, sizeof(PlayerData::Infomation), sizeof(PlayerData::Infomation), 1, fp);
	m_PlayerInfo.PlayCount++;
	
	// トロフィー読み込み
	fread_s((LPSTR)&m_TrophyData, sizeof(PlayerData::Trophy), sizeof(PlayerData::Trophy), 1, fp);

	// 隠し情報読み込み
	fread_s((LPSTR)&m_SecretData, sizeof(PlayerData::Secret), sizeof(PlayerData::Secret), 1, fp);

	// ファイル閉じる
	fclose(fp);

	return true;		// 読み込み成功
}

//------------------------------------------------------
//	セーブデータ書き込み
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