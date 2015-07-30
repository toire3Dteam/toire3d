
#include "airou.h"
#include "IEX_3DObject2.h"

TEST_airou::TEST_airou() : Character()
{
	angle = PI*0.5f;

	speed = 1.0f;

	Move_fp = &TEST_airou::Move_run;

	size_side = 1.0f;
	size_down = 0.25f;
	size_up = 1.0f;
}

TEST_airou::~TEST_airou()
{
	delete obj;
}


void TEST_airou::Move_run()
{
	if (KEY_Get(KEY_UP))
	{
		accelerator.y += 10;
		Move_fp = &TEST_airou::Move_jump;
		Move_jump();
		return;
	}

	if (move.x != 0)
		obj->SetMotion(LOWER, RUN);
	else
		obj->SetMotion(LOWER, WAIT);
}

void TEST_airou::Move_jump()
{
	obj->SetMotion(LOWER, JUMP);
}

void TEST_airou::Move()
{
	move.x = 0;
	if (KEY_Get(KEY_RIGHT))
		move.x += speed;
	if (KEY_Get(KEY_LEFT))
		move.x -= speed;

	(this->*Move_fp)();

	accelerator.y -= 0.9f;
	move += accelerator;
	accelerator = Vector3(0, 0, 0);
}

void TEST_airou::Attack()
{
	if (KEY_Get(KEY_B))
		obj->SetMotion(UPPER, CLOSE_RANGE_ATTACK);
	if (KEY_Get(KEY_A))
		obj->SetMotion(UPPER, SHOT);
}


void TEST_airou::Update()
{
	Move();
	Attack();
	obj->Animation();
}



void TEST_airou::Render()
{
	obj->SetAngle(angle);
	obj->SetPos(pos);
	obj->Render();
}



void TEST_airou::Col_vs_stage(const Vector3 &updown, const Vector3 &side)
{
	if (pos.y != updown.y)
	{
		if (move.y < 0) // 下
		{
			if (updown.y >= pos.y + move.y)
			{
				move.y = 0;
				pos.y = updown.y + size_down;
				Move_fp = &TEST_airou::Move_run;
			}
			else
				Move_fp = &TEST_airou::Move_jump;
		}
		else if (move.y > 0) // 上
		{
			if (updown.y <= pos.y + move.y)
			{
				move.y = 0;
				pos.y = updown.y - size_up;
			}
		}
	}

	if (pos.x != side.x)
	{
		if (move.x < 0) // 左
		{
			if (side.x >= pos.x + move.x)
			{
				move.x = 0;
				pos.x = side.x + size_side;
			}
		}
		else if (move.x > 0) // 右
		{
			if (side.x <= pos.x + move.x)
			{
				move.x = 0;
				pos.x = side.x - size_side;
			}
		}
	}
}

void TEST_airou::Col_vs_gimmick(const Vector3 &block_move, const Vector3 &updown, const Vector3 &side)
{
	if (pos.x != side.x)
	{
		if (move.x < 0) // 左
		{
			if (side.x >= pos.x + move.x)
			{
				move.x = 0;
				pos.x = side.x + size_side;
			}
		}
		else if (move.x > 0) // 右
		{
			if (side.x <= pos.x + move.x)
			{
				move.x = 0;
				pos.x = side.x - size_side;
			}
		}
	}

	if (pos.y != updown.y)
	{
		if (move.y < 0) // 下
		{
			if (updown.y >= pos.y + move.y) // 立ってる
			{
				move.y = 0;
				pos.y = updown.y + size_down;

				move.x += block_move.x;
				Move_fp = &TEST_airou::Move_run;
			}
			else
				Move_fp = &TEST_airou::Move_jump;
		}
		else if (move.y > 0) // 上
		{
			if (updown.y <= pos.y + move.y)
			{
				move.y = 0;
				pos.y = updown.y - size_up;
			}
		}
	}

}
