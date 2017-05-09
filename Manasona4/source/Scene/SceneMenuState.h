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
		// ����_
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
		// ����_
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
		// ����_
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

	//--------------------�g���[�j���O�̃R���g���[���[�I��
	class TrainingControllerSelectStep : public State<sceneMenu>
	{
	public:
		// ����_
		static TrainingControllerSelectStep* GetInstance() { static TrainingControllerSelectStep state; return &state; }

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
		TrainingControllerSelectStep() {};
		~TrainingControllerSelectStep() {};

		TrainingControllerSelectStep(const TrainingControllerSelectStep&) {}
		TrainingControllerSelectStep& operator=(const TrainingControllerSelectStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};


	//--------------------�`���[�g���A���I��
	class TutorialSelectStep : public State<sceneMenu>
	{
	public:
		// ����_
		static TutorialSelectStep* GetInstance() { static TutorialSelectStep state; return &state; }

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
		TutorialSelectStep() {};
		~TutorialSelectStep() {};

		TutorialSelectStep(const TutorialSelectStep&) {}
		TutorialSelectStep& operator=(const TutorialSelectStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------�I�v�V�����E�B���h�E
	class OptionStep : public State<sceneMenu>
	{
	public:
		// ����_
		static OptionStep* GetInstance() { static OptionStep state; return &state; }

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
		OptionStep() {};
		~OptionStep() {};

		OptionStep(const OptionStep&) {}
		OptionStep& operator=(const OptionStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------�`�������W�E�B���h�E
	class ChallengeStep : public State<sceneMenu>
	{
	public:
		// ����_
		static ChallengeStep* GetInstance() { static ChallengeStep state; return &state; }

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
		ChallengeStep() {};
		~ChallengeStep() {};

		ChallengeStep(const ChallengeStep&) {}
		ChallengeStep& operator=(const ChallengeStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------�`�������W�i���o�[�E�B���h�E
	class ChallengeNoStep : public State<sceneMenu>
	{
	public:
		// ����_
		static ChallengeNoStep* GetInstance() { static ChallengeNoStep state; return &state; }

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
		ChallengeNoStep() {};
		~ChallengeNoStep() {};

		ChallengeNoStep(const ChallengeNoStep&) {}
		ChallengeNoStep& operator=(const ChallengeNoStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------�V�X�e���E�B���h�E
	class SystemWindowStep : public State<sceneMenu>
	{
	public:
		// ����_
		static SystemWindowStep* GetInstance() { static SystemWindowStep state; return &state; }

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
		SystemWindowStep() {};
		~SystemWindowStep() {};

		SystemWindowStep(const SystemWindowStep&) {}
		SystemWindowStep& operator=(const SystemWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};


	//--------------------�Q�[���E�B���h�E
	class GameWindowStep : public State<sceneMenu>
	{
	public:
		// ����_
		static GameWindowStep* GetInstance() { static GameWindowStep state; return &state; }

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
		GameWindowStep() {};
		~GameWindowStep() {};

		GameWindowStep(const GameWindowStep&) {}
		GameWindowStep& operator=(const GameWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------�T�E���h�E�B���h�E
	class SoundWindowStep : public State<sceneMenu>
	{
	public:
		// ����_
		static SoundWindowStep* GetInstance() { static SoundWindowStep state; return &state; }

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
		SoundWindowStep() {};
		~SoundWindowStep() {};

		SoundWindowStep(const SoundWindowStep&) {}
		SoundWindowStep& operator=(const SoundWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};

	//--------------------�L�[�I���t�B�O�E�B���h�E
	class KeyConfigWindowStep : public State<sceneMenu>
	{
	public:
		// ����_
		static KeyConfigWindowStep* GetInstance() { static KeyConfigWindowStep state; return &state; }

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
		KeyConfigWindowStep() {};
		~KeyConfigWindowStep() {};

		KeyConfigWindowStep(const KeyConfigWindowStep&) {}
		KeyConfigWindowStep& operator=(const KeyConfigWindowStep&) {}

		bool PadUpdate(sceneMenu* pMain, int DeviceID);
	};
}