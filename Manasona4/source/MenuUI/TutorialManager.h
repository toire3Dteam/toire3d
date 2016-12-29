#pragma once
#include "Tutorial.h"

//+----------------------------
// �`���[�g���A���}�l�[�W���[
//+----------------------------

enum class TUTORIAL_TYPE
{
	WALK,
	SQUAT,
	JUMP,
	ATTACK,
	SKILL,
	OVER_DRIVE,
	RUSH,
	STAND_GUARD,
	SQUAT_GUARD,
	DUSH,
	BACK_STEP,
	PARTNER,
	INVINCIBLE,
	THROW,
	ESCAPE,
	DOKKOI,
	DOWN_ATTACK,
	OD_ONEMORE,
	OD_BURST,
	ARRAY_END
};

enum class TUTORIAL_BUSY_TYPE
{
	WALK,
	SQUAT,
	JUMP,
	ATTACK,
	SKILL,
	OVER_DRIVE,
	RUSH,
	STAND_GUARD,
	SQUAT_GUARD,
	DUSH,
	BACK_STEP,
	ARRAY_END
};


// �Z���N�g��ʂ⃁�C���ł��g���Z�[�u�f�[�^���l������Ȃ�	
// ���b�Z�[�W�Ȃǂł͈������炢�Ǝv���V���O���g���ł܂�����Ă݂��B
class TutorialManager
{
private:
	// �V���O���g���̍�@
	TutorialManager();
	~TutorialManager();
	TutorialManager(const TutorialManager&);
	TutorialManager &operator=(const TutorialManager&) {}

	// 1�����Ȃ�����
	static TutorialManager *m_pInstance;

public:
	// ���̎擾
	static TutorialManager *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new TutorialManager;
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	// ������
	void Init(TUTORIAL_TYPE type,int DeviceID);
	void Update();
	void TaskUpdate(BasePlayer* pPerson);
	void Render();

	// �Z�����l�̂��߃o�[�W�������ǉ�
	void InitBusy(TUTORIAL_BUSY_TYPE type, int DeviceID);
	void UpdateBusy();
	void TaskUpdateBusy(BasePlayer* pPerson);
	void RenderBusy();

	// �A�N�Z�T�@
	BaseTutorial* GetTutorial() { return m_pTutorial[(int)m_eType]; };// [11/4]���̏����ꂵ���g��Ȃ��Ă�������
	bool isBusy() { return m_bBusy; }
	void SetBusy(bool flag) { m_bBusy = flag; }
	BaseTutorial* GetTutorialBusy() { return m_pBusyTutorial[(int)m_eBusyType]; };

private:
	// ���ꂼ��̃`���[�g���A��
	BaseTutorial* m_pTutorial[(int)TUTORIAL_TYPE::ARRAY_END];
	TUTORIAL_TYPE m_eType;

	// �Z�����l�̂��ߗp
	bool m_bBusy;
	BaseTutorial* m_pBusyTutorial[(int)TUTORIAL_BUSY_TYPE::ARRAY_END];
	TUTORIAL_BUSY_TYPE m_eBusyType;

};

// ���̉�
#define TutorialMgr (TutorialManager::GetInstance())
