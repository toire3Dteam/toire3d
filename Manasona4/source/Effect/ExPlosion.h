#pragma once
#include "TDNLIB.h"

//+------------------------------
//	����
//+------------------------------
class BaseUVEffect;

class CExplosion
{
public:
	CExplosion();
	~CExplosion();

	void Update();
	void Render();
	void Action(Vector3 vPos);

private:

	BaseUVEffect *m_pExplosion;			// ����UV�G�t�F�N�g
	BaseUVEffect *m_pFireRing;
	BaseUVEffect *m_pFireWave;


};

