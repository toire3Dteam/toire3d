
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
	//	初期化
	bool Initialize();
	//	更新・描画
	void Update();	//	更新
	void Render();	//	描画
};


