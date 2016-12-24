#pragma once
#include "BaseWindow.h"
#include "ParamSetting\BGMParamSetting.h"
#include "ParamSetting\SEParamSetting.h"
#include "ParamSetting\VoiceParamSetting.h"

//+----------------------------------
//	�g���[�j���O�I�v�V�����E�B���h�E
//+----------------------------------

class TrainingOptionWindow : public BaseWindow
{
public:
	TrainingOptionWindow(Vector2 vPos);
	~TrainingOptionWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

					// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);


	// �I�����
	enum TRAINING_OPTION_STATE
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

};
