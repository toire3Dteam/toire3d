#pragma once

#include "SceneMain.h"

// �O���錾
class sceneMenu;
struct Message;

namespace SceneMenuState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class Intro : public State<sceneMenu>
	{
	public:
		// this is a �V���O���g��
		static Intro* GetInstance() { static Intro state; return &state; }

		// ����
		virtual void Enter(sceneMenu* pMain);

		// ���s���܂�
		virtual void Execute(sceneMenu* pMain);

		// �A��
		virtual void Exit(sceneMenu* pMain);

		// �`��
		virtual void Render(sceneMenu* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------�ŏ��̑I��
	class FirstStep : public State<sceneMenu>
	{
	public:
		// this is a �V���O���g��
		static FirstStep* GetInstance() { static FirstStep state; return &state; }

		// ����
		virtual void Enter(sceneMenu* pMain);

		// ���s���܂�
		virtual void Execute(sceneMenu* pMain);

		// �A��
		virtual void Exit(sceneMenu* pMain);

		// �`��
		virtual void Render(sceneMenu* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		FirstStep() {};
		~FirstStep() {};

		FirstStep(const FirstStep&) {}
		FirstStep& operator=(const FirstStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};


	//--------------------�o�g���̃R���g���[���[�I��
	class BattleControllerSelectStep : public State<sceneMenu>
	{
	public:
		// this is a �V���O���g��
		static BattleControllerSelectStep* GetInstance() { static BattleControllerSelectStep state; return &state; }

		// ����
		virtual void Enter(sceneMenu* pMain);

		// ���s���܂�
		virtual void Execute(sceneMenu* pMain);

		// �A��
		virtual void Exit(sceneMenu* pMain);

		// �`��
		virtual void Render(sceneMenu* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneMenu* pMain, const Message& msg);

	private:
		BattleControllerSelectStep() {};
		~BattleControllerSelectStep() {};

		BattleControllerSelectStep(const BattleControllerSelectStep&) {}
		BattleControllerSelectStep& operator=(const BattleControllerSelectStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

}