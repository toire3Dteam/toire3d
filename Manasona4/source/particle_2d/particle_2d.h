#pragma once

typedef struct Tag_particle_2d
{
public:
	int		type;			//	�`

	int		aFrame;			//	�o���t���[��
	COLOR	aColor;			//	�o���J���[

	int		eFrame;			//	���Ńt���[��
	COLOR	eColor;			//	�o���J���[

	int		mFrame;			//	�ō��t���[��
	COLOR	mColor;			//	�ō��J���[

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

	//	�X�V
	void Update();

	//	�`��
	inline bool SetVertex(TLVERTEX* v, int uindex, int vindex);
	void Render(int uindex, int vindex);

	// �ړ�
	inline void MovePos(const Vector2 &move);
};



class Particle_2d
{
protected:
	int	nParticles;					//	�p�[�e�B�N���ő吔
	Particle_2d_data*	ParticleData;	//	�p�[�e�B�N���f�[�^
	tdn2DObj* lpTexture;				//	�e�N�X�`��

	int uindex, vindex; // �e�N�X�`���̕����� type�ō��ォ��E�ɐ�����
public:
	Matrix CurViewProjection;

	Particle_2d() :nParticles(0), ParticleData(nullptr), lpTexture(nullptr), uindex(1), vindex(1){}
	~Particle_2d(){ Release(); }

	//	�������E���
	void Initialize(char* filename, int nParticles, int uindex, int vindex);
	void Release();

	//	�o��
	void Set(LPPARTICLE_2d pd);
	void	Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor,
		const Vector2 &pos, const Vector2 &Move, const Vector2 &Power, float rotate, float stretch, float scale, u8 flag);
	void	Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha,
		const Vector2 &pos, const Vector2 &Move, const Vector2 &Power,
		float r, float g, float b, float scale, u8 flag);

	//	�X�V
	void Update();
	void Reset();
	void Reset(int num);

	//	�`��
	void Render();

	// �ړ�
	void MovePos(const Vector2 &move);
};

// �Q�[�����l�͂�����g��
class Particle2dManager
{
public:
	static void Initialize(char *filename, int nParticles, int uindex, int vindex);
	static void Release();
	static void Update();
	static void Render();

	// �p�[�e�B�N���֐�����
	static void Effect_KiraKira(const Vector2 &pos, const Vector2 &range, float scale = 20.0f, float ScaleFluctuation = 5.0f, int LoopCount = 1, int EndFrame = 120);	// ���炫��G�t�F�N�g
	static void Effect_SandCloud(const Vector2 &pos);		// ����
	static void Effect_RealSheep(const Vector2 &pos);		// ���A���r�̃I�[��
	static void Effect_Hinoko(const Vector2 &pos);			// �΂̕�
	static void Effect_FatSmoke(const Vector2 &pos, float scale = 96, int LoopCount = 2);		// �f�u��
	static void Effect_Unlimited(const Vector2 &pos);		// �A�����~�I�[��
	static void Effect_NikuSmoke(const Vector2 &pos);		// �����C
private:
	static Particle_2d *m_pParticle2d;
};