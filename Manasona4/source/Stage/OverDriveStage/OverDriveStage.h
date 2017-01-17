#pragma once
#include "TDNLIB.h"
#include "Effect\UVEffect\BaseUVEffect.h"


//+------------------------------
//	�I�[�o�[�h���C�u�p�̃X�e�[�W
//+------------------------------

class OverDriveStage
{
public:
	OverDriveStage();
	~OverDriveStage();

	void Update();
	void Render();

	// �A�N�Z�T
	tdn2DObj* GetScreen() { return m_pScreen; }


private:
	tdn2DObj* m_pBG;

	tdn2DObj* m_pScreen;
	Surface* m_pStencilSurface;//Z

	// �X�s�[�h��
	BaseUVEffect* m_pSpeedLine;

	// ���s��
	tdn2DObj* m_pSpeedLine2D;
	float m_fSpeed;
};

