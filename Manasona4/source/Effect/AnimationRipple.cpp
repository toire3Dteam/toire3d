#include "AnimationRipple.h"

/*�g��A�j��*/

AnimationRipple::AnimationRipple(char* fileName, int MoveAlpha, float MoveScale)
{
	image = new tdn2DObj(fileName);
	//image->SetShiftCenter(true);

	alpha = 255;
	scale = 1.0f;
	isAction = false;

	moveAlpha = MoveAlpha;
	moveScale = MoveScale;
}

AnimationRipple::~AnimationRipple()
{
	delete image;
}

void AnimationRipple::Update()
{
	if (isAction == false)return;

	// 
	if (isAction == true)
	{
		alpha -= moveAlpha;
		scale += moveScale;

		if (alpha <= 0)
		{
			isAction = false;
		}
	}

	// �X�V
	image->SetARGB(alpha, 255, 255, 255);
	image->SetScale(scale);

}
void AnimationRipple::Render(int x, int y, DWORD dwFlag)
{
	if (isAction == false)return;
	image->Render(x, y, dwFlag);
}
void AnimationRipple::Render(int x, int y, int w, int h, int tx, int ty, int tw, int th, DWORD dwFlag)
{
	if (isAction == false)return;
	image->Render(x, y, w, h, tx, ty, tw, th, dwFlag);
}

void AnimationRipple::Action()
{
	isAction = true;
	// ������
	alpha = 255;
	scale = 1.0f;

}