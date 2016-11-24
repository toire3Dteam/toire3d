#pragma once
#include "TDNLIB.h"
#include "Data\SelectData.h"

//+----------------------
//	�R�C��
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

	// �t�h�̏ꏊ
	Vector2 m_vPos;
	Vector2 m_vNumPos;

	// �`�悷�邩
	bool m_bRender;

};
