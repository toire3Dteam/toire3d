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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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



	//--------------------トレーニングイントロ　ステート
	class TrainingIntro : public State<sceneMain>
	{
	public:
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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

	//--------------------トレーニングポーズ　ステート
	class TrainingPause : public State<sceneMain>
	{
	public:
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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

}