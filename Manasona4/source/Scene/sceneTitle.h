#pragma once

// ������C���N���[�h�����BaseScene���ǂ�����������̂�
#include "../system/FrameworkEx.h"

// �G���e�B�e�B�֘A�̃C���N���[�h
#include "../BaseEntity/Entity/BaseGameEntity.h"
#include "../BaseEntity/State/StateMachine.h"

// �e���v���[�g�N���X���t�����h�N���X�ň������߂ɂ͐�ɒ�`����K�v������
#include "sceneTitleState.h"

class sceneTitle : public BaseScene, BaseGameEntity
{
public:
	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	sceneTitle();
	bool Initialize();
	~sceneTitle();


	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	void Update();
	void Render();

	// �X�e�[�g�}�V���擾
	StateMachine<sceneTitle> *GetFSM() { return m_pStateMachine; }

	// ���b�Z�[�W��M(BaseEntity�ɂ��I�[�o�[���C�h)
	bool  HandleMessage(const Message& msg) { return m_pStateMachine->HandleMessage(msg); }


private:

	StateMachine<sceneTitle> *m_pStateMachine;	// �X�e�[�g�}�V��

	enum IMAGE
	{
		//LOGO,	// �^�C�g�����S
		MOVIE,	// ���[�r�[
		BG,		// �w�i
		TITLE,	// �^�C�g��
		CLOUD1, // �_1
		CLOUD2, // �_2
		MAX
	};

	tdn2DObj *m_pImages[IMAGE::MAX];	// �摜
	tdnMovie *m_pMovie;			// ����	

	// �w�i�p
	tdnMovie* m_pMovieBGLine;
	tdn2DObj* m_pBGLine;


	// ���S
	tdn2DAnim* m_pLogo; 
	
	// �v���X�G�j�[�{�^����
	tdn2DAnim* m_pPreaseAnyButton;

	// �_�̃X�N���[������
	float m_fCloudU;

	// �Q�[���^�C�}�[
	int m_iGameTimer;

	// �X�e�[�g���t�����h�N���X��
	friend class SceneTitleState::Intro;
	friend class SceneTitleState::LogoStep;
	friend class SceneTitleState::MovieStep;
	friend class SceneTitleState::TitleStep;
	friend class SceneTitleState::End;
	friend class SceneTitleState::BackMenu;
};
