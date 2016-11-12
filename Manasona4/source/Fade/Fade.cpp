#include	"tdnlib.h"

#include	"Fade.h"

//*********************************************************************************************
//		パラメータの設定
//*********************************************************************************************
Fade::FLAG Fade::m_mode;		/*	フェードイン、フェードアウト、何もしない	*/
BYTE Fade::m_speed;	/*	フェードする速度	*/
BYTE Fade::m_limit;	/*	どこまでフェードするか(0～255の範囲)	*/
BYTE Fade::m_alpha;	/*	α値(0～255)	*/
COLOR Fade::m_dwColor;	/*	カラー(0x○○000000 ～ 0x○○ffffff)ここの○○の部分を必ず00にしておくこと！	*/

bool Fade::m_bFadeInCompletion;	/*  フェードインが完了したか  */
bool Fade::m_bFadeOutCompletion;/*  フェードアウトが完了したか  */

//=============================================================================================
//		初	期	化
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
//		フェード設定
void Fade::Set(FLAG Mode, BYTE Speed, BYTE Limit, BYTE StartAlpha, COLOR Color)
{
	m_mode = Mode;		// フェードモード(FADE_IN or FADE_OUT)
	m_speed = Speed;		// フェードを増減させるスピード(5なら、毎フレーム「5」が増減される)
	m_limit = Limit;		// 目標alpha値(FADE_INなら「0」、FADE_OUTなら「255」が基本)
	m_alpha = StartAlpha;	// フェード開始時のalpha値(FADE_INなら「255」、DADE_OUTなら「0」が基本)
	m_dwColor = Color & 0x00ffffff;		// フェード色

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
//		更		新
void Fade::Update()
{
	switch (m_mode)
	{
	case FADE_IN:

		// 目的αまでついたら、ストップ
		if (m_alpha <= m_limit + m_speed){		// BYTEなのでオーバーフロー防止
			m_alpha = m_limit;
			m_mode = FADE_NONE;
			m_bFadeInCompletion = true;
		}
		else
			m_alpha -= m_speed;

		break;

	case FADE_OUT:

		// 目的αまでついたら、ストップ
		if (m_alpha >= m_limit - m_speed){		// BYTEなのでオーバーフロー防止
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
//		描			画
void Fade::Render(u32 dwFlags)
{
	const COLOR c = (m_alpha << 24) | m_dwColor;	// argbのフォーマットにする

	//if (KeyBoardTRG(KB_DOWN)) m_alpha -= 16;
	//if (KeyBoardTRG(KB_UP))
	//	m_alpha += 16;
	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, dwFlags, c);
}
//
//=============================================================================================