#pragma once

// ������C���N���[�h�����BaseScene���ǂ�����������̂�
#include "../system/FrameworkEx.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// ���j���[�}�l�[�W���[
#include "MenuUI\MenuUIManager.h"
#include "MenuUI\ControllerSelectUI.h"

#include "SceneCollectState.h"

// �O���錾
class MyMusicManager;
class Camera;
class RoundCallManager;
class BaseWindow;

namespace PlayerData
{
struct Infomation;
}

class sceneCollect : public BaseScene, BaseGameEntity
{
public:

	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	sceneCollect();
	bool Initialize();
	~sceneCollect();

	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	void Update();
	void Render();
 
	// �������o
	void FirstAction();

	void PiPiPiCtrl(int iDeviceID);

	//------------------------------------------------------
	//	�g���t�B�[
	//------------------------------------------------------
	void TrophyInit();
	void TrophyUpdate();
	void TrophyRender();
	void TrophyCtrl(int iDeviceID);

	//------------------------------------------------------
	//	�A�N�Z�T�[
	//------------------------------------------------------
	// �X�e�[�g�}�V���擾
	StateMachine<sceneCollect> *GetFSM() { return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	enum IMAGE
	{
		BACK,
		SCROLL_BAR,
		SCROLL_ARROW,
		TITLE,
		INFO_PLATE,
		ARRAY_END
	};

private:

	StateMachine<sceneCollect> *m_pStateMachine;	// �X�e�[�g�}�V��


	tdn2DAnim *m_pImages[IMAGE::ARRAY_END];
	
	// �v���C���
	PlayerData::Infomation *m_pPlayerInfo;

	// �X�N���[������
	tdn2DObj* m_pScrollBar;
	tdn2DObj* m_pScrollArrow;

	// �X�N���[��
	float m_iScrollHeight;	// �ő�̍���
	float m_iScrollScreenSize;// ��̃X�N���[����
	Vector2 m_vScrollPos;

	//+-------------------------------
	//	�g���t�B�[�ŕK�v�ȕϐ���p��
	//+-------------------------------
	
	// �g���t�B�[�̃A�C�R���̏�񂪋l�܂����\����
	struct TrophyIconDesc
	{
		int iTop;			// �A�C�R���̈�ԁ�
		int iBottom;		// �A�C�R���̈�ԁ�
		int iMaxHeight;	    // �A�C�R���̍ő�c��
		int iRowNum;		// �A�C�R���̗�

		int iSelectNo;		// �I�����Ă���A�C�R���̔ԍ�
		Vector2 vSelectPos;	// �I�����Ă���ꏊ
		Vector2 vNextPos;	// ���Ɉړ�����ꏊ
		tdn2DAnim* pSelectMark;// �I�𒆂̃}�[�N�G
		tdn2DAnim* pSelectMarkEdge;// �I�𒆂̃}�[�N�G(��)

		// �X�N���[��
		Vector2 vScrollPos;	// �I�����Ă���ꏊ
		Vector2 vScrollNextPos;	// ���Ɉړ�����ꏊ

		TrophyIconDesc();
	};
	TrophyIconDesc m_tagTI;

	//+-------------------------------
	//	�A�C�R������̂��߂�
	//+-------------------------------
	// �s�b...�s�b...�s�b..�s�b.�s�b�s�b�s�b�s�b
	// ���Ċ����̐G��S�n��
	int m_iSpeedLv;
	int m_iUpPushFrame;
	int m_iDownPushFrame;
	int m_iLeftPushFrame;
	int m_iRightPushFrame;

	bool m_bUpPush;			// �㉟����
	bool m_bDownPush;		// ��������
	bool m_bLeftPush;		// ��������
	bool m_bRightPush;		// �E������


	// �X�e�[�g���t�����h�N���X��
	friend class SceneCollectState::TrophyStep;

};