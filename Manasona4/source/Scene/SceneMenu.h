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

//+-----------------------------------
//	シーンメニュー
//+-----------------------------------

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

	ControllerSelectUI* GetCtrlSelectUI() { return m_pCtrlSelectUI; }

private:

	// メニューUI
	MenuUIManager*	m_pMenuUIMgr;
	static int m_iSelectNo;

	// コントローラーセレクト用
	ControllerSelectUI* m_pCtrlSelectUI;

	StateMachine<sceneMenu> *m_pStateMachine;	// ステートマシン
												

	// 奇跡のフレンドクラスの使い道
	friend class SceneMenuState::FirstStep;	

};