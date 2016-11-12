#pragma once

class sceneTitle : public BaseScene
{
public:
	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	bool Initialize();
	~sceneTitle();


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	void Update();
	void Render();

private:

	enum IMAGE
	{
		//LOGO,	// タイトルロゴ
		BACK,	// 背景
		MAX
	};

	tdn2DObj *m_pImages[IMAGE::MAX];	// 画像
	tdnMovie *m_pMovie;			// 動画	
};
