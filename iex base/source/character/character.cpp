
#include "character.h"
#include "../../IEX/IEX_3DObject2.h"

Character::Character() : angle(0), pos(0, 0, 0), move(0, 0, 0), accelerator(0, 0, 0)
{

}

Character::~Character()
{

}


void Character::Update()
{
	move += accelerator;
	accelerator = Vector3(0, 0, 0);
}
void Character::Update_pos()
{
	pos += move;
}


void Character::Render()
{
	obj->Render();
}


void Character::Set_object(iex3DObj2 *in){ assert(in != nullptr); delete obj; obj = in; };


void Character::Set_pos(const Vector3 &in){ pos = in; }
const Vector3 &Character::Get_pos(){ return pos; }

const Vector3 &Character::Get_move(){ return move; }

float Character::Get_size_side()
{
	return size_side;
}
float Character::Get_size_up()
{
	return size_up;
}
float Character::Get_size_down()
{
	return size_down;
}
