#pragma once

typedef struct Tag_particle_2d
{
public:
	int		type;			//	形

	int		aFrame;			//	出現フレーム
	COLOR	aColor;			//	出現カラー

	int		eFrame;			//	消滅フレーム
	COLOR	eColor;			//	出現カラー

	int		mFrame;			//	最高フレーム
	COLOR	mColor;			//	最高カラー

	Vector2 pos;
	Vector2 move;
	Vector2 power;
	float		rotate;
	float		stretch;

	float	angle;
	float	scale;

	u8		flag;
} PARTICLE_2d, *LPPARTICLE_2d;



class Particle_2d_data
{
protected:
	u8		bFlags;
	float	r, g, b;
	PARTICLE_2d	pdata;
	int		CurFrame;
	float	CurAlpha;

public:

	inline BOOL bActive(){ return (bFlags != 0); }

	void Set(LPPARTICLE_2d pd);
	void SetFlag(u8 flag){ bFlags = flag; }

	//	更新
	void Update();

	//	描画
	inline bool SetVertex(TLVERTEX* v, int uindex, int vindex);
	void Render(int uindex, int vindex);

	// 移動
	inline void MovePos(const Vector2 &move);
};



class Particle_2d
{
protected:
	int	nParticles;					//	パーティクル最大数
	Particle_2d_data*	ParticleData;	//	パーティクルデータ
	tdn2DObj* lpTexture;				//	テクスチャ

	int uindex, vindex; // テクスチャの分割数 typeで左上から右に数える
public:
	Matrix CurViewProjection;

	Particle_2d() :nParticles(0), ParticleData(nullptr), lpTexture(nullptr), uindex(1), vindex(1){}
	~Particle_2d(){ Release(); }

	//	初期化・解放
	void Initialize(char* filename, int nParticles, int uindex, int vindex);
	void Release();

	//	出現
	void Set(LPPARTICLE_2d pd);
	void	Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor,
		const Vector2 &pos, const Vector2 &Move, const Vector2 &Power, float rotate, float stretch, float scale, u8 flag);
	void	Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha,
		const Vector2 &pos, const Vector2 &Move, const Vector2 &Power,
		float r, float g, float b, float scale, u8 flag);

	//	更新
	void Update();
	void Reset();
	void Reset(int num);

	//	描画
	void Render();

	// 移動
	void MovePos(const Vector2 &move);
};

// ゲーム作る人はこれを使う
class Particle2dManager
{
public:
	static void Initialize(char *filename, int nParticles, int uindex, int vindex);
	static void Release();
	static void Update();
	static void Render();

	// パーティクル関数たち
	static void Effect_KiraKira(const Vector2 &pos, const Vector2 &range, float scale = 20.0f, float ScaleFluctuation = 5.0f, int LoopCount = 1, int EndFrame = 120);	// きらきらエフェクト
	static void Effect_SandCloud(const Vector2 &pos);		// 砂煙
	static void Effect_RealSheep(const Vector2 &pos);		// リアル羊のオーラ
	static void Effect_Hinoko(const Vector2 &pos);			// 火の粉
	static void Effect_FatSmoke(const Vector2 &pos, float scale = 96, int LoopCount = 2);		// デブ煙
	static void Effect_Unlimited(const Vector2 &pos);		// アンリミオーラ
	static void Effect_NikuSmoke(const Vector2 &pos);		// 肉湯気
private:
	static Particle_2d *m_pParticle2d;
};