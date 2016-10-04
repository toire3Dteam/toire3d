#pragma once
#include "TDNLIB.h"
#include "../AnimationUV.h"

/************************/
//	UVエフェクトベース
/************************/

class BaseUVEffect
{
public:
	BaseUVEffect();
	~BaseUVEffect();

	virtual void Update();
	virtual void Render();
	virtual void RenderADD();
	virtual void RenderGuard();

    virtual void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f,
        Vector3 startAngle = VECTOR_ZERO, Vector3 endAngle = VECTOR_ZERO);

	virtual void Stop();

	// ループ用
	virtual void ActionRoop(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f,
		Vector3 startAngle = VECTOR_ZERO, Vector3 endAngle = VECTOR_ZERO);
	
	virtual void StopRoop();

	AnimationUV* GetUV() { return m_pic; }

	void SetPos(Vector3 pos){ m_pos = pos; }
protected:
	AnimationUV* m_pic;
	Vector3 m_pos;
	//float m_startScale;
	//float m_endScale;

};

/**********************************************/
// わっしょい　一つのクラスに一つエフェクト作って重ねようかな
// 
// 

/**********************************************/
//	継承先エフェクト
/**********************************************/

/*****************/
//	波紋ウェーブ
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
//	ダメージ衝撃波エフェクト　
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
//	ペルソナエフェクト　
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
//	インパクトエフェクト　
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
//	ショックウェーブエフェクト　
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
//	アッパーエフェクト
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

/*****************/
//	ガードエフェクト
/*****************/
class GuardEffect :public BaseUVEffect
{
public:
	GuardEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/GuardEffect.IMO", 0.001f, -0.01f, 16, false, 1, 12);
	};
	~GuardEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderGuard(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	走りエフェクト
/*****************/
class RunEffect :public BaseUVEffect
{
public:
	RunEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/RunEffect/RunEffect.IMO", -0.01f, -0.00f, 8, false, 2, 3);
	};
	~RunEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	集まるエフェクト 1
/*****************/
class ConvEffect :public BaseUVEffect
{
public:
	ConvEffect();
	~ConvEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************/
//	集まるエフェクト 2
/*****************/
class ConvEffect2 :public BaseUVEffect
{
public:
	ConvEffect2();

	~ConvEffect2() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	集まるエフェクト 3
/*****************/
class ConvEffect3 :public BaseUVEffect
{
public:
	ConvEffect3();
	//{
	//	m_pic = new AnimationUV("Data/UVeffect/convEffect/convEffect4.IMO", 0.00f, 0.05f, 30, false, 2, 16);
	//};
	~ConvEffect3() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	集まるエフェクト 4
/*****************/
class ConvEffect4 :public BaseUVEffect
{
public:
	ConvEffect4();
	//{
	//	m_pic = new AnimationUV("Data/UVeffect/convEffect/convEffect4.IMO", 0.00f, 0.05f, 30, false, 2, 16);
	//};
	~ConvEffect4() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	バーストの玉
/*****************/
class BurstBallEffect :public BaseUVEffect
{
public:
	BurstBallEffect();

	~BurstBallEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/***************************/
//	スプラッシュエフェクト
/***************************/
class SplashEffect :public BaseUVEffect
{
public:
	SplashEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Result/Splash.IMO", 0.0005f, 0.07f, 16, false, 1, 12);
	};
	~SplashEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};
