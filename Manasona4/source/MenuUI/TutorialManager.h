#pragma once
#include "Tutorial.h"

//+----------------------------
// チュートリアルマネージャー
//+----------------------------

enum class TUTORIAL_TYPE
{
	WALK,
	SQUAT,
	JUMP,
	ATTACK,
	SKILL,
	OVER_DRIVE,
	RUSH,
	STAND_GUARD,
	SQUAT_GUARD,
	DUSH,
	BACK_STEP,
	PARTNER,
	INVINCIBLE,
	THROW,
	ESCAPE,
	DOKKOI,
	DOWN_ATTACK,
	OD_ONEMORE,
	OD_BURST,
	ARRAY_END
};

enum class TUTORIAL_BUSY_TYPE
{
	WALK,
	SQUAT,
	JUMP,
	ATTACK,
	SKILL,
	OVER_DRIVE,
	RUSH,
	STAND_GUARD,
	SQUAT_GUARD,
	DUSH,
	BACK_STEP,
	ARRAY_END
};


// セレクト画面やメインでも使われセーブデータを考慮するなら	
// メッセージなどでは扱いずらいと思いシングルトンでまず作ってみた。
class TutorialManager
{
private:
	// シングルトンの作法
	TutorialManager();
	~TutorialManager();
	TutorialManager(const TutorialManager&);
	TutorialManager &operator=(const TutorialManager&) {}

	// 1つしかない実体
	static TutorialManager *m_pInstance;

public:
	// 実体取得
	static TutorialManager *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new TutorialManager;
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	// 初期化
	void Init(TUTORIAL_TYPE type,int DeviceID);
	void Update();
	void TaskUpdate(BasePlayer* pPerson);
	void Render();

	// 忙しい人のためバージョンも追加
	void InitBusy(TUTORIAL_BUSY_TYPE type, int DeviceID);
	void UpdateBusy();
	void TaskUpdateBusy(BasePlayer* pPerson);
	void RenderBusy();

	// アクセサ　
	BaseTutorial* GetTutorial() { return m_pTutorial[(int)m_eType]; };// [11/4]今の所これしか使わなくていいかも
	bool isBusy() { return m_bBusy; }
	void SetBusy(bool flag) { m_bBusy = flag; }
	BaseTutorial* GetTutorialBusy() { return m_pBusyTutorial[(int)m_eBusyType]; };

private:
	// それぞれのチュートリアル
	BaseTutorial* m_pTutorial[(int)TUTORIAL_TYPE::ARRAY_END];
	TUTORIAL_TYPE m_eType;

	// 忙しい人のため用
	bool m_bBusy;
	BaseTutorial* m_pBusyTutorial[(int)TUTORIAL_BUSY_TYPE::ARRAY_END];
	TUTORIAL_BUSY_TYPE m_eBusyType;

};

// 実体化
#define TutorialMgr (TutorialManager::GetInstance())
