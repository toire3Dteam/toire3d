#include "TDNLIB.h"
#include "../BaseEntity/Message/Message.h"
#include "ShotManager.h"
#include "BaseShot.h"
#include "../Player/PlayerManager.h"

ShotManager::ShotManager() :BaseGameEntity(ENTITY_ID::SHOT_MGR)
{

}

ShotManager::~ShotManager()
{
	FOR((int)SIDE::ARRAY_MAX)for (auto it = m_list[i].begin(); it != m_list[i].end();){ delete (*it); it = m_list[i].erase(it); }
}

void ShotManager::Update()
{
	FOR((int)SIDE::ARRAY_MAX)for (auto it = m_list[i].begin(); it != m_list[i].end();)
	{
		(*it)->Update();
		if ((*it)->EraseOK()){ delete (*it); it = m_list[i].erase(it); }
		else it++;
	}
}

void ShotManager::Render()
{
	FOR((int)SIDE::ARRAY_MAX) for (auto it : m_list[i]) it->Render();
}

bool ShotManager::HandleMessage(const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::ADD_SHOT:
		Shot::Base *pNewShot = (Shot::Base*)msg.ExtraInfo;
		pNewShot->Update();//
		AddShot(PlayerMgr->GetPlayer(msg.Sender)->GetSide(), pNewShot);
		return true;
		break;
	}

	return false;
}