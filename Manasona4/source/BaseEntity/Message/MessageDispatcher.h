#pragma once
#include "Message.h"
#include "../Entity/EntityEnum.h"
#include <set>

/*
	Telegram�̍쐬�E���M�E�Ǘ��͂��̃N���X�ŏ�������B
	�N�������b�Z�[�W�𑗂�K�v������Ƃ��͂��̃C���X�^���X���g���B
	���b�Z�[�W�������ɑ��M����or���Ԃ�x�炷���߃L���[�ɓ���Ă����̓��

	���b�Z�[�W�𑗐M����O�ɂ��̃N���X�͑��M�҂̃G���e�B�e�B�̃|�C���^���K�v
	�C���X�^���X�����ꂽ�G�[�W�F���g�̃|�C���^���Q�Ƃł���}�l�[�W���[���~����
	(EntityManager)
*/

class BaseGameEntity;

// enum�I��
const int	MSG_NO_DELAY = 0;
const int   MSG_NO_EXINFO= 0;

class MessageDispatcher
{
public:

	static MessageDispatcher* Instance();

	//  ���b�Z�[�W����
	void Dispatch(int  delay,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		MESSAGE_TYPE    msg,
		void*  ExtraInfo);

	// �x�����b�Z�[�W�p
	void DispatchDelayed();

private:
	// std::set�͒x�����b�Z�[�W�̃R���e�i�Ƃ��Ďg��
	// �����I�Ƀ\�[�g���d��������郁���b�g������B
	// ���b�Z�[�W���M�����Ń\�[�g�����
	std::set<Message> PriorityQ;// �D��

	//�@���b�Z�[�W����
	void Discharge(BaseGameEntity* pReceiver, const Message& msg);


	// ����
	MessageDispatcher() {};
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);
};

#define MsgMgr MessageDispatcher::Instance()