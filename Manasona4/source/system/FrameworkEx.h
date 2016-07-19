#pragma once
#include "TDNLIB.h"
#include "BaseScene.h"


/****************************/
//		フレームワークEx
/****************************/


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
	void ChangeScene(BaseScene* newScene)
	{
		//	現在のシーン解放
		if (m_scene != NULL) delete m_scene;
		//	シーンの切り替え＆初期化
		m_scene = newScene;
		m_scene->Initialize();
	}

private:
	// 自分自身
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
#define MainFrameEx FrameworkEx::GetInstance()
