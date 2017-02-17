#pragma once
#include "BattleLoading.h"

// �O���錾
class BattleLoading;
struct Message;

//
int GetCharaType(SIDE eSide);

//+------------------------------
//	�o�g���O���[�h��ʃX�e�[�g
//+------------------------------

namespace BattleLoadingState
{
	//--------------------�J�n���X�e�[�g
	class Intro : public State<BattleLoading>
	{
	public:
		// ����_
		static Intro* GetInstance() { static Intro state; return &state; }

		// ����
		virtual void Enter(BattleLoading* pMain);

		// ���s���܂�
		virtual void Execute(BattleLoading* pMain);

		// �A��
		virtual void Exit(BattleLoading* pMain);

		// �`��
		virtual void Render(BattleLoading* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//�������̓X�e�[�g�ɐF�X�����ē������Ă���
	//	������Q�������� �Ƃ��ɉ����ǂݍ���łȂ�������
	//	��������`�悷��悤�ɂ���
	//	����΂�

	//--------------------1P�X���C�h�X�e�[�g
	class Slide1P : public State<BattleLoading>
	{
	public:
		// ����_
		static Slide1P* GetInstance() { static Slide1P state; return &state; }

		// ����
		virtual void Enter(BattleLoading* pMain);

		// ���s���܂�
		virtual void Execute(BattleLoading* pMain);

		// �A��
		virtual void Exit(BattleLoading* pMain);

		// �`��
		virtual void Render(BattleLoading* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		Slide1P() {};
		~Slide1P() {};

		Slide1P(const Slide1P&) {}
		Slide1P& operator=(const Slide1P&) {}
	};

	//--------------------2P�X���C�h�X�e�[�g
	class Slide2P : public State<BattleLoading>
	{
	public:
		// ����_
		static Slide2P* GetInstance() { static Slide2P state; return &state; }

		// ����
		virtual void Enter(BattleLoading* pMain);

		// ���s���܂�
		virtual void Execute(BattleLoading* pMain);

		// �A��
		virtual void Exit(BattleLoading* pMain);

		// �`��
		virtual void Render(BattleLoading* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		Slide2P() {};
		~Slide2P() {};

		Slide2P(const Slide2P&) {}
		Slide2P& operator=(const Slide2P&) {}
	};

	//--------------------�X���C�h�I���X�e�[�g
	class SlideEnd : public State<BattleLoading>
	{
	public:
		// ����_
		static SlideEnd* GetInstance() { static SlideEnd state; return &state; }

		// ����
		virtual void Enter(BattleLoading* pMain);

		// ���s���܂�
		virtual void Execute(BattleLoading* pMain);

		// �A��
		virtual void Exit(BattleLoading* pMain);

		// �`��
		virtual void Render(BattleLoading* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		SlideEnd() {};
		~SlideEnd() {};

		SlideEnd(const SlideEnd&) {}
		SlideEnd& operator=(const SlideEnd&) {}
	};

	//--------------------�Ō�X�e�[�g
	class FinalStep : public State<BattleLoading>
	{
	public:
		// ����_
		static FinalStep* GetInstance() { static FinalStep state; return &state; }

		// ����
		virtual void Enter(BattleLoading* pMain);

		// ���s���܂�
		virtual void Execute(BattleLoading* pMain);

		// �A��
		virtual void Exit(BattleLoading* pMain);

		// �`��
		virtual void Render(BattleLoading* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		FinalStep() {};
		~FinalStep() {};

		FinalStep(const FinalStep&) {}
		FinalStep& operator=(const FinalStep&) {}
	};

	//--------------------�t�F�[�h�`�F���W�X�e�[�g
	class FadeChangeStep : public State<BattleLoading>
	{
	public:
		// ����_
		static FadeChangeStep* GetInstance() { static FadeChangeStep state; return &state; }

		// ����
		virtual void Enter(BattleLoading* pMain);

		// ���s���܂�
		virtual void Execute(BattleLoading* pMain);

		// �A��
		virtual void Exit(BattleLoading* pMain);

		// �`��
		virtual void Render(BattleLoading* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BattleLoading* pMain, const Message& msg);

	private:
		FadeChangeStep() {};
		~FadeChangeStep() {};

		FadeChangeStep(const FadeChangeStep&) {}
		FadeChangeStep& operator=(const FadeChangeStep&) {}
	};
}

