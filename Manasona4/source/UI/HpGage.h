#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"

//+------------------------------
//		HPゲージ
//+------------------------------

enum class UI_SIDE 
{
	LEFT = 0, RIGHT = 1
};

class HpGage
{
public:
	HpGage(BasePlayer* pPlayer);
	~HpGage();

	void Update();
	void Render();

	void UseMaskScreen();

	void MaskUpdate();
	void MaskScreen();

	void NotMaskRender();// マスクを使わない

	void FirstAction(int waitFrame = 0);	// 最初の一回

	void JudeRemainingHP();

	// HPの割合
	int  GetHPPercentage() 
	{
		// HPのレートを計算
		MyAssert((m_pPlayerReferences->GetMaxHP() != 0), "最大HPが0だと0で割ることに");
		float HpRate = (float)m_iRemainingHP / (float)m_pPlayerReferences->GetMaxHP();
		HpRate *= 100;//　繰り上げる 
		return (int)HpRate;
	};


private:
	tdn2DObj* m_pGage;	
	int m_iGageWidth;
	int m_iGageHeight;
	
	tdn2DObj* m_pGageUsually;	// 普通位の色
	tdn2DObj* m_pGagePinch;		// ピンチの色
	
	tdn2DAnim* m_pGagePinchRip;	// ピンチの色の波紋
	int m_iGagePinchRipFrame;		// ピンチの色の波紋の周期

	tdn2DObj* m_pDamageGage;
	int m_iDamagePoint;
	float m_fDamageRate;

	// ウェーブ
	tdn2DObj* m_pWave;	
	float m_fWaveUV;

	// 
	float m_fRate;

	// HPパラメーター用
	int m_iMaxHP;
	int m_iHP;

	int m_iRemainingHP;		// 一試合の最低HP
	Vector2 m_vPos;

	SIDE m_sSideFlag;

	// プレイヤーを参照するよう
	BasePlayer* m_pPlayerReferences;

	// マスクスクリーン
	tdn2DObj* m_pMaskScreen;

	// マスク適用スクリーン
	tdn2DObj* m_pMaskPic;
	float m_fMaskPicScare;
	int m_iWaitFrame;
	tdn2DObj* m_pUseMaskScreen;

	// 
	bool m_bFirstActionFlag;
	bool m_bUseMask;			// マスク使用フラグ

	//// SPゲージ
	//tdn2DObj* m_pSpGage;
	//Vector2 m_vSpPos;
	//float m_fSpRate;
	//int m_iSPGageWidth;
	//int m_iSPGageHeight;

};
