#pragma once
#include "Tutorial.h"

//+----------------------------
// チュートリアルマネージャー
//+----------------------------

enum TUTORIAL_TYPE
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
	ESCAPE,
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

	// アクセサ　
	BaseTutorial* GetTutorial() { return m_pTutorial[m_eType]; };// [11/4]今の所これしか使わなくていいかも


private:
	// それぞれのチュートリアル
	BaseTutorial* m_pTutorial[TUTORIAL_TYPE::ARRAY_END];

	TUTORIAL_TYPE m_eType;


};

// 実体化
#define TutorialMgr (TutorialManager::GetInstance())
