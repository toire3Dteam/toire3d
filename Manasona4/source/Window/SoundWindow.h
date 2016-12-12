#pragma once
#include "BaseWindow.h"

//+-------------------------------
//	�T�E���h�E�B���h�E
//+-------------------------------


class SoundWindow : public BaseWindow
{
public:
	SoundWindow(Vector2 vPos);
	~SoundWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �I�����
	enum SOUND_STATE
	{
		BGM,
		SE,
		VOICE,
		BACK
	};


private:
	static const int SizeX = 432;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

	// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b
	// ���Ċ����̐G��S�n��
	int iSpeedLv;
	int iLeftPushFrame;
	int iRightPushFrame;
	int iFrame;
};
