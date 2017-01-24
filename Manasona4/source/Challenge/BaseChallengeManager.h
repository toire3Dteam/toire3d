#pragma once
#include "BaseChallenge.h"
#include "Data\SelectData.h"

//+------------------------------
// ベースチャレンジマネージャー
//+------------------------------

class BaseChallengeManager
{
public:
	
	BaseChallengeManager();
	virtual ~BaseChallengeManager();

	// 初期化
	void Init(int iSelectType,int iDeviceID);
	void Init(int iDeviceID);
	void Update();
	void TaskUpdate(BasePlayer* pPerson);
	void Render();

	// アクセサ　
	BaseChallenge* GetChallenge() { return m_pChallenge[m_iSelectType]; };
	int GetArrayEnd() { return m_iArrayEnd; }

	// 選択しているチュートリアルを設定する
	int GetSelectType() { return m_iSelectType; }
	void SetSelectType(int iSelectType) { m_iSelectType = iSelectType; }

	void NectStep();
	void BackStep();

	// タイトル名
	std::string GetTitleString();
	std::string GetTitleString(int iNo);

protected:
	// それぞれのチャレンジデータを入れる
	// (1/22) ここに各キャラのチャレンジデータを入れていく
	// 製品のチャレンジモードが全員同じ数なのは配列の関係もあるかも
	BaseChallenge* m_pChallenge[(int)CHALLENGE_TYPE::ARRAY_END];
	
	int m_iSelectType;
	int m_iArrayEnd;// ★配列の終り

};

//+-------------------------------
// アイル―チャレンジマネージャー
//+-------------------------------

class AirouChallengeManager :public BaseChallengeManager
{
public:
	AirouChallengeManager();
	~AirouChallengeManager() {};

private:

};

//+-------------------------------
// テキチャレンジマネージャー
//+-------------------------------

class TekiChallengeManager :public BaseChallengeManager
{
public:
	TekiChallengeManager();
	~TekiChallengeManager() {};

private:

};

//+-------------------------------
// (∵)チャレンジマネージャー
//+-------------------------------

class NazenarabaChallengeManager :public BaseChallengeManager
{
public:
	NazenarabaChallengeManager();
	~NazenarabaChallengeManager() {};

private:

};

//+-------------------------------
// アラミタマチャレンジマネージャー
//+-------------------------------

class AramitamaChallengeManager :public BaseChallengeManager
{
public:
	AramitamaChallengeManager();
	~AramitamaChallengeManager() {};

private:

};
