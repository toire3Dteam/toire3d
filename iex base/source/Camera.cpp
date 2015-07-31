#include	"iextreme.h"
#include	"system/Scene.h"

#include	"../IEX/OKB.h"

#include	"camera.h"

#define DEBUG_CAMERA

Camera::Camera()
{
	view = new iexView;
	view->Set(Vector3(0, 10.0f, -120.0f), Vector3(0, 0, 0));

	mode = FIX;

	pos = ipos = target = itarget = angle = Vector3( .0f, .0f, .0f );	

	dist = 800.0f;
}

Camera::~Camera()
{
	SAFE_DELETE(view);
}

void Camera::Fix_camera()
{
	pos = Vector3(0, 150, -dist);
	target = Vector3(0, 150, 0);
}

void Camera::Debug_camera()
{
	float speed = 2.0f;
	if (KeyBoard(KB_SPACE)) speed *= 2.0f;

	//=================================
	//	カメラZ移動
	if (KeyBoard(KB_W)){
		pos.x -= sinf(angle.y) * speed;
		pos.z -= cosf(angle.y) * speed;
	}
	else if (KeyBoard(KB_S)){
		pos.x += sinf(angle.y) * speed;
		pos.z += cosf(angle.y) * speed;
	}

	//=================================
	//	カメラX移動
	if (KeyBoard(KB_D)){
		pos.x += sinf(angle.y - (PI / 2)) * speed;
		pos.z += cosf(angle.y - (PI / 2)) * speed;
	}
	else if (KeyBoard(KB_A)){
		pos.x += sinf(angle.y + (PI / 2)) * speed;
		pos.z += cosf(angle.y + (PI / 2)) * speed;
	}

	//=================================
	//	カメラY移動
	if (KeyBoard(KB_Z)) pos.y -= speed;
	else if (KeyBoard(KB_X)) pos.y += speed;




	//=================================
	//	注視点Z移動
	if (KeyBoard(KB_UP)){
		target.x -= sinf(angle.y) * speed;
		target.z -= cosf(angle.y) * speed;
	}
	else if (KeyBoard(KB_DOWN)){
		target.x += sinf(angle.y) * speed;
		target.z += cosf(angle.y) * speed;
	}

	//=================================
	//	注視点X移動
	if (KeyBoard(KB_RIGHT)){
		target.x += sinf(angle.y - (PI * .5f)) * speed;
		target.z += cosf(angle.y - (PI * .5f)) * speed;
	}
	else if (KeyBoard(KB_LEFT)){
		target.x += sinf(angle.y + (PI * .5f)) * speed;
		target.z += cosf(angle.y + (PI * .5f)) * speed;
	}

	//=================================
	//	注視点Y移動
	if (KeyBoard(KB_C)) target.y -= speed;
	else if (KeyBoard(KB_V)) target.y += speed;

	view->Set(pos, target);
}

void Camera::Update()
{

#ifndef DEBUG_CAMERA

	Debug_camera();
	return;

#endif


	switch (mode){
	case FIX:
		Fix_camera();
		break;

	}

	//カメラのぬるーり感を制御
	pos = pos * 0.94f + ipos * 0.06f;
	target = target * .95f + itarget * .05f;


	view->Set(pos, target);
}


void Camera::Render()
{
	view->Activate();
	view->Clear(0xff0080ff);

#ifndef DEBUG_CAMERA

	char	str[64];
	sprintf_s(str, sizeof(str), "DEBUG CAMERA");
	IEX_DrawText(str, 10, 500, 200, 20, 0xffffff00);
	sprintf_s(str, sizeof(str), "pos.x    : %.2f", pos.x);
	IEX_DrawText(str, 10, 520, 200, 20, 0xffffff00);
	sprintf_s(str, sizeof(str), "pos.y    : %.2f", pos.y);
	IEX_DrawText(str, 10, 540, 200, 20, 0xffffff00);
	sprintf_s(str, sizeof(str), "pos.z    : %.2f", pos.z);
	IEX_DrawText(str, 10, 560, 200, 20, 0xffffff00);
	sprintf_s(str, sizeof(str), "target.x : %.2f", target.x);
	IEX_DrawText(str, 10, 580, 200, 20, 0xffffff00);
	sprintf_s(str, sizeof(str), "target.y : %.2f", target.y);
	IEX_DrawText(str, 10, 600, 200, 20, 0xffffff00);
	sprintf_s(str, sizeof(str), "target.z : %.2f", target.z);
	IEX_DrawText(str, 10, 620, 200, 20, 0xffffff00);

#endif


}

Camera * camera = nullptr;