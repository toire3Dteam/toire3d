#pragma once
//*****************************************************************************************************************************
//
//	�p�[�e�B�N���G�t�F�N�g
//
//*****************************************************************************************************************************

// �p�[�e�B�N���p�\����
typedef struct tagPARTICLE
{
public:
	int		type;			//	�`

	int		aFrame;			//	�o���t���[��
	COLOR	aColor;			//	�o���J���[

	int		eFrame;			//	���Ńt���[��
	COLOR	eColor;			//	�o���J���[

	int		mFrame;			//	�ō��t���[��
	COLOR	mColor;			//	�ō��J���[

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
	//	�X�V
	void Update();
	//	�`��
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
	// �����Ɋe���p�[�e�B�N��
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_
	static void EffectPersonaTrigger(const Vector3 &pos);	// �y���\�i���������u��
	static void EffectPersonaAura(const Vector3 &pos);		// �y���\�i�����I�[��
	static void EffectFinish(const Vector3 &pos);		// �t�B�j�b�V���A�[�c�q�b�g
	static void EffectFlySmoke(const Vector3 &pos);		// ������щ�
	static void EffectHit(const Vector3 &pos);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_

private:
	static int m_NumParticles;				//	�p�[�e�B�N���ő吔
	static ParticleData* m_pParticleDatas;	//	�p�[�e�B�N���f�[�^
	static tdn2DObj* m_pTexture;				//	�e�N�X�`��


	// �p�[�e�B�N���Z�b�g
	static void Set(LPPARTICLE pd);
	static void	Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float rotate, float angle, float stretch, float scale, u8 flag);
	static void	Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotete, float stretch, float scale, u8 flag);
	static void	Set2(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, const Vector3 &Pos, const Vector3 &Move, const Vector3 &Power, float r, float g, float b, float rotete, float angle, float stretch, float scale, u8 flag);

	static void Reset();
};


/*�P���G�t�F�N�g*/
void ShiningEffect(Vector3 _pos, float _scale = 1.0f);
/*�o�[�X�g�łĂ܂�EF*/
void AuraMAXEffect(Vector3 _pos, float _scale = 1.0f, Vector3 _col = Vector3(0.3f, 0.7f, 1.0f));

void Sand_effect(float x, float y, float z);
void Rend_effect(const Vector3 &pos, float scale, int num = 50);
void Rend_effect2(const Vector3 &pos, float scale, int num = 30);

//�ΉԃG�t�F�N�g
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

//�{���G�t�F�N�g
void	EffectBomb(Vector3 pos, float scale);


//������N���X

//�����G�t�F�N�g
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


