#include "FieldGraph.h"
#include "system\ItDebug.h"

FieldGraph::FieldGraph() :Graph()
{
	// NULL初期化
	m_GoalFieldNode = nullptr;


}


void FieldGraph::Planning(FieldNode * startnode, FieldNode * goalnode)
{
	// A*を採用  Dijkstra_Standard Astar_Standard
	Astar_Standard search;
	search.SetStartNode(startnode);
	search.SetGoalNode(goalnode);

	//PathSearch_Base<Field_Node, Field_Edge>* a;
	if (Graph::Search((PathSearch_Base<FieldNode, FieldEdge>*)&search) == true)
	{
		m_GoalFieldNode = search.m_dicisionGoal;
	}
	else //失敗したら　NULLに
	{
		MyDebugString("経路探索失敗！\n");
		m_GoalFieldNode = nullptr;
	}

}

void FieldGraph::MoveTaskSetting(FieldNode * startnode, FieldNode * goalnode)
{


}

// 描画
void FieldGraph::Render()
{
	NodeListRender();
	EdgeListRender();
}


// テキストからノードとエッジを追加
void FieldGraph::LoadNode(char* fileName)
{
	std::ifstream in(fileName);

	int nodeMAX = 0;
	in >> nodeMAX;

	// ノードフェイズ
	for (int i = 0; i < nodeMAX; i++)
	{
		int id = i; // For文順にIndex追加
		Vector3 nodePos = VECTOR_ZERO;
		in >> nodePos.x;
		in >> nodePos.y;
		in >> nodePos.z;

		FieldNode* node;
		node = new FieldNode();
		node->m_pos = nodePos;
		
		AddNode(node);

	}

	// エッジフェイズ
	for (int i = 0; i < nodeMAX; i++)
	{

		// エッジ最大所有数
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
			in >> (int)state;// 追加

			insert->m_from = m_nodeList[myID];
			insert->m_to = m_nodeList[toID];
			insert->m_defaultCost = 0;
			insert->m_fieldState = (FIELD_STATE)state;// 追加

			InsertEdge(m_nodeList[myID], insert);

		}

	}
}



// 一番距離の近いノードを探す
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
			// 移す
			nearNode = (*it);
		}
	}


	return nearNode;
}

// 一番距離の遠いノードを探す
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
			// 移す
			farNode = (*it);
		}
	}


	return farNode;
}



