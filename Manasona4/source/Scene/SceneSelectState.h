#pragma once

#include "SceneSelect.h"

// 前方宣言
class sceneSelect;
struct Message;

// 必要なのは
// Intro
// キャラセレ＆パートナーのセレクト
// ステージ＆BGMのセレクト


namespace SceneSelectState
{
	//--------------------ゲーム開始時ステート
	class Intro : public State<sceneSelect>
	{
	public:
		// this is a シングルトン
		static Intro* GetInstance() { static Intro state; return &state; }

		// 入る
		virtual void Enter(sceneSelect* pMain);

		// 実行します
		virtual void Execute(sceneSelect* pMain);

		// 帰る
		virtual void Exit(sceneSelect* pMain);

		// 描画
		virtual void Render(sceneSelect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------最初の選択
	class FirstStep : public State<sceneSelect>
	{
	public:
		// this is a シングルトン
		static FirstStep* GetInstance() { static FirstStep state; return &state; }

		// 入る
		virtual void Enter(sceneSelect* pMain);

		// 実行します
		virtual void Execute(sceneSelect* pMain);

		// 帰る
		virtual void Exit(sceneSelect* pMain);

		// 描画
		virtual void Render(sceneSelect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		FirstStep() {};
		~FirstStep() {};

		FirstStep(const FirstStep&) {}
		FirstStep& operator=(const FirstStep&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};



}