#include "MessageDispatcher.h"
#include "../Entity/BaseGameEntity.h"
#include "../Entity/EntityManager.h"


// シングルトン
MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;

	return &instance;
}


//　メッセージ発射
void MessageDispatcher::Discharge(BaseGameEntity* pReceiver,
	const Message& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{
		// ここに来たということは受信者側が対応してなくて弾かれた可能性がある
		printf("ID[%d] の送ったメッセージが処理されません\n", (int)telegram.Sender);
		MyDebugString("ID[%d] の送ったメッセージが処理されませんでした\n", (int)telegram.Sender);
	}
	else
	{
		// Trueなら
		printf("ID[%d] の送ったメッセージが上手く処理されました。\n", (int)telegram.Sender);
		MyDebugString("ID[%d] の送ったメッセージが上手く処理されました。\n", (int)telegram.Sender);

	}
}

// メッセージ発送
void MessageDispatcher::Dispatch(double  delay,
	ENTITY_ID    sender,
	ENTITY_ID    receiver,
	MESSAGE_TYPE    msg,
	void*  ExtraInfo)
{

	//	送信者と受信者のアドレス取得
	BaseGameEntity* pSender = EntityMgr->GetEntityFromID(sender);
	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

	//受信者のIDが有効か
	if (pReceiver == NULL)
	{
		printf("[警告]この ID[%d] を持つ受信者はいません\n",(int)receiver);
		MyDebugString("[警告]この ID[%d] を持つ受信者はいません\n",(int)receiver);
		return;
	}

	//　電報作成
	Message telegram(delay, sender, receiver, msg, ExtraInfo);

	//もし遅延がなかったらすぐに送信！                     
	if (delay <= 0.0f)
	{
		printf("ID[%d] からのメッセージを ID[%d] に送信\n", (int)sender, (int)receiver);
		MyDebugString("ID[%d] からのメッセージを ID[%d] に送信\n", (int)sender, (int)receiver);

		// 受信者に電報を送るぜ
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


