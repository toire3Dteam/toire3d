#include	"iextreme.h"
#include	"system/Framework.h"

#include	"OKB.h"

#include	"TitleScene.h"
#include	"ModeSelectScene.h"

// �f�o�b�O�p
//#define DEBUG_TITLE

extern Framework* MainFrame;


bool TitleScene::Initialize()
{
	//	���ݒ�
	iexLight::SetFog(800, 1000, 0);

	//	���s���ݒ�							
	Vector3	 dir(1.0f, -1.0f, 1.0f);
	iexLight::DirLight(0, &dir, 1.0f, 1.0f, 1.0f);
	//	�����ݒ�							
	iexLight::SetAmbient(0x202020);

	// �J����
	view = new iexView;

	// 2D�摜
	image[IMG_BACK] = new iex2DObj("DATA/BG/image/BG.png");

	// �e�L�X�g
	text = new Text(50, "�l�r �S�V�b�N");

	fade = new Fade;
	fade->Set(fade->FADE_IN, 5, 0, 255, 0x00000000);

	return true;
}

TitleScene::~TitleScene()
{
	// �r���[
	SAFE_DELETE(view);

	// 2D�摜
	for (int i = 0; i < IMG_MAX; i++)
		SAFE_DELETE(image[i]);

	// �e�L�X�g
	SAFE_DELETE(text);

	// �t�F�[�h
	SAFE_DELETE(fade);
}



void TitleScene::Update()
{
	fade->Update();

	for (int i = 0; i < 4; i++)
	{
		if (KEY(KEY_ENTER, i)){
			MainFrame->ChangeScene(new ModeSelectScene());
			break;
		}
	}
}

void TitleScene::Render()
{
#ifndef DEBUG_TITLE

	// 2D�摜�����p�f�o�b�O�ϐ�
	static int x = 0;
	static int y = 0;
	const int move = 10;

	if (KeyBoardTRG(KB_A))
		x -= move;

	if (KeyBoardTRG(KB_D))
		x += move;

	if (KeyBoardTRG(KB_W))
		y -= move;

	if (KeyBoardTRG(KB_S))
		y += move;

#endif


	//��ʃN���A
	view->Activate();
	view->Clear(0xff0080ff);


	// �^�C�g���摜
	image[IMG_BACK]->Render(0, 0, 1280, 720, 0, 0, 2024, 1028, RS_COPY, 0xffffffff, 100);

	// �e�L�X�g
	text->Draw(iexSystem::ScreenWidth / 2, iexSystem::ScreenHeight / 2, 0xff000000, "�^�C�g��");

	// �t�F�[�h
	fade->Render();
}