#pragma once
#include "BaseWindow.h"
#include "ParamSetting\BGMParamSetting.h"
#include "ParamSetting\SEParamSetting.h"
#include "ParamSetting\VoiceParamSetting.h"

//+-------------------------------
//	�T�E���h�E�B���h�E
//+-------------------------------


class SoundWindow : public BaseWindow
{
public:
	SoundWindow(Vector2 vPos);
	~SoundWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

	// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo,int DeviceID);


	// �I�����
	enum SOUND_STATE
	{
		BGM,
		SE,
		VOICE,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 768;

	BGMParamSetting* m_pBGMParam;
	SEParamSetting* m_pSEParam;
	VoiceParamSetting* m_pVoiceParam;

	//// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b
	//// ���Ċ����̐G��S�n��
	//int iSpeedLv;
	//int iLeftPushFrame;
	//int iRightPushFrame;
	//int iFrame;
};
