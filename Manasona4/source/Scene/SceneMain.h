#pragma once

// ������C���N���[�h�����BaseScene���ǂ�����������̂�
#include "../system/FrameworkEx.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

#include "MenuUI\TutorialManager.h"

// �|�[�Y�E�B���h�E
#include "Window\PauseWindow.h"
#include "Window\TutorialPauseWindow.h"
#include "Window\TrainingPauseWindow.h"
#include "Window\SoundWindow.h"
#include "Window\TrainingOptionWindow.h"
#include "Window\TrainingDummyWindow.h"
#include "Window\TrainingCharacterWindow.h"
#include "Window\ChallengePauseWindow.h"

// ���U���g�f�[�^
#include "Data\ResultData.h"

// �O���錾
class ShotManager;
class Camera;
class RoundCallManager;
class OverDriveStage;
class SceneSwitchPrev;

//class Stage::Base;	// �~
namespace Stage{		// ��
	class Base;
}

// �o�g���̃E�B���h�E�̎��
enum class BATTLE_WINDOW_TYPE
{
	PAUSE,					// �|�[�Y
	TUTORIAL_PAUSE,			// �`���[�g���A���|�[�Y
	TRAINING_PAUSE,			// �g���[�j���O�|�[�Y
	CHALLENGE_PAUSE,		// �`�������W�|�[�Y
	SOUND,					// �T�E���h
	TRAINING_OPTION,		// �g���[�j���O�I�v�V����
	TRAINING_DUMMY,			// �g���[�j���O�_�~�[�I�v�V����
	TRAINING_CHARACTER,		// �g���[�j���O�L�����N�^�[�I�v�V����
	ARRAY_END				// �z��I��

};

class sceneMain : public BaseScene, BaseGameEntity
{
public:
	//�������E���
	sceneMain();
	bool Initialize();
	~sceneMain();
	//����
	void Update();
	//�`��
	void RenderStage();
	void Render();
	void RenderShadow();
	void SurfaceRender();

	void Reset();
	
	void InitTrainingData();	// ���g���[�j���O�f�[�^��K�p


	// �X�e�[�g�}�V���擾
	StateMachine<sceneMain> *GetFSM(){ return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg){ return m_pStateMachine->HandleMessage(msg); }

	Stage::Base *GetStage() { return m_pStage;	}
	ShotManager *GetShotManager(){ return m_pShotMgr; }

	void SetRoundNum(int roundNum) { m_iRoundNum = roundNum; }
	int GetRoundNum() { return m_iRoundNum; }

	RoundCallManager *GetRoundCall(){ return m_pRoundCallMgr; }

	SceneSwitchPrev *GetSceneSwitchPrev(){ return prevEF; }

	OverDriveStage* GetOverDriveStage() { return m_pOverDriveStage; }

	void OverDriveUpdate();
	void OverDriveAction();
	void OverDriveEnd();

	//TUTORIAL_TYPE GetSelectTutorial() { return m_eSelectTutorial; }
	//void SetSelectTutorial(TUTORIAL_TYPE tutorial) { m_eSelectTutorial = tutorial; }

	BaseWindow* GetWindow(BATTLE_WINDOW_TYPE type) { return m_pWindow[(int)type]; }

	void SetPause(bool flag) { m_bPause = flag; }

	bool GetTutorialFlag() { return m_bTutorialFlag; }

	// 
	tdn2DAnim* GetHideInfo() { return m_pHideInfo; };
	bool GetHideUI() { return m_bHideUI; }
	void SetHideUI(bool flag) { m_bHideUI = flag; }

	//
	int GetPauseDeviceID() { return m_iPauseDeviceID; }
	void SetPauseDeviceID(int iDeviceID) { m_iPauseDeviceID = iDeviceID; }
	
	SIDE GetCommandSide() { return m_eCommandSide; }
	void SetCommandSide(SIDE eCommand) { m_eCommandSide = eCommand; }

private:
	int m_iRoundNum;				// ���E���h��

	Stage::Base *m_pStage;			// �X�e�[�W����

	//Camera *m_pCamera;				// �J��������
	ShotManager *m_pShotMgr;		// �V���b�g�Ǘ�����
	
	tdn2DObj* m_stageScreen;		// �X�e�[�W�I�����[�̃����_�[�^�[�Q�b�g

	Vector3 m_dirLight;// ���C�g����Ʒ
	bool m_bShaderFlag;
	bool m_bBakeStageShadow;		//	�X�e�[�W�̉e���Ă����ރt���O

	StateMachine<sceneMain> *m_pStateMachine;	// �X�e�[�g�}�V��

	RoundCallManager* m_pRoundCallMgr;

	SceneSwitchPrev* prevEF;

	// �}�X�N
	tdn2DObj* m_pMaskScreen;
	bool  m_bOverDriveStageFlag;
	float m_fMaskRate;
	float m_fMaskPower;

	//���K�E�̎������ĂԂ悤�ɂ��ȃV���X
	OverDriveStage* m_pOverDriveStage;		// �K�E�p�̔w�i	

	// �`���[�g���A�����ǂ���
	bool m_bTutorialFlag;
	//TUTORIAL_TYPE m_eSelectTutorial;		// �I�����Ă�`���[�g���A��

	// �g���[�j���O���ǂ���
	bool m_bTrainingFlag;

	// �`�������W���ǂ���
	bool m_bChallengeFlag;

	// ���j���[��\���p
	tdn2DAnim* m_pHideInfo;
	bool	   m_bHideUI;

	// Window��
	BaseWindow* m_pWindow[(int)BATTLE_WINDOW_TYPE::ARRAY_END];
	bool m_bPause;

	// �|�[�Y�������l�̃f�o�C�XID
	int m_iPauseDeviceID;
	SIDE m_eCommandSide;

};