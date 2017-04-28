#include "ExPlosion.h"
#include  "Effect\UVEffect\BaseUVEffect.h"
#include  "Effect\PanelEffect\PanelEffect.h"
#include  "Effect\Particle.h"

//+------------------------------
//	����
//+------------------------------
CExplosion::CExplosion()
{
	m_pExplosion = new BreakImpactEffect();
	m_pFireRing  = new FireRingEffect();
	m_pFireWave  = new WaveEffect();

}

CExplosion::~CExplosion()
{
	SAFE_DELETE(m_pExplosion);
	SAFE_DELETE(m_pFireRing);
	SAFE_DELETE(m_pFireWave);
}

void CExplosion::Update()
{
	// �����G�t�F�N�g�X�V
	m_pExplosion->Update();
	m_pFireRing->Update();
	m_pFireWave->Update();
}

void CExplosion::Render()
{
	// �����G�t�F�N�g�`��
	m_pExplosion->Render();
	m_pFireWave->Render();
	m_pFireRing->Render();

}

void CExplosion::Action(Vector3 vPos)
{
	// �����G�t�F�N�g����
	m_pExplosion->Action(vPos, .5f*0.5f , 1);
	m_pExplosion->Update();
	m_pFireWave->Action(vPos, 2.0f*0.5f, 3.5f*0.5f, Vector3(0, 0, 0), Vector3(0, 0, 0));
	m_pFireWave->Update();
	m_pFireRing->Action(vPos), .75f*0.5f, 1.0f*0.5f, Vector3(0, 0.4f, 0), Vector3(0, 0.4f, 0);
	m_pFireRing->Update();

	
}
