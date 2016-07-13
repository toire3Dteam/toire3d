#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
//#include "AI\Message\Message.h"
//#include "AI\Message\MessageDispatcher.h"

class FieldPlayer;

/*

�l�̃X�e�[�g�}�V���̃J�[�g���b�W

*/


//-------------------�O���[�o���X�e�[�gAI
class FieldPlayer_GlobalState_AI :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_GlobalState_AI* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

	int DEBUG_FLAME = 0;
private:
	FieldPlayer_GlobalState_AI() {};
	~FieldPlayer_GlobalState_AI() {};

	FieldPlayer_GlobalState_AI(const FieldPlayer_GlobalState_AI&);
	FieldPlayer_GlobalState_AI& operator=(const FieldPlayer_GlobalState_AI&);
};



//--------------------�ҋ@-AI
class FieldPlayer_Wait_AI :public State<FieldPlayer>
{
public:

	// this is a �V���O���g��
	static FieldPlayer_Wait_AI* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);



	int m_MoveFlame;
private:
	FieldPlayer_Wait_AI() {};
	~FieldPlayer_Wait_AI() {};

	FieldPlayer_Wait_AI(const FieldPlayer_Wait_AI&);
	FieldPlayer_Wait_AI& operator=(const FieldPlayer_Wait_AI&);
};

//--------------------����-AI
class FieldPlayer_Run_AI :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Run_AI* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Run_AI() {};
	~FieldPlayer_Run_AI() {};

	FieldPlayer_Run_AI(const FieldPlayer_Run_AI&);
	FieldPlayer_Run_AI& operator=(const FieldPlayer_Run_AI&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------�W�����v-AI
class FieldPlayer_Jump_AI :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Jump_AI* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Jump_AI() {};
	~FieldPlayer_Jump_AI() {};

	FieldPlayer_Jump_AI(const FieldPlayer_Jump_AI&);
	FieldPlayer_Jump_AI& operator=(const FieldPlayer_Jump_AI&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------����-AI
class FieldPlayer_Have_AI:public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Have_AI* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Have_AI() {};
	~FieldPlayer_Have_AI() {};

	FieldPlayer_Have_AI(const FieldPlayer_Have_AI&);
	FieldPlayer_Have_AI& operator=(const FieldPlayer_Have_AI&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------������-AI
class FieldPlayer_throw_AI :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_throw_AI* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_throw_AI() {};
	~FieldPlayer_throw_AI() {};

	FieldPlayer_throw_AI(const FieldPlayer_throw_AI&);
	FieldPlayer_throw_AI& operator=(const FieldPlayer_throw_AI&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};



//-------------------�O���[�o���X�e�[�g
class FieldPlayer_GlobalState :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_GlobalState* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_GlobalState() {};
	~FieldPlayer_GlobalState() {};

	FieldPlayer_GlobalState(const FieldPlayer_GlobalState&);
	FieldPlayer_GlobalState& operator=(const FieldPlayer_GlobalState&);
};

//--------------------�ҋ@
class FieldPlayer_Wait :public State<FieldPlayer>
{
public:

	// this is a �V���O���g��
	static FieldPlayer_Wait* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Wait() {};
	~FieldPlayer_Wait() {};

	FieldPlayer_Wait(const FieldPlayer_Wait&);
	FieldPlayer_Wait& operator=(const FieldPlayer_Wait&);
};

//--------------------����
class FieldPlayer_Run :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Run* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Run() {};
	~FieldPlayer_Run() {};

	FieldPlayer_Run(const FieldPlayer_Run&);
	FieldPlayer_Run& operator=(const FieldPlayer_Run&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------�W�����v
class FieldPlayer_Jump :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Jump* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Jump() {};
	~FieldPlayer_Jump() {};

	FieldPlayer_Jump(const FieldPlayer_Jump&);
	FieldPlayer_Jump& operator=(const FieldPlayer_Jump&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------����
class FieldPlayer_Have :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Have* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Have() {};
	~FieldPlayer_Have() {};

	FieldPlayer_Have(const FieldPlayer_Have&);
	FieldPlayer_Have& operator=(const FieldPlayer_Have&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};

//--------------------������
class FieldPlayer_throw:public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_throw* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_throw() {};
	~FieldPlayer_throw() {};

	FieldPlayer_throw(const FieldPlayer_throw&);
	FieldPlayer_throw& operator=(const FieldPlayer_throw&);


	// ���̎������̕ϐ�
	//Vector3 m_orgPos;
	//Vector3 m_trunPos;
	//float m_len;
	//bool m_trunFlag;

};


//--------------------����
class FieldPlayer_Dead :public State<FieldPlayer>
{
public:

	//this is a �V���O���g��
	static FieldPlayer_Dead* GetInstance();

	// ����
	virtual void Enter(FieldPlayer* pPerson);

	// ���s���܂�
	virtual void Execute(FieldPlayer* pPerson);

	// �A��
	virtual void Exit(FieldPlayer* pPerson);

	// �`��
	virtual void Render(FieldPlayer* pPerson);

	// �G�[�W�F���g����̃��b�Z�[�W����M�����ꍇ�A���ꂪ���s�����
	virtual bool OnMessage(FieldPlayer* pPerson, const Message& msg);

private:
	FieldPlayer_Dead() {};
	~FieldPlayer_Dead() {};

	FieldPlayer_Dead(const FieldPlayer_Dead&);
	FieldPlayer_Dead& operator=(const FieldPlayer_Dead&);

	int DeadFlame;

};


