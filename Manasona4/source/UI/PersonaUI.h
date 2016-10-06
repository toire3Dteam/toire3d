#pragma once
#include "TDNLIB.h"
//#include "BaseEntity\Entity\BaseGameEntity.h"
#include "Player\BasePlayer.h"
#include "Stand\Stand.h"

//+------------------------------
//		�y���\�i�֘A
//+------------------------------
class PersonaUI
{
public:
	PersonaUI(BasePlayer* pPlayer, bool bLeft);
	~PersonaUI();

	void Update();
	void Render();

	void Action(int waitFrame);

	// �Q�[�W�������鎞�E����Ƃ��Ɏg���Ă�������
	void UpEffectAction();
	void DownEffectAction();

private:
	tdn2DAnim* m_pIcon;	//�@�y���\�i�̃e���r�A�C�R��
	Vector2	m_vPos;

	// �y���\�i�J�[�h
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

	// �v���C���[���Q�Ɨp
	BasePlayer* m_pPlayerReferences;
	bool m_bLeftSide;// �����E��

};
