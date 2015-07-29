
#ifndef MOTION_BLEND_AIROU_H
#define MOTION_BLEND_AIROU_H

class iex3DObj2;
struct Vector3;

#include "character.h"

class TEST_airou : public Character
{
private:
	void Move_jump();
	void Move_run();
	void(TEST_airou::*Move_fp)() = nullptr;

	void Move();
	void Attack();

public:
	TEST_airou();
	~TEST_airou();

	void Update();
	void Render();

	enum
	{
		WAIT = 0, RUN = 1, JUMP = 4, CLOSE_RANGE_ATTACK = 10, SHOT = 11
	};

	void Col_vs_stage(const Vector3 &updown, const Vector3 &side) override;
};

#endif // !MOTION_BLEND_AIROU_H
