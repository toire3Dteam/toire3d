#pragma once
#include "tdnlib.h"

class AnimationPanel
{
public:
	AnimationPanel(char* _fileName, int _sizeX, int _sizeY,
		int _endCount, int _upflame, int _upCount, bool loop = false, int loopCount = 1000);
	~AnimationPanel();


	void Update();
	void Render(int x, int y, DWORD dwFlag = RS::COPY);
	void Render3D(Vector3 pos, DWORD dwFlag = RS::COPY);

	void Action();
	void Stop();

	bool GetisAction();
	bool isEndFlag() { return m_bEndFlag; }

	tdn2DObj* GetPic() { return m_image; };
private:
	tdn2DObj* m_image;

	int m_sizeX;
	int m_sizeY;

	// アニメ用
	int m_flame;
	int m_upflame;
	int m_count;
	int m_upCount;
	int m_endCount;


	// スイッチ
	bool isAction;

	// ループフラグ
	bool m_loop;
	int  m_loopMaxCount;
	//int  m_loopFlame;
	int  m_loopCount;

	// 終了フラグ
	bool m_bEndFlag;
};
