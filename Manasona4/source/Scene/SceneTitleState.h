#pragma once

#include "sceneTitle.h"

// 前方宣言
class sceneTitle;
struct Message;


namespace SceneTitleState
{
	//--------------------ゲーム開始時ステート
	class Intro : public State<sceneTitle>
	{
	public:
		// this is a シングルトン
		static Intro* GetInstance() { static Intro state; return &state; }

		// 入る
		virtual void Enter(sceneTitle* pMain);

		// 実行します
		virtual void Execute(sceneTitle* pMain);

		// 帰る
		virtual void Exit(sceneTitle* pMain);

		// 描画
		virtual void Render(sceneTitle* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};
	
	//--------------------ロゴステート
	class LogoStep : public State<sceneTitle>
	{
	public:
		// this is a シングルトン
		static LogoStep* GetInstance() { static LogoStep state; return &state; }

		// 入る
		virtual void Enter(sceneTitle* pMain);

		// 実行します
		virtual void Execute(sceneTitle* pMain);

		// 帰る
		virtual void Exit(sceneTitle* pMain);

		// 描画
		virtual void Render(sceneTitle* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		LogoStep() {};
		~LogoStep() {};

		LogoStep(const LogoStep&) {}
		LogoStep& operator=(const LogoStep&) {}
	};

	//--------------------ムービーステート
	class MovieStep : public State<sceneTitle>
	{
	public:
		// this is a シングルトン
		static MovieStep* GetInstance() { static MovieStep state; return &state; }

		// 入る
		virtual void Enter(sceneTitle* pMain);

		// 実行します
		virtual void Execute(sceneTitle* pMain);

		// 帰る
		virtual void Exit(sceneTitle* pMain);

		// 描画
		virtual void Render(sceneTitle* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		MovieStep() {};
		~MovieStep() {};

		MovieStep(const MovieStep&) {}
		MovieStep& operator=(const MovieStep&) {}
	};


	//--------------------タイトルステート
	class TitleStep : public State<sceneTitle>
	{
	public:
		// this is a シングルトン
		static TitleStep* GetInstance() { static TitleStep state; return &state; }

		// 入る
		virtual void Enter(sceneTitle* pMain);

		// 実行します
		virtual void Execute(sceneTitle* pMain);

		// 帰る
		virtual void Exit(sceneTitle* pMain);

		// 描画
		virtual void Render(sceneTitle* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		TitleStep() {};
		~TitleStep() {};

		TitleStep(const TitleStep&) {}
		TitleStep& operator=(const TitleStep&) {}
	};

	//--------------------おわり
	class End : public State<sceneTitle>
	{
	public:
		// this is a シングルトン
		static End* GetInstance() { static End state; return &state; }

		// 入る
		virtual void Enter(sceneTitle* pMain);

		// 実行します
		virtual void Execute(sceneTitle* pMain);

		// 帰る
		virtual void Exit(sceneTitle* pMain);

		// 描画
		virtual void Render(sceneTitle* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		End() {};
		~End() {};

		End(const End&) {}
		End& operator=(const End&) {}

		bool PadUpdate(sceneTitle* pMain, int DeviceID);
	};


	//--------------------メニュー画面に戻るステート
	class BackMenu : public State<sceneTitle>
	{
	public:
		// this is a シングルトン
		static BackMenu* GetInstance() { static BackMenu state; return &state; }

		// 入る
		virtual void Enter(sceneTitle* pMain);

		// 実行します
		virtual void Execute(sceneTitle* pMain);

		// 帰る
		virtual void Exit(sceneTitle* pMain);

		// 描画
		virtual void Render(sceneTitle* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		BackMenu() {};
		~BackMenu() {};

		BackMenu(const BackMenu&) {}
		BackMenu& operator=(const BackMenu&) {}
	};


}