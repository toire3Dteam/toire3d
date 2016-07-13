#pragma once
#include "TDNLIB.h"


/*
	�m�[�h�̃C���f�b�N�X�̖��O
*/
enum class NODE_INDEX
{
	INVALID = -1,	// ����
};

/*
	�������̃m�[�h�̏��
*/
enum  class NODE_STATUS
{
	NONE,	// �܂��������ׂĂ��Ȃ�
	OPEN,	// ��񌟍���������
	CLOSE
};

/*
	�ŏ����̃m�[�h(���N���X)
	�i�m�[�h�ƃG�b�W�̋��̕����j
*/
class EdgeBase;
class NodeBase
{
public:
	int	  m_index;
	float m_cost;						// �G�b�W���݂̂̃R�X�g�I
	float m_score;						// �����܂ŃX�R�A

	NodeBase*	m_parent;				// ���̃m�[�h�̐e
	NODE_STATUS m_status;				// �������̃m�[�h�̏��
	std::list<EdgeBase*> m_edgeList;	// ���̃m�[�h�̃G�b�W

	//Vector3 m_pos; //(��)
	tdn2DObj* m_obj;// (��)

	NodeBase() :m_index(-1), m_cost(0.0f),m_score(0.0f),
		m_parent(nullptr), m_status(NODE_STATUS::NONE)
	{
		// ���ɊG��
		m_obj = new tdn2DObj("Data/AI/GraphState.png");
	}

	virtual void Release()
	{
		// �G�b�W��S������
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); it++)
		{
			SAFE_DELETE((*it));
		}
		m_edgeList.clear();

		SAFE_DELETE(m_obj);
	}

	virtual void Reset() 
	{
		// �R�X�g�E�e�E��ԃ��Z�b�g
		m_cost = .0f;
		m_score = .0f;
		m_parent = nullptr;
		m_status = NODE_STATUS::NONE;
	}

	void ReleaseEdge() 
	{
		// �G�b�W��S������
		for (auto it = m_edgeList.begin(); it != m_edgeList.end(); it++)
		{
			SAFE_DELETE(*it);
		}
		m_edgeList.clear();

	}
	
	// �G�b�W����ꍞ��
	void	InsertEdge(EdgeBase* new_Edge)
	{
		//�@(�ǉ�)From�͂��̃m�[�h����Ȃ��Ƃ��������ł���
		// �Ƃ������Ƃō��͋�����from��ύX
		//(NodeBase*)new_Edge->m_from = this;

		m_edgeList.push_back(new_Edge);
	}
	
	// �G�b�W������
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

		//tdnText::Draw((int)m_pos.x, (int)m_pos.y - 32, 0xffffffff, "�����̔ԍ�-> %d ", m_index);
		//if (m_parent==nullptr)
		//{
		//	tdnText::Draw((int)m_pos.x, (int)m_pos.y, 0xffffffff, "�e�̃m�[�h-> �Ȃ� ");
		//}
		//else
		//{
		//	tdnText::Draw((int)m_pos.x, (int)m_pos.y, 0xffffffff, "�e�̃m�[�h-> %d ", m_parent->m_index);
		//}

	}

};
