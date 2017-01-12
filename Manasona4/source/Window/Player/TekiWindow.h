#pragma once
#include "../BaseWindow.h"
#include "../ParamSetting/DifficultyParamSetting.h"
#include "../ParamSetting/RoundNumTypeParamSetting.h"
#include "../ParamSetting/RoundTimeTypeParamSetting.h"

//+-------------------------------
//	�e�L�E�B���h�E
//+-------------------------------

class TekiWindow : public BaseWindow
{
public:
	TekiWindow(Vector2 vPos);
	~TekiWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �I�v�V�����p�I�����
	enum OPTION_STATE
	{
		SKILL_1,
		SKILL_2,
		SKILL_3,
		SKILL_4,
		INVINCIBLE,
		OVERDRIVE,
	};


private:
	static const int m_cSizeX = 640;

};
