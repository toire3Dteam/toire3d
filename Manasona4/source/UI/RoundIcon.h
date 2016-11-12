#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "HpGage.h"

//+------------------------------
//		���E���h�A�C�R��
//+------------------------------

class RoundIcon
{
public:
	RoundIcon(BasePlayer* pPlayer,int iRoundNum );
	~RoundIcon();

	void Update();
	void Render();
	void Action(int waitFrame);

	// Win�A�C�R���o��
	void AppIcon();
		

private:
	// WinIcon
	struct WinIcon
	{
		// �A�C�R��
		tdn2DAnim* pIconTable;
		tdn2DAnim* pWinIcon;
		tdn2DAnim* pWinIconRip;

		Vector2 vPos;
		bool bWinFlag;
	};
	int m_iMaxIcons;
	std::vector<WinIcon> m_sIcons;

	SIDE m_sSideFlag;

	// �v���C���[���Q�Ɨp
	BasePlayer* m_pPlayerReferences;

	// �A�j���[�V�����p
	int m_iAnimTimer;
};

