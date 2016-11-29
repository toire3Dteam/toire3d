#pragma once
#include "../BaseEntity/State/StateMachine.h"

// 前方宣言
class BasePlayer;

/*
	ベースプレイヤーのステートマシンのカートリッジ
*/


bool JumpCancel(BasePlayer * pPerson);
bool isThrownState(BasePlayer *pPerson);
bool RunCancel(BasePlayer * pPerson);
bool SquatCancel(BasePlayer * pPerson);
bool SquatAttackCancel(BasePlayer * pPerson);
bool StandCancel(BasePlayer * pPerson);
bool AttackCancel(BasePlayer *pPerson);
bool InvincibleAttackCancel(BasePlayer *pPerson);
bool SkillCancel(BasePlayer *pPerson);
bool OverDriveCancel(BasePlayer *pPerson);
bool EscapeCancel(BasePlayer *pPerson);
bool ThrowCancel(BasePlayer * pPerson);
bool HeaveHoCancel(BasePlayer * pPerson);
bool HeaveHoOverFlowCancel(BasePlayer * pPerson);
bool DashCancel(BasePlayer * pPerson);
bool AerialDashCancel(BasePlayer *pPerson);
bool AerialAttackCancel(BasePlayer *pPerson);
bool DokkoiAttackCancel(BasePlayer *pPerson);
bool isInputGuardCommand(BasePlayer *pPerson);
void GuardUpdate(BasePlayer *pPerson);
bool isPossibleGuardState(BasePlayer *pPerson);


namespace BasePlayerState
{
	/*******************************************************/
	//					グローバルステート
	/*******************************************************/
	class Global :public State<BasePlayer>
	{
	public:

		//this is a シングルトン
		static Global* GetInstance(){ static Global instance; return &instance; }

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
		static Wait* GetInstance(){ static Wait instance; return &instance; }

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
		static Walk* GetInstance(){ static Walk instance; return &instance; }

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
	//					後ろ歩き
	/*******************************************************/
	class BackWalk :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static BackWalk* GetInstance(){ static BackWalk instance; return &instance; }

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
		BackWalk() {};
		~BackWalk() {};

		BackWalk(const BackWalk&) {}
		BackWalk& operator=(const BackWalk&) {}
	};

	/*******************************************************/
	//					走り移動
	/*******************************************************/
	class Run :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Run* GetInstance(){ static Run instance; return &instance; }

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
	//					バクステ
	/*******************************************************/
	class BackStep :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static BackStep* GetInstance(){ static BackStep instance; return &instance; }

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
		BackStep() {};
		~BackStep() {};

		BackStep(const BackStep&) {}
		BackStep& operator=(const BackStep&) {}
	};


	/*******************************************************/
	//					ブレーキ
	/*******************************************************/
	//class FrontBrake :public State<BasePlayer>
	//{
	//public:

	//	// this is a シングルトン
	//	static FrontBrake* GetInstance();

	//	// 入る
	//	virtual void Enter(BasePlayer* pPerson);

	//	// 実行します
	//	virtual void Execute(BasePlayer* pPerson);

	//	// 帰る
	//	virtual void Exit(BasePlayer* pPerson);

	//	// 描画
	//	virtual void Render(BasePlayer* pPerson);

	//	// エージェントからのメッセージを受信した場合、これが実行される
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	//private:
	//	FrontBrake() {};
	//	~FrontBrake() {};

	//	FrontBrake(const FrontBrake&) {}
	//	FrontBrake& operator=(const FrontBrake&) {}
	//};

	/*******************************************************/
	//					Uターンブレーキ
	/*******************************************************/
	//class TurnOverBrake :public State<BasePlayer>
	//{
	//public:

	//	// this is a シングルトン
	//	static TurnOverBrake* GetInstance();

	//	// 入る
	//	virtual void Enter(BasePlayer* pPerson);

	//	// 実行します
	//	virtual void Execute(BasePlayer* pPerson);

	//	// 帰る
	//	virtual void Exit(BasePlayer* pPerson);

	//	// 描画
	//	virtual void Render(BasePlayer* pPerson);

	//	// エージェントからのメッセージを受信した場合、これが実行される
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	//private:
	//	TurnOverBrake() {};
	//	~TurnOverBrake() {};

	//	TurnOverBrake(const TurnOverBrake&) {}
	//	TurnOverBrake& operator=(const TurnOverBrake&) {}
	//};

	/*******************************************************/
	//					ジャンプ
	/*******************************************************/
	class Jump :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Jump* GetInstance(){ static Jump instance; return &instance; }

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
		static AerialJump* GetInstance(){ static AerialJump instance; return &instance; }

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
		static Fall* GetInstance(){ static Fall instance; return &instance; }

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
		static Land* GetInstance(){ static Land instance; return &instance; }

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
		static RushAttack* GetInstance(){ static RushAttack instance; return &instance; }

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
		static KnockBack* GetInstance(){ static KnockBack instance; return &instance; }

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
	//		足払い喰らいステート　
	/*******************************************************/
	class DownFall :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static DownFall* GetInstance(){ static DownFall instance; return &instance; }

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

		DownFall() {};
		~DownFall() {};

		DownFall(const DownFall&) {}
		DownFall& operator=(const DownFall&) {}
	};


	/*******************************************************/
	//					ノックダウンステート
	/*******************************************************/
	class KnockDown :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static KnockDown* GetInstance(){ static KnockDown instance; return &instance; }

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
	//					死に状態ステート
	/*******************************************************/
	class Die :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Die* GetInstance(){ static Die instance; return &instance; }

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

		Die() {};
		~Die() {};

		Die(const Die&) {}
		Die& operator=(const Die&) {}
	};

	/*******************************************************/
	//					空中リカバリーステート
	/*******************************************************/
	class LandRecovery :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static LandRecovery* GetInstance(){ static LandRecovery instance; return &instance; }

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
		static AerialRecovery* GetInstance(){ static AerialRecovery instance; return &instance; }

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
	//					しゃがみ
	/*******************************************************/
	class Squat :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Squat* GetInstance(){ static Squat instance; return &instance; }

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
	//					2B攻撃
	/*******************************************************/
	class SquatAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static SquatAttack* GetInstance(){ static SquatAttack instance; return &instance; }

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
	//					対空攻撃
	/*******************************************************/
	class AntiAirAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AntiAirAttack* GetInstance(){ static AntiAirAttack instance; return &instance; }

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
		AntiAirAttack() {};
		~AntiAirAttack() {};

		AntiAirAttack(const AntiAirAttack&) {}
		AntiAirAttack& operator=(const AntiAirAttack&) {}
	};

	/*******************************************************/
	//					下段攻撃
	/*******************************************************/
	class DownAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static DownAttack* GetInstance() { static DownAttack instance; return &instance; }

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
		DownAttack() {};
		~DownAttack() {};

		DownAttack(const DownAttack&) {}
		DownAttack& operator=(const DownAttack&) {}
	};


	/*******************************************************/
	//					中段攻撃
	/*******************************************************/
	class DokkoiAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static DokkoiAttack* GetInstance() { static DokkoiAttack instance; return &instance; }

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
		DokkoiAttack() {};
		~DokkoiAttack() {};

		DokkoiAttack(const DownAttack&) {}
		DokkoiAttack& operator=(const DownAttack&) {}
	};

	/*******************************************************/
	//					空中攻撃 
	/*******************************************************/
	class AerialAttack :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialAttack* GetInstance(){ static AerialAttack instance; return &instance; }

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
	//class AerialDropAttack :public State<BasePlayer>
	//{
	//public:

	//	// this is a シングルトン
	//	static AerialDropAttack* GetInstance();

	//	// 入る
	//	virtual void Enter(BasePlayer* pPerson);

	//	// 実行します
	//	virtual void Execute(BasePlayer* pPerson);

	//	// 帰る
	//	virtual void Exit(BasePlayer* pPerson);

	//	// 描画
	//	virtual void Render(BasePlayer* pPerson);

	//	// エージェントからのメッセージを受信した場合、これが実行される
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	//private:
	//	AerialDropAttack() {};
	//	~AerialDropAttack() {};

	//	AerialDropAttack(const AerialDropAttack&) {}
	//	AerialDropAttack& operator=(const AerialDropAttack&) {}
	//};

	/*******************************************************/
	//					回避
	/*******************************************************/
	class Escape :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Escape* GetInstance(){ static Escape instance; return &instance; }

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
		static StandAction* GetInstance(){ static StandAction instance; return &instance; }

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
		static FinishAttack* GetInstance(){ static FinishAttack instance; return &instance; }

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
		static Skill* GetInstance(){ static Skill instance; return &instance; }

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
		static OverDrive_OneMore* GetInstance(){ static OverDrive_OneMore instance; return &instance; }

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
		static OverDrive_Burst* GetInstance(){ static OverDrive_Burst instance; return &instance; }

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
		static Guard* GetInstance(){ static Guard instance; return &instance; }

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
		static Throw* GetInstance(){ static Throw instance; return &instance; }

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
	//				投げ猶予モーション
	/*******************************************************/
	class ThrowHold :public State<BasePlayer>
	{
	public:
		// this is a シングルトン
		static ThrowHold* GetInstance() { static ThrowHold instance; return &instance; }

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
		ThrowHold() {};
		~ThrowHold() {};

		ThrowHold(const ThrowHold&) {}
		ThrowHold& operator=(const ThrowHold&) {}
	};

	/*******************************************************/
	//				投げ成功モーション
	/*******************************************************/
	class ThrowSuccess :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static ThrowSuccess* GetInstance(){ static ThrowSuccess instance; return &instance; }

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
		ThrowSuccess() {};
		~ThrowSuccess() {};

		ThrowSuccess(const ThrowSuccess&) {}
		ThrowSuccess& operator=(const ThrowSuccess&) {}
	};

	/*******************************************************/
	//				掴まれたモーション
	/*******************************************************/
	class ThrowBind :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static ThrowBind* GetInstance(){ static ThrowBind instance; return &instance; }

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
		static SuccessThrowRelease* GetInstance(){ static SuccessThrowRelease instance; return &instance; }

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
		static HeavehoDrive* GetInstance(){ static HeavehoDrive instance; return &instance; }

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
		static HeavehoDriveOverFlow* GetInstance(){ static HeavehoDriveOverFlow instance; return &instance; }

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
		static HeavehoDriveOverFlow_Success* GetInstance(){ static HeavehoDriveOverFlow_Success instance; return &instance; }

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

	/*******************************************************/
	//				ラウンド取得(WIN)
	/*******************************************************/
	class Win :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Win* GetInstance(){ static Win instance; return &instance; }

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
		Win() {};
		~Win() {};

		Win(const Win&) {}
		Win& operator=(const Win&) {}
	};

	/*******************************************************/
	//				空中ダッシュ
	/*******************************************************/
	class AerialDash :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialDash* GetInstance() { static AerialDash instance; return &instance; }

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
		AerialDash() {};
		~AerialDash() {};

		AerialDash(const AerialDash&) {}
		AerialDash& operator=(const AerialDash&) {}
	};

	/*******************************************************/
	//				空中バックダッシュ
	/*******************************************************/
	class AerialBackDash :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static AerialBackDash* GetInstance() { static AerialBackDash instance; return &instance; }

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
		AerialBackDash() {};
		~AerialBackDash() {};

		AerialBackDash(const AerialBackDash&) {}
		AerialBackDash& operator=(const AerialBackDash&) {}
	};

	/*******************************************************/
	//				イントロ演出用
	/*******************************************************/
	class Intro :public State<BasePlayer>
	{
	public:

		// this is a シングルトン
		static Intro* GetInstance() { static Intro instance; return &instance; }

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
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	/*******************************************************/
	//				強攻撃 (三段目)
	/*******************************************************/
	//class StrongAttack :public State<BasePlayer>
	//{
	//public:
	//
	//	// this is a シングルトン
	//	static StrongAttack* GetInstance() { static StrongAttack instance; return &instance; }
	//
	//	// 入る
	//	virtual void Enter(BasePlayer* pPerson);
	//
	//	// 実行します
	//	virtual void Execute(BasePlayer* pPerson);
	//
	//	// 帰る
	//	virtual void Exit(BasePlayer* pPerson);
	//
	//	// 描画
	//	virtual void Render(BasePlayer* pPerson);
	//
	//	// エージェントからのメッセージを受信した場合、これが実行される
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);
	//
	//
	//private:
	//	StrongAttack() {};
	//	~StrongAttack() {};
	//
	//	StrongAttack(const StrongAttack&) {}
	//	StrongAttack& operator=(const StrongAttack&) {}
	//};

}

