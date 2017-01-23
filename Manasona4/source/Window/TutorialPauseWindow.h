#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"
#include "ParamSetting\Tutorial\TutorialSelectParamSetting.h"

//+-------------------------------
// �`���[�g���A���|�[�Y�E�B���h�E
//+-------------------------------


class TutorialPauseWindow : public BaseWindow
{
public:
	TutorialPauseWindow(Vector2 vPos);
	~TutorialPauseWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);

	// �`���[�g���A���p�I�����
	enum TUTORIAL_PAUSE_STATE
	{
		//BACK_PREV_TUTORIAL,
		SELECT_TUTORIAL,
		COMMAND_LIST,
		BACK_MENU_SELECT,
		BACK
	};


private:
	static const int SizeX = 768;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	// �����̃v���[�g
	InformationPlate* m_pInfoPlate;

	TutorialSelectParamSetting* m_TutorialSelectParam;

	// ���߂�h�~
	bool m_bBackPush;
};
