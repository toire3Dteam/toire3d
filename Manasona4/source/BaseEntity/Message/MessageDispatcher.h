#pragma once
#include "Message.h"
#include "../Entity/EntityEnum.h"
#include <set>

/*
	Telegramの作成・送信・管理はこのクラスで処理する。
	誰かがメッセージを送る必要があるときはこのインスタンスを使う。
	メッセージをすぐに送信するor時間を遅らすためキューに入れておくの二択

	メッセージを送信する前にこのクラスは送信者のエンティティのポインタが必要
	インスタンス化されたエージェントのポインタが参照できるマネージャーが欲しい
	(EntityManager)
*/

class BaseGameEntity;

// enum的な
const int	MSG_NO_DELAY = 0;
const int   MSG_NO_EXINFO= 0;

class MessageDispatcher
{
public:

	static MessageDispatcher* Instance();

	//  メッセージ発送
	void Dispatch(int  delay,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		MESSAGE_TYPE    msg,
		void*  ExtraInfo);

	// 遅延メッセージ用
	void DispatchDelayed();

private:
	// std::setは遅延メッセージのコンテナとして使う
	// 自動的にソートし重複を避けるメリットがある。
	// メッセージ送信時刻でソートされる
	std::set<Message> PriorityQ;// 優先

	//　メッセージ発射
	void Discharge(BaseGameEntity* pReceiver, const Message& msg);


	// 封印
	MessageDispatcher() {};
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);
};

#define MsgMgr MessageDispatcher::Instance()