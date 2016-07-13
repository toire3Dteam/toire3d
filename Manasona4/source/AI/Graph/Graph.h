#pragma once
#include "TDNLIB.h"
#include "GraphNode.h"
#include "GraphEdge.h"
#include "AI\Utility\MyPriority_Queue.h"
#include "AI\PathSearch\PathSearch.h"


// �m�[�h�ƃG�b�W���ꏏ�ɃO���[�v��


//�v���C�I���e�B�L���[����m�[�h�����o���Ƃ��̊֐��I�u�W�F�N�g
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
	int				m_indexCounter;	// �m�[�h��index�̊��蓖�ėp�ɁI
	std::vector<Node_Type*>	m_nodeList;// <-�m�[�h�̃��X�g
										//std::priority_queue<Node_Type*, std::vector<Node_Type*>, Cost_Sort<Node_Type>> m_openQue;
	int				m_numEdge;
	int				m_numNode;
	
	//std::priority_queue<Node_Type, std::vector<Node_Type>, Cost_Sort<Node_Type>> m_openQue;
	MyPriority_Queue<Node_Type, Cost_Sort<Node_Type>> m_openQue;

protected:
	//�ł���Έ�萔�̊֐��B������
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
		// �S�m�[�h������iDelete�j
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
		Delete�ł͂Ȃ��A���̃m�[�h��State�E�e�����Z�b�g����֐�
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
	
	// �G�b�W�ǉ�
	void	InsertEdge(Node_Type* node, Edge_Type* InsertEdge)
	{
		// �m�[�h�N���X��InsertEdge������Ă���
		node->InsertEdge(InsertEdge);
		m_numEdge++;
	}


	void	Open(Node_Type* insert_Node)
	{
		insert_Node->m_status = NODE_STATUS::OPEN;
		// ��
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

		//�ڑ��֌W������(���₵��)
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
		//�m�[�h���폜
		delete erase_Node;
		//node_List.erase(std::remove_if(node_List.begin(), node_List.end(), erase_Node),node_List.end());
		m_nodeList.erase(it);
		m_numNode--;
	}

	bool	Search(Node_Type* m_startNode, Node_Type* goal_Node)
	{
		// �G���[�`�F�b�N
		if (m_nodeList.empty()) //�m�[�h�����������Ă��Ȃ�������
		{
			return false;
		}
			

		// �͈͓��Ŏw�肳�ꂽ�l�����v�f���ŏ��ɏo������ʒu���������܂��B
		// �l���R���e�i����T���Ă���܂��B
		// ���������炻�̏ꏊ�̃C�e���[�^�[���Ԃ��Ă��܂��B
		// ������Ȃ�������A�͈͂̍Ōオ�Ԃ��Ă��܂��B
		auto it = std::find(m_nodeList.begin(), m_nodeList.end(), m_startNode);
		if (it == m_nodeList.end())// ���X�g�ɂ��̃m�[�h���������Ă��Ȃ�������B
		{
			return false;
		}
			
		// �I�[�v���L���[����x�������i�܂��͌����O��NONE�ɖ߂��j
		m_openQue.Clear();

		// �ŏ��̈��
		// ���X�^�[�g�m�[�h��OPEN�ɕύX����openQueue�ɒǉ�
		Open(m_startNode);
		
		//��ԃX�R�A�̒Ⴂ�m�[�h�����o��
		// �I�[�v���L���[�̒��g������ۂɂȂ�܂Ŗ������[�v
		for (; !m_openQue.Empty();)
		{
			// ���݂�top���i�E�ɓ����
			Node_Type* Now;
			Now = m_openQue.Top();//�@����ԃX�R�A�̒Ⴂ�m�[�h�����o��
			Close(Now); // ����ԏ�̃f�[�^���|�b�v���܂��B

			// ���݃i�E�ɐݒ肵�Ă�m�[�h�̃G�b�W��S�����ׂ�I�I�I
			for (auto it = Now->m_edgeList.begin(); it != Now->m_edgeList.end(); it++)
			{
				// �ڑ���̃m�[�h����
				Node_Type* connect = reinterpret_cast<Node_Type*>((*it)->m_to);
				// �X�R�A�v�Z(�Ȃ񂩂��낢�낷��B) �i���͌��݂̃m�[�h�̍��v�X�R�A�{���̃G�b�W�̃R�X�g�j
				float score = (*it)->m_defaultCost + Now->m_score;

				//�@����r���Ă���
				// ���ׂ��悪�u�܂����ׂĂ��Ȃ��v�Ȃ�
				if (connect->m_status == NODE_STATUS::NONE)
				{
					// �ڑ����̑��R�X�g �{ �G�b�W�i���̂�j�̃R�X�g
					connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
					connect->m_score = score;						// ���܂ł̃X�R�A���󂯌p������I
					connect->m_parent = Now;						// ���G�b�W�̐L�΂����ɂȂ��Ă�i�E��e��
					Open(connect);// �� ���̐ڑ����OpenQueue�ɒǉ��I
					continue;
				}

				// ���ׂ��悪�u��x���ׂ���������v�Ȃ�
				if (connect->m_status == NODE_STATUS::OPEN)
				{
					// �������݂̃X�R�A���ڑ���̃m�[�h�̃X�R�A��菭�Ȃ��̂Ȃ�
					// ���̐ڑ���̐e������������I�I
					if (connect->m_score > score)
					{
						connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
						connect->m_score = score;
						connect->m_parent = Now;
						Open(connect);
						continue;
					}
				}

				// ���ׂ��悪�u���Ă���v�Ȃ�
				if (connect->m_status == NODE_STATUS::CLOSE)
				{
					// �������݂̃X�R�A���ڑ���̃m�[�h�̃X�R�A��菭�Ȃ��̂Ȃ�
					// ���̐ڑ���̐e������������I�I
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
		// �o�H��������Ȃ������ꍇ�@�e�̌q�����Open�������B
		//AllReset();

		return true;
	}

	/*
		���̂Q
	*/
	bool	Search(PathSearch_Base<Node_Type, Edge_Type>* search_Alg)
	{
		// �i�ǉ��j�܂��S�m�[�h�����Z�b�g�I�I
		AllReset();


		// �G���[�`�F�b�N
		if (m_nodeList.empty()) //�m�[�h�����������Ă��Ȃ�������
		{
			return false;
		}

		// �͈͓��Ŏw�肳�ꂽ�l�����v�f���ŏ��ɏo������ʒu���������܂��B
		// �l���R���e�i����T���Ă���܂��B
		// ���������炻�̏ꏊ�̃C�e���[�^�[���Ԃ��Ă��܂��B
		// ������Ȃ�������A�͈͂̍Ōオ�Ԃ��Ă��܂��B
		auto it = std::find(m_nodeList.begin(), m_nodeList.end(), search_Alg->Get_StartNode());
		if (it == m_nodeList.end())// ���X�g�ɂ��̃m�[�h���������Ă��Ȃ�������B
		{
			return false;
		}
		
		//StartNode��GoalNode�������ꍇ
		for (auto it = search_Alg->GetGoalList()->begin(); it != search_Alg->GetGoalList()->end(); it++)
		{
			if ((*it) == search_Alg->Get_StartNode())
				return false;
		}



		// �I�[�v���L���[����x�������i�܂��͌����O��NONE�ɖ߂��j
		m_openQue.Clear();

		// �ŏ��̈��
		// ���X�^�[�g�m�[�h��OPEN�ɕύX����openQueue�ɒǉ�
		Open(search_Alg->Get_StartNode());

		//��ԃX�R�A�̒Ⴂ�m�[�h�����o��
		// �I�[�v���L���[�̒��g������ۂɂȂ�܂Ŗ������[�v
		for (; !m_openQue.Empty();)
		{
			// ���݂�top���i�E�ɓ����
			Node_Type* Now;
			Now = m_openQue.Top();//�@����ԃX�R�A�̒Ⴂ�m�[�h�����o��
			Close(Now); // ����ԏ�̃f�[�^���|�b�v���܂��B

						// ���݃i�E�ɐݒ肵�Ă�m�[�h�̃G�b�W��S�����ׂ�I�I�I
			for (auto it = Now->m_edgeList.begin(); it != Now->m_edgeList.end(); it++)
			{
				// �ڑ���̃m�[�h����
				Node_Type* connect = reinterpret_cast<Node_Type*>((*it)->m_to);
				// �X�R�A�v�Z(�Ȃ񂩂��낢�낷��B) �i���͌��݂̃m�[�h�̍��v�X�R�A�{���̃G�b�W�̃R�X�g�j
				// float score = (*it)->m_defaultCost + Now->m_score;
				// ���ꂼ��iA*�E�_�C�N�X�g���j�̃X�R�A�v�Z
				float score = search_Alg->CalculationScore(connect, reinterpret_cast<Edge_Type*>(*it), Now->m_score);

				// ����r���Ă���
				// ���ׂ��悪�u�܂����ׂĂ��Ȃ��v�Ȃ�
				if (connect->m_status == NODE_STATUS::NONE)
				{
					// �ڑ����̑��R�X�g �{ �G�b�W�i���̂�j�̃R�X�g
					connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
					connect->m_score = score;						// ���܂ł̃X�R�A���󂯌p������I
					connect->m_parent = Now;						// ���G�b�W�̐L�΂����ɂȂ��Ă�i�E��e��
					Open(connect);// �� ���̐ڑ����OpenQueue�ɒǉ��I
					continue;
				}

				// ���ׂ��悪�u��x���ׂ���������v�Ȃ�
				if (connect->m_status == NODE_STATUS::OPEN)
				{
					// �������݂̃X�R�A���ڑ���̃m�[�h�̃X�R�A��菭�Ȃ��̂Ȃ�
					// ���̐ڑ���̐e������������I�I
					if (connect->m_score > score)
					{
						connect->m_cost = Now->m_cost + (*it)->m_defaultCost;
						connect->m_score = score;
						connect->m_parent = Now;
						Open(connect);
						continue;
					}
				}

				// ���ׂ��悪�u���Ă���v�Ȃ�
				if (connect->m_status == NODE_STATUS::CLOSE)
				{
					// �������݂̃X�R�A���ڑ���̃m�[�h�̃X�R�A��菭�Ȃ��̂Ȃ�
					// ���̐ڑ���̐e������������I�I
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

			// �S�[������
			if (search_Alg->IsGoal(Now))
			{
				//�S�[���m�[�h�m��
				search_Alg->m_dicisionGoal = Now;
				return true;
			}

		}
		// �o�H��������Ȃ������ꍇ�@�e�̌q�����Open�������B
		AllReset();

		return false;
	}


	/**
	*@breif �m�[�h����
	*@param �m�[�h��ID
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
	*@breif �ŒZ�o�H�܂ł̃G�b�W�����X�g�ɑ}������
	*@param1 �S�[���m�[�h
	*@param2 �X�^�[�g�m�[�h
	*@param3 �i�[����std::list�̃|�C���^
	*/
	//bool	Set_Route(Node_Type* goal_Node, Node_Type* m_startNode, std::list<Edge_Type*>* route_Edge)
	//{
	//	Node_Type* work = goal_Node;
	//	for (; work != m_startNode;)
	//	{
	//		Node_Type* parent = reinterpret_cast<Node_Type*>(work->parent);
	//		//�e��to�ɂȂ��Ă���Edge����������
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
