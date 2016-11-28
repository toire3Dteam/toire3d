#pragma once

#include "BaseEntity\Message\Message.h"

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
enum class STAGE { SAND, SEA, SYUTEN, A, HEAVE_HO, MAX };

struct SideData
{
	CHARACTER character;
	PARTNER partner;
	int iDeviceID;	// �R���g���[���[�f�o�C�X��ID
	bool bAI;		// AI�t���O
	SideData() :character(CHARACTER::AIROU), partner(PARTNER::MAYA), iDeviceID(0), bAI(false) {}
	SideData &operator=(const SideData &data) { character = data.character, partner = data.partner, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// ���E�̏��
	STAGE stage;			// �I��ł�X�e�[�W
	bool bTutorial;			// �`���[�g���A���t���O
	int iWinRound;			// �������E���h��
	int iBattleMusicID;		// �퓬�Ȃ̔ԍ�

	SelectData();
	SelectData &operator=(const SelectData &data) { tagSideDatas[(int)SIDE::LEFT] = data.tagSideDatas[(int)SIDE::LEFT], tagSideDatas[(int)SIDE::RIGHT] = data.tagSideDatas[(int)SIDE::RIGHT], stage = data.stage, bTutorial = data.bTutorial, iWinRound = data.iWinRound, iBattleMusicID = data.iBattleMusicID; return *this; }
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