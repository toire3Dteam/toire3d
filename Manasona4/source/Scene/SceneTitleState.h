#pragma once

#include "sceneTitle.h"

// �O���錾
class sceneTitle;
struct Message;


namespace SceneTitleState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class Intro : public State<sceneTitle>
	{
	public:
		// this is a �V���O���g��
		static Intro* GetInstance() { static Intro state; return &state; }

		// ����
		virtual void Enter(sceneTitle* pMain);

		// ���s���܂�
		virtual void Execute(sceneTitle* pMain);

		// �A��
		virtual void Exit(sceneTitle* pMain);

		// �`��
		virtual void Render(sceneTitle* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};
	
	//--------------------���S�X�e�[�g
	class LogoStep : public State<sceneTitle>
	{
	public:
		// this is a �V���O���g��
		static LogoStep* GetInstance() { static LogoStep state; return &state; }

		// ����
		virtual void Enter(sceneTitle* pMain);

		// ���s���܂�
		virtual void Execute(sceneTitle* pMain);

		// �A��
		virtual void Exit(sceneTitle* pMain);

		// �`��
		virtual void Render(sceneTitle* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		LogoStep() {};
		~LogoStep() {};

		LogoStep(const LogoStep&) {}
		LogoStep& operator=(const LogoStep&) {}
	};

	//--------------------���[�r�[�X�e�[�g
	class MovieStep : public State<sceneTitle>
	{
	public:
		// this is a �V���O���g��
		static MovieStep* GetInstance() { static MovieStep state; return &state; }

		// ����
		virtual void Enter(sceneTitle* pMain);

		// ���s���܂�
		virtual void Execute(sceneTitle* pMain);

		// �A��
		virtual void Exit(sceneTitle* pMain);

		// �`��
		virtual void Render(sceneTitle* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		MovieStep() {};
		~MovieStep() {};

		MovieStep(const MovieStep&) {}
		MovieStep& operator=(const MovieStep&) {}
	};


	//--------------------�^�C�g���X�e�[�g
	class TitleStep : public State<sceneTitle>
	{
	public:
		// this is a �V���O���g��
		static TitleStep* GetInstance() { static TitleStep state; return &state; }

		// ����
		virtual void Enter(sceneTitle* pMain);

		// ���s���܂�
		virtual void Execute(sceneTitle* pMain);

		// �A��
		virtual void Exit(sceneTitle* pMain);

		// �`��
		virtual void Render(sceneTitle* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		TitleStep() {};
		~TitleStep() {};

		TitleStep(const TitleStep&) {}
		TitleStep& operator=(const TitleStep&) {}
	};

	//--------------------�����
	class End : public State<sceneTitle>
	{
	public:
		// this is a �V���O���g��
		static End* GetInstance() { static End state; return &state; }

		// ����
		virtual void Enter(sceneTitle* pMain);

		// ���s���܂�
		virtual void Execute(sceneTitle* pMain);

		// �A��
		virtual void Exit(sceneTitle* pMain);

		// �`��
		virtual void Render(sceneTitle* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		End() {};
		~End() {};

		End(const End&) {}
		End& operator=(const End&) {}

		bool PadUpdate(sceneTitle* pMain, int DeviceID);
	};


	//--------------------���j���[��ʂɖ߂�X�e�[�g
	class BackMenu : public State<sceneTitle>
	{
	public:
		// this is a �V���O���g��
		static BackMenu* GetInstance() { static BackMenu state; return &state; }

		// ����
		virtual void Enter(sceneTitle* pMain);

		// ���s���܂�
		virtual void Execute(sceneTitle* pMain);

		// �A��
		virtual void Exit(sceneTitle* pMain);

		// �`��
		virtual void Render(sceneTitle* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneTitle* pMain, const Message& msg);

	private:
		BackMenu() {};
		~BackMenu() {};

		BackMenu(const BackMenu&) {}
		BackMenu& operator=(const BackMenu&) {}
	};


}