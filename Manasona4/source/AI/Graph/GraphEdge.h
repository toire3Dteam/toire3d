#pragma once
#include "GraphNode.h"

/*
	最小限のエッジ(基底クラス)
	ノードとノードを繋ぎ合わせる
*/
class EdgeBase
{
public:

	// 二つのノードを連結させる。
	NodeBase* m_from;
	NodeBase* m_to;
	// このエッジを移動するコスト
	float m_defaultCost; 

	//ctors
	EdgeBase() {}

	virtual ~EdgeBase() {}

	// FlomToを引数に
	EdgeBase(NodeBase* f, NodeBase* t)
	{
		m_from = f; m_to = t;

		// 初期化
		m_defaultCost = 0.0f;
	}

	// (追加)Toのみ
	EdgeBase(NodeBase* t)
	{
		m_from = nullptr; m_to = t;

		// 初期化
		m_defaultCost = 0.0f;
	}


	void	SetCost(const float& c)
	{
		m_defaultCost = c;
	}

	void	SetingRoute(NodeBase* from, NodeBase* to)
	{
		this->m_from = from;	this->m_to = to;
	}

	// 描画
	virtual void Render()
	{



	}

};
