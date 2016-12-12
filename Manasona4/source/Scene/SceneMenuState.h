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
		// this is a シングルトン
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
		// this is a シングルトン
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
		// this is a シングルトン
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


	//--------------------チュートリアル選択
	class TutorialSelectStep : public State<sceneMenu>
	{
	public:
		// this is a シングルトン
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
		// this is a シングルトン
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

	//--------------------サウンドウィンドウ
	class SoundWindowStep : public State<sceneMenu>
	{
	public:
		// this is a シングルトン
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

}