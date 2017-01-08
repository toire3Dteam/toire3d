#pragma once

// �O���錾
class MyMusicManager;
class sceneRenderingTest2 : public BaseScene
{
public:
	//�������E���
	bool Initialize();
	~sceneRenderingTest2();
	//����
	void Update();
	//�`��
	void Render();
	void RenderShadow();
	void SurfaceRender();

	// ���p�̊��}�b�v
	void WaterEnvRender();

private:
	// �ȈՃJ����
	struct Camera
	{
		Vector3 pos;
		Vector3 target;
	}m_camera;

	Vector3 m_dirLight;// ���C�g����Ʒ
	bool m_bShaderFlag;

	// ���p
	Surface*	m_pStencilSurface;	// ���s
	tdn2DObj*	m_pWaterEnvScreen;	// �Q�[�����


	// �|�C���g���C�g�z��
	std::vector<Vector3> m_aPos;


	// �Z�M
	iex3DObj* m_pAniki;

};