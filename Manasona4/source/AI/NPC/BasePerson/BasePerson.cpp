#include "AI\NPC\BasePerson\BasePerson.h"
#include "system\System.h"
#include "Stage\Stage.h"


// 初期化
BasePerson::BasePerson(ENTITY_ID id) :BaseGameEntity(id)
{
	m_obj = NULL;

	m_pos = VECTOR_ZERO;
	m_move = VECTOR_ZERO;

	m_angle = PI;	// 0.0→PI

	m_bRand = true;
	m_bCollWall = false;

}


// 解放
BasePerson::~BasePerson()
{

	//SAFE_DELETE(m_pStateMachine);	
	SAFE_DELETE(m_obj);

}

void BasePerson::Update()
{
	// ★ステートマシン更新
	//m_pStateMachine->Update();

	// Move更新
	m_pos += m_move;

	// 摩擦
	if (m_move.z > 0.0f)
	{
		m_move.z -= 0.1f;
		if (m_move.z < 0.1f)m_move.z = 0.0f;
	}
	if (m_move.z < 0.0f)
	{
		m_move.z += 0.1f;
		if (m_move.z > 0.1f)m_move.z = 0.0f;
	}
	if (m_move.x > 0.0f)
	{
		m_move.x -= 0.1f;
		if (m_move.x < 0.1f)m_move.x = 0.0f;
	}
	if (m_move.x < 0.0f)
	{
		m_move.x += 0.1f;
		if (m_move.x > 0.1f)m_move.x = 0.0f;
	}

	// 重力
	m_move.y -= 0.05f;
	if (m_move.y <= -1.0f) { m_move.y = -1.0f; }


	// 地面
	//Vector3 UDpos = m_pos;
	//bool ok = STAGE.Collision(UDpos, m_pos, Vector3(0, -1, 0), 1);
	//if (ok == true)
	//{
	//	//m_obj->SetPos(UDpos);
	//	//m_move = Vector3(m_move.x, 0.0f, m_move.z);
	//}

	// 壁 
	Vector3 saveWallPos = m_pos;
	Vector3 cllPos;
	Vector3 myVec;
	myVec.x = m_obj->TransMatrix._31;
	myVec.y = m_obj->TransMatrix._32;
	myVec.z = m_obj->TransMatrix._33;
	myVec.Normalize();

	cllPos = STAGE.Collision(m_pos, myVec, 6.7);
	m_pos = cllPos;
	//cllPos = STAGE.Collision(m_pos, Vector3(-1, 0, 0), 2);
	//m_pos = cllPos;	
	if (saveWallPos != m_pos)
	{
		m_bCollWall= true;
	}
	else
	{
		m_bCollWall = false;
	}

	Vector3 savePos = m_pos;
	cllPos = STAGE.Collision(m_pos, Vector3(0, -1, 0), 2);
	m_pos = cllPos;
	if (savePos != m_pos)
	{
		m_bRand = true;
	}
	else
	{
		m_bRand = false;
	}

	// 場所更新
	m_obj->SetPos(m_pos);
	m_obj->SetAngle(m_angle);
	m_obj->Update();
	m_obj->Animation();

	if (m_pos.y <= -30)
	{
		m_pos = VECTOR_ZERO;
		m_pos.y = 50;
		m_move = VECTOR_ZERO;
	}



}

void BasePerson::Render()
{

}

void BasePerson::RangeRender()
{

}

void BasePerson::UIRender()
{

}




// ステートマシンのメッセージ
//bool BasePerson::HandleMessage(const Message & msg)
//{
//	return m_pStateMachine->HandleMessage(msg);
//}
