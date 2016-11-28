#pragma once

#include "../BaseEntity/Message/Message.h"

// 前方宣言
class MyMusicManager;
class Camera;
class BaseResultPerformance;
class SceneSwitch;
class UVEffectManager;
class EffectCamera;
class ScoreUI;
class ResultWindow;

//class Stage::Base;	// ×
namespace Stage{		// ○
	class Base;
}

class sceneResult: public BaseScene
{
public:
	//初期化・解放
	sceneResult(SIDE WinnerSide);
	bool Initialize();
	~sceneResult();
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
	// ステージ
	iexMesh* m_grand;


	// 勝者
	SIDE m_WinnerSide;
	iex3DObj* m_WinPlayer;
	Vector3 m_winPos;

	// 敗者
	iex3DObj* m_LosePlayer;
	Vector3 m_losePos;
	int m_iLoseMotion;

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

	// カメラ用
	ViewData m_ViewData;
	EffectCamera *m_pEffectCamera;

	// スコア用UI
	ScoreUI* m_pScoreUI;

	// Info
	tdn2DAnim* m_pInfoPlate;

	// リザルトウィンドウ
	ResultWindow* m_pResultWindow;

	// 簡易ステートマシン
	enum STEP
	{
		INTRO, SCORE, WINDOW_SELECT, END
	};
	STEP m_eStep;

	// ★★★シームレスに読み込むので、イニシャライズで初期化すると都合が悪い(例えばストリーミングBGMとか)やつをUpdateで1回だけ呼び出したいときに使う
	bool m_bFirstUpdate;
};
