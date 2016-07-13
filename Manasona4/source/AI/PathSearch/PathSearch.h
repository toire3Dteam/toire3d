#pragma once
#include "TDNLIB.h"


/*�@�x�[�X�T�[�`�@*/

template<class Node_Type, class Edge_Type>
class PathSearch_Base
{
protected:
	// ��A*�Ȃ��_�C�N�X�g���Ȃ畡���S�[����ݒ�ł���̂Ń��X�g��
	std::list<Node_Type*> m_goalList;				//�S�[�����̃��X�g
public:
	Node_Type*						m_dicisionGoal;	//�o�H�T����̃S�[���m�[�h
	Node_Type*						m_startNode;		//�X�^�[�g�m�[�h

	//�@�R���X�g���N�^
	PathSearch_Base() : m_dicisionGoal(nullptr), m_startNode(nullptr) { m_goalList.clear(); }
	//�@�S�[�����X�g�N���A
	void	Clear() { m_goalList.clear(); }
	//�@Get�S�[�����X�g
	std::list<Node_Type*>*	GetGoalList() { return &m_goalList; }
	//�@Set�X�^�[�g�m�[�h
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
	//	�p����Őݒ��ς���
	/***********************/

	/**
	*@breif	�S�[���m�[�h��ǉ�����
	*@param1 �S�[���m�[�h
	*@return ������true
	*@note�@Astar�̏ꍇ�̓S�[���͈��
	*/
	virtual bool	SetGoalNode(Node_Type* node) = 0;
	/**
	*@brief �S�[������
	*@param ���肵�����m�[�h
	*@return true�ŃS�[��
	*/
	virtual bool	IsGoal(Node_Type* node) = 0;
	/**
	*@breif �m�[�h�̃R�X�g�v�Z
	*@param1 �v�Z�������m�[�h
	*@param2 ���̃m�[�h�ɐڑ�����Ă���G�b�W
	*@param3 ���̃m�[�h�܂ł�score
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
	�t�B�[���h�p�@�_�C�N�X�g��
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
	�t�B�[���h�p�@A*
*/
class Astar_Standard : public PathSearch_Base<FieldNode, FieldEdge>
{
public:
	bool	SetGoalNode(FieldNode* node);
	bool	IsGoal(FieldNode* node);
	float	CalculationScore(FieldNode*node,
		FieldEdge* edge, const float& score);
};


