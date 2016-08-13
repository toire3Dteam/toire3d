#pragma once
#include "TDNLIB.h"
#include "../AnimationUV.h"

/************************/
//	UV�G�t�F�N�g�x�[�X
/************************/

class BaseUVEffect
{
public:
	BaseUVEffect();
	~BaseUVEffect();

	virtual void Update();
	virtual void Render();
	virtual void RenderADD();

    virtual void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f,
        Vector3 startAngle = VECTOR_ZERO, Vector3 endAngle = VECTOR_ZERO);

	virtual void Stop();

	AnimationUV* GetUV() { return m_pic; }

protected:
	AnimationUV* m_pic;
	Vector3 m_pos;
	//float m_startScale;
	//float m_endScale;

};

/**********************************************/
// ������傢�@��̃N���X�Ɉ�G�t�F�N�g����ďd�˂悤����
// 
// 

/**********************************************/
//	�p����G�t�F�N�g
/**********************************************/

/*****************/
//	�g��E�F�[�u
/*****************/
class WaveEffect :public BaseUVEffect
{
public:
	WaveEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/wave.imo", 0.0f, 0.01f, 18, false, 1, 12);
	};
	~WaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	�_���[�W�Ռ��g�G�t�F�N�g�@
/*****************/
class HitImpactEffect :public BaseUVEffect
{
public:
	HitImpactEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/hitImpact.imo", -0.01f, 0.0f, 8, false, 2, 4);
	};
	~HitImpactEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	�y���\�i�G�t�F�N�g�@
/*****************/
class PersonaEffect :public BaseUVEffect
{
public:
	PersonaEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Persona.imo", 0.01f, 0.01f, 18, false, 4, 8);
	};
	~PersonaEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	�C���p�N�g�G�t�F�N�g�@
/*****************/
class ImpactEffect :public BaseUVEffect
{
public:
	ImpactEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/impact.IMO", 0.005f, -0.05f, 8, false, 1, 7);
	};
	~ImpactEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	�V���b�N�E�F�[�u�G�t�F�N�g�@
/*****************/
class ShockWaveEffect :public BaseUVEffect
{
public:
	ShockWaveEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/shockWave.IMO", 0.01f, -0.0075f, 12, false, 1, 6);
	};
	~ShockWaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	�A�b�p�[�G�t�F�N�g
/*****************/
class UpperEffect :public BaseUVEffect
{
public:
	UpperEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/UpperEffect.IMO", 0.0f, 0.03f, 16, false, 1, 12);
	};
	~UpperEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};
