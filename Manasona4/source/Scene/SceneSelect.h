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
#include "sceneSelectState.h"

// セレクト
#include "SelectUI\SelectUIManager.h"



// 前方宣言
class MyMusicManager;
class Camera;
class RoundCallManager;

//+-----------------------------------
//	シーンキャラクターセレクト
//+-----------------------------------

class sceneSelect: public BaseScene, BaseGameEntity
{
public:
	//初期化・解放
	sceneSelect();
	bool Initialize();
	~sceneSelect();
	//処理
	void Update();
	//描画
	void Render();
	void StageAndBGMRender();

	void FirstAction();

	// ステートマシン取得
	StateMachine<sceneSelect> *GetFSM() { return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	// アクセサー
	//MenuUIManager* GetMenuUIMgr() { return m_pMenuUIMgr; };
	//int GetSelectNo() { return m_iSelectNo; }
	//void AddSelectNo(int add) { m_iSelectNo += add; }
	//void SetSelectNo(int set) { m_iSelectNo = set; }

	SelectUIManager*	 GetSelectUIMgr() { return m_pSelectUIMgr; }
private:

	// selectUI
	SelectUIManager*	m_pSelectUIMgr;
	//static int m_iSelectNo;


	StateMachine<sceneSelect> *m_pStateMachine;	// ステートマシン
												
												// 画像
	enum PIC_TYPE
	{
		BG,
		TITLE,
		BLACK_LINE,
		INFO_PLATE,
		ARRAY_END,
	};
	tdn2DAnim* m_pPic[PIC_TYPE::ARRAY_END];

	int m_iRectAlpha;

	// ステージとBGMの選択番号
	static int m_iSelectStageNo;
	static int m_iSelectBGMNo;

	// ステージとBGM
	// 画像はっつけるだけ
	struct SECOND_SELECT
	{
		int iX, iY;

		bool bBGMSelect;	

		tdn2DObj* pPlate;
		tdn2DObj* pStagePlate;
		tdn2DObj* pBGMPlate;
		tdn2DObj* pStagePic;

		// 矢印
		tdn2DObj* pArrow;

		tdn2DAnim* pStageLeftRip;
		tdn2DAnim* pStageRightRip;			 
		tdn2DAnim* pBGMLeftRip;
		tdn2DAnim* pBGMRightRip;
	};

	SECOND_SELECT m_tagSecondSelect;


	// 奇跡のフレンドクラスの使い道
	friend class SceneSelectState::Intro;	
	friend class SceneSelectState::CharaSelect;
	friend class SceneSelectState::StageAndBGM;
};