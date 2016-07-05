#pragma once
#include "tdnlib.h"
#include "AnimationRipple.h"


class AnimationRipple
{
public:
	AnimationRipple(char* fileName, int MoveAlpha, float MoveScale);
	~AnimationRipple();


	void Update();
	void Render(int x,int y,DWORD dwFlag=RS::COPY);
	void Render(int x, int y, int w, int h, int tx, int ty, int tw, int th, DWORD dwFlag = RS::COPY);

	void Action();

private:
	tdn2DObj* image;

	int alpha;
	float scale;
	int moveAlpha;
	float moveScale;
	bool isAction;

};
