#pragma once
#include "../BaseEntity/State/StateMachine.h"

// �O���錾
class BasePlayer;

/*
	�x�[�X�v���C���[�̃X�e�[�g�}�V���̃J�[�g���b�W
*/


namespace BasePlayerState
{
	/*******************************************************/
	//					�O���[�o���X�e�[�g
	/*******************************************************/
	class Global :public State<BasePlayer>
	{
	public:

		//this is a �V���O���g��
		static Global* GetInstance();

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);

		int DEBUG_FLAME = 0;
	private:
		Global() {};
		~Global() {};

		Global(const Global&){}
		Global& operator=(const Global&){}
	};


	/*******************************************************/
	//					�ҋ@
	/*******************************************************/
	class Wait :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Wait* GetInstance();

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Wait() {};
		~Wait() {};

		Wait(const Wait&){}
		Wait& operator=(const Wait&){}
	};

	/*******************************************************/
	//					�����ړ�
	/*******************************************************/
	class Walk :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Walk* GetInstance();

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Walk() {};
		~Walk() {};

		Walk(const Wait&) {}
		Walk& operator=(const Wait&) {}
	};

	/*******************************************************/
	//					����ړ�
	/*******************************************************/
	class Run :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Run* GetInstance();

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Run() {};
		~Run() {};

		Run(const Wait&) {}
		Run& operator=(const Wait&) {}
	};

	/*******************************************************/
	//					�u���[�L
	/*******************************************************/
	class TrunOverBrake :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static TrunOverBrake* GetInstance();

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		TrunOverBrake() {};
		~TrunOverBrake() {};

		TrunOverBrake(const Wait&) {}
		TrunOverBrake& operator=(const Wait&) {}
	};
}