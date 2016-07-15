#pragma once
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
class BasePlayer;

/*
	ベースプレイヤーのステートマシンのカートリッジ
*/


namespace BasePlayerState
{

	/*******************************************************/
	//					グローバルステート
	/*******************************************************/
	class Global :public State<BasePlayer>
	{
	public:

		//this is a シングルトン
		static Global* GetInstance();

		// 入る
		virtual void Enter(BasePlayer* pPerson);

		// 実行します
		virtual void Execute(BasePlayer* pPerson);

		// 帰る
		virtual void Exit(BasePlayer* pPerson);

		// 描画
		virtual void Render(BasePlayer* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);

		int DEBUG_FLAME = 0;
	private:
		Global() {};
		~Global() {};

		Global(const Global&){}
		Global& operator=(const Global&){}
	};


	/*******************************************************/
	//					待機
	/*******************************************************/
	class Wait :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Wait* GetInstance();

		// 入る
		virtual void Enter(BasePlayer* pPerson);

		// 実行します
		virtual void Execute(BasePlayer* pPerson);

		// 帰る
		virtual void Exit(BasePlayer* pPerson);

		// 描画
		virtual void Render(BasePlayer* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Wait() {};
		~Wait() {};

		Wait(const Wait&){}
		Wait& operator=(const Wait&){}
	};


}