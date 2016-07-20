#pragma once

#include <cassert>

// 元のステート
#include "State.h"


/*
	ステートマシンクラスの中にステート関連の全てを
	カプセル化するかなりすっきりする

	内容：
	エージェントは自分自身(テンプレート)のステートマシンの実態を所有し
	・現在のステート
	・グローバルステート
	・前回のステート（ステートブリップ）
	を委譲することができる。
*/


template <class entity_type>
class StateMachine
{
private:
	// この実体を所有するエージェントへのポインタ
	entity_type* m_pOwner;

	// 現在のステートを保持する
	State<entity_type>* m_pCurrentState;

	// 前回のステート
	State<entity_type>* m_pPreviousState;

	// どんなステートでも更新されるたびに呼ばれるステート
	State<entity_type>* m_pGlobalState;

public:

	// コンストラクタ（初期設定）
	StateMachine(entity_type* owner) :
		m_pOwner(owner),// ここでエージェントのポインタを入れる
		m_pCurrentState(NULL),
		m_pPreviousState(NULL),
		m_pGlobalState(NULL)
	{}

	// FSMを初期化するときなどに使うセッター
	void SetCurrentState(State<entity_type>* s) { MyAssert(s, "SetCurrentState　中身がNULL"); m_pCurrentState = s; m_pCurrentState->Enter(m_pOwner); }	// Enterを追加
	void SetGlobalState(State<entity_type>* s) { MyAssert(s, "SetGlobalState　中身がNULL"); m_pGlobalState = s; }
	void SetPreviousState(State<entity_type>* s) { m_pPreviousState = s; }

	// FSMを更新するときにこれを呼ぶ
	void  Update()const
	{
		// グローバルステートがあるなら、実行
		if (m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);

		//　現在のステートの実行関数
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	// FSMによる描画
	void  Render()const
	{
		// グローバルステートがあるなら、実行
		if (m_pGlobalState)   m_pGlobalState->Render(m_pOwner);

		//　現在のステートの実行関数
		if (m_pCurrentState) m_pCurrentState->Render(m_pOwner);
	}

	// 画像とかの描画が必要になったので、2Dの描画も作りました
	void  Render2D()const
	{
		// グローバルステートがあるなら、実行
		if (m_pGlobalState)   m_pGlobalState->Render2D(m_pOwner);

		//　現在のステートの実行関数
		if (m_pCurrentState) m_pCurrentState->Render2D(m_pOwner);
	}

	// ★メッセージ
	bool  HandleMessage(const Message& msg)const
	{
		// 最初に現在のステートが有効で、メッセージの処理ができるか
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		//　有効でないとき、グローバルステートが実装されいるなら
		//　グローバルステートへメッセージを送る
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		return false;
	}


	// ★New新しいステートに変更
	void ChangeState(State<entity_type> * pNewState)
	{
		// 両立のステートが有効かを確かめる
		assert(m_pCurrentState&&pNewState);

		// 以前のステートを保存
		m_pPreviousState = m_pCurrentState;

		// 存在するステートの終了関数を呼ぶ
		m_pCurrentState->Exit(m_pOwner);

		// 新しいステートに変更
		m_pCurrentState = pNewState;

		// 新しいステートの開始関数を呼ぶ
		m_pCurrentState->Enter(m_pOwner);

	}

	// 以前のステートに変更（ひとつ前のステートに戻す）
	void  RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}


	// アクセサ
	State<entity_type>*  GetCurrentState()  const { return m_pCurrentState; }
	State<entity_type>*  GetGlobalState()   const { return m_pGlobalState; }
	State<entity_type>*  GetPreviousState() const { return m_pPreviousState; }


	// 現在のステートの型とパラメータで渡されたクラスの型が同じならTrueを返す
	bool  isInState(const State<entity_type>& st)const
	{
		if (typeid(*m_pCurrentState) == typeid(st)) return true;
		return false;
	}
	bool  isPrevState(const State<entity_type>& st)const
	{
		if (typeid(*m_pPreviousState) == typeid(st)) return true;
		return false;
	}
};
