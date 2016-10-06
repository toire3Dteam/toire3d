#include "GameUI.h"
#include "../Number/Number.h"
#include "../Player/PlayerManager.h"

// ���̂̐錾
GameUIManager *GameUIManager::pInstance = nullptr;

/***************/
//	�������E���
/***************/
GameUIManager::GameUIManager() :BaseGameEntity(ENTITY_ID::UI_MGR)	// �G���e�B�e�BID�o�^
{
	m_pHpFrame = new tdn2DAnim("Data/UI/Game/HPFrame.png");
	m_pHpFrame->OrderMoveAppeared(18, 0, -100);

	m_pSpFrame = new tdn2DAnim("Data/UI/Game/SPFrame.png");
	m_pSpFrame->OrderMoveAppeared(16, 0, -80);
	
	m_pHpGage1P = nullptr;
	m_pHpGage2P = nullptr;

	m_pSpGage1P = nullptr;
	m_pSpGage2P = nullptr;

	m_pPersona1P = nullptr;
	m_pPersona2P = nullptr;
}


GameUIManager::~GameUIManager()
{
	SAFE_DELETE(m_pHpFrame);
	SAFE_DELETE(m_pSpFrame);

	SAFE_DELETE(m_pHpGage1P);
	SAFE_DELETE(m_pHpGage2P);

	SAFE_DELETE(m_pSpGage1P);
	SAFE_DELETE(m_pSpGage2P);

	SAFE_DELETE(m_pPersona1P);
	SAFE_DELETE(m_pPersona2P);
}

void GameUIManager::ReferencesPlayer(BasePlayer* pLeftPlayer, BasePlayer* pRightPlayer)
{
	m_pHpGage1P = new HpGage(pLeftPlayer, pLeftPlayer->GetSide());
	m_pHpGage2P = new HpGage(pRightPlayer, pRightPlayer->GetSide());

	m_pSpGage1P = new SpGage(pLeftPlayer, pLeftPlayer->GetSide());
	m_pSpGage2P = new SpGage(pRightPlayer, pRightPlayer->GetSide());

	m_pPersona1P = new PersonaUI(pLeftPlayer, true);
	m_pPersona2P = new PersonaUI(pRightPlayer, false);
}

void GameUIManager::Update()
{
	if (m_pHpGage1P == nullptr)MyAssert(0,"�v���C���[�f�[�^��n���Ă���");

	m_pHpFrame->Update();
	m_pSpFrame->Update();

	// HP�Q�[�W
	m_pHpGage1P->Update();
	m_pHpGage2P->Update();

	// SP�Q�[�W
	m_pSpGage1P->Update();
	m_pSpGage2P->Update();

	// �y���\�iUI
	m_pPersona1P->Update();
	m_pPersona2P->Update();
}

void GameUIManager::Action() 
{
	m_pHpFrame->Action(2);
	m_pSpFrame->Action(16);

	// HP�Q�[�W
	m_pHpGage1P->FirstAction(18);
	m_pHpGage2P->FirstAction(18);


	// �y���\�iUI
	m_pPersona1P->Action(18);
	m_pPersona2P->Action(18);
}

void GameUIManager::Render()
{
	if (m_pHpGage1P == nullptr)MyAssert(0, "�v���C���[�f�[�^��n���Ă���");

	m_pHpFrame->Render(0, 0);
	m_pSpFrame->Render(0, 0);
	
	// HP�Q�[�W
	m_pHpGage1P->Render();
	m_pHpGage2P->Render();

	// SP�Q�[�W
	m_pSpGage1P->Render();
	m_pSpGage2P->Render();

	// �y���\�i
	m_pPersona1P->Render();
	m_pPersona2P->Render();

	tdnText::Draw(350,70,0xffff00ff,"%d", PlayerMgr->GetPointA());
	tdnText::Draw(900, 70, 0xffff00ff, "%d", PlayerMgr->GetPointB());
}

bool GameUIManager::HandleMessage(const Message& msg)
{
	// ���ꂳ������΂Ȃ�ł��ł���

	switch (msg.Msg)
	{
	case PERSONA_CARD_COUNT_UP:
	{
		SIDE *data = (SIDE*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		SIDE side = *data;

		if (side == SIDE::LEFT)
		{
			m_pPersona1P->UpEffectAction();
		}
		else
		{
			m_pPersona2P->UpEffectAction();
		}

	}
		break;
	case PERSONA_CARD_COUNT_DOWN:
	{
		SIDE *data = (SIDE*)msg.ExtraInfo;		// �I���W�i�����\���̎󂯎��
		SIDE side = *data;

		if (side == SIDE::LEFT)
		{
			m_pPersona1P->DownEffectAction();
		}
		else
		{
			m_pPersona2P->DownEffectAction();
		}

	}
		break;
	case OTHER:
		break;
	default:
		break;
	}

	return false;
}
