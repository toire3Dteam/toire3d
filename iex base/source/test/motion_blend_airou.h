
#ifndef MOTION_BLEND_AIROU_H
#define MOTION_BLEND_AIROU_H

class iex3DObj2;

class TEST_airou
{
public:
	iex3DObj2* obj;

	float angle;

	TEST_airou();
	~TEST_airou();
	void Update();
	void Render();

	enum
	{
		WAIT = 0, RUN = 1, ATTACK = 10
	};
};

extern TEST_airou* test_airou;

#endif // !MOTION_BLEND_AIROU_H
