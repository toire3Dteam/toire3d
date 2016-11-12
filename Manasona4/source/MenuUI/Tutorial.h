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
	BaseTutorial();
	virtual ~BaseTutorial();

	virtual void Init() = 0;// �ŏ��ɖ��񏉊���
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

	// �ŏ���Tips�E�Ō��Tips TODO�r��Tips�݂����ȃy�b����񂱂ɂȂ鉉�o
	TipsCard* m_pIntroTips;
	TipsCard* m_pClearTips;

	// �^�X�N�f�[�^
	struct Task
	{
		LPSTR pSting;	// ����
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

	tdn2DAnim* m_pClearPic;
};

//+-----------------------------
//	��������
//+-----------------------------
class  WalkTutorial :public BaseTutorial
{
public:
	WalkTutorial();
	~WalkTutorial() {};

	void Init();

	// ���ꂼ��̃^�X�N�������ō��
	void TaskUpdate(BasePlayer* pPerson); //�������bClear��_�������Ă���

private:
	int m_iWalkFrame;
	int m_iBackFrame;

};
