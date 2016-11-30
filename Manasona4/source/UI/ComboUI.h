#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "../Number/Number.h"

class ComboUI
{
public:
	//ComboUI();
	ComboUI(BasePlayer* PlayerData);
	~ComboUI();

	void Update();
	void Render();

	void GageUpdate();

	void SetRecoveryFrame(int recovery){ m_iRecoveryFrame = recovery; }

	// カウント
	void Count(int damage, int maxRecovery, bool bCounterHit = false);
	//void Count(int damage, int maxRecovery,int* recovery);
	void Guard();

	// 最大ダメージ
	int GetMaxDamage() { return m_iMaxDamage; }

private:
	SIDE m_eSide;
	Vector2 m_vPos;
	BasePlayer* m_pPlayerData;

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

	int m_iRecoveryFrame;
	int m_iMaxRecoveryFrame;

	bool m_bRenderFlag;
	int m_iRenderFlagWaitFrame;

	bool m_bGuardFlag;

	// カウンター
	struct Counter
	{
		tdn2DAnim* pPic;
		int iAddX;
	};
	Counter m_tagCounter;
};
