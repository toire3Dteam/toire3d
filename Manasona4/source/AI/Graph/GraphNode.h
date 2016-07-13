#pragma once
#include "TDNLIB.h"


/*
	ノードのインデックスの名前
*/
enum class NODE_INDEX
{
	INVALID = -1,	// 無効
};

/*
	検索時のノードの状態
*/
enum  class NODE_STATUS
{
	NONE,	// まだ一回も調べていない
	OPEN,	// 一回検索したあと
	CLOSE
};

/*
	最小限のノード(基底クラス)
	（ノードとエッジの球の部分）
*/
class EdgeBase;
class NodeBase
{
public:
	int	  m_index;
	float m_cost;						// エッジ感のみのコスト！
	float m_score;						// ここまでスコア

	NodeBase*	m_parent;				// このノードの親
	NODE_STATUS m_status;				// 検索時のノードの状態
	std::list<EdgeBase*> m_edgeList;	// このノードのエッジ

	//Vector3 m_pos; //(仮)
	tdn2DObj* m_obj;// (仮)

	NodeBase() :m_index(-1), m_cost(0.0f),m_score(0.0f),
		m_parent(nullptr), m_status(NODE_STATUS::NONE)
	{
		// 仮に絵を
		m_obj = new tdn2DObj("Data/AI/GraphState.png");
	}

	virtual void Release()
	{
		// エッジを全部消す
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); it++)
		{
			SAFE_DELETE((*it));
		}
		m_edgeList.clear();

		SAFE_DELETE(m_obj);
	}

	virtual void Reset() 
	{
		// コスト・親・状態リセット
		m_cost = .0f;
		m_score = .0f;
		m_parent = nullptr;
		m_status = NODE_STATUS::NONE;
	}

	void ReleaseEdge() 
	{
		// エッジを全部消す
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); it++)
		{
			SAFE_DELETE(*it);
		}
		m_edgeList.clear();

	}
	
	// エッジを入れ込む
	void	InsertEdge(EdgeBase* new_Edge)
	{
		//　(追加)Fromはこのノードじゃないとおかしいでしょ
		// ということで今は強制でfromを変更
		//(NodeBase*)new_Edge->m_from = this;

		m_edgeList.push_back(new_Edge);
	}
	
	// エッジを消す
	void	EraseEdge(EdgeBase* erase_Edge)
	{
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); it++)
		{
			if ((*it) == erase_Edge)
			{
				it = m_edgeList.erase(it);
				break;
			}
		}
	}


	virtual void Render() 
	{
		//m_obj->Render((int)m_pos.x, (int)m_pos.y);

		//tdnText::Draw((int)m_pos.x, (int)m_pos.y - 32, 0xffffffff, "自分の番号-> %d ", m_index);
		//if (m_parent==nullptr)
		//{
		//	tdnText::Draw((int)m_pos.x, (int)m_pos.y, 0xffffffff, "親のノード-> なし ");
		//}
		//else
		//{
		//	tdnText::Draw((int)m_pos.x, (int)m_pos.y, 0xffffffff, "親のノード-> %d ", m_parent->m_index);
		//}

	}

};
