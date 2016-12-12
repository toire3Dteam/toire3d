#pragma once

// 他からインクルードするとBaseSceneがどうこう言われるので
#include "../system/FrameworkEx.h"

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// メニューマネージャー
#include "MenuUI\MenuUIManager.h"
#include "MenuUI\ControllerSelectUI.h"

// テンプレートクラスをフレンドクラスで扱うためには先に定義する必要がある
#include "SceneMenuState.h"


// 前方宣言
class MyMusicManager;
class Camera;
class RoundCallManager;
class BaseWindow;
//+-----------------------------------
//	シーンメニュー
//+-----------------------------------

// ヒントカードの種類
enum class TIPS_TYPE
{
	TUTORIAL, // チュートリアル確認
	
	ARRAY_END	// 配列終了

};

// メニューのウィンドウの種類
enum class WINDOW_TYPE
{
	OPTION,		// オプション
	SOUND,		// サウンド
	ARRAY_END	// 配列終了

};

class sceneMenu : public BaseScene, BaseGameEntity
{
public:
	//初期化・解放
	sceneMenu();
	bool Initialize();
	~sceneMenu();
	//処理
	void Update();
	//描画
	void Render();

	// ステートマシン取得
	StateMachine<sceneMenu> *GetFSM() { return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	// アクセサー
	MenuUIManager* GetMenuUIMgr() { return m_pMenuUIMgr; };
	int GetSelectNo() { return m_iSelectNo; }
	void AddSelectNo(int add) { m_iSelectNo += add; }
	void SetSelectNo(int set) { m_iSelectNo = set; }

	int GetCtrlDevice() { return m_iCtrlDevice; }
	void SetCtrlDevice(int set) { m_iCtrlDevice = set; }

	ControllerSelectUI* GetCtrlSelectUI() { return m_pCtrlSelectUI; }

	TipsCard* GetTips(TIPS_TYPE type) { return m_pTips[(int)type]; }
	BaseWindow* GetWindow(WINDOW_TYPE type) { return m_pWindow[(int)type]; }

private:

	// メニューUI
	MenuUIManager*	m_pMenuUIMgr;
	static int m_iSelectNo;
	static int m_iCtrlDevice;

	// コントローラーセレクト用
	ControllerSelectUI* m_pCtrlSelectUI;

	// 確認用ティップス
	TipsCard* m_pTips[(int)TIPS_TYPE::ARRAY_END];

	// Window類
	BaseWindow* m_pWindow[(int)WINDOW_TYPE::ARRAY_END];

	StateMachine<sceneMenu> *m_pStateMachine;	// ステートマシン
												

	// ステートをフレンドクラスへ
	friend class SceneMenuState::FirstStep;	

};