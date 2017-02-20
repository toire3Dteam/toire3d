#pragma once
#include "TDNLIB.h"
#include "BaseScene.h"
#include "Framework.h"


/****************************/
//		フレームワークEx
/****************************/
enum class FPS_MODE
{
	NORMAL,
	FPS_30,
	FPS_40,
	FPS_45,
};

class FrameworkEx
{
private:
	//	初期化・解放
	FrameworkEx();
	~FrameworkEx();
	// 封印
	FrameworkEx(const FrameworkEx&) {}
	FrameworkEx& operator=(const FrameworkEx&) {}

public:
	//シングルトン
	static FrameworkEx* GetInstance();
	//  解放
	static void Release();

	//	更新・描画
	bool Update();	
	void Render();

	//	シーンの切り替え
	void ChangeScene(BaseScene* newScene, bool bLoadingScene = false);

	// FPSモード切替
	FPS_MODE GetFPSMode() { return m_eFPSMode; }
	void SetFPSMode(FPS_MODE eFpsMode) { m_eFPSMode = eFpsMode; }

private:
	// 実体
	static FrameworkEx* m_pInstance;
	
	/**********************/
	//	フレームレート関連
	/**********************/
	double m_dFreq;		// 一秒当たりの周波数

	// 1フレームを何FPSにするかに必要な変数
	double m_dFlameTime;	// フレームの時間
	double m_dCurrentTime;	// 現在の時間

	// １秒間に何FPSか表記するために必要なもの
	int m_iFPS;
	int m_iRenderFPS;

	int m_iWorkFPS;
	int m_iWorkRenderFPS;

	// 描画・更新フラグ
	bool m_bRenderFlag;
	bool m_bUpdateFlag;
	bool m_bSlowFlag;

	// FPSのモード
	FPS_MODE m_eFPSMode;

	// 起動からのフレーム数
	DWORD m_dwGameFrame;	

	// FPSの操作
	bool FPSCtrl();
	/*************/
	//	シーン
	/*************/
	BaseScene	*m_scene;

	/*************/
	//	動画
	/*************/
	bool m_bMoviePlay; //	動画を再生しているかどうか
};

// インスタンス
#define MainFrameEX FrameworkEx::GetInstance()
