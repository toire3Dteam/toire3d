#include "BaseGameEntity.h"
#include <cassert>
#include "EntityManager.h"

// 宣言
// int BaseGameEntity::m_isNextValidID = 0;

//void BaseGameEntity::SetID(int val)
//{
//	// ゲームIDはとても重要、したがってIDが被っていないか
//	// この関数でテストする
//	//assert((val >= m_isNextValidID) && "SetID");
//		
//	m_ID = val;
//	m_isNextValidID = m_ID + 1;	// インクリメント
//
//}

/*
	自分を消せば
	マネージャーの自分のデータも消してくれる
	一石二鳥モード
*/

// ↑のＩＤが被っていないかの処理は　EntityMGR　で行うことにしました。
BaseGameEntity::BaseGameEntity(ENTITY_ID id) :m_ID(id)
{
	// マネージャー起動
	MyAssert(EntityMgr->Register(this), "ID登録に失敗しました \n ID番号= %d ", m_ID);
}

// 削除したとき今の自分の要素も　EntityMGR から消す
BaseGameEntity::~BaseGameEntity()
{
	// マネージャー起動
	MyAssert(EntityMgr->Erace(this), "ID登録に失敗しました \n ID番号= %d ", m_ID);

}