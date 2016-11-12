#include	"tdnlib.h"

#include	"Fade.h"

//*********************************************************************************************
//		�p�����[�^�̐ݒ�
//*********************************************************************************************
Fade::FLAG Fade::m_mode;		/*	�t�F�[�h�C���A�t�F�[�h�A�E�g�A�������Ȃ�	*/
BYTE Fade::m_speed;	/*	�t�F�[�h���鑬�x	*/
BYTE Fade::m_limit;	/*	�ǂ��܂Ńt�F�[�h���邩(0�`255�͈̔�)	*/
BYTE Fade::m_alpha;	/*	���l(0�`255)	*/
COLOR Fade::m_dwColor;	/*	�J���[(0x����000000 �` 0x����ffffff)�����́����̕�����K��00�ɂ��Ă������ƁI	*/

bool Fade::m_bFadeInCompletion;	/*  �t�F�[�h�C��������������  */
bool Fade::m_bFadeOutCompletion;/*  �t�F�[�h�A�E�g������������  */

//=============================================================================================
//		��	��	��
void Fade::Initialize()
{
	m_mode = FADE_NONE;
	m_speed = 1;
	m_alpha = 255;
	m_dwColor = 0x00000000;

	m_bFadeInCompletion = false;
	m_bFadeOutCompletion = false;
}
//
//=============================================================================================


//=============================================================================================
//		�t�F�[�h�ݒ�
void Fade::Set(FLAG Mode, BYTE Speed, BYTE Limit, BYTE StartAlpha, COLOR Color)
{
	m_mode = Mode;		// �t�F�[�h���[�h(FADE_IN or FADE_OUT)
	m_speed = Speed;		// �t�F�[�h�𑝌�������X�s�[�h(5�Ȃ�A���t���[���u5�v�����������)
	m_limit = Limit;		// �ڕWalpha�l(FADE_IN�Ȃ�u0�v�AFADE_OUT�Ȃ�u255�v����{)
	m_alpha = StartAlpha;	// �t�F�[�h�J�n����alpha�l(FADE_IN�Ȃ�u255�v�ADADE_OUT�Ȃ�u0�v����{)
	m_dwColor = Color & 0x00ffffff;		// �t�F�[�h�F

	m_bFadeInCompletion = false;	
	m_bFadeOutCompletion = false;	
}


void Fade::Set(FLAG Mode, BYTE Speed, COLOR Color)
{
	m_mode = Mode;
	m_speed = Speed;
	m_dwColor = Color & 0x00ffffff;

	switch (m_mode)
	{
	case FADE_IN:
		m_alpha = 255;
		m_limit = 0;
		break;

	case FADE_OUT:
		m_alpha = 0;
		m_limit = 255;
		break;

	case FADE_NONE:
		break;
	}

	m_bFadeInCompletion = false;
	m_bFadeOutCompletion = false;
}
//
//=============================================================================================



//=============================================================================================
//		�X		�V
void Fade::Update()
{
	switch (m_mode)
	{
	case FADE_IN:

		// �ړI���܂ł�����A�X�g�b�v
		if (m_alpha <= m_limit + m_speed){		// BYTE�Ȃ̂ŃI�[�o�[�t���[�h�~
			m_alpha = m_limit;
			m_mode = FADE_NONE;
			m_bFadeInCompletion = true;
		}
		else
			m_alpha -= m_speed;

		break;

	case FADE_OUT:

		// �ړI���܂ł�����A�X�g�b�v
		if (m_alpha >= m_limit - m_speed){		// BYTE�Ȃ̂ŃI�[�o�[�t���[�h�~
			m_alpha = m_limit;
			m_mode = FADE_NONE;
			m_bFadeOutCompletion = true;
		}
		else
			m_alpha += m_speed;

		break;

	case FADE_NONE:
		break;	// NothingDo
	}
}


//=============================================================================================
//		�`			��
void Fade::Render(u32 dwFlags)
{
	const COLOR c = (m_alpha << 24) | m_dwColor;	// argb�̃t�H�[�}�b�g�ɂ���

	//if (KeyBoardTRG(KB_DOWN)) m_alpha -= 16;
	//if (KeyBoardTRG(KB_UP))
	//	m_alpha += 16;
	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, dwFlags, c);
}
//
//=============================================================================================