#pragma once
#include "BaseWindow.h"

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

	// �`���[�g���A���p�I�����
	enum TUTORIAL_PAUSE_STATE
	{
		BACK_MENU_SELECT,
		BACK
	};


private:
	static const int SizeX = 432;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

};
