
#include "Move_block.h"

Move_block::Move_block() : pos(10, 5, 0), move(0, 0.1f, 0)
{
	mesh = new iexMesh("./DATA/BOX/24bbbb.IMO");

	mesh->SetAngle(PI);
}
Move_block::~Move_block()
{

}


void Move_block::Update()
{
	timer++;

	if (timer == 180)
		move.y = -0.1f;
	else if (timer == 360)
	{
		move.y = 0.1f;
		timer = 0;
	}
}

void Move_block::Update_pos()
{
	pos += move;
}


void Move_block::Render()
{
	mesh->SetPos(pos);

	mesh->Update();
	mesh->Render();
}


void Move_block::Col_vs_stage(const Vector3 &updown, const Vector3 &side)
{

}
