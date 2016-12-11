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
	//					プラクティスグローバルステート
	/*******************************************************/
	class PracticeGlobal :public State<AI>
	{
	public:

		//this is a シングルトン
		static PracticeGlobal* GetInstance();

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
		PracticeGlobal() {};
		~PracticeGlobal() {};

		PracticeGlobal(const PracticeGlobal&){}
		PracticeGlobal& operator=(const PracticeGlobal&){}
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
	//					練習用地面ステート
	/*******************************************************/
	class PracticeLand :public State<AI>
	{
	public:

		//this is a シングルトン
		static PracticeLand* GetInstance();

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
		PracticeLand() {};
		~PracticeLand() {};

		PracticeLand(const PracticeLand&){}
		PracticeLand& operator=(const PracticeLand&){}
	};

	/*******************************************************/
	//					練習用攻撃ステート
	/*******************************************************/
	class PracticeAttack :public State<AI>
	{
	public:

		//this is a シングルトン
		static PracticeAttack* GetInstance();

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
		PracticeAttack() {};
		~PracticeAttack() {};

		PracticeAttack(const PracticeAttack&) {}
		PracticeAttack& operator=(const PracticeAttack&) {}
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
	//		ガードステート　
	/*******************************************************/
	class Guard :public State<AI>
	{
	public:

		//this is a シングルトン
		static Guard* GetInstance();

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
		Guard() {};
		~Guard() {};

		Guard(const Guard&){}
		Guard& operator=(const Guard&){}
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
	//				掴まれたステート
	/*******************************************************/
	class ThrowTech:public State<AI>
	{
	public:

		//this is a シングルトン
		static ThrowTech* GetInstance();

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
		ThrowTech() {};
		~ThrowTech() {};

		ThrowTech(const ThrowTech&) {}
		ThrowTech& operator=(const ThrowTech&) {}
	};


	/*******************************************************/
	//				投げ準備ステート
	/*******************************************************/
	class SetThrow :public State<AI>
	{
	public:

		//this is a シングルトン
		static SetThrow* GetInstance();

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
		SetThrow() {};
		~SetThrow() {};

		SetThrow(const SetThrow&) {}
		SetThrow& operator=(const SetThrow&) {}
	};

	/*******************************************************/
	//				投げステート
	/*******************************************************/
	class Throw :public State<AI>
	{
	public:

		//this is a シングルトン
		static Throw* GetInstance();

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
		Throw() {};
		~Throw() {};

		Throw(const Throw&) {}
		Throw& operator=(const Throw&) {}
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
	//				空中崩し攻撃 ステート　（空中で地上の相手を崩すための攻撃）
	/*******************************************************/
	class AerialDestructAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static AerialDestructAtack* GetInstance();

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
		AerialDestructAtack() {};
		~AerialDestructAtack() {};

		AerialDestructAtack(const AerialDestructAtack&){}
		AerialDestructAtack& operator=(const AerialDestructAtack&){}
	};

	/*******************************************************/
	//				無敵攻撃ステート
	/*******************************************************/
	class InvincibleAtack :public State<AI>
	{
	public:

		//this is a シングルトン
		static InvincibleAtack* GetInstance();

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
		InvincibleAtack() {};
		~InvincibleAtack() {};

		InvincibleAtack(const InvincibleAtack&){}
		InvincibleAtack& operator=(const InvincibleAtack&){}
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



