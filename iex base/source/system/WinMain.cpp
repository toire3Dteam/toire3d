#include	"iextreme.h"
#include	"Framework.h"
#include	"../IEX/OKB.h"
#include	"../Scene/sceneMain.h"
#include	"../Scene/CharaSelectScene.h"

// �ǉ�
#include	"../utility/sound/sound.h"


#include<crtdbg.h>

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

char*	AppTitle = "3D";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode = SCREEN720p;

Framework*	MainFrame = NULL;



//*****************************************************************************************************************************
//		�A�v���P�[�V����������
//*****************************************************************************************************************************

BOOL	InitApp( HWND hWnd )
{
	srand((unsigned int)time(NULL));

	//	IEX�V�X�e��������
	IEX_Initialize( hWnd, bFullScreen, ScreenMode );
	IEX_InitAudio();
	IEX_InitInput();

	// �e�L�X�g������
	Text::Init();

	//�ÓIDebug�֘A
	Debug::Init();

	// �L�[�{�[�h���͏�����
	OKB_Init();

	// �T�E���h������
	Init_musics();


	//	�V�X�e���̏�����
	SYSTEM_Initialize();
	//	���C���t���[�����[�N����
	MainFrame = new Framework();
	//	�����V�[���o�^
	//MainFrame->ChangeScene( new TitleScene() );
	//MainFrame->ChangeScene( new sceneMain() );
	MainFrame->ChangeScene( new CharaSelectScene() );

	return TRUE;
}

//*****************************************************************************************************************************
//		�E�B���h�E�v���V�[�W��
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_KEYDOWN:
			switch (wParam){
			case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
			case VK_F1:		iexSystem::OpenDebugWindow(); return 0;
			}
			break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//*****************************************************************************************************************************
//		�E�B���h�E�쐬
//*****************************************************************************************************************************
HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HWND			hWnd;

	//	�X�N���[���T�C�Y�擾
	RECT	WindowSize;
	iexSystem::GetScreenRect( ScreenMode, WindowSize );

	//	�E�B���h�E�N���X�ݒ�
	WNDCLASS	wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "IEX2010";
	RegisterClass(&wc);

	//	�E�B���h�E�쐬
	if( !bFullScreen ){
		AdjustWindowRect( &WindowSize, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow( "IEX2010", AppTitle, WS_OVERLAPPEDWINDOW, 
								0, 0, WindowSize.right-WindowSize.left, WindowSize.bottom-WindowSize.top,
								NULL, NULL, hInstance, NULL);
	} else {
		hWnd = CreateWindow( "IEX2010", AppTitle, WS_POPUP, 0, 0, WindowSize.right,WindowSize.bottom, NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);
	}
	if( !hWnd ) return NULL;
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// ���������[�N���m��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �������m�ۂ����u�Ԍ��m��
	//_CrtSetBreakAlloc(90);

	MSG		msg;
	HWND	hWnd;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		ScreenMode = SCREEN1080p;
		bFullScreen = TRUE;
	}

	hWnd = InitWindow(hInstance, nCmdShow);
	InitApp(hWnd);

	//	���C�����[�v
	for(;;){
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if( !GetMessage(&msg, NULL, 0, 0) ) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if( MainFrame->Update() ) MainFrame->Render();
		}
	}

	//	�S���

	//����̐擪�ŐÓIDebug�֘A���
	Debug::CleanUp();
	delete	MainFrame;
	SYSTEM_Release();
	iexSystem::CloseDebugWindow();
	IEX_ReleaseInput();
	IEX_Release();
	IEX_ReleaseAudio();
	OKB_Release();
	Text::CleanUpModule();
	Release_musics();

	return 0;
}
