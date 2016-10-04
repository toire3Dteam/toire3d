#pragma once

class sceneLoading : public BaseScene
{
private:
	BaseScene*	m_newScene;

	//------------------------------------------------------
	//	ロードの炎
	//------------------------------------------------------
	int			m_AnimeCount;
	int			m_AnimeFrame;
	tdn2DObj	*m_pImage, *m_pGauge;
	float		m_fLoadGaugePercent;		// 描画用のpercentage(★補間で使う)

public:

	//------------------------------------------------------
	//	スレッド関係
	//------------------------------------------------------
	static void Thread_funk(void *arg);
	static bool isThread;


	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	bool Initialize(BaseScene *newScene);
	~sceneLoading();


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	void Update();
	void Render();

};
