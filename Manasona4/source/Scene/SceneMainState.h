#pragma once

#include "SceneMain.h"

// �O���錾
class sceneMain;
struct Message;

namespace SceneMainState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class Intro : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static Intro* GetInstance(){ static Intro state; return &state; }

		// ����
		virtual void Enter(sceneMain* pMain);

		// ���s���܂�
		virtual void Execute(sceneMain* pMain);

		// �A��
		virtual void Exit(sceneMain* pMain);

		// �`��
		virtual void Render(sceneMain* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&){}
		Intro& operator=(const Intro&){}
	};

	//--------------------���C���X�e�[�g
	class Main : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static Main* GetInstance(){ static Main state; return &state; }

		// ����
		virtual void Enter(sceneMain* pMain);

		// ���s���܂�
		virtual void Execute(sceneMain* pMain);

		// �A��
		virtual void Exit(sceneMain* pMain);

		// �`��
		virtual void Render(sceneMain* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		Main() {};
		~Main() {};

		Main(const Main&){}
		Main& operator=(const Main&){}
	};

	//--------------------�����I�����X�e�[�g
	class End : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static End* GetInstance(){ static End state; return &state; }

		// ����
		virtual void Enter(sceneMain* pMain);

		// ���s���܂�
		virtual void Execute(sceneMain* pMain);

		// �A��
		virtual void Exit(sceneMain* pMain);

		// �`��
		virtual void Render(sceneMain* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneMain* pMain, const Message& msg);


	private:
		End() {};
		~End() {};

		End(const End&){}
		End& operator=(const End&){}
	};
}