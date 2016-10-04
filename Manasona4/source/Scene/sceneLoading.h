#pragma once

class sceneLoading : public BaseScene
{
private:
	BaseScene*	m_newScene;

	//------------------------------------------------------
	//	���[�h�̉�
	//------------------------------------------------------
	int			m_AnimeCount;
	int			m_AnimeFrame;
	tdn2DObj	*m_pImage, *m_pGauge;
	float		m_fLoadGaugePercent;		// �`��p��percentage(����ԂŎg��)

public:

	//------------------------------------------------------
	//	�X���b�h�֌W
	//------------------------------------------------------
	static void Thread_funk(void *arg);
	static bool isThread;


	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	bool Initialize(BaseScene *newScene);
	~sceneLoading();


	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	void Update();
	void Render();

};
