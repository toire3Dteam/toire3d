#include "TrophyManager.h"
#include "Data\PlayerData.h"
#include "Data\SelectData.h"
#include "Player\PlayerManager.h"
#include "UI\GameUI.h"
#include "BaseEntity\Message\MessageDispatcher.h"

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
		case TROPHY_TYPE::BIG_DAMAGE:
			m_pTrophy[i] = new BigDamageTrophy();

			break;
		case TROPHY_TYPE::MAX_HP_FINISH:
			m_pTrophy[i] = new MaxHpFinishTrophy();

			break;
		case TROPHY_TYPE::SPEED_FINISH:
			m_pTrophy[i] = new SpeedFinishTrophy();

			break;
		case TROPHY_TYPE::TRAINING_TIME:
			m_pTrophy[i] = new TrainingTimeTrophy();

			break;
		case TROPHY_TYPE::POWERFUL_ENEMY:
			m_pTrophy[i] = new PowerfulEnemyWinTrophy();

			break;
		case TROPHY_TYPE::SS_RANK_WIN:
			m_pTrophy[i] = new SSRankWinTrophy();

			break;
		case TROPHY_TYPE::SS_POWERFUL_ENEMY:
			m_pTrophy[i] = new PowerfulEnemySSRankWinTrophy();

			break;
		case TROPHY_TYPE::MENY_CONTENT:
			m_pTrophy[i] = new BuyManyContentTrophy();

			break;
		case TROPHY_TYPE::CHALLENGE_CLEAR:
			m_pTrophy[i] = new ChallengeTrophy();

			break;
		case TROPHY_TYPE::COMPLETE_TROPHY:
			m_pTrophy[i] = new CompleteTrophy();

			break;
		default:
			m_pTrophy[i] = new FirstTrophy();
			//MyAssert(0, "���̃^�C�v�͑��݂��Ȃ��B");	
			break;
		}

	}

	// ���b�N�A�C�R��
	m_pRockIcon = new tdn2DObj("Data/Trophy/Rock.png");


	// �A���擾�p
	m_aActionStack.clear();
	m_aActionStack.reserve(8);
	m_iIntervalFrame = 0;

	m_iTrainingFrame = 0;
}

TrophyManager::~TrophyManager()
{
	// �g���t�B�[�S���
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		SAFE_DELETE(m_pTrophy[i]);
	}

	SAFE_DELETE(m_pRockIcon);

}

void TrophyManager::Update()
{
	// �A���擾�p
	m_iIntervalFrame--;
	if (m_iIntervalFrame <= 0)
	{
		m_iIntervalFrame = 0;	// 0�Ŏ~�߂�

		for (int i = 0; i < (int)m_aActionStack.size(); i++)
		{
			// �Z���N�g�g���t�B�[��ݒ� �ŏ��̓����Ă��鐔�l������
			m_eSelectTrophy = m_aActionStack[i];

			m_pTrophy[(int)m_eSelectTrophy]->Action(20);

			// �����������o��
			//m_aActionStack.pop_back();
			m_aActionStack.erase(m_aActionStack.begin());

			// �Ԋu�t���[���ݒ�(�g���t�B�[�̃A�C�R�����o�銴�o�͂����Œ���)
			m_iIntervalFrame = 240;
			break;
		}

	}


	// �I�����Ă���g���t�B�[
	m_pTrophy[(int)m_eSelectTrophy]->Update();

	// �S�g���t�B�[�X�V
	//for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	//{
	//	m_pTrophy[i]->Update();
	//}

	// ���g���t�B�[��S�ďW�߂����m�F����
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iCompleteTrophy== 0)
	{
		int l_iCount = 0;
		for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
		{
			// �R���v���[�g�g���t�B�[�͔�΂�
			if (PlayerDataMgr->m_TrophyData.iAllData[i]
				== (int)TROPHY_TYPE::COMPLETE_TROPHY)return;

			// ��ɓ���Ă���J�E���g
			if (PlayerDataMgr->m_TrophyData.iAllData[i] == 1)
			{
				l_iCount++;
			}	

		}

		// �S����ɓ��ꂽ��
		if (l_iCount == (int)TROPHY_TYPE::ARRAY_END - 1)
		{
			// �R���v���[�g���߂łƂ�
			TROPHY_TYPE eType = TROPHY_TYPE::COMPLETE_TROPHY;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
		}
	}

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

void TrophyManager::RenderRoom(int iTrophyType, int iX, int iY)
{
	// ���b�N
	bool l_bRock = true;

	//switch ((TROPHY_TYPE)iTrophyType)
	//{
	//case TROPHY_TYPE::FIRST:
	//	if (PlayerDataMgr->m_TrophyData.iFirstPlay == 1)
	//	{
	//		l_bRock = false;
	//	}
	//
	//	break;
	//case TROPHY_TYPE::FIRST_BATTLE:
	//	if (PlayerDataMgr->m_TrophyData.iFirstBattle == 1)
	//	{
	//		l_bRock = false;
	//	}
	//	break;
	//default:
	//	//MyAssert(0, "���̃^�C�v�Ȃ��ł�");
	//	break;
	//}

	if (PlayerDataMgr->m_TrophyData.iAllData[iTrophyType] == 1)
	{
		// �I�����Ă���g���t�B�[
		m_pTrophy[(int)iTrophyType]->RenderRoom(iX, iY);
	}
	else
	{
		// ���b�N�C���X�g
		m_pRockIcon->SetScale(0.5f);
		m_pRockIcon->Render((int)m_pTrophy[(int)iTrophyType]->GetRoomPos().x + iX,
			(int)m_pTrophy[(int)iTrophyType]->GetRoomPos().y + iY);
	}


}

void TrophyManager::RenderInfo(int iTrophyType, int iX, int iY)
{
	// ���b�N ����������Ă���Ȃ�
	if (PlayerDataMgr->m_TrophyData.iAllData[iTrophyType] == 1)
	{
		// ���ʂɐ����`��
		m_pTrophy[(int)iTrophyType]->RenderInfo(iX, iY);

	}else
	{
		// ��������ĂȂ����B���g���t�B�[�Ȃ�
		if (m_pTrophy[(int)iTrophyType]->isHide())
		{
			// �B���p�̃��b�Z�[�W
			// �^�C�g��
			tdnFont::RenderString("�B���g���t�B�[", "HG�޼��E",
				22, iX, iY, 0xffffffff, RS::COPY);
			// ����
			tdnFont::RenderString("���֏�񂪉B����Ă���g���t�B�[�ł��B", "HG�޼��E",
				19, iX, iY + 30, 0xffffffff, RS::COPY);
		}
		else
		{
			// ���ʂɐ����`��
			m_pTrophy[(int)iTrophyType]->RenderInfo(iX, iY);
		}


	}


}

// ���o��؂�
void TrophyManager::Stop()
{
	// �I�����Ă���g���t�B�[
	m_pTrophy[(int)m_eSelectTrophy]->Stop();

}

// �S�g���t�B�[����
void TrophyManager::AllReset()
{
	// �S�g���t�B�[���b�N
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		PlayerDataMgr->m_TrophyData.iAllData[i] = 0;
	}

}

bool TrophyManager::HandleMessage(const Message & msg)
{
	// ��񕜌�
	TROPHY_TYPE* l_eType = (TROPHY_TYPE*)msg.ExtraInfo;

	// ID���m�F���đ����Ă������b�Z�[�W���d��
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::TROPHY_GET:
		

		//switch (*l_eType)
		//{
		//case TROPHY_TYPE::FIRST:
		//	if (PlayerDataMgr->m_TrophyData.iFirstPlay == 0) 
		//	{
		//		PlayerDataMgr->m_TrophyData.iFirstPlay = 1;
		//		m_eSelectTrophy = TROPHY_TYPE::FIRST;
		//		m_pTrophy[(int)m_eSelectTrophy]->Action(30);
		//		return true;
		//	}
		//	break;
		//case TROPHY_TYPE::FIRST_BATTLE:
		//	if (PlayerDataMgr->m_TrophyData.iFirstBattle== 0)
		//	{
		//		PlayerDataMgr->m_TrophyData.iFirstBattle = 1;
		//		m_eSelectTrophy = TROPHY_TYPE::FIRST_BATTLE;
		//		m_pTrophy[(int)m_eSelectTrophy]->Action(30);
		//		return true;
		//	}
		//	break;
		//default:
		//	break;
		//}


		if (PlayerDataMgr->m_TrophyData.iAllData[(int)*l_eType] == 0)
		{
			PlayerDataMgr->m_TrophyData.iAllData[(int)*l_eType] = 1;
			//m_eSelectTrophy = *l_eType;
			//m_pTrophy[(int)m_eSelectTrophy]->Action(20);
			
			// ���ŃA�N�V���������ꎞ�I�ɉ���X�^�b�N�ɔԍ�������
			m_aActionStack.push_back(*l_eType);
			
			return true;
		}


		break;
	default:

		break;
	}

	return false;
}

int TrophyManager::GetTrophyOwned()
{
	int l_iCount = 0;
	//
	for (int i = 0; i < (int)TROPHY_TYPE::ARRAY_END; i++)
	{
		if (PlayerDataMgr->m_TrophyData.iAllData[i] == 1)
		{
			l_iCount++;
		}

	}


	return l_iCount;
}

void TrophyManager::InitSeceneMain()
{
	// �g���[�j���O�؍ݎ���
	m_iTrainingFrame = 0;
}

// �v���C�J�E���g
void TrophyManager::CheakPlayCount(int iPlayCount)
{
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iFirstPlay == 0)
	{
		// ���ȏ�v���C���Ă�����
		if (iPlayCount >= 1)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::FIRST;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
		}

	}

}

// �ΐ��
void TrophyManager::CheakBattleCount(int iBattleCount)
{
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iFirstBattle == 0)
	{
		// ���ȏ�ΐ킵�Ă�����
		if (iBattleCount >= 1)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::FIRST_BATTLE;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);
		}

	}

}

void TrophyManager::CheakBigDamage(bool bVS)
{

	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iBigDamage == 0)
	{

		if (PlayerMgr->GetPlayer(SIDE::LEFT)->isAI() == false)
		{
			// �ő�_���[�W5000�ȏ�
			if (GameUIMgr->GetComboUI(SIDE::RIGHT)->GetMaxDamage() >= 5000)
			{
				TROPHY_TYPE eType = TROPHY_TYPE::BIG_DAMAGE;
				MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

			}

		}
		else if (PlayerMgr->GetPlayer(SIDE::RIGHT)->isAI() == false)
		{
			// �ő�_���[�W5000�ȏ�
			if (GameUIMgr->GetComboUI(SIDE::LEFT)->GetMaxDamage() >= 5000)
			{
				TROPHY_TYPE eType = TROPHY_TYPE::BIG_DAMAGE;
				MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

			}
		}

	}

	

}

void TrophyManager::CheakMaxHpFinish(int iRemainingHP)
{
	// �ΐ킶��Ȃ�������Ԃ�
	//if (bVS == false)return;

	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iMaxHpFinish == 0)
	{
		// 100%�����Ȃ�
		if (iRemainingHP >= 100)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::MAX_HP_FINISH;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}


}

void TrophyManager::CheakSpeedFinish(int iElapsedTime)
{
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iSpeedFinish == 0)
	{
		// ���̐��l�����������������Ȃ�
		if (iElapsedTime <= 20)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::SPEED_FINISH;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}

}

void TrophyManager::CheakTrainingTime()
{
	m_iTrainingFrame++;

	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iTrainingTime == 0)
	{
		// �����Ԉȏ�v���C���Ă���
		if (m_iTrainingFrame >= (60 * 60) * 5)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::TRAINING_TIME;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}
}


void TrophyManager::CheakPowerfulEnemyWin(SIDE eWinnerSide)
{
	// ���������̃T�C�h��m��
	SIDE l_eLoseSide = SIDE::LEFT;
	if (eWinnerSide == SIDE::LEFT)
	{
		l_eLoseSide = SIDE::RIGHT;
	}
	
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iPowerfulEnemyWin == 0)
	{
		// ������������AI�ŏ����ō���Փx��������
		if (SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].bAI == true && 
			SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].eAIType == (AI_TYPE::CPU_YOKOE))
		{

			TROPHY_TYPE eType = TROPHY_TYPE::POWERFUL_ENEMY;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}
}

void TrophyManager::CheakRank(SIDE eWinnerSide, RANK_TYPE eRank)
{
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iSSRankWin == 0)
	{
		// �����������̓v���C���[�ŏ�����SS�����N�Ȃ�
		if (SelectDataMgr->Get()->tagSideDatas[(int)eWinnerSide].bAI == false &&
			eRank == RANK_TYPE::SS)
		{

			TROPHY_TYPE eType = TROPHY_TYPE::SS_RANK_WIN;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}


	// ���������̃T�C�h��m��
	SIDE l_eLoseSide = SIDE::LEFT;
	if (eWinnerSide == SIDE::LEFT)
	{
		l_eLoseSide = SIDE::RIGHT;
	}

	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iSSRankPowerfulEnemyWin == 0)
	{
		// ������������AI�ŏ����ō���Փx��������
		if (SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].bAI == true &&
			SelectDataMgr->Get()->tagSideDatas[(int)l_eLoseSide].eAIType == (AI_TYPE::CPU_YOKOE))
		{

			// �����������̓v���C���[�ŏ�����SS�����N�Ȃ�
			if (SelectDataMgr->Get()->tagSideDatas[(int)eWinnerSide].bAI == false &&
				eRank == RANK_TYPE::SS)
			{

				TROPHY_TYPE eType = TROPHY_TYPE::SS_POWERFUL_ENEMY;
				MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

			}
		}

	}
}

void TrophyManager::CheakBuyManyContent(int iOwnedContent)
{
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iBuyManyContent == 0)
	{
		// ���ȏ㏊�����Ă�����
		if (iOwnedContent >= 10)
		{
			TROPHY_TYPE eType = TROPHY_TYPE::MENY_CONTENT;
			MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

		}

	}


}


void TrophyManager::CheakChallengeClear()
{
	// �܂����̃g���t�B�[����ɓ���Ă��Ȃ�������
	if (PlayerDataMgr->m_TrophyData.iChallengeClear == 0)
	{
		TROPHY_TYPE eType = TROPHY_TYPE::CHALLENGE_CLEAR;
		MsgMgr->Dispatch(0, ENTITY_ID::TROPHY_MGR, ENTITY_ID::TROPHY_MGR, MESSAGE_TYPE::TROPHY_GET, &eType);

	}

}



