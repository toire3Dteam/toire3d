class	sceneTitle : public Scene
{
private:
	iex2DObj* title ;	//�@�^�C�g���摜
public:
	~sceneTitle();
	//	������
	bool Initialize();
	//	�X�V�E�`��
	void Update();	//	�X�V
	void Render();	//	�`��
};
