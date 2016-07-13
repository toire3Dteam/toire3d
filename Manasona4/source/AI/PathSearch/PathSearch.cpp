#include "TDNLIB.h"
#include "AI\PathSearch\PathSearch.h"
#include "AI\ActionGraph\ActionGraph.h"
#include "AI\FieldGraph\FieldGraph.h"

//*******************************************************************************************************
//planning用探索
//*******************************************************************************************************
bool	ActionSearch::SetGoalNode(Action_Node* node)
{
	m_goalList.push_back(node);
	return true;
}

bool	ActionSearch::IsGoal(Action_Node* node)
{
	for (auto it = m_goalList.begin(); it != m_goalList.end(); it++)
	{
		if (node == (*it))
			return true;
	}
	return false;
}

float	ActionSearch::CalculationScore(Action_Node*node,
	Action_Edge* edge, const float& score)
{
	return edge->m_defaultCost + score;
}

//*******************************************************************************************************
//	普通のパス検索
//*******************************************************************************************************
bool	Dijkstra_Standard::SetGoalNode(FieldNode* node)
{
	m_goalList.push_back(node);
	return true;
}

bool	Dijkstra_Standard::IsGoal(FieldNode* node)
{
	for (auto it = m_goalList.begin(); it != m_goalList.end(); it++)
	{
		if ((*it) == node)
			return true;
	}
	return false;
}

float	Dijkstra_Standard::CalculationScore(FieldNode*node,
	FieldEdge* edge, const float& score)
{
	float ret;
	if (edge)
	{
		ret = edge->m_defaultCost + score;
	}
	else
	{
		ret = edge->m_defaultCost + score;
	}
	return ret;

}

bool	Astar_Standard::SetGoalNode(FieldNode* node)
{
	// A*は目的地が一つにする必要があるので
	// 複数入れれないようにする！
	if (m_goalList.empty())
	{
		m_goalList.push_back(node);
		return true;
	}

	return false;
}

bool	Astar_Standard::IsGoal(FieldNode* node)
{
	return (node == m_goalList.front()) ? true : false;
}

float	Astar_Standard::CalculationScore(FieldNode*node,
	FieldEdge* edge, const float& score)
{
	float heuristic = (m_goalList.front()->m_pos - node->m_pos).Length();
	float ret;
	if (edge)
	{
		ret = score + edge->m_defaultCost + heuristic;
	}
	else
	{
		ret = score + heuristic;
	}


	// ステート追加
	// node->m_fieldState = edge->m_fieldState;

	// ※このままだとRunだった所がJumpに書き変わってしまうので
	// 調べた先が「閉じている」なら飛ばすに改良
	// ※これだと順番により駄目なことしてしまう可能性が
	if (node->m_status != NODE_STATUS::CLOSE)
	{
		node->m_fieldState = edge->m_fieldState;
	}

	return ret;
}
