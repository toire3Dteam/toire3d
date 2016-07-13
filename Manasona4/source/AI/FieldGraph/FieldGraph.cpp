#include "FieldGraph.h"
#include "system\ItDebug.h"

FieldGraph::FieldGraph() :Graph()
{
	// NULL������
	m_GoalFieldNode = nullptr;


}


void FieldGraph::Planning(FieldNode * startnode, FieldNode * goalnode)
{
	// A*���̗p  Dijkstra_Standard Astar_Standard
	Astar_Standard search;
	search.SetStartNode(startnode);
	search.SetGoalNode(goalnode);

	//PathSearch_Base<Field_Node, Field_Edge>* a;
	if (Graph::Search((PathSearch_Base<FieldNode, FieldEdge>*)&search) == true)
	{
		m_GoalFieldNode = search.m_dicisionGoal;
	}
	else //���s������@NULL��
	{
		MyDebugString("�o�H�T�����s�I\n");
		m_GoalFieldNode = nullptr;
	}

}

void FieldGraph::MoveTaskSetting(FieldNode * startnode, FieldNode * goalnode)
{


}

// �`��
void FieldGraph::Render()
{
	NodeListRender();
	EdgeListRender();
}


// �e�L�X�g����m�[�h�ƃG�b�W��ǉ�
void FieldGraph::LoadNode(char* fileName)
{
	std::ifstream in(fileName);

	int nodeMAX = 0;
	in >> nodeMAX;

	// �m�[�h�t�F�C�Y
	for (int i = 0; i < nodeMAX; i++)
	{
		int id = i; // For������Index�ǉ�
		Vector3 nodePos = VECTOR_ZERO;
		in >> nodePos.x;
		in >> nodePos.y;
		in >> nodePos.z;

		FieldNode* node;
		node = new FieldNode();
		node->m_pos = nodePos;
		
		AddNode(node);

	}

	// �G�b�W�t�F�C�Y
	for (int i = 0; i < nodeMAX; i++)
	{

		// �G�b�W�ő及�L��
		int edgeMax = 0;
		in >> edgeMax ;

		for (int e = 0; e < edgeMax; e++)
		{
			FieldEdge* insert;
			insert = new FieldEdge();

			int myID = i;
			int toID = 0;
			in >> toID;
			int state;
			in >> (int)state;// �ǉ�

			insert->m_from = m_nodeList[myID];
			insert->m_to = m_nodeList[toID];
			insert->m_defaultCost = 0;
			insert->m_fieldState = (FIELD_STATE)state;// �ǉ�

			InsertEdge(m_nodeList[myID], insert);

		}

	}
}



// ��ԋ����̋߂��m�[�h��T��
FieldNode* FieldGraph::NearPosNode(Vector3 myPos)
{
	FieldNode* nearNode = nullptr;

	float len2 = FLT_MAX;
	for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
	{
		float len = ((*it)->m_pos - myPos).Length();

		if (len < len2)
		{
			len2 = len;
			// �ڂ�
			nearNode = (*it);
		}
	}


	return nearNode;
}

// ��ԋ����̉����m�[�h��T��
FieldNode* FieldGraph::FarPosNode(Vector3 myPos)
{
	FieldNode* farNode = nullptr;

	float len2 = FLT_MIN;
	for (auto it = m_nodeList.begin(); it != m_nodeList.end(); it++)
	{
		float len = ((*it)->m_pos - myPos).Length();

		if (len > len2)
		{
			len2 = len;
			// �ڂ�
			farNode = (*it);
		}
	}


	return farNode;
}



