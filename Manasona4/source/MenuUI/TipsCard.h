#pragma once
#include "TDNLIB.h"

//+---------------------
//	ヒントカードクラス
//+---------------------

// 概要
// 色々メッセ―ジとしても使う
// 文字　を　生成するときに書くだけ


class TipsCard
{
public:
	TipsCard(LPSTR string);
	~TipsCard();

	void Update();
	void Render();

	void Action();
	void End();
private:
	LPSTR m_pString;
	tdn2DObj* m_pCardPic;

	bool m_bRender;

};

