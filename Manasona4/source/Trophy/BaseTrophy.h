#pragma once
#include "TDNLIB.h"

//+------------------------------
//	基本のトロフィー
//+------------------------------

struct TROPHY_DATA
{
	tdn2DObj* pIcon;	// トロフィーのアイコン
	std::string sTitle;	// トロフィーのタイトル
	std::string sText;	// トロフィーの文字
	Vector2 vPos;		// 場所
	float iAlpha;		// 透明度

	TROPHY_DATA();
};

class BaseTrophy
{
public:
	BaseTrophy();
	virtual ~BaseTrophy();

	virtual void Update();
	virtual void Render();
	virtual void Action(int iDelayFrame = 0);
	virtual void Stop();

	void RenderRoom(int iX, int iY/*, bool bRock*/);		//	閲覧用
	void RenderInfo(int iX, int iY);					// トロフィーのタイトルと説明
	
	// アクセサ
	TROPHY_DATA GetTrophy() { return m_tagTrophy; }// トロフィーの構造体

	// 閲覧用のポジション
	Vector2 GetRoomPos() { return m_vRoomPos; };
	void SetRoomPos(Vector2 vPos) { m_vRoomPos = vPos; }

	bool isHide() { return m_bHide; }

protected:
	bool m_bAction;
	int m_iActiveFrame;		//	動いてる時間

	TROPHY_DATA m_tagTrophy;
	
	tdn2DObj* m_pPlate;
	tdn2DAnim* m_pIconRip;// トロフィーのアイコンの波紋
	int m_iDelayFrame;
	Vector2 m_vRoomPos;		//	閲覧用の場所
	
	// ロック関連
	// bool m_bRock;
	// tdn2DObj* m_pRockIcon;

	bool m_bHide;			// 隠しトロフィー

};

//+---------------------------------
//	初プレイのトロフィー
//+---------------------------------
class FirstTrophy :public BaseTrophy
{
public:
	FirstTrophy();
	~FirstTrophy() {};

private:

};


//+---------------------------------
//	初めて対戦をしたのトロフィー
//+---------------------------------
class FirstBattleTrophy :public BaseTrophy
{
public:
	FirstBattleTrophy();
	~FirstBattleTrophy() {};

private:

};


//+---------------------------------
//	大ダメージトロフィー
//+---------------------------------
class BigDamageTrophy :public BaseTrophy
{
public:
	BigDamageTrophy();
	~BigDamageTrophy() {};

private:

};

//+---------------------------------
//	MAXHP勝利トロフィー
//+---------------------------------
class MaxHpFinishTrophy :public BaseTrophy
{
public:
	MaxHpFinishTrophy();
	~MaxHpFinishTrophy() {};

private:

};


//+---------------------------------
//	最速勝利トロフィー
//+---------------------------------
class SpeedFinishTrophy :public BaseTrophy
{
public:
	SpeedFinishTrophy();
	~SpeedFinishTrophy() {};

private:

};

//+---------------------------------
//	トレーニングでの連続時間トロフィー
//+---------------------------------
class TrainingTimeTrophy :public BaseTrophy
{
public:
	TrainingTimeTrophy();
	~TrainingTimeTrophy() {};

private:

};

//+---------------------------------
//	最高難易度でCOMを倒したトロフィー
//+---------------------------------
class PowerfulEnemyWinTrophy :public BaseTrophy
{
public:
	PowerfulEnemyWinTrophy();
	~PowerfulEnemyWinTrophy() {};

private:

};

//+--------------------------------------------
//	SSランクで倒したトロフィー
//+--------------------------------------------
class SSRankWinTrophy :public BaseTrophy
{
public:
	SSRankWinTrophy();
	~SSRankWinTrophy() {};

private:

};

//+--------------------------------------------
//	最高難易度でCOMを倒した＋SSランクで倒したトロフィー
//+--------------------------------------------
class PowerfulEnemySSRankWinTrophy :public BaseTrophy
{
public:
	PowerfulEnemySSRankWinTrophy();
	~PowerfulEnemySSRankWinTrophy() {};

private:

};

//+-----------------------------------------
//	コンテンツをたくさん解放したトロフィー
//+-----------------------------------------
class BuyManyContentTrophy :public BaseTrophy
{
public:
	BuyManyContentTrophy();
	~BuyManyContentTrophy() {};

private:

};

//+---------------------------------
//	チャレンジモードでタスクをクリア
//+---------------------------------
class ChallengeTrophy :public BaseTrophy
{
public:
	ChallengeTrophy();
	~ChallengeTrophy() {};

private:

};

//+---------------------------------
//	アイル―チャレンジトロフィー
//+---------------------------------
class AirouChallengeTrophy :public BaseTrophy
{
public:
	AirouChallengeTrophy();
	~AirouChallengeTrophy() {};

private:

};

//+---------------------------------
//	テキチャレンジトロフィー
//+---------------------------------
class TekiChallengeTrophy :public BaseTrophy
{
public:
	TekiChallengeTrophy();
	~TekiChallengeTrophy() {};

private:

};
//+---------------------------------
//	大大ダメージトロフィー
//+---------------------------------
class BigBigDamageTrophy :public BaseTrophy
{
public:
	BigBigDamageTrophy();
	~BigBigDamageTrophy() {};

private:

};




//+---------------------------------
//	コンプリートトロフィー
//+---------------------------------
class CompleteTrophy :public BaseTrophy
{
public:
	CompleteTrophy();
	~CompleteTrophy() {};

private:

};
