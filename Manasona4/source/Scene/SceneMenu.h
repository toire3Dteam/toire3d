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
#include "SceneMenuState.h"


// �O���錾
class MyMusicManager;
class Camera;
class RoundCallManager;
class BaseWindow;
//+-----------------------------------
//	�V�[�����j���[
//+-----------------------------------

// �q���g�J�[�h�̎��
enum class TIPS_TYPE
{
	TUTORIAL, // �`���[�g���A���m�F
	
	ARRAY_END	// �z��I��

};

// ���j���[�̃E�B���h�E�̎��
enum class WINDOW_TYPE
{
	OPTION,		// �I�v�V����
	SOUND,		// �T�E���h
	ARRAY_END	// �z��I��

};

class sceneMenu : public BaseScene, BaseGameEntity
{
public:
	//�������E���
	sceneMenu();
	bool Initialize();
	~sceneMenu();
	//����
	void Update();
	//�`��
	void Render();

	// �X�e�[�g�}�V���擾
	StateMachine<sceneMenu> *GetFSM() { return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	// �A�N�Z�T�[
	MenuUIManager* GetMenuUIMgr() { return m_pMenuUIMgr; };
	int GetSelectNo() { return m_iSelectNo; }
	void AddSelectNo(int add) { m_iSelectNo += add; }
	void SetSelectNo(int set) { m_iSelectNo = set; }

	int GetCtrlDevice() { return m_iCtrlDevice; }
	void SetCtrlDevice(int set) { m_iCtrlDevice = set; }

	ControllerSelectUI* GetCtrlSelectUI() { return m_pCtrlSelectUI; }

	TipsCard* GetTips(TIPS_TYPE type) { return m_pTips[(int)type]; }
	BaseWindow* GetWindow(WINDOW_TYPE type) { return m_pWindow[(int)type]; }

private:

	// ���j���[UI
	MenuUIManager*	m_pMenuUIMgr;
	static int m_iSelectNo;
	static int m_iCtrlDevice;

	// �R���g���[���[�Z���N�g�p
	ControllerSelectUI* m_pCtrlSelectUI;

	// �m�F�p�e�B�b�v�X
	TipsCard* m_pTips[(int)TIPS_TYPE::ARRAY_END];

	// Window��
	BaseWindow* m_pWindow[(int)WINDOW_TYPE::ARRAY_END];

	StateMachine<sceneMenu> *m_pStateMachine;	// �X�e�[�g�}�V��
												

	// �X�e�[�g���t�����h�N���X��
	friend class SceneMenuState::FirstStep;	

};