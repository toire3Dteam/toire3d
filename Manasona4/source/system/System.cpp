#include	"TDNLIB.h"
#include	"system.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

tdnShader*	shader2D;
tdnShader*	shader;
tdnShader*	shaderM;

//*****************************************************************************************************************************
//
//*****************************************************************************************************************************
void	SYSTEM_Initialize()
{
	//	�V�F�[�_�[�ǂݍ���
	shader = new tdnShader("DATA/Shader/3D.fx");
	shader2D = new tdnShader("DATA/SHADER/2D.fx");
	shaderM = new tdnShader("DATA/SHADER/SHADER.fx",false);
}

void	SYSTEM_Release()
{
	delete shader;
	delete shader2D;
	delete shaderM;
}


