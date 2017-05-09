#pragma once

#include "SceneMain.h"

// 前方宣言
class sceneMenu;
struct Message;

namespace SceneMenuState
{
	//--------------------ゲーム開始時ステート
	class Intro : public State<sceneMenu>
	{
	public:
		// 実体_
		static Intro* GetInstance() { static Intro state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------最初の選択
	class FirstStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static FirstStep* GetInstance() { static FirstStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		FirstStep() {};
		~FirstStep() {};

		FirstStep(const FirstStep&) {}
		FirstStep& operator=(const FirstStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};


	//--------------------バトルのコントローラー選択
	class BattleControllerSelectStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static BattleControllerSelectStep* GetInstance() { static BattleControllerSelectStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		BattleControllerSelectStep() {};
		~BattleControllerSelectStep() {};

		BattleControllerSelectStep(const BattleControllerSelectStep&) {}
		BattleControllerSelectStep& operator=(const BattleControllerSelectStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------トレーニングのコントローラー選択
	class TrainingControllerSelectStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static TrainingControllerSelectStep* GetInstance() { static TrainingControllerSelectStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		TrainingControllerSelectStep() {};
		~TrainingControllerSelectStep() {};

		TrainingControllerSelectStep(const TrainingControllerSelectStep&) {}
		TrainingControllerSelectStep& operator=(const TrainingControllerSelectStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};


	//--------------------チュートリアル選択
	class TutorialSelectStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static TutorialSelectStep* GetInstance() { static TutorialSelectStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		TutorialSelectStep() {};
		~TutorialSelectStep() {};

		TutorialSelectStep(const TutorialSelectStep&) {}
		TutorialSelectStep& operator=(const TutorialSelectStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------オプションウィンドウ
	class OptionStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static OptionStep* GetInstance() { static OptionStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		OptionStep() {};
		~OptionStep() {};

		OptionStep(const OptionStep&) {}
		OptionStep& operator=(const OptionStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------チャレンジウィンドウ
	class ChallengeStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static ChallengeStep* GetInstance() { static ChallengeStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		ChallengeStep() {};
		~ChallengeStep() {};

		ChallengeStep(const ChallengeStep&) {}
		ChallengeStep& operator=(const ChallengeStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------チャレンジナンバーウィンドウ
	class ChallengeNoStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static ChallengeNoStep* GetInstance() { static ChallengeNoStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		ChallengeNoStep() {};
		~ChallengeNoStep() {};

		ChallengeNoStep(const ChallengeNoStep&) {}
		ChallengeNoStep& operator=(const ChallengeNoStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------システムウィンドウ
	class SystemWindowStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static SystemWindowStep* GetInstance() { static SystemWindowStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		SystemWindowStep() {};
		~SystemWindowStep() {};

		SystemWindowStep(const SystemWindowStep&) {}
		SystemWindowStep& operator=(const SystemWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};


	//--------------------ゲームウィンドウ
	class GameWindowStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static GameWindowStep* GetInstance() { static GameWindowStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		GameWindowStep() {};
		~GameWindowStep() {};

		GameWindowStep(const GameWindowStep&) {}
		GameWindowStep& operator=(const GameWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------サウンドウィンドウ
	class SoundWindowStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static SoundWindowStep* GetInstance() { static SoundWindowStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		SoundWindowStep() {};
		~SoundWindowStep() {};

		SoundWindowStep(const SoundWindowStep&) {}
		SoundWindowStep& operator=(const SoundWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------キーオンフィグウィンドウ
	class KeyConfigWindowStep : public State<sceneMenu>
	{
	public:
		// 実体_
		static KeyConfigWindowStep* GetInstance() { static KeyConfigWindowStep state; return &state; }

		// 入る
		virtual void Enter(sceneMenu* pMain);

		// 実行します
		virtual void Execute(sceneMenu* pMain);

		// 帰る
		virtual void Exit(sceneMenu* pMain);

		// 描画
		virtual void Render(sceneMenu* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		KeyConfigWindowStep() {};
		~KeyConfigWindowStep() {};

		KeyConfigWindowStep(const KeyConfigWindowStep&) {}
		KeyConfigWindowStep& operator=(const KeyConfigWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};
}