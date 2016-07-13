#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
噂の波紋マネージャー.h

*/

class BasePerson;

class PersonManager:public BaseGameEntity
{
public:
	static PersonManager& GetInstance();
	static void Release();
	virtual ~PersonManager();

private:
	static PersonManager* pInstance;

	// Entityのカウント
	static int m_IDcount;

	// 封印
	PersonManager();
	PersonManager(const PersonManager&);
	PersonManager& operator=(const PersonManager&);

	// 人データ
	std::list<BasePerson*> m_PersonData;
	std::list<BasePerson*>::iterator m_it;


public:
	// 大量生産
	/*
		中で勝手にＩＤを振る
	*/
	ENTITY_ID AddPerson( Vector3 pos, bool isAi = false);

	//	リセット
	void Reset();

	// 更新・描画
	void Update();
	void Render();

	// メッセージ更新
	bool  HandleMessage(const Message& msg);

	// アクセサ
	BasePerson* GetPerson(int no);
	int GetPersonSize() { return (int)m_PersonData.size(); }

	// 近いキャラクターを探す
	Vector3 NearPlayerPosSearch(PLAYER_INFO* info );
	// 近いキャラクターを探す
	BasePerson* NearPlayerSearch(PLAYER_INFO* info);

	// 近いキャラクターを探す
	ENTITY_ID SoccerNearPlayerSearch(PLAYER_INFO* info);


};


// インスタンス化
#define PersonMgr	(PersonManager::GetInstance())


