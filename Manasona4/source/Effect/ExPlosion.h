#pragma once
#include "TDNLIB.h"

//+------------------------------
//	爆発
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

	BaseUVEffect *m_pExplosion;			// 爆発UVエフェクト
	BaseUVEffect *m_pFireRing;
	BaseUVEffect *m_pFireWave;


};

