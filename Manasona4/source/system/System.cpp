#include	"TDNLIB.h"
#include	"system.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

tdnShader*	shader2D;
//tdnShader*	shader;
tdnShader*	shaderM;

//*****************************************************************************************************************************
//
//*****************************************************************************************************************************
void	SYSTEM_Initialize()
{
	//	�V�F�[�_�[�ǂݍ���
	//shader = new tdnShader("DATA/Shader/3D.fx");
	shader2D = new tdnShader("DATA/SHADER/2D.cfx", true);
	shaderM = new tdnShader("DATA/SHADER/SHADER.cfx",true);
}

void	SYSTEM_Release()
{
	//delete shader;
	SAFE_DELETE(shader2D);
	SAFE_DELETE(shaderM);

}


