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
	virtual ~BaseUVEffect();

	virtual void Update();
	virtual void Render();
	virtual void RenderADD();
	virtual void RenderADDNOZ();
	virtual void RenderGuard();
	virtual void RenderSoul();
	virtual void RenderAreaWall();// 壁

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

	Vector3 GetPos() { return m_pos; };

protected:
	AnimationUV* m_pic;
	Vector3 m_pos;
	//float m_startScale;
	//float m_endScale;

};


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
	void Render() { BaseUVEffect::RenderADDNOZ(); };
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
	void Render() { BaseUVEffect::RenderADDNOZ(); };
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
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	マーヤショット2エフェクト
/*****************************/
class MayaShot2Effect :public BaseUVEffect
{
public:
	MayaShot2Effect()
	{
		m_pic = new AnimationUV("Data/UVeffect/maya/shot2.IMO", 0.08f, 0.0f, 46, false, 1, 40);
	};
	~MayaShot2Effect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
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


/*************************/
//	赤バーストエフェクト
/*************************/
class RedBurstEffect :public BaseUVEffect
{
public:
	RedBurstEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/WillPower/Burst.IMO", 0.0f, 0.0f, 14, false, 1, 8);
	};
	~RedBurstEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	赤い波紋ウェーブ
/*****************/
class RedWaveEffect :public BaseUVEffect
{
public:
	RedWaveEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/WillPower/wave.imo", 0.0f, 0.01f, 18, false, 1, 12);
	};
	~RedWaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/**********************************/
//	根性値オーラ
/**********************************/
class WillPowerAuraEffect :public BaseUVEffect
{
public:
	WillPowerAuraEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/WillPower/WillPowerAura.IMO", 0.0f, 0.015f, 18, true, 1, 12);
	};
	~WillPowerAuraEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/**********************************/
//	エリアウォール
/**********************************/
class AreaWallEffect :public BaseUVEffect
{
public:
	AreaWallEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/AreaWall/AreaWall.IMO", 0.0f, 0.003f, 18, true, 1, 12);
		m_pMaskPic = new tdn2DObj("Data/UVeffect/AreaWall/HexagonMask.png");
		shaderM->SetValue("AreaWallTex", m_pMaskPic);
	};
	~AreaWallEffect() 
	{
		shaderM->SetTextureNULL("AreaWallTex");
		SAFE_DELETE(m_pMaskPic);
	};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderAreaWall(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
	// 壁用のマスク（ヘキサ）
	tdn2DObj* m_pMaskPic;


};


/**************************/
//	サイクロン
/**************************/
class CycloneEffect :public BaseUVEffect
{
public:
	CycloneEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Cyclone/Cyclone.imo", 0.065f, -0.025f, 38, false, 2, 30);
	};
	~CycloneEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/**************************/
//	サイクロンショート
/**************************/
class CycloneShortEffect :public BaseUVEffect
{
public:
	CycloneShortEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Cyclone/Cyclone.imo", 0.070f, -0.035f, 16, false, 2, 4);
	};
	~CycloneShortEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};
/**************************/
//	ゴッドハンド
/**************************/
class GodHandEffect :public BaseUVEffect
{
public:
	GodHandEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/nazenara/godhand.imo", 0.045f, 0.02f, 72, false, 4, 34);
	};
	~GodHandEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************/
//	ドロップエフェクト
/*****************/
class NazenarabaDropEffect :public BaseUVEffect
{
public:
	NazenarabaDropEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/nazenara/nazenarabaDrop.IMO", 0.0f, 0.055f, 12, false, 1, 8);
	};
	~NazenarabaDropEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	
private:
};

/*****************************************/
//	TEKIアッパーライン
/*****************************************/
class TekiUpperLineEffect :public BaseUVEffect
{
public:
	TekiUpperLineEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Teki/tekiUpperLine.IMO", 0.0f, -0.01f, 16, false, 3, 11);
	};
	~TekiUpperLineEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/*****************************************/
//	TEKIカウンター
/*****************************************/
class TekiCounterEffect :public BaseUVEffect
{
public:
	TekiCounterEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Teki/Counter.IMO", 0.0f, -0.01f, 12, false, 1, 10);
	};
	~TekiCounterEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/**************************/
//	ステップエフェクト
/**************************/
class StepEffect :public BaseUVEffect
{
public:
	StepEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Teki/StepEffect.imo", 0.0f, 0.05f, 10, false, 1, 4);
	};
	~StepEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	
private:
};


/*************************/
//	多段ヒットエフェクト
/*************************/
class HitImpactMulEffect :public BaseUVEffect
{
public:
	HitImpactMulEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/HitImpact.IMO", 0.0f, 0.0f, 14, false, 2, 8);
	};
	~HitImpactMulEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*************************/
//	多段物理ヒットリップエフェクト
/*************************/
class HitRipBlowMulEffect :public BaseUVEffect
{
public:
	HitRipBlowMulEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/HitRipBlow.IMO", 0.0f, 0.0f, 12, false, 2, 6);
	};
	~HitRipBlowMulEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*******************************/
//	アラミタマチャージウェーブ
/*******************************/
class AramitamaChargeWaveEffect :public BaseUVEffect
{
public:
	AramitamaChargeWaveEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/ChargeWave.imo", 0.005f, 0.015f, 20, false, 2, 16);
	};
	~AramitamaChargeWaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/**********************************/
//	アラミタマチャージオーラ
/**********************************/
class AramitamaChargeAuraEffect :public BaseUVEffect
{
public:
	AramitamaChargeAuraEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/ChargeAura.IMO", 0.0f, 0.03f, 20, false, 2, 16);
	};
	~AramitamaChargeAuraEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*************************************/
//	アラミタマサークル
/*************************************/
class AramitamaCircleEffect :public BaseUVEffect
{
public:
	AramitamaCircleEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/Circle.imo", 0.0f, 0.0f, 14, false, 2, 8);
	};
	~AramitamaCircleEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************************/
//	アラミタマキャノン
/*****************************************/
class AramitamaCanonEffect :public BaseUVEffect
{
public:
	AramitamaCanonEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/Canon.IMO", 0.0f, 0.15f, 180, false, 2, 175);
	};
	~AramitamaCanonEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/*****************************************/
//	アラミタママズルフラッシュ
/*****************************************/
class AramitamaNozzleFlashEffect :public BaseUVEffect
{
public:
	AramitamaNozzleFlashEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/NozzleFlash.IMO", 0.125f, 0.0f, 180, false, 2, 175);
	};
	~AramitamaNozzleFlashEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};

/*****************************************/
//	アラミタマ魂オーラ
/*****************************************/
class AramitamaSoulAuraEffect :public BaseUVEffect
{
public:
	AramitamaSoulAuraEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/SoulAura.IMO", 0.0022f, -0.0125f, 16, false, 4, 12);
	};
	~AramitamaSoulAuraEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderSoul(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	アラミタマ隕石ショットエフェクト
/*****************************/
class AramitamaShotEffect :public BaseUVEffect
{
public:
	AramitamaShotEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/shot.IMO", 0.08f, 0.0f, 66, false, 2, 60);
	};
	~AramitamaShotEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};

/*****************************/
//	アラミタマ波紋エフェクト
/*****************************/
class AramitamaRippleEffect :public BaseUVEffect
{
public:
	AramitamaRippleEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aramitama/Ripple.IMO", 0.022f, -0.0225f, 90, false, 6, 76);
	};
	~AramitamaRippleEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************************************/
//	サモン
/*****************************************/
class SummonEffect :public BaseUVEffect
{
public:
	SummonEffect();
	~SummonEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};


/*****************************************/
//	サモンウェーブ
/*****************************************/
class SummonWaveEffect :public BaseUVEffect
{
public:
	SummonWaveEffect();
	~SummonWaveEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};


/*****************************************/
//	ペルソナ帰宅
/*****************************************/
class ExitEffect :public BaseUVEffect
{
public:
	ExitEffect();
	~ExitEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADD(); };
	//void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:

};


/*****************************/
//	兄貴のスピアエフェクト
/*****************************/
class SpearEffect :public BaseUVEffect
{
public:
	SpearEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Aniki/Spear.IMO", 0.08f, 0.05f, 10, false, 1, 8);
	};
	~SpearEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };

private:
};

/*****************************/
//	兄貴の必殺の光
/*****************************/
class RayEffect :public BaseUVEffect
{
public:
	RayEffect();
	~RayEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };

private:
};

/*****************************/
//	兄貴の手の光
/*****************************/
class HandLightEffect :public BaseUVEffect
{
public:
	HandLightEffect();
	~HandLightEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };

private:
};

/*****************************/
//	兄貴のキャノン
/*****************************/
class AnikiCanonEffect :public BaseUVEffect
{
public:
	AnikiCanonEffect();
	~AnikiCanonEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };

private:
};


/*****************************/
//	人参ショットエフェクト
/*****************************/
class NinjinShotEffect :public BaseUVEffect
{
public:
	NinjinShotEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Balance/ninjin.IMO", 0.08f, 0.0f, 46, false, 1, 40);
	};
	~NinjinShotEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::Render(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************************/
//	青い衝撃波エフェクト 
/*****************************/
class BlueImpactEffect :public BaseUVEffect
{
public:
	BlueImpactEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Balance/BlueImpact.IMO", 0.0f, 0.0f, 12, false, 0, 8);
	};
	~BlueImpactEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************************/
//	USAショットエフェクト
/*****************************/
class USAShotEffect :public BaseUVEffect
{
public:
	USAShotEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/Balance/USAShot.IMO", 0.0f, 0.0f, 46, false, 1, 40);
	};
	~USAShotEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};


/*****************************/
//	対空エフェクト 
/*****************************/
class AntiAirEffect :public BaseUVEffect
{
public:
	AntiAirEffect()
	{
		m_pic = new AnimationUV("Data/UVeffect/HitEffect/AntiAir.IMO", 0.0f, 0.0f, 10, false, 1, 8);
	};
	~AntiAirEffect() {};

	void Update() { BaseUVEffect::Update(); };
	void Render() { BaseUVEffect::RenderADDNOZ(); };
	void Action(Vector3 pos = VECTOR_ZERO, float startScale = 1.0f, float endScale = 1.0f) { BaseUVEffect::Action(pos, startScale, endScale); };

private:
};
