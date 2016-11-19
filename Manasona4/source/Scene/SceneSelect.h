#pragma once

// ������C���N���[�h�����BaseScene���ǂ�����������̂�
#include "../system/FrameworkEx.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// ���j���[�}�l�[�W���[
#include "MenuUI\MenuUIManager.h"
#include "MenuUI\ControllerSelectUI.h"

// �e���v���[�g�N���X���t�����h�N���X�ň������߂ɂ͐�ɒ�`����K�v������
#include "sceneSelectState.h"

// �Z���N�g
#include "SelectUI\SelectUIManager.h"



// �O���錾
class MyMusicManager;
class Camera;
class RoundCallManager;

//+-----------------------------------
//	�V�[���L�����N�^�[�Z���N�g
//+-----------------------------------

class sceneSelect: public BaseScene, BaseGameEntity
{
public:
	//�������E���
	sceneSelect();
	bool Initialize();
	~sceneSelect();
	//����
	void Update();
	//�`��
	void Render();

	// �X�e�[�g�}�V���擾
	StateMachine<sceneSelect> *GetFSM() { return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	// �A�N�Z�T�[
	//MenuUIManager* GetMenuUIMgr() { return m_pMenuUIMgr; };
	//int GetSelectNo() { return m_iSelectNo; }
	//void AddSelectNo(int add) { m_iSelectNo += add; }
	//void SetSelectNo(int set) { m_iSelectNo = set; }

	SelectUIManager*	 GetSelectUIMgr() { return m_pSelectUIMgr; }
private:

	// selectUI
	SelectUIManager*	m_pSelectUIMgr;
	//static int m_iSelectNo;


	StateMachine<sceneSelect> *m_pStateMachine;	// �X�e�[�g�}�V��
												

	// ��Ղ̃t�����h�N���X�̎g����
	//friend class SceneSelectState::FirstStep;	

};