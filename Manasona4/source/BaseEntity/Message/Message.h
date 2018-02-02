#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// ���b�Z�[�W�^�C�v
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,		// �G�t�F�N�g�J�����������b�Z�[�W
	WINNER_CAMERA,
	SHAKE_CAMERA,		// �J�����U�����b�Z�[�W
	HIT_ATTACK,			// �U�����q�b�g�������l�ɑ��郁�b�Z�[�W�I
	HIT_DAMAGE,			// �U�����q�b�g��������ɑ��郁�b�Z�[�W
	FALL,				// �v���C���[���n�ʂɂ��Ă��Ȃ��Ƃ��ɑ����郁�b�Z�[�W
	//THROW_SUCCESS,		// �͂񂾐l�ɑ��郁�b�Z�[�W
	BE_THROWN,			// �͂܂ꂽ����ɑ��郁�b�Z�[�W(���͂񂾑����ID�𑗂��Ă�����)
	CAN_THROW_RELEASE,	// �͂܂ꂽ�l�������ʂ��R�}���h����͂����Ƃ��ɒ͂񂾂�ɑ΂��đ��郁�b�Z�[�W
	THROW_RELEASE,		// �����ʂ����Ă�����Ƒ��郁�b�Z�[�W
	HEAVE_HO_OVER_DRIVE_HIT,	// �q�[�z�[�t�B�j�b�V���q�b�g
	PERSONA_CARD_COUNT_UP,// �y���\�i�J�[�h��������
	PERSONA_CARD_COUNT_DOWN,// �y���\�i�J�[�h��������
	APP_WIN_ICON,// �������A�C�R���_��
	CUTIN_ACTION,// �J�b�g�C�������I
	HEAVE_HO_OVERFLOW_START,	// �q�[�z�[�I�[�o�[�t���[�J�n�I
	END_ROUNDCALL,
	END_FINISHCALL,
	END_WINNER,			// ���҂̉��o�I��
	OVER_DRIVE_STAGE,	// �I�[�o�[�h���C�u�̃X�e�[�W
	KO,					// HP���[���ɂȂ����Ƃ��ɑ��M
	OVER_DRIVE_CUTIN,   // �K�E�Z�̃J�b�g�C��
	KAKEAI_END,			// �|�������I��
	COMBO_COUNT,		// �R���{�J�E���g
	COMBO_GUARD,		// �R���{�K�[�h���ꂽ��
	ADD_SHOT,			// �V���b�g�̎��̂��i�[���Ă�
	ERASE_SIDE_SHOT,	// �Е��̏o�����V���b�g������
	BACK_MENU,			// ���j���[��ʂɂ��ǂ肽���ł��B
	TROPHY_GET,			// �g���t�B�[�Q�b�g
	PICTURE_GET,		// �C���X�g�Q�b�g
	MOVIE_GET,			// ���[�r�[�Q�b�g
	OTHER				// ���̑��B

};


// �L�����N�^�[�T�C�h
enum class SIDE
{
	LEFT,
	RIGHT,
	ARRAY_MAX
};

// �R���{�p
struct COMBO_DESK
{
	SIDE side;
	int damage;
	int recoveryFrame;
	bool bCounter;
	COMBO_DESK() :side(SIDE::LEFT), damage(0), recoveryFrame(0), bCounter(false) {}
};

// KO�̃^�C�v
enum class FINISH_TYPE
{
	NORMAL,
	OVER_DRIVE
};

// KO�����Ƃ��ɑ��郁�b�Z�[�W�̍\����
struct KO_INFO
{
	FINISH_TYPE FinishType;		// KO�̃^�C�v
	int iNumWinnerRound;		// KO������̏�������(���E���h�ݒ肵������������烊�U���g�ɍs��)
	SIDE WinnerSide;			// KO������̃T�C�h
	SIDE LoseSide;				// KO���ꂽ��̃T�C�h
};

enum class DAMAGE_MOTION
{
	KNOCK_BACK,	// �̂�����
	KNOCK_DOWN,	// �������
	DOWN_FALL,	// ���������炢
};

// �U������������p
struct HIT_DAMAGE_INFO
{
	bool BeInvincible;	// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
	int damage;			// �󂯂�_���\�W(�X�R�A)
	Vector2 FlyVector;	// ������уx�N�g��(����������)
	int iHitStopFrame;		// �q�b�g�X�g�b�v�t���[��
	int HitRecoveryFrame;	// �q�b�g�d���t���[��
	int GuardRecoveryFrame;	// �K�[�h�d���t���[��
	int HitEffectType;		// HIT��������ɑ���G�t�F�N�g (A���)����int����Ȃ���EFFECT_TYPE�ɂ������B�ǂ��������̂��E�E
	bool bOverDrive;		// �t�B�j�b�V���A�^�b�N
	int iAttackType;		// ���̍U���𓖂Ă�ꂽ��(����Ȃ��U���������炻�̕��x������������)
	int iAntiGuard;			// �K�[�h�˂��j��U���̎��
	LPCSTR HitSE;			// �q�b�g����SE��ID
	bool bFinishOK;			// �t�B�j�b�V������U����
	float fGuardKnockBackPower;	// �K�[�h�̂�����̗�
	DAMAGE_MOTION DamageMotion;	// ���������[�V�����^�C�v
	float fComboRate;		// �R���{�␳
	int iAttribute;			// ����
	float fRepeatAttackRate;// ���Z�␳�l(�Z���ƂɎ����Ƃɂ���)
	bool bStandAttack;		// �X�^���h�U�����ǂ���
	HIT_DAMAGE_INFO() { ZeroMemory(this, sizeof(this)); bStandAttack = false; }
};

// �U���q�b�g�����v���C���[�p
struct HIT_ATTACK_INFO
{
	int iHitStopFrame;		// �q�b�g�X�g�b�v�t���[��
	int HitScore;			// ���Z�����X�R�A
	bool bOverDrive;		// �t�B�j�b�V���A�^�b�N
	bool bFirstAttack;		// (12/27)�ŏ��̈ꌂ���ǂ���
};

// �v���C���[Info
struct PLAYER_INFO
{
	Vector3 pos;
	ENTITY_ID id;
};

/* �G�t�F�N�g�J�����������ɑ��鎞�̍\���� */
struct EFFECT_CAMERA_INFO
{
	int scriptID;			// �J�����X�N���v�g��ID
	Vector3 *pOrgPos;		// �G�t�F�N�g�J�����̌��_�̍��W(nullptr�Ȃ�Vector3(0,0,0))
	EFFECT_CAMERA_INFO() :scriptID(0), pOrgPos(nullptr){}
};

/* �J�����U�����ɑ��鎞�̍\���� */
struct SHAKE_CAMERA_INFO
{
	float ShakePower;	// �U���̗�(0�`1)
	int ShakeFrame;			// �h��Ă鎞��(�O�̓�������3�ɂ��Ă�)
	void Set(float power, int frame){ ShakePower = power; ShakeFrame = frame; }
};


/*
	���b�Z�[�W�̓V���v���ȗ񋓌^�B
	���e�͂Ȃ�ł�����B
	
	�E�N��
	�E�N��
	�E���b�Z�[�W�̗�
	�E���b�Z�[�W�̒ǉ����(Void*)
	�E�f�B���C�p
*/

/*�d��\����*/
struct Message
{
	// ����Telegram�𑗂����G���e�B�e�B
	ENTITY_ID          Sender;

	// ����Telegram���󂯎��G���e�B�e�B
	ENTITY_ID          Receiver;

	// ���b�Z�[�W�̓��e�B�@�i�񋓌^�ɐݒ肳��Ă�����́j
	MESSAGE_TYPE          Msg;

	// ���b�Z�[�W�̒ǉ���� (�L�����N�^�[�������� [�ǉ����] X�ɂQ�ړ� )
	// �ǉ��������O�ɒm�邱�Ƃ��s�\�Ȃ̂�Void*�Œ�
	void*        ExtraInfo;

	// ���b�Z�[�W�̒x������
	int       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::ID_ERROR),
		Receiver(ENTITY_ID::ID_ERROR),
		Msg(MESSAGE_TYPE::OTHER)
	{}

	Message(int time,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		MESSAGE_TYPE    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};