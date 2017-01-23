#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"
#include "ParamSetting\Challenge\ChallengeSelectParamSetting.h"

//+-------------------------------
// �`�������W�|�[�Y�E�B���h�E
//+-------------------------------


class ChallengePauseWindow : public BaseWindow
{
public:
	ChallengePauseWindow(Vector2 vPos);
	~ChallengePauseWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);

	// �`���[�g���A���p�I�����
	enum CHALLENGE_PAUSE_STATE
	{
		//BACK_PREV_TUTORIAL,
		SELECT_CHALLENGE,
		POSITION_RESET,
		COMMAND_LIST,
		SOUND_SETTING,
		HIDE_MENU,
		BACK_MENU_SELECT,
		BACK
	};


private:
	static const int SizeX = 704;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	// �����̃v���[�g
	InformationPlate* m_pInfoPlate;

	ChallengeSelectParamSetting* m_ChallengeSelectParam;

	// ���߂�h�~
	bool m_bBackPush;
};
