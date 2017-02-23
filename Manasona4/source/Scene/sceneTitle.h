#pragma once

// 他からインクルードするとBaseSceneがどうこう言われるので
#include "../system/FrameworkEx.h"

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// テンプレートクラスをフレンドクラスで扱うためには先に定義する必要がある
#include "sceneTitleState.h"

class sceneTitle : public BaseScene, BaseGameEntity
{
public:
	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	sceneTitle();
	bool Initialize();
	~sceneTitle();


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	void Update();
	void Render();

	// ステートマシン取得
	StateMachine<sceneTitle> *GetFSM() { return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }


private:

	StateMachine<sceneTitle> *m_pStateMachine;	// ステートマシン

	enum IMAGE
	{
		//LOGO,	// タイトルロゴ
		MOVIE,	// ムービー
		BG,		// 背景
		TITLE,	// タイトル
		CLOUD1, // 雲1
		CLOUD2, // 雲2
		MAX
	};

	tdn2DObj *m_pImages[IMAGE::MAX];	// 画像
	tdnMovie *m_pMovie;			// 動画	

	// 背景用
	tdnMovie* m_pMovieBGLine;
	tdn2DObj* m_pBGLine;


	// ロゴ
	tdn2DAnim* m_pLogo; 
	
	// プレスエニーボタンの
	tdn2DAnim* m_pPreaseAnyButton;

	// 雲のスクロール処理
	float m_fCloudU;

	// ゲームタイマー
	int m_iGameTimer;

	// ステートをフレンドクラスへ
	friend class SceneTitleState::Intro;
	friend class SceneTitleState::LogoStep;
	friend class SceneTitleState::MovieStep;
	friend class SceneTitleState::TitleStep;
	friend class SceneTitleState::End;
	friend class SceneTitleState::BackMenu;
};
