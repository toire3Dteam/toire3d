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
#include "MenuUI\TipsCard.h"

// イラスト用
#include "Collect\PictureManager.h"

// 前方宣言
class MyMusicManager;
class Camera;
class RoundCallManager;
class BaseWindow;
class CoinUI;

// タグの種類
enum class TAG_TYPE
{
	TROPHY,
	PICTURE,

};

// ヒントカードの種類
enum class TIPS_TYPE_COLLECT
{
	BACK_MENU,		// メニューに戻る確認
	CONTENT_BUY,	// 購入確認

	ARRAY_END		// 配列終了

};

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
	void TrophyFirstAction();
	void TrophyUpdate();
	void TrophyRender();
	void TrophyCtrl(int iDeviceID);

	//------------------------------------------------------
	//	イラスト
	//------------------------------------------------------
	void PictureInit();
	void PictureFirstAction();
	void PictureUpdate();
	void PictureRender();

	void PictureInitExe();
	void PictureRenderExe();
	void PictureCtrl(int iDeviceID);

	//------------------------------------------------------
	//	アクセサー
	//------------------------------------------------------
	// ステートマシン取得
	StateMachine<sceneCollect> *GetFSM() { return m_pStateMachine; }
	
	TipsCard* GetTips(TIPS_TYPE_COLLECT type) { return m_pTips[(int)type]; }
	int GetCtrlDevice() { return m_iCtrlDevice; }
	void SetCtrlDevice(int set) { m_iCtrlDevice = set; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	enum IMAGE
	{
		BACK,
		SCROLL_BAR,
		SCROLL_ARROW,
		TITLE,
		INFO_PLATE,
		INFO_PLATE_WATCH,
		TAG_TROPHY,
		TAG_PICTURE,
		TROPHY_COMP_PERCENT,
		PRICE_PLATE,
		NUMBER,
		NUMBER_TROPHY,
		ARRAY_END
	};

	tdn2DAnim* GetImage(IMAGE eImage) { return m_pImages[(int)eImage]; };

	TAG_TYPE GetTagType() { return m_eTagType; };
	void SetTagType(TAG_TYPE eTagType) { m_eTagType = eTagType; }

	PictureManager* GetPictureMgr() { return m_pPictureMgr; }


private:
	static int m_iCtrlDevice;
	static TAG_TYPE m_eTagType;

	// 確認用ティップス
	TipsCard* m_pTips[(int)TIPS_TYPE_COLLECT::ARRAY_END];

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

	// タグの場所
	Vector2 m_vTagPos;

	// トロフィーコンプの場所
	Vector2 m_vTrophyPercentPos;

	// 画像鑑賞前の黒いフェード
	bool m_bBlackRect;
	int	 m_iBlackRectAlpha;

	// 画像鑑賞の時の説明文ありなし
	bool m_bWatchInfo;

	CoinUI* m_pCoinUI;

	//+-------------------------------
	//	トロフィーで必要な変数を用意
	//+-------------------------------
	
	// アイコンの情報が詰まった構造体
	struct IconDesc
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
		int iScrollAddX;		//	配置間隔
		int iScrollAddY;

		IconDesc();
	};
	IconDesc m_tagTI;

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

	//+-------------------------------
	//	イラストで必要な変数を用意
	//+-------------------------------

	// ★イラストマネージャー
	PictureManager* m_pPictureMgr;
	IconDesc m_tagPI;

	// ステートをフレンドクラスへ
	friend class SceneCollectState::TrophyStep;
	friend class SceneCollectState::PictureStep;
	friend class SceneCollectState::PictureWatch;
	friend class SceneCollectState::BuyPictureStep;
	friend class SceneCollectState::BackMenuStep;


};