#pragma once

// 前方宣言
class BasePlayer;

//*****************************************************************************
//
//		コマンドのセーブデータ
//
//*****************************************************************************
class CommandSaver
{
public:
	//------------------------------------------------------
	//	コマンド計測・終了
	//------------------------------------------------------
	void Start(BasePlayer *pPlayer);
	void End();

	//------------------------------------------------------
	//	初期化
	//------------------------------------------------------
	void Initialize()
	{
		m_pRefPlayer = nullptr; 
		m_vCommandBuffer.clear();
	}

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	ゲッター
	//------------------------------------------------------
	// 計測フレーム数取得
	unsigned int GetFrame(){ return m_vCommandBuffer.size(); }

private:

	//------------------------------------------------------
	//	変数
	//------------------------------------------------------
	BasePlayer *m_pRefPlayer;				// 参照するプレイヤー
	std::vector<WORD> m_vCommandBuffer;		// コマンド保存用
};


//*****************************************************************************
//
//		コマンド
//
//*****************************************************************************
class CommandManager
{
public:
	//------------------------------------------------------
	//	実体取得
	//------------------------------------------------------
	static CommandManager *GetInstance(){ static CommandManager instance; return &instance; }

	//------------------------------------------------------
	//	保存したコマンド発動
	//------------------------------------------------------
	void Action(LPCSTR lpcCommandFilePath)
	{
		m_iCurrentFrame = 0;

		// コマンドをロードする
		if (!LoadCommand(lpcCommandFilePath))return;

		m_bAction = true;
		m_bEnd = false;
	}

	//------------------------------------------------------
	//	初期化
	//------------------------------------------------------
	void Initialize()
	{
		m_iCurrentFrame = 0;
		m_iNumCommand = 0;
		m_bAction = false;
		m_bEnd = false;
		SAFE_DELETE(m_paCommandBuffer);

		// セーブくん初期化
		if (!m_pCommandSaver) m_pCommandSaver = new CommandSaver;
		m_pCommandSaver->Initialize();
	}

	//------------------------------------------------------
	//	解放
	//------------------------------------------------------
	void Release()
	{
		SAFE_DELETE(m_pCommandSaver);
		SAFE_DELETE(m_paCommandBuffer);
	}

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update()
	{
		// 発動してなかったら出ていけぇ！！
		if (!m_bAction || m_bEnd) return;

		// コマンド再生カーソルを進める
		if (++m_iCurrentFrame >= m_iNumCommand)
		{
			m_bAction = false;
			m_bEnd = true;
		}
	}

	//------------------------------------------------------
	//	ゲッター
	//------------------------------------------------------
	// 現在再生しているカーソルのコマンド
	WORD GetCommand(){ return (m_iCurrentFrame < m_iNumCommand && m_paCommandBuffer) ? m_paCommandBuffer[m_iCurrentFrame] : 0; }
	// コマンド再生の終了フラグ
	bool isAction(){ return m_bAction; }
	bool isEnd(){ return m_bEnd; }

	//------------------------------------------------------
	//	コマンドのセーブ関連
	//------------------------------------------------------
	void SaveUpdate(){ m_pCommandSaver->Update(); }
	void SaveStart(BasePlayer *pPlayer){ m_pCommandSaver->Start(pPlayer); }
	void SaveEnd(){ m_pCommandSaver->End(); }
	int GetSaveFrame(){ return m_pCommandSaver->GetFrame(); }

private:
	//------------------------------------------------------
	//	コマンドのロード
	//------------------------------------------------------
	bool LoadCommand(LPCSTR lpcCommandFilePath)
	{
		FILE *fp;
		if (fopen_s(&fp, lpcCommandFilePath, "rb") != 0)
		{
			MyAssert(0, "コマンドのセーブデータが入っていません。\n%s", lpcCommandFilePath);	// まず止まることはないと思うが…
			return false;
		}

		// フレーム数読み込み
		fread_s((LPSTR)&m_iNumCommand, sizeof(int), sizeof(int), 1, fp);
		if (m_iNumCommand <= 0)
		{
			MyAssert(0, "コマンド数がおかしい");
			return false;
		}

		// コマンドバッファの確保
		if (m_paCommandBuffer) delete m_paCommandBuffer;
		m_paCommandBuffer = new WORD[m_iNumCommand];

		// コマンドデータ読み込み
		for (int i = 0; i < m_iNumCommand; i++)
		{
			fread_s((LPSTR)&m_paCommandBuffer[i], sizeof(WORD), sizeof(WORD), 1, fp);
		}

		// ファイル閉じる
		fclose(fp);

		return true;
	}

	//------------------------------------------------------
	//	変数
	//------------------------------------------------------
	int m_iNumCommand;			// コマンド数(保存しているフレーム)
	int m_iCurrentFrame;		// 再生位置
	WORD *m_paCommandBuffer;	// コマンドを格納する可変長ポインタ(値は分かりきっているのでポインタにした)
	bool m_bAction;
	bool m_bEnd;

	CommandSaver *m_pCommandSaver;	// コマンドをセーブするやつの実体

	//------------------------------------------------------
	//	シングルトンの作法
	//------------------------------------------------------
	CommandManager() :m_pCommandSaver(nullptr), m_paCommandBuffer(nullptr){ Initialize(); }
	CommandManager(const CommandManager&){}
	CommandManager &operator =(const CommandManager&){}
};

/********************************************/
//	インスタンス簡略化
/********************************************/
#define CommandMgr (CommandManager::GetInstance())