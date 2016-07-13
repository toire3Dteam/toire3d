#pragma once
#include "TDNLIB.h"
#include "GraphNode.h"
#include "GraphEdge.h"
#include "AI\Utility\MyPriority_Queue.h"
#include "AI\PathSearch\PathSearch.h"


// ノードとエッジを一緒にグループ化


//プライオリティキューからノードを取り出すときの関数オブジェクト
template<class Node_Type>
class Cost_Sort
{
public:

	bool	operator ()(const Node_Type* left, const Node_Type* right)
	{
		return (left->m_score > right->m_score);
	}
};



template<class Node_Type, class Edge_Type>
class Graph
{
protected:
	int				m_indexCounter;	// ノードのindexの割り当て用に！
	std::vector<Node_Type*>	m_nodeList;// <-ノードのリスト
										//std::priority_queue<Node_Type*, std::vector<Node_Type*>, Cost_Sort<Node_Type>> m_openQue;
	int				m_numEdge;
	int				m_numNode;
	
	//std::priority_queue<Node_Type, std::vector<Node_Type>, Cost_Sort<Node_Type>> m_openQue;
	MyPriority_Queue<Node_Type, Cost_Sort<Node_Type>> m_openQue;

protected:
	//できれば一定数の関数隠したい
	void		Clear_OpenQue()
	{
		m_openQue.Clear();
	}

public:
	Graph() : m_indexCounter(0), m_numEdge(0), m_numNode(0),
		m_openQue(new Cost_Sort<Node_Type>, 1000)
	{
		m_nodeList.reserve(1000);
		m_nodeList.clear();
	}

	virtual ~Graph()
	{
		// 全ノードを解放（Delete）
		AllRelease();
		m_openQue.Release();
	}

	void	AllRelease()
	{
		for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
		{
			(*it)->Release();
			delete (*it);
		}

		m_nodeList.clear();
		m_numNode = 0;
		m_numEdge = 0;
	}
	void	ResetIndex() { m_indexCounter = 0; }
	

	/*
		Deleteではなく、そのノードのState・親をリセットする関数
	*/
	virtual void	AllReset()
	{
		for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
		{
			(*it)->Reset();
		}
	}

	void	AddNode(Node_Type* new_Node)
	{
		if (new_Node->m_index <0)
			new_Node->m_index = m_indexCounter;
		m_indexCounter++;
		m_numNode++;
		m_nodeList.push_back(new_Node);
		//CONSOLE_PRINT

	}
	
	// エッジ追加
	void	InsertEdge(Node_Type* node, Edge_Type* InsertEdge)
	{
		// ノードクラスにInsertEdgeがあるていで
		node->InsertEdge(InsertEdge);
		m_numEdge++;
	}


	void	Open(Node_Type* insert_Node)
	{
		insert_Node->m_status = NODE_STATUS::OPEN;
		// ☆
		m_openQue.Push(insert_Node);
	}

	void	Close(Node_Type* close_Node)
	{
		close_Node->m_status = NODE_STATUS::CLOSE;
		m_openQue.Pop(close_Node);
	}

	bool	EraseNode(Node_Type* erase_Node)
	{
		auto it = std::find(m_nodeList.begin(), m_nodeList.end(), erase_Node);
		if (it == m_nodeList.end())
		{
			return false;
		}

		//接続関係を消去(あやしい)
		for (auto co = erase_Node->m_edgeList.begin();
		co != erase_Node->m_edgeList.end(); co++)
		{
			for (auto work = (*co)->to->m_edgeList.begin();
			work != (*co)->to->m_edgeList.end(); work++)
			{
				if ((*work)->to == erase_Node)
				{
					(*work)->from->EraseEdge((*work));
					break;
				}
			}
		}
		//ノードを削除
		delete erase_Node;
		//node_List.erase(std::remove_if(node_List.begin(), node_List.end(), erase_Node),node_List.end());
		m_nodeList.erase(it);
		m_numNode--;
	}

	bool	Search(Node_Type* m_startNode, Node_Type* goal_Node)
	{
		// エラーチェック
		if (m_nodeList.empty()) //ノードが何も入っていなかったら
		{
			return false;
		}
			

		// 範囲内で指定された値を持つ要素が最初に出現する位置を検索します。
		// 値をコンテナから探してくれます。
		// 見つかったらその場所のイテレーターが返ってきます。
		// 見つからなかったら、範囲の最後が返ってきます。
		auto it = std::find(m_nodeList.begin(), m_nodeList.end(), m_startNode);
		if (it == m_nodeList.end())// リストにこのノードしか入っていなかったら。
		{
			return false;
		}
			
		// オープンキューを一度初期化（まずは検索前のNONEに戻す）
		m_openQue.Clear();

		// 最初の一回
		// ★スタートノードをOPENに変更してopenQueueに追加
		Open(m_startNode);
		
		//一番スコアの低いノードを取り出す
		// オープンキューの中身が空っぽになるまで無限ループ
		for (; !m_openQue.Empty();)
		{
			// 現在のtopをナウに入れる
			Node_Type* Now;
			Now = m_openQue.Top();//　☆一番スコアの低いノードを取り出す
			Close(Now); // ★一番上のデータをポップします。

			// 現在ナウに設定してるノードのエッジを全部調べる！！！
			for (auto it = Now->m_edgeList.begin(); it != Now->m_edgeList.end(); it++)
			{
				// 接続先のノードさん
				Node_Type* connect = reinterpret_cast<Node_Type*>((*it)->m_to);
				// スコア計算(なんかいろいろする。) （今は現在のノードの合計スコア＋そのエッジのコスト）
				float score = (*it)->m_defaultCost + Now->m_score;

				//　★比較していく
				// 調べた先が「まだ調べていない」なら
				if (connect->m_status == NODE_STATUS::NONE)
				{
					// 接続元の総コスト ＋ エッジ（道のり）のコスト
					connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
					connect->m_score = score;						// 今までのスコアを受け継がせる！
					connect->m_parent = Now;						// 今エッジの伸ばし元になってるナウを親に
					Open(connect);// ★ この接続先をOpenQueueに追加！
					continue;
				}

				// 調べた先が「一度調べた事がある」なら
				if (connect->m_status == NODE_STATUS::OPEN)
				{
					// もし現在のスコアが接続先のノードのスコアより少ないのなら
					// その接続先の親を書き換える！！
					if (connect->m_score > score)
					{
						connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
						connect->m_score = score;
						connect->m_parent = Now;
						Open(connect);
						continue;
					}
				}

				// 調べた先が「閉じている」なら
				if (connect->m_status == NODE_STATUS::CLOSE)
				{
					// もし現在のスコアが接続先のノードのスコアより少ないのなら
					// その接続先の親を書き換える！！
					if (connect->m_score > score)
					{
						connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
						connect->m_score = score;
						connect->m_parent = Now;
						Open(connect);
						continue;
					}
				}
				
			}

			
		}
		// 経路が見つからなかった場合　親の繋がりとOpenを消す。
		//AllReset();

		return true;
	}

	/*
		その２
	*/
	bool	Search(PathSearch_Base<Node_Type, Edge_Type>* search_Alg)
	{
		// （追加）まず全ノードをリセット！！
		AllReset();


		// エラーチェック
		if (m_nodeList.empty()) //ノードが何も入っていなかったら
		{
			return false;
		}

		// 範囲内で指定された値を持つ要素が最初に出現する位置を検索します。
		// 値をコンテナから探してくれます。
		// 見つかったらその場所のイテレーターが返ってきます。
		// 見つからなかったら、範囲の最後が返ってきます。
		auto it = std::find(m_nodeList.begin(), m_nodeList.end(), search_Alg->Get_StartNode());
		if (it == m_nodeList.end())// リストにこのノードしか入っていなかったら。
		{
			return false;
		}
		
		//StartNodeとGoalNodeが同じ場合
		for (auto it = search_Alg->GetGoalList()->begin(); it != search_Alg->GetGoalList()->end(); it++)
		{
			if ((*it) == search_Alg->Get_StartNode())
				return false;
		}



		// オープンキューを一度初期化（まずは検索前のNONEに戻す）
		m_openQue.Clear();

		// 最初の一回
		// ★スタートノードをOPENに変更してopenQueueに追加
		Open(search_Alg->Get_StartNode());

		//一番スコアの低いノードを取り出す
		// オープンキューの中身が空っぽになるまで無限ループ
		for (; !m_openQue.Empty();)
		{
			// 現在のtopをナウに入れる
			Node_Type* Now;
			Now = m_openQue.Top();//　☆一番スコアの低いノードを取り出す
			Close(Now); // ★一番上のデータをポップします。

						// 現在ナウに設定してるノードのエッジを全部調べる！！！
			for (auto it = Now->m_edgeList.begin(); it != Now->m_edgeList.end(); it++)
			{
				// 接続先のノードさん
				Node_Type* connect = reinterpret_cast<Node_Type*>((*it)->m_to);
				// スコア計算(なんかいろいろする。) （今は現在のノードの合計スコア＋そのエッジのコスト）
				// float score = (*it)->m_defaultCost + Now->m_score;
				// それぞれ（A*・ダイクストラ）のスコア計算
				float score = search_Alg->CalculationScore(connect, reinterpret_cast<Edge_Type*>(*it), Now->m_score);

				// ★比較していく
				// 調べた先が「まだ調べていない」なら
				if (connect->m_status == NODE_STATUS::NONE)
				{
					// 接続元の総コスト ＋ エッジ（道のり）のコスト
					connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
					connect->m_score = score;						// 今までのスコアを受け継がせる！
					connect->m_parent = Now;						// 今エッジの伸ばし元になってるナウを親に
					Open(connect);// ★ この接続先をOpenQueueに追加！
					continue;
				}

				// 調べた先が「一度調べた事がある」なら
				if (connect->m_status == NODE_STATUS::OPEN)
				{
					// もし現在のスコアが接続先のノードのスコアより少ないのなら
					// その接続先の親を書き換える！！
					if (connect->m_score > score)
					{
						connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
						connect->m_score = score;
						connect->m_parent = Now;
						Open(connect);
						continue;
					}
				}

				// 調べた先が「閉じている」なら
				if (connect->m_status == NODE_STATUS::CLOSE)
				{
					// もし現在のスコアが接続先のノードのスコアより少ないのなら
					// その接続先の親を書き換える！！
					if (connect->m_score > score)
					{
						connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
						connect->m_score = score;
						connect->m_parent = Now;
						Open(connect);
						continue;
					}
				}

			}

			// ゴール判定
			if (search_Alg->IsGoal(Now))
			{
				//ゴールノード確定
				search_Alg->m_dicisionGoal = Now;
				return true;
			}

		}
		// 経路が見つからなかった場合　親の繋がりとOpenを消す。
		AllReset();

		return false;
	}


	/**
	*@breif ノード検索
	*@param ノードのID
	*/
	//Node_Type* Find_Node(const int& id)
	//{
	//	for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
	//	{
	//		if (id == (*it)->index)
	//			return (*it);
	//	}
	//	return nullptr;
	//}

	/**
	*@breif 最短経路までのエッジをリストに挿入する
	*@param1 ゴールノード
	*@param2 スタートノード
	*@param3 格納するstd::listのポインタ
	*/
	//bool	Set_Route(Node_Type* goal_Node, Node_Type* m_startNode, std::list<Edge_Type*>* route_Edge)
	//{
	//	Node_Type* work = goal_Node;
	//	for (; work != m_startNode;)
	//	{
	//		Node_Type* parent = reinterpret_cast<Node_Type*>(work->parent);
	//		//親がtoになっているEdgeを検索する
	//		for (auto it = parent->m_edgeList.begin(); it != parent->m_edgeList.end(); it++)
	//		{
	//			if ((*it)->to == work)
	//			{
	//				route_Edge->push_front(reinterpret_cast<Edge_Type*>(*it));
	//				work = parent;
	//				break;
	//			}
	//		}
	//	}
	//	return true;
	//}

	int		Get_GraphSize() { return m_nodeList.size(); }

	void NodeListRender() {
		for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
		{
			(*it)->Render();
		}
	}
	void EdgeListRender() {
		for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
		{
			Node_Type* Now = (*it);
			for (auto it2 = Now->m_edgeList.begin(); it2 != Now->m_edgeList.end(); it2++)
			{
				(*it2)->Render();
			}

		}
	}
};
