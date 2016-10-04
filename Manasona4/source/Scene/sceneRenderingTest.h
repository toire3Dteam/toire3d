#pragma once

// 前方宣言
class MyMusicManager;
class sceneRenderingTest : public BaseScene
{
public:
	//初期化・解放
	bool Initialize();
	~sceneRenderingTest();
	//処理
	void Update();
	//描画
	void Render();
	void RenderShadow();
	void SurfaceRender();

private:
	// 簡易カメラ
	struct Camera
	{
		Vector3 pos;
		Vector3 target;
	}m_camera;

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;
};