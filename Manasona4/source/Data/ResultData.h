#pragma once
#include "TDNLIB.h"
#include "BaseEntity\Message\Message.h"

//+------------------------
// リザルトデータ
//+------------------------
struct ResultData
{
	SIDE eWinnerSide;		// 勝者のサイド
	int iMaxDamage;			// 最大ダメージ
	int iRemainingHP;		// 残りHP
	int iElapsedTime;		// 経過時間

	// 初期化
	ResultData() :eWinnerSide(SIDE::LEFT), iMaxDamage(0), iRemainingHP(0), iElapsedTime(0) {}

};