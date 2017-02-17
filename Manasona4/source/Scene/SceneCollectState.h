#pragma once
#include "SceneCollect.h"

// 前方宣言
class sceneCollect;
struct Message;

namespace SceneCollectState
{
	//--------------------ゲーム開始時ステート
	class Intro : public State<sceneCollect>
	{
	public:
		// 実体_
		static Intro* GetInstance() { static Intro state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------トロフィーの選択
	class TrophyStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static TrophyStep* GetInstance() { static TrophyStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		TrophyStep() {};
		~TrophyStep() {};

		TrophyStep(const TrophyStep&) {}
		TrophyStep& operator=(const TrophyStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

		//+-------------------------------
		//	トロフィーで必要な変数を用意
		//+-------------------------------

	};

	//--------------------イラストの選択
	class PictureStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static PictureStep* GetInstance() { static PictureStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		PictureStep() {};
		~PictureStep() {};

		PictureStep(const PictureStep&) {}
		PictureStep& operator=(const PictureStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------イラスト鑑賞
	class PictureWatch : public State<sceneCollect>
	{
	public:
		// 実体_
		static PictureWatch* GetInstance() { static PictureWatch state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		PictureWatch() {};
		~PictureWatch() {};

		PictureWatch(const PictureWatch&) {}
		PictureWatch& operator=(const PictureWatch&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------コンテンツ購入確認(イラスト)
	class BuyPictureStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static BuyPictureStep* GetInstance() { static BuyPictureStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		BuyPictureStep() {};
		~BuyPictureStep() {};

		BuyPictureStep(const BuyPictureStep&) {}
		BuyPictureStep& operator=(const BuyPictureStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);
	};


	//--------------------ムービーの選択
	class MovieStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static MovieStep* GetInstance() { static MovieStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		MovieStep() {};
		~MovieStep() {};

		MovieStep(const MovieStep&) {}
		MovieStep& operator=(const MovieStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------ムービー鑑賞
	class MovieWatch : public State<sceneCollect>
	{
	public:
		// 実体_
		static MovieWatch* GetInstance() { static MovieWatch state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		MovieWatch() {};
		~MovieWatch() {};

		MovieWatch(const MovieWatch&) {}
		MovieWatch& operator=(const MovieWatch&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------コンテンツ購入確認(ムービー)
	class BuyMovieStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static BuyMovieStep* GetInstance() { static BuyMovieStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		BuyMovieStep() {};
		~BuyMovieStep() {};

		BuyMovieStep(const BuyMovieStep&) {}
		BuyMovieStep& operator=(const BuyMovieStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);
	};


	//--------------------レコードの選択
	class RecordStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static RecordStep* GetInstance() { static RecordStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		RecordStep() {};
		~RecordStep() {};

		RecordStep(const RecordStep&) {}
		RecordStep& operator=(const RecordStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);


	};

	//--------------------バックメニュー選択
	class BackMenuStep : public State<sceneCollect>
	{
	public:
		// 実体_
		static BackMenuStep* GetInstance() { static BackMenuStep state; return &state; }

		// 入る
		virtual void Enter(sceneCollect* pMain);

		// 実行します
		virtual void Execute(sceneCollect* pMain);

		// 帰る
		virtual void Exit(sceneCollect* pMain);

		// 描画
		virtual void Render(sceneCollect* pMain);

		// エージェントからのメッセージを受信した場合、これが実行される
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		BackMenuStep() {};
		~BackMenuStep() {};

		BackMenuStep(const BackMenuStep&) {}
		BackMenuStep& operator=(const BackMenuStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);
	};
}