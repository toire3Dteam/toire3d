#pragma once
#include "BaseWindow.h"
#include "MenuUI\InformationPlate\InformationPlate.h"

//+-------------------------------
// �g���[�j���O�|�[�Y�E�B���h�E
//+-------------------------------


class TrainingPauseWindow : public BaseWindow
{
public:
	TrainingPauseWindow(Vector2 vPos);
	~TrainingPauseWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �g���[�j���O�p�I�����
	enum TRAINING_PAUSE_STATE
	{
		TRAINING_SETTING,
		POSITION_RESET,
		SOUND_SETTING,
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

	// �����̃v���[�g
	InformationPlate* m_pInfoPlate;

};
