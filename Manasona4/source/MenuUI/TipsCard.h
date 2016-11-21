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

	// 動き
	enum STEP
	{
		INTRO,	// 始め
		EXECUTE,// 実行中
		END		// 終り
	};

	// 選択肢
	enum SELECT_STATE
	{
		OK,			// OK
		CANCEL,		// キャンセル
		HOLD		// 選択中
	};

	TipsCard(LPSTR string, bool bChoice = false, bool bOK = false);
	~TipsCard();

	void Update(int DeviceID = 0);
	void Render();

	void Action();
	void End();

	STEP GetStep() { return m_eStep; }
	SELECT_STATE GetSelectState() { return m_eSelectState; }

private:
	LPSTR m_pString;
	tdn2DAnim* m_pCardPic;
	tdn2DAnim* m_pChoiceOKPic;
	tdn2DAnim* m_pChoiceCancelPic;
	Vector2 m_vCardPos;

	bool m_bRender;
	int m_iAlpha;
	int m_iWaitTimer;
	bool m_bChoice;	//	選択肢かどうか
	bool m_bOK;		//  OKかどうか

	STEP m_eStep;
	SELECT_STATE m_eSelectState;

};

