#pragma once

class sceneLoading : public BaseScene
{
private:
	BaseScene		*m_pNewScene;			// スレッドで読み込む新しいシーン

	//------------------------------------------------------
	//	ロードの情報
	//------------------------------------------------------
	int			m_AnimeCount;
	int			m_AnimeFrame;
	tdn2DObj	*m_pImage, *m_pGauge;
	float		m_fLoadGaugePercent;		// 描画用のpercentage(★補間で使う)
	float		m_fLoadAngle;
public:

	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	sceneLoading(BaseScene *newScene);
	bool Initialize();
	~sceneLoading();


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	void Update();
	void Render();

};
