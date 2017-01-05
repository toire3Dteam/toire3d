#pragma once

// 他からインクルードするとBaseSceneがどうこう言われるので
#include "../system/FrameworkEx.h"

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// メニューマネージャー
#include "MenuUI\MenuUIManager.h"
#include "MenuUI\ControllerSelectUI.h"

#include "SceneCollectState.h"

// 前方宣言
class MyMusicManager;
class Camera;
class RoundCallManager;
class BaseWindow;

namespace PlayerData
{
struct Infomation;
}

class sceneCollect : public BaseScene, BaseGameEntity
{
public:

	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	sceneCollect();
	bool Initialize();
	~sceneCollect();

	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	void Update();
	void Render();
 
	// 初期演出
	void FirstAction();

	void PiPiPiCtrl(int iDeviceID);

	//------------------------------------------------------
	//	トロフィー
	//------------------------------------------------------
	void TrophyInit();
	void TrophyUpdate();
	void TrophyRender();
	void TrophyCtrl(int iDeviceID);

	//------------------------------------------------------
	//	アクセサー
	//------------------------------------------------------
	// ステートマシン取得
	StateMachine<sceneCollect> *GetFSM() { return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	enum IMAGE
	{
		BACK,
		SCROLL_BAR,
		SCROLL_ARROW,
		TITLE,
		INFO_PLATE,
		ARRAY_END
	};

private:

	StateMachine<sceneCollect> *m_pStateMachine;	// ステートマシン


	tdn2DAnim *m_pImages[IMAGE::ARRAY_END];
	
	// プレイ情報
	PlayerData::Infomation *m_pPlayerInfo;

	// スクロール実験
	tdn2DObj* m_pScrollBar;
	tdn2DObj* m_pScrollArrow;

	// スクロール
	float m_iScrollHeight;	// 最大の高さ
	float m_iScrollScreenSize;// 一つのスクリーン幅
	Vector2 m_vScrollPos;

	//+-------------------------------
	//	トロフィーで必要な変数を用意
	//+-------------------------------
	
	// トロフィーのアイコンの情報が詰まった構造体
	struct TrophyIconDesc
	{
		int iTop;			// アイコンの一番↑
		int iBottom;		// アイコンの一番↓
		int iMaxHeight;	    // アイコンの最大縦幅
		int iRowNum;		// アイコンの列

		int iSelectNo;		// 選択しているアイコンの番号
		Vector2 vSelectPos;	// 選択している場所
		Vector2 vNextPos;	// 次に移動する場所
		tdn2DAnim* pSelectMark;// 選択中のマーク絵
		tdn2DAnim* pSelectMarkEdge;// 選択中のマーク絵(線)

		// スクロール
		Vector2 vScrollPos;	// 選択している場所
		Vector2 vScrollNextPos;	// 次に移動する場所

		TrophyIconDesc();
	};
	TrophyIconDesc m_tagTI;

	//+-------------------------------
	//	アイコン操作のための
	//+-------------------------------
	// ピッ...ピッ...ピッ..ピッ.ピッピッピッピッ
	// って感じの触り心地に
	int m_iSpeedLv;
	int m_iUpPushFrame;
	int m_iDownPushFrame;
	int m_iLeftPushFrame;
	int m_iRightPushFrame;

	bool m_bUpPush;			// 上押した
	bool m_bDownPush;		// 下押した
	bool m_bLeftPush;		// 左押した
	bool m_bRightPush;		// 右押した


	// ステートをフレンドクラスへ
	friend class SceneCollectState::TrophyStep;

};