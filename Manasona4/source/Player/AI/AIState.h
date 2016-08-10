#pragma once
#include "AI.h"

// 前方宣言
class AI;

namespace AIState
{
	/*******************************************************/
	//					グローバルステート
	/*******************************************************/
	class Global :public State<AI>
	{
	public:

		//this is a シングルトン
		static Global* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Global() {};
		~Global() {};

		Global(const Global&){}
		Global& operator=(const Global&){}
	};



	/*******************************************************/
	//					練習用ジャンプステート
	/*******************************************************/
	class PracticeJump :public State<AI>
	{
	public:

		//this is a シングルトン
		static PracticeJump* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeJump() {};
		~PracticeJump() {};

		PracticeJump(const PracticeJump&){}
		PracticeJump& operator=(const PracticeJump&){}
	};


	/*******************************************************/
	//				敵を追いかけるステート
	/*******************************************************/
	class Chase :public State<AI>
	{
	public:

		//this is a シングルトン
		static Chase* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Chase() {};
		~Chase() {};

		Chase(const Chase&){}
		Chase& operator=(const Chase&){}
	};

	/*******************************************************/
	//				ラッシュ攻撃ステート
	/*******************************************************/
	class RushAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static RushAtack* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		RushAtack() {};
		~RushAtack() {};

		RushAtack(const RushAtack&){}
		RushAtack& operator=(const RushAtack&){}
	};

	/*******************************************************/
	//				復帰ステート
	/*******************************************************/
	class Recovery :public State<AI>
	{
	public:

		//this is a シングルトン
		static Recovery* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Recovery() {};
		~Recovery() {};

		Recovery(const Recovery&){}
		Recovery& operator=(const Recovery&){}
	};


	/*******************************************************/
	//				対空準備ステート
	/*******************************************************/
	class SetAnitiAir :public State<AI>
	{
	public:

		//this is a シングルトン
		static SetAnitiAir* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SetAnitiAir() {};
		~SetAnitiAir() {};

		SetAnitiAir(const SetAnitiAir&){}
		SetAnitiAir& operator=(const SetAnitiAir&){}
	};

	/*******************************************************/
	//				対空ステート
	/*******************************************************/
	class AnitiAir :public State<AI>
	{
	public:

		//this is a シングルトン
		static AnitiAir* GetInstance();

		// 入る
		virtual void Enter(AI* pPerson);

		// 実行します
		virtual void Execute(AI* pPerson);

		// 帰る
		virtual void Exit(AI* pPerson);

		// 描画
		virtual void Render(AI* pPerson);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		AnitiAir() {};
		~AnitiAir() {};

		AnitiAir(const AnitiAir&){}
		AnitiAir& operator=(const AnitiAir&){}
	};
}



