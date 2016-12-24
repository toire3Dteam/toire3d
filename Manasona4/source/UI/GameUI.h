#pragma once
#include "TDNLIB.h"
// エンティティ関連のインクルード
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"
#include "HpGage.h"
#include "SpGage.h"
#include "PersonaUI.h"
#include "RoundIcon.h"
#include "FacePic.h"
#include "OverDriveCutin\OverDriveCutin.h"
#include "ComboUI.h"
#include "Timer\Timer.h"

class GameUIManager :public BaseGameEntity
{
private:
	// シングルトンの作法
	GameUIManager();
	~GameUIManager();
	GameUIManager(const GameUIManager&);
	GameUIManager &operator=(const GameUIManager&) {}

	// 1つしかない実体
	static GameUIManager *pInstance;

public:
	// 実体取得
	static GameUIManager *GetInstance()
	{
		if (!pInstance) pInstance = new GameUIManager;
		return pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(pInstance);
	}

	// 参照させる
	void InitData(BasePlayer* pLeftPlayer, BasePlayer* pRightPlayer, int iRoundNum, ROUND_TIME_TYPE eRoundTimeType);

	void Update();
	void Render();
	void RenderBack();

	void Action();
	
	// タイマー関連
	void TimerReset();
	void TimerStart();
	void TimerStop();
	bool isTimerUp();

	// BaseGameEntiryサブクラスはメッセージを使って通信する
	bool  HandleMessage(const Message& msg);

	// アクセサー
	ComboUI* GetComboUI(SIDE eSide)
	{
		switch (eSide)
		{
		case SIDE::LEFT:
			return  m_pCombo1P;
			break;
		case SIDE::RIGHT:
			return  m_pCombo2P;
			break;
		default:
			MyAssert(0,"そのサイドはない");
			break;
		}
		return m_pCombo1P;
	}
	
	HpGage* GetHpGage(SIDE eSide)
	{
		switch (eSide)
		{
		case SIDE::LEFT:
			return  m_pHpGage1P;
			break;
		case SIDE::RIGHT:
			return  m_pHpGage2P;
			break;
		default:
			MyAssert(0, "そのサイドはない");
			break;
		}
		return m_pHpGage1P;
	}

	// 時間
	Timer* GetTimer()
	{
		return m_pTimer;
	}

private:

	//tdn2DObj* m_teamPic;

	tdn2DAnim* m_pHpFrame;
	tdn2DAnim* m_pSpFrame;

	// HPゲージ
	HpGage* m_pHpGage1P;
	HpGage* m_pHpGage2P;

	// SPゲージ
	SpGage* m_pSpGage1P;
	SpGage* m_pSpGage2P;

	// ペルソナ用
	PersonaUI* m_pPersona1P;
	PersonaUI* m_pPersona2P;

	// ラウンドアイコン用
	RoundIcon* m_pRoundIcon1P;
	RoundIcon* m_pRoundIcon2P;

	// 顔グラ
	FacePic* m_pFacePic1P;
	FacePic* m_pFacePic2P;

	// オーバードライブのカットイン
	OverDriveCutin* m_pOverDriveCutin1P;
	OverDriveCutin* m_pOverDriveCutin2P;
	
	// UI
	ComboUI*  m_pCombo1P;
	ComboUI*  m_pCombo2P;

	// タイマー
	Timer* m_pTimer;

	// 奥行
	Surface* m_pStencilSurface;

	//struct  HPGage
	//{
	//	tdn2DObj* obj;
	//	float frame;
	//};
	//HPGage	

	//Combo* m_combo;

};

// 実体化
#define GameUIMgr (GameUIManager::GetInstance())