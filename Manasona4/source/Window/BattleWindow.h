#pragma once
#include "BaseWindow.h"

//+------------------------
// �o�g���E�B���h�E
//+------------------------


class BattleWindow: public BaseWindow
{
public:
	BattleWindow(Vector2 vPos);
	~BattleWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �o�g���p�I�����
	enum BATTLE_STATE
	{
		BACK_CHARA_SELECT,
		BACK
	};

	
private:
	static const int SizeX = 384;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	int m_iAlpha;
	int m_iAddX;
	int m_iAddY;

};
