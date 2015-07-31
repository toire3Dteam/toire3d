#ifndef EFFECT_H_
#define EFFECT_H_

class Effect
{
private:
	iexParticle *particle;


public:

	void Initialize();
	void Release();
	void Update();
	void Render();

	void Effect1(const Vector3 &pos);		// 爆発エフェクト 
	void Effect2(const Vector3 &pos);		// 煙エフェクト
	void Effect3(const Vector3 &pos);		// 燃え上がるエフェクト

};


#endif