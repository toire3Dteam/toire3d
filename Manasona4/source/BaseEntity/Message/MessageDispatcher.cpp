#include "MessageDispatcher.h"
#include "../Entity/BaseGameEntity.h"
#include "../Entity/EntityManager.h"


// �V���O���g��
MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;

	return &instance;
}


//�@���b�Z�[�W����
void MessageDispatcher::Discharge(BaseGameEntity* pReceiver,
	const Message& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{
		// �����ɗ����Ƃ������Ƃ͎�M�ґ����Ή����ĂȂ��Ēe���ꂽ�\��������
		printf("ID[%d] �̑��������b�Z�[�W����������܂���\n", (int)telegram.Sender);
		MyDebugString("ID[%d] �̑��������b�Z�[�W����������܂���ł���\n", (int)telegram.Sender);
	}
	else
	{
		// True�Ȃ�
		printf("ID[%d] �̑��������b�Z�[�W����肭��������܂����B\n", (int)telegram.Sender);
		MyDebugString("ID[%d] �̑��������b�Z�[�W����肭��������܂����B\n", (int)telegram.Sender);

	}
}

// ���b�Z�[�W����
void MessageDispatcher::Dispatch(double  delay,
	ENTITY_ID    sender,
	ENTITY_ID    receiver,
	MESSAGE_TYPE    msg,
	void*  ExtraInfo)
{

	//	���M�҂Ǝ�M�҂̃A�h���X�擾
	BaseGameEntity* pSender = EntityMgr->GetEntityFromID(sender);
	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

	//��M�҂�ID���L����
	if (pReceiver == NULL)
	{
		printf("[�x��]���� ID[%d] ������M�҂͂��܂���\n",(int)receiver);
		MyDebugString("[�x��]���� ID[%d] ������M�҂͂��܂���\n",(int)receiver);
		return;
	}

	//�@�d��쐬
	Message telegram(delay, sender, receiver, msg, ExtraInfo);

	//�����x�����Ȃ������炷���ɑ��M�I                     
	if (delay <= 0.0f)
	{
		printf("ID[%d] ����̃��b�Z�[�W�� ID[%d] �ɑ��M\n", (int)sender, (int)receiver);
		MyDebugString("ID[%d] ����̃��b�Z�[�W�� ID[%d] �ɑ��M\n", (int)sender, (int)receiver);

		// ��M�҂ɓd��𑗂邺
		Discharge(pReceiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	//else
	//{
	//	double CurrentTime = Clock->GetCurrentTime();

	//	telegram.DispatchTime = CurrentTime + delay;

	//	//and put it in the queue
	//	PriorityQ.insert(telegram);

	//	cout << "\nDelayed telegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time "
	//		<< Clock->GetCurrentTime() << " for " << GetNameOfEntity(pReceiver->ID())
	//		<< ". Msg is " << MsgToStr(msg);

	//}
}


void MessageDispatcher::DispatchDelayed()
{


	////get current time
	//double CurrentTime = Clock->GetCurrentTime();

	////now peek at the queue to see if any telegrams need dispatching.
	////remove all telegrams from the front of the queue that have gone
	////past their sell by date
	//while (!PriorityQ.empty() &&
	//	(PriorityQ.begin()->DispatchTime < CurrentTime) &&
	//	(PriorityQ.begin()->DispatchTime > 0))
	//{
	//	//read the telegram from the front of the queue
	//	const Telegram& telegram = *PriorityQ.begin();

	//	//find the recipient
	//	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

	//	cout << "\nQueued telegram ready for dispatch: Sent to "
	//		<< GetNameOfEntity(pReceiver->ID()) << ". Msg is " << MsgToStr(telegram.Msg);

	//	//send the telegram to the recipient
	//	Discharge(pReceiver, telegram);

	//	//remove it from the queue
	//	PriorityQ.erase(PriorityQ.begin());
	//}
}


