#pragma once

// �O���錾
class MyMusicManager;
class sceneRenderingTest : public BaseScene
{
public:
	//�������E���
	bool Initialize();
	~sceneRenderingTest();
	//����
	void Update();
	//�`��
	void Render();
	void RenderShadow();
	void SurfaceRender();

private:
	// �ȈՃJ����
	struct Camera
	{
		Vector3 pos;
		Vector3 target;
	}m_camera;

	Vector3 m_dirLight;// ���C�g����Ʒ
	bool m_bShaderFlag;
};