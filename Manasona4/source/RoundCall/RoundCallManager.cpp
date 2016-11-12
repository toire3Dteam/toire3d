#include "RoundCallManager.h"


//+-----------------------------
//	���E���h�R�[���}�l�[�W���[
//+-----------------------------

RoundCallManager::RoundCallManager() :BaseGameEntity(ENTITY_ID::ROUND_CALL_MGR)
{

	for (int i = 0; i < (int)ROUND_TYPE::ARRAY_END; i++)
	{
		switch ((ROUND_TYPE)i)
		{
		case ROUND_TYPE::FIRST:
			m_pRoundCall[i] = new FirstRoundCall();
			break;
		case ROUND_TYPE::REBEL:
			m_pRoundCall[i] = new RebelCall();
			break;
		case ROUND_TYPE::ACTION:
			m_pRoundCall[i] = new ActionCall();
			break;
		case ROUND_TYPE::FINISH:
			m_pRoundCall[i] = new FinishCall();
			break;
		case ROUND_TYPE::OVERDRIVE_FINISH:
			m_pRoundCall[i] = new OverDriveFinishCall();
			break;
		default:
			break;
		}
	}

	// �f�B���C������
	for (int i = 0; i < (int)ROUND_TYPE::ARRAY_END; i++)
	{
		m_iDelayTimer[i] = 0;
	}

}

RoundCallManager::~RoundCallManager()
{
	for (int i = 0; i < (int)ROUND_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pRoundCall[i]);
	}
}

void RoundCallManager::Update()
{
	for (int i = 0; i < (int)ROUND_TYPE::ARRAY_END; i++)
	{
		if (--m_iDelayTimer[(int)i] >= 0) { continue; };// ����Ƀf�N�������g���ăf�B���C���Ȃ�͂���	
		m_pRoundCall[i]->Update();

	}

}

void RoundCallManager::Render()
{
	for (int i = 0; i < (int)ROUND_TYPE::ARRAY_END; i++)
	{
		if (m_iDelayTimer[(int)i] >= 0) { continue; };// �f�B���C���Ȃ�͂���	
		m_pRoundCall[i]->Render();
	}
}

void RoundCallManager::Action(ROUND_TYPE type, int delayTimer)
{
	m_pRoundCall[(int)type]->Action();
	m_iDelayTimer[(int)type] = delayTimer;
}

void  RoundCallManager::RebelAction(int roundNum, int delayTimer)
{
	m_pRoundCall[(int)ROUND_TYPE::REBEL]->Action(roundNum);
	m_iDelayTimer[(int)ROUND_TYPE::REBEL] = delayTimer;
}

void RoundCallManager::CallFirstGameRound()
{

	// �ŏ�
	m_pRoundCall[(int)ROUND_TYPE::FIRST]->Action();
	m_iDelayTimer[(int)ROUND_TYPE::FIRST] = 30;
	
	// ���x�����񂽂炩�񂽂�
	m_pRoundCall[(int)ROUND_TYPE::REBEL]->Action(1);
	m_iDelayTimer[(int)ROUND_TYPE::REBEL] = 150;

	// �A�N�V����
	m_pRoundCall[(int)ROUND_TYPE::ACTION]->Action();
	m_iDelayTimer[(int)ROUND_TYPE::ACTION] = 270;

}

void RoundCallManager::CallRound(int roundNum)
{
	// ���x�����񂽂炩�񂽂�
	m_pRoundCall[(int)ROUND_TYPE::REBEL]->Action(roundNum);
	m_iDelayTimer[(int)ROUND_TYPE::REBEL] = 0;

	// �A�N�V����
	m_pRoundCall[(int)ROUND_TYPE::ACTION]->Action();
	m_iDelayTimer[(int)ROUND_TYPE::ACTION] = 120;
}

void RoundCallManager::CallFinish(ENTITY_ID WinnerID)
{
	// �t�B�j�b�V��
	m_pRoundCall[(int)ROUND_TYPE::FINISH]->Action(WinnerID);
	m_iDelayTimer[(int)ROUND_TYPE::FINISH] = 0;
}

void RoundCallManager::CallOverDriveFinish(ENTITY_ID WinnerID)
{
	// �I�[�o�[�h���C�u�t�B�j�b�V��
	m_pRoundCall[(int)ROUND_TYPE::OVERDRIVE_FINISH]->Action(WinnerID);
	m_iDelayTimer[(int)ROUND_TYPE::OVERDRIVE_FINISH] = 0;
}

bool RoundCallManager::HandleMessage(const Message& msg)
{
	//switch ()
	//{
	//default:
	//	break;
	//}

	return false;
}
