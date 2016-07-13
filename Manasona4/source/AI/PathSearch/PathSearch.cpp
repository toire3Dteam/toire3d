#include "TDNLIB.h"
#include "AI\PathSearch\PathSearch.h"
#include "AI\ActionGraph\ActionGraph.h"
#include "AI\FieldGraph\FieldGraph.h"

//*******************************************************************************************************
//planning�p�T��
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
//	���ʂ̃p�X����
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
	// A*�͖ړI�n����ɂ���K�v������̂�
	// ���������Ȃ��悤�ɂ���I
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


	// �X�e�[�g�ǉ�
	// node->m_fieldState = edge->m_fieldState;

	// �����̂܂܂���Run����������Jump�ɏ����ς���Ă��܂��̂�
	// ���ׂ��悪�u���Ă���v�Ȃ��΂��ɉ���
	// �����ꂾ�Ə��Ԃɂ��ʖڂȂ��Ƃ��Ă��܂��\����
	if (node->m_status != NODE_STATUS::CLOSE)
	{
		node->m_fieldState = edge->m_fieldState;
	}

	return ret;
}
