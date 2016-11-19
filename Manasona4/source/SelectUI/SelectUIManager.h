#pragma once
#include "TDNLIB.h"
#include "SelectUI.h"

//+-----------------------------------
//	キャラクターセレクトマネージャー
//+-----------------------------------

class SelectUIManager
{
public:
	SelectUIManager();
	~SelectUIManager();

	void  Update();
	void  Render();

	void FirstAction();// 最初の演出

	// 両方選択したか
	bool IsOK()
	{
		if (m_pLeftSide->IsOK() == true &&
			m_pRightSide->IsOK() == true)
		{
			return true;
		}
		else
		{
			return false;
		}

	};

	// ★★★お互いの選択したキャラとパートナーを取得するアクセサ
	int GetSelectCharacter(SIDE side) 
	{
		if (side == SIDE::LEFT)
		{
			return m_pLeftSide->GetSelectCharacter();
		}
		else
		{
			return m_pRightSide->GetSelectCharacter();
		}
	}

	int GetSelectPartner(SIDE side)
	{
		if (side == SIDE::LEFT)
		{
			return m_pLeftSide->GetSelectPartner();
		}
		else
		{
			return m_pRightSide->GetSelectPartner();
		}
	}


private:
	// (TODO) 両サイド分　SelectUI
	SelectUI* m_pLeftSide;
	SelectUI* m_pRightSide;


	// 共通の画像
	// 画像
	//enum PIC_TYPE
	//{
	//	BG,
	//	TITLE,
	//	BLACK_LINE,
	//	INFO_PLATE,
	//	//INFO_BOX_LEFT,
	//	//INFO_BOX_RIGHT,
	//	ARRAY_END,
	//};
	//tdn2DAnim* m_pPic[PIC_TYPE::ARRAY_END];

	// キャラクター分のアイコン
	tdn2DAnim* m_pCharacterIcon[(int)CHARACTER::END];


};
