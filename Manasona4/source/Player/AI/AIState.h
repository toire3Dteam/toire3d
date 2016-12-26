#pragma once
#include "AI.h"

// �O���錾
class AI;

namespace AIState
{
	/*******************************************************/
	//					�O���[�o���X�e�[�g
	/*******************************************************/
	class Global :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Global* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Global() {};
		~Global() {};

		Global(const Global&){}
		Global& operator=(const Global&){}
	};

	/*******************************************************/
	//					�v���N�e�B�X�O���[�o���X�e�[�g
	/*******************************************************/
	class PracticeGlobal :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeGlobal* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeGlobal() {};
		~PracticeGlobal() {};

		PracticeGlobal(const PracticeGlobal&){}
		PracticeGlobal& operator=(const PracticeGlobal&){}
	};



	/*******************************************************/
	//					���K�p�W�����v�X�e�[�g
	/*******************************************************/
	class PracticeJump :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeJump* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeJump() {};
		~PracticeJump() {};

		PracticeJump(const PracticeJump&){}
		PracticeJump& operator=(const PracticeJump&){}
	};

	/*******************************************************/
	//					���K�p�n�ʃX�e�[�g
	/*******************************************************/
	class PracticeLand :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeLand* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeLand() {};
		~PracticeLand() {};

		PracticeLand(const PracticeLand&){}
		PracticeLand& operator=(const PracticeLand&){}
	};

	/*******************************************************/
	//					���K�p�n�ʂ��Ⴊ�݃X�e�[�g
	/*******************************************************/
	class PracticeSquat :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeSquat* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeSquat() {};
		~PracticeSquat() {};

		PracticeSquat(const PracticeSquat&) {}
		PracticeSquat& operator=(const PracticeSquat&) {}
	};

	/*******************************************************/
	//					���K�p�U���X�e�[�g
	/*******************************************************/
	class PracticeAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeAttack() {};
		~PracticeAttack() {};

		PracticeAttack(const PracticeAttack&) {}
		PracticeAttack& operator=(const PracticeAttack&) {}
	};

	/*******************************************************/
	//					���K�p�K�[�h�X�e�[�g
	/*******************************************************/
	class PracticeGuard :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PracticeGuard* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PracticeGuard() {};
		~PracticeGuard() {};

		PracticeGuard(const PracticeGuard&) {}
		PracticeGuard& operator=(const PracticeGuard&) {}
	};

	/*******************************************************/
	//					���G���荞�ݗp�X�e�[�g
	/*******************************************************/
	class InterruptInvincible :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static InterruptInvincible* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		InterruptInvincible() {};
		~InterruptInvincible() {};

		InterruptInvincible(const InterruptInvincible&) {}
		InterruptInvincible& operator=(const InterruptInvincible&) {}
	};

	/*******************************************************/
	//				�ҋ@�X�e�[�g
	/*******************************************************/
	class Wait :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Wait* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Wait() {};
		~Wait() {};

		Wait(const Wait&){}
		Wait& operator=(const Wait&){}
	};

	/*******************************************************/
	//		���o���󂯂�X�e�[�g�@(���G����̓���)
	/*******************************************************/
	class Space :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Space* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Space() {};
		~Space() {};

		Space(const Space&){}
		Space& operator=(const Space&){}
	};

	/*******************************************************/
	//		��ԃX�e�[�g�@���ɈӖ��͂Ȃ����Ǎ���Ă݂�
	/*******************************************************/
	class SimpleJump :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static SimpleJump* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SimpleJump() {};
		~SimpleJump() {};

		SimpleJump(const SimpleJump&){}
		SimpleJump& operator=(const SimpleJump&){}
	};

	/*******************************************************/
	//		����X�e�[�g�@
	/*******************************************************/
	class Escape :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Escape* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Escape() {};
		~Escape() {};

		Escape(const Escape&){}
		Escape& operator=(const Escape&){}
	};

	/*******************************************************/
	//		�K�[�h�X�e�[�g�@
	/*******************************************************/
	class Guard :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Guard* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Guard() {};
		~Guard() {};

		Guard(const Guard&){}
		Guard& operator=(const Guard&){}
	};

	/*******************************************************/
	//				�G��ǂ�������X�e�[�g
	/*******************************************************/
	class Chase :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Chase* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Chase() {};
		~Chase() {};

		Chase(const Chase&){}
		Chase& operator=(const Chase&){}
	};

	/*******************************************************/
	//			�W�����v���ċ󒆂œG��ǂ�������X�e�[�g
	/*******************************************************/
	class ChaseAir :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static ChaseAir* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		ChaseAir() {};
		~ChaseAir() {};

		ChaseAir(const ChaseAir&){}
		ChaseAir& operator=(const ChaseAir&){}
	};

	/*******************************************************/
	//				���b�V���U���X�e�[�g
	/*******************************************************/
	class RushAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static RushAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		RushAttack() {};
		~RushAttack() {};

		RushAttack(const RushAttack&){}
		RushAttack& operator=(const RushAttack&){}
	};

	/*******************************************************/
	//				���A�X�e�[�g
	/*******************************************************/
	class Recovery :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Recovery* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Recovery() {};
		~Recovery() {};

		Recovery(const Recovery&){}
		Recovery& operator=(const Recovery&){}
	};

	/*******************************************************/
	//				�͂܂ꂽ�X�e�[�g
	/*******************************************************/
	class ThrowTech:public State<AI>
	{
	public:

		//this is a �V���O���g��
		static ThrowTech* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		ThrowTech() {};
		~ThrowTech() {};

		ThrowTech(const ThrowTech&) {}
		ThrowTech& operator=(const ThrowTech&) {}
	};


	/*******************************************************/
	//				���������X�e�[�g
	/*******************************************************/
	class SetThrow :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static SetThrow* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SetThrow() {};
		~SetThrow() {};

		SetThrow(const SetThrow&) {}
		SetThrow& operator=(const SetThrow&) {}
	};

	/*******************************************************/
	//				�����X�e�[�g
	/*******************************************************/
	class Throw :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static Throw* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		Throw() {};
		~Throw() {};

		Throw(const Throw&) {}
		Throw& operator=(const Throw&) {}
	};

	/*******************************************************/
	//				�΋󏀔��X�e�[�g
	/*******************************************************/
	class SetAnitiAir :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static SetAnitiAir* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SetAnitiAir() {};
		~SetAnitiAir() {};

		SetAnitiAir(const SetAnitiAir&){}
		SetAnitiAir& operator=(const SetAnitiAir&){}
	};

	/*******************************************************/
	//				�΋�X�e�[�g
	/*******************************************************/
	class AnitiAir :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static AnitiAir* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		AnitiAir() {};
		~AnitiAir() {};

		AnitiAir(const AnitiAir&){}
		AnitiAir& operator=(const AnitiAir&){}
	};


	/*******************************************************/
	//				�󒆍U�� �����X�e�[�g
	/*******************************************************/
	class SetAerialAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static SetAerialAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SetAerialAttack() {};
		~SetAerialAttack() {};

		SetAerialAttack(const SetAerialAttack&){}
		SetAerialAttack& operator=(const SetAerialAttack&){}
	};

	/*******************************************************/
	//				�󒆍U�� �X�e�[�g
	/*******************************************************/
	class AerialAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static AerialAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		AerialAttack() {};
		~AerialAttack() {};

		AerialAttack(const AerialAttack&){}
		AerialAttack& operator=(const AerialAttack&){}
	};

	
	/*******************************************************/
	//				�󒆕����U�� �X�e�[�g�@�i�󒆂Œn��̑����������߂̍U���j
	/*******************************************************/
	class AerialDestructAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static AerialDestructAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		AerialDestructAttack() {};
		~AerialDestructAttack() {};

		AerialDestructAttack(const AerialDestructAttack&){}
		AerialDestructAttack& operator=(const AerialDestructAttack&){}
	};

	/*******************************************************/
	//				���G�U���X�e�[�g
	/*******************************************************/
	class InvincibleAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static InvincibleAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		InvincibleAttack() {};
		~InvincibleAttack() {};

		InvincibleAttack(const InvincibleAttack&){}
		InvincibleAttack& operator=(const InvincibleAttack&){}
	};


	/*******************************************************/
	//			�L�����N�^�[�ŗL�̍U���n�X�L�������X�e�[�g
	/*******************************************************/
	class SkillAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static SkillAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		SkillAttack() {};
		~SkillAttack() {};

		SkillAttack(const SkillAttack&){}
		SkillAttack& operator=(const SkillAttack&){}
	};


	/*******************************************************/
	//			���i�Z
	/*******************************************************/
	class DokkoiAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static DokkoiAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		DokkoiAttack() {};
		~DokkoiAttack() {};

		DokkoiAttack(const DokkoiAttack&) {}
		DokkoiAttack& operator=(const DokkoiAttack&) {}
	};


	/*******************************************************/
	//			�C�[�W�[�A�^�b�N
	/*******************************************************/
	class EasyAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static EasyAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		EasyAttack() {};
		~EasyAttack() {};

		EasyAttack(const EasyAttack&) {}
		EasyAttack& operator=(const EasyAttack&) {}
	};

	/*******************************************************/
	//			������ 
	/*******************************************************/
	class BackWark :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static BackWark* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		BackWark() {};
		~BackWark() {};

		BackWark(const BackWark&) {}
		BackWark& operator=(const BackWark&) {}
	};

	/*******************************************************/
	//			�ŋ��̕��s (��Փx ���]����p)
	/*******************************************************/
	class MightyWark :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static MightyWark* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		MightyWark() {};
		~MightyWark() {};

		MightyWark(const MightyWark&) {}
		MightyWark& operator=(const MightyWark&) {}
	};

	/*******************************************************/
	//			�o�b�N�X�e�b�v 
	/*******************************************************/
	class BackStep :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static BackStep* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		BackStep() {};
		~BackStep() {};

		BackStep(const BackStep&) {}
		BackStep& operator=(const BackStep&) {}
	};


	/*******************************************************/
	//			�p�[�g�i�[�����X�e�[�g
	/*******************************************************/
	class PartnerAttack :public State<AI>
	{
	public:

		//this is a �V���O���g��
		static PartnerAttack* GetInstance();

		// ����
		virtual void Enter(AI* pPerson);

		// ���s���܂�
		virtual void Execute(AI* pPerson);

		// �A��
		virtual void Exit(AI* pPerson);

		// �`��
		virtual void Render(AI* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(AI* pPerson, const Message& msg);


	private:
		PartnerAttack() {};
		~PartnerAttack() {};

		PartnerAttack(const PartnerAttack&) {}
		PartnerAttack& operator=(const PartnerAttack&) {}
	};
}



