#ifndef FADE_H_
#define	FADE_H_

//=======================================================================
//						�t�F�[�h�N���X
//=======================================================================


class Fade
{
public:
	//------------------------------------------------------
	//	�萔
	//------------------------------------------------------
	enum FLAG{ FADE_NONE, FADE_IN, FADE_OUT, MAX };

	//------------------------------------------------------
	//	��{�p�����[�^
	//------------------------------------------------------
	static FLAG m_mode;				/*	�t�F�[�h�C���A�t�F�[�h�A�E�g�A�������Ȃ�	*/
	static BYTE m_speed;		/*	�t�F�[�h���鑬�x							*/
	static BYTE m_limit;		/*	�ǂ��܂Ńt�F�[�h���邩(0�`255�͈̔�)		*/
	static BYTE m_alpha;		/*	���l(0�`255)								*/
	static COLOR m_dwColor;		/*	�J���[(0x00000000 �` 0x00ffffff)			*/

	static bool m_bFadeInCompletion;	/*	�t�F�[�h�C������	*/
	static bool m_bFadeOutCompletion;	/*	�t�F�[�h�A�E�g����	*/

	//void None();
	//void In();
	//void Out();
	//static void(Fade::*Fade_mode_funk[FLAG::MAX])();

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	static void Initialize();


	//------------------------------------------------------
	//	�ݒ�
	//------------------------------------------------------
	static void Set(FLAG Mode, BYTE Speed, BYTE Limit, BYTE StartAlpha, COLOR Color = 0x00000000);	/*	�t���ݒ��			*/
	static void Set(FLAG Mode, BYTE Speed, COLOR Color = 0x00000000);													/*	�ȈՔ�				*/


	//------------------------------------------------------
	//	�X�V�ƕ`��
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
