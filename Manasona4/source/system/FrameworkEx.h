#pragma once
#include "TDNLIB.h"
#include "BaseScene.h"


/****************************/
//		�t���[�����[�NEx
/****************************/


class FrameworkEx
{
private:
	//	�������E���
	FrameworkEx();
	~FrameworkEx();
	// ����
	FrameworkEx(const FrameworkEx&) {}
	FrameworkEx& operator=(const FrameworkEx&) {}

public:
	//�V���O���g��
	static FrameworkEx* GetInstance();
	//  ���
	static void Release();

	//	�X�V�E�`��
	bool Update();
	void Render();

	//	�V�[���̐؂�ւ�
	void ChangeScene(BaseScene* newScene)
	{
		//	���݂̃V�[�����
		if (m_scene != NULL) delete m_scene;
		//	�V�[���̐؂�ւ���������
		m_scene = newScene;
		m_scene->Initialize();
	}

private:
	// �������g
	static FrameworkEx* m_pInstance;
	
	/**********************/
	//	�t���[�����[�g�֘A
	/**********************/
	double m_dFreq;		// ��b������̎��g��

	// 1�t���[������FPS�ɂ��邩�ɕK�v�ȕϐ�
	double m_dFlameTime;	// �t���[���̎���
	double m_dCurrentTime;	// ���݂̎���

	// �P�b�Ԃɉ�FPS���\�L���邽�߂ɕK�v�Ȃ���
	int m_iFPS;
	int m_iRenderFPS;

	int m_iWorkFPS;
	int m_iWorkRenderFPS;

	// �`��E�X�V�t���O
	bool m_bRenderFlag;
	bool m_bUpdateFlag;
	bool m_bSlowFlag;

	// FPS�̑���
	bool FPSCtrl();
	/*************/
	//	�V�[��
	/*************/
	BaseScene	*m_scene;

	/*************/
	//	����
	/*************/
	bool m_bMoviePlay; //	������Đ����Ă��邩�ǂ���
};

// �C���X�^���X
#define MainFrameEx FrameworkEx::GetInstance()
