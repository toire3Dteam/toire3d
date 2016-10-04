#pragma once
#include "../Message/Message.h"
#include "EntityEnum.h"

/*
*	この世界の住人は全てこのクラスから派生します。
*	これはID番号を振られるクラスです。
*	
*
*/

class BaseGameEntity
{


private:

	// どのエンティティも一意のID番号を持つ
	ENTITY_ID m_ID;

	// これは次の有効なID。このクラスが生成される度に自動的にこの値は更新される
	//static int m_isNextValidID;

	// IDが正しく設定されたのかを確認するためにコンストラクタの中で呼ばれる
	// IDを設定し次の有効なIDをインクリメントする前に、これに渡された値が次の有効なID以上であるか確認する。
	//void SetID(int val);

public:
	// ★ＥｎｔｉｔｙＭＧＲと連携をとるやり方にするぜ！
	BaseGameEntity(ENTITY_ID id);
	virtual ~BaseGameEntity();

	// 全てのエンティティにUpdateを引き継ぐよう純粋仮想関数に
	virtual void Update(){};

	/*
		メッセージを作成と配信をする
		どのクラスでもメッセージを使える用に
		HandleMessageを追加する
	*/
	// 全てのサブクラスはメッセージを使って通信する
	virtual bool  HandleMessage(const Message& msg) = 0;

	// アクセサー
	ENTITY_ID GetID() { return m_ID; };

};

