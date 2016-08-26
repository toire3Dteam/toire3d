#pragma once

// �O���錾
class MyMusicManager;
class Camera;
//class Stage::Base;	// �~
namespace Stage{		// ��
	class Base;
}

class sceneMain	: public BaseScene
{
public:
	//�������E���
	bool Initialize();
	~sceneMain();
	//����
	bool Update();
	//�`��
	void RenderStage();
	void Render();
	void RenderShadow();
	void SurfaceRender();

private:

	Stage::Base *m_pStage;			// �X�e�[�W����
	Camera *m_pCamera;				// �J��������
	MyMusicManager *m_pMyMusicMgr;	// �I���ȊǗ�����
	
	tdn2DObj* m_stageScreen;		// �X�e�[�W�I�����[�̃����_�[�^�[�Q�b�g

	Vector3 m_dirLight;// ���C�g����Ʒ
	bool m_bShaderFlag;
};