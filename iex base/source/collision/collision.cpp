
#include "Collision.h"
#include "../player/BasePlayer.h"

void Collision::Raypic(iexMesh2 *obj, BasePlayer *player) // ステージとプレイヤー
{
	Vector3 pos(player->Get_pos()), move(player->Get_move());
	const Collision_shape::Square *square(player->Get_hit_square());

	Vector3 hit_pos, ray_pos, ray_vec; float dist;

	if (move.y > 0) // 上
	{
		ray_pos = pos;
		ray_pos += square->pos;
		ray_vec.Set(0, 1, 0);
		dist = square->height + move.y + 1;

		if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			dist = abs(hit_pos.y - ray_pos.y);
			if (dist <= square->height + move.y)
			{
				pos.y = hit_pos.y - square->height - square->pos.y;
				move.y = 0;
			}
		}
	}
	else if (move.y <= 0) // 下
	{
		ray_pos = pos;
		ray_pos += square->pos;
		ray_vec.Set(0, -1, 0);
		dist = square->height - move.y + 1;

		if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			dist = abs(hit_pos.y - ray_pos.y);
			if (dist <= square->height - move.y)
			{
				pos.y = hit_pos.y + square->height - square->pos.y;
				move.y = 0;
				player->Set_isLand_true();
			}
			else
				player->Set_isLand_false();
		}
		else
			player->Set_isLand_false();

	}

	if (move.x > 0) // 右
	{
		ray_pos = pos;
		ray_pos += square->pos;
		ray_vec.Set(1, 0, 0);
		dist = square->width + move.x + 1;

		if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			dist = abs(hit_pos.x - ray_pos.x);
			if (dist <= square->width + move.x)
			{
				pos.x = hit_pos.x - square->width - square->pos.x;
				move.x = 0;
			}
		}
	}
	else if (move.x < 0) // 左
	{
		ray_pos = pos;
		ray_pos += square->pos;
		ray_vec.Set(-1, 0, 0);
		dist = square->width - move.x + 1;

		if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			dist = abs(hit_pos.x - ray_pos.x);
			if (dist <= square->width - move.x)
			{
				pos.x = hit_pos.x + square->width - square->pos.x;
				move.x = 0;
			}
		}
	}

	player->Set_move(move);
	player->Set_pos(pos);
}
