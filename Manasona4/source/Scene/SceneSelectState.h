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

	//--------------------キャラクターの選択
	class CharaSelect : public State<sceneSelect>
	{
	public:
		// this is a シングルトン
		static CharaSelect* GetInstance() { static CharaSelect state; return &state; }

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
		CharaSelect() {};
		~CharaSelect() {};

		CharaSelect(const CharaSelect&) {}
		CharaSelect& operator=(const CharaSelect&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};


	//--------------------ステージ＆BGMの選択
	class StageAndBGM : public State<sceneSelect>
	{
	public:
		// this is a シングルトン
		static StageAndBGM* GetInstance() { static StageAndBGM state; return &state; }

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
		StageAndBGM() {};
		~StageAndBGM() {};

		StageAndBGM(const StageAndBGM&) {}
		StageAndBGM& operator=(const StageAndBGM&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};



	//--------------------おわり
	class End : public State<sceneSelect>
	{
	public:
		// this is a シングルトン
		static End* GetInstance() { static End state; return &state; }

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
		End() {};
		~End() {};

		End(const End&) {}
		End& operator=(const End&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};
}