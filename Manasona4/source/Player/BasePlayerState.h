#pragma once
#include "../BaseEntity/State/StateMachine.h"

// �O���錾
class BasePlayer;

/*
	�x�[�X�v���C���[�̃X�e�[�g�}�V���̃J�[�g���b�W
*/


bool JumpCancel(BasePlayer * pPerson);
bool isThrownState(BasePlayer *pPerson);
bool RunCancel(BasePlayer * pPerson);
bool SquatCancel(BasePlayer * pPerson);
bool SquatAttackCancel(BasePlayer * pPerson);
bool StandCancel(BasePlayer * pPerson);
bool AttackCancel(BasePlayer *pPerson);
bool InvincibleAttackCancel(BasePlayer *pPerson);
bool SkillCancel(BasePlayer *pPerson);
bool OverDriveCancel(BasePlayer *pPerson);
bool EscapeCancel(BasePlayer *pPerson);
bool ThrowCancel(BasePlayer * pPerson);
bool HeaveHoCancel(BasePlayer * pPerson);
bool HeaveHoOverFlowCancel(BasePlayer * pPerson);
bool DashCancel(BasePlayer * pPerson);
bool AerialDashCancel(BasePlayer *pPerson);
bool AerialAttackCancel(BasePlayer *pPerson);
bool DokkoiAttackCancel(BasePlayer *pPerson);
bool isInputGuardCommand(BasePlayer *pPerson);
void GuardUpdate(BasePlayer *pPerson);
bool isPossibleGuardState(BasePlayer *pPerson);


namespace BasePlayerState
{
	/*******************************************************/
	//					�O���[�o���X�e�[�g
	/*******************************************************/
	class Global :public State<BasePlayer>
	{
	public:

		//this is a �V���O���g��
		static Global* GetInstance(){ static Global instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);

		int DEBUG_FLAME = 0;
	private:
		Global() {};
		~Global() {};

		Global(const Global&){}
		Global& operator=(const Global&){}
	};


	/*******************************************************/
	//					�ҋ@
	/*******************************************************/
	class Wait :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Wait* GetInstance(){ static Wait instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Wait() {};
		~Wait() {};

		Wait(const Wait&){}
		Wait& operator=(const Wait&){}
	};

	/*******************************************************/
	//					�����ړ�
	/*******************************************************/
	class Walk :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Walk* GetInstance(){ static Walk instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Walk() {};
		~Walk() {};

		Walk(const Walk&) {}
		Walk& operator=(const Walk&) {}
	};

	/*******************************************************/
	//					������
	/*******************************************************/
	class BackWalk :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static BackWalk* GetInstance(){ static BackWalk instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		BackWalk() {};
		~BackWalk() {};

		BackWalk(const BackWalk&) {}
		BackWalk& operator=(const BackWalk&) {}
	};

	/*******************************************************/
	//					����ړ�
	/*******************************************************/
	class Run :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Run* GetInstance(){ static Run instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Run() {};
		~Run() {};

		Run(const Run&) {}
		Run& operator=(const Run&) {}
	};

	/*******************************************************/
	//					�o�N�X�e
	/*******************************************************/
	class BackStep :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static BackStep* GetInstance(){ static BackStep instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		BackStep() {};
		~BackStep() {};

		BackStep(const BackStep&) {}
		BackStep& operator=(const BackStep&) {}
	};


	/*******************************************************/
	//					�u���[�L
	/*******************************************************/
	//class FrontBrake :public State<BasePlayer>
	//{
	//public:

	//	// this is a �V���O���g��
	//	static FrontBrake* GetInstance();

	//	// ����
	//	virtual void Enter(BasePlayer* pPerson);

	//	// ���s���܂�
	//	virtual void Execute(BasePlayer* pPerson);

	//	// �A��
	//	virtual void Exit(BasePlayer* pPerson);

	//	// �`��
	//	virtual void Render(BasePlayer* pPerson);

	//	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	//private:
	//	FrontBrake() {};
	//	~FrontBrake() {};

	//	FrontBrake(const FrontBrake&) {}
	//	FrontBrake& operator=(const FrontBrake&) {}
	//};

	/*******************************************************/
	//					U�^�[���u���[�L
	/*******************************************************/
	//class TurnOverBrake :public State<BasePlayer>
	//{
	//public:

	//	// this is a �V���O���g��
	//	static TurnOverBrake* GetInstance();

	//	// ����
	//	virtual void Enter(BasePlayer* pPerson);

	//	// ���s���܂�
	//	virtual void Execute(BasePlayer* pPerson);

	//	// �A��
	//	virtual void Exit(BasePlayer* pPerson);

	//	// �`��
	//	virtual void Render(BasePlayer* pPerson);

	//	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	//private:
	//	TurnOverBrake() {};
	//	~TurnOverBrake() {};

	//	TurnOverBrake(const TurnOverBrake&) {}
	//	TurnOverBrake& operator=(const TurnOverBrake&) {}
	//};

	/*******************************************************/
	//					�W�����v
	/*******************************************************/
	class Jump :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Jump* GetInstance(){ static Jump instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		Jump() {};
		~Jump() {};

		Jump(const Jump&) {}
		Jump& operator=(const Jump&) {}
	};

	/*******************************************************/
	//					�󒆃W�����v
	/*******************************************************/
	class AerialJump :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static AerialJump* GetInstance(){ static AerialJump instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		AerialJump() {};
		~AerialJump() {};

		AerialJump(const AerialJump&) {}
		AerialJump& operator=(const AerialJump&) {}
	};

	/*******************************************************/
	//					����
	/*******************************************************/
	class Fall :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Fall* GetInstance(){ static Fall instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		Fall() {};
		~Fall() {};

		Fall(const Fall&) {}
		Fall& operator=(const Fall&) {}
	};


	/*******************************************************/
	//					���n�X�e�[�g
	/*******************************************************/
	class Land :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Land* GetInstance(){ static Land instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		Land() {};
		~Land() {};

		Land(const Land&) {}
		Land& operator=(const Land&) {}
	};


	/*******************************************************/
	//					���b�V���U���X�e�[�g
	/*******************************************************/
	class RushAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static RushAttack* GetInstance(){ static RushAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		RushAttack() {};
		~RushAttack() {};

		RushAttack(const RushAttack&) {}
		RushAttack& operator=(const RushAttack&) {}
	};

	/*******************************************************/
	//					�m�b�N�o�b�N�X�e�[�g
	/*******************************************************/
	class KnockBack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static KnockBack* GetInstance(){ static KnockBack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		KnockBack() {};
		~KnockBack() {};

		KnockBack(const KnockBack&) {}
		KnockBack& operator=(const KnockBack&) {}
	};

	/*******************************************************/
	//		��������炢�X�e�[�g�@
	/*******************************************************/
	class DownFall :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static DownFall* GetInstance(){ static DownFall instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		DownFall() {};
		~DownFall() {};

		DownFall(const DownFall&) {}
		DownFall& operator=(const DownFall&) {}
	};


	/*******************************************************/
	//					�m�b�N�_�E���X�e�[�g
	/*******************************************************/
	class KnockDown :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static KnockDown* GetInstance(){ static KnockDown instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		KnockDown() {};
		~KnockDown() {};

		KnockDown(const KnockDown&) {}
		KnockDown& operator=(const KnockDown&) {}
	};

	/*******************************************************/
	//					���ɏ�ԃX�e�[�g
	/*******************************************************/
	class Die :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Die* GetInstance(){ static Die instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		Die() {};
		~Die() {};

		Die(const Die&) {}
		Die& operator=(const Die&) {}
	};

	/*******************************************************/
	//					�󒆃��J�o���[�X�e�[�g
	/*******************************************************/
	class LandRecovery :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static LandRecovery* GetInstance(){ static LandRecovery instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		LandRecovery() {};
		~LandRecovery() {};

		LandRecovery(const LandRecovery&) {}
		LandRecovery& operator=(const LandRecovery&) {}
	};

	/*******************************************************/
	//					�󒆃��J�o���[�X�e�[�g
	/*******************************************************/
	class AerialRecovery :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static AerialRecovery* GetInstance(){ static AerialRecovery instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:

		AerialRecovery() {};
		~AerialRecovery() {};

		AerialRecovery(const AerialRecovery&) {}
		AerialRecovery& operator=(const AerialRecovery&) {}
	};

	/*******************************************************/
	//					���Ⴊ��
	/*******************************************************/
	class Squat :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Squat* GetInstance(){ static Squat instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Squat() {};
		~Squat() {};

		Squat(const Squat&) {}
		Squat& operator=(const Squat&) {}
	};

	/*******************************************************/
	//					2B�U��
	/*******************************************************/
	class SquatAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static SquatAttack* GetInstance(){ static SquatAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		SquatAttack() {};
		~SquatAttack() {};

		SquatAttack(const SquatAttack&) {}
		SquatAttack& operator=(const SquatAttack&) {}
	};

	/*******************************************************/
	//					�΋�U��
	/*******************************************************/
	class AntiAirAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static AntiAirAttack* GetInstance(){ static AntiAirAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		AntiAirAttack() {};
		~AntiAirAttack() {};

		AntiAirAttack(const AntiAirAttack&) {}
		AntiAirAttack& operator=(const AntiAirAttack&) {}
	};

	/*******************************************************/
	//					���i�U��
	/*******************************************************/
	class DownAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static DownAttack* GetInstance() { static DownAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		DownAttack() {};
		~DownAttack() {};

		DownAttack(const DownAttack&) {}
		DownAttack& operator=(const DownAttack&) {}
	};


	/*******************************************************/
	//					���i�U��
	/*******************************************************/
	class DokkoiAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static DokkoiAttack* GetInstance() { static DokkoiAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		DokkoiAttack() {};
		~DokkoiAttack() {};

		DokkoiAttack(const DownAttack&) {}
		DokkoiAttack& operator=(const DownAttack&) {}
	};

	/*******************************************************/
	//					�󒆍U�� 
	/*******************************************************/
	class AerialAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static AerialAttack* GetInstance(){ static AerialAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		AerialAttack() {};
		~AerialAttack() {};

		AerialAttack(const AerialAttack&) {}
		AerialAttack& operator=(const AerialAttack&) {}
	};

	/*******************************************************/
	//					�󒆉��U�� 
	/*******************************************************/
	//class AerialDropAttack :public State<BasePlayer>
	//{
	//public:

	//	// this is a �V���O���g��
	//	static AerialDropAttack* GetInstance();

	//	// ����
	//	virtual void Enter(BasePlayer* pPerson);

	//	// ���s���܂�
	//	virtual void Execute(BasePlayer* pPerson);

	//	// �A��
	//	virtual void Exit(BasePlayer* pPerson);

	//	// �`��
	//	virtual void Render(BasePlayer* pPerson);

	//	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	//private:
	//	AerialDropAttack() {};
	//	~AerialDropAttack() {};

	//	AerialDropAttack(const AerialDropAttack&) {}
	//	AerialDropAttack& operator=(const AerialDropAttack&) {}
	//};

	/*******************************************************/
	//					���
	/*******************************************************/
	class Escape :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Escape* GetInstance(){ static Escape instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Escape() {};
		~Escape() {};

		Escape(const Escape&) {}
		Escape& operator=(const Escape&) {}
	};


	/*******************************************************/
	//					�y���\�i�������[�V����
	/*******************************************************/
	class StandAction :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static StandAction* GetInstance(){ static StandAction instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		StandAction() {};
		~StandAction() {};

		StandAction(const StandAction&) {}
		StandAction& operator=(const StandAction&) {}
	};

	/*******************************************************/
	//				�t�B�j�b�V�����[�V����
	/*******************************************************/
	class FinishAttack :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static FinishAttack* GetInstance(){ static FinishAttack instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		FinishAttack() {};
		~FinishAttack() {};

		FinishAttack(const StandAction&) {}
		FinishAttack& operator=(const StandAction&) {}
	};
	
	/*******************************************************/
	//				�L�����N�^�[�ŗL���[�V����
	/*******************************************************/
	class Skill :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Skill* GetInstance(){ static Skill instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Skill() {};
		~Skill() {};

		Skill(const Skill&) {}
		Skill& operator=(const Skill&) {}
	};

	/*******************************************************/
	//				1more�o�����[�V����
	/*******************************************************/
	class OverDrive_OneMore :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static OverDrive_OneMore* GetInstance(){ static OverDrive_OneMore instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		OverDrive_OneMore() {};
		~OverDrive_OneMore() {};

		OverDrive_OneMore(const OverDrive_OneMore&) {}
		OverDrive_OneMore& operator=(const OverDrive_OneMore&) {}
	};

	/*******************************************************/
	//				Burst�o�����[�V����
	/*******************************************************/
	class OverDrive_Burst :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static OverDrive_Burst* GetInstance(){ static OverDrive_Burst instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		OverDrive_Burst() {};
		~OverDrive_Burst() {};

		OverDrive_Burst(const OverDrive_Burst&) {}
		OverDrive_Burst& operator=(const OverDrive_Burst&) {}
	};

	/*******************************************************/
	//				�K�[�h���[�V����
	/*******************************************************/
	class Guard :public State<BasePlayer>
	{
	public:
		// this is a �V���O���g��
		static Guard* GetInstance(){ static Guard instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Guard() {};
		~Guard() {};

		Guard(const Guard&) {}
		Guard& operator=(const Guard&) {}
	};


	/*******************************************************/
	//				�������[�V����
	/*******************************************************/
	class Throw :public State<BasePlayer>
	{
	public:
		// this is a �V���O���g��
		static Throw* GetInstance(){ static Throw instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Throw() {};
		~Throw() {};

		Throw(const Throw&) {}
		Throw& operator=(const Throw&) {}
	};

	/*******************************************************/
	//				�����P�\���[�V����
	/*******************************************************/
	class ThrowHold :public State<BasePlayer>
	{
	public:
		// this is a �V���O���g��
		static ThrowHold* GetInstance() { static ThrowHold instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		ThrowHold() {};
		~ThrowHold() {};

		ThrowHold(const ThrowHold&) {}
		ThrowHold& operator=(const ThrowHold&) {}
	};

	/*******************************************************/
	//				�����������[�V����
	/*******************************************************/
	class ThrowSuccess :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static ThrowSuccess* GetInstance(){ static ThrowSuccess instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		ThrowSuccess() {};
		~ThrowSuccess() {};

		ThrowSuccess(const ThrowSuccess&) {}
		ThrowSuccess& operator=(const ThrowSuccess&) {}
	};

	/*******************************************************/
	//				�͂܂ꂽ���[�V����
	/*******************************************************/
	class ThrowBind :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static ThrowBind* GetInstance(){ static ThrowBind instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		ThrowBind() {};
		~ThrowBind() {};

		ThrowBind(const ThrowBind&) {}
		ThrowBind& operator=(const ThrowBind&) {}
	};

	/*******************************************************/
	//				�͂݉����������Ă͂����X�e�[�g
	/*******************************************************/
	class SuccessThrowRelease :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static SuccessThrowRelease* GetInstance(){ static SuccessThrowRelease instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		SuccessThrowRelease() {};
		~SuccessThrowRelease() {};

		SuccessThrowRelease(const SuccessThrowRelease&) {}
		SuccessThrowRelease& operator=(const SuccessThrowRelease&) {}
	};

	/*******************************************************/
	//				�K�E�Z�X�e�[�g
	/*******************************************************/
	class HeavehoDrive :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static HeavehoDrive* GetInstance(){ static HeavehoDrive instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		HeavehoDrive() {};
		~HeavehoDrive() {};

		HeavehoDrive(const HeavehoDrive&) {}
		HeavehoDrive& operator=(const HeavehoDrive&) {}
	};

	/*******************************************************/
	//				���K�E�Z�X�e�[�g
	/*******************************************************/
	class HeavehoDriveOverFlow :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static HeavehoDriveOverFlow* GetInstance(){ static HeavehoDriveOverFlow instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		HeavehoDriveOverFlow() {};
		~HeavehoDriveOverFlow() {};

		HeavehoDriveOverFlow(const HeavehoDriveOverFlow&) {}
		HeavehoDriveOverFlow& operator=(const HeavehoDriveOverFlow&) {}
	};

	
	/*******************************************************/
	//				���K�E�Z_�����X�e�[�g
	/*******************************************************/
	class HeavehoDriveOverFlow_Success :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static HeavehoDriveOverFlow_Success* GetInstance(){ static HeavehoDriveOverFlow_Success instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		HeavehoDriveOverFlow_Success() {};
		~HeavehoDriveOverFlow_Success() {};

		HeavehoDriveOverFlow_Success(const HeavehoDriveOverFlow_Success&) {}
		HeavehoDriveOverFlow_Success& operator=(const HeavehoDriveOverFlow_Success&) {}
	};

	/*******************************************************/
	//				���E���h�擾(WIN)
	/*******************************************************/
	class Win :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Win* GetInstance(){ static Win instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		Win() {};
		~Win() {};

		Win(const Win&) {}
		Win& operator=(const Win&) {}
	};

	/*******************************************************/
	//				�󒆃_�b�V��
	/*******************************************************/
	class AerialDash :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static AerialDash* GetInstance() { static AerialDash instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);


	private:
		AerialDash() {};
		~AerialDash() {};

		AerialDash(const AerialDash&) {}
		AerialDash& operator=(const AerialDash&) {}
	};

	/*******************************************************/
	//				�󒆃o�b�N�_�b�V��
	/*******************************************************/
	class AerialBackDash :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static AerialBackDash* GetInstance() { static AerialBackDash instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);

	private:
		AerialBackDash() {};
		~AerialBackDash() {};

		AerialBackDash(const AerialBackDash&) {}
		AerialBackDash& operator=(const AerialBackDash&) {}
	};

	/*******************************************************/
	//				�C���g�����o�p
	/*******************************************************/
	class Intro :public State<BasePlayer>
	{
	public:

		// this is a �V���O���g��
		static Intro* GetInstance() { static Intro instance; return &instance; }

		// ����
		virtual void Enter(BasePlayer* pPerson);

		// ���s���܂�
		virtual void Execute(BasePlayer* pPerson);

		// �A��
		virtual void Exit(BasePlayer* pPerson);

		// �`��
		virtual void Render(BasePlayer* pPerson);

		// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
		virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);

	private:
		Intro() {};
		~Intro() {};

		Intro(const Intro&) {}
		Intro& operator=(const Intro&) {}
	};

	/*******************************************************/
	//				���U�� (�O�i��)
	/*******************************************************/
	//class StrongAttack :public State<BasePlayer>
	//{
	//public:
	//
	//	// this is a �V���O���g��
	//	static StrongAttack* GetInstance() { static StrongAttack instance; return &instance; }
	//
	//	// ����
	//	virtual void Enter(BasePlayer* pPerson);
	//
	//	// ���s���܂�
	//	virtual void Execute(BasePlayer* pPerson);
	//
	//	// �A��
	//	virtual void Exit(BasePlayer* pPerson);
	//
	//	// �`��
	//	virtual void Render(BasePlayer* pPerson);
	//
	//	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	//	virtual bool OnMessage(BasePlayer* pPerson, const Message& msg);
	//
	//
	//private:
	//	StrongAttack() {};
	//	~StrongAttack() {};
	//
	//	StrongAttack(const StrongAttack&) {}
	//	StrongAttack& operator=(const StrongAttack&) {}
	//};

}

