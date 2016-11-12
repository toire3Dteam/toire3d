#pragma once
#include "MenuIcon.h"

//+--------------------------
//	メニューUIマネージャー
//+--------------------------
 
// アイコンの種類
enum class MENU_ICON_TYPE
{
	TUTORIAL,
	BATTLE,
	TRAINING,
	COLLECT,
	OPTION,
	ARRAY_END
};



class MenuUIManager
{
public:
	MenuUIManager();
	~MenuUIManager();

	void Update();
	void Move();
	void Render();
	void Action();

	void ActionFall();	// 下るアクション[選択肢2へ]
	void ActionUp();	// 登るアクション[選択肢1へ]

	void InfoStop();	// 説明文を全て消す
	void InfoAction();	// 説明文を表示

	// セレクトナンバー変更
	//void AddSelectNo(int add) { m_iSelectNo += add; }
	//void SetSelectNo(int set) { m_iSelectNo = set; }
	void ChangeSelectNo(int setSelectNo);

	// アクセサ
	int GetMaxIcon() { return (int)MENU_ICON_TYPE::ARRAY_END; }

	MENU_ICON_TYPE GetIconType()
	{
		return (MENU_ICON_TYPE)m_iSelectNo;
	}

private:
	
	BaseMenuIcon* m_pIcon[(int)MENU_ICON_TYPE::ARRAY_END];

	int m_iSelectNo;	// セレクトナンバー

	float m_fIconWidth;		// アイコンの横幅
	float m_fIconHeight;	// アイコンの縦幅

	// 線
	tdn2DObj* m_pblueLine;
	int m_iBlueLineY;
	int m_iNextBlueLineY;

	tdn2DAnim* m_pBlackLine;

	// 説明枠
	tdn2DAnim* m_pInfo;
	int m_iInfoY;

	// 仮
	tdn2DObj* pExp;

	// 動画
	tdn2DObj* m_pMove;
	tdnMovie*  m_pMoveMgr;

	
};
