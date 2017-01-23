#pragma once
#include "TDNLIB.h"


//+-------------------
//	ベースメニューアイコン
//+-------------------

class BaseMenuIcon
{
public:
	BaseMenuIcon();
	virtual ~BaseMenuIcon();

	virtual void Update();
	virtual void Render(int addY = 0);
	//void RenderGray();
	virtual void Action(int delayFrame = 0);

	// アクセサー
	//Vector2 GetPrevPos() { return m_vPrevPos; }					// 過去の座標
	//void SetPrevPos(Vector2 prevPos) { m_vPrevPos = prevPos; }	// 過去の座標
	//Vector2 GetCurrPos() { return m_vCurrPos; }					// 現在の座標
	//void SetCurrPos(Vector2 currPos) { m_vCurrPos = currPos; }	// 現在の座標
	//void SetAllPos(Vector2 pos) { m_vCurrPos = pos; m_vPrevPos = pos; }
	//void SetMoveRate(float rate) { m_fMoveRate = rate; };
	Vector2 GetPos() { return m_vPos; }
	void SetPos(Vector2 pos) { m_vPos = pos; m_vNextPos = pos; }
	Vector2 GetNextPos() { return m_vNextPos; }
	void SetNextPos(Vector2 pos) { m_vNextPos = pos; }

	void SetSelectFlag(bool flag) { m_bSelectFlag = flag; };
	void SetScale(float scale) { m_fScale= scale; m_fNextScale = scale;};
	void SetNextScale(float scale) { m_fNextScale = scale; };

	tdn2DAnim* GetInfo() { return m_pInfo; }
protected:
	//tdn2DAnim* m_pFont;					// 文字画像
	tdn2DAnim* m_pIcon;					// アイコン画像
	float	   m_fScale;				// 画像スケール
	float	   m_fNextScale;			// 画像の次のスケール

	Vector2 m_vPos;						// 現在の座標
	Vector2 m_vNextPos;					// 次の座標
	//Vector2 m_vPrevPos;					// 過去の座標
	//Vector2 m_vCurrPos;					// 現在の座標
	//float	m_fMoveRate;				// 過去・現在の移動の補間の値
	//float	m_fMovePower;				// 移動力

	bool m_bSelectFlag;					// 選択されたかのフラグ

	// 説明分
	tdn2DAnim* m_pInfo;
	Vector2 m_vInfoPos;
};


//+--------------------------
//	チュートリアルアイコン
//+--------------------------

class TutorialIcon :public BaseMenuIcon
{
public:
	TutorialIcon();
	~TutorialIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	バトルアイコン
//+--------------------------

class BattleIcon :public BaseMenuIcon
{
public:
	BattleIcon();
	~BattleIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	トレーニングアイコン
//+--------------------------

class TrainingIcon :public BaseMenuIcon
{
public:
	TrainingIcon();
	~TrainingIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	チャレンジアイコン
//+--------------------------

class ChallengeIcon :public BaseMenuIcon
{
public:
	ChallengeIcon();
	~ChallengeIcon();

	void Update();
	void Render();

protected:

};

//+--------------------------
//	コレクションアイコン
//+--------------------------
class CollectIcon :public BaseMenuIcon
{
public:
	CollectIcon();
	~CollectIcon();

	void Update();
	void Render();

protected:

};


//+--------------------------
//	オプションアイコン
//+--------------------------
class OptionIcon :public BaseMenuIcon
{
public:
	OptionIcon();
	~OptionIcon();

	void Update();
	void Render();

protected:

};