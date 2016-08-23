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
	//				待機ステート
	/*******************************************************/
	class Wait :public State<AI>
	{
	public:

		//this is a シングルトン
		static Wait* GetInstance();

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
		Wait() {};
		~Wait() {};

		Wait(const Wait&){}
		Wait& operator=(const Wait&){}
	};

	/*******************************************************/
	//		感覚を空けるステート　(無敵からの逃げ)
	/*******************************************************/
	class Space :public State<AI>
	{
	public:

		//this is a シングルトン
		static Space* GetInstance();

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
		Space() {};
		~Space() {};

		Space(const Space&){}
		Space& operator=(const Space&){}
	};

	/*******************************************************/
	//		飛ぶステート　特に意味はないけど作ってみた
	/*******************************************************/
	class SimpleJump :public State<AI>
	{
	public:

		//this is a シングルトン
		static SimpleJump* GetInstance();

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
		SimpleJump() {};
		~SimpleJump() {};

		SimpleJump(const SimpleJump&){}
		SimpleJump& operator=(const SimpleJump&){}
	};

	/*******************************************************/
	//		回避ステート　
	/*******************************************************/
	class Escape :public State<AI>
	{
	public:

		//this is a シングルトン
		static Escape* GetInstance();

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
		Escape() {};
		~Escape() {};

		Escape(const Escape&){}
		Escape& operator=(const Escape&){}
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
	//			ジャンプして空中で敵を追いかけるステート
	/*******************************************************/
	class ChaseAir :public State<AI>
	{
	public:

		//this is a シングルトン
		static ChaseAir* GetInstance();

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
		ChaseAir() {};
		~ChaseAir() {};

		ChaseAir(const ChaseAir&){}
		ChaseAir& operator=(const ChaseAir&){}
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


	/*******************************************************/
	//				空中攻撃 準備ステート
	/*******************************************************/
	class SetAerialAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static SetAerialAtack* GetInstance();

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
		SetAerialAtack() {};
		~SetAerialAtack() {};

		SetAerialAtack(const SetAerialAtack&){}
		SetAerialAtack& operator=(const SetAerialAtack&){}
	};

	/*******************************************************/
	//				空中攻撃 ステート
	/*******************************************************/
	class AerialAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static AerialAtack* GetInstance();

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
		AerialAtack() {};
		~AerialAtack() {};

		AerialAtack(const AerialAtack&){}
		AerialAtack& operator=(const AerialAtack&){}
	};

	
	/*******************************************************/
	//				空中下攻撃 ステート
	/*******************************************************/
	class AerialDropAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static AerialDropAtack* GetInstance();

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
		AerialDropAtack() {};
		~AerialDropAtack() {};

		AerialDropAtack(const AerialDropAtack&){}
		AerialDropAtack& operator=(const AerialDropAtack&){}
	};

	/*******************************************************/
	//				フィニッシュ攻撃ステート
	/*******************************************************/
	class FinishAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static FinishAtack* GetInstance();

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
		FinishAtack() {};
		~FinishAtack() {};

		FinishAtack(const FinishAtack&){}
		FinishAtack& operator=(const FinishAtack&){}
	};


	/*******************************************************/
	//			キャラクター固有の攻撃系スキル発動ステート
	/*******************************************************/
	class SkillAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static SkillAtack* GetInstance();

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
		SkillAtack() {};
		~SkillAtack() {};

		SkillAtack(const SkillAtack&){}
		SkillAtack& operator=(const SkillAtack&){}
	};
}



