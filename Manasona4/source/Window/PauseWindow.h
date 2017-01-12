#pragma once
#include "BaseWindow.h"

#include "MenuUI\InformationPlate\InformationPlate.h"
//+------------------------
// �|�[�Y�E�B���h�E
//+------------------------


class PauseWindow: public BaseWindow
{
public:
	PauseWindow(Vector2 vPos);
	~PauseWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �o�g���p�I�����
	enum BATTLE_STATE
	{
		COMMAND_LIST,
		SOUND_SETTING,
		HIDE_MENU,
		BACK_CHARA_SELECT,
		BACK_MENU_SELECT,
		BACK
	};

	
private:
	static const int SizeX = 432;

	// ���̃E�B���h�E�̉��o�̂��߂ɐF�X��낤
	//int m_iAlpha;
	//int m_iAddX;
	//int m_iAddY;

	InformationPlate* m_pInfoPlate;

	// ���߂�h�~
	bool m_bBackPush;
};
