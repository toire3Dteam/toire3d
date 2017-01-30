#pragma once
#include "TDNLIB.h"
#include "Number\Number.h"
#include "Effect\PanelEffect\PanelEffectManager.h"
#include "Data\ResultData.h"
#include "Data\SelectData.h"

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

	RANK_TYPE GetRankType() { return m_tagRank.eType; }

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
		tdn2DAnim* pFont;		// フォントのイラスト
		Number* pNumber;		// 数値表示のイラスト
		Number* pFlashNumber;	// 点滅用数値表示のイラスト
		int x, y;				// 場所
		int iAddNumX, iAddNumY;	// 数字の場所
		int iPoint;				// 表示する得点
		CloudEffect* pEffect;	// 出現エフェクト
	};
	SCORE m_tagScore[SCORE_TYPE::ARRAY_END];

	// ランク処理
	struct RANK
	{
		tdn2DAnim* pRank;	// イラスト
		tdn2DAnim* pRankRip;	// 波紋用
		BasePanelEffect* pLight;	// 光の演出
		RANK_TYPE eType;			// ランクのタイプ
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

