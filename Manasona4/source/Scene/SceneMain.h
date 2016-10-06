#pragma once

// 他からインクルードするとBaseSceneがどうこう言われるので
#include "../system/FrameworkEx.h"

// エンティティ関連のインクルード
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
class MyMusicManager;
class Camera;

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

private:

	Stage::Base *m_pStage;			// ステージさん
	//Camera *m_pCamera;				// カメラさん
	MyMusicManager *m_pMyMusicMgr;	// オレ曲管理さん
	
	tdn2DObj* m_stageScreen;		// ステージオンリーのレンダーターゲット

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;

	StateMachine<sceneMain> *m_pStateMachine;	// ステートマシン
};