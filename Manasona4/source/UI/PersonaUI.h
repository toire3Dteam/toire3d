#pragma once
#include "TDNLIB.h"
//#include "BaseEntity\Entity\BaseGameEntity.h"
#include "Player\BasePlayer.h"
#include "Stand\Stand.h"

//+------------------------------
//		ペルソナ関連
//+------------------------------
class PersonaUI
{
public:
	PersonaUI(BasePlayer* pPlayer, bool bLeft);
	~PersonaUI();

	void Update();
	void Render();

	void Action(int waitFrame);

	// ゲージが増える時・減るときに使ってください
	void UpEffectAction();
	void DownEffectAction();

private:
	tdn2DAnim* m_pIcon;	//　ペルソナのテレビアイコン
	Vector2	m_vPos;

	// ペルソナカード
	struct PersonaCard
	{
		tdn2DAnim* obj;
		tdn2DObj* fullCard;
		tdn2DAnim* fullCardRip;

		Vector2 pos;
		bool fullflag;
	};
	int m_iMaxCards;
	std::vector<PersonaCard> m_sCards;

	tdn2DObj* m_pFullCard;
	tdn2DAnim* m_pFullCardRip;

	// プレイヤーを参照用
	BasePlayer* m_pPlayerReferences;
	bool m_bLeftSide;// 左か右か

};
