
#include "Move_block.h"

Move_block::Move_block() : pos(-5, 15, 0), move(0, 0, 0), angle(0, PI, 0)
{
	size = 2;

	mesh = new iexMesh("./DATA/BOX/24bbbb.IMO");

	move_mode[0] = Vector3(0, 0.1f, 0);
	move_mode[1] = Vector3(-0.1f, 0, 0);
	move_mode[2] = Vector3(0, -0.1f, 0);
	move_mode[3] = Vector3(0.1f, 0, 0);
}
Move_block::~Move_block()
{

}


void Move_block::Update()
{
	timer--;

	if (timer <= 0)
	{
		move = move_mode[mm_num];

		mm_num++;
		if (mm_num >= 4)
			mm_num = 0;

		timer = 180;
	}
}

void Move_block::Update_pos()
{
	pos += move;
	angle.z += 0.01f;

	if (timer > 90)
		size += 0.01f;
	else
		size -= 0.01f;
}


void Move_block::Render()
{
	mesh->SetPos(pos);
	mesh->SetAngle(angle);
	mesh->SetScale(size);

	mesh->Update();
	mesh->Render();
}



int Move_block::RayPick(Vector3* out, const Vector3* pos, Vector3* vec, float *Dist)
{
	return mesh->RayPick2(out, pos, vec, Dist);
}



const Vector3 &Move_block::Get_pos()
{
	return pos;
}

const Vector3 &Move_block::Get_move()
{
	return move;
}
