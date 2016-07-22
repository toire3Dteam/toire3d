#include "TDNLIB.h"
#include "Collision.h"
#include "../Player/BasePlayer.h"
#include "../BaseEntity/Message/MessageDispatcher.h"

void Collision::Raypic(iexMesh *obj, BasePlayer *player) // ステージとプレイヤー
{
	Vector3 move(player->GetMove());

	if (move.y > 0) // 上
	{
		RaypicUp(obj, player);
	}
	else if (move.y <= 0) // 下
	{
		RaypicDown(obj, player);
	}

	if (move.x > 0) // 右
	{
		RaypicRight(obj, player);
	}
	else if (move.x < 0) // 左
	{
		RaypicLeft(obj, player);
	}
}

void Collision::RaypicUp(iexMesh *obj, BasePlayer *player)
{
	Vector3 pos(player->GetPos()), move(player->GetMove());
	const CollisionShape::Square *square(player->GetHitSquare());

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
				move.y = -0.1e-10f;

				player->SetMove(move);
				player->SetPos(pos);
			}
		}
	}
}
void Collision::RaypicDown(iexMesh *obj, BasePlayer *player)
{
	Vector3 move(player->GetMove());

	if (move.y <= 0) // 下
	{
		bool hit(false);

		Vector3 pos(player->GetPos());
		const CollisionShape::Square *square(player->GetHitSquare());
		Vector3 hit_pos, ray_pos, ray_vec; float dist;

		ray_pos = pos;
		ray_pos += square->pos;
		ray_pos.x += square->width; // 右から
		ray_vec.Set(0, -1, 0);
		dist = square->height - move.y + 1;

		if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			dist = abs(hit_pos.y - ray_pos.y);
			if (dist <= square->height - move.y)
			{
				pos.y = hit_pos.y + square->height - square->pos.y;
				move.y = 0;

				player->SetMove(move);
				player->SetPos(pos);

				// ★★★　地面に付いたら
				player->SetLand(true);//プレイヤーのポジションをセットした後に変更
				player->SetAerialJump(true);// 空中ジャンプの権利も戻る
				hit = true;
			}
		}
		// 空中にいるよ！
		else
		{
			player->SetLand(false);//プレイヤーのポジションをセットした後に変更
			// 落下メッセージ送信
			MsgMgr->Dispatch(0, ENTITY_ID::PLAYER_MGR, (ENTITY_ID)(ENTITY_ID::ID_PLAYER_FIRST + player->GetDeviceID()), MESSAGE_TYPE::FALL, nullptr);
		}

		if (!hit)
		{
			ray_pos = pos;
			ray_pos += square->pos;
			ray_pos.x -= square->width; // 左から
			ray_vec.Set(0, -1, 0);
			dist = square->height - move.y + 1;

			if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
			{
				dist = abs(hit_pos.y - ray_pos.y);
				if (dist <= square->height - move.y)
				{
					pos.y = hit_pos.y + square->height - square->pos.y;
					move.y = 0;

					player->SetMove(move);
					player->SetPos(pos);

					//player->Land();//プレイヤーのポジションをセットした後に変更
				}
			}
		}
	}
}
void Collision::RaypicLeft(iexMesh *obj, BasePlayer *player)
{
	Vector3 pos(player->GetPos()), move(player->GetMove());
	const CollisionShape::Square *square(player->GetHitSquare());

	Vector3 hit_pos, ray_pos, ray_vec; float dist;

	if (move.x < 0) // 左
	{
		bool hit = false;

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

				player->SetMove(move);
				player->SetPos(pos);
				hit = true;
			}
		}
		
		if(!hit) // 当たらなかったから上下からray出す 
		{
			float h(0);
			if (move.y > 0) // 上
			{
				h = square->height * 0.85f;
			}
			else // 下
			{
				h = -square->height * 0.85f;
			}
			ray_pos = pos;
			ray_pos += square->pos;
			ray_pos.y += h;
			ray_vec.Set(-1, 0, 0);
			dist = square->width - move.x + 1;



			if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
			{
				dist = abs(hit_pos.x - ray_pos.x);
				if (dist <= square->width - move.x)
				{
					pos.x = hit_pos.x + square->width - square->pos.x;
					move.x = 0;

					player->SetMove(move);
					player->SetPos(pos);
				}
			}
		}
	}
}
void Collision::RaypicRight(iexMesh *obj, BasePlayer *player)
{
	Vector3 pos(player->GetPos()), move(player->GetMove());
	const CollisionShape::Square *square(player->GetHitSquare());

	Vector3 hit_pos, ray_pos, ray_vec; float dist;

	if (move.x > 0) // 右
	{
		bool hit = false;

		ray_pos = pos;
		ray_pos += square->pos;
		ray_pos.y += square->height * 0.1f;
		ray_vec.Set(1, 0, 0);
		dist = square->width + move.x + 1;

		if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
		{
			dist = abs(hit_pos.x - ray_pos.x);
			if (dist <= square->width + move.x)
			{
				pos.x = hit_pos.x - square->width - square->pos.x;
				move.x = 0;

				player->SetMove(move);
				player->SetPos(pos);
				hit = true;
			}
		}

		if (!hit) // 当たらなかったから上下からray出す 
		{
			float h(0);
			if (move.y > 0) // 上
			{
				h = square->height * 0.85f;
			}
			else // 下
			{
				h = -square->height * 0.85f;
			}
			ray_pos = pos;
			ray_pos += square->pos;
			ray_pos.y += h;
			ray_vec.Set(1, 0, 0);
			dist = square->width + move.x + 1;

			if (obj->RayPick2(&hit_pos, &ray_pos, &ray_vec, &dist) != -1) // 当たった
			{
				dist = abs(hit_pos.x - ray_pos.x);
				if (dist <= square->width + move.x)
				{
					pos.x = hit_pos.x - square->width - square->pos.x;
					move.x = 0;

					player->SetMove(move);
					player->SetPos(pos);
				}
			}
		}

	}
}



bool Collision::HitCheck(CollisionShape::Circle* c1, CollisionShape::Circle* c2)
{
	float dist(c1->radius + c2->radius);
	float lenx(c1->pos.x - c2->pos.x),
		leny(c1->pos.y - c2->pos.y);

	return ((dist * dist) > (lenx*lenx + leny*leny));
}

bool Collision::HitCheck(CollisionShape::Square* s1, CollisionShape::Square* s2)
{
	if (s1->height + s2->height < abs(s1->pos.y - s2->pos.y))
		return false;

	if ((s1->width + s2->width) < abs(s1->pos.x - s2->pos.x))
		return false;

	return true;
}

bool Collision::HitCheck(CollisionShape::Circle* c, CollisionShape::Square* s)
{
	// 円に一番近い頂点の位置
	float vx, vy;
	// x
	if (c->pos.x > s->pos.x)
		vx = s->pos.x + s->width;
	else
		vx = s->pos.x - s->width;
	// y
	if (c->pos.y > s->pos.y)
		vy = s->pos.y + s->height;
	else
		vy = s->pos.y - s->height;


	// 円から頂点までの距離
	float lx, ly;
	lx = s->pos.x - c->pos.x;
	ly = s->pos.y - c->pos.y;
	// 円の中に頂点がある
	if (c->radius*c->radius < lx*lx + ly*ly)
		return true;


	bool inx(s->pos.x - s->width <= c->pos.x && c->pos.x <= s->pos.x + s->width)
		, iny(s->pos.y - s->height <= c->pos.y && c->pos.y <= s->pos.y + s->height);
	// 長方形の中に円の中心がある
	if (inx && iny)
		return true;

	// 円が長方形と触れている
	if (inx)
	{
		if (c->radius + s->height > abs(c->pos.y - s->pos.y))
			return true;
	}
	else if (iny)
	{
		if (c->radius + s->width > abs(c->pos.x - s->pos.x))
			return true;
	}

	return false;
}
