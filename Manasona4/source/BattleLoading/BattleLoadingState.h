#pragma once
#include "BattleLoading.h"

// 前方宣言
class BattleLoading;
struct Message;

//
int GetCharaType(SIDE eSide);

//+------------------------------
//	バトル前ロード画面ステート
//+------------------------------

namespace BattleLoadingState
{
	//--------------------開始時ステート
	class Intro : public State<BattleLoading>
	{
	public:
		// 実体_
		static Intro* GetInstance() { static Intro state; return &state; }

		// 入る
		virtual void Enter(BattleLoading* pMain);

		// 実行します
		virtual void Execute(BattleLoading* pMain);

		// 帰る
		virtual void Exit(BattleLoading* pMain);

		// 描画
		virtual void Render(BattleLoading* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//あしたはステートに色々かいて動かしていく
	//	ｔｄん２ｄおｂｊ とかに何も読み込んでなかったら
	//	白い□を描画するようにする
	//	がんばろ

	//--------------------1Pスライドステート
	class Slide1P : public State<BattleLoading>
	{
	public:
		// 実体_
		static Slide1P* GetInstance() { static Slide1P state; return &state; }

		// 入る
		virtual void Enter(BattleLoading* pMain);

		// 実行します
		virtual void Execute(BattleLoading* pMain);

		// 帰る
		virtual void Exit(BattleLoading* pMain);

		// 描画
		virtual void Render(BattleLoading* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		Slide1P() {};
		~Slide1P() {};

		Slide1P(const Slide1P&) {}
		Slide1P& operator=(const Slide1P&) {}
	};

	//--------------------2Pスライドステート
	class Slide2P : public State<BattleLoading>
	{
	public:
		// 実体_
		static Slide2P* GetInstance() { static Slide2P state; return &state; }

		// 入る
		virtual void Enter(BattleLoading* pMain);

		// 実行します
		virtual void Execute(BattleLoading* pMain);

		// 帰る
		virtual void Exit(BattleLoading* pMain);

		// 描画
		virtual void Render(BattleLoading* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		Slide2P() {};
		~Slide2P() {};

		Slide2P(const Slide2P&) {}
		Slide2P& operator=(const Slide2P&) {}
	};

	//--------------------スライド終了ステート
	class SlideEnd : public State<BattleLoading>
	{
	public:
		// 実体_
		static SlideEnd* GetInstance() { static SlideEnd state; return &state; }

		// 入る
		virtual void Enter(BattleLoading* pMain);

		// 実行します
		virtual void Execute(BattleLoading* pMain);

		// 帰る
		virtual void Exit(BattleLoading* pMain);

		// 描画
		virtual void Render(BattleLoading* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		SlideEnd() {};
		~SlideEnd() {};

		SlideEnd(const SlideEnd&) {}
		SlideEnd& operator=(const SlideEnd&) {}
	};

	//--------------------最後ステート
	class FinalStep : public State<BattleLoading>
	{
	public:
		// 実体_
		static FinalStep* GetInstance() { static FinalStep state; return &state; }

		// 入る
		virtual void Enter(BattleLoading* pMain);

		// 実行します
		virtual void Execute(BattleLoading* pMain);

		// 帰る
		virtual void Exit(BattleLoading* pMain);

		// 描画
		virtual void Render(BattleLoading* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		FinalStep() {};
		~FinalStep() {};

		FinalStep(const FinalStep&) {}
		FinalStep& operator=(const FinalStep&) {}
	};

	//--------------------フェードチェンジステート
	class FadeChangeStep : public State<BattleLoading>
	{
	public:
		// 実体_
		static FadeChangeStep* GetInstance() { static FadeChangeStep state; return &state; }

		// 入る
		virtual void Enter(BattleLoading* pMain);

		// 実行します
		virtual void Execute(BattleLoading* pMain);

		// 帰る
		virtual void Exit(BattleLoading* pMain);

		// 描画
		virtual void Render(BattleLoading* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		FadeChangeStep() {};
		~FadeChangeStep() {};

		FadeChangeStep(const FadeChangeStep&) {}
		FadeChangeStep& operator=(const FadeChangeStep&) {}
	};
}

