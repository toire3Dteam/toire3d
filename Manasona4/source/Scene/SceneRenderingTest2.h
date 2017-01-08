#pragma once

// 前方宣言
class MyMusicManager;
class sceneRenderingTest2 : public BaseScene
{
public:
	//初期化・解放
	bool Initialize();
	~sceneRenderingTest2();
	//処理
	void Update();
	//描画
	void Render();
	void RenderShadow();
	void SurfaceRender();

	// 水用の環境マップ
	void WaterEnvRender();

private:
	// 簡易カメラ
	struct Camera
	{
		Vector3 pos;
		Vector3 target;
	}m_camera;

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;

	// 水用
	Surface*	m_pStencilSurface;	// 奥行
	tdn2DObj*	m_pWaterEnvScreen;	// ゲーム画面


	// ポイントライト配列
	std::vector<Vector3> m_aPos;


	// 兄貴
	iex3DObj* m_pAniki;

};