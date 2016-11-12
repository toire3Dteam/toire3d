#pragma once
#include <map>
#include <cassert>
#include <string>
#include "BaseGameEntity.h"

/*
	エージェントのポインタのマネージャー

*/

class BaseGameEntity;

class EntityManager
{
public:
	static EntityManager* Instance();
	~EntityManager() {};

	//この関数はエンティティのIDで示されたインデックスの位置にある
	// エンティティ登録！
	bool            Register(BaseGameEntity* NewEntity);

	//　パラメータとして与えられたIDを持つエンティティのポインタを返す
	BaseGameEntity* GetEntityFromID(ENTITY_ID id)const;

	// このメソッドはリストからエンティティを削除する
	bool            Erace(BaseGameEntity* pEntity);

private:

	// 入力を楽にするために
	typedef std::map<ENTITY_ID, BaseGameEntity*> EntityMap;

	// エンティティがすぐに調べられる用に
	// このなかではエンティティへのポインタはそのIDで相互参照される
	EntityMap m_EntityMap;

	// 封印
	EntityManager() {};
	
	// ctorをコピーし割り当てはPrivateに
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);
};

// インスタンス化
#define EntityMgr EntityManager::Instance()
