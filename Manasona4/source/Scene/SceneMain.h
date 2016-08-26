#pragma once

// 前方宣言
class MyMusicManager;
class Camera;
//class Stage::Base;	// ×
namespace Stage{		// ○
	class Base;
}

class sceneMain	: public BaseScene
{
public:
	//初期化・解放
	bool Initialize();
	~sceneMain();
	//処理
	bool Update();
	//描画
	void RenderStage();
	void Render();
	void RenderShadow();
	void SurfaceRender();

private:

	Stage::Base *m_pStage;			// ステージさん
	Camera *m_pCamera;				// カメラさん
	MyMusicManager *m_pMyMusicMgr;	// オレ曲管理さん
	
	tdn2DObj* m_stageScreen;		// ステージオンリーのレンダーターゲット

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;
};