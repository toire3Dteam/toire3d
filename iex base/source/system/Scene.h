#ifndef __SCENE_H__
#define __SCENE_H__
#include	"../../IEX/iextreme.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//		���N���X�Q�V�[��
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class Scene
{
public:
	//	�����E���
	Scene(){}
	virtual ~Scene(){}
	//	������
	virtual bool Initialize(){ return true; }
	//	�X�V�E�`��
	virtual void Update(){}
	virtual void Render(){}
};

//*****************************************************************************
#endif
