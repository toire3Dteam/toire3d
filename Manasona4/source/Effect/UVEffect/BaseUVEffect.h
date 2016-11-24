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
	void SetScaleAnimation(float startScale, float endScale) { m_pic->ScaleAnimation(startScale, endScale); }
	void SetAngleAnimation(Vector3 startAngle, Vector3 endAngle) { m_pic->AngleAnimation(startAngle, endAngle); }

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
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/penetrateEffect.imo", 0.00f, 0.0f, 8, false, 2, 4);
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

/**************************/
//	ヒットリングエフェクト　
/**************************/
class HitRingEffect :public BaseUVEffect
{
public:
	HitRingEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/HitRing.imo", 0.00f, 0.0f, 16, false, 2, 10);
	};
	~HitRingEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	アイル―ドリルエフェクト
/*****************************/
class AirouDrillEffect :public BaseUVEffect
{
public:
	AirouDrillEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/AirouDrill.IMO", 0.0f, 0.07f, 46, false, 1, 42);
	};
	~AirouDrillEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	[アイル―超必殺技用]メテオエフェクト
/*****************************/
class AirouBoostEffect :public BaseUVEffect
{
public:
	AirouBoostEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/AirouBoost.IMO", 0.08f, 0.0f, 46, false, 1, 40);
	};
	~AirouBoostEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************************/
//	[アイル―超必殺技用]衝撃波エフェクト 
/*****************************/
class AirouImpactEffect :public BaseUVEffect
{
public:
	AirouImpactEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/AirouImpact.IMO", 0.0f, 0.0f, 12, false, 1, 8);
	};
	~AirouImpactEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************************/
//	[超必殺技用]スピードライン青エフェクト 
/*****************************/
class SpeedLineBlueEffect :public BaseUVEffect
{
public:
	SpeedLineBlueEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/OverFlow/SpeedLineBlue.IMO", 0.03f, 0.0f, 10, false, 1, 6);
	};
	~SpeedLineBlueEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	[超必殺技用]スピードライン緑エフェクト 
/*****************************/
class SpeedLineGreenEffect :public BaseUVEffect
{
public:
	SpeedLineGreenEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/OverFlow/SpeedLineGreen.IMO", 0.03f, 0.0f, 10, false, 1, 6);
	};
	~SpeedLineGreenEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************************/
//	★ブレイクインパクト
/*****************************************/
class BreakImpactEffect :public BaseUVEffect
{
public:
	BreakImpactEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/AirouBreakImpact.IMO", 0.0f, 0.0f, 16, false, 3, 11);
	};
	~BreakImpactEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/*****************************************/
//	★ファイアーリング
/*****************************************/
class FireRingEffect :public BaseUVEffect
{
public:
	FireRingEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/FireRing.IMO", 0.0f, 0.0f, 18, false, 3, 12);
	};
	~FireRingEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/*****************************************/
//	サークルウィンド
/*****************************************/
class SircleWindEffect :public BaseUVEffect
{
public:
	SircleWindEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/SircleWind.IMO", 0.0f, 0.0f, 28, false, 4, 24);
	};
	~SircleWindEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/*****************/
//	波紋ウェーブ
/*****************/
class GuardWaveEffect :public BaseUVEffect
{
public:
	GuardWaveEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Guard/GuardWave.imo", 0.0f, 0.01f, 16, false, 1, 10);
	};
	~GuardWaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************/
//	ガードグリッドエフェクト
/*****************/
class GuardGridEffect :public BaseUVEffect
{
public:
	GuardGridEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Guard/GuardGrid.imo", 0.0f, 0.0f, 14, false, 2, 8);
	};
	~GuardGridEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	アイル―サークルグリッドエフェクト
/*****************/
class AirouCircleGridEffect :public BaseUVEffect
{
public:
	AirouCircleGridEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/AirouCircle.imo", 0.0f, 0.0f, 14, false, 2, 8);
	};
	~AirouCircleGridEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	ヒットリップエフェクト
/*****************/
class HitRipGridEffect :public BaseUVEffect
{
public:
	HitRipGridEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/HitRip.IMO", 0.0f, 0.0f, 14, false, 2, 8);
	};
	~HitRipGridEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	ヒットスラッシュエフェクト
/*****************/
class HitSlashGridEffect :public BaseUVEffect
{
public:
	HitSlashGridEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/HitSlash.IMO", 0.0f, 0.0f, 14, false, 2, 8);
	};
	~HitSlashGridEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*************************/
//	アイル―登場エフェクト
/*************************/
class AirouEntryEffect :public BaseUVEffect
{
public:
	AirouEntryEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/darkZone.IMO", 0.0f, 0.0f, 150, false, 24, 140);
	};
	~AirouEntryEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*************************/
//	アイル―登場サークルエフェクト
/*************************/
class AirouEntrySircleEffect :public BaseUVEffect
{
public:
	AirouEntrySircleEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Airou/wave.IMO", 0.0f, 0.01f, 18, true, 1, 12);
	};
	~AirouEntrySircleEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*************************/
//	オーバードライブエフェクト
/*************************/
class OverDriveRingEffect:public BaseUVEffect
{
public:
	OverDriveRingEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/OverDrive/OverDriveRing.IMO", 0.0f, 0.0f, 18, false, 2, 12);
	};
	~OverDriveRingEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/**************************/
//	ジャンプ波紋ウェーブ
/**************************/
class JumpWaveEffect :public BaseUVEffect
{
public:
	JumpWaveEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Jump/wave.imo", 0.0f, 0.015f, 16, false, 1, 10);
	};
	~JumpWaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/**************************/
//	ジャンプスピード線
/**************************/
class JumpSpeedLineEffect :public BaseUVEffect
{
public:
	JumpSpeedLineEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Jump/JumpSpeedLine.imo", 0.0f, 0.03f, 16, false, 1, 10);
	};
	~JumpSpeedLineEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/**************************/
//	プレッシャー
/**************************/
class PressureEffect :public BaseUVEffect
{
public:
	PressureEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Skill/Pressure.imo", 0.0f, 0.05f, 10, false, 1, 4);
	};
	~PressureEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*************************/
//	オレンジバーストエフェクト
/*************************/
class OrangeBurstEffect :public BaseUVEffect
{
public:
	OrangeBurstEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Skill/OrangeBurst.IMO", 0.0f, 0.0f, 12, false, 1, 8);
	};
	~OrangeBurstEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	マーヤショットエフェクト
/*****************************/
class MayaShotEffect :public BaseUVEffect
{
public:
	MayaShotEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/maya/shot.IMO", 0.08f, 0.0f, 46, false, 1, 40);
	};
	~MayaShotEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	マズルフラッシュエフェクト 
/*****************************/
class MuzzleFlashEffect :public BaseUVEffect
{
public:
	MuzzleFlashEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/maya/MuzzleFlash.IMO", 0.0f, 0.0f, 12, false, 1, 3);
	};
	~MuzzleFlashEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};
