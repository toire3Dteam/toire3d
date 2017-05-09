#pragma once
#include "BaseWindow.h"

//+------------------------
// �I�v�V�����E�B���h�E
//+------------------------


class OptionWindow: public BaseWindow
{
public:
	OptionWindow(Vector2 vPos);
	~OptionWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �I�v�V�����p�I�����
	enum OPTION_STATE
	{
		SYSTEM,
		GAME,
		SOUND,
		KEYCONFIG,
		BACK
	};

	
private:
	static const int SizeX = 384;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;

};
