
#include "motion_blend_airou.h"
#include "IEX_3DObject2.h"

TEST_airou::TEST_airou() : Character()
{
	angle = PI*0.5f;
}

TEST_airou::~TEST_airou()
{
	delete obj;
}



void TEST_airou::Move()
{
	float axisx = KEY_GetAxisX() * 0.001f;

	if (-0.250 < axisx && axisx < 0.250)
		axisx = 0;

	move.x += axisx;

	if (move.x != 0)
		obj->SetMotion(0, 1);
	else
		obj->SetMotion(0, 0);
}

void TEST_airou::Attack()
{
	if (KEY_Get(KEY_B))
		obj->SetMotion(1, 10);
}


void TEST_airou::Update()
{
	Move();
	Attack();
	pos += move;
	move = Vector3(0, 0, 0);
	obj->Animation();
}



void TEST_airou::Render()
{
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->Render();
}
