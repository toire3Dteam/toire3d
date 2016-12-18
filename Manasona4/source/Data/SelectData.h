#pragma once

#include "../BaseEntity/Message/Message.h"

//*****************************************************************************
//
//		�Z���N�g�V�[���őI�������f�[�^�p(�V���O���g��)
//
//*****************************************************************************

// �L�����N�^�[�̃^�C�v
enum class CHARACTER { AIROU, TEKI, NAZENARA, END };

// �p�[�g�i�[�̃^�C�v
enum class PARTNER { MOKOI, MAYA, END };

// �X�e�[�W�̃^�C�v
enum class STAGE { SAND, SYUTEN, SEA, GARDEN, NANASATO_SITY, 
	//HEAVE_HO, A,	// �����������Ƃɂ��ꂽ�X�e�[�W����
	MAX };

// AI�̃^�C�v
enum class AI_TYPE
{
	CPU_EASY,			// �アAI
	CPU_NORMAL,			// ���ʂ�AI
	CPU_HARD,			// ����AI
	CPU_YOKOE,			// ���]����p�ɍ��ꂽ�}�W�L�`��AI

	PRACTICE_LAND,		//	[���K�p]���������Ȃ�
	PRACTICE_JUMP,		//	[���K�p]��ё�����
	PRACTICE_ATTACK,	//	[���K�p]���Ԋu�ōU������

	INTERRUPT_INVINCIBLE	// [�f�o�b�O�p]�K�[�h���ɂЂ����疳�G���荞�݂�����
};

struct SideData
{
	CHARACTER eCharacter;
	PARTNER ePartner;
	int iDeviceID;	// �R���g���[���[�f�o�C�X��ID
	bool bAI;		// AI�t���O
	AI_TYPE eAIType;// AI�̃^�C�v
	SideData();
	//SideData &operator=(const SideData &data) { character = data.character, partner = data.partner, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// ���E�̏��
	STAGE eStage;			// �I��ł�X�e�[�W
	bool bTutorial;			// �`���[�g���A���t���O
	bool bTraining;			// �g���[�j���O�t���O
	int iWinRound;			// �������E���h��
	int iRoundTime;			// �ꎎ������
	int iBattleMusicID;		// �퓬�Ȃ̔ԍ�

	SelectData();
	//SelectData &operator=(const SelectData &data) { tagSideDatas[(int)SIDE::LEFT] = data.tagSideDatas[(int)SIDE::LEFT], tagSideDatas[(int)SIDE::RIGHT] = data.tagSideDatas[(int)SIDE::RIGHT], stage = data.stage, bTutorial = data.bTutorial, iWinRound = data.iWinRound, iBattleMusicID = data.iBattleMusicID; return *this; }
};

class SelectDataManager
{
public:
	static SelectDataManager *GetInstance(){ static SelectDataManager i; return &i; }

	SelectData *Get() { return &m_tagData; }
private:

	SelectData m_tagData;

	SelectDataManager(){}
	SelectDataManager(const SelectDataManager&) {}
	SelectDataManager &operator=(const SelectDataManager&) {}
};

#define SelectDataMgr (SelectDataManager::GetInstance())