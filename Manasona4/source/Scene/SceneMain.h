#pragma once

class sceneMain	: public BaseScene
{
public:
	//初期化・解放
	bool Initialize();
	~sceneMain();
	//処理
	bool Update();
	//描画
	void Render();
private:
};