#pragma once

#pragma once

#include "SelectUI.h"

// 前方宣言
class SelectUI;
struct Message;

// 必要なのは
// Intro
// キャラセレ＆パートナーのセレクト
// ステージ＆BGMのセレクト


namespace SelectUIState
{
	//--------------------ゲーム開始時ステート
	class Intro : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static Intro* GetInstance() { static Intro state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------最初の選択ステート(キャラセレ)
	class FirstStep : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static FirstStep* GetInstance() { static FirstStep state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		FirstStep() {};
		~FirstStep() {};

		FirstStep(const FirstStep&) {}
		FirstStep& operator=(const FirstStep&) {}

	};

	//--------------------中間の選択ステート(キャラセレとパートナーの中間)
	class MiddleStep : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static MiddleStep* GetInstance() { static MiddleStep state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		MiddleStep() {};
		~MiddleStep() {};

		MiddleStep(const MiddleStep&) {}
		MiddleStep& operator=(const MiddleStep&) {}

	};


	//--------------------二つ目の選択ステート(パートナー)
	class SecondStep : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static SecondStep* GetInstance() { static SecondStep state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		SecondStep() {};
		~SecondStep() {};

		SecondStep(const SecondStep&) {}
		SecondStep& operator=(const SecondStep&) {}

	};

	//--------------------セカンドから設定完了まで
	class SecondToOKStep : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static SecondToOKStep* GetInstance() { static SecondToOKStep state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		SecondToOKStep() {};
		~SecondToOKStep() {};

		SecondToOKStep(const SecondToOKStep&) {}
		SecondToOKStep& operator=(const SecondToOKStep&) {}

	};

	//--------------------設定完了
	class OKStep : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static OKStep* GetInstance() { static OKStep state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		OKStep() {};
		~OKStep() {};

		OKStep(const OKStep&) {}
		OKStep& operator=(const OKStep&) {}

	};



	//--------------------設定完了からセカンドまで
	class OKToSecondStep : public State<SelectUI>
	{
	public:
		// this is a シングルトン
		static OKToSecondStep * GetInstance() { static OKToSecondStep state; return &state; }

		// 入る
		virtual void Enter(SelectUI* pMain);

		// 実行します
		virtual void Execute(SelectUI* pMain);

		// 帰る
		virtual void Exit(SelectUI* pMain);

		// 描画
		virtual void Render(SelectUI* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		OKToSecondStep() {};
		~OKToSecondStep() {};

		OKToSecondStep(const OKToSecondStep&) {}
		OKToSecondStep& operator=(const OKToSecondStep&) {}

	};
}