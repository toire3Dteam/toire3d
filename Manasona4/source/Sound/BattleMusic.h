#pragma once

#include "TDNLIB.h"

class BattleMusicManager
{
public:
	//------------------------------------------------------
	//	実体取得
	//------------------------------------------------------
	static BattleMusicManager *GetInstance(){ static BattleMusicManager i; return &i; }

	//------------------------------------------------------
	//	デストラクタ(リストの開放)
	//------------------------------------------------------
	~BattleMusicManager(){ m_list.clear(); }

	//------------------------------------------------------
	//	AUTOとRANDOM用
	//------------------------------------------------------
	enum{ AUTO = 0, RANDOM = 1 };

	//------------------------------------------------------
	//	ゲッター
	//------------------------------------------------------
	// 曲数を返す
	int GetNumMusic(){ return m_NumMusic; }

	// 引数の曲名の番号を返す
	int GetMusicNo(LPCSTR MusicName)
	{
		FOR((int)m_list.size()) if (tdnFile::GetFileName((LPSTR)m_list[i].c_str(), false) == MusicName) return i;

		MyAssert(0, "そんなデータは存在しない");
		return -1;
	}

	// 引数の番号の曲名を返す(主にセレクトで)
	std::string GetMusicName(int ID)
	{
		assert(ID >= 0 && ID < m_NumMusic);
		switch (ID)
		{
		case AUTO:
			return std::string("AUTO");
			break;

		case RANDOM:
			return std::string("RANDOM");
			break;

		default:
			return tdnFile::GetFileName((LPSTR)m_list[ID - 2].c_str(), false);
			break;
		}
	}

	// 引数の番号の曲のファイルパスを返す
	std::string GetMusicFilePath(int ID)
	{
		assert(ID >= 0 && ID < m_NumMusic);
		switch (ID)
		{
		case AUTO:
			return std::string("DATA/Sound/BGM/battle/tori.ogg");
			break;

		case RANDOM:
			return std::string("DATA/Sound/BGM/battle/" + m_list[tdnRandom::Get(0,m_NumMusic - 3)]);
			break;

		default:
			return std::string("DATA/Sound/BGM/battle/" + m_list[ID - 2]);
			break;
		}

	}
private:

	//------------------------------------------------------
	//	曲の名前兼ファイル名、その番号が格納されている
	//------------------------------------------------------
	std::vector<std::string> m_list;

	//------------------------------------------------------
	//	曲の総数
	//------------------------------------------------------
	int m_NumMusic;

	//------------------------------------------------------
	//	コンストラクタ(列挙して格納)
	//------------------------------------------------------
	BattleMusicManager()
	{
		// フォルダー列挙用
		DirectoryInfo info;
		tdnFile::EnumDirectory("DATA/Sound/BGM/battle", &info);

		// 曲の総数
		m_NumMusic = (int)info.FileNames.size();

		// フォルダーの中のoggファイルを列挙して格納していく
		FOR(m_NumMusic)
		{
			// oggファイル名とその番号を格納
			m_list.push_back(info.FileNames[i]);
		}

		// ★AUTOとランダム用
		m_NumMusic += 2;
	}
	BattleMusicManager(const BattleMusicManager&){}
	BattleMusicManager &operator=(const BattleMusicManager&){}
};

#define BattleMusicMgr (BattleMusicManager::GetInstance())