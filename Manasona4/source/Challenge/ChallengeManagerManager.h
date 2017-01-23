#pragma once
#include "BaseChallengeManager.h"

//+----------------------------------------------------------
//	�e�L�����N�^�[�̃`�������W�}�l�[�W���[���Ǘ�����N���X
//+----------------------------------------------------------

// �X�[�p�T�C���l�S�b�g�X�[�p�[�T�C���l����

class ChallengeManagerManager
{
private:
	// �V���O���g���̍�@
	ChallengeManagerManager();
	~ChallengeManagerManager();
	ChallengeManagerManager(const ChallengeManagerManager&);
	ChallengeManagerManager &operator=(const ChallengeManagerManager&) {}

	// 1�����Ȃ�����
	static ChallengeManagerManager *m_pInstance;
public:
	// ���̎擾
	static ChallengeManagerManager *GetInstance()
	{
		if (!m_pInstance) m_pInstance = new ChallengeManagerManager;
		return m_pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(m_pInstance);
	}

	// �����C���Ɏg��
	BaseChallengeManager* GetSelectMgr() { return m_pChallengeManager[(int)m_eSelectCharaType]; };

	// �I�����Ă���ϐ��̑���
	CHARACTER GetSelectCharaType() { return m_eSelectCharaType; }
	void SetSelectCharaType(CHARACTER eType) { m_eSelectCharaType = eType; }

private:
	// ���ꂼ��̃`�������W
	BaseChallengeManager* m_pChallengeManager[(int)CHARACTER::END];

	// ���ݑI�����Ă�`�������W�f�[�^
	CHARACTER m_eSelectCharaType;

};

// ���̉�
#define ChallengeMgrMgr (ChallengeManagerManager::GetInstance())
