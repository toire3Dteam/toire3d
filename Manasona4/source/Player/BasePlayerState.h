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

	/*******************************************************/
	//					歩き移動
	/*******************************************************/
	class Walk :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Walk* GetInstance();

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
		Walk() {};
		~Walk() {};

		Walk(const Walk&) {}
		Walk& operator=(const Walk&) {}
	};

	/*******************************************************/
	//					走り移動
	/*******************************************************/
	class Run :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Run* GetInstance();

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
		Run() {};
		~Run() {};

		Run(const Run&) {}
		Run& operator=(const Run&) {}
	};

	/*******************************************************/
	//					フロントブレーキ
	/*******************************************************/
	class FrontBrake :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static FrontBrake* GetInstance();

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
		FrontBrake() {};
		~FrontBrake() {};

		FrontBrake(const FrontBrake&) {}
		FrontBrake& operator=(const FrontBrake&) {}
	};

	/*******************************************************/
	//					Uターンブレーキ
	/*******************************************************/
	class TurnOverBrake :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static TurnOverBrake* GetInstance();

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
		TurnOverBrake() {};
		~TurnOverBrake() {};

		TurnOverBrake(const TurnOverBrake&) {}
		TurnOverBrake& operator=(const TurnOverBrake&) {}
	};

	/*******************************************************/
	//					ジャンプ
	/*******************************************************/
	class Jump :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Jump* GetInstance();

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

		Jump() {};
		~Jump() {};

		Jump(const Jump&) {}
		Jump& operator=(const Jump&) {}
	};

	/*******************************************************/
	//					空中ジャンプ
	/*******************************************************/
	class AerialJump :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialJump* GetInstance();

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

		AerialJump() {};
		~AerialJump() {};

		AerialJump(const AerialJump&) {}
		AerialJump& operator=(const AerialJump&) {}
	};

	/*******************************************************/
	//					落下
	/*******************************************************/
	class Fall :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Fall* GetInstance();

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

		Fall() {};
		~Fall() {};

		Fall(const Fall&) {}
		Fall& operator=(const Fall&) {}
	};


	/*******************************************************/
	//					着地ステート
	/*******************************************************/
	class Land :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Land* GetInstance();

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

		Land() {};
		~Land() {};

		Land(const Land&) {}
		Land& operator=(const Land&) {}
	};


	/*******************************************************/
	//					ラッシュ攻撃ステート
	/*******************************************************/
	class RushAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static RushAttack* GetInstance();

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

		RushAttack() {};
		~RushAttack() {};

		RushAttack(const RushAttack&) {}
		RushAttack& operator=(const RushAttack&) {}
	};

	/*******************************************************/
	//					ノックバックステート
	/*******************************************************/
	class KnockBack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static KnockBack* GetInstance();

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

		KnockBack() {};
		~KnockBack() {};

		KnockBack(const KnockBack&) {}
		KnockBack& operator=(const KnockBack&) {}
	};

	/*******************************************************/
	//		エアリアルノックバックステート(空中やられ)　
	/*******************************************************/
	class AerialKnockBack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialKnockBack* GetInstance();

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

		AerialKnockBack() {};
		~AerialKnockBack() {};

		AerialKnockBack(const AerialKnockBack&) {}
		AerialKnockBack& operator=(const AerialKnockBack&) {}
	};


	/*******************************************************/
	//					ノックダウンステート
	/*******************************************************/
	class KnockDown :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static KnockDown* GetInstance();

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

		KnockDown() {};
		~KnockDown() {};

		KnockDown(const KnockDown&) {}
		KnockDown& operator=(const KnockDown&) {}
	};

	/*******************************************************/
	//					空中リカバリーステート
	/*******************************************************/
	class LandRecovery :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static LandRecovery* GetInstance();

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

		LandRecovery() {};
		~LandRecovery() {};

		LandRecovery(const LandRecovery&) {}
		LandRecovery& operator=(const LandRecovery&) {}
	};

	/*******************************************************/
	//					空中リカバリーステート
	/*******************************************************/
	class AerialRecovery :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialRecovery* GetInstance();

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

		AerialRecovery() {};
		~AerialRecovery() {};

		AerialRecovery(const AerialRecovery&) {}
		AerialRecovery& operator=(const AerialRecovery&) {}
	};

	/*******************************************************/
	//					待機
	/*******************************************************/
	class Squat :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Squat* GetInstance();

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
		Squat() {};
		~Squat() {};

		Squat(const Squat&) {}
		Squat& operator=(const Squat&) {}
	};

	/*******************************************************/
	//					待機攻撃
	/*******************************************************/
	class SquatAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static SquatAttack* GetInstance();

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
		SquatAttack() {};
		~SquatAttack() {};

		SquatAttack(const SquatAttack&) {}
		SquatAttack& operator=(const SquatAttack&) {}
	};

	/*******************************************************/
	//					空中攻撃 
	/*******************************************************/
	class AerialAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialAttack* GetInstance();

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
		AerialAttack() {};
		~AerialAttack() {};

		AerialAttack(const AerialAttack&) {}
		AerialAttack& operator=(const AerialAttack&) {}
	};

	/*******************************************************/
	//					空中下攻撃 
	/*******************************************************/
	class AerialDropAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialDropAttack* GetInstance();

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
		AerialDropAttack() {};
		~AerialDropAttack() {};

		AerialDropAttack(const AerialDropAttack&) {}
		AerialDropAttack& operator=(const AerialDropAttack&) {}
	};

	/*******************************************************/
	//					回避
	/*******************************************************/
	class Escape :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Escape* GetInstance();

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
		Escape() {};
		~Escape() {};

		Escape(const Escape&) {}
		Escape& operator=(const Escape&) {}
	};


	/*******************************************************/
	//					ペルソナ召喚モーション
	/*******************************************************/
	class StandAction :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static StandAction* GetInstance();

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
		StandAction() {};
		~StandAction() {};

		StandAction(const StandAction&) {}
		StandAction& operator=(const StandAction&) {}
	};

	/*******************************************************/
	//				フィニッシュモーション
	/*******************************************************/
	class FinishAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static FinishAttack* GetInstance();

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
		FinishAttack() {};
		~FinishAttack() {};

		FinishAttack(const StandAction&) {}
		FinishAttack& operator=(const StandAction&) {}
	};
	
	/*******************************************************/
	//				キャラクター固有モーション
	/*******************************************************/
	class Skill :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Skill* GetInstance();

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
		Skill() {};
		~Skill() {};

		Skill(const Skill&) {}
		Skill& operator=(const Skill&) {}
	};

	/*******************************************************/
	//				1more覚醒モーション
	/*******************************************************/
	class OverDrive_OneMore :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static OverDrive_OneMore* GetInstance();

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
		OverDrive_OneMore() {};
		~OverDrive_OneMore() {};

		OverDrive_OneMore(const OverDrive_OneMore&) {}
		OverDrive_OneMore& operator=(const OverDrive_OneMore&) {}
	};

	/*******************************************************/
	//				Burst覚醒モーション
	/*******************************************************/
	class OverDrive_Burst :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static OverDrive_Burst* GetInstance();

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
		OverDrive_Burst() {};
		~OverDrive_Burst() {};

		OverDrive_Burst(const OverDrive_Burst&) {}
		OverDrive_Burst& operator=(const OverDrive_Burst&) {}
	};

	/*******************************************************/
	//				ガードモーション
	/*******************************************************/
	class Guard :public State<BasePlayer>
	{
	public:
		// this is a シングルトン
		static Guard* GetInstance();

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
		Guard() {};
		~Guard() {};

		Guard(const Guard&) {}
		Guard& operator=(const Guard&) {}
	};


	/*******************************************************/
	//				投げモーション
	/*******************************************************/
	class Throw :public State<BasePlayer>
	{
	public:
		// this is a シングルトン
		static Throw* GetInstance();

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
		Throw() {};
		~Throw() {};

		Throw(const Throw&) {}
		Throw& operator=(const Throw&) {}
	};

	/*******************************************************/
	//				投げ失敗モーション
	/*******************************************************/
	class ThrowMiss :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static ThrowMiss* GetInstance();

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
		ThrowMiss() {};
		~ThrowMiss() {};

		ThrowMiss(const ThrowMiss&) {}
		ThrowMiss& operator=(const ThrowMiss&) {}
	};

	/*******************************************************/
	//				掴まれたモーション
	/*******************************************************/
	class ThrowBind :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static ThrowBind* GetInstance();

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
		ThrowBind() {};
		~ThrowBind() {};

		ThrowBind(const ThrowBind&) {}
		ThrowBind& operator=(const ThrowBind&) {}
	};

	/*******************************************************/
	//				掴み解除成功してはじくステート
	/*******************************************************/
	class SuccessThrowRelease :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static SuccessThrowRelease* GetInstance();

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
		SuccessThrowRelease() {};
		~SuccessThrowRelease() {};

		SuccessThrowRelease(const SuccessThrowRelease&) {}
		SuccessThrowRelease& operator=(const SuccessThrowRelease&) {}
	};

	/*******************************************************/
	//				必殺技ステート
	/*******************************************************/
	class HeavehoDrive :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static HeavehoDrive* GetInstance();

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
		HeavehoDrive() {};
		~HeavehoDrive() {};

		HeavehoDrive(const HeavehoDrive&) {}
		HeavehoDrive& operator=(const HeavehoDrive&) {}
	};

	/*******************************************************/
	//				超必殺技ステート
	/*******************************************************/
	class HeavehoDriveOverFlow :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static HeavehoDriveOverFlow* GetInstance();

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
		HeavehoDriveOverFlow() {};
		~HeavehoDriveOverFlow() {};

		HeavehoDriveOverFlow(const HeavehoDriveOverFlow&) {}
		HeavehoDriveOverFlow& operator=(const HeavehoDriveOverFlow&) {}
	};

	
	/*******************************************************/
	//				超必殺技_成功ステート
	/*******************************************************/
	class HeavehoDriveOverFlow_Success :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static HeavehoDriveOverFlow_Success* GetInstance();

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
		HeavehoDriveOverFlow_Success() {};
		~HeavehoDriveOverFlow_Success() {};

		HeavehoDriveOverFlow_Success(const HeavehoDriveOverFlow_Success&) {}
		HeavehoDriveOverFlow_Success& operator=(const HeavehoDriveOverFlow_Success&) {}
	};
}