#pragma once

// �O���錾
namespace CollisionShape
{
	class Square;
}
namespace Stage
{
	class Base;
}

// ����
enum class DIR{ LEFT, RIGHT };

namespace Fighter
{
	class Base
	{
	public:
		Base(int deviceID);
		~Base();
		void Control();			// �v���C���[����̓��͂��󂯕t����
		virtual void Update();	// ��{�I�ȍX�V
		void UpdatePos();		// ���W�X�V(�����ɌĂяo��)
		virtual void Render();

		// �Q�b�^�[
		Vector3 &GetMove(){ return m_move; }
		Vector3 &GetPos(){ return m_pos; }
		Vector3 *GetPosAddress(){ return &m_pos; }

		// �Z�b�^�[
		void SetMove(const Vector3 &v){ m_move.Set(v.x, v.y, v.z); }
		void SetPos(const Vector3 &v){ m_pos.Set(v.x, v.y, v.z); }

		// �X�e�[�W�̂����蔻��p
		CollisionShape::Square *GetHitSquare(){ return m_pHitSquare; }

	protected:
		const int m_deviceID;		// �����̃R���g���[���[�̔ԍ�(�����A�X�}�u����xP�ɒl����)
		iex3DObj *m_pObj;	// ���b�V������
		Vector3 m_pos;	// ���W
		Vector3 m_move;	// �ړ��l
		CollisionShape::Square *m_pHitSquare;	// �l�p����(�X�e�[�W�Փ˂Ŏg��)
		DIR m_dir;				// ���E�̃t���O
		const float m_Gravity;	// �d�͒l
	};

	class Airou: public Base
	{
	public:
		Airou(int deviceID);

	};
}

class PlayerManager
{
public:
	PlayerManager(int NumPlayer);
	~PlayerManager();
	void Update(Stage::Base *stage);
	void Render();

	int GetNumPlayer(){ return m_NumPlayer; }
	Fighter::Base *GetPlayer(int no){ return m_pPlayers[no]; }

private:
	const int m_NumPlayer;
	Fighter::Base **m_pPlayers;
};