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
		// ����_
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
		// ����_
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
		// ����_
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
		// ����_
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

	//--------------------�R���e���c�w���m�F(�C���X�g)
	class BuyPictureStep : public State<sceneCollect>
	{
	public:
		// ����_
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


	//--------------------���[�r�[�̑I��
	class MovieStep : public State<sceneCollect>
	{
	public:
		// ����_
		static MovieStep* GetInstance() { static MovieStep state; return &state; }

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
		MovieStep() {};
		~MovieStep() {};

		MovieStep(const MovieStep&) {}
		MovieStep& operator=(const MovieStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------���[�r�[�ӏ�
	class MovieWatch : public State<sceneCollect>
	{
	public:
		// ����_
		static MovieWatch* GetInstance() { static MovieWatch state; return &state; }

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
		MovieWatch() {};
		~MovieWatch() {};

		MovieWatch(const MovieWatch&) {}
		MovieWatch& operator=(const MovieWatch&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);

	};

	//--------------------�R���e���c�w���m�F(���[�r�[)
	class BuyMovieStep : public State<sceneCollect>
	{
	public:
		// ����_
		static BuyMovieStep* GetInstance() { static BuyMovieStep state; return &state; }

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
		BuyMovieStep() {};
		~BuyMovieStep() {};

		BuyMovieStep(const BuyMovieStep&) {}
		BuyMovieStep& operator=(const BuyMovieStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);
	};


	//--------------------���R�[�h�̑I��
	class RecordStep : public State<sceneCollect>
	{
	public:
		// ����_
		static RecordStep* GetInstance() { static RecordStep state; return &state; }

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
		RecordStep() {};
		~RecordStep() {};

		RecordStep(const RecordStep&) {}
		RecordStep& operator=(const RecordStep&) {}

		bool PadUpdate(sceneCollect* pMain, int DeviceID);


	};

	//--------------------�o�b�N���j���[�I��
	class BackMenuStep : public State<sceneCollect>
	{
	public:
		// ����_
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