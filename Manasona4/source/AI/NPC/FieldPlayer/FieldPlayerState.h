#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
//#include "AI\Message\Message.h"
//#include "AI\Message\MessageDispatcher.h"

class FieldPlayer;

/*

人のステートマシンのカートリッジ

*/


//-------------------グローバルステートAI
class FieldPlayer_GlobalState_AI :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_GlobalState_AI* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

	int DEBUG_FLAME = 0;
private:
	FieldPlayer_GlobalState_AI() {};
	~FieldPlayer_GlobalState_AI() {};

	FieldPlayer_GlobalState_AI(const FieldPlayer_GlobalState_AI&);
	FieldPlayer_GlobalState_AI& operator=(const FieldPlayer_GlobalState_AI&);
};



//--------------------待機-AI
class FieldPlayer_Wait_AI :public State<FieldPlayer>
{
public:

	// this is a シングルトン
	static FieldPlayer_Wait_AI* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);



	int m_MoveFlame;
private:
	FieldPlayer_Wait_AI() {};
	~FieldPlayer_Wait_AI() {};

	FieldPlayer_Wait_AI(const FieldPlayer_Wait_AI&);
	FieldPlayer_Wait_AI& operator=(const FieldPlayer_Wait_AI&);
};

//--------------------走る-AI
class FieldPlayer_Run_AI :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Run_AI* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Run_AI() {};
	~FieldPlayer_Run_AI() {};

	FieldPlayer_Run_AI(const FieldPlayer_Run_AI&);
	FieldPlayer_Run_AI& operator=(const FieldPlayer_Run_AI&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------ジャンプ-AI
class FieldPlayer_Jump_AI :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Jump_AI* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Jump_AI() {};
	~FieldPlayer_Jump_AI() {};

	FieldPlayer_Jump_AI(const FieldPlayer_Jump_AI&);
	FieldPlayer_Jump_AI& operator=(const FieldPlayer_Jump_AI&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------持つ-AI
class FieldPlayer_Have_AI:public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Have_AI* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Have_AI() {};
	~FieldPlayer_Have_AI() {};

	FieldPlayer_Have_AI(const FieldPlayer_Have_AI&);
	FieldPlayer_Have_AI& operator=(const FieldPlayer_Have_AI&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------投げる-AI
class FieldPlayer_throw_AI :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_throw_AI* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_throw_AI() {};
	~FieldPlayer_throw_AI() {};

	FieldPlayer_throw_AI(const FieldPlayer_throw_AI&);
	FieldPlayer_throw_AI& operator=(const FieldPlayer_throw_AI&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};



//-------------------グローバルステート
class FieldPlayer_GlobalState :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_GlobalState* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_GlobalState() {};
	~FieldPlayer_GlobalState() {};

	FieldPlayer_GlobalState(const FieldPlayer_GlobalState&);
	FieldPlayer_GlobalState& operator=(const FieldPlayer_GlobalState&);
};

//--------------------待機
class FieldPlayer_Wait :public State<FieldPlayer>
{
public:

	// this is a シングルトン
	static FieldPlayer_Wait* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Wait() {};
	~FieldPlayer_Wait() {};

	FieldPlayer_Wait(const FieldPlayer_Wait&);
	FieldPlayer_Wait& operator=(const FieldPlayer_Wait&);
};

//--------------------走る
class FieldPlayer_Run :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Run* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Run() {};
	~FieldPlayer_Run() {};

	FieldPlayer_Run(const FieldPlayer_Run&);
	FieldPlayer_Run& operator=(const FieldPlayer_Run&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------ジャンプ
class FieldPlayer_Jump :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Jump* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Jump() {};
	~FieldPlayer_Jump() {};

	FieldPlayer_Jump(const FieldPlayer_Jump&);
	FieldPlayer_Jump& operator=(const FieldPlayer_Jump&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------持つ
class FieldPlayer_Have :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Have* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Have() {};
	~FieldPlayer_Have() {};

	FieldPlayer_Have(const FieldPlayer_Have&);
	FieldPlayer_Have& operator=(const FieldPlayer_Have&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------投げる
class FieldPlayer_throw:public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_throw* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_throw() {};
	~FieldPlayer_throw() {};

	FieldPlayer_throw(const FieldPlayer_throw&);
	FieldPlayer_throw& operator=(const FieldPlayer_throw&);


	// この時だけの変数
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};


//--------------------死ぬ
class FieldPlayer_Dead :public State<FieldPlayer>
{
public:

	//this is a シングルトン
	static FieldPlayer_Dead* GetInstance();

	// 入る
	virtual void Enter(FieldPlayer* pPerson);

	// 実行します
	virtual void Execute(FieldPlayer* pPerson);

	// 帰る
	virtual void Exit(FieldPlayer* pPerson);

	// 描画
	virtual void Render(FieldPlayer* pPerson);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Dead() {};
	~FieldPlayer_Dead() {};

	FieldPlayer_Dead(const FieldPlayer_Dead&);
	FieldPlayer_Dead& operator=(const FieldPlayer_Dead&);

	int DeadFlame;

};


