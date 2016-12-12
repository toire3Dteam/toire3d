
#include	"TDNLIB.h"
#include	"SoundManager.h"


//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★
//
//								サウンド呼び出しID一覧														　☆★
//
//☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★

//==============================================================================================
//				S	E
//==============================================================================================
const SE_Manager::DATA all_dataS[]
{
	//{ "通常1段ヒット",			"DATA/Sound/SE/Battle/attack1.wav",	2, false },
	//{ "通常2段ヒット",			"DATA/Sound/SE/Battle/attack2.wav",	2, false },
	//{ "通常3段ヒット",			"DATA/Sound/SE/Battle/attack3.wav",	2, false },
	{ "ヒット1",					"DATA/Sound/SE/P4U2/hit1.wav",	2, false },
	{ "ヒット2",					"DATA/Sound/SE/P4U2/hit2.wav",	2, false },
	{ "ヒット3",					"DATA/Sound/SE/P4U2/hit3.wav",	2, false },
	{ "ヒット4",					"DATA/Sound/SE/BlazBlue/hit1.wav", 2, false },
	{ "ヒット5",					"DATA/Sound/SE/BlazBlue/hit2.wav", 2, false },
	{ "ヒット6",					"DATA/Sound/SE/BlazBlue/hit3.wav", 2, false },
	{ "ぐちょヒット",				"DATA/Sound/SE/Battle/gutyo.wav", 8, false },
	//{ "空中攻撃ヒット",			"DATA/Sound/SE/Battle/hit1.wav", 2, false },
	//{ "空中下攻撃ヒット",			"DATA/Sound/SE/Battle/hit2.wav", 2, false },
	//{ "全ブレイク",				"DATA/Sound/SE/Battle/all_break.wav",	2, false },
	//{ "チャージ",					"DATA/Sound/SE/Battle/charge.wav",2, false },
	//{ "チャージ溜まる",			"DATA/Sound/SE/Battle/fill.wav",2, false },
	{ "フィニッシュ小",				"DATA/Sound/SE/Battle/finish_base.wav",2, false },
	{ "フィニッシュ中",				"DATA/Sound/SE/Battle/finish2.wav",	2, false },
	{ "フィニッシュ大",				"DATA/Sound/SE/Battle/finish3.wav",	2, false },
	{ "撃ち落とし",					"DATA/Sound/SE/Battle/smack_down.wav",	2, false },
	{ "ジャンプ",					"DATA/Sound/SE/BlazBlue/jump.wav",	2, false },
	{ "空振り1",					"DATA/Sound/SE/BlazBlue/whiff1.wav", 2, false },
	{ "空振り2",					"DATA/Sound/SE/BlazBlue/whiff2.wav", 2, false },
	//{ "空振り3",					"DATA/Sound/SE/BlazBlue/whiff3.wav", 2, false },
	{ "空振り4",					"DATA/Sound/SE/BlazBlue/whiff4.wav", 2, false },
	{ "空振り5",					"DATA/Sound/SE/BlazBlue/whiff5.wav", 2, false },
	{ "空振り6",					"DATA/Sound/SE/BlazBlue/whiff6.wav", 2, false },
	{ "アイルードリル",				"DATA/Sound/SE/BlazBlue/drill.wav", 2, false },
	{ "エスケープ",					"DATA/Sound/SE/Battle/escape.wav",	2, false },
	{ "ガード小",					"DATA/Sound/SE/BlazBlue/guard1.wav", 8, false },
	{ "ガード中",					"DATA/Sound/SE/BlazBlue/guard2.wav", 4, false },
	{ "ガード強",					"DATA/Sound/SE/BlazBlue/guard3.wav", 4, false },
	{ "空中ダッシュ",				"DATA/Sound/SE/BlazBlue/aerial_dash.wav", 2, false },
	//{ "Eカウンター成功",			"DATA/Sound/SE/Battle/counter_success.wav",	2, false },
	//{ "ブレーキ1",				"DATA/Sound/SE/Battle/brake1.wav",	2, false },
	{ "ダッシュ",					"DATA/Sound/SE/Battle/dash.wav", 2, false },
	{ "ペルソナ召喚",				"DATA/Sound/SE/Battle/kyuin.wav", 2, false },
	{ "掴み",						"DATA/Sound/SE/Battle/wriff_catch.wav", 1, false },
	{ "掴み成功",					"DATA/Sound/SE/Battle/catch.wav", 1, false },
	{ "投げ抜け",					"DATA/Sound/SE/Battle/throw_escape.wav", 1, false },	
	{ "オーバードライブ開始",		"DATA/Sound/SE/BlazBlue/over_drive.wav", 2, false },
	{ "オーバードライブ終了",		"DATA/Sound/SE/BlazBlue/over_drive_end.wav", 2, false },
	{ "必殺発動",					"DATA/Sound/SE/P4U2/special1.wav", 2, false },
	{ "超必殺発動",					"DATA/Sound/SE/P4U2/special2.wav", 2, false },
	{ "リザルト1",					"DATA/Sound/SE/Result/result1.wav", 1, false },
	{ "リザルト2",					"DATA/Sound/SE/Result/result2.wav", 1, false },
	{ "リザルト3",					"DATA/Sound/SE/Result/result3.wav", 1, false },

	{ "カーソル1",					"DATA/Sound/SE/P4U2/cursor.wav", 2, false },
	{ "カーソル2",					"DATA/Sound/SE/P4U2/cursor2.wav", 32, false },
	{ "決定1",						"DATA/Sound/SE/P4U2/decide.wav", 2, false },
	{ "キャンセル1",				"DATA/Sound/SE/P4U2/cancel.wav", 2, false },
	{ "ステップクリア",				"DATA/Sound/SE/P4U2/gagin.wav", 3, false },
	{ "オールクリア",				"DATA/Sound/SE/P4U2/clear.wav", 1, false },

	{ "フェイトイズターニング",		"DATA/Sound/SE/BlazBlue/voice/the_wheel.wav",	1, false },
	{ "リベル",						"DATA/Sound/SE/BlazBlue/voice/revel.wav",		1, false },
	{ "ワン",						"DATA/Sound/SE/BlazBlue/voice/one.wav",			1, false },
	{ "ツー",						"DATA/Sound/SE/BlazBlue/voice/two.wav",			1, false },
	{ "スリー",						"DATA/Sound/SE/BlazBlue/voice/three.wav",		1, false },
	{ "アクション",					"DATA/Sound/SE/BlazBlue/voice/action.wav",		1, false },
};

//==============================================================================================
//				B	G	M
//==============================================================================================
BGM_Manager::DATA all_dataB[]
{
	{ "END", nullptr, false }
};


//**************************************************************************************************************
//
//		サウンド管理クラス(winmainとframeworkで使うだけ)
//
//**************************************************************************************************************
//=============================================================================================
//		初	期	化
void SoundManager::Initialize()
{
	se = new SE_Manager;
	se->Initialize();
	bgm = new BGM_Manager;
	bgm->Initialize();

	// ボリュームの読み込み
	//std::ifstream ifs("DATA/Sound/volume.txt");
	//MyAssert(ifs, "エラー: ボリューム設定テキストが入ってない");
	//
	//// ボリューム読み込み
	//char skip[8];	// 読み飛ばし用
	//ifs >> skip;
	//ifs >> m_SEVolume;
	//ifs >> skip;
	//ifs >> m_BGMVolume;
}

//=============================================================================================
//		解		放
void SoundManager::Release()
{
	SAFE_DELETE(se);
	SAFE_DELETE(bgm);
}

//=============================================================================================
//		更新
void SoundManager::Update()
{
	se->Update();
	bgm->Update();
}


///********************************************************************************************
//		リスナー情報設定！
void SoundManager::SetListenerPos(const Vector2 &pos)
{
	se->SetListener(pos);
}

void SoundManager::SetBaseVolume(int BGMVol, int SEVol, int VoiceVol)
{
	// 基本ボリューム設定
	bgm->SetBaseVolume(BGMVol * .01f);
	se->SetBaseVolume(SEVol * .01f);
	// (TODO) ボイス
}


//**************************************************************************************************************
//
//		SE管理クラス
//
//**************************************************************************************************************


//=============================================================================================
//		初	期	化
void SE_Manager::Initialize()
{
	play_manager = new tdnSoundSE;

	for (int i = 0; i < _countof(all_dataS); i++)
	{
		if (strcmp(all_dataS[i].id, "END") == 0) break;	// 終端

		ID[all_dataS[i].id] = i;
		play_manager->Set(i, all_dataS[i].play_simultaneously, all_dataS[i].file_name, all_dataS[i].b3D);
	}

	// リスナーの初期設定
	m_ListenerPos = Vector2(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f);
	//play_manager->SetListenerAll(
	//	Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// リスナー座標(画面の真ん中と仮定する)
	//	Vector3(1, 0, 0),	// 正面ベクトル
	//	Vector3(0, 1, 0),	// 上方ベクトル
	//	Vector3(0, 0, 0)	// 移動値
	//	);
}
//
//=============================================================================================



//=============================================================================================
//		解		放
SE_Manager::~SE_Manager()
{
	delete play_manager;
}
//
//=============================================================================================



//=============================================================================================
//		更		新
void SE_Manager::Update()
{
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		処		理
int SE_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->SetPan(data_num, DSBPAN_CENTER);
		play_manager->SetVolume(data_num, DSBVOLUME_MAX);
		play_manager->Play(data_num, loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		// ステレオ手動で設定してみる
		static const int DSBPAN_WIDTH = 10000;
		const int pan = (int)((DSBPAN_WIDTH / 8)*((pos.x - m_ListenerPos.x) / (tdnSystem::GetScreenSize().right / 2)));
		const int vol = DSBVOLUME_MAX + (int)((DSBVOLUME_MIN / 32) * (sqrtf((m_ListenerPos.x - pos.x)*(m_ListenerPos.x - pos.x)) / m_ListenerPos.x) + (DSBVOLUME_MIN / 16) * (sqrtf((m_ListenerPos.y - pos.y)*(m_ListenerPos.y - pos.y)) / m_ListenerPos.y));
		play_manager->SetPan(data_num, pan);
		play_manager->SetVolume(data_num, vol);

		return play_manager->Play(data_num, loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play(LPSTR _ID, bool loop)
{
	/*
	std::map<char*, int> map;
	map["HOGE0"] = 0;
	map["HOGE1"] = 1;

	const char* str1 = "HOGE1";
	std::string str2 = "HOGE1";

	int a = map[(char*)str1]			// 1が代入される
	int b = map[(char*)str2.c_str()];	// 0が代入される
	*/

	// 何が言いたいかというと連想配列は想像以上に信用できない。

	return Play_in(ID[_ID], loop);
}

int SE_Manager::Play(LPSTR _ID, const Vector2 &pos, const Vector2 &move, bool loop)
{
	return Play_in(ID[_ID], pos, move, loop);
}

void SE_Manager::Stop(LPSTR _ID, int no)
{
	play_manager->Stop(ID[_ID], no);
}

void SE_Manager::Stop_all()
{
	play_manager->AllStop();
}

void SE_Manager::SetTone(LPSTR _ID, int tone)
{
	play_manager->SetSpeed(ID[_ID], 1.0f + (float)tone*.048f);
}

void SE_Manager::SetPos(LPSTR _ID, int no, const Vector2 &pos)
{
	// ステレオ手動で設定してみる
	static const int DSBPAN_WIDTH = 10000;
	const int pan = (int)((DSBPAN_WIDTH / 8)*((pos.x - m_ListenerPos.x) / (tdnSystem::GetScreenSize().right / 2)));
	const int vol = DSBVOLUME_MAX + (int)((DSBVOLUME_MIN / 32) * (sqrtf((m_ListenerPos.x - pos.x)*(m_ListenerPos.x - pos.x)) / m_ListenerPos.x) + (DSBVOLUME_MIN / 16) * (sqrtf((m_ListenerPos.y - pos.y)*(m_ListenerPos.y - pos.y)) / m_ListenerPos.y));
	play_manager->SetPan(no, pan);
	play_manager->SetVolume(no, vol);
}

void SE_Manager::SetListener(const Vector2 &pos)
{
	m_ListenerPos = pos;
	//play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	//play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================






//**************************************************************************************************************
//
//		BGM管理クラス
//
//**************************************************************************************************************


//=============================================================================================
//		初	期	化
void BGM_Manager::Initialize()
{
	play_manager = new tdnSoundBGM;
	m_bPlayStream = false;

	for (int i = 0;; i++)
	{
		if (strcmp(all_dataB[i].id, "END") == 0) break;	// 終端

		ID[all_dataB[i].id] = i;
		play_manager->Set(i, all_dataB[i].file_name, all_dataB[i].b3D);
	}

	// リスナーの初期設定
	play_manager->SetListenerAll(
		Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// リスナー座標(画面の真ん中と仮定する)
		Vector3(1, 0, 0),	// 正面ベクトル
		Vector3(0, 1, 0),	// 上方ベクトル
		Vector3(0, 0, 0)	// 移動値
		);
}
//
//=============================================================================================



//=============================================================================================
//		解		放
BGM_Manager::~BGM_Manager()
{
	delete play_manager;
}
//
//=============================================================================================

//=============================================================================================
//		更		新
void BGM_Manager::Update()
{
	play_manager->Update();
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		処		理
void BGM_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->Play(data_num, loop);
	}
}

void BGM_Manager::Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->Play(data_num, pos, front, move, loop);
	}
}

void BGM_Manager::Play(LPSTR _ID, bool loop)
{
	Play_in(ID[_ID], loop);
}

void BGM_Manager::Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	return Play_in(ID[_ID], pos, front, move, loop);
}

void BGM_Manager::Stop(LPSTR _ID)
{
	play_manager->Stop(ID[_ID]);
}

void BGM_Manager::Stop_all()
{
	play_manager->AllStop();
}

void BGM_Manager::Fade_in(LPSTR _ID, float fade_speed, bool loop)
{
	play_manager->FadeIn(ID[_ID], fade_speed, loop);
}

void BGM_Manager::Fade_out(LPSTR _ID, float fade_speed)
{
	play_manager->FadeOut(ID[_ID], fade_speed);
}

void BGM_Manager::Cross_fade(LPSTR inID, LPSTR outID, float fade_speed, bool loop)
{
	play_manager->CrossFade(ID[inID], ID[outID], fade_speed, tdnSoundBGM::CROSS_FADE_TYPE::END_OF_ETERNITY, loop);
}

void BGM_Manager::Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop)
{
	play_manager->CrossFade(ID[inID], ID[outID], in_speed, out_speed, tdnSoundBGM::CROSS_FADE_TYPE::END_OF_ETERNITY, loop);
}

void BGM_Manager::SetListener(const Vector2 &pos, const Vector2 &move)
{
	play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================



//=============================================================================================
//		実		体
SE_Manager *se;
BGM_Manager *bgm;
//
//=============================================================================================



MyMusicManager::MyMusicManager(MY_MUSIC_ID id) :m_bPlay(false), m_pStreamSound(nullptr), m_pHeaveHoStream(nullptr), m_bHeaveHoPlay(false)
{
	std::string directory = "DATA/Sound/BGM/MyMusic";
	switch (id)
	{
	case MY_MUSIC_ID::SELECT:
		directory += "/Select";
		break;
	case MY_MUSIC_ID::SENJO:
		directory += "/Senjo";
		break;
	case MY_MUSIC_ID::SYUTEN:
		directory += "/Syuten";
		break;
	case MY_MUSIC_ID::A:
		directory += "/A";
		break;
	default:
		assert(0);
		break;
	}

	// ファイル読み込み
	std::ifstream ifs((char*)(directory + "/info.txt").c_str());
	MyAssert(ifs, "エラー: オレ曲の読み込みに失敗");

	// 終端まで
	while (!ifs.eof())
	{
		char MusicName[256], FileName[256], path[256];
		int percent;

		// オレ曲情報読み取り
		ifs >> MusicName;
		ifs >> FileName;
		sprintf_s(path, 256, "%s/%s", directory.c_str(), FileName);	// パス作成
		ifs >> percent;

		// リストに突っ込む
		m_list.push_back(new MyMusic(MusicName, path, percent));
	}

}

MyMusicManager::~MyMusicManager()
{
	for (auto it : m_list) delete it;	// リスト消去
	
	// 曲のストップ
	if(m_bPlay)m_pStreamSound->Stop();	
	if(m_bHeaveHoPlay)m_pHeaveHoStream->Stop();
}

void MyMusicManager::Play()
{
	// 2重再生防止
	if (m_bPlay) return;

	// 結構アレなコードなので、気が向いたら程度に直す
	int count(0);
	for (auto &it : m_list)
	{
		// 全曲のパーセントの数ぶん、数字をカウントする
		count += it->percent;
	}

	// 数字格納領域確保
	int *numbers = new int[count];

	int count2(0);
	for (UINT i = 0; i < m_list.size();i++)
	{
		for (int remain = m_list[i]->percent - 1; remain >= 0; remain--)
		{
			// 数字格納領域に曲の番号を入れていく
			numbers[count2++] = i;
		}
	}

	// その数字格納領域の中からランダムで抽選、引いた数字を再生
	int r = (true) ? tdnRandom::Get(0, count - 1) : rand() % count;
	m_pStreamSound = bgm->PlayStream((char*)m_list[numbers[r]]->path.c_str());
	m_bPlay = true;	// 再生フラグON

	// 解放
	delete[] numbers;
}

void MyMusicManager::PlayHeaveHo()
{
	// 現在流してるやつと超必殺のBGMをクロスフェードさせる
	m_pStreamSound->SetMode(STR_FADEOUT, 16);
	m_bPlay = false;

	m_pHeaveHoStream = bgm->PlayStream("DATA/Sound/BGM/battle/HeaveHo.ogg");
	m_bHeaveHoPlay = true;
}