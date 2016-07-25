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

	virtual void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f,float endScale = 1.0f);
	virtual void Stop();

	AnimationUV* GetUV() { return m_pic; }

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