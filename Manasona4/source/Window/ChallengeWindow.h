#pragma once
#include "BaseWindow.h"

//+------------------------
// �`�������W�E�B���h�E
//+------------------------


class ChallengeWindow: public BaseWindow
{
public:
	ChallengeWindow(Vector2 vPos);
	~ChallengeWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �I�����
	enum CHALLENGE_STATE
	{
		AIROU,
		TEKI,
		NAZENARABA,
		ARAMITAMA,
		BACK
	};

	
private:
	static const int SizeX = 432;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;

};
