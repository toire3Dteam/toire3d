#pragma once

// 他からインクルードするとBaseSceneがどうこう言われるので
#include "../system/FrameworkEx.h"

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

#include "MenuUI\TutorialManager.h"

// ポーズウィンドウ
#include "Window\PauseWindow.h"
#include "Window\TutorialPauseWindow.h"
#include "Window\TrainingPauseWindow.h"
#include "Window\SoundWindow.h"
#include "Window\TrainingOptionWindow.h"
#include "Window\TrainingDummyWindow.h"
#include "Window\TrainingCharacterWindow.h"
#include "Window\ChallengePauseWindow.h"

// リザルトデータ
#include "Data\ResultData.h"

// 前方宣言
class ShotManager;
class Camera;
class RoundCallManager;
class OverDriveStage;
class SceneSwitchPrev;

//class Stage::Base;	// ×
namespace Stage{		// ○
	class Base;
}

// バトルのウィンドウの種類
enum class BATTLE_WINDOW_TYPE
{
	PAUSE,					// ポーズ
	TUTORIAL_PAUSE,			// チュートリアルポーズ
	TRAINING_PAUSE,			// トレーニングポーズ
	CHALLENGE_PAUSE,		// チャレンジポーズ
	SOUND,					// サウンド
	TRAINING_OPTION,		// トレーニングオプション
	TRAINING_DUMMY,			// トレーニングダミーオプション
	TRAINING_CHARACTER,		// トレーニングキャラクターオプション
	ARRAY_END				// 配列終了

};

class sceneMain : public BaseScene, BaseGameEntity
{
public:
	//初期化・解放
	sceneMain();
	bool Initialize();
	~sceneMain();
	//処理
	void Update();
	//描画
	void RenderStage();
	void Render();
	void RenderShadow();
	void SurfaceRender();

	void Reset();
	
	void InitTrainingData();	// ★トレーニングデータを適用

	void StartPosition();				//	開始座標
	void StartChallengePosition();		//	チャレンジ開始座標


	// ステートマシン取得
	StateMachine<sceneMain> *GetFSM(){ return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg){ return m_pStateMachine->HandleMessage(msg); }

	Stage::Base *GetStage() { return m_pStage;	}
	// ShotManager *GetShotManager(){ return m_pShotMgr; }

	void SetRoundNum(int roundNum) { m_iRoundNum = roundNum; }
	int GetRoundNum() { return m_iRoundNum; }

	RoundCallManager *GetRoundCall(){ return m_pRoundCallMgr; }

	SceneSwitchPrev *GetSceneSwitchPrev(){ return prevEF; }

	OverDriveStage* GetOverDriveStage() { return m_pOverDriveStage; }

	void OverDriveUpdate();
	void OverDriveAction();
	void OverDriveEnd();

	//TUTORIAL_TYPE GetSelectTutorial() { return m_eSelectTutorial; }
	//void SetSelectTutorial(TUTORIAL_TYPE tutorial) { m_eSelectTutorial = tutorial; }

	BaseWindow* GetWindow(BATTLE_WINDOW_TYPE type) { return m_pWindow[(int)type]; }

	void SetPause(bool flag) { m_bPause = flag; }

	bool GetTutorialFlag() { return m_bTutorialFlag; }

	// 
	tdn2DAnim* GetHideInfo() { return m_pHideInfo; };
	bool GetHideUI() { return m_bHideUI; }
	void SetHideUI(bool flag) { m_bHideUI = flag; }

	//
	int GetPauseDeviceID() { return m_iPauseDeviceID; }
	void SetPauseDeviceID(int iDeviceID) { m_iPauseDeviceID = iDeviceID; }
	
	SIDE GetCommandSide() { return m_eCommandSide; }
	void SetCommandSide(SIDE eCommand) { m_eCommandSide = eCommand; }

private:
	int m_iRoundNum;				// ラウンド数

	Stage::Base *m_pStage;			// ステージさん

	//Camera *m_pCamera;				// カメラさん
	//ShotManager *m_pShotMgr;		// ショット管理さん
	
	tdn2DObj* m_stageScreen;		// ステージオンリーのレンダーターゲット

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;
	bool m_bBakeStageShadow;		//	ステージの影を焼きこむフラグ

	StateMachine<sceneMain> *m_pStateMachine;	// ステートマシン

	RoundCallManager* m_pRoundCallMgr;

	SceneSwitchPrev* prevEF;

	// マスク
	tdn2DObj* m_pMaskScreen;
	bool  m_bOverDriveStageFlag;
	float m_fMaskRate;
	float m_fMaskPower;

	//★必殺の時だけ呼ぶようにおなシャス
	OverDriveStage* m_pOverDriveStage;		// 必殺用の背景	

	// チュートリアルかどうか
	bool m_bTutorialFlag;
	//TUTORIAL_TYPE m_eSelectTutorial;		// 選択してるチュートリアル

	// トレーニングかどうか
	bool m_bTrainingFlag;

	// チャレンジかどうか
	bool m_bChallengeFlag;

	// メニュー非表示用
	tdn2DAnim* m_pHideInfo;
	bool	   m_bHideUI;

	// Window類
	BaseWindow* m_pWindow[(int)BATTLE_WINDOW_TYPE::ARRAY_END];
	bool m_bPause;

	// ポーズ押した人のデバイスID
	int m_iPauseDeviceID;
	SIDE m_eCommandSide;

	// ★★★シームレスに読み込むので、イニシャライズで初期化すると都合が悪い(例えばストリーミングBGMとか)やつをUpdateで1回だけ呼び出したいときに使う
	bool m_bFirstUpdate;
};