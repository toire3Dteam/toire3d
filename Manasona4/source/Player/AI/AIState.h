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

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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
	//					練習用地面しゃがみステート
	/*******************************************************/
	class PracticeSquat :public State<AI>
	{
	public:

		//実体_
		static PracticeSquat* GetInstance();

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
		PracticeSquat() {};
		~PracticeSquat() {};

		PracticeSquat(const PracticeSquat&) {}
		PracticeSquat& operator=(const PracticeSquat&) {}
	};

	/*******************************************************/
	//					練習用攻撃ステート
	/*******************************************************/
	class PracticeAttack :public State<AI>
	{
	public:

		//実体_
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
	//					練習用ガードステート
	/*******************************************************/
	class PracticeGuard :public State<AI>
	{
	public:

		//実体_
		static PracticeGuard* GetInstance();

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
		PracticeGuard() {};
		~PracticeGuard() {};

		PracticeGuard(const PracticeGuard&) {}
		PracticeGuard& operator=(const PracticeGuard&) {}
	};


	/*******************************************************/
	//					練習用中段攻撃ステート
	/*******************************************************/
	class PracticeDokkoiAttack:public State<AI>
	{
	public:

		//実体_
		static PracticeDokkoiAttack* GetInstance();

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
		PracticeDokkoiAttack() {};
		~PracticeDokkoiAttack() {};

		PracticeDokkoiAttack(const PracticeDokkoiAttack&) {}
		PracticeDokkoiAttack& operator=(const PracticeDokkoiAttack&) {}
	};

	/*******************************************************/
	//					練習用足払い攻撃ステート
	/*******************************************************/
	class PracticeDownAttack :public State<AI>
	{
	public:

		//実体_
		static PracticeDownAttack* GetInstance();

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
		PracticeDownAttack() {};
		~PracticeDownAttack() {};

		PracticeDownAttack(const PracticeDownAttack&) {}
		PracticeDownAttack& operator=(const PracticeDownAttack&) {}
	};


	/*******************************************************/
	//					無敵割り込み用ステート
	/*******************************************************/
	class InterruptInvincible :public State<AI>
	{
	public:

		//実体_
		static InterruptInvincible* GetInstance();

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
		InterruptInvincible() {};
		~InterruptInvincible() {};

		InterruptInvincible(const InterruptInvincible&) {}
		InterruptInvincible& operator=(const InterruptInvincible&) {}
	};

	/*******************************************************/
	//				待機ステート
	/*******************************************************/
	class Wait :public State<AI>
	{
	public:

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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
	class RushAttack :public State<AI>
	{
	public:

		//実体_
		static RushAttack* GetInstance();

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
		RushAttack() {};
		~RushAttack() {};

		RushAttack(const RushAttack&){}
		RushAttack& operator=(const RushAttack&){}
	};

	/*******************************************************/
	//				復帰ステート
	/*******************************************************/
	class Recovery :public State<AI>
	{
	public:

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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

		//実体_
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
	class SetAerialAttack :public State<AI>
	{
	public:

		//実体_
		static SetAerialAttack* GetInstance();

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
		SetAerialAttack() {};
		~SetAerialAttack() {};

		SetAerialAttack(const SetAerialAttack&){}
		SetAerialAttack& operator=(const SetAerialAttack&){}
	};

	/*******************************************************/
	//				空中攻撃 ステート
	/*******************************************************/
	class AerialAttack :public State<AI>
	{
	public:

		//実体_
		static AerialAttack* GetInstance();

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
		AerialAttack() {};
		~AerialAttack() {};

		AerialAttack(const AerialAttack&){}
		AerialAttack& operator=(const AerialAttack&){}
	};

	
	/*******************************************************/
	//				空中崩し攻撃 ステート　（空中で地上の相手を崩すための攻撃）
	/*******************************************************/
	class AerialDestructAttack :public State<AI>
	{
	public:

		//実体_
		static AerialDestructAttack* GetInstance();

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
		AerialDestructAttack() {};
		~AerialDestructAttack() {};

		AerialDestructAttack(const AerialDestructAttack&){}
		AerialDestructAttack& operator=(const AerialDestructAttack&){}
	};

	/*******************************************************/
	//				無敵攻撃ステート
	/*******************************************************/
	class InvincibleAttack :public State<AI>
	{
	public:

		//実体_
		static InvincibleAttack* GetInstance();

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
		InvincibleAttack() {};
		~InvincibleAttack() {};

		InvincibleAttack(const InvincibleAttack&){}
		InvincibleAttack& operator=(const InvincibleAttack&){}
	};


	/*******************************************************/
	//			キャラクター固有の攻撃系スキル発動ステート
	/*******************************************************/
	class SkillAttack :public State<AI>
	{
	public:

		//実体_
		static SkillAttack* GetInstance();

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
		SkillAttack() {};
		~SkillAttack() {};

		SkillAttack(const SkillAttack&){}
		SkillAttack& operator=(const SkillAttack&){}
	};


	/*******************************************************/
	//			中段技
	/*******************************************************/
	class DokkoiAttack :public State<AI>
	{
	public:

		//実体_
		static DokkoiAttack* GetInstance();

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
		DokkoiAttack() {};
		~DokkoiAttack() {};

		DokkoiAttack(const DokkoiAttack&) {}
		DokkoiAttack& operator=(const DokkoiAttack&) {}
	};


	/*******************************************************/
	//			イージーアタック
	/*******************************************************/
	class EasyAttack :public State<AI>
	{
	public:

		//実体_
		static EasyAttack* GetInstance();

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
		EasyAttack() {};
		~EasyAttack() {};

		EasyAttack(const EasyAttack&) {}
		EasyAttack& operator=(const EasyAttack&) {}
	};

	/*******************************************************/
	//			後ろ歩き 
	/*******************************************************/
	class BackWark :public State<AI>
	{
	public:

		//実体_
		static BackWark* GetInstance();

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
		BackWark() {};
		~BackWark() {};

		BackWark(const BackWark&) {}
		BackWark& operator=(const BackWark&) {}
	};

	/*******************************************************/
	//			最強の歩行 (難易度 横江くん用)
	/*******************************************************/
	class MightyWark :public State<AI>
	{
	public:

		//実体_
		static MightyWark* GetInstance();

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
		MightyWark() {};
		~MightyWark() {};

		MightyWark(const MightyWark&) {}
		MightyWark& operator=(const MightyWark&) {}
	};

	/*******************************************************/
	//			バックステップ 
	/*******************************************************/
	class BackStep :public State<AI>
	{
	public:

		//実体_
		static BackStep* GetInstance();

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
		BackStep() {};
		~BackStep() {};

		BackStep(const BackStep&) {}
		BackStep& operator=(const BackStep&) {}
	};


	/*******************************************************/
	//			パートナー発動ステート
	/*******************************************************/
	class PartnerAttack :public State<AI>
	{
	public:

		//実体_
		static PartnerAttack* GetInstance();

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
		PartnerAttack() {};
		~PartnerAttack() {};

		PartnerAttack(const PartnerAttack&) {}
		PartnerAttack& operator=(const PartnerAttack&) {}
	};
}



