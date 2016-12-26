#pragma once
#include "BaseWindow.h"
#include "ParamSetting\Training2\EnemyGuardParamSetting.h"
#include "ParamSetting\Training2\EnemyGuardSwitchParamSetting.h"
#include "ParamSetting\Training2\EnemyStateParamSetting.h"
#include "ParamSetting\Training2\TechParamSetting.h"
#include "ParamSetting\Training2\ThrowTechParamSetting.h"
#include "ParamSetting\Training2\EnemyLvParamSetting.h"

//+----------------------------------
//	�g���[�j���O�_�~�[�E�B���h�E
//+----------------------------------

class TrainingDummyWindow : public BaseWindow
{
public:
	TrainingDummyWindow(Vector2 vPos);
	~TrainingDummyWindow();

	bool Update();	// �߂�l�ŋN�����Ă��邩�m�F�@���̐�����~�߂�ׂɎg���܂�
	void Redner();	// �`��

	bool Ctrl(int DeviceID);    // ����

	void Action();	// Window���o��
	void Stop();	// �~�߂�

					// �p�����[�^�ݒ�p��
	void RenderParamSetting(int No, int x, int y);
	void CtrlParamSetting(int SelectNo, int DeviceID);


	// �I�����
	enum TRAINING_DUMMY_STATE
	{
		ENEMY_STATE,
		ENEMY_LV,
		ENEMY_GUARD,
		ENEMY_GUARD_SWITCH,
		ENEMY_TECH,
		ENEMY_THROW_TECH,
		RESET,
		BACK
	};


private:
	static const int m_cSizeX = 832;

	EnemyStateParamSetting*				m_pEStateParam;
	EnemyLvParamSetting*				m_pELvParam;
	EnemyGuardParamSetting*				m_pEGuardParam;
	EnemyGuardSwitchParamSetting*		m_pEGuardSwitchParam;
	TechParamSetting*					m_pETechParam;
	ThrowTechParamSetting*				m_pEThrowTechParam;

};
