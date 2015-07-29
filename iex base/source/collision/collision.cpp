
#include "collision.h"
#include "../../IEX/iextreme.h"
#include "../../IEX/IEX_3DObject2.h"
#include "../test/character.h"
#include "../test/motion_blend_airou.h"

void Collision::Check(iexMesh *obj, Character *chara)
{
	Vector3 out, pos(chara->Get_pos() + Vector3(0, 0.1f, 0)), ray(0, -1, 0);
	float dist(100);

	if (obj->RayPick(&out, &pos, &ray, &dist) != -1)
	{
		chara->Set_pos(out);
	}
}

void Collision::Check(iexMesh *obj, TEST_airou *chara)
{
	Vector3 updown(chara->Get_pos()), side(updown), pos(side + Vector3(0, 0.5f, 0)), ray(0, 0, 0);
	float dist(0);

	Vector3 move(chara->Get_move());

	if (move.y != 0)
	{
		ray.x = 0;

		if (move.y < 0)
			ray.y = -1;
		else if (move.y > 0)
			ray.y = 1;

		dist = 100;
		obj->RayPick(&updown, &pos, &ray, &dist);
	}

	if (move.x != 0)
	{
		ray.y = 0;

		if (move.x < 0)
			ray.x = -1;
		else if (move.x > 0)
			ray.x = 1;

		dist = 100;
		obj->RayPick(&side, &pos, &ray, &dist);
	}

	chara->Col_vs_stage(updown, side);
}
