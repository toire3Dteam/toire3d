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

void Fade::Set(int Mode, int Speed, BYTE Limit, BYTE Alpha, DWORD Color)	// alpha��0�`255�͈̔�
{
	mode = Mode;		// �t�F�[�h���[�h(FADE_IN or FADE_OUT)
	speed = Speed;		// �t�F�[�h�𑝌�������X�s�[�h(5�Ȃ�A���t���[���u5�v�����������)
	limit = Limit;		// �ڕWalpha�l(FADE_IN�Ȃ�u0�v�AFADE_OUT�Ȃ�u255�v����{)
	alpha = Alpha;		// �t�F�[�h�J�n����alpha�l(FADE_IN�Ȃ�u255�v�ADADE_OUT�Ȃ�u0�v����{)
	color = Color;		// �t�F�[�h�F

	// 0x����ffffff�́��̕�����0�ɂ��鏈��(alpha�l�̒���)
	color = color << 2;
	color = color >> 2;
}

void Fade::Set(int Mode, int Speed, BYTE Limit)	// alpha��0�`255�͈̔�
{
	mode = Mode;
	speed = Speed;
	limit = Limit;
}

void Fade::Set(int Mode, int Speed)	// alpha��0�`255�͈̔�
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


// �t�F�[�h����
void Fade::Update()
{
	switch (mode)
	{
	case FADE_IN:

		// �ړI���܂ł�����A�X�g�b�v
		if (alpha <= limit + speed){		// unsigned char�Ȃ̂ŃI�[�o�[�t���[�h�~
			alpha = limit;
			mode = FADE_STOP;
		}
		else
			alpha -= speed;

		break;

	case FADE_OUT:

		// �ړI���܂ł�����A�X�g�b�v
		if (alpha >= limit - speed){		// unsigned char�Ȃ̂ŃI�[�o�[�t���[�h�~
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
	col = (alpha << 24) | color;	// argb�̃t�H�[�}�b�g�ɂ���

	iexPolygon::Rect(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, col);
}