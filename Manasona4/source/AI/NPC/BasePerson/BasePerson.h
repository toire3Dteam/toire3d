#pragma once
#include "TDNLIB.h"
#include "AI\State\StateMachine.h"
#include "AI\Entity\EntityManager.h"
#include "AI\Message\MessageDispatcher.h"


// ★「テンプレート<クラスENTITY_TYPE>」で事前に宣言します
//template <class entity_type> class State;
template <class entity_type> class StateMachine;

/*
	人の種類
*/
enum class PERSON_TYPE
{
	FRIEND_KEEPER,
	FRIEND_FIELD,

	ENEMY_KEEPER,
	ENEMY_FIELD
};


/*
	基本の人の形
*/
class BasePerson :public BaseGameEntity
{
public:
	BasePerson(ENTITY_ID id);
	virtual ~BasePerson();

	// BaseGameEntityの純粋仮想関数により必須
	virtual void  Update();
	
	// 
	virtual void  Render();
	virtual void  RangeRender();
	virtual void  UIRender();

	// メッセージを取り扱う
	virtual bool  HandleMessage(const Message& msg) = 0;


	// アクセサ
	// ★この形にするからステートマシンのアクセサを作る
	//StateMachine<BasePerson>* GetFSM()const { return m_pStateMachine; }

	Vector3 GetPos() { return m_pos; }
	void SetPos(Vector3 pos) { m_pos = pos; }

	Vector3 GetMove() { return m_move; }
	void SetMove(Vector3 move) { m_move = move; }

	float GetAngle() { return m_angle; }
	void SetAngle(float angle) { m_angle = angle; }

	PERSON_TYPE GetPersonType() { return m_PersonType;	}
	
	iex3DObj* GetObj() { return m_obj; }

	bool IsRand() { return m_bRand; }
	void Jump() { m_bRand = false; }

	bool IsCollWall() { return m_bCollWall; }

	// ★ベースにステートマシンがなく触れないが
	// virtureを利用して各々のステートマシンに呼びかける事にトライ！
	//virtual void ResetState() = 0; // 初期のステートに戻す


	int m_LifePoint;
protected:

	PERSON_TYPE m_PersonType;

	iex3DObj* m_obj;
	Vector3 m_pos;
	Vector3 m_move;

	float m_angle;

	// 着地
	bool m_bRand;// 着地フラグ
	bool m_bCollWall;// 壁ぶつかり

	// ★ステートマシン
	// StateMachine<BasePerson>*  m_pStateMachine;


};
