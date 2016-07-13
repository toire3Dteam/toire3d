#pragma once

/*
	継承用
	純粋仮想関数で
*/

//class Miner;	// 鉄夫

struct Message;// 

// State基底クラスをキャラクタ毎に再利用可能に
// クラステンプレートにすると人生が楽になる。
template <class entity_type>
class State
{
public:
	virtual ~State() {};

	// 入る
	virtual void Enter(entity_type*) = 0;

	// 実行します
	virtual void Execute(entity_type*) = 0;

	// 帰る
	virtual void Exit(entity_type*) = 0;

	
	// [追加]描画
	virtual void Render(entity_type*){}

	// 画像とかの描画が必要になったので、2Dの描画も作りました
	virtual void Render2D(entity_type*){}

	/*
		BaseGameEntityのステートがメッセージを受け取り
		処理を選択できるように修正する必要がある
	*/
	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(entity_type*, const Message&) = 0;

};
