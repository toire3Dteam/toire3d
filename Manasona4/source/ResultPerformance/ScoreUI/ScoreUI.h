#pragma once
#include "TDNLIB.h"
#include "Number\Number.h"
#include "Effect\PanelEffect\PanelEffectManager.h"
#include "Data\ResultData.h"

//+--------------------------
//	スコア表示UI
//+--------------------------

class ScoreUI
{
public:
	ScoreUI(ResultData data);
	~ScoreUI();

	void Update();
	void Render();

	void Action();

	bool GetEnd() { return m_bEnd; };

private:

	// スコアの種類
	enum SCORE_TYPE
	{
		DAMAGE,
		HP,
		TIME,
		ARRAY_END
	};
	// 得点
	struct SCORE
	{
		tdn2DAnim* pFont;	// フォントのイラスト
		Number* pNumber;	// 数値表示のイラスト
		int x, y;			// 場所
		int iAddNumX;		// 数字の場所
		int iPoint;			// 表示する得点
	};
	SCORE m_tagScore[SCORE_TYPE::ARRAY_END];

	// ランク処理
	enum RANK_TYPE
	{
		SS, S, A, B, C
	};
	struct RANK
	{
		tdn2DAnim* pRank;	// イラスト
		tdn2DAnim* pRankRip;	// 波紋用
		BasePanelEffect* pLight;	// 光の演出
		RANK_TYPE iType;			// ランクのタイプ
		int x, y;			// 場所
	};
	RANK m_tagRank;

	// 演出完了フラグ
	bool m_bEnd;
	bool m_bActive;
	int m_iFrame;

private:

	int CalcRankScore();	// ランク計算
	int CalcCoin();	// お金

};

