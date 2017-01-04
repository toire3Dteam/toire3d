#include "TrophyManager.h"
#include "Data\PlayerData.h"

//+--------------------------
//	�g���t�B�[�̊Ǘ��N���X
//+--------------------------

// ���̂̐錾
TrophyManager* TrophyManager::m_pInstance = nullptr;

TrophyManager::TrophyManager() :BaseGameEntity(ENTITY_ID::TROPHY_MGR)	// �G���e�B�e�BID�o�^
{
	m_eSelectTrophy = TROPHY_TYPE::FIRST;

	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		switch ((TROPHY_TYPE)i)
		{
		case TROPHY_TYPE::FIRST:
			m_pTrophy[i] = new FirstTrophy();

			break;
		case TROPHY_TYPE::FIRST_BATTLE:
			m_pTrophy[i] = new FirstBattleTrophy();

			break;
		default:
			m_pTrophy[i] = new FirstTrophy();
			//MyAssert(0, "���̃^�C�v�͑��݂��Ȃ��B");	
			break;
		}

	}
}

TrophyManager::~TrophyManager()
{
	// �g���t�B�[�S���
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTrophy[i]);
	}

}

void TrophyManager::Update()
{
	// �I�����Ă���g���t�B�[
	m_pTrophy[(int)m_eSelectTrophy]->Update();

	// �S�g���t�B�[�X�V
	//for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	//{
	//	m_pTrophy[i]->Update();
	//}

}

void TrophyManager::Render()
{
	// �I�����Ă���g���t�B�[
	m_pTrophy[(int)m_eSelectTrophy]->Render();

	// �S�g���t�B�[�`��
	//for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	//{
	//	m_pTrophy[i]->Render();
	//}

#ifdef _DEBUG

	tdnText::Draw(10, 690, 0xffffffff, "1�Ԗڂ̃g���t�B�[->%d", PlayerDataMgr->m_TrophyData.iFirstPlay);	
	tdnText::Draw(300, 690, 0xffffffff, "2�Ԗڂ̃g���t�B�[->%d", PlayerDataMgr->m_TrophyData.iFirstBattle);

#endif // _DEBUG

}

bool TrophyManager::HandleMessage(const Message & msg)
{
	// ��񕜌�
	TROPHY_TYPE* l_eType = (TROPHY_TYPE*)msg.ExtraInfo;

	// ID���m�F���đ����Ă������b�Z�[�W���d��
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TROPHY_GET:
		

		switch (*l_eType)
		{
		case TROPHY_TYPE::FIRST:
			if (PlayerDataMgr->m_TrophyData.iFirstPlay == 0) 
			{
				PlayerDataMgr->m_TrophyData.iFirstPlay = 1;
				m_eSelectTrophy = TROPHY_TYPE::FIRST;
				m_pTrophy[(int)m_eSelectTrophy]->Action(30);
				return true;
			}
			break;
		case TROPHY_TYPE::FIRST_BATTLE:
			if (PlayerDataMgr->m_TrophyData.iFirstBattle== 0)
			{
				PlayerDataMgr->m_TrophyData.iFirstBattle = 1;
				m_eSelectTrophy = TROPHY_TYPE::FIRST_BATTLE;
				m_pTrophy[(int)m_eSelectTrophy]->Action(30);
				return true;
			}
			break;
		default:
			break;
		}


		break;
	default:

		break;
	}

	return false;
}
