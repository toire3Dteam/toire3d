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

	//--------------------�C���X�g�̑I��
	class PictureStep : public State<sceneCollect>
	{
	public:
		// this is a �V���O���g��
		static PictureStep* GetInstance() { static PictureStep state; return &state; }

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
		PictureStep() {};
		~PictureStep() {};

		PictureStep(const PictureStep&) {}
		PictureStep& operator=(const PictureStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------�C���X�g�ӏ�
	class PictureWatch : public State<sceneCollect>
	{
	public:
		// this is a �V���O���g��
		static PictureWatch* GetInstance() { static PictureWatch state; return &state; }

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
		PictureWatch() {};
		~PictureWatch() {};

		PictureWatch(const PictureWatch&) {}
		PictureWatch& operator=(const PictureWatch&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------�R���e���c�w���m�F
	class BuyPictureStep : public State<sceneCollect>
	{
	public:
		// this is a �V���O���g��
		static BuyPictureStep* GetInstance() { static BuyPictureStep state; return &state; }

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
		BuyPictureStep() {};
		~BuyPictureStep() {};

		BuyPictureStep(const BuyPictureStep&) {}
		BuyPictureStep& operator=(const BuyPictureStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);
	};

	//--------------------�o�b�N���j���[�I��
	class BackMenuStep : public State<sceneCollect>
	{
	public:
		// this is a �V���O���g��
		static BackMenuStep* GetInstance() { static BackMenuStep state; return &state; }

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
		BackMenuStep() {};
		~BackMenuStep() {};

		BackMenuStep(const BackMenuStep&) {}
		BackMenuStep& operator=(const BackMenuStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);
	};
}