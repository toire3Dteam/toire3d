#pragma once
#include "BaseWindow.h"
#include "ParamSetting/DifficultyParamSetting.h"

//+-------------------------------
//	�Q�[���E�B���h�E
//+-------------------------------


class GameWindow : public BaseWindow
{
public:
	GameWindow(Vector2 vPos);
	~GameWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo,int DeviceID);


	// �I�����
	enum GAME_STATE
	{
		DIFFICULTY,
		BACK
	};


private:
	static const int m_cSizeX = 768;

	DifficultyParamSetting* m_pDifficultyParam;

	//// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b
	//// ���Ċ����̐G��S�n��
	//int iSpeedLv;
	//int iLeftPushFrame;
	//int iRightPushFrame;
	//int iFrame;
};
