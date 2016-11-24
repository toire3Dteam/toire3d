#pragma once
#include "TDNLIB.h"
#include "Data\SelectData.h"

//+----------------------
//	ÉRÉCÉì
//+----------------------

class CoinUI
{
public:
	CoinUI(Vector2 vPos);
	~CoinUI();

	void Update();
	void Render();

	void Action();
	void Stop();

private:
	tdn2DAnim* m_pPlate;
	tdn2DAnim* m_pNumber;

	// ÇtÇhÇÃèÍèä
	Vector2 m_vPos;
	Vector2 m_vNumPos;

	// ï`âÊÇ∑ÇÈÇ©
	bool m_bRender;

};
