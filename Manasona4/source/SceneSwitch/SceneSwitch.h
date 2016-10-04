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

	// ÉVÅ[ÉìêÿÇËë÷Ç¶óp
	static const int PIC_MAX = 4;
	tdn2DObj* m_Pic[PIC_MAX];
	int m_iFrame;
	int m_iCount;

	bool m_bActionFlag;

};
