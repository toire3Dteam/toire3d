#pragma once

#include "SceneMain.h"

// �O���錾
class sceneMain;
class LoadSceneThread;
struct Message;

namespace SceneMainState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class StageIntro : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static StageIntro* GetInstance(){ static StageIntro state; return &state; }

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
		bool m_bSkip;	// �C���g���X�L�b�v���ăt�F�[�h�������ǂ���

		StageIntro() {};
		~StageIntro() {};

		StageIntro(const StageIntro&){}
		StageIntro& operator=(const StageIntro&){}
	};

	//--------------------�Q�[���J�n���X�e�[�g
	class CharaIntro : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static CharaIntro* GetInstance(){ static CharaIntro state; return &state; }

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

		bool m_bSkip;		// �C���g���X�L�b�v���ăt�F�[�h�������ǂ���
		int m_iKakeaiTimer;	// �|�������̎��Ԍv���p

		CharaIntro() {};
		~CharaIntro() {};

		CharaIntro(const CharaIntro&){}
		CharaIntro& operator=(const CharaIntro&){}
	};

	//--------------------���E���h�R�[��
	class Round : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static Round* GetInstance(){ static Round state; return &state; }

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
		Round() {};
		~Round() {};

		Round(const Round&){}
		Round& operator=(const Round&){}
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

	//--------------------�t�B�j�b�V���X�e�[�g(���E���h���܂��������U���g�ɍs�����̕���͂��̒��ōs���\��)
	class Finish : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static Finish* GetInstance(){ static Finish state; return &state; }

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
		bool m_bEndCall;

		Finish() :m_bEndCall(false) {};
		~Finish() {};

		Finish(const Finish&){}
		Finish& operator=(const Finish&){}
	};

	//--------------------�q�[�z�[�I�[�o�[�t���[�������o���X�e�[�g
	class HeaveHoDriveOverFlowSuccess : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static HeaveHoDriveOverFlowSuccess* GetInstance() { static HeaveHoDriveOverFlowSuccess state; return &state; }

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
		HeaveHoDriveOverFlowSuccess(){};
		~HeaveHoDriveOverFlowSuccess(){}

		HeaveHoDriveOverFlowSuccess(const HeaveHoDriveOverFlowSuccess&) {}
		HeaveHoDriveOverFlowSuccess& operator=(const HeaveHoDriveOverFlowSuccess&) {}
	};


	//--------------------�`���[�g���A���i�C���g���j�X�e�[�g
	class TutorialIntro : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static TutorialIntro* GetInstance() { static TutorialIntro state; return &state; }

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
		TutorialIntro() {};
		~TutorialIntro() {};

		TutorialIntro(const TutorialIntro&) {}
		TutorialIntro& operator=(const TutorialIntro&) {}
	};
	
	//--------------------�`���[�g���A���i���C���j�X�e�[�g
	class TutorialMain : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static TutorialMain* GetInstance() { static TutorialMain state; return &state; }

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
		TutorialMain() {};
		~TutorialMain() {};

		TutorialMain(const TutorialMain&) {}
		TutorialMain& operator=(const TutorialMain&) {}
	};


	//--------------------�`���[�g���A���i�N���A�j�X�e�[�g
	class TutorialClear: public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static TutorialClear* GetInstance() { static TutorialClear state; return &state; }

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
		TutorialClear() {};
		~TutorialClear() {};

		TutorialClear(const TutorialClear&) {}
		TutorialClear& operator=(const TutorialClear&) {}
	};



	//--------------------�|�[�Y���j���[�X�e�[�g
	class PauseMenu : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static PauseMenu* GetInstance() { static PauseMenu state; return &state; }

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
		PauseMenu() {};
		~PauseMenu() {};

		PauseMenu(const PauseMenu&) {}
		PauseMenu& operator=(const PauseMenu&) {}
	};

	//--------------------�`���[�g���A���|�[�Y���j���[�X�e�[�g
	class TutorialPauseMenu : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static TutorialPauseMenu* GetInstance() { static TutorialPauseMenu state; return &state; }

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
		TutorialPauseMenu() {};
		~TutorialPauseMenu() {};

		TutorialPauseMenu(const TutorialPauseMenu&) {}
		TutorialPauseMenu& operator=(const TutorialPauseMenu&) {}
	};


	//--------------------�T�E���h���j���[�X�e�[�g
	class SoundMenu : public State<sceneMain>
	{
	public:
		// this is a �V���O���g��
		static SoundMenu* GetInstance() { static SoundMenu state; return &state; }

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
		SoundMenu() {};
		~SoundMenu() {};

		SoundMenu(const SoundMenu&) {}
		SoundMenu& operator=(const SoundMenu&) {}
	};
}