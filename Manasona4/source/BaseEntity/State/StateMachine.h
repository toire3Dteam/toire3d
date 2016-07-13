#pragma once

#include <cassert>

// ���̃X�e�[�g
#include "State.h"


/*
	�X�e�[�g�}�V���N���X�̒��ɃX�e�[�g�֘A�̑S�Ă�
	�J�v�Z�������邩�Ȃ肷�����肷��

	���e�F
	�G�[�W�F���g�͎������g(�e���v���[�g)�̃X�e�[�g�}�V���̎��Ԃ����L��
	�E���݂̃X�e�[�g
	�E�O���[�o���X�e�[�g
	�E�O��̃X�e�[�g�i�X�e�[�g�u���b�v�j
	���Ϗ����邱�Ƃ��ł���B
*/


template <class entity_type>
class StateMachine
{
private:
	// ���̎��̂����L����G�[�W�F���g�ւ̃|�C���^
	entity_type* m_pOwner;

	// ���݂̃X�e�[�g��ێ�����
	State<entity_type>* m_pCurrentState;

	// �O��̃X�e�[�g
	State<entity_type>* m_pPreviousState;

	// �ǂ�ȃX�e�[�g�ł��X�V����邽�тɌĂ΂��X�e�[�g
	State<entity_type>* m_pGlobalState;

public:

	// �R���X�g���N�^�i�����ݒ�j
	StateMachine(entity_type* owner) :
		m_pOwner(owner),// �����ŃG�[�W�F���g�̃|�C���^������
		m_pCurrentState(NULL),
		m_pPreviousState(NULL),
		m_pGlobalState(NULL)
	{}

	// FSM������������Ƃ��ȂǂɎg���Z�b�^�[
	void SetCurrentState(State<entity_type>* s) { m_pCurrentState = s; m_pCurrentState->Enter(m_pOwner); }	// Enter��ǉ�
	void SetGlobalState(State<entity_type>* s) {   m_pGlobalState = s; }
	void SetPreviousState(State<entity_type>* s) { m_pPreviousState = s; }

	// FSM���X�V����Ƃ��ɂ�����Ă�
	void  Update()const
	{
		// �O���[�o���X�e�[�g������Ȃ�A���s
		if (m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);

		//�@���݂̃X�e�[�g�̎��s�֐�
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	// FSM�ɂ��`��
	void  Render()const
	{
		// �O���[�o���X�e�[�g������Ȃ�A���s
		if (m_pGlobalState)   m_pGlobalState->Render(m_pOwner);

		//�@���݂̃X�e�[�g�̎��s�֐�
		if (m_pCurrentState) m_pCurrentState->Render(m_pOwner);
	}

	// �摜�Ƃ��̕`�悪�K�v�ɂȂ����̂ŁA2D�̕`������܂���
	void  Render2D()const
	{
		// �O���[�o���X�e�[�g������Ȃ�A���s
		if (m_pGlobalState)   m_pGlobalState->Render2D(m_pOwner);

		//�@���݂̃X�e�[�g�̎��s�֐�
		if (m_pCurrentState) m_pCurrentState->Render2D(m_pOwner);
	}

	// �����b�Z�[�W
	bool  HandleMessage(const Message& msg)const
	{
		// �ŏ��Ɍ��݂̃X�e�[�g���L���ŁA���b�Z�[�W�̏������ł��邩
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		//�@�L���łȂ��Ƃ��A�O���[�o���X�e�[�g���������ꂢ��Ȃ�
		//�@�O���[�o���X�e�[�g�փ��b�Z�[�W�𑗂�
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		return false;
	}


	// ��New�V�����X�e�[�g�ɕύX
	void ChangeState(State<entity_type> * pNewState)
	{
		// �����̃X�e�[�g���L�������m���߂�
		assert(m_pCurrentState&&pNewState);

		// �ȑO�̃X�e�[�g��ۑ�
		m_pPreviousState = m_pCurrentState;

		// ���݂���X�e�[�g�̏I���֐����Ă�
		m_pCurrentState->Exit(m_pOwner);

		// �V�����X�e�[�g�ɕύX
		m_pCurrentState = pNewState;

		// �V�����X�e�[�g�̊J�n�֐����Ă�
		m_pCurrentState->Enter(m_pOwner);

	}

	// �ȑO�̃X�e�[�g�ɕύX�i�ЂƂO�̃X�e�[�g�ɖ߂��j
	void  RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}


	// �A�N�Z�T
	State<entity_type>*  GetCurrentState()  const { return m_pCurrentState; }
	State<entity_type>*  GetGlobalState()   const { return m_pGlobalState; }
	State<entity_type>*  GetPreviousState() const { return m_pPreviousState; }


	// ���݂̃X�e�[�g�̌^�ƃp�����[�^�œn���ꂽ�N���X�̌^�������Ȃ�True��Ԃ�
	bool  isInState(const State<entity_type>& st)const
	{
		if (typeid(*m_pCurrentState) == typeid(st)) return true;
		return false;
	}
};
