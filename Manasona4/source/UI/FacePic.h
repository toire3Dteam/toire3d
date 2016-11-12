#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"

//+-----------------------
//		��O��
//+-----------------------

class FacePic
{
public:
	FacePic(BasePlayer* pPlayer);
	~FacePic();

	void Update();
	void Render();
	void Action(int waitFrame);

private:
	
	// ��O���p
	Vector2 m_vPos;

	SIDE m_sSideFlag;
	// �v���C���[���Q�Ɨp
	BasePlayer* m_pPlayerReferences;

	// �p�l���A�j���[�V����
	BasePanelEffect* m_pPanelEF;

};
