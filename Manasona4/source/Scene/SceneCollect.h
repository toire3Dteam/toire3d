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
#include "MenuUI\TipsCard.h"

// �C���X�g�p
#include "Collect\PictureManager.h"

// �O���錾
class MyMusicManager;
class Camera;
class RoundCallManager;
class BaseWindow;
class CoinUI;

// �^�O�̎��
enum class TAG_TYPE
{
	TROPHY,
	PICTURE,

};

// �q���g�J�[�h�̎��
enum class TIPS_TYPE_COLLECT
{
	BACK_MENU,		// ���j���[�ɖ߂�m�F
	CONTENT_BUY,	// �w���m�F

	ARRAY_END		// �z��I��

};

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
	void TrophyFirstAction();
	void TrophyUpdate();
	void TrophyRender();
	void TrophyCtrl(int iDeviceID);

	//------------------------------------------------------
	//	�C���X�g
	//------------------------------------------------------
	void PictureInit();
	void PictureFirstAction();
	void PictureUpdate();
	void PictureRender();

	void PictureInitExe();
	void PictureRenderExe();
	void PictureCtrl(int iDeviceID);

	//------------------------------------------------------
	//	�A�N�Z�T�[
	//------------------------------------------------------
	// �X�e�[�g�}�V���擾
	StateMachine<sceneCollect> *GetFSM() { return m_pStateMachine; }
	
	TipsCard* GetTips(TIPS_TYPE_COLLECT type) { return m_pTips[(int)type]; }
	int GetCtrlDevice() { return m_iCtrlDevice; }
	void SetCtrlDevice(int set) { m_iCtrlDevice = set; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }

	enum IMAGE
	{
		BACK,
		SCROLL_BAR,
		SCROLL_ARROW,
		TITLE,
		INFO_PLATE,
		INFO_PLATE_WATCH,
		TAG_TROPHY,
		TAG_PICTURE,
		TROPHY_COMP_PERCENT,
		PRICE_PLATE,
		NUMBER,
		NUMBER_TROPHY,
		ARRAY_END
	};

	tdn2DAnim* GetImage(IMAGE eImage) { return m_pImages[(int)eImage]; };

	TAG_TYPE GetTagType() { return m_eTagType; };
	void SetTagType(TAG_TYPE eTagType) { m_eTagType = eTagType; }

	PictureManager* GetPictureMgr() { return m_pPictureMgr; }


private:
	static int m_iCtrlDevice;
	static TAG_TYPE m_eTagType;

	// �m�F�p�e�B�b�v�X
	TipsCard* m_pTips[(int)TIPS_TYPE_COLLECT::ARRAY_END];

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

	// �^�O�̏ꏊ
	Vector2 m_vTagPos;

	// �g���t�B�[�R���v�̏ꏊ
	Vector2 m_vTrophyPercentPos;

	// �摜�ӏܑO�̍����t�F�[�h
	bool m_bBlackRect;
	int	 m_iBlackRectAlpha;

	// �摜�ӏ܂̎��̐���������Ȃ�
	bool m_bWatchInfo;

	CoinUI* m_pCoinUI;

	//+-------------------------------
	//	�g���t�B�[�ŕK�v�ȕϐ���p��
	//+-------------------------------
	
	// �A�C�R���̏�񂪋l�܂����\����
	struct IconDesc
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
		int iScrollAddX;		//	�z�u�Ԋu
		int iScrollAddY;

		IconDesc();
	};
	IconDesc m_tagTI;

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

	//+-------------------------------
	//	�C���X�g�ŕK�v�ȕϐ���p��
	//+-------------------------------

	// ���C���X�g�}�l�[�W���[
	PictureManager* m_pPictureMgr;
	IconDesc m_tagPI;

	// �X�e�[�g���t�����h�N���X��
	friend class SceneCollectState::TrophyStep;
	friend class SceneCollectState::PictureStep;
	friend class SceneCollectState::PictureWatch;
	friend class SceneCollectState::BuyPictureStep;
	friend class SceneCollectState::BackMenuStep;


};