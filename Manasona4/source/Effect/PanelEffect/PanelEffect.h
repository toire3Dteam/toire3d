#pragma once
#include "TDNLIB.h"
#include "../AnimationPanel.h"


/*ベースエフェクト*/

class BasePanelEffect
{
public:
	//BaseEffect(char* name, int size, int panelNum, int flame, int upCount, bool isRoop = false);
	BasePanelEffect();
	~BasePanelEffect();

	virtual void Update();
	virtual void Render();
	virtual void Render3D();
	virtual void Action(int x = 0, int y = 0, int delayFrame = 0);
	virtual void Action(Vector3 pos = VECTOR_ZERO, int delayFrame = 0);

	virtual void Stop();

	AnimationPanel* GetPanel() { return m_pic; }

protected:
	AnimationPanel* m_pic;
	Vector3 m_pos;
	int m_picSize;
	float m_pic3DScale;
	RS state;

	int m_iDelayFrame;
	bool m_b3D;
};



/**********************************************/
//	継承先エフェクト
/**********************************************/

class PlusEffect :public BasePanelEffect
{
public:
	PlusEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/plusEffect.png", 256, 256, 15, 1, 4, false);
	};
	~PlusEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class HitEffect :public BasePanelEffect
{
public:
	HitEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/eraseEffect.png", 256, 256, 15, 1, 4, false);
		state = RS::ADD_NOZ;
		m_pic3DScale = 80.0f;
	};
	~HitEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};


class PutEffect :public BasePanelEffect
{
public:
	PutEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/nodeEffect.png", 256, 256, 15, 2, 4, false);
	};
	~PutEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};



class DamageEffect :public BasePanelEffect
{
public:
	DamageEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/Damage.png", 256, 256, 15, 1, 4, false);
		state = RS::ADD_NOZ;
	};
	~DamageEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class InEffect :public BasePanelEffect
{
public:
	InEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/ineffect.png", 256, 256, 15, 1, 4, false);
		
		m_pic->GetPic()->SetScale(2.0f);
		
		//m_pic = new AnimationPanel("Data/Effect/in2.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
	};
	~InEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class InEffectMini :public BasePanelEffect
{
public:
	InEffectMini()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/in2.png", 256, 256, 15, 1, 4, false);
		
		m_pic3DScale = 75.0f;
		//state = RS::COPY;
	};
	~InEffectMini() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class DogEffect :public BasePanelEffect
{
public:
	DogEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/DogEffect.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
	};
	~DogEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class BurnEffect :public BasePanelEffect
{
public:
	BurnEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/burn.png", 256, 256, 24, 1, 4, false);
		//state = RS::COPY;
	};
	~BurnEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class ClearEffect :public BasePanelEffect
{
public:
	ClearEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/clear.png", 256, 256, 15, 2, 4, false);
		//state = RS::COPY;
		//m_pic->GetPic()->SetScale(3.0f);// [10/23]スケールもとに戻した
	};
	~ClearEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class DonEffect :public BasePanelEffect
{
public:
	DonEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/don.png", 256, 256, 15, 1, 4, false);
		m_pic->GetPic()->SetScale(3.0f);
		//state = RS::COPY;
	};
	~DonEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class PushEffect :public BasePanelEffect
{
public:
	PushEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/push.png", 256, 256, 14, 1, 4, true, 3);
		//m_pic->GetPic()->SetScale(3.0f);
		state = RS::COPY;
	};
	~PushEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class DeliciousEffect :public BasePanelEffect
{
public:
	DeliciousEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/Delicious.png", 256, 256, 15, 2, 6, true);
		state = RS::COPY;
		//m_pic->GetPic()->SetScale(3.0f);
	};
	~DeliciousEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class EatEffect :public BasePanelEffect
{
public:
	EatEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/Eat.png", 256, 256, 28, 1, 6, false);
		//m_pic->GetPic()->SetScale(3.0f);
		state = RS::COPY;
	};
	~EatEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class PerfectEffect :public BasePanelEffect
{
public:
	PerfectEffect()
	{
		m_picSize = 128;
		//m_pic = new AnimationPanel("Data/Effect/Perfect.png", 128, 128, 59, 1, 8, false);
		m_pic = new AnimationPanel("Data/Effect/Perfect.png", 256, 256, 59, 1, 8, false);
		//m_pic->GetPic()->SetScale(2.0f);
		state = RS::COPY;
	};
	~PerfectEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};


// 
class GreatEffect :public BasePanelEffect
{
public:
	GreatEffect()
	{
		m_picSize = 128;
		//m_pic = new AnimationPanel("Data/Effect/Great.png", 128, 128, 59, 1, 8, false);
		m_pic = new AnimationPanel("Data/Effect/Great.png", 256, 256, 59, 1, 8, false);
		//m_pic->GetPic()->SetScale(2.0f);
		state = RS::COPY;
	};
	~GreatEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class BadEffect :public BasePanelEffect
{
public:
	BadEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/bad.png", 256, 256, 28, 2, 6, false);
		//m_pic->GetPic()->SetScale(3.0f);
		state = RS::COPY;
	};
	~BadEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class HappyEffect :public BasePanelEffect
{
public:
	HappyEffect()
	{
		m_picSize = 256;
		//m_pic = new AnimationPanel("Data/Effect/Great.png", 128, 128, 59, 1, 8, false);
		m_pic = new AnimationPanel("Data/Effect/happy.png", 256, 256, 25, 1, 6, false);
		m_pic->GetPic()->SetScale(2.0f);
		//state = RS::COPY;
	};
	~HappyEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};


class DarkNoticeEffect :public BasePanelEffect
{
public:
	DarkNoticeEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/Darkbikkuri.png", 256, 256, 15, 1, 4, true, 3);
		state = RS::COPY;
	};
	~DarkNoticeEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 煙
class SmokeEffect :public BasePanelEffect
{
public:
	SmokeEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/smoke.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
		m_pic->GetPic()->SetScale(1.5f);
	};
	~SmokeEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class BurstEffect :public BasePanelEffect
{
public:
	BurstEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/BurstEffect.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
		m_pic3DScale = 100;//3Dのスケール
	};
	~BurstEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class OneMoreBurstEffect :public BasePanelEffect
{
public:
	OneMoreBurstEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/OneMoreBurstEffect.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
		m_pic3DScale = 100;//3Dのスケール
	};
	~OneMoreBurstEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class GuardActionEffect :public BasePanelEffect
{
public:
	GuardActionEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/guardAction3.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
		m_pic3DScale = 50;//3Dのスケール
	};
	~GuardActionEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class BurstPrevEffect :public BasePanelEffect
{
public:
	BurstPrevEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/burstPrev.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
		m_pic3DScale = 100;//3Dのスケール
	};
	~BurstPrevEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class GlassEffect :public BasePanelEffect
{
public:
	GlassEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/Glass.png", 256, 256, 15, 2, 4, false);
		//state = RS::COPY;
		m_pic3DScale = 80;//3Dのスケール
	};
	~GlassEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class AirouWindEffect :public BasePanelEffect
{
public:
	AirouWindEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/AirouWind.png", 256, 256, 15, 1, 4, false);
		m_pic3DScale = 500;//3Dのスケール
	};
	~AirouWindEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class WeakEffect :public BasePanelEffect
{
public:
	WeakEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/Weak.png", 256, 256, 59, 1, 8, false);
		state = RS::COPY_NOZ;
		m_pic3DScale = 75;//3Dのスケール
	};
	~WeakEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class OverDriveEffect :public BasePanelEffect
{
public:
	OverDriveEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/OverDrive.png", 256, 256, 15, 2, 4, false);
		state = RS::ADD_NOZ;
		m_pic3DScale = 150;//3Dのスケール
	};
	~OverDriveEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

// 
class OverDriveStartEffect :public BasePanelEffect
{
public:
	OverDriveStartEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/overDriveStart.png", 256, 256, 15, 1, 4, false);
		state = RS::ADD_NOZ;
		m_pic3DScale = 100;//3Dのスケール
	};
	~OverDriveStartEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class OrangeLightEffect :public BasePanelEffect
{
public:
	OrangeLightEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/OrangeLight.png", 256, 256, 15, 1, 4, false);
		state = RS::ADD_NOZ;
		m_pic3DScale = 50;//3Dのスケール
	};
	~OrangeLightEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};


class DokkoiEffect :public BasePanelEffect
{
public:
	DokkoiEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/dokkoi.png", 256, 256, 15, 1, 4, false);
		state = RS::ADD_NOZ;
		m_pic3DScale = 50;//3Dのスケール
	};
	~DokkoiEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class RedLightEffect :public BasePanelEffect
{
public:
	RedLightEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/RedLight.png", 256, 256, 15, 2, 4, false);
		state = RS::ADD_NOZ;
		m_pic3DScale = 50;//3Dのスケール
	};
	~RedLightEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};

class StarEffect :public BasePanelEffect
{
public:
	StarEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/StarEffect.png", 256, 256, 14, 1, 4, false);
		//m_pic->GetPic()->SetScale(3.0f);
		state = RS::ADD;
		m_pic3DScale = 70;//3Dのスケール
	};
	~StarEffect() {};

	void Update() { BasePanelEffect::Update(); };
	void Render() { BasePanelEffect::Render(); };
	//void Action(int x = 0, int y = 0, int delayFrame = 0) { BasePanelEffect::Action(x, y, delayFrame); };

private:
};