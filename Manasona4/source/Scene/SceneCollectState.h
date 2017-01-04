#pragma once
#include "SceneCollect.h"

// �O���錾
class sceneCollect;
struct Message;

namespace SceneCollectState
{
	//--------------------�Q�[���J�n���X�e�[�g
	class Intro : public State<sceneCollect>
	{
	public:
		// this is a �V���O���g��
		static Intro* GetInstance() { static Intro state; return &state; }

		// ����
		virtual void Enter(sceneCollect* pMain);

		// ���s���܂�
		virtual void Execute(sceneCollect* pMain);

		// �A��
		virtual void Exit(sceneCollect* pMain);

		// �`��
		virtual void Render(sceneCollect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	//--------------------�g���t�B�[�̑I��
	class TrophyStep : public State<sceneCollect>
	{
	public:
		// this is a �V���O���g��
		static TrophyStep* GetInstance() { static TrophyStep state; return &state; }

		// ����
		virtual void Enter(sceneCollect* pMain);

		// ���s���܂�
		virtual void Execute(sceneCollect* pMain);

		// �A��
		virtual void Exit(sceneCollect* pMain);

		// �`��
		virtual void Render(sceneCollect* pMain);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(sceneCollect* pMain, const Message& msg);

	private:
		TrophyStep() {};
		~TrophyStep() {};

		TrophyStep(const TrophyStep&) {}
		TrophyStep& operator=(const TrophyStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

		//+-------------------------------
		//	�g���t�B�[�ŕK�v�ȕϐ���p��
		//+-------------------------------



	};
}