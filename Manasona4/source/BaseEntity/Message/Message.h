#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// ���b�Z�[�W�^�C�v
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,		// �G�t�F�N�g�J�����������b�Z�[�W
	SHAKE_CAMERA,		// �J�����U�����b�Z�[�W
	HIT_ATTACK,			// �U�����q�b�g�������l�ɑ��郁�b�Z�[�W�I
	HIT_DAMAGE,			// �U�����q�b�g��������ɑ��郁�b�Z�[�W
	FALL,				// �v���C���[���n�ʂɂ��Ă��Ȃ��Ƃ��ɑ����郁�b�Z�[�W
	THROW_SUCCESS,		// �͂񂾐l�ɑ��郁�b�Z�[�W
	BE_THROWN,			// �͂܂ꂽ����ɑ��郁�b�Z�[�W(���͂񂾑����ID�𑗂��Ă�����)
	CAN_THROW_RELEASE,	// �͂܂ꂽ�l�������ʂ��R�}���h����͂����Ƃ��ɒ͂񂾂�ɑ΂��đ��郁�b�Z�[�W
	THROW_RELEASE,		// �����ʂ����Ă�����Ƒ��郁�b�Z�[�W
	HEAVE_HO_OVER_DRIVE_HIT,	// �q�[�z�[�t�B�j�b�V���q�b�g
	PERSONA_CARD_COUNT_UP,// �y���\�i�J�[�h��������
	PERSONA_CARD_COUNT_DOWN,// �y���\�i�J�[�h��������
	OTHER				// ���̑��B
};

// �U������������p
struct HIT_DAMAGE_INFO
{
	bool BeInvincible;	// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
	int damage;			// �󂯂�_���\�W(�X�R�A)
	Vector2 FlyVector;	// ������уx�N�g��(����������)
	int hitStopFlame;		// �q�b�g�X�g�b�v�t���[��
	int recoveryFlame;		// �d���t���[��
	int HitEffectType;		// HIT��������ɑ���G�t�F�N�g (A���)����int����Ȃ���EFFECT_TYPE�ɂ������B�ǂ��������̂��E�E
	bool bFinishAttack;		// �t�B�j�b�V���A�^�b�N
	int iAttackType;		// ���̍U���𓖂Ă�ꂽ��(����Ȃ��U���������炻�̕��x������������)
};

// �U���q�b�g�����v���C���[�p
struct HIT_ATTACK_INFO
{
	int HitPlayerDeviceID;	// �_���[�W��^��������̔ԍ�
	int hitStopFlame;		// �q�b�g�X�g�b�v�t���[��
	int HitScore;			// ���Z�����X�R�A
	bool bFinishAttack;		// �t�B�j�b�V���A�^�b�N
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
	int ShakeFrame;			// �h��Ă鎞��(�O�̐^�Ă���3�ɂ��Ă�)
	void Set(float power, int frame){ ShakePower = power; ShakeFrame = frame; }
};

/* �͂܂ꂽ����ɑ�����\���� */
struct BE_THROWN_INFO
{
	ENTITY_ID ThrowPlayerID;	// ���񂾂��ID
	int iThrowPlayerDir;		// ���񂾂�̌���(�󂯂����͂���̔��΂�����)
};

// �`�[��
enum class TEAM
{
	A,
	B
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
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::ID_ERROR),
		Receiver(ENTITY_ID::ID_ERROR),
		Msg(MESSAGE_TYPE::OTHER)
	{}

	Message(double time,
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