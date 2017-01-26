#pragma once

#include "../BaseEntity/Message/Message.h"

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
	void Write(LPCSTR lpcFilename);

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
	bool isAction() { return (m_pRefPlayer != nullptr); }
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
	void Action(LPCSTR lpcCommandFilePath, SIDE side = SIDE::LEFT)
	{
		m_tagDatas[(int)side].iCurrentFrame = 0;

		// コマンドをロードする
		if (!LoadCommand(lpcCommandFilePath, side))return;

		m_bAction = true;
		m_bEnd = false;
	}

	//------------------------------------------------------
	//	初期化
	//------------------------------------------------------
	void Initialize()
	{
		m_bAction = false;
		m_bEnd = false;

		FOR((int)SIDE::ARRAY_MAX)
		{
			m_tagDatas[i].Initialize();

			// セーブくん初期化
			if (!m_pCommandSaver[i])
			{
				m_pCommandSaver[i] = new CommandSaver;
				m_pCommandSaver[i]->Initialize();
			}
		}
	}

	//------------------------------------------------------
	//	解放
	//------------------------------------------------------
	void Release()
	{
		FOR((int)SIDE::ARRAY_MAX)
		{
			SAFE_DELETE(m_pCommandSaver[i]);
			m_tagDatas[i].Release();
		}
	}

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update(SIDE side)
	{
		// 発動してなかったら出ていけぇ！！
		if (!m_bAction || m_bEnd) return;

		// コマンド再生カーソルを進める
		if (++m_tagDatas[(int)side].iCurrentFrame >= m_tagDatas[(int)side].iNumCommand)
		{
			m_bAction = false;
			m_bEnd = true;
		}
	}

	//------------------------------------------------------
	//	停止
	//------------------------------------------------------
	void Stop()
	{
		m_bAction = false;
		m_bEnd = false;
	}

	//------------------------------------------------------
	//	ゲッター
	//------------------------------------------------------
	// 現在再生しているカーソルのコマンド
	WORD GetCommand(SIDE side){ return (m_tagDatas[(int)side].iCurrentFrame < m_tagDatas[(int)side].iNumCommand && m_tagDatas[(int)side].paCommandBuffer) ? m_tagDatas[(int)side].paCommandBuffer[m_tagDatas[(int)side].iCurrentFrame] : 0; }
	// コマンド再生の終了フラグ
	bool isAction(){ return m_bAction; }
	bool isEnd(){ return m_bEnd; }

	//------------------------------------------------------
	//	コマンドのセーブ関連
	//------------------------------------------------------
	void SaveUpdate(SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->Update(); }
	void SaveStart(BasePlayer *pPlayer, SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->Start(pPlayer); }
	void SaveEnd(SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->End(); }
	void SaveWrite(LPCSTR lpcFileName, SIDE side = SIDE::LEFT){ m_pCommandSaver[(int)side]->Write(lpcFileName); }
	int GetSaveFrame(SIDE side = SIDE::LEFT){ return m_pCommandSaver[(int)side]->GetFrame(); }
	bool SaveIsAction(SIDE side = SIDE::LEFT){ return m_pCommandSaver[(int)side]->isAction(); }

private:
	//------------------------------------------------------
	//	コマンドのロード
	//------------------------------------------------------
	bool LoadCommand(LPCSTR lpcCommandFilePath, SIDE side)
	{
		FILE *fp;
		if (fopen_s(&fp, lpcCommandFilePath, "rb") != 0)
		{
			MyAssert(0, "コマンドのセーブデータが入っていません。\n%s", lpcCommandFilePath);	// まず止まることはないと思うが…
			return false;
		}

		// フレーム数読み込み
		int l_iNumCommand;
		fread_s((LPSTR)&l_iNumCommand, sizeof(int), sizeof(int), 1, fp);
		if (l_iNumCommand <= 0)
		{
			MyAssert(0, "コマンド数がおかしい");
			return false;
		}

		// コマンドバッファの確保
		m_tagDatas[(int)side].Partition(l_iNumCommand);

		// コマンドデータ読み込み
		for (int i = 0; i < l_iNumCommand; i++)
		{
			fread_s((LPSTR)&m_tagDatas[(int)side].paCommandBuffer[i], sizeof(WORD), sizeof(WORD), 1, fp);
		}

		// ファイル閉じる
		fclose(fp);

		return true;
	}

	//------------------------------------------------------
	//	変数
	//------------------------------------------------------
	struct Data
	{
		int iNumCommand;			// コマンド数(保存しているフレーム)
		int iCurrentFrame;		// 再生位置
		WORD *paCommandBuffer;	// コマンドを格納する可変長ポインタ(値は分かりきっているのでポインタにした)
		Data() :iNumCommand(0), iCurrentFrame(0), paCommandBuffer(nullptr){}
		~Data(){ SAFE_DELETE(paCommandBuffer); }
		void Initialize()
		{
			iNumCommand = 0;
			iCurrentFrame = 0;
			SAFE_DELETE(paCommandBuffer); 
		}
		void Release(){ SAFE_DELETE(paCommandBuffer); }
		void Partition(int num)
		{
			if (paCommandBuffer) delete paCommandBuffer;
			paCommandBuffer = new WORD[num];
			iNumCommand = num;
		}
	}m_tagDatas[(int)SIDE::ARRAY_MAX];
	bool m_bAction;
	bool m_bEnd;

	CommandSaver *m_pCommandSaver[(int)SIDE::ARRAY_MAX];	// コマンドをセーブするやつの実体

	//------------------------------------------------------
	//	シングルトンの作法
	//------------------------------------------------------
	CommandManager()
	{
		m_pCommandSaver[0] = nullptr;
		m_pCommandSaver[1] = nullptr;
		Initialize(); 
	}
	CommandManager(const CommandManager&){}
	CommandManager &operator =(const CommandManager&){}
};

/********************************************/
//	インスタンス簡略化
/********************************************/
#define CommandMgr (CommandManager::GetInstance())