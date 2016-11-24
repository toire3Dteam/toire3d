#pragma once
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/Entity/EntityEnum.h"
#include "../BaseEntity/State/StateMachine.h"
#include "BasePlayerState.h"

// ����̃C���N���[�h
#include "../Collision/Collision.h"

// �G�t�F�N�g�֘A
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect\UVEffect\UVEffectManager.h"

// UI
#include "../UI/ComboUI.h"


// �O���錾
class AI;
namespace Stand
{
	class Base;
}
namespace Stage
{
	class Base;
}
struct SideData;

// �萔
enum class PLAYER_INPUT
{
	A,		// (�q�[�z�[�h���C�u�L�����Z��)��(��U��)���ǂ������U��(�㉺�i)
	B,		// (�U��)��(���U��)���K�E
	C,		// (�W�����v)��(�K�E�Z)����U��
	D,		// (�L�����ŗL)
	RIGHT,
	LEFT,
	UP,
	DOWN,
	R1,		// �G�X�P�[�v
	R2,
	R3,
	L1,
	L2,
	L3,
	START,
	SELECT,
	MAX
};

enum class PLAYER_COMMAND_BIT
{
	NEUTRAL = 0,
	A = 1 << 1,
	B = 1 << 2,
	C = 1 << 3,
	D = 1 << 4,
	RIGHT = 1 << 5,
	LEFT = 1 << 6,
	UP = 1 << 7,
	DOWN = 1 << 8,
	R1 = 1 << 9,
	R2 = 1 << 10,
	R3 = 1 << 11,
	L1 = 1 << 12,
	L2 = 1 << 13,
	L3 = 1 << 14,
};

enum class PLAYER_UPDATE
{
	CONTROL_NO,		// BasePlayerState��ʂ����ǃR���g���[���͂��Ȃ�
	NO_FSM,			// �R���g���[���͎󂯕t���邯�ǁABasePlayerState�͔������Ȃ�
	CONTROL_OK		// �ǂ�����OK
};

//static CommandList[(int)PLAYER_INPUT::MAX] = 
//{
//	
//	NANAME = PLAYER_INPUT::RIGHT + PLAYER_INPUT::LEFT;
//};


// ����
enum class DIR { LEFT, RIGHT, MAX };
static const float DIR_ANGLE[(int)DIR::MAX] =
{
	PI * 1.5f,	// ������
	PI * .5,	// �E����
};

enum class FRAME_STATE
{
	START,	// ����
	ACTIVE,	// ����(���̊ԂɍU������)
	FOLLOW,	// �d��(�㌄)
	END	// �I��(���̃t���[���ɗ�����U���I��)
};
static const int FRAME_MAX = 256; // �U���̃t���[���̍ő�l(�����蒴���邱�Ƃ͂Ȃ����낤)

// �S�L�����ŗL�Ŏ��A�N�V����
enum class BASE_ACTION_STATE
{
	NO_ACTION = -1,
	ESCAPE,				// ���
	BACKSTEP,			// �o�N�X�e
	STAND,				// �X�^���h����
	OVERDRIVE_ONEMORE,	// �������A�o��
	OVERDRIVE_BURST,	// �o�[�X�g�o��
	RUSH1,				// �ʏ�1�i��
	RUSH2,				// �ʏ�2�i��
	//RUSH3,				// �ʏ�3�i��
	ANTI_AIR,				// �΋�U��
	DOKKOI_ATTACK,		// ���i�U��
	SQUAT_ATTACK,		// ���Ⴊ�ݍU��
	DOWN_ATTACK,		// �������U��
	AERIAL,				// �󒆍U��
	AERIALDROP,			// �󒆉��U��
	INVINCIBLE_ATTACK,	// �t�B�j�b�V���A�[�c�����G�Z
	SKILL,				// �n��L�����N�^�[�ŗL�Z
	SKILL_SQUAT,		// �n��L�����N�^�[�ŗL�Z
	SKILL_AERIAL,		// �󒆉��L�����N�^�[�ŗL�Z
	SKILL_AERIALDROP,	// �󒆉��L�����N�^�[�ŗL�Z
	THROW,				// ����
	THROW_SUCCESS,		// ��������
	HEAVEHO_DRIVE,		// �q�[�z�[�h���C�u
	HEAVEHO_DRIVE_OVERFLOW,// �q�[�z�[�h���C�u_�I�[�o�[�t���[
	FRAMECOUNT,			// ���Ƃ肠�������t���[���v�肽���Ƃ����Ƃ��ɁA���g���Ă���̂�(�������s�̊Ԃ�������A���������̊Ԃ�������B)
	END					// �����Ȃ��@(A���)���O
};

enum class SKILL_ACTION_TYPE
{
	LAND,
	SQUAT,
	AERIAL,
	AERIALDROP,
	MAX
};


enum class MOTION_TYPE
{
	WAIT,					// �ҋ@
	WALK,					// �ړ�
	RUN,					// �_�b�V��
	BACK_WALK,				// ������
	BACK_STEP,				// �o�N�X�e
	//BRAKE,				// �u���[�L
	//U_TURN,				// U�^�[��
	RUSH_ATTACK1,			// ���b�V��1
	RUSH_ATTACK2,			// ���b�V��2
	//RUSH_ATTACK3,			// ���b�V���Ō�
	FINISH_ATTACK,			// �t�B�j�b�V���A�[�c
	SQUAT,					// ���Ⴊ��
	SQUAT_ATTACK,		// ���Ⴊ�ݍU��
	ANTI_AIR_ATTACK,		// ���Ⴊ�ݍU��
	DOKKOI_ATTACK,			// ���i�U��
	DOWN_ATTACK,			// ���i�U��
	JUMP,					// �W�����v
	FALL,					// ����
	RAND,					// ���n
	AERIAL_ATTACK,			// �󒆍U��
	//AERIALDROP_ATTACK,
	AERIAL_DASH,			// �󒆃_�b�V��
	PERSONA,				// �y���\�i����
	CHARA_ATTACK_LAND,		// �L�����ŗL�n��
	CHARA_ATTACK_SQUAT,		// �L�����ŗL���Ⴊ��
	CHARA_ATTACK_AERIAL,	// �L�����ŗL��
	CHARA_ATTACK_AERIALDROP,// �L�����ŗL�󒆉�
	HEAVEHO_DRIVE,			// DD
	BURST,					// �o�[�X�g
	HEAVEHO_DRIVE_OVERFLOW,	// AH
	THROW,					// ����
	THROW_SUCCESS,			// ��������
	THROW_DRAW,				// �����͂���
	UP_GUARD,				// ��i�K�[�h
	DOWN_GUARD,				// ���i�K�[�h
	ESCAPE,					// ���
	KNOCKBACK,
	KNOCKDOWN,				// �������
	KNOCKDOWN_DOWN,			// �������������
	//KNOCKDOWN_AERIAL,
	REVERSAL,				// �N���オ��
	//DIE,
	APPEAR,					// �o��(�|������)
	WIN,
	MAX
};

// �G�t�F�N�g�̎��
enum class EFFECT_TYPE
{
	DAMAGE,				// �_���[�W�G�t�F�N�g
	WHIFF,				// �U��
	RECOVERY,			// ���J�o���[
	PERSONA,			// �y...��..�\..�i!!
	DROP_IMPACT,		// �h���b�v�C���p�N�g
	UPPER,				// �A�b�p�[
	FINISH_HIT,			// �t�B�j�b�V���A�[�c�q�b�g
	BURST,				// �o�[�X�g
	ONEMORE_BURST,		// �������A�o�[�X�g
	ONEMORE_BURST_START,// �������A�o�[�X�g�̑O
	RUN,				// ����u��
	GUARD_BREAK,		// �K�[�h�u���C�N
	AIROU_DRILL,		// �A�C���[�h����
	AIROU_CIRCLE,		// �A�C���[�T�[�N��
	GUARD_WAVE,			// �K�[�h�E�F�[�u
	MULTIPLE_HIT,		// ���i�q�b�g
	OVER_DRIVE_ACTION,	// �K�E�Z�A�N�V����
	JUMP,			// ��ԂƂ��̃G�t�F�N�g
	AERIAL_JUMP,		//�󒆃W�����v�̃G�t�F�N�g
	INVINCIBLE_ATTACK, // �t�؂�i���G�Z�j
	DOKKOI,				//	���i
	MUZZLE_FLASH,		// �}�Y���t���b�V��
};

// �K�[�h�̎��
enum class GUARD_STATE
{
	NO_GUARD,	// �K�[�h���ĂȂ�
	UP_GUARD,	// �����K�[�h
	DOWN_GUARD,	// ���Ⴊ�݃K�[�h
};

// �U���̃^�C�v
enum class ANTIGUARD_ATTACK
{
	NONE,			// �ǂ̃K�[�h�ł��h���镁�ʂ̍U��
	DOWN_ATTACK,	// ���ɂ������i
	UP_ATTACK,		// ���ɂ�����i
	ALL_BREAK,		// �K�[�h�s��
};

// �U������
enum class ATTACK_ATTRIBUTE
{
	STRIKE,	// �Ō�����
	BULLET,	// �e����
	THROW,	// ��������
};

/****************************************/
//		�A�^�b�N�f�[�^
/****************************************/
class AttackData
{
public:
	enum class HIT_PLACE{ LAND, AERIAL, MAX };	// ���肪�ǂ����Ńq�b�g�������B�n��A��

	// �����ɂ���G���A�͒n��q�b�g�Ƌ󒆃q�b�g�Ƃ��֌W�Ȃ����ʂ̏��
	CollisionShape::Square *pCollisionShape;	// �������蔻��`��(�l�p�ɂ���)
	int HitScore;				// ���Z�����X�R�A
	int damage;					// �^����_���[�W
	bool bHit;					// �����������ǂ���(�����i�q�b�g�h�~�p)���i�q�b�g���Ăق����U�����ƁA�܂��l����K�v������
	bool bHitSuccess;			// �U�����K�[�h���ꂸ�ɓ����������ǂ���
	int WhiffDelayFrame;		// ���t���[����ɋ�U���SE���Đ����邩(WhiffSE���g��Ȃ�(nullptr)�Ȃ�ʂɐݒ肵�Ȃ��Ă�����)
	EFFECT_TYPE HitEffectType;	// �G�t�F�N�g�̃^�C�v
	LPCSTR HitSE;				// ���������Ƃ��ɖ炷SE
	LPCSTR WhiffSE;				// ��U��SE
	EFFECT_TYPE WhiffEffectType;	// �U��G�t�F�N�g
	int pierceLV;				// �ђʃ��x��
	bool bAntiAir;				// �΋�U�����ǂ����@(�ǉ�)
	bool bFinish;				// �t�B�j�b�V���ɂȂ�U�����ǂ���
	ANTIGUARD_ATTACK AntiGuard;	// �K�[�h��˂��j��U���̃^�C�v
	ATTACK_ATTRIBUTE attribute;	// �U���̑���(�e�A�����A�Ō�)
	SHAKE_CAMERA_INFO ShakeCameraInfo;	// �J�����U���p�\����
	int GuardRecoveryFrame;		// �K�[�h���ꂽ�Ƃ��Ɂu�K�[�h���Ă��鑊��ɗ^����v�d������
	float fGuardKnockBackPower;	// �K�[�h�������Ƃ��̂������
	float fComboRate;			// �R���{�␳(0.8�Ȃ玟�̍U����0.8�{�̕␳��������A�␳�͊|�����킳���)


	// �������n��q�b�g�Ƌ󒆃q�b�g�ŕ����������
	struct
	{
		bool bBeInvincible;			// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
		//Vector2 LandFlyVector;		// �������Đ�����΂��x�N�g��(����{�I��x��+�ɂ��邱��)
		//Vector2 AerialFlyVector;	// �����肪�󒆂ɂ������@�������Đ�����΂��󒆃x�N�g��(����{�I��x��+�ɂ��邱��)
		Vector2 FlyVector;
		int hitStopFlame;			// �q�b�g�X�g�b�v�̎���
		int HitRecoveryFrame;		// �U�����q�b�g�����ۂɁu����ɗ^����v�d���̎���
		DAMAGE_MOTION DamageMotion;	// ���������̃��[�V����
	}places[(int)HIT_PLACE::MAX];
	AttackData() :fComboRate(1), attribute(ATTACK_ATTRIBUTE::STRIKE), HitSE(nullptr), WhiffSE(nullptr), bHit(false), bHitSuccess(false), HitScore(0), damage(0), WhiffDelayFrame(0), pierceLV(0), bAntiAir(false), bFinish(true), AntiGuard(ANTIGUARD_ATTACK::NONE), pCollisionShape(new CollisionShape::Square),fGuardKnockBackPower(0){}
	~AttackData(){ SAFE_DELETE(pCollisionShape); }
};



// �o���̎��
enum class OVERDRIVE_TYPE
{
	//HEAVE_HO,	// �ǂ������ʏ�̊o��
	ONEMORE,	// �R���{���ɂ���Ȃ�ǌ������邽�߂ɔ���
	BURST		// ����̍U�����󂯂Ă���Ƃ��Ƀo�[�X�g�Ƃ��Ĕ���
};

/***************************************************************/
//
//		�x�[�X�v���C���[
//
/***************************************************************/
class BasePlayer : public BaseGameEntity
{
protected:
	//------------------------------------------------------
	//	�R�}���h�Ɋ֌W����ϐ�
	//------------------------------------------------------
	static const int c_COMMAND_FRAME_MAX = 30;
	WORD m_CommandHistory[c_COMMAND_FRAME_MAX];
	WORD m_AheadCommand;	// ��s���͗p

	//------------------------------------------------------
	//	�W�����v�p�Ɏg���\����
	//------------------------------------------------------
	struct Jump
	{
		bool bHold;	// ���Ⴊ��ł���
		bool bAttackPush;	// ���Ⴊ��ł�Œ��ɍU���{�^������������
		int HoldTimer;		// ���Ⴊ��ł鎞��
		int PlayerHoldTimer;	// ���Ⴊ��ł�ԂɃv���C���[����W�����v�{�^�����󂯕t���鎞��
		int LandTimer;		// ���n�̎���
		void Clear()
		{
			bHold = true;
			bAttackPush = false;
			HoldTimer = PlayerHoldTimer = LandTimer = 0;
		}
	};
	Jump m_jump;

	//------------------------------------------------------
	//	���b�V���U���̃X�e�b�v
	//------------------------------------------------------
	struct RushAttack
	{
		int step;	// 0,1,2�Ői�߂�
		//bool bNextOK;	// ���ꂪtrue�̎��Ƀ{�^���������Ǝ��̍U���ɍs��
		void Clear()
		{
			step = 0;
			//bNextOK = false;
		}
	};
	RushAttack m_RushAttack;

	//------------------------------------------------------
	//	�L���������A�N�V�����f�[�^
	//------------------------------------------------------
	class ActionData
	{
	public:
		AttackData *pAttackData;

		ActionData() :pAttackData(nullptr){}
		~ActionData() { SAFE_DELETE(pAttackData); }

		bool isAttackData() { return (pAttackData != nullptr); }	// �A�^�b�N�f�[�^������Ƃ������Ƃ́A�U���n�̃X�e�[�g�ł���
		void InstanceAttackData()
		{
			// isAttackData��true���Ԃ�悤�ɂȂ�
			if (!pAttackData) pAttackData = new AttackData;
		}
	}m_ActionDatas[(int)BASE_ACTION_STATE::END];

public:
	//------------------------------------------------------
	//	�R���X�g���N�^�E������
	//------------------------------------------------------
	BasePlayer(SIDE side, const SideData &data);
	void InitAI();	// AI�p�̏�����

	//------------------------------------------------------
	//	�f�X�g���N�^
	//------------------------------------------------------
	virtual ~BasePlayer();

	//------------------------------------------------------
	//	�X�V
	//------------------------------------------------------
	virtual void Update(PLAYER_UPDATE flag);	// ��{�I�ȍX�V
	virtual void UpdateDrive();			// �q�[�z�[�h���C�u�I�[�o�[�t���[�̂Ƃ������ʂ�X�V
	void UpdatePos();			// ���W�X�V(�����ɌĂяo��)
	void MoveUpdate();			// �����̐���
	void InvincibleUpdate();	 // ���G�̐���
	void OverDriveUpdate();		// �o���̐���
	void ColorUpdate(); // ���F�̐���

	//------------------------------------------------------
	//	�R���g���[������
	//------------------------------------------------------
	void Control();			// �v���C���[����̓��͂��󂯕t����
	void AIControl();

	//------------------------------------------------------
	//	�`��
	//------------------------------------------------------
	virtual void Render();
	virtual void Render(tdnShader* shader, char* name);
	virtual void RenderShadow();
	virtual void RenderDrive();
	virtual void RenderDeferred();
	virtual void RenderUI();


	//------------------------------------------------------
	//	�p�����Ă��ɋ����I�ɌĂ΂���n
	//------------------------------------------------------
	virtual void InitActionDatas() = 0;
	virtual void InitMotionDatas() = 0;

	/****************************/
	//	�L�����N�^�[�ŗL�̗l�X�ȃ|�W�V����
	/****************************/
	virtual Vector3 GetCenterPos() = 0;	// �v���C���[�̐^�񒆂̏ꏊ
	virtual Vector3 GetFlontPos() = 0;   // ���݂̑���̏ꏊ

	/****************************/
	// ���E���h�ׂ��̏�����
	/****************************/
	virtual void Reset();

	/****************************/
	// �|�������̃L�������Ƃ̍X�V
	/****************************/
	virtual void KakeaiInit() = 0;
	virtual void KakeaiExit(){}
	virtual void KakeaiUpdate() = 0;

	/****************************/
	//	���b�V��3�i��
	/****************************/
	virtual void ThirdRushInit() = 0;		// �X�L���������ɌĂяo��
	virtual void ThirdRushExit() = 0;
	virtual void ThirdRushUpdate() = 0;//

	/****************************/
	//	�L�����N�^�[�ŗL�X�L��
	/****************************/
	virtual void SkillInit() = 0;		// �X�L���������ɌĂяo��
	virtual void SkillExit() = 0;
	virtual bool SkillUpdate() = 0;//

	/****************************/
	//	�q�[�z�[�h���C�u
	/****************************/
	virtual void HeavehoDriveInit() = 0;		// �X�L���������ɌĂяo��
	virtual void HeavehoDriveExit() = 0;
	virtual bool HeavehoDriveUpdate() = 0;
	virtual void HeavehoDriveHitEvent(){}		// �ŏ��̈ꌂ�����������Ƃ��ɌĂяo�����֐�

	/****************************/
	//	�q�[�z�[�h���C�u_�I�[�o�[�t���[
	/****************************/
	virtual void HeavehoDriveOverFlowInit() = 0;		// �X�L���������ɌĂяo��
	virtual void HeavehoDriveOverFlowExit() = 0;
	virtual void HeavehoDriveOverFlowUpdate() = 0;
	virtual void HeavehoDriveOverFlowSuccessInit() = 0;		// �K�E���Ăĉ��o���̏�����
	virtual void HeavehoDriveOverFlowSuccessUpdate() = 0;		// �K�E���Ăĉ��o���̍X�V

	/*****************/
	// AI�ɕK�v�Ȋ֐�
	/*****************/
	//(�l��)�@AI���X�e�[�g�}�V�������΂����̂����@�l��->�s������Enter��Execute�݂����Ȋ����Ȃ̂Ł@

	// [����]�܂�AI�p�ɃX�e�[�g�}�V�������
	// ���̒��̂����ȓ����̓{�^���������B�������ꂾ���@
	// enter�ŏ󋵔��f�@execute�Ń{�^������@
	// �X�e�[�g�̎�ށ@�ӂ��i�G���˂炤�j�@�����̃|�C���g�����܂��Ă���(�����FA�����߂ɂ�)�@�Ȃ�
	// 

	// ���ꂼ��̃L�����N�^�[����AI�ɂ��Z�̃{�^����ݒ�I
	virtual void AIHighAtackButton() = 0;	// ���̃L�����N�^�[�́Y�Z(�����Z)
	virtual void AIAtackSkillButton() = 0;	// ���̃L�����N�^�[�̃X�L���U��(�U���n�X�L������)

	virtual float AIAtackRange() = 0;		// �U���͈�
	virtual float AIRunAtackRange() = 0;	// �����Ă�Ƃ��̍U���͈�

	//void AI_Brain();	// �����ōl��->�s���̏������s��
	//void AI_Think();	// �l����֐�
	//void AI_Execute();	// 


	//------------------------------------------------------
	//	���b�Z�[�W��M(BaseEntity�̃I�[�o�[���C�h)
	//------------------------------------------------------
	bool HandleMessage(const Message& msg); //���b�Z�[�W���󂯎��


	//------------------------------------------------------
	//	�X�e�[�g�}�V���̃A�N�Z�T
	//------------------------------------------------------
	StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }


	//------------------------------------------------------
	//	�ʒu�E�ړ���
	//------------------------------------------------------
	Vector3 &GetMove() { return m_move; }
	Vector3 *GetMoveAddress(){ return &m_move; }
	Vector3 &GetPos() { return m_pos; }
	Vector3 *GetPosAddress() { return &m_pos; }
	void AddMove(const Vector3 &v){ m_move += v; }
	void SetMove(const Vector3 &v){ m_move.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void MoveClampX(float val) { m_move.x = Math::Clamp(m_move.x, -val, val); }
	void SetMoveUpdate(bool bMove){ m_bMoveUpdate = bMove; }


	//------------------------------------------------------
	//	�����E�A���O��
	//------------------------------------------------------
	DIR GetDir() { return m_dir; }
	void SetDir(DIR dir) { m_dir = dir; }
	void ReverseDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }
	DIR GetTargetDir(){ return m_TargetDir; }
	void SetTargetDir(DIR dir){ m_TargetDir = dir; }
	void SetAngleY(float a){ m_angleY = a; }
	void SetDirAngle(){ m_angleY = DIR_ANGLE[(int)m_dir]; }
	void TurnOverDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }


	//------------------------------------------------------
	//	�v���C���[�A�N�V�����E�U���A�N�V�����E�X�e�[�g
	//------------------------------------------------------
	BASE_ACTION_STATE GetActionState(){ return m_ActionState; }
	AttackData *GetAttackData(BASE_ACTION_STATE state)
	{
		assert(isAttackState());	// �A�^�b�N�f�[�^���Ȃ��X�e�[�g�ŃA�^�b�N�f�[�^���Q�Ƃ��悤�Ƃ��Ă���}
		return m_ActionDatas[(int)state].pAttackData;
	}
	AttackData *GetAttackData(){ return GetAttackData(m_ActionState);}// �����̏�Ԃ̍U���f�[�^��n��
	bool isHitAttack() { return (isAttackState()) ? GetAttackData()->bHit : false; }
	bool isFrameAction() { return (m_ActionState != BASE_ACTION_STATE::NO_ACTION); }
	bool isAttackState()
	{
		// ���X�e�[�g�������Ȃ��̂������łȂ����m�F�ƁA���ꂪtrue�Ȃ�AttackData��null�o�Ȃ����Ƃ��ۏ؂����
		if (isFrameAction()){ return m_ActionDatas[(int)m_ActionState].isAttackData(); }// �A�N�V�����f�[�^��null����Ȃ����ǂ�����Ԃ�
		else return false;
	}
	bool isActiveFrame()
	{
		if (!isFrameAction()) return false;
		return (m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
	}
	FRAME_STATE GetActionFrame()
	{
		if (!isFrameAction()) return FRAME_STATE::END;
		return m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame];
	}
	void SetActionState(BASE_ACTION_STATE state)
	{
		m_ActionState = state;

		// NO_ACTION����Ȃ�������
		if (isFrameAction())
		{
			m_CurrentActionFrame = 0;				// �t���[��0�ɃZ�b�g

			// �U����������
			// ������HIT�t���O�������Ă���
			if (isAttackState())
			{
				m_ActionDatas[(int)state].pAttackData->bHit = false;	// �q�b�g�t���O���Z�b�g
				m_ActionDatas[(int)state].pAttackData->bHitSuccess = false;
			}
		}
	}
	int GetCurrentFrame(){ return m_CurrentActionFrame; }
	RushAttack *GetRushAttack() { return &m_RushAttack; }
	void SetSkillActionType(SKILL_ACTION_TYPE type){ m_SkillActionType = type; }
	bool isDownState(){ return (m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance())); }


	//------------------------------------------------------
	//	�n��E��
	//------------------------------------------------------
	bool isLand() { return m_bLand; }
	bool isSquat() { return m_bSquat; }
	bool isAerialJump() { return m_bAerialJump; }
	bool isAerialDash() { return m_bAerialDash; }
	int  GetAerialDashFrame() { return m_iAerialDashFrame; }
	Jump *GetJump() { return &m_jump; }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetSquat(bool bSquat) { m_bSquat = bSquat; }
	void SetAerialJump(bool bAerialJump) { m_bAerialJump = bAerialJump; }
	void SetAerialDash(bool bAerialDash) { m_bAerialDash = bAerialDash; }
	void SetAerialDashFrame(int iAerialDashFrame) { m_iAerialDashFrame = iAerialDashFrame; }

	//------------------------------------------------------
	//	�U���̃q�b�g�X�g�b�v�E�d��
	//------------------------------------------------------
	int GetRecoveryFrame() { return m_RecoveryFlame; }
	int GetHitStopFrame(){ return m_HitStopFrame; }
	void SetHitStopFrame(int frame) { m_HitStopFrame = frame; }
	void SetRecoveryFrame(int frame) { m_RecoveryFlame = frame; }
	std::list<BASE_ACTION_STATE> *GetRecoveryDamageCount(){ return &m_RecoveryDamageCount; }

	//------------------------------------------------------
	//	�K�[�h
	//------------------------------------------------------
	GUARD_STATE GetGuardState(){ return m_GuardState; }
	void SetGuardState(GUARD_STATE state){ m_GuardState = state; }


	//------------------------------------------------------
	//	���G�֘A
	//------------------------------------------------------
	int GetInvincibleLV(){ return m_InvincibleLV; }
	int GetInvincibleTime(){ return m_InvincibleTime; }
	bool isInvincible(){ return  (m_InvincibleLV >= 1) ? true : false; }
	void SetInvincible(int time, int lv){ m_InvincibleTime = time; m_InvincibleLV = lv; }
	void SetInvincibleLV(int lv = 1){ m_InvincibleLV = lv; }
	void SetInvincibleTime(int time){ m_InvincibleTime = time; }
	void InvincibleOff(){ m_InvincibleLV = 0; m_InvincibleTime = 0; }
	bool isEscape() { return m_bEscape; }
	void SetEscapeFlag(bool bEscape) { m_bEscape = bEscape; }


	//------------------------------------------------------
	//	���[�V����
	//------------------------------------------------------
	void SetMotion(MOTION_TYPE type){ if (m_pObj) m_pObj->SetMotion(m_MotionNumbers[(int)type]); }
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }


	//------------------------------------------------------
	//	HP
	//------------------------------------------------------
	int GetMaxHP() { return m_MaxHP; }
	int GetHP() { return m_HP; }
	void SetHP(int hp) { m_HP = hp; }


	//------------------------------------------------------
	//	�X�R�A
	//------------------------------------------------------
	int GetCollectScore(){ return m_CollectScore; }
	void AddCollectScore(int score) { m_CollectScore += score; }
	void ConversionScore(){ m_score += m_CollectScore; m_CollectScore = 0; }
	int GetScore(){ return m_score; }


	//------------------------------------------------------
	//	�C���v�b�g�E�R�}���h
	//------------------------------------------------------
	int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }
	void SetInputList(PLAYER_INPUT inputNo, int inputFlag){ m_InputList[(int)inputNo] = inputFlag; }
	bool isAnyPushKey(){ return (m_InputList[(int)PLAYER_INPUT::A] == 3 || m_InputList[(int)PLAYER_INPUT::B] == 3 || m_InputList[(int)PLAYER_INPUT::C] == 3 || m_InputList[(int)PLAYER_INPUT::D] == 3); }
	bool isPushInput(PLAYER_COMMAND_BIT CommandBit, bool bAhead = false) { if (bAhead){ if ((int)CommandBit & (int)m_AheadCommand) return true; } return (m_CommandHistory[0] & (int)CommandBit) ? true : false; }
	PLAYER_COMMAND_BIT GetInputCommandBit(PLAYER_INPUT input)
	{
		PLAYER_COMMAND_BIT bit(PLAYER_COMMAND_BIT::NEUTRAL);
		switch (input)
		{
		case PLAYER_INPUT::A:
			bit = PLAYER_COMMAND_BIT::A;
			break;
		case PLAYER_INPUT::B:
			bit = PLAYER_COMMAND_BIT::B;
			break;
		case PLAYER_INPUT::C:
			bit = PLAYER_COMMAND_BIT::C;
			break;
		case PLAYER_INPUT::D:
			bit = PLAYER_COMMAND_BIT::D;
			break;
		case PLAYER_INPUT::RIGHT:
			bit = PLAYER_COMMAND_BIT::RIGHT;
			break;
		case PLAYER_INPUT::LEFT:
			bit = PLAYER_COMMAND_BIT::LEFT;
			break;
		case PLAYER_INPUT::UP:
			bit = PLAYER_COMMAND_BIT::UP;
			break;
		case PLAYER_INPUT::DOWN:
			bit = PLAYER_COMMAND_BIT::DOWN;
			break;
		case PLAYER_INPUT::R1:
			bit = PLAYER_COMMAND_BIT::R1;
			break;
		case PLAYER_INPUT::R2:
			bit = PLAYER_COMMAND_BIT::R2;
			break;
		case PLAYER_INPUT::R3:
			bit = PLAYER_COMMAND_BIT::R3;
			break;
		case PLAYER_INPUT::L1:
			bit = PLAYER_COMMAND_BIT::L1;
			break;
		case PLAYER_INPUT::L2:
			bit = PLAYER_COMMAND_BIT::L2;
			break;
		case PLAYER_INPUT::L3:
			bit = PLAYER_COMMAND_BIT::L3;
			break;
		}
		return bit;
	}
	bool isPushInputTRG(PLAYER_COMMAND_BIT CommandBit, bool bAhead = false)
	{
		// ��s���̓t���O���������s���͂��Ԃ�
		if (bAhead) if ((int)CommandBit & (int)m_AheadCommand) return true;

		// �܂��A�������u�Ԃ��ǂ������m�F
		if ((m_CommandHistory[0] & (int)CommandBit) == 0) return false;

		// ���������ǂ����͕��������̂ŁA�������ςȂ��łȂ����Ƃ��m�F����
		if ((m_CommandHistory[1] & (int)CommandBit)) return false;		// �������ςȂ���񂯁I

		return true;
	}
	WORD GetCommandHistory(int no = 0) { return m_CommandHistory[no]; }
	WORD GetAheadCommand(){ return m_AheadCommand; }
	void AheadCommandUpdate()
	{
		FOR((int)PLAYER_INPUT::MAX)
		{
			const PLAYER_COMMAND_BIT bit(GetInputCommandBit((PLAYER_INPUT)i));
			if (isPushInputTRG(bit)) m_AheadCommand |= (int)bit;
		}
	}
	void AheadCommandReset(){ m_AheadCommand &= 0; }
	void AheadCommandReset(PLAYER_COMMAND_BIT ResetBit){ m_AheadCommand &= ((int)ResetBit ^ 0xffff); }
	bool isDoublePush(PLAYER_COMMAND_BIT InputBit, int JudgeFrame = c_COMMAND_FRAME_MAX)
	{
		// �������u�Ԃ��ǂ������m�F
		if (!isPushInputTRG(InputBit)) return false;

		// 2�O�ɉ����Ă���OK�I�I
		for (int i = 2; i < JudgeFrame; i++) if ((m_CommandHistory[i] & (int)InputBit)) return true;
		return false;
	}

	//------------------------------------------------------
	//	�X�^���h
	//------------------------------------------------------
	Stand::Base *GetStand(){ return m_pStand; }


	//------------------------------------------------------
	//	�ΐ푊��̃|�C���^
	//------------------------------------------------------
	void SetTargetPlayer(BasePlayer *pPlayer){ m_pTargetPlayer = pPlayer; }
	BasePlayer *GetTargetPlayer(){ return m_pTargetPlayer; }


	//------------------------------------------------------
	//	�G�t�F�N�g
	//------------------------------------------------------
	PanelEffectManager* GetPanelEffectManager() { return m_PanelEffectMGR; }
	UVEffectManager* GetUVEffectManager() { return m_UVEffectMGR; }
	void AddEffectAction(Vector3 pos, EFFECT_TYPE effectType, Vector3 AttackVec = VECTOR_ZERO);
	void GuardEffectAction();
	void GuardEffectStop();
	void GuardEffectUpdate();


	//------------------------------------------------------
	//	�o��
	//------------------------------------------------------
	int GetOverDriveGage(){ return m_OverDriveGage; } // �o���Q�[�W
	void AddOverDriveGage(int add){ m_OverDriveGage = min(m_OverDriveGage + add, 100); }
	void SetOverDriveGage(int set) { m_OverDriveGage = set; }
	bool isOverDrive(){ return m_bOverDrive; } // �o�����Ă邩
	int GetOverDriveFrame(){ return m_OverDriveFrame; } // �o������
	void SetOverDriveFrame(int frame) { m_OverDriveFrame = frame; } // �o������
	OVERDRIVE_TYPE GetOverDriveType(){ return m_OverDriveType; }	// �o���̎��
	void ActionOverDrive(OVERDRIVE_TYPE type);


	//------------------------------------------------------
	//	���̑�
	//------------------------------------------------------
	int GetDeviceID() { return m_deviceID; }
	bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	float GetMaxSpeed() { return m_maxSpeed; }
	SIDE GetSide(){ return m_side; }
	CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }	// �X�e�[�W�̂����蔻��p

	// UI
	//ComboUI* GetComboUI(){ return m_pComboUI; }

	// 
	bool isGameTimerStopFlag() { return m_bGameTimerStopFlag; }
	void SetGameTimerStopFlag(bool flag) { m_bGameTimerStopFlag = flag; }

	iex3DObj *GetObj(){ return m_pObj; }

	// �I�u�W�F�؂�ւ�
	void ChangeHeaveHoDriveOverFlowObj()
	{
		m_pObj = m_pHHDOFObj;
		m_pObj->SetMotion(0);
		m_pObj->SetFrame(0);
	}

	// ������
	int GetWinNum() { return m_WinNum; }
	void AddWinNum(int add) { m_WinNum += add; }

	// ��O��
	tdn2DAnim* GetFacePic() { return m_pFacePic; }

	//
	tdn2DAnim* GetCutinPic() { return m_pCutinPic; };
	char* GetName() { return m_pName; };

	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// �萔
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float	c_END_MOVE_LINE;		// �ړ����I����đ�C�ɖ߂�ړ��l���C��
	static const int	c_RUSH_AHEAD_START;		// ���b�V�����̐�s���͎�t�J�n�t���[��
	static const float	c_GRAVITY;				// �S�������ʂŎ����E�̏d��
	static const float	c_MAX_JUMP;				// �W�����v�ő僉�C��
	static const int	c_RECOVERY_FLAME;		// ���J�o���\�t���[��
	static const int	c_OVERDRIVE_MAX_GAGE;	// �o���Q�[�W�̍ő�l
	static const int	c_OVERDRIVE_MAX_TIME;	// �o�����؂��܂ł̎���
	static const int	c_THROW_ESCAPE_FRAME;	// �����ʂ��̗P�\�t���[��
	static const int	c_THROW_MISS_FRAME;		// �������s�̃t���[��(�S�L�������ʂ��낤�Ƃ����l��)
	static const int	c_THROW_RELEASE_FRAME;	// ���������ŁA�p�V���ĂȂ��Ă�Ԃ̃t���[��(������S�L�������ʂ��낤)
	static const int	c_WINNER_TIME;			// �������[�V�����Đ�����
	static const float	c_GUARD_DISTANCE;		// �K�[�h���������鋗��

protected:
	const int m_deviceID;		// �����̃R���g���[���[�̔ԍ�(�����A�X�}�u����xP�ɒl����)
	SIDE m_side;			// ���̃L�����N�^�[�̏������Ă�`�[��

	iex3DObj *m_pDefaultObj, *m_pHHDOFObj;	// ���b�V������
	iex3DObj *m_pObj;				// �Q�Ƃ��邾���̃��b�V������
	BaseUVEffect *m_pSpeedLine;		// ���K�E�Z�̃X�s�[�h���C��
	tdn2DAnim* m_pFacePic;			// ��O��				
	Vector3 m_pos;	// ���W
	Vector3 m_move;	// �ړ��l
	CollisionShape::Square *m_pHitSquare;	// �l�p����(�X�e�[�W�Փ˂Ŏg��)
	DIR m_dir;				// ���E�̃t���O
	DIR m_TargetDir;		// ����ɑ΂��鍶�E�̃t���O
	//float m_Gravity;		// �d�͒l
	float m_maxSpeed;		// �L�����N�^�[�̍ő�X�s�[�h 
	float m_angleY;
	bool m_bLand;			// ���n�t���O
	bool m_bSquat;			// ���Ⴊ�݃t���O
	bool m_bAerialJump;		// �󒆃W�����v�̃t���O
	bool m_bAerialDash;		// �󒆃_�b�V���̃t���O
	int m_iAerialDashFrame;	// �󒆃_�b�V���̎���
	int m_InputList[(int)PLAYER_INPUT::MAX]; 	// �����Ă���L�[���i�[
	StateMachine<BasePlayer>* m_pStateMachine; // ���X�e�[�g�}�V��
	AI*						  m_pAI;			// ��AI
	int m_InvincibleLV;			// ���G���ǂ���
	int m_InvincibleTime;		// ���G����		���G����0�ȏ�̒l������A0�ɂȂ�܂Ńf�N�������g�����
	float m_InvincibleColRate;	// ���G���Ԃ̓_�ł̃��[�g
	int m_InvincibleColRateFlame;		// ���G�_�ł̃t���[��(�����j
	bool m_bInvincibleColRateUpFlag;		// ���G�_�ł̏グ�����t���O
	int m_CurrentActionFrame;	// �U���t���[�����X�g�̒����Đ����Ă���t���[��
	int m_HitStopFrame;			// 1�ȏ�Ȃ�0�ɂȂ�܂ŃX�g�b�v
	int m_RecoveryFlame;		// 1�ȏ�Ȃ�0�ɂȂ�܂ő�����󂯕t���Ȃ�
	bool m_bEscape;				// �G�X�P�[�v����̎��ɗ��t���O
	bool m_bAI;					// AI���ǂ���
	int m_score;				// ���̌�̖{���̃X�R�A
	int m_CollectScore;			// ���̑O�̒��߂Ă���X�R�A
	bool m_bMoveUpdate;			// ���ꂪtrue�Ȃ�A�d�͂Ƃ���C��R�Ƃ����󂯂�(�A�C���[�h�����Ƃ��y���\�i�Ƃ���false�ɂ���)
	// �K�[�h�ɕK�v�Ȃ��
	GUARD_STATE m_GuardState;
	// HP
	int m_MaxHP;
	int m_HP;
	// �q�[�z�[�p�̕ϐ�
	int m_HeavehoStopTimer;			// �q�[�z�[�h���C�u�����̃U���[���h�̎���
	int m_HeaveHoDriveOverFlowFrame;	// �q�[�z�[�̕K�E��SE�Ƃ��G�t�F�N�g�p�ɍ��
	BasePlayer *m_pTargetPlayer;		// �Ё[�ف[��������v���C���[�̃|�C���^������(�D������ɂ�����)
	float m_fOrangeColRate;	// ���G�Z�̃I�����W�̌����[�g
	float m_fMagentaColRate;	// ���i�Z�̃}�[���^�̌����[�g

	// �K�[�h


	// �X�g�b�v�t���O
	bool m_bGameTimerStopFlag;

	// ���[�V����
	int m_MotionNumbers[(int)MOTION_TYPE::MAX];

	// �o��
	int m_OverDriveGage;			// �o���Q�[�W
	bool m_bOverDrive;				// �o�����Ă邩
	int m_OverDriveFrame;			// �o������
	OVERDRIVE_TYPE m_OverDriveType;	// �o���̎��

	// ���E���h�l����
	int m_WinNum;

	// �X�L���A�N�V����
	SKILL_ACTION_TYPE m_SkillActionType;

	// �u��炢���v�ɍU����������Ă�J�E���g
	std::list<BASE_ACTION_STATE> m_RecoveryDamageCount;

	Stand::Base *m_pStand;		// �X�^���h�̊��N���X�̎���

	FRAME_STATE m_ActionFrameList[(int)BASE_ACTION_STATE::END][FRAME_MAX];
	BASE_ACTION_STATE m_ActionState;

	// (A���)�@//////////////////////////////////////////////////////////
	// �G�t�F�N�g�}�l�[�W���[����l�������������R��
	// ��̃}�l�[�W���[�Œǉ����Ă����`���Ɓ@�ϒ��z��ő��₵�Ă�����
	// �Q�[������new Delete�͂ƂĂ��d������
	// ���������_���[�W�G�t�F�N�g�ȂǃL�����N�^�[�Ɉ����΂����Ǝv����
	//	4�l�ΐ��5���q�b�g�G�t�F�N�g���ł邱�Ƃ͂Ȃ��̂Ł@
	// �������[�h�͏d���Ȃ邪�Q�[�����������Ȃ���}�V�B

	PanelEffectManager* m_PanelEffectMGR;	// �p���p���A�j���G�t�F�N�g 
	UVEffectManager* m_UVEffectMGR;			// UV�G�t�F�N�g

	// �p�����Ă��ɋ����I�ɌĂ΂���
	void LoadAttackFrameList(char *filename);

	// UI
	//ComboUI* m_pComboUI;

	/*****************************/ 
	// �L�����N�^�[�̗����G�����O
	tdn2DAnim* m_pCutinPic;
	char* m_pName;


};


