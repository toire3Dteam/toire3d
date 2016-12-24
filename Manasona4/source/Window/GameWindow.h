#pragma once
#include "BaseWindow.h"
#include "ParamSetting/DifficultyParamSetting.h"
#include "ParamSetting/RoundNumTypeParamSetting.h"
#include "ParamSetting/RoundTimeTypeParamSetting.h"

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
		ROUND,
		TIME,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 704;

	DifficultyParamSetting* m_pDifficultyParam;
	RoundNumTypeParamSetting* m_pRoundNumTypeParam;
	RoundTimeTypeParamSetting* m_pRoundTimeTypeParam;

};
