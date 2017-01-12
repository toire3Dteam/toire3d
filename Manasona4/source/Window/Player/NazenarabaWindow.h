#pragma once
#include "../BaseWindow.h"
#include "../ParamSetting/DifficultyParamSetting.h"
#include "../ParamSetting/RoundNumTypeParamSetting.h"
#include "../ParamSetting/RoundTimeTypeParamSetting.h"

//+-------------------------------
//	�A�C���\�E�B���h�E
//+-------------------------------

class NazenarabaWindow : public BaseWindow
{
public:
	NazenarabaWindow(Vector2 vPos);
	~NazenarabaWindow();

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
		INVINCIBLE,
		OVERDRIVE,
	};


private:
	static const int m_cSizeX = 640;

};
