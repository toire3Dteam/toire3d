#pragma once
#include "BaseWindow.h"

//+------------------------
// �|�[�Y�E�B���h�E
//+------------------------


class PoseWindow: public BaseWindow
{
public:
	PoseWindow(Vector2 vPos);
	~PoseWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �o�g���p�I�����
	enum BATTLE_STATE
	{
		BACK_CHARA_SELECT,
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
