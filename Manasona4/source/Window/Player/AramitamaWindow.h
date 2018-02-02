#pragma once
#include "../BaseWindow.h"
#include "../ParamSetting/DifficultyParamSetting.h"
#include "../ParamSetting/RoundNumTypeParamSetting.h"
#include "../ParamSetting/RoundTimeTypeParamSetting.h"

//+-------------------------------
//	���󂳂ꂵ���E�B���h�E
//+-------------------------------

class AramitamaWindow : public BaseWindow
{
public:
	AramitamaWindow(Vector2 vPos);
	~AramitamaWindow();

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
		SKILL_5,
		SKILL_6,
		SKILL_7,
		INVINCIBLE,
		OVERDRIVE,
	};


private:
	static const int m_cSizeX = 640;

};
