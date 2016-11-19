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
		// this is a �V���O���g��
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

	//--------------------�ŏ��̑I��
	class FirstStep : public State<sceneSelect>
	{
	public:
		// this is a �V���O���g��
		static FirstStep* GetInstance() { static FirstStep state; return &state; }

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
		FirstStep() {};
		~FirstStep() {};

		FirstStep(const FirstStep&) {}
		FirstStep& operator=(const FirstStep&) {}

		bool PadUpdate(sceneSelect* pMain, int DeviceID);
	};



}