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
	m_teamPic = new tdn2DObj("Data/UI/Game/GameUI2.png");

	//m_combo
}

GameUIManager::~GameUIManager()
{
	SAFE_DELETE(m_teamPic);
}

void GameUIManager::Update()
{

	
}

void GameUIManager::Render()
{
	m_teamPic->Render(0, 0);
	tdnText::Draw(350,70,0xffff00ff,"%d", PlayerMgr->GetPointA());
	tdnText::Draw(900, 70, 0xffff00ff, "%d", PlayerMgr->GetPointB());
}

bool GameUIManager::HandleMessage(const Message& msg)
{

	return false;
}
