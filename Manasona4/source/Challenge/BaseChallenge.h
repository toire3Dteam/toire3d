#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "MenuUI\TipsCard.h"

//+-------------------------------------
// ベースチャレンジ
//+-------------------------------------

class BaseChallenge
{
public:
	BaseChallenge(int iTitleNo);
	virtual ~BaseChallenge();

	virtual void Init(int iDeviceID = 0) = 0;// 最初に毎回初期化
	virtual void Update();
	virtual void Render();

	// ここは共通なのでVirtureはいらない
	void ActionIntroTips();
	void StopIntroTips();
	void ActionClearTips();
	void StopClearTips();

	void ActionClear();// クリア！を出す

					   // これで最後のTipsをだすタイミングを取得
	bool EndTipsStart() { return (m_iWaitFrame <= 0) ? true : false; }

	//
	bool isClear() { return m_bClearFlag; }

	TipsCard* GetIntroTipsCard() { return m_pIntroTips; }
	TipsCard* GetClearTipsCard() { return m_pClearTips; }

	// それぞれのやること
	virtual void TaskUpdate(BasePlayer* pPerson) = 0; //★一つずつbClearを点灯させていく
													  //virtual void TaskReset() = 0; // ミスった時に発動

	void AddTaskData(LPSTR string);

	// 成功！
	void TaskSuccess(int select);

	void titleTaskClear() { m_pTaskTitle.bClear = true; };

	// タイトル名
	std::string GetTitleString() { return m_pTaskTitle.pString; }

protected:
	int m_iWaitFrame;// クリアからどれくらいでエンドTipsへ移行するか
	int m_iWaitFrameMAX;// 最大ウェイト数
	bool m_bClearFlag;	//	

	int m_iDeviceID;   // 操作しているデバイス

					   // 最初のTips・最後のTips 
	TipsCard* m_pIntroTips;
	TipsCard* m_pClearTips;

	// タスクデータ
	struct Task
	{
		std::string pString;	// 文字
		bool bClear;	// クリアしたか
		int iStingLength;	//　文字数 
	};
	std::vector<Task> m_aTask;

	// タスクタイトル名
	Task m_pTaskTitle;// タイトルの画像

	tdn2DObj* m_pTaskIcon;
	Vector2 m_vTaskPos;

	int m_iSelectTask;

	bool m_bAllTaskClear;

	int m_iTitleNo;//

	tdn2DAnim* m_pClearPic;
	tdn2DAnim* m_pClearPicRip;
};
