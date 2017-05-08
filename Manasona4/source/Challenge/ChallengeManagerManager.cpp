#include "ChallengeManagerManager.h"

//+----------------------------------------------------------
//	�e�L�����N�^�[�̃`�������W�}�l�[�W���[���Ǘ�����N���X
//+----------------------------------------------------------

// ���̂̐錾
ChallengeManagerManager* ChallengeManagerManager::m_pInstance = nullptr;

ChallengeManagerManager::ChallengeManagerManager()
{
	// ������
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		switch ((CHARACTER)i)
		{
		case CHARACTER::AIROU:
			m_pChallengeManager[i] = new AirouChallengeManager();
			break;
		case CHARACTER::TEKI:
			m_pChallengeManager[i] = new TekiChallengeManager();
			break;
		case CHARACTER::NAZENARA:
			m_pChallengeManager[i] = new NazenarabaChallengeManager();
			break;
		case CHARACTER::ARAMITAMA:
			m_pChallengeManager[i] = new AramitamaChallengeManager();
			break;
		case CHARACTER::ANIKI:
			m_pChallengeManager[i] = new AramitamaChallengeManager();
			break;
		case CHARACTER::BALANCE:
			m_pChallengeManager[i] = new AirouChallengeManager();
			break;
		default:
			MyAssert(0, "���̃^�C�v�͑��݂��Ȃ�");
			break;
		}
	}

	// �����őI�����Ă�L����
	m_eSelectCharaType = CHARACTER::AIROU;

}

ChallengeManagerManager::~ChallengeManagerManager()
{
	// ���
	for (int i = 0; i < (int)CHARACTER::END; i++)
	{
		SAFE_DELETE(m_pChallengeManager[i]);
	}
}
