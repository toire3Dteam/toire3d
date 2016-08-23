#pragma once
//*****************************************************************************************************************************
//
//	パーティクルエフェクト
//
//*****************************************************************************************************************************

// パーティクル用構造体
typedef struct tagPARTICLE
{
public:
	int		type;			//	形

	int		aFrame;			//	出現フレーム
	COLOR	aColor;			//	出現カラー

	int		eFrame;			//	消滅フレーム
	COLOR	eColor;			//	出現カラー

	int		mFrame;			//	最高フレーム
	COLOR	mColor;			//	最高カラー

	Vector3		 pos;
	Vector3		 move;
	Vector3		 power;
	float		rotate;
	float		stretch;

	float	angle;
	float	scale;

	u8		flag;
} PARTICLE, *LPPARTICLE;


class ParticleData
{
protected:
	u8		bFlags;
	float	r, g, b;
	PARTICLE	pdata;
	int		CurFrame;
	float	CurAlpha;

public:

	inline bool isActive(){ return (bFlags != 0); }

	void Set(LPPARTICLE pd);
	void SetFlag(u8 flag){ bFlags = flag; }
	//	更新
	void Update();
	//	描画
	inline bool SetVertex(TLVERTEX* v);
	void Render();
	void Render(tdnShader* shader, char* name);
};

class ParticleManager
{
public:
	static Matrix CurViewProjection;

	static void Initialize(char* filename, int NumParticles);
	static void Release();
	static void Update();
	static void Render();
	static void Render(tdnShader* shader, char* name);

	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_
	// ここに各自パーティクル
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_
	static void EffectPersonaTrigger(const Vector3 &pos);	// ペルソナ発動した瞬間
	static void EffectPersonaAura(const Vector3 &pos);		// ペルソナ持続オーラ
	static void EffectFinish(const Vector3 &pos);		// フィニッシュアーツヒット
	static void EffectFlySmoke(const Vector3 &pos);		// 吹っ飛び煙
	static void EffectHit(const Vector3 &pos);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_

private:
	static int m_NumParticles;				//	パーティクル最大数
	static ParticleData* m_pParticleDatas;	//	パーティクルデータ
	static tdn2DObj* m_pTexture;				//	テクスチャ


	// パーティクルセット
	static void Set(LPPARTICLE pd);
	static void	Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float rotate, float angle, float stretch, float scale, u8 flag);
	static void	Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotete, float stretch, float scale, u8 flag);
	static void	Set2(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotete, float angle, float stretch, float scale, u8 flag);

	static void Reset();
};


/*輝くエフェクト*/
void ShiningEffect(Vector3 _pos, float _scale = 1.0f);
/*バースト打てますEF*/
void AuraMAXEffect(Vector3 _pos, float _scale = 1.0f, Vector3 _col = Vector3(0.3f, 0.7f, 1.0f));

void Sand_effect(float x, float y, float z);
void Rend_effect(const Vector3 &pos, float scale, int num = 50);
void Rend_effect2(const Vector3 &pos, float scale, int num = 30);

//火花エフェクト
enum FIRE_COLOR
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	PURPLE,
	RANDOM
};
void EffectFireFlour(Vector3 p, FIRE_COLOR color = RED, float scale = 1.0f);

//ボムエフェクト
void	EffectBomb(Vector3 pos, float scale);


//↓からクラス

//爆発エフェクト
class SimpleExplosion
{
private:

	iexMesh* chip_obj;

	int timer;
	Vector3 pos;
	float scale;
	static const int CHIP_MAX = 10;

	struct CHIP_STATUS
	{
		Vector3 pos;
		Vector3 move;
		Vector3 angle;
		Vector3 rotate;

		float scale;
	};

	CHIP_STATUS chip[CHIP_MAX];

	bool isLive;
public:

	SimpleExplosion();
	~SimpleExplosion();

	void Init(Vector3 pos, float Scale);
	int Update();
	void Render();

	void EffectBomb();
	void EffectChup(Vector3 p);

	bool GetLive(){ return isLive; }
};

class BombScar
{
private:
protected:

	iexMesh* obj;

	struct SCAR_STATUS{
		bool isLive;
		Vector3 pos;
		float scale;
		float angle;

		int timer;

	};
	static const int SCAR_MAX = 8;
	SCAR_STATUS scar[SCAR_MAX];

public:


	BombScar();
	~BombScar();

	virtual void Init();
	virtual void Update();
	virtual void Render();

	virtual void Set(Vector3 pos, float scale = 0.5f, float angle = -1.0f);

	void chipEffect();
};


