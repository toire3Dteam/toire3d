#pragma once
#include "TDNLIB.h"
#include "Player\BasePlayer.h"
#include "MenuUI\TipsCard.h"

//+-----------------
//	�`���[�g���A��
//+-----------------

// [����]
// �������E�N���A�����E�N���A��̐����i�E�F�C�g�t���j�@
// AI�̐ݒ�@�����̏��

// �\��
/*
	1	����
	2	���Ⴊ��
	3	�W�����v
	4	�U��
	5	�h���C�u�U��
	6	�K�E�Z
	7	���b�V���U���@����Ɂ��̎O���Ȃ��Ă���܂�
	8	�K�[�h�@�Ō�ɂ��Ⴊ�݂Ƃ��������邱�Ƃ����
	9	�e�N�j�J��(����)�U���i���i�j
	10	�e�N�j�J��(����)�U���i���i�j
	11	����
	12	�_�b�V���E�o�b�N�X�e�b�v
	13	�󒆃_�b�V���E�󒆃o�b�N�X�e�b�v
	14	���
	15	
	16	�y���\�i����
	17	�o��
	18	�ꌂ�K�E
	19	
	20	

*/


// [����]
// �������E�N���A�����E�N���A��̐����i�E�F�C�g�t���j�@
// AI�̐ݒ�@�����̏��


// �����̕����͂܂�Tuto�̃X�e�[�g�Ńr���E�K�������ۂ��������ȁA���R�ӂ���������@�ۂ���łł����̂��̂ł͂Ȃ��v

//+-----------------------------
//	�x�[�X
//+-----------------------------
class BaseTutorial
{
public:
	BaseTutorial(int iTitleNo);
	virtual ~BaseTutorial();

	virtual void Init(int iDeviceID = 0) = 0;// �ŏ��ɖ��񏉊���
	virtual void Update();
	virtual void Render();

	// �����͋��ʂȂ̂�Virture�͂���Ȃ�
	void ActionIntroTips();
	void StopIntroTips();
	void ActionClearTips();
	void StopClearTips();

	void ActionClear();// �N���A�I���o��

	// ����ōŌ��Tips�������^�C�~���O���擾
	bool EndTipsStart() { return (m_iWaitFrame <= 0) ? true : false; }

	//
	bool isClear() { return m_bClearFlag; }

	TipsCard* GetIntroTipsCard() { return m_pIntroTips; }
	TipsCard* GetClearTipsCard() { return m_pClearTips; }

	// ���ꂼ��̂�邱��
	virtual void TaskUpdate(BasePlayer* pPerson) = 0; //�������bClear��_�������Ă���
	//virtual void TaskReset() = 0; // �~�X�������ɔ���

	void AddTaskData(LPSTR string);

	// �����I
	void TaskSuccess(int select);

	void titleTaskClear() { m_pTaskTitle.bClear = true; };

protected:
	int m_iWaitFrame;// �N���A����ǂꂭ�炢�ŃG���hTips�ֈڍs���邩
	int m_iWaitFrameMAX;// �ő�E�F�C�g��
	bool m_bClearFlag;	//	

	int m_iDeviceID;   // ���삵�Ă���f�o�C�X

	// �ŏ���Tips�E�Ō��Tips TODO�r��Tips�݂����ȃy�b����񂱂ɂȂ鉉�o
	TipsCard* m_pIntroTips;
	TipsCard* m_pClearTips;

	// �^�X�N�f�[�^
	struct Task
	{
		std::string pString;	// ����
		bool bClear;	// �N���A������
		int iStingLength;	//�@������ 
	};
	std::vector<Task> m_aTask;

	// �^�X�N�^�C�g����
	Task m_pTaskTitle;// �^�C�g���̉摜

	tdn2DObj* m_pTaskIcon;
	Vector2 m_vTaskPos;

	int m_iSelectTask;

	bool m_bAllTaskClear;

	int m_iTitleNo;//

	tdn2DAnim* m_pClearPic;
	tdn2DAnim* m_pClearPicRip;
};

//+-----------------------------
//	��������
//+-----------------------------
class  WalkTutorial :public BaseTutorial
{
public:
	WalkTutorial(int iTitleNo);
	~WalkTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:
	int m_iWalkFrame;
	int m_iBackFrame;

};


//+-----------------------------
//	���Ⴊ�݂̐���
//+-----------------------------
class  SquatTutorial :public BaseTutorial
{
public:
	SquatTutorial(int iTitleNo);
	~SquatTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:
	int m_iSquatFrame;

};

//+-----------------------------
//	�W�����v�̐���
//+-----------------------------
class  JumpTutorial :public BaseTutorial
{
public:
	JumpTutorial(int iTitleNo);
	~JumpTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};


//+-----------------------------
//	�U���̐���
//+-----------------------------
class  AttackTutorial :public BaseTutorial
{
public:
	AttackTutorial(int iTitleNo);
	~AttackTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	�X�L���U���̐���
//+-----------------------------
class  SkillTutorial :public BaseTutorial
{
public:
	SkillTutorial(int iTitleNo);
	~SkillTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};


//+-----------------------------
//	�K�E�Z�̐���
//+-----------------------------
class  OverDriveTutorial :public BaseTutorial
{
public:
	OverDriveTutorial(int iTitleNo);
	~OverDriveTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	���b�V���U���̐���
//+-----------------------------
class  RushTutorial :public BaseTutorial
{
public:
	RushTutorial(int iTitleNo);
	~RushTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};


//+-----------------------------
//	�����K�[�h�̐���
//+-----------------------------
class  StandGuardTutorial :public BaseTutorial
{
public:
	StandGuardTutorial(int iTitleNo);
	~StandGuardTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

	bool m_bStop;
	int m_iCount;
};

//+-----------------------------
//	���Ⴊ�݃K�[�h�̐���
//+-----------------------------
class  SquatGuardTutorial :public BaseTutorial
{
public:
	SquatGuardTutorial(int iTitleNo);
	~SquatGuardTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

	bool m_bStop;
	int m_iCount;
};

//+-----------------------------
//	����̐���
//+-----------------------------
class  EscapeTutorial :public BaseTutorial
{
public:
	EscapeTutorial(int iTitleNo);
	~EscapeTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:


};

//+-----------------------------
//	�����̐���
//+-----------------------------
class  ThrowTutorial :public BaseTutorial
{
public:
	ThrowTutorial(int iTitleNo);
	~ThrowTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:


};

//+-----------------------------
//	���G�Z�̐���
//+-----------------------------
class  InvincibleTutorial :public BaseTutorial
{
public:
	InvincibleTutorial(int iTitleNo);
	~InvincibleTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:


};


//+-----------------------------
//	�_�b�V������
//+-----------------------------
class  DushTutorial :public BaseTutorial
{
public:
	DushTutorial(int iTitleNo);
	~DushTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:
	int m_iDushFrame;

};

//+-----------------------------
//	�o�b�N�X�e�b�v����
//+-----------------------------
class  BackStepTutorial :public BaseTutorial
{
public:
	BackStepTutorial(int iTitleNo);
	~BackStepTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	�p�[�g�i�[����
//+-----------------------------
class  PartnerTutorial :public BaseTutorial
{
public:
	PartnerTutorial(int iTitleNo);
	~PartnerTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	���i�U������
//+-----------------------------
class  DokkoiTutorial :public BaseTutorial
{
public:
	DokkoiTutorial(int iTitleNo);
	~DokkoiTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	����������
//+-----------------------------
class  DownAttackTutorial :public BaseTutorial
{
public:
	DownAttackTutorial(int iTitleNo);
	~DownAttackTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	�o������
//+-----------------------------
class  OverDriveOneMoreTutorial :public BaseTutorial
{
public:
	OverDriveOneMoreTutorial(int iTitleNo);
	~OverDriveOneMoreTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};

//+-----------------------------
//	�o�[�X�g�o������
//+-----------------------------
class  OverDriveBurstTutorial :public BaseTutorial
{
public:
	OverDriveBurstTutorial(int iTitleNo);
	~OverDriveBurstTutorial() {};

	void Init(int iDeviceID = 0);

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:

};
