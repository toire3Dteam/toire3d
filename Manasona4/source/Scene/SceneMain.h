#pragma once

// ������C���N���[�h�����BaseScene���ǂ�����������̂�
#include "../system/FrameworkEx.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// �O���錾
//class MyMusicManager;
class Camera;
class RoundCallManager;
class OverDriveStage;
class SceneSwitchPrev;

//class Stage::Base;	// �~
namespace Stage{		// ��
	class Base;
}

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
	
	// �X�e�[�g�}�V���擾
	StateMachine<sceneMain> *GetFSM(){ return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg){ return m_pStateMachine->HandleMessage(msg); }

	Stage::Base *GetStage() { return m_pStage;	}
	//MyMusicManager *GetMyMusicManager(){ return m_pMyMusicMgr; }

	void SetRoundNum(int roundNum) { m_iRoundNum = roundNum; }
	int GetRoundNum() { return m_iRoundNum; }

	RoundCallManager *GetRoundCall(){ return m_pRoundCallMgr; }

	SceneSwitchPrev *GetSceneSwitchPrev(){ return prevEF; }

	OverDriveStage* GetOverDriveStage() { return m_pOverDriveStage; }

	void OverDriveUpdate();
	void OverDriveAction();
	void OverDriveEnd();

private:
	int m_iRoundNum;				// ���E���h��

	Stage::Base *m_pStage;			// �X�e�[�W����

	//Camera *m_pCamera;				// �J��������
	//MyMusicManager *m_pMyMusicMgr;	// �I���ȊǗ�����
	
	tdn2DObj* m_stageScreen;		// �X�e�[�W�I�����[�̃����_�[�^�[�Q�b�g

	Vector3 m_dirLight;// ���C�g����Ʒ
	bool m_bShaderFlag;

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

	// �i���j�`���[�g���A�����ǂ���
	bool m_bTutorialFlag;

};