#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "HpGage.h"

//+------------------------------
//		SPゲージ
//+------------------------------

class SpGage
{
public:
	SpGage(BasePlayer* pPlayer);
	~SpGage();

	void Update();
	void Render();

	void Action(int delayTimer = 0);

private:	
	// SPゲージ
	tdn2DAnim* m_pGage;	
	tdn2DAnim* m_pGageRip;
	Vector2 m_vPos;
	tdn2DAnim* m_pSpNum;
	Vector2 m_vNumPos;
	float m_fRate;
	int m_iGageWidth;
	int m_iGageHeight;

	tdn2DAnim* m_pGageFlash;
	int m_iAlpha;
	bool m_bAlphaReturnFlag;
	int m_iAlphaPow;

	enum GAGE_LEVEL
	{
		NORMAL = 0,
		HARF = 1,
		MAX = 2,
		OVER_DRIVE = 3
	};
	int m_iGageLevel;

	SIDE m_sSideFlag;

	// プレイヤーを参照用
	BasePlayer* m_pPlayerReferences;

	// 根性値によるSP徐々に回復
	tdn2DAnim* m_pSpGetting;
	tdn2DAnim* m_pSpGettingRip;
	bool m_bSpGetting;			// SPが徐々に増えてくフラグ

	// ゲージ使用不可のアイコン
	bool m_bSpPenalty;		
	tdn2DAnim*		m_pNotOverDriveMark;	// オーバードライブ使用不可マーク


private:

	void SpGettingUpdate();

	void SpPenaltyUpdate();
};