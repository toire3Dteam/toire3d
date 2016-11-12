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

}