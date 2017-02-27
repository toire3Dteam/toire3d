#pragma once

#include	"BaseScene.h"

//*****************************************************************************
//
//		ゲームシステム全般
//
//*****************************************************************************

//*****************************************************************************
//		グローバル変数
//*****************************************************************************
//extern tdnShader* shader;
extern tdnShader* shader2D;
extern tdnShader* shaderM;

//*****************************************************************************
//		関数
//*****************************************************************************
//------------------------------------------------------
//	システム初期化・解放
//------------------------------------------------------
void	SYSTEM_Initialize();
void	SYSTEM_Release();