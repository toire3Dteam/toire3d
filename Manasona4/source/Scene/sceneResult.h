#pragma once



// 前方宣言
class MyMusicManager;
class Camera;
class BaseResultPerformance;
class SceneSwitch;
class UVEffectManager;

//class Stage::Base;	// ×
namespace Stage{		// ○
	class Base;
}

class sceneResurt: public BaseScene
{
public:
	//初期化・解放
	bool Initialize();
	~sceneResurt();
	//処理
	void Update();
	//描画
	void Render();
	
	// それぞれの描画先
	void BackRender();
	void CameraRender();
	void FrontRender();
	void AllRender();


private:

	// 勝者
	iex3DObj* m_WinPlayer;
	Vector3 m_winPos;

	// 敗者
	iex3DObj* m_LosePlayer;
	Vector3 m_losePos;

	/****************************/
	// ★３のレンダーターゲット
	tdn2DObj* m_BackScreen;
	tdn2DObj* m_CameraScreen;
	tdn2DObj* m_FrontScreen;

	tdn2DObj* m_AllScreen;

	// 描画先
	Surface* m_pBackBuffer;
	/****************************/
	
	static const int PERFORMANCE_ACTION_TIME_MAX = 150;
	static const int PERFORMANCE_TIME_MAX = 150;
	//  パフォーマンス２D
	int m_iPerformanceActionTimer;
	int m_iPerformanceTimer;
	bool m_bPerformanceFlag;

	// エンド
	bool m_bEndFlag;

	Vector3 m_dirLight;// ライト方向ﾆｷ
	bool m_bShaderFlag;

	// 
	tdn2DObj* m_move;
	tdnMovie*  m_moveMgr;

	// ★各自キャラクターのリザルトの演出
	BaseResultPerformance* m_ResultPerformance;
	enum class WINNER_PLAYER 
	{
		ARAMITAMA,AIROU
	};

	SceneSwitch* m_sceneSwitch;

	tdn2DAnim* m_winnerPic;
	float m_iWinnerPicX;

	// UVエフェクト
	UVEffectManager* m_UVEffectMgr;

	// ウェーブスクリーン
	tdn2DObj* m_wavePic;
	tdn2DObj* m_waveScreen;
	float m_waveTU;
	void WaveRender();

	// ストリーミング用
	tdnStreamSound *m_pStream;
};
