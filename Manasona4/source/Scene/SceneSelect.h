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
	void StageAndBGMRender();

	void FirstAction();

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
												
												// �摜
	enum PIC_TYPE
	{
		BG,
		TITLE,
		BLACK_LINE,
		INFO_PLATE,
		ARRAY_END,
	};
	tdn2DAnim* m_pPic[PIC_TYPE::ARRAY_END];

	int m_iRectAlpha;

	// �X�e�[�W��BGM�̑I��ԍ�
	static int m_iSelectStageNo;
	static int m_iSelectBGMNo;

	// �X�e�[�W��BGM
	// �摜�͂����邾��
	struct SECOND_SELECT
	{
		int iX, iY;

		bool bBGMSelect;	

		tdn2DObj* pPlate;
		tdn2DObj* pStagePlate;
		tdn2DObj* pBGMPlate;
		tdn2DObj* pStagePic;

		// ���
		tdn2DObj* pArrow;

		tdn2DAnim* pStageLeftRip;
		tdn2DAnim* pStageRightRip;			 
		tdn2DAnim* pBGMLeftRip;
		tdn2DAnim* pBGMRightRip;
	};

	SECOND_SELECT m_tagSecondSelect;


	// ��Ղ̃t�����h�N���X�̎g����
	friend class SceneSelectState::Intro;	
	friend class SceneSelectState::CharaSelect;
	friend class SceneSelectState::StageAndBGM;
};