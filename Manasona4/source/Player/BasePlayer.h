#pragma once
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/Entity/EntityEnum.h"
#include "../BaseEntity/State/StateMachine.h"
#include "BasePlayerState.h"

//
#include "Data/SelectData.h"
#include "../Cutin/CutIn.h"

// ����̃C���N���[�h
#include "../Collision/Collision.h"

// �G�t�F�N�g�֘A
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect\UVEffect\UVEffectManager.h"

// UI
#include "../UI/ComboUI.h"

// �J����
#include "../Camera/camera.h"

// �R�}���h�\�p
#include "Window\BaseWindow.h"


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

static const PLAYER_COMMAND_BIT c_BitList[(int)PLAYER_INPUT::MAX] =
{
	PLAYER_COMMAND_BIT::A,
	PLAYER_COMMAND_BIT::B,
	PLAYER_COMMAND_BIT::C,
	PLAYER_COMMAND_BIT::D,
	PLAYER_COMMAND_BIT::RIGHT,
	PLAYER_COMMAND_BIT::LEFT,
	PLAYER_COMMAND_BIT::UP,
	PLAYER_COMMAND_BIT::DOWN,
	PLAYER_COMMAND_BIT::R1,
	PLAYER_COMMAND_BIT::R2,
	PLAYER_COMMAND_BIT::R3,
	PLAYER_COMMAND_BIT::L1,
	PLAYER_COMMAND_BIT::L2,
	PLAYER_COMMAND_BIT::L3,

	// START��SELECT�g�͖���
	PLAYER_COMMAND_BIT::NEUTRAL,
	PLAYER_COMMAND_BIT::NEUTRAL
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
	END,	// �I��(���̃t���[���ɗ�����U���I��)
	RECOVERY_HIT,	// bHit����������t���[��
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
	RUSH3,				// �ʏ�3�i��
	ANTI_AIR,				// �΋�U��
	DOKKOI_ATTACK,		// ���i�U��
	SQUAT_ATTACK,		// ���Ⴊ�ݍU��
	DOWN_ATTACK,		// �������U��
	AERIAL_ATTACK,		// �󒆍U��
	AERIALDROP,			// �󒆉��U��
	INVINCIBLE_ATTACK,	// �t�B�j�b�V���A�[�c�����G�Z
	SKILL,				// �n��L�����N�^�[�ŗL�Z
	SKILL2,				// �n��L�����N�^�[�ŗL�Z2
	SKILL3,				// �n��L�����N�^�[�ŗL�Z3
	SKILL4,				// �n��L�����N�^�[�ŗL�Z3
	SKILL_SQUAT,		// ���Ⴊ�݃L�����N�^�[�ŗL�Z
	SKILL_AERIAL,		// �󒆉��L�����N�^�[�ŗL�Z
	SKILL_AERIALDROP,	// �󒆉��L�����N�^�[�ŗL�Z
	THROW,				// ����
	THROW_SUCCESS,		// ��������
	HEAVEHO_DRIVE,		// �q�[�z�[�h���C�u
	HEAVEHO_DRIVE2,		// �q�[�z�[�h���C�u2
	HEAVEHO_DRIVE3,		// �q�[�z�[�h���C�u3
	HEAVEHO_DRIVE_OVERFLOW,// �q�[�z�[�h���C�u_�I�[�o�[�t���[
	FRAMECOUNT,			// ���Ƃ肠�������t���[���v�肽���Ƃ����Ƃ��ɁA���g���Ă���̂�(�������s�̊Ԃ�������A���������̊Ԃ�������B)
	END					// �����Ȃ��@(A���)���O
};

enum class SKILL_ACTION_TYPE
{
	NO_ACTION = -1,
	LAND,
	SQUAT,
	AERIAL,
	AERIALDROP,

	// �ǉ��͂����ɁB�����ȑO�ɏ����ƃX�^���h�����������Ȃ�
	LAND2,
	LAND3,
	LAND4,
	AERIAL2,
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
	RUSH_ATTACK3,			// ���b�V���Ō�
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
	SKILL_LAND,				// �L�����ŗL�n��
	SKILL_SQUAT,			// �L�����ŗL���Ⴊ��
	SKILL_AERIAL,			// �L�����ŗL��
	SKILL_AERIALDROP,		// �L�����ŗL�󒆉�
	SKILL2,					// �X�L���g2
	SKILL3,					// �X�L���g3
	SKILL4,					// �X�L���g4
	HEAVEHO_DRIVE,			// DD
	HEAVEHO_DRIVE2,			// DD2
	HEAVEHO_DRIVE3,			// DD3
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
	BACK_STEP,			// �o�b�N�X�e�b�v
	GUARD_BREAK,		// �K�[�h�u���C�N
	AIROU_DRILL,		// �A�C���[�h����
	AIROU_CIRCLE,		// �A�C���[�T�[�N��
	GUARD_WAVE,			// �K�[�h�E�F�[�u
	MULTIPLE_HIT,		// ���i�q�b�g
	OVER_DRIVE_ACTION,	// �K�E�Z�A�N�V����
	JUMP,				// ��ԂƂ��̃G�t�F�N�g
	AERIAL_JUMP,		// �󒆃W�����v�̃G�t�F�N�g
	ESCAPE,				// ���
	INVINCIBLE_ATTACK,	// �t�؂�i���G�Z�j
	DOKKOI,				// ���i
	MUZZLE_FLASH,		// �}�Y���t���b�V��
	THROW,				// ����
	WILL_POWER,			// ����
	DROP_IMPACT_SUPER,	// �Ă��̏Ռ��g
	CYCLONE_RUN,		// �������N�����đ���
	MULTIPLE_HIT_BLOW,	// ���i�q�b�g�i�����j
	THROW_HOLD,			// �͂܂�����
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
	int damage;					// �^����_���[�W
	bool bHit;					// �����������ǂ���(�����i�q�b�g�h�~�p)���i�q�b�g���Ăق����U�����ƁA�܂��l����K�v������
	bool bHitSuccess;			// �U�����K�[�h���ꂸ�ɓ����������ǂ���
	int WhiffDelayFrame;		// ���t���[����ɋ�U���SE���Đ����邩(WhiffSE���g��Ȃ�(nullptr)�Ȃ�ʂɐݒ肵�Ȃ��Ă�����)
	EFFECT_TYPE HitEffectType;	// �G�t�F�N�g�̃^�C�v
	LPCSTR HitSE;				// ���������Ƃ��ɖ炷SE
	LPCSTR WhiffSE;				// ��U��SE
	EFFECT_TYPE WhiffEffectType;// �U��G�t�F�N�g
	int pierceLV;				// �ђʃ��x��
	bool bAntiAir;				// �΋�U�����ǂ����@(�ǉ�)
	bool bFinish;				// �t�B�j�b�V���ɂȂ�U�����ǂ���
	ANTIGUARD_ATTACK AntiGuard;	// �K�[�h��˂��j��U���̃^�C�v
	ATTACK_ATTRIBUTE attribute;	// �U���̑���(�e�A�����A�Ō�)
	SHAKE_CAMERA_INFO ShakeCameraInfo;	// �J�����U���p�\����
	int GuardRecoveryFrame;		// �K�[�h���ꂽ�Ƃ��Ɂu�K�[�h���Ă��鑊��ɗ^����v�d������
	float fGuardKnockBackPower;	// �K�[�h�������Ƃ��̂������
	float fComboRate;			// �R���{�␳(0.8�Ȃ玟�̍U����0.8�{�̕␳��������A�␳�͊|�����킳���)
	float fRepeatAttackRate;	// ���Z�␳�l

	// �������n��q�b�g�Ƌ󒆃q�b�g�ŕ����������
	struct
	{
		bool bBeInvincible;			// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
		//Vector2 LandFlyVector;		// �������Đ�����΂��x�N�g��(����{�I��x��+�ɂ��邱��)
		//Vector2 AerialFlyVector;	// �����肪�󒆂ɂ������@�������Đ�����΂��󒆃x�N�g��(����{�I��x��+�ɂ��邱��)
		Vector2 FlyVector;
		int iHitStopFrame;			// �q�b�g�X�g�b�v�̎���
		int HitRecoveryFrame;		// �U�����q�b�g�����ۂɁu����ɗ^����v�d���̎���
		DAMAGE_MOTION DamageMotion;	// ���������̃��[�V����
	}places[(int)HIT_PLACE::MAX];
	AttackData() :
		fRepeatAttackRate(0.75f),
		fComboRate(1), 
		attribute(ATTACK_ATTRIBUTE::STRIKE), 
		HitSE(nullptr),
		WhiffSE(nullptr),
		bHit(false),
		bHitSuccess(false),
		damage(0),
		WhiffDelayFrame(0),
		pierceLV(0),
		bAntiAir(false),
		bFinish(true),
		AntiGuard(ANTIGUARD_ATTACK::NONE),
		pCollisionShape(new CollisionShape::Square),
		fGuardKnockBackPower(0),
		GuardRecoveryFrame(0),
		WhiffEffectType(EFFECT_TYPE::WHIFF){}
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
	//	�W�����v�p�Ɏg���\����
	//------------------------------------------------------
	struct Jump
	{
		bool bHold;	// ���Ⴊ��ł���
		bool bAttackPush;	// ���Ⴊ��ł�Œ��ɍU���{�^������������
		int HoldTimer;		// ���Ⴊ��ł鎞��
		int iAerialJumpCoolTime;	// 1�t���[���󒆃W�����v���Ȃ��悤�ɁA���t���[���͑҂�
		int LandTimer;		// ���n�̎���
		void Clear()
		{
			bHold = true;
			bAttackPush = false;
			HoldTimer = iAerialJumpCoolTime = LandTimer = 0;
		}
	};
	Jump m_jump;

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
	};

public:
	//------------------------------------------------------
	//	�R���X�g���N�^�E������
	//------------------------------------------------------
	BasePlayer(SIDE side, const SideData &data);
	void InitAI(AI_TYPE type);	// AI�p�̏�����

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
	virtual void ShaderSendParam();
	void RenderCommandFrame(int x, int y);
	void RenderDebug();

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
	//	������(�Z�M�p)
	/****************************/
	virtual void DownAttackInit()
	{
		// ���������[�V�����ɕς���
		SetMotion(MOTION_TYPE::DOWN_ATTACK);

		// ���U���X�e�[�g��2�i�ڂɐݒ肷��@����HIT�t���O�������Ă���
		SetActionState(BASE_ACTION_STATE::DOWN_ATTACK);

		SetDirAngle();
	}
	virtual void DownAttackExit()
	{
		// ��s���̓��Z�b�g
		AheadCommandReset();
	}
	virtual bool DownAttackUpdate()
	{
		// �U���I�����Ă���
		if (!isAttackState())
		{
			return true;
		}

		// HIT���Ă�����L�����Z���ł���
		if (GetAttackData()->bHit == true)
		{
			//////////////////////////////////////////////
			//	�X�L���L�����Z��
			//============================================
			if (SkillCancel(this)) return false;

			//////////////////////////////////////////////
			//	�X�^���h�L�����Z��
			//============================================
			if (StandCancel(this)) return false;

			//////////////////////////////////////////////
			//	�t�B�j�b�V���U���{�^��
			//============================================
			if (InvincibleAttackCancel(this)) return false;

			//////////////////////////////////////////////
			//	�q�[�z�[�h���C�u�L�����Z��
			//============================================
			if (HeaveHoCancel(this)) return false;
		}
		return false;
	}

	/****************************/
	//	���G�U���J�E���^�[���������Ƃ��̃C�x���g
	/****************************/
	virtual void OnInvincibleCounterSuccess(){ MyAssert(0, "�p�����悤�B"); }

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
	virtual void HeavehoDriveHitEvent()		// �ŏ��̈ꌂ�����������Ƃ��ɌĂяo�����֐�
	{
		// ����̃o�[�X�g�g�p�s��
		GetTargetPlayer()->ActionNotOverDrive();
		// m_bNotOverDrive = true; 
	}		

	/****************************/
	//	�q�[�z�[�h���C�u_�I�[�o�[�t���[
	/****************************/
	virtual void HeavehoDriveOverFlowInit() = 0;		// �X�L���������ɌĂяo��
	virtual void HeavehoDriveOverFlowExit() = 0;
	virtual void HeavehoDriveOverFlowUpdate() = 0;
	virtual void HeavehoDriveOverFlowSuccessInit() = 0;		// �K�E���Ăĉ��o���̏�����
	virtual void HeavehoDriveOverFlowSuccessUpdate() = 0;		// �K�E���Ăĉ��o���̍X�V

	/****************************/
	//	�L�����N�^�[�ʂ̃f�[�^
	/****************************/
	virtual void SetCharacterData(void* desk) {};
	virtual void CharacterDataReset() {};			//	�L�������̏��������
	virtual void CharacterDataUIRender() {};			//	�L�������̕K�v��UI�̕`��
	virtual void CharacterDataUIAction(int iDelayTimer) {};			//	�L�������̕K�v��UI�̋N��

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
	virtual void AIHighAttackButton() = 0;	// ���̃L�����N�^�[�́Y�Z(�����Z)
	
	virtual bool AINearSkill() = 0;			// �e�L�����N�^�[�̋ߋ����̃X�L���U��
	virtual bool AIMiddleSkill() = 0;		// �e�L�����N�^�[�̒������̃X�L���U��
	virtual bool AIFarSkill() = 0;			// �e�L�����N�^�[�̉������̃X�L���U��
	virtual bool AIAerialNearSkill() = 0;	// �e�L�����N�^�[�̋󒆋ߋ����̃X�L���U��

	virtual void AIAttackSkillButton() = 0;	// ���̃L�����N�^�[�̃X�L���U��(�U���n�X�L������)

	virtual float AIAttackRange() = 0;		// �U���͈�
	virtual float AIRunAttackRange() = 0;	// �����Ă�Ƃ��̍U���͈�
	virtual float AIThrowRange() = 0;		// �����͈̔�

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
	Vector3 &GetMove() { return m_vMove; }
	Vector3 *GetMoveAddress(){ return &m_vMove; }
	Vector3 &GetPos() { return m_vPos; }
	Vector3 *GetPosAddress() { return &m_vPos; }
	void AddMove(const Vector3 &v){ m_vMove += v; }
	void SetMove(const Vector3 &v){ m_vMove.Set(v.x, v.y, v.z); }
	void SetPos(const Vector3 &v) { m_vPos.Set(v.x, v.y, v.z); }
	void MoveClampX(float val) { m_vMove.x = Math::Clamp(m_vMove.x, -val, val); }
	void SetMoveUpdate(bool bMove){ m_bMoveUpdate = bMove; }
	void SetSinkingUpdate(bool bSinking){ m_bSinkingUpdate = bSinking; }
	bool isSinkingUpdateOK(){ return m_bSinkingUpdate; }

	//------------------------------------------------------
	//	�����E�A���O��
	//------------------------------------------------------
	DIR GetDir() { return m_dir; }
	void SetDir(DIR dir) { m_dir = dir; }
	void ReverseDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }
	DIR GetTargetDir(){ return m_TargetDir; }
	void SetTargetDir(DIR dir){ m_TargetDir = dir; }
	void SetAngleY(float a){ m_fAngleY = a; }
	void SetDirAngle(){ m_fAngleY = DIR_ANGLE[(int)m_dir]; }
	void TurnOverDir(){ m_dir = (m_dir == DIR::LEFT) ? DIR::RIGHT : DIR::LEFT; }
	float GetDirVecX(){ return (m_dir == DIR::LEFT) ? -1.0f : 1.0f; }
	float GetTargetDirVecX(){ return (m_TargetDir == DIR::LEFT) ? -1.0f : 1.0f; }

	//------------------------------------------------------
	//	�v���C���[�A�N�V�����E�U���A�N�V�����E�X�e�[�g
	//------------------------------------------------------
	BASE_ACTION_STATE GetActionState(){ return m_eActionState; }
	AttackData *GetAttackData(BASE_ACTION_STATE state)
	{
		// [12/10]
		MyAssert(isAttackState(),"GetAttackData�ŃA�^�b�N�f�[�^���Ȃ����Ƃ�����");	// �A�^�b�N�f�[�^���Ȃ��X�e�[�g�ŃA�^�b�N�f�[�^���Q�Ƃ��悤�Ƃ��Ă���}
		return m_ActionDatas[(int)state].pAttackData;
	}
	AttackData *GetAttackData(){ return GetAttackData(m_eActionState);}// �����̏�Ԃ̍U���f�[�^��n��
	bool isHitAttack() { return (isAttackState()) ? GetAttackData()->bHit : false; }
	bool isFrameAction() { return (m_eActionState != BASE_ACTION_STATE::NO_ACTION); }
	bool isAttackState()
	{
		// ���X�e�[�g�������Ȃ��̂������łȂ����m�F�ƁA���ꂪtrue�Ȃ�AttackData��null�o�Ȃ����Ƃ��ۏ؂����
		if (isFrameAction()){ return m_ActionDatas[(int)m_eActionState].isAttackData(); }// �A�N�V�����f�[�^��null����Ȃ����ǂ�����Ԃ�
		else return false;
	}
	bool isActiveFrame()
	{
		if (!isFrameAction()) return false;
		return (m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame] == FRAME_STATE::ACTIVE);
	}
	FRAME_STATE GetActionFrame()
	{
		if (!isFrameAction()) return FRAME_STATE::END;
		return m_ActionFrameList[(int)m_eActionState][m_iCurrentActionFrame];
	}
	FRAME_STATE GetActionFrame(BASE_ACTION_STATE state, int frame)
	{
		return m_ActionFrameList[(int)state][frame];
	}
	void SetActionState(BASE_ACTION_STATE state)
	{
		m_eActionState = state;

		// NO_ACTION����Ȃ�������
		if (isFrameAction())
		{
			m_iCurrentActionFrame = 0;				// �t���[��0�ɃZ�b�g

			// �U����������
			// ������HIT�t���O�������Ă���
			if (isAttackState())
			{
				m_ActionDatas[(int)state].pAttackData->bHit = false;	// �q�b�g�t���O���Z�b�g
				m_ActionDatas[(int)state].pAttackData->bHitSuccess = false;
			}
		}
	}
	int GetCurrentFrame(){ return m_iCurrentActionFrame; }
	int GetRushStep(){ return m_iRushStep; }
	SKILL_ACTION_TYPE GetSkillActionType(){ return m_eSkillActionType; }
	void AddRushStep(){ m_iRushStep++; }
	void RushStepReset(){ m_iRushStep = 0; }
	void SetSkillActionType(SKILL_ACTION_TYPE type){ m_eSkillActionType = type; }
	bool isDownState(){ return (m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance())); }
	bool isHeavehoDriveState() { return (m_pStateMachine->isInState(*BasePlayerState::HeavehoDrive::GetInstance())); }


	//------------------------------------------------------
	//	�n��E��
	//------------------------------------------------------
	bool isLand() { return m_bLand; }
	bool isSquat() { return m_bSquat; }
	bool isAerialJump() { return m_bAerialJump; }
	bool isAerialDash() { return m_bAerialDash; }
	int  GetAerialDashFrame() { return m_iAerialDashFrame; }
	float GetAerialDashSpeed(){ return m_tagCharacterParam.fAerialDashSpeed; }
	float GetMaxJump() { return m_tagCharacterParam.fMaxJump; }
	Jump *GetJump() { return &m_jump; }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetSquat(bool bSquat) { m_bSquat = bSquat; }
	void SetAerialJump(bool bAerialJump) { m_bAerialJump = bAerialJump; }
	void SetAerialDash(bool bAerialDash) { m_bAerialDash = bAerialDash; }
	void SetAerialDashFrame(int iAerialDashFrame) { m_iAerialDashFrame = iAerialDashFrame; }

	//------------------------------------------------------
	//	�U���̃q�b�g�X�g�b�v�E�d��
	//------------------------------------------------------
	int GetRecoveryFrame() { return m_iRecoveryFrame; }
	int GetHitStopFrame(){ return m_iHitStopFrame; }
	void SetHitStopFrame(int frame){m_iHitStopFrame = frame; }
	void SetRecoveryFrame(int frame) { m_iRecoveryFrame = frame; }
	std::list<BASE_ACTION_STATE> *GetRecoveryDamageCount(){ return &m_RecoveryDamageCount; }
	bool isDown() { return m_bDown; }
	void SetDown(bool down) { m_bDown = down; }

	//------------------------------------------------------
	//	�K�[�h
	//------------------------------------------------------
	GUARD_STATE GetGuardState(){ return m_eGuardState; }
	void SetGuardState(GUARD_STATE state){ m_eGuardState = state; }
	bool isGuardSuccess() { return m_bGuardSuccess; }
	void SetGuardSuccess(bool flag) { m_bGuardSuccess = flag; }

	//------------------------------------------------------
	//	���G�֘A
	//------------------------------------------------------
	int GetInvincibleLV(){ return m_iInvincibleLV; }
	int GetInvincibleTime(){ return m_iInvincibleTime; }
	bool isInvincible(){ return  (m_iInvincibleLV >= 1) ? true : false; }
	void SetInvincible(int time, int lv){ m_iInvincibleTime = time; m_iInvincibleLV = lv; }
	void SetInvincibleLV(int lv = 1){ m_iInvincibleLV = lv; }
	void SetInvincibleTime(int time){ m_iInvincibleTime = time; }
	void InvincibleOff(){ m_iInvincibleLV = 0; m_iInvincibleTime = 0; }
	bool isEscape() { return m_bEscape; }
	void SetEscapeFlag(bool bEscape)
	{
		m_bEscape = bEscape; 
		m_bSinkingUpdate = !bEscape;
	}
	bool isInvincibleCounterType(){ return m_tagCharacterParam.bInvincibleCoutner; }

	//------------------------------------------------------
	//	���[�V����
	//------------------------------------------------------
	void SetMotion(MOTION_TYPE type){ if (m_pObj) m_pObj->SetMotion(m_iMotionNumbers[(int)type]); }
	void SetMotion(int MotionNo)
	{
		if (m_pObj)
		{
			if (m_pObj->GetMotion() != MotionNo)
				m_pObj->SetMotion(MotionNo);
		}
	}


	//------------------------------------------------------
	//	HP�E�_���[�W
	//------------------------------------------------------
	int GetMaxHP() { return m_tagCharacterParam.iMaxHP; }
	int GetHP() { return m_iHP; }
	void SetHP(int hp) { m_iHP = hp; }
	void AddHP(int add) { m_iHP += add; }
	float GetDamageRate(){ return m_fDamageRate; }
	void AddDamageRate(float rate){ m_fDamageRate = min(m_fDamageRate + rate, 1);}		// ���Z(1.0�ȏ�ɂȂ�Ȃ��悤�ɂ���)
	void MultDamageRate(float rate){ m_fDamageRate = max(m_fDamageRate * rate, 0.2f); }	// ��Z(0.3�ȉ��ɂ͂Ȃ�Ȃ��悤�ɂ���) // (12/31) �R���{�̕������Ȃ�L�����Ă����̂ŏ��������܂�3.0->2.0
	//void MultOverDriveDamageRate(float rate) { m_fDamageRate = max(m_fDamageRate * rate, 0.5f); }	// �K�E�Z�͍Œ�ۏ�l���グ��
	void ResetDamageRate(){ m_fDamageRate = 1.0f; }
	// HP�̊���
	int  GetHPPercentage()
	{
		// HP�̃��[�g���v�Z
		MyAssert((m_tagCharacterParam.iMaxHP != 0), "�ő�HP��0����0�Ŋ��邱�Ƃ�");
		float HpRate = (float)m_iHP / (float)m_tagCharacterParam.iMaxHP;
		HpRate *= 100;//�@�J��グ�� 
		return (int)HpRate;
	};

	//------------------------------------------------------
	//	�X�R�A
	//------------------------------------------------------
	int GetCollectScore(){ return m_iCollectScore; }
	void AddCollectScore(int score) { m_iCollectScore += score; }
	void ConversionScore(){ m_iScore += m_iCollectScore; m_iCollectScore = 0; }
	int GetScore(){ return m_iScore; }


	//------------------------------------------------------
	//	�C���v�b�g�E�R�}���h
	//------------------------------------------------------
	int GetInputList(PLAYER_INPUT input) { return m_iInputList[(int)input]; }
	void SetInputList(PLAYER_INPUT inputNo, int inputFlag)
	{
		m_iInputList[(int)inputNo] = inputFlag;
		m_wCommandHistory[0] |= (inputFlag | m_iInputList[(int)inputNo] & 0x01) ? (int)c_BitList[(int)inputNo] : 0;
	}
	bool isAnyPushKey(){ return (m_iInputList[(int)PLAYER_INPUT::A] == 3 || m_iInputList[(int)PLAYER_INPUT::B] == 3 || m_iInputList[(int)PLAYER_INPUT::C] == 3 || m_iInputList[(int)PLAYER_INPUT::D] == 3); }
	bool isPushInput(PLAYER_COMMAND_BIT CommandBit, bool bAhead = false) { if (bAhead){ if ((int)CommandBit & (int)m_wAheadCommand) return true; } return (m_wCommandHistory[0] & (int)CommandBit) ? true : false; }
	//PLAYER_COMMAND_BIT GetInputCommandBit(PLAYER_INPUT input)
	//{
	//	PLAYER_COMMAND_BIT bit(PLAYER_COMMAND_BIT::NEUTRAL);

	//	switch (input)
	//	{
	//	case PLAYER_INPUT::A:
	//		bit = PLAYER_COMMAND_BIT::A;
	//		break;
	//	case PLAYER_INPUT::B:
	//		bit = PLAYER_COMMAND_BIT::B;
	//		break;
	//	case PLAYER_INPUT::C:
	//		bit = PLAYER_COMMAND_BIT::C;
	//		break;
	//	case PLAYER_INPUT::D:
	//		bit = PLAYER_COMMAND_BIT::D;
	//		break;
	//	case PLAYER_INPUT::RIGHT:
	//		bit = PLAYER_COMMAND_BIT::RIGHT;
	//		break;
	//	case PLAYER_INPUT::LEFT:
	//		bit = PLAYER_COMMAND_BIT::LEFT;
	//		break;
	//	case PLAYER_INPUT::UP:
	//		bit = PLAYER_COMMAND_BIT::UP;
	//		break;
	//	case PLAYER_INPUT::DOWN:
	//		bit = PLAYER_COMMAND_BIT::DOWN;
	//		break;
	//	case PLAYER_INPUT::R1:
	//		bit = PLAYER_COMMAND_BIT::R1;
	//		break;
	//	case PLAYER_INPUT::R2:
	//		bit = PLAYER_COMMAND_BIT::R2;
	//		break;
	//	case PLAYER_INPUT::R3:
	//		bit = PLAYER_COMMAND_BIT::R3;
	//		break;
	//	case PLAYER_INPUT::L1:
	//		bit = PLAYER_COMMAND_BIT::L1;
	//		break;
	//	case PLAYER_INPUT::L2:
	//		bit = PLAYER_COMMAND_BIT::L2;
	//		break;
	//	case PLAYER_INPUT::L3:
	//		bit = PLAYER_COMMAND_BIT::L3;
	//		break;
	//	}
	//	return bit;
	//}
	bool isPushInputTRG(PLAYER_COMMAND_BIT CommandBit, bool bAhead = false)
	{
		// ��s���̓t���O���������s���͂��Ԃ�
		if (bAhead) if ((int)CommandBit & (int)m_wAheadCommand) return true;

		// �܂��A�������u�Ԃ��ǂ������m�F
		if ((m_wCommandHistory[0] & (int)CommandBit) == 0) return false;

		// ���������ǂ����͕��������̂ŁA�������ςȂ��łȂ����Ƃ��m�F����
		if ((m_wCommandHistory[1] & (int)CommandBit)) return false;		// �������ςȂ���񂯁I

		return true;
	}
	WORD GetCommandHistory(int no = 0) { return m_wCommandHistory[no]; }
	WORD GetAheadCommand(){ return m_wAheadCommand; }
	void AheadCommandUpdate()
	{
		FOR((int)PLAYER_INPUT::MAX)
		{
			const PLAYER_COMMAND_BIT bit(c_BitList[i]);
			if (isPushInputTRG(bit)) m_wAheadCommand |= (int)bit;
		}
	}
	void AheadCommandReset(){ m_wAheadCommand &= 0; }
	void AheadCommandReset(PLAYER_COMMAND_BIT ResetBit){ m_wAheadCommand &= ((int)ResetBit ^ 0xffff); }
	bool isDoublePush(PLAYER_COMMAND_BIT InputBit, int JudgeFrame = c_COMMAND_FRAME_MAX)
	{
		// �������u�Ԃ��ǂ������m�F
		if (!isPushInputTRG(InputBit)) return false;

		// 2�O�ɉ����Ă���OK�I�I
		int iCheckFrame(-1);
		for (int i = 2; i < c_COMMAND_FRAME_MAX; i++)
		{
			if ((m_wCommandHistory[i] & (int)InputBit))
			{
				if (iCheckFrame == -1 && i > 12) break;
				iCheckFrame++;
			}
			else
			{
				if (iCheckFrame >= 0) break;
			}
		}
		return (iCheckFrame != -1 && iCheckFrame < JudgeFrame);
	}

	//int GetCOMMAND_FRAME_MAX() { return c_COMMAND_FRAME_MAX; }
	
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
	PanelEffectManager *GetPanelEffectManager() { return m_pPanelEffectMGR; }
	UVEffectManager *GetUVEffectManager() { return m_pUVEffectMGR; }
	void AddEffectAction(Vector3 pos, EFFECT_TYPE effectType, Vector3 AttackVec = VECTOR_ZERO);
	void GuardEffectAction();
	void GuardEffectStop();
	void GuardEffectUpdate();
	tdn2DAnim* GetThrowMark() { return m_pThrowMark; }

	//------------------------------------------------------
	//	�o��
	//------------------------------------------------------
	float GetOverDriveGage(){ return m_OverDriveGage; } // �o���Q�[�W
	void AddOverDriveGage(float add) { m_OverDriveGage = max(0, min(m_OverDriveGage + add, 100)); }
	void SetOverDriveGage(float set) { m_OverDriveGage = set; }
	bool isOverDrive(){ return m_bOverDrive; } // �o�����Ă邩
	int GetOverDriveFrame(){ return m_OverDriveFrame; } // �o������
	void SetOverDriveFrame(int frame) { m_OverDriveFrame = frame; } // �o������
	OVERDRIVE_TYPE GetOverDriveType(){ return m_OverDriveType; }	// �o���̎��
	void ActionOverDrive(OVERDRIVE_TYPE type);

	//------------------------------------------------------
	//	����
	//------------------------------------------------------
	bool isWillPower() { return m_bWillPower; }
	void WillPowerUpdate(PLAYER_UPDATE updateFlag);

	//------------------------------------------------------
	//	�E�B���h�E�p 
	//------------------------------------------------------
	BaseWindow* GetCommandWindow() { return m_pCommandWindow; };

	//------------------------------------------------------
	//	�L�����N�^�[�̃^�C�v
	//------------------------------------------------------
	CHARACTER GetCharacterType() { return m_eCharacterType; };

	//------------------------------------------------------
	//	���̑�
	//------------------------------------------------------
	int GetDeviceID() { return m_iDeviceID; }
	//bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	float GetMaxSpeed() { return m_tagCharacterParam.fMaxSpeed; }
	SIDE GetSide(){ return m_side; }
	CollisionShape::Square *GetHitSquare() { return &m_tagCharacterParam.HitSquare; }	// �X�e�[�W�̂����蔻��p

	// UI
	//ComboUI* GetComboUI(){ return m_pComboUI; }

	// �Œ�_�b�V���t���[��
	void AddDushFrame(int add) { m_iDashFrame += add; }
	void SetDushFrame(int set) { m_iDashFrame = set; }
	int  GetDushFrame() { return m_iDashFrame; }

	// 
	bool isGameTimerStopFlag() { return m_bGameTimerStopFlag; }
	void SetGameTimerStopFlag(bool flag) { m_bGameTimerStopFlag = flag; }

	// �Ё[�ف[
	EFFECT_CAMERA_ID GetOverFlowCameraID(){ return m_tagCharacterParam.eHeaveHoOverFlowCameraID; }

	void SetLastOneWin(){ m_bLastOneWin = true; }

	// �o�������E���h������珟����Ԃ̃`�F�b�N(�ꌂ�K�E)
	bool isHeaveHoDriveOverFlowOK(){ return (m_bLastOneWin && m_pTargetPlayer->isWillPower()); }

	CUTIN_TYPE_NAME GetCutInType(){ return m_tagCharacterParam.eCutInType; }

	iex3DObj *GetDefaultObj(){ return m_pDefaultObj; }
	iex3DObj *GetObj(){ return m_pObj; }

	// �I�u�W�F�؂�ւ�
	void ChangeHeaveHoDriveOverFlowObj()
	{
		m_pObj = m_pHHDOFObj;
		m_pObj->SetMotion(0);
		m_pObj->SetFrame(0);
	}

	// ������
	int GetWinNum() { return m_iWinNum; }
	void AddWinNum(int add) { m_iWinNum += add; }

	// ��O��
	tdn2DAnim* GetFacePic() { return m_pFacePic; }

	//
	tdn2DAnim* GetCutinPic() { return m_pCutinPic; };
	char* GetName() { return m_pName; };

	// �I�[�o�[�h���C�u�Q�[�W���g���Ȃ�
	bool isNotOverDrive() { return m_bNotOverDrive; }
	void SetNotOverDrive(bool flag) { m_bNotOverDrive = flag; }
	void ActionNotOverDrive() { m_bNotOverDrive = true; /*m_pNotOverDriveMark->Action(); */}
	void StopNotOverDrive() {	m_bNotOverDrive = false;/* m_pNotOverDriveMark->Stop();*/}

	// AI��
	bool isAI() { return m_bAI; }
	AI* GetAI() { return m_pAI; }

	// �J���[
	virtual void ChangeColor() = 0;// (12/28) ��ŐF�ς����悤�ɂ�����

	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// �萔
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float	c_END_MOVE_LINE;		// �ړ����I����đ�C�ɖ߂�ړ��l���C��
	static const int	c_RUSH_AHEAD_START;		// ���b�V�����̐�s���͎�t�J�n�t���[��
	static const float	c_GRAVITY;				// �S�������ʂŎ����E�̏d��
	static const float	c_GRAVITY_MAX;			// �d�͂̍ő�l
	//static const float	c_MAX_JUMP;				// �W�����v�ő僉�C��
	static const int	c_RECOVERY_FLAME;		// ���J�o���\�t���[��
	static const int	c_OVERDRIVE_MAX_GAGE;	// �o���Q�[�W�̍ő�l
	static const int	c_OVERDRIVE_MAX_TIME;	// �o�����؂��܂ł̎���
	static const int	c_THROW_ESCAPE_FRAME;	// �����ʂ��̗P�\�t���[��
	static const int	c_THROW_MISS_FRAME;		// �������s�̃t���[��(�S�L�������ʂ��낤�Ƃ����l��)
	static const int	c_THROW_RELEASE_FRAME;	// ���������ŁA�p�V���ĂȂ��Ă�Ԃ̃t���[��(������S�L�������ʂ��낤)
	static const int	c_WINNER_TIME;			// �������[�V�����Đ�����
	static const float	c_GUARD_DISTANCE;		// �K�[�h���������鋗��
	static const int	c_FIRST_HIT_ADD_DAMAGE;	// ���i�q�b�g���Z�_���[�W
	static const int	c_COUNTER_BONUS_DAMAGE;	// �J�E���^�[�q�b�g���̃{�[�i�X���Z�l(�_���[�W)

protected:
	//------------------------------------------------------
	//	�L�������ŗL�Ŏ��l
	//------------------------------------------------------
	struct CharacterParam
	{
		int iMaxHP;									// �ő�HP
		float fMaxSpeed;							// �ő�X�s�[�h(�������鑬��)
		float fMaxJump;								// �ő�W�����v��
		float fAerialDashSpeed;						// �󒆃_�b�V���̑��x
		bool bInvincibleCoutner;					// ���G�U�����J�E���^�[�^�C�v��
		CollisionShape::Square HitSquare;			// �l�p����(�X�e�[�W�Փ˂Ŏg��)
		EFFECT_CAMERA_ID eHeaveHoOverFlowCameraID;	// �ꌂ�K�E�J������ID
		CUTIN_TYPE_NAME eCutInType;					// �ꌂ�K�E�J�b�g�C����ID
	}m_tagCharacterParam;
	CharacterParam m_tagOrgCharacterParam;			// �����̏���ۑ�
	
	//------------------------------------------------------
	//	�ʒu�E�ړ���
	//------------------------------------------------------
	Vector3 m_vPos;			// ���W
	Vector3 m_vMove;		// �ړ��l
	bool m_bMoveUpdate;		// ���ꂪtrue�Ȃ�A�d�͂Ƃ���C��R�Ƃ����󂯂�(�A�C���[�h�����Ƃ��y���\�i�Ƃ���false�ɂ���)
	bool m_bSinkingUpdate;	// ���ꂪtrue�Ȃ�A�L�����ƃL�����̂߂荞�ݔ�����s��

	//------------------------------------------------------
	//	�����E�A���O��
	//------------------------------------------------------
	float m_fAngleY;		// Y�̉�]�p�x
	DIR m_dir;				// ���E�̃t���O
	DIR m_TargetDir;		// ����ɑ΂��鍶�E�̃t���O


	//------------------------------------------------------
	//	�v���C���[�A�N�V�����E�U���A�N�V�����E�X�e�[�g
	//------------------------------------------------------
	FRAME_STATE m_ActionFrameList[(int)BASE_ACTION_STATE::END][FRAME_MAX];
	ActionData m_ActionDatas[(int)BASE_ACTION_STATE::END];
	BASE_ACTION_STATE m_eActionState;
	int m_iCurrentActionFrame;				// �U���t���[�����X�g�̒����Đ����Ă���t���[��
	int m_iRushStep;						// ���b�V���A�^�b�N�̃X�e�b�v
	SKILL_ACTION_TYPE m_eSkillActionType;	// �X�L���A�N�V����


	//------------------------------------------------------
	//	�n��E��
	//------------------------------------------------------
	bool m_bLand;			// ���n�t���O
	bool m_bSquat;			// ���Ⴊ�݃t���O
	bool m_bAerialJump;		// �󒆃W�����v�̃t���O
	bool m_bAerialDash;		// �󒆃_�b�V���̃t���O
	int m_iAerialDashFrame;	// �󒆃_�b�V���̎���

	//------------------------------------------------------
	//	�U���̃q�b�g�X�g�b�v�E�d��
	//------------------------------------------------------
	std::list<BASE_ACTION_STATE> m_RecoveryDamageCount;	//�u��炢���v�ɍU����������Ă�J�E���g
	int m_iRecoveryFrame;								// 1�ȏ�Ȃ�0�ɂȂ�܂ő�����󂯕t���Ȃ�
	int m_iHitStopFrame;									// 1�ȏ�Ȃ�0�ɂȂ�܂ŃX�g�b�v
	bool m_bDown;										// �_�E�����

	//------------------------------------------------------
	//	�K�[�h
	//------------------------------------------------------
	GUARD_STATE m_eGuardState;
	bool m_bGuardSuccess;

	//------------------------------------------------------
	//	���G�֘A
	//------------------------------------------------------
	int m_iInvincibleLV;				// ���G���ǂ���
	int m_iInvincibleTime;				// ���G����		���G����0�ȏ�̒l������A0�ɂȂ�܂Ńf�N�������g�����
	float m_fInvincibleColRate;			// ���G���Ԃ̓_�ł̃��[�g
	int m_iInvincibleColRateFlame;		// ���G�_�ł̃t���[��(�����j
	bool m_bInvincibleColRateUpFlag;	// ���G�_�ł̏グ�����t���O
	bool m_bEscape;						// �G�X�P�[�v����̎��ɗ��t���O


	//------------------------------------------------------
	//	���[�V����
	//------------------------------------------------------
	int m_iMotionNumbers[(int)MOTION_TYPE::MAX];


	//------------------------------------------------------
	//	HP�E�_���[�W
	//------------------------------------------------------
	int m_iHP;
	float m_fDamageRate;				// �����Ƀ_���[�W�̂�����␳


	//------------------------------------------------------
	//	�X�R�A
	//------------------------------------------------------
	int m_iScore;				// ���̌�̖{���̃X�R�A
	int m_iCollectScore;			// ���̑O�̒��߂Ă���X�R�A


	//------------------------------------------------------
	//	�C���v�b�g�E�R�}���h
	//------------------------------------------------------
	static const int c_COMMAND_FRAME_MAX = 30;
	int m_iInputList[(int)PLAYER_INPUT::MAX]; 	// �����Ă���L�[���i�[
	WORD m_wCommandHistory[c_COMMAND_FRAME_MAX];
	WORD m_wAheadCommand;	// ��s���͗p

	//------------------------------------------------------
	//	�X�^���h
	//------------------------------------------------------
	Stand::Base *m_pStand;		// �X�^���h�̊��N���X�̎���


	//------------------------------------------------------
	//	�ΐ푊��̃|�C���^
	//------------------------------------------------------
	BasePlayer *m_pTargetPlayer;		// ����v���C���[�̃|�C���^������(�D������ɂ�����)

	//------------------------------------------------------
	//	�I�u�W�F�N�g�̎���
	//------------------------------------------------------
	iex3DObj *m_pDefaultObj, *m_pHHDOFObj;	// ���b�V������
	iex3DObj *m_pObj;				// �Q�Ƃ��邾���̃��b�V������
	BaseUVEffect *m_pSpeedLine;		// ���K�E�Z�̃X�s�[�h���C��
	tdn2DAnim* m_pFacePic;			// ��O��


	//------------------------------------------------------
	//	�G�t�F�N�g
	//------------------------------------------------------
	PanelEffectManager* m_pPanelEffectMGR;	// �p���p���A�j���G�t�F�N�g 
	UVEffectManager* m_pUVEffectMGR;			// UV�G�t�F�N�g
	tdn2DAnim*		m_pThrowMark;			//	�����}�[�N

	//------------------------------------------------------
	//	�o��
	//------------------------------------------------------
	float m_OverDriveGage;			// �o���Q�[�W
	bool m_bOverDrive;				// �o�����Ă邩
	int m_OverDriveFrame;			// �o������
	OVERDRIVE_TYPE m_OverDriveType;	// �o���̎��

	//------------------------------------------------------
	//	����
	//------------------------------------------------------
	bool m_bWillPower;				// �����l�t���O
	int m_iSpGettingFrame;			// SP���񕜂��Ă����p�x 

	//------------------------------------------------------
	//	�g���[�j���O�p	(12/24) �O���̃f�[�^�Őݒ肷��̂Ń����o�ϐ��͍��Ȃ� 
	//------------------------------------------------------

	//------------------------------------------------------
	//	�E�B���h�E�p 
	//------------------------------------------------------
	BaseWindow* m_pCommandWindow;

	//------------------------------------------------------
	//	�L�����N�^�[�̃^�C�v
	//------------------------------------------------------
	CHARACTER m_eCharacterType;

	//------------------------------------------------------
	//	���̑�
	//------------------------------------------------------
	const int m_iDeviceID;		// �����̃R���g���[���[�̔ԍ�(�����A�X�}�u����xP�ɒl����)
	SIDE m_side;			// ���̃L�����N�^�[�̏������Ă�`�[��		
	StateMachine<BasePlayer>* m_pStateMachine; // ���X�e�[�g�}�V��
	AI*						  m_pAI;			// ��AI
	bool m_bAI;					// AI���ǂ���

	int m_iDashFrame;			//  �_�b�V���̍Œ᎞��

	// �q�[�z�[�p�̕ϐ�
	int m_iHeavehoStopTimer;			// �q�[�z�[�h���C�u�����̃U���[���h�̎���
	int m_iHeaveHoDriveOverFlowFrame;	// �q�[�z�[�̕K�E��SE�Ƃ��G�t�F�N�g�p�ɍ��
	bool m_bLastOneWin;					// ����1���E���h�ŏ���(�ꌂ�K�E�̃t���O�Ŏg��)

	float m_fOrangeColRate;		// ���G�Z�̃I�����W�̌����[�g
	float m_fMagentaColRate;	// ���i�Z�̃}�[���^�̌����[�g

	// �X�g�b�v�t���O
	bool m_bGameTimerStopFlag;

	// ���E���h�l����
	int m_iWinNum;

	// �I�[�o�[�h���C�u�Q�[�W���g���Ȃ�
	bool m_bNotOverDrive;		 


	// (����)�@//////////////////////////////////////////////////////////
	// �G�t�F�N�g�}�l�[�W���[����l�������������R��
	// ��̃}�l�[�W���[�Œǉ����Ă����`���Ɓ@�ϒ��z��ő��₵�Ă�����
	// �Q�[������new Delete�͂ƂĂ��d������
	// ���������_���[�W�G�t�F�N�g�ȂǃL�����N�^�[�Ɉ����΂����Ǝv����
	//	4�l�ΐ��5���q�b�g�G�t�F�N�g���ł邱�Ƃ͂Ȃ��̂Ł@
	// �������[�h�͏d���Ȃ邪�Q�[�����������Ȃ���}�V�B

	// �p�����Ă��ɋ����I�ɌĂ΂���
	void LoadCharacterParam(LPSTR filename);
	void LoadAttackFrameList(LPSTR filename);
	void LoadAttackDatas(LPSTR filename);

	// UI
	//ComboUI* m_pComboUI;

	/*****************************/ 
	// �L�����N�^�[�̗����G�����O
	tdn2DAnim* m_pCutinPic;
	char* m_pName;


};


