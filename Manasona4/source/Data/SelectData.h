#pragma once

#include "../BaseEntity/Message/Message.h"

//*****************************************************************************
//
//		�Z���N�g�V�[���őI�������f�[�^�p(�V���O���g��)
//
//*****************************************************************************

// �L�����N�^�[�̃^�C�v
enum class CHARACTER { AIROU, TEKI, NAZENARA, ARAMITAMA, ANIKI, END };

// �p�[�g�i�[�̃^�C�v
enum class PARTNER { MOKOI, MAYA, END };

// �X�e�[�W�̃^�C�v
enum class STAGE { SAND, SYUTEN, SEA, GARDEN, NANASATO_SITY, MAX };

// AI�̃^�C�v
enum class AI_TYPE
{
	CPU_EASY,			// �アAI
	CPU_NORMAL,			// ���ʂ�AI
	CPU_HARD,			// ����AI
	CPU_YOKOE,			// ���]����p�ɍ��ꂽ�}�W�L�`��AI

	PRACTICE_LAND,		//	[���K�p]���������Ȃ�
	PRACTICE_SQUAT,		//	[���K�p]���Ⴊ��ŉ��������Ȃ�
	PRACTICE_JUMP,		//	[���K�p]��ё�����
	PRACTICE_ATTACK,	//	[���K�p]���Ԋu�ōU������

	T_DOKKOI_ATTACK,	// [�����p]���i�U��
	T_DOWN_ATTACK,		// [�����p]���i�U��

	INTERRUPT_INVINCIBLE	// [�f�o�b�O�p]�K�[�h���ɂЂ����疳�G���荞�݂�����
};

// ���E���h���^�C�v
enum class ROUND_NUM_TYPE
{
	ROUND_1,
	ROUND_2,
	ROUND_3,
	ROUND_4,
};

// ���E���h���ԃ^�C�v
enum class ROUND_TIME_TYPE
{
	SEC_30,
	SEC_45,
	SEC_60,
	SEC_75,
	SEC_99,
	SEC_INFINITY,
};

// �����N
enum class RANK_TYPE
{
	SS, S, A, B, C
};

//+----------------------
//	�V�X�e���p
//+----------------------

// �I�[�g�Z�[�u���邩���Ȃ���
enum class AUTO_SAVE_TYPE
{
	YES,		// �I�[�g�Z�[�u����
	NO,			// �I�[�g�Z�[�u���Ȃ�
};


//+----------------------
//	�g���[�j���O�p
//+----------------------

// �����̃|�W�V����
enum class START_POSITION
{
	MIDDLE,		// �^�񒆂���
	LEFT,		// ������
	RIGHT,		// �E����
};

// �����񕜂��邩���Ȃ���
enum class HP_RECOVERY_TYPE
{
	DEFAULT,		// �ʏ�
	AUTO_RECOVERY,	// ������
};

// �p�[�g�i�[�Q�[�W
enum class PARTNER_RECOVERY_TYPE
{
	DEFAULT,		// �ʏ�
	MAX,			// MAX�Œ�
};

// �ő�_���[�W����͕�����\�����邩�ǂ���
enum class TRAINING_INFO_TYPE
{
	NONE,		// �Ȃ�
	DAMEGE,		// �_���[�W�̂�
	COMMAND,	// �R�}���h�̂�
	ALL			// �S��
};

// �_�~�[
// �G�̏��
enum class ENEMY_STATE_TYPE
{
	STAND,				// �������
	SQUAT,				// ���Ⴊ�ݏ��
	JUMP,				// �W�����v��������
	ATTACK,				// ���Ԋu�ōU��
	COM,				// �R���s���[�^
	T_DOKKOI_ATTACK,	// [�����p]���i�U��
	T_DOWN_ATTACK,		// [�����p]�������U��
};

// �G�̃K�[�h�̐ݒ�
enum class ENEMY_GUARD_TYPE
{
	NO,		// �K�[�h���Ȃ�
	WAY,	// �r������K�[�h����
	ALL		// �S���K�[�h����
};

// �G�̃K�[�h�؂�ւ��̐ݒ�
enum class ENEMY_GUARD_SWITCH_TYPE
{
	NO,		// �؂�ւ����Ȃ�
	OK,		// �؂�ւ�����
};

// �G�̎󂯐g�̐ݒ�
enum class ENEMY_TECH_TYPE
{
	NO,		// �󂯐g���Ȃ�
	LAND,	// �n��̂ݎ󂯐g������
	AIR,	// �󒆂̂ݎ󂯐g������
	ALL,	// �S��
};

// �G�̂Ȃ������̐ݒ�
enum class ENEMY_THROW_TECH_TYPE
{
	NO,		// �Ȃ��������Ȃ�
	OK		// �Ȃ���������
};


//+-------------------------
//	�L�����N�^�[�ŗL�f�[�^
//+-------------------------

// �A���~�^�}�p�f�X�N
struct AramitamaDesk
{
	int iWassyoiGauge;
};

// �A���~�^�}�̂�����傢�Q�[�W�ݒ�
enum class WASSYOI_TYPE
{
	NO,		// ����
	MAX		// ��ɍő�
};


// �g���[�j���O�p�f�[�^
struct TrainingData
{
	START_POSITION			eStartPosition;		// �����|�W�V����
	HP_RECOVERY_TYPE		eHpRecovery;		// �����񕜂��邩���Ȃ���
	int						iHp1P;				// 1P��HP
	int						iHp2P;				// 1P��HP
	int						iSpGage;			// SP�Q�[�W
	PARTNER_RECOVERY_TYPE	ePartnerRecovery;	// �p�[�g�i�[�Q�[�W
	TRAINING_INFO_TYPE		eTrainingInfo;		// �ő�_���[�W����͕�����\�����邩�ǂ���
	ENEMY_STATE_TYPE		eEnemyState;		// �G�̏��
	ENEMY_GUARD_TYPE		eEnemyGuard;		// �G�̃K�[�h�̐ݒ�
	ENEMY_GUARD_SWITCH_TYPE eEnemyGuardSwitch;	// �G�̃K�[�h�؂�ւ��̐ݒ�
	ENEMY_TECH_TYPE			eEnemyTech;			// �G�̎󂯐g�̐ݒ�
	ENEMY_THROW_TECH_TYPE	eEnemyThrowTech;	// �G�̂Ȃ������̐ݒ�
	int						iEnemyLv;			// �G��LV
	WASSYOI_TYPE			eWassyoiType;		// �A���~�^�}�̂�����傢�Q�[�W
	TrainingData();
};


//+---------------------------
//	�`���[�g���A���^�C�v
//+---------------------------
enum class TUTORIAL_TYPE
{
	WALK,
	SQUAT,
	JUMP,
	ATTACK,
	SKILL,
	OVER_DRIVE,
	RUSH,
	STAND_GUARD,
	SQUAT_GUARD,
	DUSH,
	BACK_STEP,
	PARTNER,
	INVINCIBLE,
	THROW,
	ESCAPE,
	DOKKOI,
	ANTI_AIR,
	DOWN_ATTACK,
	OD_ONEMORE,
	OD_BURST,
	ARRAY_END
};


//+-----------------------------------------
//	�`�������W���[�h�̎�ށ@�i�S�L�������ʁj
//+-----------------------------------------
enum class CHALLENGE_TYPE
{
	MISSION_1,
	MISSION_2,
	MISSION_3,
	ARRAY_END
};


struct SideData
{
	CHARACTER eCharacter;
	PARTNER ePartner;
	int iDeviceID;	// �R���g���[���[�f�o�C�X��ID
	bool bAI;		// AI�t���O
	AI_TYPE eAIType;// AI�̃^�C�v
	SideData();
	//SideData &operator=(const SideData &data) { character = data.character, partner = data.partner, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// ���E�̏��
	STAGE eStage;					// �I��ł�X�e�[�W
	bool bTutorial;					// �`���[�g���A���t���O
	bool bTraining;					// �g���[�j���O�t���O
	bool bChallenge;				// �`�������W�t���O
	int iRoundTime;					// �ꎎ������
	int iBattleMusicID;				// �퓬�Ȃ̔ԍ�
	SIDE eOrderTrainingSide;		// �g���[�j���O��I�񂾑�
	TrainingData tagTrainingDatas;	// �g���[�j���O�̃f�[�^
	int iTutorialType;				// �I�����Ă���`���[�g���A�� 
	int iChallengeType;				// �I�����Ă���`�������W
	SelectData();
	//SelectData &operator=(const SelectData &data) { tagSideDatas[(int)SIDE::LEFT] = data.tagSideDatas[(int)SIDE::LEFT], tagSideDatas[(int)SIDE::RIGHT] = data.tagSideDatas[(int)SIDE::RIGHT], stage = data.stage, bTutorial = data.bTutorial, iWinRound = data.iWinRound, iBattleMusicID = data.iBattleMusicID; return *this; }
};

class SelectDataManager
{
public:
	static SelectDataManager *GetInstance(){ static SelectDataManager i; return &i; }

	SelectData *Get() { return &m_tagData; }
private:

	SelectData m_tagData;

	SelectDataManager(){}
	SelectDataManager(const SelectDataManager&) {}
	SelectDataManager &operator=(const SelectDataManager&) {}
};

#define SelectDataMgr (SelectDataManager::GetInstance())