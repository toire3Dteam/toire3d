#pragma once

#include "BaseEntity\Message\Message.h"

//*****************************************************************************
//
//		�Z���N�g�V�[���őI�������f�[�^�p(�V���O���g��)
//
//*****************************************************************************

// �L�����N�^�[�̃^�C�v
enum class CHARACTER { AIROU, END };

// �X�e�[�W�̃^�C�v
enum class STAGE { SENJO, SYUTEN, A, HEAVE_HO, MAX };

struct SideData
{
	CHARACTER CharacterType;
	int iDeviceID;	// �R���g���[���[�f�o�C�X��ID
	bool bAI;		// AI�t���O
	SideData() :CharacterType(CHARACTER::AIROU), iDeviceID(0), bAI(false) {}
	SideData &operator=(const SideData &data) { CharacterType = data.CharacterType, iDeviceID = data.iDeviceID, bAI = data.bAI; return *this; }
};

struct SelectData
{
	SideData tagSideDatas[(int)SIDE::ARRAY_MAX];	// ���E�̏��
	STAGE stage;			// �I��ł�X�e�[�W
	bool bTutorial;			// �`���[�g���A���t���O
	int iWinRound;			// �������E���h��
	int iBattleMusicID;		// �퓬�Ȃ̔ԍ�

	SelectData();
	SelectData &operator=(const SelectData &data) { tagSideDatas[(int)SIDE::LEFT] = data.tagSideDatas[(int)SIDE::LEFT], tagSideDatas[(int)SIDE::RIGHT] = data.tagSideDatas[(int)SIDE::RIGHT], stage = data.stage, bTutorial = data.bTutorial; return *this; }
};

class SelectDataManager
{
public:
	static SelectDataManager *GetInstance(){ static SelectDataManager i; return &i; }

	SelectData *Get() { return &m_data; }
private:

	SelectData m_data;

	SelectDataManager(){}
	SelectDataManager(const SelectDataManager&) {}
	SelectDataManager &operator=(const SelectDataManager&) {}
};

#define SelectDataMgr (SelectDataManager::GetInstance())