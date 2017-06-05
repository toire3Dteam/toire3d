#include "TDNLIB.h"
#include "../BaseEntity/Message/Message.h"
#include "ShotManager.h"
#include "BaseShot.h"
#include "../Player/PlayerManager.h"

// ショットマネージャーの実態
ShotManager *ShotManager::m_pInstance = nullptr;

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
		if ((*it)->EraseOK())
		{ 
			delete (*it);
			it = m_list[i].erase(it); 
		}
		else it++;
	}
}

// 消すかどうかチェックするだけの更新
void ShotManager::EraseCheakUpdate()
{
	FOR((int)SIDE::ARRAY_MAX)for (auto it = m_list[i].begin(); it != m_list[i].end();)
	{
		if ((*it)->EraseOK())
		{
			delete (*it);
			it = m_list[i].erase(it);
		}
		else it++;
	}
}

// 片方全消去
void ShotManager::AllEraseSide(SIDE eSide)
{	
	for (auto it = m_list[(int)eSide].begin(); it != m_list[(int)eSide].end();)
	{

			delete (*it);
			it = m_list[(int)eSide].erase(it);
	
	}
}

void ShotManager::Render()
{
	//FOR((int)SIDE::ARRAY_MAX) for (auto it : m_list[i]) it->Render();

	FOR((int)SIDE::ARRAY_MAX)for (auto it = m_list[i].begin(); it != m_list[i].end();)
	{
		(*it)->Render();
		it++;
	}
}

bool ShotManager::HandleMessage(const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::ADD_SHOT:
	{
		Shot::Base *pNewShot = (Shot::Base*)msg.ExtraInfo;
		pNewShot->Update();//
		AddShot(PlayerMgr->GetPlayer(msg.Sender)->GetSide(), pNewShot);
		return true;
		break;
	}
	case MESSAGE_TYPE::ERASE_SIDE_SHOT:
	{
		SIDE *eSide = (SIDE*)msg.ExtraInfo;
		AllEraseSide(*eSide);

		return true;
		break;
	}

	}

	return false;
}

void ShotManager::Reset()
{
	FOR((int)SIDE::ARRAY_MAX) for (auto it = m_list[i].begin(); it != m_list[i].end();)
	{
		delete (*it);
		it = m_list[i].erase(it);
	}
}