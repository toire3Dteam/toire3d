#pragma once
#include "BaseWindow.h"
#include "ParamSetting\Training\Hp1PParamSetting.h"
#include "ParamSetting\Training\Hp2PParamSetting.h"
#include "ParamSetting\Training\HPRecoveryParamSetting.h"
#include "ParamSetting\Training\InfoParamSetting.h"
#include "ParamSetting\Training\PartnerRecoveryParamSetting.h"
#include "ParamSetting\Training\SpGageParamSetting.h"


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
		HP_RECOVERY,
		MAXHP_1P,
		MAXHP_2P,
		SP_GAGE,
		PARTNER_GAGE,
		INFO,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 832;

	Hp1pParamSetting*				m_pHp1PParam;
	Hp2pParamSetting*				m_pHp2PParam;
	HpRecoveryParamSetting*			m_pHpRecoveryParam;
	InfoParamSetting*				m_pInfoParam;
	PartnerRecoveryParamSetting*	m_pPartnerRecoveryParam;
	SpGageParamSetting*				m_pSpGageParam;


};
