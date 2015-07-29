
#ifndef MOTION_BLEND_AIROU_H
#define MOTION_BLEND_AIROU_H

class iex3DObj2;
struct Vector3;

#include "character.h"

class TEST_airou : public Character
{
private:
	void Move();
	void Attack();

public:
	TEST_airou();
	~TEST_airou();

	void Update();
	void Render();

	enum
	{
		WAIT = 0, RUN = 1, ATTACK = 10
	};
};

#endif // !MOTION_BLEND_AIROU_H
