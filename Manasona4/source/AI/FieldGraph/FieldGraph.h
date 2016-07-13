#pragma once
#include "TDNLIB.h"
#include "AI\Graph\Graph.h"

// フィールドのステート
enum class FIELD_STATE
{
	RUN,
	JUMP,
	DOWN
};

/*
	フィールドノード
*/
class FieldNode : public NodeBase
{
public:
	// フィールドならではの変数をここに追加
	// ノードのポジション
	Vector3 m_pos;

	//  フィールド
	FIELD_STATE m_fieldState;

public:
	FieldNode() : NodeBase() { m_fieldState = FIELD_STATE::RUN; }
	~FieldNode() {}

	// このノード
	void	Release()
	{
		NodeBase::Release();
	}

	void	Reset()
	{
		m_fieldState = FIELD_STATE::RUN;// 一応
		NodeBase::Reset();
	}

	// フィールド用の描画
	void Render()
	{
		// ステータスによって絵を変える
		m_obj->SetScale(15.0f);
		switch (m_status)
		{
		case NODE_STATUS::NONE:
			m_obj->Render3D(m_pos, 128, 128, 0, 0, 128, 128,RS::COPY_NOZ);
			break;
		case NODE_STATUS::OPEN:
			m_obj->Render3D(m_pos, 128, 128, 128, 0, 128, 128, RS::COPY_NOZ);
			break;
		case NODE_STATUS::CLOSE:
			m_obj->Render3D(m_pos, 128, 128, 128 * 2, 0, 128, 128, RS::COPY_NOZ);
			break;
		default:
			break;
		}

		// スクリーン座標へ
		Vector2 screenPos = Math::WorldToScreen(m_pos);

		//tdnText::Draw((int)screenPos.x, (int)screenPos.y - 64, 0xffffffff, "自分の番号 %d ", m_index);
		//if (m_parent==nullptr)
		//{
		//	tdnText::Draw((int)screenPos.x, (int)screenPos.y - 32, 0xffffffff, "親のノード なし ");
		//}
		//else
		//{
		//	tdnText::Draw((int)screenPos.x, (int)screenPos.y - 32, 0xffffffff, "親のノード %d ", m_parent->m_index);
		//}
	}
};

/*
	フィールドエッジ
*/

class FieldEdge : public EdgeBase
{
public:

	FIELD_STATE m_fieldState;

	//direction		m_direction;					//エッジの方角
	FieldEdge() : EdgeBase()//, m_direction(direction::none)
	{
		m_fieldState = FIELD_STATE::RUN;
	}

	// FlomToを引数に
	FieldEdge(NodeBase* f, NodeBase* t)
	{
		m_from = f; m_to = t;

		// 初期化
		m_defaultCost = 0.0f;
	}


	// フィールドエッジ用の描画
	void Render()
	{
		FieldNode* from;
		FieldNode* to;

		from = (FieldNode*)m_from;
		to = (FieldNode*)m_to;

		//Vector3 edgePos = (from->m_pos + to->m_pos) / 2;

		//// エッジの親のINDEX分ずらす
		//edgePos.y += from->m_index * 34;

		//tdnText::Draw((int)edgePos.x - (128), (int)edgePos.y, 0xff00ffff,
		//	"[%d]->[%d]のエッジ->[%.1f] ", m_from->m_index, m_to->m_index, m_defaultCost);

		// スクリーン座標へ
		Vector2 screenFromPos = Math::WorldToScreen(from->m_pos);
		Vector2 screenToPos = Math::WorldToScreen(to->m_pos);

		// 線
		tdnPolygon::DrawLine(screenFromPos.x, screenFromPos.y,
			screenToPos.x, screenToPos.y, 0xffffffff, 4.0f);

	}
};


/*
	フィールドグラフ
*/
class FieldGraph :public Graph<FieldNode, FieldEdge>
{
public:
	FieldGraph();
	//~FieldGraph();

	void Planning(FieldNode* startnode, FieldNode* goalnode);
	void MoveTaskSetting(FieldNode* startnode, FieldNode* goalnode);

	// デバッグ描画
	void Render();

	// データ読み込み
	void LoadNode(char* fileName);

	FieldNode* m_GoalFieldNode;

	// 一番距離の近いノードを探す
	FieldNode* NearPosNode(Vector3 myPos);
	// 一番距離の遠いノードを探す
	FieldNode* FarPosNode(Vector3 myPos);

private:




};

