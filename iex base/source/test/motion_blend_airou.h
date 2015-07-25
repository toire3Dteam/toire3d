
#ifndef MOTION_BLEND_AIROU_H
#define MOTION_BLEND_AIROU_H

class iex3DObj2;

#include "character.h"

class TEST_airou : public Character
{
public:
	float angle = 0;

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
