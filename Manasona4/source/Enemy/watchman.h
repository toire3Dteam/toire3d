#pragma once
#include "IEX_Expansion.h"

enum class ENEMY_TYPE
{
	WOLF,	// �T
	UNLIMITED_WOLF,
	MAX
};


enum class FAT_WOLF_TYPE
{
	SMALL, MIDDLE, LARGE
};

enum class SHEEP_TYPE;

namespace Enemy
{
	class Base
	{
	protected:
		bool m_bErase;			// �����t���O
		tdn2DObj *m_pImage;		// �G�̉摜(�}�l�[�W���̂��Q�Ƃ��邾��)
		Vector2 m_pos;			// ���W
		Vector2 m_MoveVec;		// �ړ��̕���
		float m_speed;			// �ړ����x
		int m_floor;			// ���[��
		int m_AnimeFrame, m_AnimePanel;	// �A�j���֌W
		int W, H;			// ���ƍ���

		// �ړ��ʂ̗ނ̍X�V
		void MoveUpdate();
	public:
		Base(tdn2DObj *image, int floor, float speed) :col_check(false), m_bErase(false), m_floor(floor), m_pImage(image), m_AnimeFrame(0), m_AnimePanel(0), m_MoveVec(Vector2(-1, 0)), m_speed(speed){}
		virtual void Update() = 0;
		virtual void Render(){}

		// �Q�b�^�[
		int GetFloor(){ return m_floor; }
		Vector2 GetCenterPos(){ return Vector2(m_pos.x + W*.5f, m_pos.y + H*.5f); }
		Vector2 GetPos(){ return m_pos; }
		int GetWidth(){ return W; }

		// �Z�b�^�[
		void SetCenterPos(const Vector2 &pos){ m_pos = Vector2(pos.x - 60, pos.y - 60); }

		// �����֌W
		bool EraseOK(){ return m_bErase; }
		void Erase(){ m_bErase = true; }

		bool col_check;
	};

	class Wolf : public Base
	{
	public:
		Wolf(tdn2DObj *image, tdn2DObj *pniku, tdn2DObj *pHone, int floor, float speed, int nikustopTime, bool unlimited);
		~Wolf();
		void Update();
		void Render();

		enum class MODE{ RUN, NIKU, DEAD, MAX }m_mode;
		void ChangeMode(MODE m);
		MODE GetMode(){ return m_mode; }

		void Kill(){ ChangeMode(MODE::DEAD); }

		void SetFatType(FAT_WOLF_TYPE type){ m_type = type; }
		void SetSheepType(SHEEP_TYPE type){ m_SheepType = type; }

		bool isUnlimited(){ return m_bUNLIMITED; }

	private:
		tdn2DObj *m_pNikukutteru, *m_pHoneImage;
		int m_EAT_NIKU_TIMER;	// �Œ�l
		int m_EatNikuTimer;
		float m_OrgSpeed;
		FAT_WOLF_TYPE m_type;	// ���H���I�������ɂȂ�^�C�v
		SHEEP_TYPE m_SheepType;
		BYTE m_alpha;
		int m_seID;		// ���H���p

		bool m_bUNLIMITED;		// �A����mi�T
		struct Zanzou
		{
			Vector2 pos;	// ���W
			Vector2 src;	// �摜����Ă���
			BYTE alpha;
			int frame;
			Zanzou(const Vector2 &pos, const Vector2 &src) :pos(pos), src(src), alpha(255),frame(0){}
			bool Update()
			{
				static const int END_FRAME = 10;
				alpha = (BYTE)((1 - (float)(++frame) / END_FRAME) * 255);
				return (frame > END_FRAME); 
			}
			void Render(tdn2DObj *pImage){ pImage->SetARGB(alpha, (BYTE)255, (BYTE)255, (BYTE)255); pImage->Render((int)pos.x, (int)pos.y, 120, 120, (int)src.x, (int)src.y, 120, 120, RS::COPY); }
		};
		std::list<Zanzou*> m_ZanzouList;
		int m_ZanzouFrame;

		void Run();
		void Niku();
		void Dead();
		void(Wolf::*ModeFunk[(int)MODE::MAX])();
	};
}

class FatWolf : public DebuBase
{
public:
	FatWolf(tdn2DObj *image, const Vector2 &pos, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType);
	~FatWolf();
	void Update();
	void Render();

	// �Q�b�^�[
	FAT_WOLF_TYPE GetType(){ return m_type; }
	SHEEP_TYPE GetSheepType(){ return m_SheepType; }

private:
	FAT_WOLF_TYPE m_type;
	SHEEP_TYPE m_SheepType;
};

class EnemyManager
{
public:
	// ���̂̎擾
	static EnemyManager *GetInstance(){ static EnemyManager i; return &i; }

	void Initialize();
	~EnemyManager();

	void Release();
	void Update();
	void Render();
	void RenderFat();

	/* �f�[�^���Z�b�g */
	void Clear();

	std::list<Enemy::Wolf*> *GetList(){ return &m_list; }
	std::list<FatWolf*> *GetFatList(){ return &m_FatList; }
	void Set_Pointers(StageManager *sm, DataManager *dm){ sp = sm, dmp = dm; }

	// ���H���I�������Ăяo��
	void CreateFatWolf(Enemy::Wolf *wolf, FAT_WOLF_TYPE type, SHEEP_TYPE SheepType);

	// �T�|������\������
	void CheckChangeSpeed(int WolfKillCount);

private:

	// �V���O���g���̍�@
	EnemyManager();
	EnemyManager(const EnemyManager&){}
	EnemyManager &operator=(const EnemyManager){}

	// �G�̉摜
	tdn2DObj *m_pImages[(int)ENEMY_TYPE::MAX];

	// ���H���Ă�I�I�J�~�̉摜
	tdn2DObj *m_pNikukutteru[(int)ENEMY_TYPE::MAX], *m_pFatWolfImages[(int)ENEMY_TYPE::MAX][(int)SHEEP_TYPE::MAX], *m_pHoneImage;

	// �G�̈ړ����x
	float m_EnemySpeed[(int)ENEMY_TYPE::MAX];

	// �j�b�H���Ă鎞�Ԃ�
	int m_NikuTime;

	// �G�������i�[���郊�X�g
	std::list<Enemy::Wolf*> m_list;
	int m_CreateTimer;					// �G�����^�C�}�[
	int m_CREATETIME;					// ��������鎞��
	int m_NextFloor;
	float m_CreateSpeed;	// 0�`1�̒l�B�o���Ԋu
	bool m_bWarning;	// ��[�ɂ񂮏o������

	struct ChangeSpeedLine
	{
		int line;
		float speed;
		int U_percent;	// �A�����~�p�[�Z���g
	};
	int m_UnlimitedPercent;	// �A�����~�p�[�Z���g
	bool m_bUnlimitedCreate;
	std::vector<ChangeSpeedLine*> m_ChangeSpeedLineList;	// ���ԃ��X�g

	// ���H���đ������T���X�g
	std::list<FatWolf*> m_FatList;

	StageManager *sp;
	DataManager *dmp;

	// �G����
	void Create(int floor, ENEMY_TYPE type);
};


// �C���X�^���X��
#define EnemyMgr (EnemyManager::GetInstance())