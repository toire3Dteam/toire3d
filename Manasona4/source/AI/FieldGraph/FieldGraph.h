#pragma once
#include "TDNLIB.h"
#include "AI\Graph\Graph.h"

// �t�B�[���h�̃X�e�[�g
enum class FIELD_STATE
{
	RUN,
	JUMP,
	DOWN
};

/*
	�t�B�[���h�m�[�h
*/
class FieldNode : public NodeBase
{
public:
	// �t�B�[���h�Ȃ�ł͂̕ϐ��������ɒǉ�
	// �m�[�h�̃|�W�V����
	Vector3 m_pos;

	//  �t�B�[���h
	FIELD_STATE m_fieldState;

public:
	FieldNode() : NodeBase() { m_fieldState = FIELD_STATE::RUN; }
	~FieldNode() {}

	// ���̃m�[�h
	void	Release()
	{
		NodeBase::Release();
	}

	void	Reset()
	{
		m_fieldState = FIELD_STATE::RUN;// �ꉞ
		NodeBase::Reset();
	}

	// �t�B�[���h�p�̕`��
	void Render()
	{
		// �X�e�[�^�X�ɂ���ĊG��ς���
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

		// �X�N���[�����W��
		Vector2 screenPos = Math::WorldToScreen(m_pos);

		//tdnText::Draw((int)screenPos.x, (int)screenPos.y - 64, 0xffffffff, "�����̔ԍ� %d ", m_index);
		//if (m_parent==nullptr)
		//{
		//	tdnText::Draw((int)screenPos.x, (int)screenPos.y - 32, 0xffffffff, "�e�̃m�[�h �Ȃ� ");
		//}
		//else
		//{
		//	tdnText::Draw((int)screenPos.x, (int)screenPos.y - 32, 0xffffffff, "�e�̃m�[�h %d ", m_parent->m_index);
		//}
	}
};

/*
	�t�B�[���h�G�b�W
*/

class FieldEdge : public EdgeBase
{
public:

	FIELD_STATE m_fieldState;

	//direction		m_direction;					//�G�b�W�̕��p
	FieldEdge() : EdgeBase()//, m_direction(direction::none)
	{
		m_fieldState = FIELD_STATE::RUN;
	}

	// FlomTo��������
	FieldEdge(NodeBase* f, NodeBase* t)
	{
		m_from = f; m_to = t;

		// ������
		m_defaultCost = 0.0f;
	}


	// �t�B�[���h�G�b�W�p�̕`��
	void Render()
	{
		FieldNode* from;
		FieldNode* to;

		from = (FieldNode*)m_from;
		to = (FieldNode*)m_to;

		//Vector3 edgePos = (from->m_pos + to->m_pos) / 2;

		//// �G�b�W�̐e��INDEX�����炷
		//edgePos.y += from->m_index * 34;

		//tdnText::Draw((int)edgePos.x - (128), (int)edgePos.y, 0xff00ffff,
		//	"[%d]->[%d]�̃G�b�W->[%.1f] ", m_from->m_index, m_to->m_index, m_defaultCost);

		// �X�N���[�����W��
		Vector2 screenFromPos = Math::WorldToScreen(from->m_pos);
		Vector2 screenToPos = Math::WorldToScreen(to->m_pos);

		// ��
		tdnPolygon::DrawLine(screenFromPos.x, screenFromPos.y,
			screenToPos.x, screenToPos.y, 0xffffffff, 4.0f);

	}
};


/*
	�t�B�[���h�O���t
*/
class FieldGraph :public Graph<FieldNode, FieldEdge>
{
public:
	FieldGraph();
	//~FieldGraph();

	void Planning(FieldNode* startnode, FieldNode* goalnode);
	void MoveTaskSetting(FieldNode* startnode, FieldNode* goalnode);

	// �f�o�b�O�`��
	void Render();

	// �f�[�^�ǂݍ���
	void LoadNode(char* fileName);

	FieldNode* m_GoalFieldNode;

	// ��ԋ����̋߂��m�[�h��T��
	FieldNode* NearPosNode(Vector3 myPos);
	// ��ԋ����̉����m�[�h��T��
	FieldNode* FarPosNode(Vector3 myPos);

private:




};

