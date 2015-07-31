#include	"iextreme.h"

#include	"Fade.h"

void Fade::Initialize()
{
	mode = FADE_STOP;
	speed = 1;
	alpha = 255;
	color = 0x00000000;
}

Fade::Fade()
{
	Initialize();
}

void Fade::Set(int Mode, int Speed, BYTE Limit, BYTE Alpha, DWORD Color)	// alphaは0～255の範囲
{
	mode = Mode;		// フェードモード(FADE_IN or FADE_OUT)
	speed = Speed;		// フェードを増減させるスピード(5なら、毎フレーム「5」が増減される)
	limit = Limit;		// 目標alpha値(FADE_INなら「0」、FADE_OUTなら「255」が基本)
	alpha = Alpha;		// フェード開始時のalpha値(FADE_INなら「255」、DADE_OUTなら「0」が基本)
	color = Color;		// フェード色

	// 0x○○ffffffの○の部分を0にする処理(alpha値の調整)
	color = color << 2;
	color = color >> 2;
}

void Fade::Set(int Mode, int Speed, BYTE Limit)	// alphaは0～255の範囲
{
	mode = Mode;
	speed = Speed;
	limit = Limit;
}

void Fade::Set(int Mode, int Speed)	// alphaは0～255の範囲
{
	mode = Mode;
	speed = Speed;

	switch (mode)
	{
	case FADE_IN:
		limit = 0;
		break;

	case FADE_OUT:
		limit = 255;
		break;

	case FADE_STOP:
		break;
	}
}


// フェード処理
void Fade::Update()
{
	switch (mode)
	{
	case FADE_IN:

		// 目的αまでついたら、ストップ
		if (alpha <= limit + speed){		// unsigned charなのでオーバーフロー防止
			alpha = limit;
			mode = FADE_STOP;
		}
		else
			alpha -= speed;

		break;

	case FADE_OUT:

		// 目的αまでついたら、ストップ
		if (alpha >= limit - speed){		// unsigned charなのでオーバーフロー防止
			alpha = limit;
			mode = FADE_STOP;
		}
		else
			alpha += speed;

		break;

	case FADE_STOP:
		break;	// NothingDo
	}
}

void Fade::Render()
{
	DWORD col;
	col = (alpha << 24) | color;	// argbのフォーマットにする

	iexPolygon::Rect(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, col);
}