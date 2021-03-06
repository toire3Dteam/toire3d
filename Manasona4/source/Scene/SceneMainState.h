#pragma once

#include "SceneMain.h"

// 前方宣言
class sceneMain;
class LoadSceneThread;
struct Message;

namespace SceneMainState
{
	//--------------------ゲーム開始時ステート
	class StageIntro : public State<sceneMain>
	{
	public:
		// 実体_
		static StageIntro* GetInstance(){ static StageIntro state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);

	private:
		bool m_bSkip;	// イントロスキップしてフェードしたかどうか

		StageIntro() {};
		~StageIntro() {};

		StageIntro(const StageIntro&){}
		StageIntro& operator=(const StageIntro&){}
	};

	//--------------------ゲーム開始時ステート
	class CharaIntro : public State<sceneMain>
	{
	public:
		// 実体_
		static CharaIntro* GetInstance(){ static CharaIntro state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);

	private:

		bool m_bSkip;		// イントロスキップしてフェードしたかどうか
		int m_iKakeaiTimer;	// 掛け合いの時間計測用

		CharaIntro() {};
		~CharaIntro() {};

		CharaIntro(const CharaIntro&){}
		CharaIntro& operator=(const CharaIntro&){}
	};

	//--------------------ラウンドコール
	class Round : public State<sceneMain>
	{
	public:
		// 実体_
		static Round* GetInstance(){ static Round state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);

	private:
		Round() {};
		~Round() {};

		Round(const Round&){}
		Round& operator=(const Round&){}
	};

	//--------------------メインステート
	class Main : public State<sceneMain>
	{
	public:
		// 実体_
		static Main* GetInstance(){ static Main state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		Main() {};
		~Main() {};

		Main(const Main&){}
		Main& operator=(const Main&){}
	};

	//--------------------フィニッシュステート(ラウンドをまたぐかリザルトに行くかの分岐はこの中で行う予定)
	class Finish : public State<sceneMain>
	{
	public:
		// 実体_
		static Finish* GetInstance(){ static Finish state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		bool m_bEndCall;

		Finish() :m_bEndCall(false) {};
		~Finish() {};

		Finish(const Finish&){}
		Finish& operator=(const Finish&){}
	};

	//--------------------ヒーホーオーバーフロー成功演出中ステート
	class HeaveHoDriveOverFlowSuccess : public State<sceneMain>
	{
	public:
		// 実体_
		static HeaveHoDriveOverFlowSuccess* GetInstance() { static HeaveHoDriveOverFlowSuccess state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		HeaveHoDriveOverFlowSuccess(){};
		~HeaveHoDriveOverFlowSuccess(){}

		HeaveHoDriveOverFlowSuccess(const HeaveHoDriveOverFlowSuccess&) {}
		HeaveHoDriveOverFlowSuccess& operator=(const HeaveHoDriveOverFlowSuccess&) {}
	};


	//--------------------チュートリアル（イントロ）ステート
	class TutorialIntro : public State<sceneMain>
	{
	public:
		// 実体_
		static TutorialIntro* GetInstance() { static TutorialIntro state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TutorialIntro() {};
		~TutorialIntro() {};

		TutorialIntro(const TutorialIntro&) {}
		TutorialIntro& operator=(const TutorialIntro&) {}
	};
	
	//--------------------チュートリアル（メイン）ステート
	class TutorialMain : public State<sceneMain>
	{
	public:
		// 実体_
		static TutorialMain* GetInstance() { static TutorialMain state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TutorialMain() {};
		~TutorialMain() {};

		TutorialMain(const TutorialMain&) {}
		TutorialMain& operator=(const TutorialMain&) {}
	};


	//--------------------チュートリアル（クリア）ステート
	class TutorialClear: public State<sceneMain>
	{
	public:
		// 実体_
		static TutorialClear* GetInstance() { static TutorialClear state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TutorialClear() {};
		~TutorialClear() {};

		TutorialClear(const TutorialClear&) {}
		TutorialClear& operator=(const TutorialClear&) {}
	};


	//--------------------チャレンジ（イントロ）ステート
	class ChallengeIntro : public State<sceneMain>
	{
	public:
		// 実体_
		static ChallengeIntro* GetInstance() { static ChallengeIntro state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		ChallengeIntro() {};
		~ChallengeIntro() {};

		ChallengeIntro(const ChallengeIntro&) {}
		ChallengeIntro& operator=(const ChallengeIntro&) {}
	};

	//--------------------チャレンジ（メイン）ステート
	class ChallengeMain : public State<sceneMain>
	{
	public:
		// 実体_
		static ChallengeMain* GetInstance() { static ChallengeMain state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		ChallengeMain() {};
		~ChallengeMain() {};

		ChallengeMain(const ChallengeMain&) {}
		ChallengeMain& operator=(const ChallengeMain&) {}
	};


	//--------------------チャレンジ（クリア）ステート
	class ChallengeClear : public State<sceneMain>
	{
	public:
		// 実体_
		static ChallengeClear* GetInstance() { static ChallengeClear state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		ChallengeClear() {};
		~ChallengeClear() {};

		ChallengeClear(const ChallengeClear&) {}
		ChallengeClear& operator=(const ChallengeClear&) {}
	};

	////--------------------チャレンジ（リプレイ最初）ステート
	//class ChallengeReplayIntro : public State<sceneMain>
	//{
	//public:
	//	// 実体_
	//	static ChallengeReplayIntro* GetInstance() { static ChallengeReplayIntro state; return &state; }

	//	// 入る
	//	virtual void Enter(sceneMain* pMain);

	//	// 実行します
	//	virtual void Execute(sceneMain* pMain);

	//	// 帰る
	//	virtual void Exit(sceneMain* pMain);

	//	// 描画
	//	virtual void Render(sceneMain* pMain);

	//	// エージェントからのメッセージを受信した場合、これが実行される
	//	virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	//private:
	//	ChallengeReplayIntro() {};
	//	~ChallengeReplayIntro() {};

	//	ChallengeReplayIntro(const ChallengeReplayIntro&) {}
	//	ChallengeReplayIntro& operator=(const ChallengeReplayIntro&) {}
	//};

	//--------------------チャレンジ（リプレイ）ステート
	class ChallengeReplay : public State<sceneMain>
	{
	public:
		// 実体_
		static ChallengeReplay* GetInstance() { static ChallengeReplay state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		ChallengeReplay() {};
		~ChallengeReplay() {};

		ChallengeReplay(const ChallengeReplay&) {}
		ChallengeReplay& operator=(const ChallengeReplay&) {}
	};

	//--------------------トレーニングイントロ　ステート
	class TrainingIntro : public State<sceneMain>
	{
	public:
		// 実体_
		static TrainingIntro* GetInstance() { static TrainingIntro state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TrainingIntro() {};
		~TrainingIntro() {};

		TrainingIntro(const TrainingIntro&) {}
		TrainingIntro& operator=(const TrainingIntro&) {}
	};

	//--------------------トレーニング　ステート
	class Training : public State<sceneMain>
	{
	public:
		// 実体_
		static Training* GetInstance() { static Training state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		Training() {};
		~Training() {};

		Training(const Training&) {}
		Training& operator=(const Training&) {}
	};


	//--------------------ポーズメニューステート
	class PauseMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static PauseMenu* GetInstance() { static PauseMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		PauseMenu() {};
		~PauseMenu() {};

		PauseMenu(const PauseMenu&) {}
		PauseMenu& operator=(const PauseMenu&) {}
	};

	//--------------------チュートリアルポーズメニューステート
	class TutorialPauseMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static TutorialPauseMenu* GetInstance() { static TutorialPauseMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TutorialPauseMenu() {};
		~TutorialPauseMenu() {};

		TutorialPauseMenu(const TutorialPauseMenu&) {}
		TutorialPauseMenu& operator=(const TutorialPauseMenu&) {}
	};

	//--------------------チャレンジポーズメニューステート
	class ChallengePauseMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static ChallengePauseMenu* GetInstance() { static ChallengePauseMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		ChallengePauseMenu() {};
		~ChallengePauseMenu() {};

		ChallengePauseMenu(const ChallengePauseMenu&) {}
		ChallengePauseMenu& operator=(const ChallengePauseMenu&) {}
	};

	//--------------------トレーニングポーズ　ステート
	class TrainingPause : public State<sceneMain>
	{
	public:
		// 実体_
		static TrainingPause* GetInstance() { static TrainingPause state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TrainingPause() {};
		~TrainingPause() {};

		TrainingPause(const TrainingPause&) {}
		TrainingPause& operator=(const TrainingPause&) {}
	};

	//--------------------サウンドメニューステート
	class SoundMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static SoundMenu* GetInstance() { static SoundMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		SoundMenu() {};
		~SoundMenu() {};

		SoundMenu(const SoundMenu&) {}
		SoundMenu& operator=(const SoundMenu&) {}
	};

	//--------------------トレーニングオプションメニューステート
	class TrainingOptionMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static TrainingOptionMenu* GetInstance() { static TrainingOptionMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TrainingOptionMenu() {};
		~TrainingOptionMenu() {};

		TrainingOptionMenu(const TrainingOptionMenu&) {}
		TrainingOptionMenu& operator=(const TrainingOptionMenu&) {}
	};


	//--------------------トレーニングダミーメニューステート
	class TrainingDummyMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static TrainingDummyMenu* GetInstance() { static TrainingDummyMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TrainingDummyMenu() {};
		~TrainingDummyMenu() {};

		TrainingDummyMenu(const TrainingDummyMenu&) {}
		TrainingDummyMenu& operator=(const TrainingDummyMenu&) {}
	};


	//--------------------トレーニングキャラクターメニューステート
	class TrainingCharacterMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static TrainingCharacterMenu* GetInstance() { static TrainingCharacterMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		TrainingCharacterMenu() {};
		~TrainingCharacterMenu() {};

		TrainingCharacterMenu(const TrainingCharacterMenu&) {}
		TrainingCharacterMenu& operator=(const TrainingCharacterMenu&) {}
	};

	//--------------------メニュー非表示メニューステート
	class HideMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static HideMenu* GetInstance() { static HideMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		HideMenu() {};
		~HideMenu() {};

		HideMenu(const HideMenu&) {}
		HideMenu& operator=(const HideMenu&) {}
	};

	//--------------------コマンドステート
	class CommandMenu : public State<sceneMain>
	{
	public:
		// 実体_
		static CommandMenu* GetInstance() { static CommandMenu state; return &state; }

		// 入る
		virtual void Enter(sceneMain* pMain);

		// 実行します
		virtual void Execute(sceneMain* pMain);

		// 帰る
		virtual void Exit(sceneMain* pMain);

		// 描画
		virtual void Render(sceneMain* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		CommandMenu() {};
		~CommandMenu() {};

		CommandMenu(const CommandMenu&) {}
		CommandMenu& operator=(const CommandMenu&) {}
	};

}