#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "MenuUI\TipsCard.h"

//+-----------------
//	チュートリアル
//+-----------------

// [メモ]
// 説明文・クリア条件・クリア後の説明（ウェイト付き）　
// AIの設定　自分の状態

// 予定
/*
	1	歩き
	2	しゃがみ
	3	ジャンプ
	4	攻撃
	5	ドライブ攻撃
	6	必殺技
	7	ラッシュ攻撃　勝手に↑の三つをつなげてくれます
	8	ガード　最後にしゃがみとたちがあることを説明
	9	テクニカル(崩し)攻撃（中段）
	10	テクニカル(崩し)攻撃（下段）
	11	投げ
	12	ダッシュ・バックステップ
	13	空中ダッシュ・空中バックステップ
	14	回避
	15	
	16	ペルソナ召喚
	17	覚醒
	18	一撃必殺
	19	
	20	

*/


// [メモ]
// 説明文・クリア条件・クリア後の説明（ウェイト付き）　
// AIの設定　自分の状態


// ←↑の文字はまたTutoのステートでビョウガしたｈぽうがいいな、理由ふくざつだから　ぽりもでできｒのものではない」

//+-----------------------------
//	ベース
//+-----------------------------
class BaseTutorial
{
public:
	BaseTutorial();
	virtual ~BaseTutorial();

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

protected:
	int m_iWaitFrame;// クリアからどれくらいでエンドTipsへ移行するか
	int m_iWaitFrameMAX;// 最大ウェイト数
	bool m_bClearFlag;	//	

	int m_iDeviceID;   // 操作しているデバイス

	// 最初のTips・最後のTips TODO羊のTipsみたいなペッちゃんこになる演出
	TipsCard* m_pIntroTips;
	TipsCard* m_pClearTips;

	// タスクデータ
	struct Task
	{
		LPSTR pSting;	// 文字
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

	tdn2DAnim* m_pClearPic;
	tdn2DAnim* m_pClearPicRip;
};

//+-----------------------------
//	歩く説明
//+-----------------------------
class  WalkTutorial :public BaseTutorial
{
public:
	WalkTutorial();
	~WalkTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:
	int m_iWalkFrame;
	int m_iBackFrame;

};


//+-----------------------------
//	しゃがみの説明
//+-----------------------------
class  SquatTutorial :public BaseTutorial
{
public:
	SquatTutorial();
	~SquatTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:
	int m_iSquatFrame;

};

//+-----------------------------
//	ジャンプの説明
//+-----------------------------
class  JumpTutorial :public BaseTutorial
{
public:
	JumpTutorial();
	~JumpTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};


//+-----------------------------
//	攻撃の説明
//+-----------------------------
class  AttackTutorial :public BaseTutorial
{
public:
	AttackTutorial();
	~AttackTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	スキル攻撃の説明
//+-----------------------------
class  SkillTutorial :public BaseTutorial
{
public:
	SkillTutorial();
	~SkillTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};


//+-----------------------------
//	必殺技の説明
//+-----------------------------
class  OverDriveTutorial :public BaseTutorial
{
public:
	OverDriveTutorial();
	~OverDriveTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};

//+-----------------------------
//	ラッシュ攻撃の説明
//+-----------------------------
class  RushTutorial :public BaseTutorial
{
public:
	RushTutorial();
	~RushTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

};


//+-----------------------------
//	立ちガードの説明
//+-----------------------------
class  StandGuardTutorial :public BaseTutorial
{
public:
	StandGuardTutorial();
	~StandGuardTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

	bool m_bStop;
	int m_iCount;
};

//+-----------------------------
//	しゃがみガードの説明
//+-----------------------------
class  SquatGuardTutorial :public BaseTutorial
{
public:
	SquatGuardTutorial();
	~SquatGuardTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:

	bool m_bStop;
	int m_iCount;
};

//+-----------------------------
//	回避の説明
//+-----------------------------
class  EscapeTutorial :public BaseTutorial
{
public:
	EscapeTutorial();
	~EscapeTutorial() {};

	void Init(int iDeviceID = 0);

	// それぞれのタスクをここで作る
	void TaskUpdate(BasePlayer* pPerson); //★一つずつbClearを点灯させていく

private:


};
