
#ifndef MOVE_BLOCK_H
#define MOVE_BLOCK_H

#include "../../../IEX/iextreme.h"

class Move_block
{
private:
	iexMesh *mesh = nullptr;

	Vector3 pos, move, angle;
	int timer = 0;

	float size = 1;

	int mm_num = 0;
	Vector3 move_mode[4];
public:
	Move_block();
	~Move_block();

	void Update();
	void Update_pos();

	void Render();

	int RayPick(Vector3* out, const Vector3* pos, Vector3* vec, float *Dist);

	const Vector3 &Get_pos();
	const Vector3 &Get_move();
};

#endif // !MOVE_BLOCK_H
