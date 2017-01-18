#pragma once
#include "BaseWindow.h"
#include "ParamSetting/AutoSaveParamSetting.h"

//+-------------------------------
//	�V�X�e���E�B���h�E
//+-------------------------------


class SystemWindow : public BaseWindow
{
public:
	SystemWindow(Vector2 vPos);
	~SystemWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo,int DeviceID);


	// �I�����
	enum SYSTEM_STATE
	{
		AUTO_SAVE,
		SAVE,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 704;

	AutoSaveParamSetting* m_pAutoSaveParam;

};
