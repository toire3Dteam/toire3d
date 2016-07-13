#pragma once
#include "TDNLIB.h"
#include "../Entity/EntityEnum.h"

// ���b�Z�[�W�^�C�v
enum MESSAGE_TYPE
{
	EFFECT_CAMERA,	// �G�t�F�N�g�J�����������b�Z�[�W
};

enum class PERSON_TYPE;	// �O���錾
// EX_INFO�p�̍\����
struct RIPPLE_INFO
{
	PERSON_TYPE type;
	Vector3 pos;
	float size;
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
	int          Msg;

	// ���b�Z�[�W�̒ǉ���� (�L�����N�^�[�������� [�ǉ����] X�ɂQ�ړ� )
	// �ǉ��������O�ɒm�邱�Ƃ��s�\�Ȃ̂�Void*�Œ�
	void*        ExtraInfo;

	// ���b�Z�[�W�̒x������
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::ID_ERROR),
		Receiver(ENTITY_ID::ID_ERROR),
		Msg(-1)
	{}

	Message(double time,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		int    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};