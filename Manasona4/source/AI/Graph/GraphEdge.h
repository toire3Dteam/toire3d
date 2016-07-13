#pragma once
#include "GraphNode.h"

/*
	�ŏ����̃G�b�W(���N���X)
	�m�[�h�ƃm�[�h���q�����킹��
*/
class EdgeBase
{
public:

	// ��̃m�[�h��A��������B
	NodeBase* m_from;
	NodeBase* m_to;
	// ���̃G�b�W���ړ�����R�X�g
	float m_defaultCost; 

	//ctors
	EdgeBase() {}

	virtual ~EdgeBase() {}

	// FlomTo��������
	EdgeBase(NodeBase* f, NodeBase* t)
	{
		m_from = f; m_to = t;

		// ������
		m_defaultCost = 0.0f;
	}

	// (�ǉ�)To�̂�
	EdgeBase(NodeBase* t)
	{
		m_from = nullptr; m_to = t;

		// ������
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

	// �`��
	virtual void Render()
	{



	}

};
