#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"


// ���u�e���v���[�g<�N���XENTITY_TYPE>�v�Ŏ��O�ɐ錾���܂�
//template <class entity_type> class State;
template <class entity_type> class StateMachine;

/*
	�l�̎��
*/
enum class PERSON_TYPE
{
	FRIEND_KEEPER,
	FRIEND_FIELD,

	ENEMY_KEEPER,
	ENEMY_FIELD
};


/*
	��{�̐l�̌`
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(ENTITY_ID id);
	virtual ~BasePerson();

	// BaseGameEntity�̏������z�֐��ɂ��K�{
	virtual void  Update();
	
	// 
	virtual void  Render();
	virtual void  RangeRender();
	virtual void  UIRender();

	// ���b�Z�[�W����舵��
	virtual bool  HandleMessage(const Message& msg) = 0;


	// �A�N�Z�T
	// �����̌`�ɂ��邩��X�e�[�g�}�V���̃A�N�Z�T�����
	//StateMachine<BasePerson>* GetFSM()const { return m_pStateMachine; }

	Vector3 GetPos() { return m_pos; }
	void SetPos(Vector3 pos) { m_pos = pos; }

	Vector3 GetMove() { return m_move; }
	void SetMove(Vector3 move) { m_move = move; }

	float GetAngle() { return m_angle; }
	void SetAngle(float angle) { m_angle = angle; }

	PERSON_TYPE GetPersonType() { return m_PersonType;	}
	
	iex3DObj* GetObj() { return m_obj; }

	bool IsRand() { return m_bRand; }
	void Jump() { m_bRand = false; }

	bool IsCollWall() { return m_bCollWall; }

	// ���x�[�X�ɃX�e�[�g�}�V�����Ȃ��G��Ȃ���
	// virture�𗘗p���Ċe�X�̃X�e�[�g�}�V���ɌĂт����鎖�Ƀg���C�I
	//virtual void ResetState() = 0; // �����̃X�e�[�g�ɖ߂�


	int m_LifePoint;
protected:

	PERSON_TYPE m_PersonType;

	iex3DObj* m_obj;
	Vector3 m_pos;
	Vector3 m_move;

	float m_angle;

	// ���n
	bool m_bRand;// ���n�t���O
	bool m_bCollWall;// �ǂԂ���

	// ���X�e�[�g�}�V��
	// StateMachine<BasePerson>*  m_pStateMachine;


};
