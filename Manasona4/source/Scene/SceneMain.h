#pragma once

// 他からインクルードするとBaseSceneがどうこう言われるので
#include "../system/FrameworkEx.h"

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
//class MyMusicManager;
class Camera;
class RoundCallManager;
class OverDriveStage;
class SceneSwitchPrev;

//class Stage::Base;	// ×
namespace Stage{		// ○
	class Base;
}

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
	
	// ステートマシン取得
	StateMachine<sceneMain> *GetFSM(){ return m_pStateMachine; }

	// メッセージ受信(BaseEntityによるオーバーライド)
	bool  HandleMessage(const Message& msg){ return m_pStateMachine->HandleMessage(msg); }

	Stage::Base *GetStage() { return m_pStage;	}
	//MyMusicManager *GetMyMusicManager(){ return m_pMyMusicMgr; }

	void SetRoundNum(int roundNum) { m_iRoundNum = roundNum; }
	int GetRoundNum() { return m_iRoundNum; }

	RoundCallManager *GetRoundCall(){ return m_pRoundCallMgr; }

	SceneSwitchPrev *GetSceneSwitchPrev(){ return prevEF; }

	OverDriveStage* GetOverDriveStage() { return m_pOverDriveStage; }

	void OverDriveUpdate();
	void OverDriveAction();
	void OverDriveEnd();

private:
	int m_iRoundNum;				// ラウンド数

	Stage::Base *m_pStage;			// ステージさん

	//Camera *m_pCamera;				// カメラさん
	//MyMusicManager *m_pMyMusicMgr;	// オレ曲管理さん
	
	tdn2DObj* m_stageScreen;		// ステージオンリーのレンダーターゲット

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;

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

	// （仮）チュートリアルかどうか
	bool m_bTutorialFlag;

};