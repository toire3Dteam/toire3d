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

enum class PLAYER_INPUT
{
	A,
	B,
	C,
	D,
	RIGHT,
	LEFT,
	UP,
	DOWN,
	R1,
	R2,
	R3,
	L1,
	L2,
	L3,
	START,
	SELECT,
	MAX
};

// �L�[�{�[�h�\
static const KEYCODE KeyCodeList[(int)PLAYER_INPUT::MAX] = {
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_UP,
	KEY_DOWN,
	KEY_R1,
	KEY_R2,
	KEY_R3,
	KEY_L1,
	KEY_L2,
	KEY_L3,
	KEY_START,
	KEY_SELECT,
};

// ����
enum class DIR { LEFT, RIGHT };

	class BasePlayer: public BaseGameEntity
	{
	public:
		BasePlayer(int deviceID);
		~BasePlayer();
		void Control();			// �v���C���[����̓��͂��󂯕t����
		virtual void Update();	// ��{�I�ȍX�V
		void UpdatePos();		// ���W�X�V(�����ɌĂяo��)
		virtual void Render();
		
		bool HandleMessage(const Message& msg); //���b�Z�[�W���󂯎��

		// �Q�b�^�[
		Vector3 &GetMove() { return m_move; }
		Vector3 &GetPos() { return m_pos; }
		Vector3 *GetPosAddress() { return &m_pos; }
		int GetInputList(PLAYER_INPUT input) { return m_InputList[(int)input]; }	
		StateMachine<BasePlayer>* GetFSM()const { return m_pStateMachine; }// ���X�e�[�g�}�V���̃A�N�Z�T�����

		// �Z�b�^�[
		void SetMove(const Vector3 &v) { m_move.Set(v.x, v.y, v.z); }
		void SetPos(const Vector3 &v) { m_pos.Set(v.x, v.y, v.z); }

		// �X�e�[�W�̂����蔻��p
		CollisionShape::Square *GetHitSquare() { return m_pHitSquare; }

		
		

	protected:
		const int m_deviceID;		// �����̃R���g���[���[�̔ԍ�(�����A�X�}�u����xP�ɒl����)
		iex3DObj *m_pObj;	// ���b�V������
		Vector3 m_pos;	// ���W
		Vector3 m_move;	// �ړ��l
		CollisionShape::Square *m_pHitSquare;	// �l�p����(�X�e�[�W�Փ˂Ŏg��)
		DIR m_dir;				// ���E�̃t���O
		const float m_Gravity;	// �d�͒l

		int m_InputList[(int)PLAYER_INPUT::MAX]; 	// �����Ă���L�[���i�[
		StateMachine<BasePlayer>* m_pStateMachine; // ���X�e�[�g�}�V��
		
	};


