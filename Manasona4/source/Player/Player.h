#pragma once

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

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

class PlayerManager :public BaseGameEntity
{
public:
	PlayerManager(int NumPlayer, Stage::Base *pStage);
	~PlayerManager();
	//void Update(Stage::Base *pStage);
	void Update();
	void Render();

	int GetNumPlayer(){ return m_NumPlayer; }
	Fighter::Base *GetPlayer(int no){ return m_pPlayers[no]; }

private:
	const int m_NumPlayer;
	Fighter::Base **m_pPlayers;
	Stage::Base *m_pStage;	// �Q�Ƃ��邾��(Update�̈����ł�����Ă��̂����ABaseGameEntity�̌p���ň����Ȃ���Update���g��Ȃ��Ƃ����Ȃ��Ȃ������߁A�����o�ϐ���)

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);
};