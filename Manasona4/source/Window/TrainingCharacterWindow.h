#pragma once
#include "BaseWindow.h"
#include "ParamSetting\Training3\AramitamaWassyoiParamSetting.h"

//+----------------------------------
//	�g���[�j���O�_�~�[�E�B���h�E
//+----------------------------------

class TrainingCharacterWindow : public BaseWindow
{
public:
	TrainingCharacterWindow(Vector2 vPos);
	~TrainingCharacterWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

					// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);


	// �I�����
	enum TRAINING_CHARACTER_STATE
	{
		ARAMITAMA_WASSYOI,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 832;

	AramitamaWassyoiParamSetting*				m_pAWassyoiParam;

};
