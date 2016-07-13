#include "EntityManager.h"






EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}

BaseGameEntity* EntityManager::GetEntityFromID(ENTITY_ID id)const
{
	// エンティティを見つけます
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	// エンティティがマップのメンバーかどうか
	assert((ent != m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

	// BaseEntityのポインタを返す
	return ent->second;
}



// 要素登録
bool EntityManager::Register(BaseGameEntity* NewEntity)
{
	// この関数はBaseGameEntityのコンストラクタで呼ばれます。
	// コンストラクタでＩＤがすでに降られています

	// まずIDがMapにあるか調べるぜ！
	ENTITY_ID id = NewEntity->GetID();
	// エンティティを見つけます
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	// エンティティがマップのメンバーにもう存在している
	MyAssert((ent == m_EntityMap.end()) , "もうそのID誰かが登録しとるやんけ！");


	// 要素を追加する
	m_EntityMap.insert(std::make_pair(NewEntity->GetID(), NewEntity));
	MyDebugString("ID番号　%d が登録されました\n", id);

	return true;
}

// 削除
bool EntityManager::Erace(BaseGameEntity* pEntity)
{
	// この関数はBaseGameEntityのデストラクタで呼ばれます。
	// コンストラクタでＩＤがすでに降られています

	// まずIDがMapにあるか調べるぜ！
	ENTITY_ID id = pEntity->GetID();
	// エンティティを見つけます
	EntityMap::const_iterator ent = m_EntityMap.find(id);
	// エンティティがマップのメンバーにもう存在している
	MyAssert((ent != m_EntityMap.end()), "もうそのIDの場所に要素なかったで！");


	// 要素を消す
	m_EntityMap.erase(m_EntityMap.find(pEntity->GetID()));
	MyDebugString("ID番号　%d が削除されました\n", id);

	return true;
}