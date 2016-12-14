#pragma once
#include "BaseWindow.h"

//+-------------------------------
// ���U���g�E�B���h�E
//+-------------------------------


class ResultWindow : public BaseWindow
{
public:
	ResultWindow(Vector2 vPos);
	~ResultWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �I�����
	enum RESULT_STATE
	{
		AGAIN,
		BACK_CHARA,
		BACK_MENU
	};


private:
	static const int SizeX = 432;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

};
