#pragma once
#include "TDNLIB.h"
// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Message/Message.h"
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"
#include "HpGage.h"
#include "SpGage.h"
#include "PersonaUI.h"
//#include "Combo.h"


class GameUIManager :public BaseGameEntity
{
private:
	// �V���O���g���̍�@
	GameUIManager();
	~GameUIManager();
	GameUIManager(const GameUIManager&);
	GameUIManager &operator=(const GameUIManager&){}
	
	// 1�����Ȃ�����
	static GameUIManager *pInstance;

public:
	// ���̎擾
	static GameUIManager *GetInstance()
	{
		if (!pInstance) pInstance = new GameUIManager;
		return pInstance;
	}
	static void Rerease(){
		SAFE_DELETE(pInstance);
	}

	// �Q�Ƃ�����
	void ReferencesPlayer(BasePlayer* pLeftPlayer, BasePlayer* pRightPlayer);

	void Update();
	void Render();

	void Action();

	// BaseGameEntiry�T�u�N���X�̓��b�Z�[�W���g���ĒʐM����
	bool  HandleMessage(const Message& msg);

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