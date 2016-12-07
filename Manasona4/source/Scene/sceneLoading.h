#pragma once

class sceneLoading : public BaseScene
{
private:
	BaseScene		*m_pNewScene;			// �X���b�h�œǂݍ��ސV�����V�[��

	//------------------------------------------------------
	//	���[�h�̏��
	//------------------------------------------------------
	int			m_AnimeCount;
	int			m_AnimeFrame;
	tdn2DObj	*m_pImage, *m_pGauge;
	float		m_fLoadGaugePercent;		// �`��p��percentage(����ԂŎg��)
	float		m_fLoadAngle;
public:

	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	sceneLoading(BaseScene *newScene);
	bool Initialize();
	~sceneLoading();


	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	void Update();
	void Render();

};
