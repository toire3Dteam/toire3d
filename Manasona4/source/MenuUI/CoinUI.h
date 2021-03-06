#pragma once
#include "TDNLIB.h"
#include "Data\SelectData.h"

//+----------------------
//	コイン
//+----------------------

class CoinUI
{
public:
	CoinUI(Vector2 vPos, bool bCollect = false);
	~CoinUI();

	void Update();
	void Render();

	void Action();
	void Stop();

private:
	tdn2DAnim* m_pPlate;
	tdn2DAnim* m_pNumber;

	// ＵＩの場所
	Vector2 m_vPos;
	Vector2 m_vNumPos;

	// 描画するか
	bool m_bRender;

};
