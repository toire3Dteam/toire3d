#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// ���b�Z�[�W�^�C�v
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,	// �G�t�F�N�g�J�����������b�Z�[�W
	HIT_ATTACK,		// �U�����q�b�g�������l�ɑ��郁�b�Z�[�W�I
	HIT_DAMAGE,		// �U�����q�b�g��������ɑ��郁�b�Z�[�W
	OTHER,			// ���̑��B
};

// �U������������p
struct HIT_DAMAGE_INFO
{
	bool BeInvincible;	// ���G�ɂȂ邩�ǂ���(���Ƃ��΁A�ʏ��1.2�i�ڂȂ�false�����A3�i�ڂȂ�true)
	int damage;			// �󂯂�_���\�W(�X�R�A)
	Vector2 FlyVector;	// ������уx�N�g��(����������)
};

// �U���q�b�g�����v���C���[�p
struct HIT_ATTACK_INFO
{
	int HitPlayerDeviceID;	// �_���[�W��^��������̔ԍ�
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

// �C���g���������b�Z�[�W���M�̂Ƃ��ɂ����\����
struct INTRO_CAMERA_INFO
{
	Vector3 start_person_pos;	// ���킳�𗬂��l�Ԃ̍��W
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