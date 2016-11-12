#ifndef FADE_H_
#define	FADE_H_

//=======================================================================
//						フェードクラス
//=======================================================================


class Fade
{
public:
	//------------------------------------------------------
	//	定数
	//------------------------------------------------------
	enum FLAG{ FADE_NONE, FADE_IN, FADE_OUT, MAX };

	//------------------------------------------------------
	//	基本パラメータ
	//------------------------------------------------------
	static FLAG m_mode;				/*	フェードイン、フェードアウト、何もしない	*/
	static BYTE m_speed;		/*	フェードする速度							*/
	static BYTE m_limit;		/*	どこまでフェードするか(0～255の範囲)		*/
	static BYTE m_alpha;		/*	α値(0～255)								*/
	static COLOR m_dwColor;		/*	カラー(0x00000000 ～ 0x00ffffff)			*/

	static bool m_bFadeInCompletion;	/*	フェードイン完了	*/
	static bool m_bFadeOutCompletion;	/*	フェードアウト完了	*/

	//void None();
	//void In();
	//void Out();
	//static void(Fade::*Fade_mode_funk[FLAG::MAX])();

	//------------------------------------------------------
	//	初期化
	//------------------------------------------------------
	static void Initialize();


	//------------------------------------------------------
	//	設定
	//------------------------------------------------------
	static void Set(FLAG Mode, BYTE Speed, BYTE Limit, BYTE StartAlpha, COLOR Color = 0x00000000);	/*	フル設定版			*/
	static void Set(FLAG Mode, BYTE Speed, COLOR Color = 0x00000000);													/*	簡易版				*/


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	static void Update();
	static void Render(u32 dwFlags = RS::COPY);

	//------------------------------------------------------
	//	Get_Set
	//------------------------------------------------------
	static bool isFadeStop(){ return (m_mode == FADE_NONE); }
	static FLAG GetMode() { return m_mode; }
	static bool isFadeInCompletion() { return m_bFadeInCompletion; }
	static bool isFadeOutCompletion() { return m_bFadeOutCompletion; }
};

#endif
