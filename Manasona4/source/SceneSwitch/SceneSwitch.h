#pragma once
#include "TDNLIB.h"

class SceneSwitch
{
public:
	SceneSwitch();
	~SceneSwitch();
	void Update();
	void Render();
	
	void Action();
	
private:

	// シーン切り替え用
	static const int PIC_MAX = 4;
	tdn2DObj* m_Pic[PIC_MAX];
	int m_iFrame;
	int m_iCount;

	bool m_bActionFlag;

};


// 前のシーンでの演出
class SceneSwitchPrev
{
public:
	SceneSwitchPrev();
	~SceneSwitchPrev();
	void Update();
	void Render();

	void Action();

	bool IsEnd() { return m_bEndFlag; }
	bool IsAction(){ return m_bActionFlag; }
private:

	// シーン切り替え用
	static const int PIC_MAX = 4;
	tdn2DObj* m_Pic[PIC_MAX];
	int m_iFrame;
	int m_iCount;

	bool m_bActionFlag;

	bool m_bEndFlag;
};
