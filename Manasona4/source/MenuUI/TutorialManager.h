#pragma once
#include "Tutorial.h"

//+----------------------------
// �`���[�g���A���}�l�[�W���[
//+----------------------------

enum TUTORIAL_TYPE
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
	ESCAPE,
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

	// �A�N�Z�T�@
	BaseTutorial* GetTutorial() { return m_pTutorial[m_eType]; };// [11/4]���̏����ꂵ���g��Ȃ��Ă�������


private:
	// ���ꂼ��̃`���[�g���A��
	BaseTutorial* m_pTutorial[TUTORIAL_TYPE::ARRAY_END];

	TUTORIAL_TYPE m_eType;


};

// ���̉�
#define TutorialMgr (TutorialManager::GetInstance())
