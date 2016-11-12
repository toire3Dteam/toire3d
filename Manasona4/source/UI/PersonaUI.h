#pragma once
#include "TDNLIB.h"
//#include "BaseEntity\Entity\BaseGameEntity.h"
#include "Player\BasePlayer.h"
#include "Stand\Stand.h"
#include "pie_graph\pie_graph.h"

//+------------------------------
//		ペルソナ関連
//+------------------------------
class PersonaUI
{
public:
	PersonaUI(BasePlayer* pPlayer);
	~PersonaUI();

	void Update();
	void Render();

	void Action(int waitFrame);

	// ゲージが増える時・減るときに使ってください
	void UpEffectAction();
	void DownEffectAction();

private:
	//tdn2DAnim* m_pIcon;	//　ペルソナのテレビアイコン
	Vector2	m_vPos;

	bool m_bTriggerFlag;

	// トリガーアイコン
	tdn2DAnim* m_pTrigger;
	Pie_graph* m_pTriggerGage;
	tdn2DAnim* m_pTriggerGageRip;
	Vector3 m_vTriggerPos;

	// ペルソナカード
	//struct PersonaCard
	//{
	//	tdn2DAnim* obj;
	//	tdn2DObj* fullCard;
	//	tdn2DAnim* fullCardRip;

	//	Vector2 pos;
	//	bool fullflag;
	//};
	//int m_iMaxCards;
	//std::vector<PersonaCard> m_sCards;

	//tdn2DObj* m_pFullCard;
	//tdn2DAnim* m_pFullCardRip;

	// プレイヤーを参照用
	BasePlayer* m_pPlayerReferences;
	bool m_bLeftSide;// 左か右か

	// フラッシュ用
	int m_iFlashRate;
	bool m_bFlashFlag;

};
