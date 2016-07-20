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

		Walk(const Walk&) {}
		Walk& operator=(const Walk&) {}
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

		Run(const Run&) {}
		Run& operator=(const Run&) {}
	};

	/*******************************************************/
	//					�t�����g�u���[�L
	/*******************************************************/
	class FrontBrake :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static FrontBrake* GetInstance();

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
		FrontBrake() {};
		~FrontBrake() {};

		FrontBrake(const FrontBrake&) {}
		FrontBrake& operator=(const FrontBrake&) {}
	};

	/*******************************************************/
	//					U�^�[���u���[�L
	/*******************************************************/
	class TurnOverBrake :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static TurnOverBrake* GetInstance();

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
		TurnOverBrake() {};
		~TurnOverBrake() {};

		TurnOverBrake(const TurnOverBrake&) {}
		TurnOverBrake& operator=(const TurnOverBrake&) {}
	};

	/*******************************************************/
	//					�W�����v
	/*******************************************************/
	class Jump :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Jump* GetInstance();

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

		Jump() {};
		~Jump() {};

		Jump(const Jump&) {}
		Jump& operator=(const Jump&) {}
	};
}