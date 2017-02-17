#pragma once

#include "SceneSelect.h"

// �O���錾
class sceneSelect;
struct Message;

// �K�v�Ȃ̂�
// Intro
// �L�����Z�����p�[�g�i�[�̃Z���N�g
// �X�e�[�W��BGM�̃Z���N�g


namespace SceneSelectState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class Intro : public State<sceneSelect>
	{
	public:
		// ����_
		static Intro* GetInstance() { static Intro state; return &state; }

		// ����
		virtual void Enter(sceneSelect* pMain);

		// ���s���܂�
		virtual void Execute(sceneSelect* pMain);

		// �A��
		virtual void Exit(sceneSelect* pMain);

		// �`��
		virtual void Render(sceneSelect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------�L�����N�^�[�̑I��
	class CharaSelect : public State<sceneSelect>
	{
	public:
		// ����_
		static CharaSelect* GetInstance() { static CharaSelect state; return &state; }

		// ����
		virtual void Enter(sceneSelect* pMain);

		// ���s���܂�
		virtual void Execute(sceneSelect* pMain);

		// �A��
		virtual void Exit(sceneSelect* pMain);

		// �`��
		virtual void Render(sceneSelect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		CharaSelect() {};
		~CharaSelect() {};

		CharaSelect(const CharaSelect&) {}
		CharaSelect& operator=(const CharaSelect&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};


	//--------------------�X�e�[�W��BGM�̑I��
	class StageAndBGM : public State<sceneSelect>
	{
	public:
		// ����_
		static StageAndBGM* GetInstance() { static StageAndBGM state; return &state; }

		// ����
		virtual void Enter(sceneSelect* pMain);

		// ���s���܂�
		virtual void Execute(sceneSelect* pMain);

		// �A��
		virtual void Exit(sceneSelect* pMain);

		// �`��
		virtual void Render(sceneSelect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		StageAndBGM() {};
		~StageAndBGM() {};

		StageAndBGM(const StageAndBGM&) {}
		StageAndBGM& operator=(const StageAndBGM&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};



	//--------------------�����
	class End : public State<sceneSelect>
	{
	public:
		// ����_
		static End* GetInstance() { static End state; return &state; }

		// ����
		virtual void Enter(sceneSelect* pMain);

		// ���s���܂�
		virtual void Execute(sceneSelect* pMain);

		// �A��
		virtual void Exit(sceneSelect* pMain);

		// �`��
		virtual void Render(sceneSelect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		End() {};
		~End() {};

		End(const End&) {}
		End& operator=(const End&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};


	//--------------------���j���[��ʂɖ߂�X�e�[�g
	class BackMenu : public State<sceneSelect>
	{
	public:
		// ����_
		static BackMenu* GetInstance() { static BackMenu state; return &state; }

		// ����
		virtual void Enter(sceneSelect* pMain);

		// ���s���܂�
		virtual void Execute(sceneSelect* pMain);

		// �A��
		virtual void Exit(sceneSelect* pMain);

		// �`��
		virtual void Render(sceneSelect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneSelect* pMain, const Message& msg);

	private:
		BackMenu() {};
		~BackMenu() {};

		BackMenu(const BackMenu&) {}
		BackMenu& operator=(const BackMenu&) {}
	};


}