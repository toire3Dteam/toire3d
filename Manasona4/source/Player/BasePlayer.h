#pragma once
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"
#include "BasePlayerState.h"

// ����̃C���N���[�h
#include "../Collision/Collision.h"

// �G�t�F�N�g�֘A
#include "../Effect/PanelEffect/PanelEffectManager.h"
#include "../Effect\UVEffect\UVEffectManager.h"

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

// �萔
enum class PLAYER_INPUT
{
	A,		// 
	B,		// �U��
	C,		// �W�����v
	D,		// 
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
enum BASE_ACTION_STATE
{
	NO_ACTION = -1,
	ESCAPE,		// ���
	STAND,		// �X�^���h����
	RUSH1,		// �ʏ�1�i��
	RUSH2,		// �ʏ�2�i��
	RUSH3,		// �ʏ�3�i��
	SQUAT,		// ���i�U��
	AERIAL,		// �󒆍U��
	AERIALDROP,	// �󒆉��U��
	FINISH,		// �t�B�j�b�V���A�[�c
	SKILL,		// �n��L�����N�^�[�ŗL�Z
	END		// �����Ȃ��@(A���)���O
};

// �G�t�F�N�g�̎��
enum class EFFECT_TYPE
{
	DAMAGE,	// �_���[�W�G�t�F�N�g
	WHIFF,	//	�U��
	RECOVERY, //���J�o���[
	PERSONA,  //�@�y...��..�\..�i!!
	DROP_IMPACT,// �h���b�v�C���p�N�g
	UPPER,		// �A�b�p�[
	FINISH_HIT,		// �t�B�j�b�V���A�[�c�q�b�g
 	//AIROU_CIRCLE,// �A�C���[�T�[�N��
	

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
	int WhiffDelayFrame;		// ���t���[����ɋ�U���SE���Đ����邩(WhiffSE���g��Ȃ�(nullptr)�Ȃ�ʂɐݒ肵�Ȃ��Ă�����)
	EFFECT_TYPE HitEffectType;	// �G�t�F�N�g�̃^�C�v
	LPCSTR HitSE;				// ���������Ƃ��ɖ炷SE
	LPCSTR WhiffSE;				// ��U��SE
	EFFECT_TYPE WhiffEffectType;	// �U��G�t�F�N�g
	int pierceLV;				// �ђʃ��x��
	bool bAntiAir;				// �΋�U�����ǂ����@(�ǉ�)
	SHAKE_CAMERA_INFO ShakeCameraInfo;	// �J�����U���p�\����

	// �������n��q�b�g�Ƌ󒆃q�b�g�ŕ����������
	struct
	{
		bool bBeInvincible;			// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
		//Vector2 LandFlyVector;		// �������Đ�����΂��x�N�g��(����{�I��x��+�ɂ��邱��)
		//Vector2 AerialFlyVector;	// �����肪�󒆂ɂ������@�������Đ�����΂��󒆃x�N�g��(����{�I��x��+�ɂ��邱��)
		Vector2 FlyVector;
		int hitStopFlame;			// �q�b�g�X�g�b�v�̎���
		int recoveryFlame;			// �d���̎���
	}places[(int)HIT_PLACE::MAX];
	AttackData() :HitSE(nullptr), WhiffSE(nullptr), bHit(false), pCollisionShape(new CollisionShape::Square){}
	~AttackData(){ SAFE_DELETE(pCollisionShape); }
};

/***************************************************************/
//
//		�x�[�X�v���C���[
//
/***************************************************************/
class BasePlayer : public BaseGameEntity
{
protected:
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
	BasePlayer(int deviceID, bool bAI);
	~BasePlayer();
	void InitAI();
	void Control();			// �v���C���[����̓��͂��󂯕t����
	void AIControl();
	virtual void Update();	// ��{�I�ȍX�V
	void UpdatePos();		// ���W�X�V(�����ɌĂяo��)
	void Move();			// �����̐���
	virtual void Render(tdnShader* shader,char* name);
	virtual void RenderDeferred();

	/****************************/
	//	�L�����N�^�[�ŗL�X�L��
	/****************************/
	virtual void SkillUpdate() = 0;//

	/*****************/
	// AI�ɕK�v�Ȋ֐�

	//(�l��)�@AI���X�e�[�g�}�V�������΂����̂����@�l��->�s������Enter��Execute�݂����Ȋ����Ȃ̂Ł@

	// [����]�܂�AI�p�ɃX�e�[�g�}�V�������
	// ���̒��̂����ȓ����̓{�^���������B�������ꂾ���@
	// enter�ŏ󋵔��f�@execute�Ń{�^������@
	// �X�e�[�g�̎�ށ@�ӂ��i�G���˂炤�j�@�����̃|�C���g�����܂��Ă���(�����FA�����߂ɂ�)�@�Ȃ�
	// 

	//void AI_Brain();	// �����ōl��->�s���̏������s��
	//void AI_Think();	// �l����֐�
	//void AI_Execute();	// 


	// �p�����Ă��ɋ����I�ɌĂ΂���
	virtual void InitActionDatas() = 0;

	bool HandleMessage(const Message& msg); //���b�Z�[�W���󂯎��

	// �Q�b�^�[
	int GetDeviceID() { return m_deviceID; }
	Vector3 &GetMove() { return m_move; }
	//Vector3 &GetStandSaveMove(){ return m_StandSaveMove; }
	Vector3 &GetPos() { return m_pos; }
	Vector3 *GetPosAddress() { return &m_pos; }
	int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }
	bool isPushInput(PLAYER_INPUT input) { return (m_InputList[(int)input] == 1 || m_InputList[(int)input] == 3); }	// �������u�ԂƉ����Ă���Ԃǂ��炩��������true
	StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }// ���X�e�[�g�}�V���̃A�N�Z�T�����
	DIR GetDir() { return m_dir; }
	bool isMaxSpeed() { return (abs(m_move.x) >= m_maxSpeed); }
	bool isLand() { return m_bLand; }
	bool isAerialJump() { return m_bAerialJump; }
	Jump *GetJump() { return &m_jump; }
	RushAttack *GetRushAttack() { return &m_RushAttack; }
	float GetMaxSpeed() { return m_maxSpeed; }
	BASE_ACTION_STATE GetActionState(){ return m_ActionState; }

	void SetInputList(PLAYER_INPUT inputNo, int inputFlag){ m_InputList[(int)inputNo] = inputFlag; }

	AttackData *GetAttackData(BASE_ACTION_STATE state)
	{
		if (!isAttackState())
		{
			int i = 0;
			assert(isAttackState());	// �A�^�b�N�f�[�^���Ȃ��X�e�[�g�ŃA�^�b�N�f�[�^���Q�Ƃ��悤�Ƃ��Ă���
		}
		return m_ActionDatas[(int)state].pAttackData; 
	}

	AttackData *GetAttackData()
	{
		// �����̏�Ԃ̍U���f�[�^��n��
		return GetAttackData(m_ActionState);
	}

	int GetRecoveryCount(){ return m_recoveryCount; }

	//int GetPierceLV(){ return m_ActionDatas[(int)m_ActionState].pierceLV; }
	//LPCSTR GetAttackSE(){ return m_ActionDatas[(int)m_ActionState].SE_ID; }
	bool isFrameAction() { return (m_ActionState != BASE_ACTION_STATE::NO_ACTION); }
	bool isAttackState()	// ���X�e�[�g�������Ȃ��̂������łȂ����m�F�ƁA���ꂪtrue�Ȃ�AttackData��null�o�Ȃ����Ƃ��ۏ؂����
	{ 
		// ��������A�N�V�������Ă���
		if (isFrameAction())
		{
			// �A�N�V�����f�[�^��null����Ȃ����ǂ�����Ԃ�
			return m_ActionDatas[(int)m_ActionState].isAttackData();
		}
		else return false;
	}
	int GetInvincibleLV(){ return m_InvincibleLV; }
	bool isActiveFrame()
	{
		if (!isFrameAction()) return false;
		return (m_ActionFrameList[m_ActionState][m_CurrentActionFrame] == FRAME_STATE::ACTIVE);
	}
	FRAME_STATE GetActionFrame()
	{
		if (!isFrameAction()) return FRAME_STATE::END;
		return m_ActionFrameList[(int)m_ActionState][m_CurrentActionFrame]; 
	}
	int GetRecoveryFrame() { return m_RecoveryFlame; }
	bool isEscape() { return m_bEscape; }
	Stand::Base *GetStand(){ return m_pStand; }
	std::list<BASE_ACTION_STATE> *GetRecoveryDamageCount(){ return &m_RecoveryDamageCount; }
	bool isDownState(){ return (m_pStateMachine->isInState(*BasePlayerState::KnockDown::GetInstance())); }

	// �Z�b�^�[
	void SetMove(const Vector3 &v) 
	{
		if (v.y > 0)
			int i = 0;
		m_move.Set(v.x, v.y, v.z);
	}
	void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }
	void SetDir(DIR dir) { m_dir = dir; }
	void SetMotion(int MotionNo) { if (m_pObj) { if (m_pObj->GetMotion() != MotionNo)m_pObj->SetMotion(MotionNo); } }
	void SetLand(bool bLand) { m_bLand = bLand; }
	void SetAerialJump(bool bAerialJump) { m_bAerialJump = bAerialJump; }
	void SetHitStopFrame(int frame) { m_HitStopFrame = frame; }
	void SetRecoveryFrame(int frame) { m_RecoveryFlame = frame; }
	void SetEscapeFlag(bool bEscape) { m_bEscape = bEscape; }
	void SetInvincible(int lv = 1){ m_InvincibleLV = lv; }
	void InvincibleOff(){ m_InvincibleLV = 0; }
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
			}
		}
	}
	//void SetStandSaveMove(const Vector3 &move){ m_StandSaveMove = move; }
	void SetRecoveryCount(int recoverycount){ m_recoveryCount = recoverycount; }

	// �A�N�Z�T�[
	void AddMove(const Vector3 &v) 
	{ 
		m_move += v;
	}
	void AddCollectScore(int score) { m_CollectScore += score; }
	void ConversionScore()
	{
		m_score += m_CollectScore;
		m_CollectScore = 0;
	}
	void AddRecoveryCount(int add){ m_recoveryCount += add; }

	void MoveClampX(float val) { m_move.x = Math::Clamp(m_move.x, -val, val); }


	// �G�t�F�N�g
	PanelEffectManager* GetPanelEffectManager() { return m_PanelEffectMGR; }
	UVEffectManager* GetUVEffectManager() { return m_UVEffectMGR; }
	void AddEffectAction(Vector3 pos,EFFECT_TYPE effectType);

	// �X�e�[�W�̂����蔻��p
	CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }


	//_/_/_/_/_/_/__/_/__/_/__/_/__
	// �萔
	//_/_/_/_/__/_/__/_/__/_/__/_/_
	static const float c_END_MOVE_LINE;		// �ړ����I����đ�C�ɖ߂�ړ��l���C��
	static const float c_FRONT_BRAKE_LINE;	// ���蒆�ɑ���𗣂��ău���[�L�Ɉڍs����܂ł̈ړ��l���C��
	static const float c_GRAVITY;			// �S�������ʂŎ����E�̏d��
	static const float c_MAX_JUMP;			// �W�����v�ő僉�C��
	static const int   c_RECOVERY_FLAME;	// ���J�o���\�t���[��
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
	bool m_bAerialJump;		// �󒆃W�����v�̃t���O
	int m_InputList[(int)PLAYER_INPUT::MAX]; 	// �����Ă���L�[���i�[
	StateMachine<BasePlayer>* m_pStateMachine; // ���X�e�[�g�}�V��
	AI*						  m_pAI;			// ��AI
	int m_InvincibleLV;			// ���G���ǂ���
	int m_InvincibleTime;		// ���G����		���G����0�ȏ�̒l������A0�ɂȂ�܂Ńf�N�������g�����
	int m_CurrentActionFrame;	// �U���t���[�����X�g�̒����Đ����Ă���t���[��
	int m_HitStopFrame;			// 1�ȏ�Ȃ�0�ɂȂ�܂ŃX�g�b�v
	int m_RecoveryFlame;		// 1�ȏ�Ȃ�0�ɂȂ�܂ő�����󂯕t���Ȃ�
	bool m_bEscape;				// �G�X�P�[�v����̎��ɗ��t���O
	bool m_bAI;					// AI���ǂ���
	int m_score;				// ���̌�̖{���̃X�R�A
	int m_CollectScore;			// ���̑O�̒��߂Ă���X�R�A
	//Vector3 m_StandSaveMove;	// �X�^���h�����̕ۑ��ړ���
	int m_recoveryCount;		// ���J�o���[�X�e�[�g�̂��鎞�Ԃ��J�E���g

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
};


