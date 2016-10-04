#pragma once

#include "SceneMain.h"

// 前方宣言
class sceneMain;
struct Message;

namespace SceneMainState
{
	//--------------------ゲーム開始時ステート
	class Intro : public State<sceneMain>
	{
	public:
		// this is a シングルトン
		static Intro* GetInstance(){ static Intro state; return &state; }

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
		Intro() {};
		~Intro() {};

		Intro(const Intro&){}
		Intro& operator=(const Intro&){}
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

	//--------------------試合終了中ステート
	class End : public State<sceneMain>
	{
	public:
		// this is a シングルトン
		static End* GetInstance(){ static End state; return &state; }

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
		End() {};
		~End() {};

		End(const End&){}
		End& operator=(const End&){}
	};
}