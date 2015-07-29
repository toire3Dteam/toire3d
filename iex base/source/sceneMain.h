
#include "system/Scene.h"

class iexView;
class TEST_airou;
class iexMesh;
class Collision;

class	sceneMain : public Scene
{
private:
	iexView *view = nullptr;
	TEST_airou *airou = nullptr;
	iexMesh *lpStage = nullptr;
	Collision *collision = nullptr;

	void Camera_update();
public:
	~sceneMain();
	//	������
	bool Initialize();
	//	�X�V�E�`��
	void Update();	//	�X�V
	void Render();	//	�`��
};


