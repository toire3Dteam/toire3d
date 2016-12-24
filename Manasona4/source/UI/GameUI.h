#pragma once
#include "TDNLIB.h"
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"
#include "HpGage.h"
#include "SpGage.h"
#include "PersonaUI.h"
#include "RoundIcon.h"
#include "FacePic.h"
#include "OverDriveCutin\OverDriveCutin.h"
#include "ComboUI.h"
#include "Timer\Timer.h"

class GameUIManager :public BaseGameEntity
{
private:
	// �V���O���g���̍�@
	GameUIManager();
	~GameUIManager();
	GameUIManager(const GameUIManager&);
	GameUIManager &operator=(const GameUIManager&) {}

	// 1�����Ȃ�����
	static GameUIManager *pInstance;

public:
	// ���̎擾
	static GameUIManager *GetInstance()
	{
		if (!pInstance) pInstance = new GameUIManager;
		return pInstance;
	}
	static void Rerease() {
		SAFE_DELETE(pInstance);
	}

	// �Q�Ƃ�����
	void InitData(BasePlayer* pLeftPlayer, BasePlayer* pRightPlayer, int iRoundNum, ROUND_TIME_TYPE eRoundTimeType);

	void Update();
	void Render();
	void RenderBack();

	void Action();
	
	// �^�C�}�[�֘A
	void TimerReset();
	void TimerStart();
	void TimerStop();
	bool isTimerUp();

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

	// �A�N�Z�T�[
	ComboUI* GetComboUI(SIDE eSide)
	{
		switch (eSide)
		{
		case SIDE::LEFT:
			return  m_pCombo1P;
			break;
		case SIDE::RIGHT:
			return  m_pCombo2P;
			break;
		default:
			MyAssert(0,"���̃T�C�h�͂Ȃ�");
			break;
		}
		return m_pCombo1P;
	}
	
	HpGage* GetHpGage(SIDE eSide)
	{
		switch (eSide)
		{
		case SIDE::LEFT:
			return  m_pHpGage1P;
			break;
		case SIDE::RIGHT:
			return  m_pHpGage2P;
			break;
		default:
			MyAssert(0, "���̃T�C�h�͂Ȃ�");
			break;
		}
		return m_pHpGage1P;
	}

	// ����
	Timer* GetTimer()
	{
		return m_pTimer;
	}

private:

	//tdn2DObj* m_teamPic;

	tdn2DAnim* m_pHpFrame;
	tdn2DAnim* m_pSpFrame;

	// HP�Q�[�W
	HpGage* m_pHpGage1P;
	HpGage* m_pHpGage2P;

	// SP�Q�[�W
	SpGage* m_pSpGage1P;
	SpGage* m_pSpGage2P;

	// �y���\�i�p
	PersonaUI* m_pPersona1P;
	PersonaUI* m_pPersona2P;

	// ���E���h�A�C�R���p
	RoundIcon* m_pRoundIcon1P;
	RoundIcon* m_pRoundIcon2P;

	// ��O��
	FacePic* m_pFacePic1P;
	FacePic* m_pFacePic2P;

	// �I�[�o�[�h���C�u�̃J�b�g�C��
	OverDriveCutin* m_pOverDriveCutin1P;
	OverDriveCutin* m_pOverDriveCutin2P;
	
	// UI
	ComboUI*  m_pCombo1P;
	ComboUI*  m_pCombo2P;

	// �^�C�}�[
	Timer* m_pTimer;

	// ���s
	Surface* m_pStencilSurface;

	//struct  HPGage
	//{
	//	tdn2DObj* obj;
	//	float frame;
	//};
	//HPGage	

	//Combo* m_combo;

};

// ���̉�
#define GameUIMgr (GameUIManager::GetInstance())