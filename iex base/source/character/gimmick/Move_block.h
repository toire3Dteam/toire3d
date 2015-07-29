
#ifndef MOVE_BLOCK_H
#define MOVE_BLOCK_H

#include "../../../IEX/iextreme.h"

class Move_block
{
private:
	iexMesh *mesh = nullptr;

	Vector3 pos, move;
	int timer = 0;

public:
	Move_block();
	~Move_block();

	void Update();
	void Update_pos();

	void Render();

	void Col_vs_stage(const Vector3 &updown, const Vector3 &side);
};

#endif // !MOVE_BLOCK_H
