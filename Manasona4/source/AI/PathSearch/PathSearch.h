#pragma once
#include "TDNLIB.h"


/*　ベースサーチ　*/

template<class Node_Type, class Edge_Type>
class PathSearch_Base
{
protected:
	// ★A*なら一つダイクストラなら複数ゴールを設定できるのでリストに
	std::list<Node_Type*> m_goalList;				//ゴール候補のリスト
public:
	Node_Type*						m_dicisionGoal;	//経路探索後のゴールノード
	Node_Type*						m_startNode;		//スタートノード

	//　コンストラクタ
	PathSearch_Base() : m_dicisionGoal(nullptr), m_startNode(nullptr) { m_goalList.clear(); }
	//　ゴールリストクリア
	void	Clear() { m_goalList.clear(); }
	//　Getゴールリスト
	std::list<Node_Type*>*	GetGoalList() { return &m_goalList; }
	//　Setスタートノード
	bool	SetStartNode(Node_Type* insert)
	{
		//if (!m_startNode)
		//{
			m_startNode = insert;
			return true;
		//}
		//return false;
	}

	Node_Type* Get_StartNode()const
	{ return m_startNode; }


	/***********************/
	//	継承先で設定を変える
	/***********************/

	/**
	*@breif	ゴールノードを追加する
	*@param1 ゴールノード
	*@return 成功でtrue
	*@note　Astarの場合はゴールは一つ
	*/
	virtual bool	SetGoalNode(Node_Type* node) = 0;
	/**
	*@brief ゴール判定
	*@param 判定したいノード
	*@return trueでゴール
	*/
	virtual bool	IsGoal(Node_Type* node) = 0;
	/**
	*@breif ノードのコスト計算
	*@param1 計算したいノード
	*@param2 そのノードに接続されているエッジ
	*@param3 そのノードまでのscore
	*/
	virtual float	CalculationScore(Node_Type*node,
		Edge_Type* edge, const float& score) = 0;

};


class Action_Node;
class Action_Edge;

class FieldNode;
class FieldEdge;

class ActionSearch : public PathSearch_Base<Action_Node, Action_Edge>
{
public:
	bool	SetGoalNode(Action_Node* node);
	bool	IsGoal(Action_Node* node);
	float	CalculationScore(Action_Node*node,
		Action_Edge* edge, const float& score);
};

/*
	フィールド用　ダイクストラ
*/
class Dijkstra_Standard : public PathSearch_Base<FieldNode, FieldEdge>
{
public:
	bool	SetGoalNode(FieldNode* node);
	bool	IsGoal(FieldNode* node);
	float	CalculationScore(FieldNode*node,
		FieldEdge* edge, const float& score);
};

/*
	フィールド用　A*
*/
class Astar_Standard : public PathSearch_Base<FieldNode, FieldEdge>
{
public:
	bool	SetGoalNode(FieldNode* node);
	bool	IsGoal(FieldNode* node);
	float	CalculationScore(FieldNode*node,
		FieldEdge* edge, const float& score);
};


