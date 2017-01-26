#pragma once

//********************************************************************************************************************
//
//		クラスの宣言
//
//********************************************************************************************************************
//		サウンドマネージャー(グローバル実体で関数呼び出し)
//****************************************************************************************************************

class SoundManager
{
public:
	// 関数
	static void Initialize();
	static void Release();
	static void Update();
	static void SetListenerPos(const Vector2 &pos);

	static void SetBaseVolume(int BGMVol, int SEVol, int VoiceVol);
};

class iexSoundBase;
class tdnSoundSE;
class tdnSoundBGM;

class SE_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*型で番号を振り分ける

	tdnSoundSE *play_manager;	// iexSound
	Vector2 m_ListenerPos;		// 聞こえる人の座標


	int Play_in(int data_num, bool loop);
	int Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop);

public:

	//===============================================
	//	音データ
	//===============================================
	struct	DATA{
		LPSTR id;					// 呼び出す際に使用する識別コード
		LPSTR file_name;			// ファイルパス
		int play_simultaneously;	// 同一の音を複数再生する最大値
		bool b3D;					// 3Dサウンドフラグ
	};


	//===============================================
	//	初期化と解放
	//===============================================
	SE_Manager(){}
	~SE_Manager();
	void Initialize();

	//===============================================
	//	更		新
	//===============================================
	void Update();


	//===============================================
	//	処		理
	//===============================================
	bool isPlay(LPSTR _ID, int no){ return play_manager->isPlay(ID[_ID], no); }
	int Play(LPSTR _ID, bool loop = false);																			// 簡易版																								// ボリューム設定版
	int Play(LPSTR _ID, const Vector2 &pos, const Vector2 &move = Vector2(0, 0), bool loop = false);				// 3D設定版
	void SetTone(LPSTR _ID, int tone);
	void Stop(LPSTR _ID, int no);																					// Playで返ってきた数値を入れる
	void Stop_all();																								// 全部止める
	void SetFX(LPSTR _ID, int no, DXA_FX flag);
	void SetPos(LPSTR _ID, int no, const Vector2 &pos);
	void SetBaseVolume(float volume){ play_manager->SetBaseVolume(volume); }
	void SetListener(const Vector2 &pos);			// リスナー情報
};

class BGM_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*型で番号を振り分ける

	tdnSoundBGM *play_manager;	// iexSound

	// ストリーミング用
	tdnStreamSound *m_pStreamSound;
	bool m_bPlayStream;

	void Play_in(int data_num, bool loop);
	void Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop);

public:

	//===============================================
	//	音データ
	//===============================================
	struct	DATA{
		LPSTR id;					// 呼び出す際に使用する識別コード
		LPSTR file_name;			// ファイルパス
		bool b3D;
	};


	//===============================================
	//	初期化と解放
	//===============================================
	void Initialize();
	~BGM_Manager();

	//===============================================
	//	更		新
	//===============================================
	void Update();


	//===============================================
	//	処		理
	//===============================================
	void Play(LPSTR _ID, bool loop = true);																											// 簡易版																								// ボリューム設定版
	void Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front = Vector3(0, 0, -1), const Vector3 &move = Vector3(0, 0, 0), bool loop = true);	// 3D設定版
	void Stop(LPSTR _ID);																															// Playで返ってきた数値を入れる
	void Stop_all();																																// 全部止める

	void Fade_in(LPSTR _ID, float fade_speed, bool loop = true);
	void Fade_out(LPSTR _ID, float fade_speed);
	void Cross_fade(LPSTR inID, LPSTR outID, float fade_speed = .0075f, bool loop = true);
	void Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop);
	bool isPlay(LPSTR _ID);
	void SetSpeed(LPSTR _ID, float speed){ play_manager->SetSpeed(ID[_ID], speed); }
	void SetBaseVolume(float volume)
	{
		play_manager->SetBaseVolume(volume); 

		if (m_bPlayStream)
		{
			m_pStreamSound->SetVolume(volume);
		}
	}
	void SetVolume(LPSTR _ID, float volume){ play_manager->SetVolume(ID[_ID], volume); }
	void SetListener(const Vector2 &pos, const Vector2 &move);											// リスナー情報
	tdnStreamSound *PlayStream(LPSTR filename){ return play_manager->PlayStream(filename); }
	tdnStreamSound *PlayStream(LPSTR filename, BYTE mode, int param){ return play_manager->PlayStream(filename, mode, param); }
	void PlayStreamIn(LPSTR filename, bool bFadeIn = false)
	{
		if (m_bPlayStream) m_pStreamSound->Stop();
		if (bFadeIn)m_pStreamSound = play_manager->PlayStream(filename, STR_FADEIN, 8);
		else m_pStreamSound = play_manager->PlayStream(filename);
		m_bPlayStream = true;
	}
	void StopStreamIn(bool bFadeOut = false)
	{
		if (m_bPlayStream)
		{
			if (bFadeOut)m_pStreamSound->SetMode(STR_FADEOUT, 48);
			else m_pStreamSound->Stop();
			m_bPlayStream = false;
		}
	}
};


enum class MY_MUSIC_ID
{
	SELECT,		// セレクト
	SENJO,		// 戦場
	SYUTEN,		// 終点
	A,		// Aステージ
	MAX
};

class MyMusicManager
{
public:
	MyMusicManager(MY_MUSIC_ID id);
	~MyMusicManager();

	// オレ曲構造体
	struct MyMusic
	{
		std::string MusicName;	// 曲名
		std::string path;		// そのファイルへのパス
		int percent;			// 再生率
		MyMusic(char *MusicName, char *path, int percent) :MusicName(MusicName), path(path), percent(percent){}
	};

	std::vector<MyMusic*> *GetList(){ return &m_list; }	// リスト取得
	void Play();// オレ曲からランダムで再生
	void PlayHeaveHo();	// ヒーホードライブ音楽再生
private:

	std::vector<MyMusic*> m_list;	// オレ曲を格納するリスト
	tdnStreamSound *m_pStreamSound;	// オレ曲再生するストリームサウンド
	tdnStreamSound *m_pHeaveHoStream;;	// ひーほー再生用

	bool m_bPlay;	// 2重再生防止用
	bool m_bHeaveHoPlay;	// ヒーホー再生してたらふらgu

};

//===============================================
//	実体(関数呼び出しはこれらを使う)
//===============================================
extern SE_Manager *se;
extern BGM_Manager *bgm;