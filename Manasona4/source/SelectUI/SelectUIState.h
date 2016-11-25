#pragma once

#pragma once

#include "SelectUI.h"

// �O���錾
class SelectUI;
struct Message;

// �K�v�Ȃ̂�
// Intro
// �L�����Z�����p�[�g�i�[�̃Z���N�g
// �X�e�[�W��BGM�̃Z���N�g


namespace SelectUIState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class Intro : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static Intro* GetInstance() { static Intro state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------�ŏ��̑I���X�e�[�g(�L�����Z��)
	class FirstStep : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static FirstStep* GetInstance() { static FirstStep state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		FirstStep() {};
		~FirstStep() {};

		FirstStep(const FirstStep&) {}
		FirstStep& operator=(const FirstStep&) {}

	};

	//--------------------���Ԃ̑I���X�e�[�g(�L�����Z���ƃp�[�g�i�[�̒���)
	class MiddleStep : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static MiddleStep* GetInstance() { static MiddleStep state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		MiddleStep() {};
		~MiddleStep() {};

		MiddleStep(const MiddleStep&) {}
		MiddleStep& operator=(const MiddleStep&) {}

	};


	//--------------------��ڂ̑I���X�e�[�g(�p�[�g�i�[)
	class SecondStep : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static SecondStep* GetInstance() { static SecondStep state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		SecondStep() {};
		~SecondStep() {};

		SecondStep(const SecondStep&) {}
		SecondStep& operator=(const SecondStep&) {}

	};

	//--------------------�Z�J���h����ݒ芮���܂�
	class SecondToOKStep : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static SecondToOKStep* GetInstance() { static SecondToOKStep state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		SecondToOKStep() {};
		~SecondToOKStep() {};

		SecondToOKStep(const SecondToOKStep&) {}
		SecondToOKStep& operator=(const SecondToOKStep&) {}

	};

	//--------------------�ݒ芮��
	class OKStep : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static OKStep* GetInstance() { static OKStep state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		OKStep() {};
		~OKStep() {};

		OKStep(const OKStep&) {}
		OKStep& operator=(const OKStep&) {}

	};



	//--------------------�ݒ芮������Z�J���h�܂�
	class OKToSecondStep : public State<SelectUI>
	{
	public:
		// this is a �V���O���g��
		static OKToSecondStep * GetInstance() { static OKToSecondStep state; return &state; }

		// ����
		virtual void Enter(SelectUI* pMain);

		// ���s���܂�
		virtual void Execute(SelectUI* pMain);

		// �A��
		virtual void Exit(SelectUI* pMain);

		// �`��
		virtual void Render(SelectUI* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(SelectUI* pMain, const Message& msg);

	private:
		OKToSecondStep() {};
		~OKToSecondStep() {};

		OKToSecondStep(const OKToSecondStep&) {}
		OKToSecondStep& operator=(const OKToSecondStep&) {}

	};
}