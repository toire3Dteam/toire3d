#pragma once
#include "TDNLIB.h"
#include "../Number/Number.h"

class ComboUI
{
public:
	ComboUI();
	ComboUI(int* recoveryFrame);
	~ComboUI();

	void Update();
	void Render(int x, int y);

	void GageUpdate();

	void SetRecoveryFrame(int recovery){ m_iRecoveryFrame = recovery; }

	// ƒJƒEƒ“ƒg
	void Count(int damage,int maxRecovery);
	void Count(int damage, int maxRecovery,int* recovery);
	void Guard();

private:
	Number* m_num;
	Number* m_damageNum;

	tdn2DObj* m_frontPic;
	tdn2DObj* m_backPic;

	int m_iCount;
	int m_iMaxCount;

	int m_iDamage;
	int m_iMaxDamage;

	tdn2DObj* m_gageFramePic;
	tdn2DObj* m_gageBackPic;
	tdn2DObj* m_gagePic;

	int* m_pRecoveryFrame;
	int m_iRecoveryFrame;
	int m_iMaxRecoveryFrame;

	bool m_bRenderFlag;
	int m_iRenderFlagWaitFrame;

	bool m_bGuardFlag;
};
