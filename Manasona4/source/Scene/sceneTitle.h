#pragma once

class sceneTitle : public BaseScene
{
public:
	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	bool Initialize();
	~sceneTitle();


	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	void Update();
	void Render();

private:

	enum IMAGE
	{
		//LOGO,	// �^�C�g�����S
		BACK,	// �w�i
		MAX
	};

	tdn2DObj *m_pImages[IMAGE::MAX];	// �摜
	tdnMovie *m_pMovie;			// ����	
};
