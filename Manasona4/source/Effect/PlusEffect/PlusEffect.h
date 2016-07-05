#pragma once
#include "../BaseEffect/BaseEffect.h"


/*ベースエフェクト*/

class PlusEffect :public BaseEffect
{
public:
	PlusEffect() 
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/plusEffect.png", 256, 256, 15, 1, 4, false);
	};
	~PlusEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};

class HitEffect :public BaseEffect
{
public:
	HitEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/eraseEffect.png", 256, 256, 15, 1, 4, false);
	};
	~HitEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};


class PutEffect :public BaseEffect
{
public:
	PutEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/nodeEffect.png", 256, 256, 15, 1, 4, false);
	};
	~PutEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};
