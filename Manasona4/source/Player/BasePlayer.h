#pragma once
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// �O���錾
namespace CollisionShape
{
	class Square;
}
namespace Stage
{
	class Base;
}

enum class PLAYER_INPUT
{
	A,		// 
	B,		// 
	C,		// �W�����v
	D,		// 
	RIGHT,
	LEFT,
	UP,
	DOWN,
	R1,
	R2,
	R3,
	L1,
	L2,
	L3,
	START,
	SELECT,
	MAX
};

// �L�[�{�[�h�\
static const KEYCODE KeyCodeList[(int)PLAYER_INPUT::MAX] = {
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_UP,
	KEY_DOWN,
	KEY_R1,
	KEY_R2,
	KEY_R3,
	KEY_L1,
	KEY_L2,
	KEY_L3,
	KEY_START,
	KEY_SELECT,
};

// ����
enum class DIR { LEFT, RIGHT, MAX };

enum class ATTACK_FRAME
{
	START,	// ����
	ATTACK,	// ����(���̊ԂɍU������)
	FOLLOW,	// �d��(�㌄)
	END	// �I��(���̃t���[���ɗ�����U���I��)
};

// �S�L�����ŗL�Ŏ��U���^�C�v
enum BASE_ATTACK_STATE
{
	RUSH1,	// �ʏ�1�i��
	RUSH2,	// �ʏ�2�i��
	RUSH3,	// �ʏ�3�i��
	END		// �����Ȃ�
};

class BasePlayer : public BaseGameEntity
{
protected:
	struct Jump
	{
		bool bHold;	// ���Ⴊ��ł���
		int HoldTimer;		// ���Ⴊ��ł鎞��
		int PlayerHoldTimer;	// ���Ⴊ��ł�ԂɃv���C���[����W�����v�{�^�����󂯕t���鎞��
		int LandTimer;		// ���n�̎���
		void Clear()
		{
			bHold = true;
			HoldTimer = PlayerHoldTimer = LandTimer = 0;
		}
	};
	Jump m_jump;

	struct RushAttack
	{
		int step;	// 0,1,2�Ői�߂�
		bool bHit;	// ���ꂪtrue�̎��Ƀ{�^���������Ǝ��̍U���ɍs��
		void Clear()
		{
			step = 0;
			bHit = false;
		}
	};
	RushAttack m_RushAttack;

	struct AttackData
	{
		//Attack_collision shape;		// �������蔻��̌`
		int damage;	// �^����_���[�W(�X�R�A�H)
		int pierceLV;		// �ђʃ��x��
		bool bBeInvincible;	// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
		bool bHit;			// �����������ǂ���(�����i�q�b�g�h�~�p)���i�q�b�g���Ăق����U�����ƁA�܂��l����K�v������
		Vector2 FlyVector;	// �������Đ�����΂��x�N�g��(����{�I��x��+�ɂ��邱��)
		LPCSTR SE_ID;		// ���������Ƃ��ɖ炷SE
	}m_AttackDatas[(int)BASE_ATTACK_STATE::END];

public:
	BasePlayer(int deviceID);
	~BasePlayer();
	void Control();			// �v���C���[����̓��͂��󂯕t����
	virtual void Update();	// ��{�I�ȍX�V
	void UpdatePos();		// ���W�X�V(�����ɌĂяo��)
	void Move(); // �����̐���
	virtual void Render();

	// �p�����Ă��ɋ����I�ɌĂ΂���
	virtual void InitAttackDatas() = 0;

	bool HandleMessage(const Message& msg); //���b�Z�[�W���󂯎��

	// �Q�b�^�[
	Vector3 &GetMove() { return m_move; }
	Vector3 &GetPos() { return m_pos; }
	Vector3 *GetPosAddress() { return &m_pos; }
	int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }
	bool isPushInput(PLAYER_INPUT input) { return (m_InputList[(int)input] == 1 || m_InputList[(int)input] == 3); }	// �������u�ԂƉ����Ă���Ԃǂ��炩��������true
	StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }// ���X�e�[�g�}�V���̃A�N�Z�T�����
	DIR GetDir() { return m_dir; }
	bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	bool isLand() { return m_bLand; }
	Jump *GetJump() { return &m_jump; }
	RushAttack *GetRushAttack() { return &m_RushAttack; }
	float GetMaxSpeed() { return m_maxSpeed; }
	BASE_ATTACK_STATE GetAttackState(){ return m_AttackState; }
	AttackData *GetAttackData(){ return &m_AttackDatas[(int)m_AttackState]; }
	int GetPierceLV(){ return m_AttackDatas[(int)m_AttackState].pierceLV; }
	LPCSTR GetAttackSE(){ return m_AttackDatas[(int)m_AttackState].SE_ID; }
	bool isAttackState(){ return (m_AttackState != BASE_ATTACK_STATE::END); }
	bool isAttackFrame()
	{
		if (!isAttackState()) return false;
		return (m_AttackFrameList[m_AttackState][m_CurrentAttackFrame] == ATTACK_FRAME::ATTACK);
	}
	int GetInvincibleLV(){ return m_InvincibleLV; }

	// �Z�b�^�[
	void SetMove(const Vector3 &v) { m_move.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void SetDir(DIR dir) { m_dir = dir; }
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetAttackState(BASE_ATTACK_STATE state)
	{
		m_AttackState = state; 

		// �U����������
		if (isAttackState())
		{
			m_AttackDatas[(int)state].bHit = false;	// �q�b�g�t���O���Z�b�g
			m_CurrentAttackFrame = 0;				// �t���[��0�ɃZ�b�g
		}
	}


	// �A�N�Z�T�[
	void AddMove(const Vector3 &v) { m_move += v; }
	void MoveClampX(float val) { m_move.x = Math::Clamp(m_move.x, -val, val); }

	// �X�e�[�W�̂����蔻��p
	CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }


	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// �萔
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float c_END_MOVE_LINE;		// �ړ����I����đ�C�ɖ߂�ړ��l���C��
	static const float c_FRONT_BRAKE_LINE;	// ���蒆�ɑ���𗣂��ău���[�L�Ɉڍs����܂ł̈ړ��l���C��
	static const float c_GRAVITY;			// �S�������ʂŎ����E�̏d��
	static const float c_MAX_JUMP;			// �W�����v�ő僉�C��

protected:
	const int m_deviceID;		// �����̃R���g���[���[�̔ԍ�(�����A�X�}�u����xP�ɒl����)
	iex3DObj *m_pObj;	// ���b�V������
	Vector3 m_pos;	// ���W
	Vector3 m_move;	// �ړ��l
	CollisionShape::Square *m_pHitSquare;	// �l�p����(�X�e�[�W�Փ˂Ŏg��)
	DIR m_dir;				// ���E�̃t���O
	//float m_Gravity;		// �d�͒l
	float m_maxSpeed;		// �L�����N�^�[�̍ő�X�s�[�h 
	float m_angleY;
	bool m_bLand;			// ���n�t���O
	int m_InputList[(int)PLAYER_INPUT::MAX]; 	// �����Ă���L�[���i�[
	StateMachine<BasePlayer>* m_pStateMachine; // ���X�e�[�g�}�V��
	int m_InvincibleLV;			// ���G���ǂ���
	int m_InvincibleTime;		// ���G����		���G����0�ȏ�̒l������A0�ɂȂ�܂Ńf�N�������g�����
	int m_CurrentAttackFrame;	// �U���t���[�����X�g�̒����Đ����Ă���t���[��

	static const int FRAME_MAX = 256; // �U���̃t���[���̍ő�l(�����蒴���邱�Ƃ͂Ȃ����낤)
	ATTACK_FRAME m_AttackFrameList[(int)BASE_ATTACK_STATE::END][FRAME_MAX];
	BASE_ATTACK_STATE m_AttackState;

	// �p�����Ă��ɋ����I�ɌĂ΂���
	void LoadAttackFrameList(char *filename);
};


