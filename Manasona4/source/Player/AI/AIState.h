#pragma once
#include "AI.h"

// �O���錾
class AI;

namespace AIState
{
	/*******************************************************/
	//					�O���[�o���X�e�[�g
	/*******************************************************/
	class Global :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Global* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Global() {};
		~Global() {};

		Global(const Global&){}
		Global& operator=(const Global&){}
	};



	/*******************************************************/
	//					���K�p�W�����v�X�e�[�g
	/*******************************************************/
	class PracticeJump :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeJump* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeJump() {};
		~PracticeJump() {};

		PracticeJump(const PracticeJump&){}
		PracticeJump& operator=(const PracticeJump&){}
	};


	/*******************************************************/
	//				�G��ǂ�������X�e�[�g
	/*******************************************************/
	class Chase :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Chase* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Chase() {};
		~Chase() {};

		Chase(const Chase&){}
		Chase& operator=(const Chase&){}
	};

	/*******************************************************/
	//				���b�V���U���X�e�[�g
	/*******************************************************/
	class RushAtack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static RushAtack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		RushAtack() {};
		~RushAtack() {};

		RushAtack(const RushAtack&){}
		RushAtack& operator=(const RushAtack&){}
	};

	/*******************************************************/
	//				���A�X�e�[�g
	/*******************************************************/
	class Recovery :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Recovery* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Recovery() {};
		~Recovery() {};

		Recovery(const Recovery&){}
		Recovery& operator=(const Recovery&){}
	};


	/*******************************************************/
	//				�΋󏀔��X�e�[�g
	/*******************************************************/
	class SetAnitiAir :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static SetAnitiAir* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SetAnitiAir() {};
		~SetAnitiAir() {};

		SetAnitiAir(const SetAnitiAir&){}
		SetAnitiAir& operator=(const SetAnitiAir&){}
	};

	/*******************************************************/
	//				�΋�X�e�[�g
	/*******************************************************/
	class AnitiAir :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static AnitiAir* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		AnitiAir() {};
		~AnitiAir() {};

		AnitiAir(const AnitiAir&){}
		AnitiAir& operator=(const AnitiAir&){}
	};
}



