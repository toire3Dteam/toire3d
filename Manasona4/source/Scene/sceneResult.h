#pragma once

#include "../BaseEntity/Message/Message.h"

// �O���錾
class MyMusicManager;
class Camera;
class BaseResultPerformance;
class SceneSwitch;
class UVEffectManager;
class EffectCamera;
class ScoreUI;
class ResultWindow;

//class Stage::Base;	// �~
namespace Stage{		// ��
	class Base;
}

class sceneResult: public BaseScene
{
public:
	//�������E���
	sceneResult(SIDE WinnerSide);
	bool Initialize();
	~sceneResult();
	//����
	void Update();
	//�`��
	void Render();
	
	// ���ꂼ��̕`���
	void BackRender();
	void CameraRender();
	void FrontRender();
	void AllRender();


private:
	// �X�e�[�W
	iexMesh* m_grand;


	// ����
	SIDE m_WinnerSide;
	iex3DObj* m_WinPlayer;
	Vector3 m_winPos;

	// �s��
	iex3DObj* m_LosePlayer;
	Vector3 m_losePos;
	int m_iLoseMotion;

	/****************************/
	// ���R�̃����_�[�^�[�Q�b�g
	tdn2DObj* m_BackScreen;
	tdn2DObj* m_CameraScreen;
	tdn2DObj* m_FrontScreen;

	tdn2DObj* m_AllScreen;

	// �`���
	Surface* m_pBackBuffer;
	/****************************/
	
	static const int PERFORMANCE_ACTION_TIME_MAX = 150;
	static const int PERFORMANCE_TIME_MAX = 150;
	//  �p�t�H�[�}���X�QD
	int m_iPerformanceActionTimer;
	int m_iPerformanceTimer;
	bool m_bPerformanceFlag;

	// �G���h
	bool m_bEndFlag;

	Vector3 m_dirLight;// ���C�g����Ʒ
	bool m_bShaderFlag;

	// 
	tdn2DObj* m_move;
	tdnMovie*  m_moveMgr;

	// ���e���L�����N�^�[�̃��U���g�̉��o
	BaseResultPerformance* m_ResultPerformance;
	enum class WINNER_PLAYER 
	{
		ARAMITAMA,AIROU
	};

	SceneSwitch* m_sceneSwitch;

	tdn2DAnim* m_winnerPic;
	float m_iWinnerPicX;

	// UV�G�t�F�N�g
	UVEffectManager* m_UVEffectMgr;

	// �E�F�[�u�X�N���[��
	tdn2DObj* m_wavePic;
	tdn2DObj* m_waveScreen;
	float m_waveTU;
	void WaveRender();

	// �X�g���[�~���O�p
	tdnStreamSound *m_pStream;

	// �J�����p
	ViewData m_ViewData;
	EffectCamera *m_pEffectCamera;

	// �X�R�A�pUI
	ScoreUI* m_pScoreUI;

	// Info
	tdn2DAnim* m_pInfoPlate;

	// ���U���g�E�B���h�E
	ResultWindow* m_pResultWindow;

	// �ȈՃX�e�[�g�}�V��
	enum STEP
	{
		INTRO, SCORE, WINDOW_SELECT, END
	};
	STEP m_eStep;

	// �������V�[�����X�ɓǂݍ��ނ̂ŁA�C�j�V�����C�Y�ŏ���������Ɠs��������(�Ⴆ�΃X�g���[�~���OBGM�Ƃ�)���Update��1�񂾂��Ăяo�������Ƃ��Ɏg��
	bool m_bFirstUpdate;
};
