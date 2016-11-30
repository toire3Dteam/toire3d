#pragma once
#include "TDNLIB.h"

//+-----------------------------------
//	�����p�̃v���[�g
//+-----------------------------------

class InformationPlate
{
public:
	InformationPlate();
	~InformationPlate();

	void Update();
	void Render();

	void Action();
	void Stop();

private:
	enum 
	{
		START_POS_Y = 598
	};

	// �v���[�g�̊G
	tdn2DAnim* m_pPic;

	// �ꏊ
	Vector2 m_vPos;
	float m_fMoveY;

	// �t���O
	bool m_bRender;

};
