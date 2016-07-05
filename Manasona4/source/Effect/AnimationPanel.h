#pragma once
#include "tdnlib.h"

class AnimationPanel
{
public:
	AnimationPanel(char* _fileName, int _sizeX, int _sizeY, int _endCount, int _upflame, int _upCount, bool loop = false, int loopFlame = 180);
	~AnimationPanel();


	void Update();
	void Render(int x, int y, DWORD dwFlag = RS::COPY);

	void Action();
	void Stop();

	bool GetisAction();

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
	int  m_loopMaxFlame;
	int  m_loopFlame;
};
