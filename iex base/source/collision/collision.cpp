
#include "collision.h"
#include "../../IEX/iextreme.h"
#include "../../IEX/IEX_3DObject2.h"
#include "../character/character.h"
#include "../character/airou/airou.h"
#include "../character/gimmick/Move_block.h"

void Collision::Check(iexMesh2 *obj, Character *chara)
{
	Vector3 out, pos(chara->Get_pos() + Vector3(0, 0.1f, 0)), ray(0, -1, 0);
	float dist(100);

	if (obj->RayPick(&out, &pos, &ray, &dist) != -1)
	{
		chara->Set_pos(out);
	}
}

void Collision::Check(iexMesh2 *obj, TEST_airou *chara)
{
	Vector3 updown(chara->Get_pos()), side(updown), pos(updown), ray(0, 0, 0);
	float dist(0);

	Vector3 move(chara->Get_move());
	pos.y += chara->Get_size_down();

	if (move.y != 0)
	{
		ray = Vector3(0, 0, 0);

		if (move.y < 0)
			ray.y = -1;
		else if (move.y > 0)
			ray.y = 1;

		dist = 100;
		obj->RayPick(&updown, &pos, &ray, &dist);
	}

	if (move.x != 0)
	{
		ray = Vector3(0, 0, 0);

		if (move.x < 0)
			ray.x = -1;
		else if (move.x > 0)
			ray.x = 1;

		dist = 100;
		obj->RayPick(&side, &pos, &ray, &dist);
	}

	chara->Col_vs_stage(updown, side);
}

void Collision::Check(Move_block *block, TEST_airou *chara)
{
	Vector3 cpos(chara->Get_pos()), updown(cpos), side(cpos), raypos(cpos), ray(0, 0, 0);
	float dist(0);

	Vector3 move(chara->Get_move());
	raypos.y += chara->Get_size_down();

	if (move.y != 0)
	{
		ray = Vector3(0, 0, 0);

		if (move.y < 0)
			ray.y = -1;
		else if (move.y > 0)
			ray.y = 1;

		dist = 100;
		if (block->RayPick(&updown, &raypos, &ray, &dist) == -1)
			updown = cpos;
		else
			updown.y += 0.01f;
	}

	if (move.x != 0)
	{
		ray = Vector3(0, 0, 0);

		if (move.x < 0)
			ray.x = -1;
		else if (move.x > 0)
			ray.x = 1;

		dist = 100;
		if (block->RayPick(&side, &raypos, &ray, &dist) == -1)
			side = cpos;
	}

	chara->Col_vs_gimmick(block->Get_move(), updown, side);
}
